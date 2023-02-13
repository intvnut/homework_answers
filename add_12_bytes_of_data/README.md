# ASM: Add 12 Bytes of Data Using 3 Types of Registers

This was an oddly specific request, probably derived from something covered
in class.  In particular, the fact that it asked about different types of
registers implies this is about assembly language.

While I've put the assembly code functions into separate files, I've also
reproduced the entire post below the cut.

Original: [What is a program to add 12 bytes of data using 3 types of registers?](https://www.quora.com/What-is-a-program-to-add-12-bytes-of-data-using-3-types-of-registers/answer/Joe-Zbiciak)

---

What is it? Woefully underspecified, I'd say. You don't give a language, a
processor, the precision of the result, or even where we can find these 12
bytes or where we should place the result.

In particular, what do you mean by “using 3 types of registers”? That implies
assembly language, probably. But, for which CPU?

Let's fill in the blanks. Since _you_ didn't specify, _I'll_ pick the details.
_Dealer's choice!_

![Dealer's Choice](../Images/dealers_choice.png)

Here's what I propose:
  
* Language and processor: TMS320C62x assembly.
* Input data is unsigned 8-bit bytes.
* Result precision: 32 bits.
* Input data is stored in a buffer whose address is initialized in `A4`.
This corresponds to the first argument of a C callable function.
* Input data starts at a 32-bit boundary.
* The final result should be delivered in `A4`, which corresponds to the
return value from a C function for return types up to 32 bits.
* The program should otherwise adhere to the C calling conventions, and `RET`
to `B3`, so we can call it from C code. (I'm happy to share what those are if
you like. I still have them memorized after all these years.)
  
So, without further ado, here's my hand-coded VLIW assembly for that. Note: to
get to _three_ kinds of registers, I counted a register pair as a distinct
register type. Otherwise, we really just have the A registers and B registers
for arithmetic and pointers.

```
; C callable as int add12bytes(uint8_t*); 
; Pointer must be word aligned. 
   .global add12bytes 
add12bytes: 
   MV   .L2X   A4,     B4    ; twin pointer 
|| LDW  .D1T1  *A4[0], A0    ; first 4 bytes 
 
   LDW  .D1T1  *A4[1], A2    ; next 4 bytes 
|| LDW  .D2T2  *B4[2], B0    ; last 4 bytes 
 
   MVKL .S1    0x00FF, A5    ; even byte mask lo 
|| MVKL .S2,   0xFF00, B6    ; odd byte mask lo 
 
   MVKLH.S1    0x00FF, A5    ; even byte mask hi 
|| MVKLH.S2,   0xFF00, B6    ; odd byte mask hi 
 
   MV   .S2X   A5,     B5    ; twin mask 
|| MV   .S1X   B6,     A6    ; twin mask 
 
   AND  .L1    A0, A5, A0    ; bytes 0, 2 
|| AND  .S1    A0, A6, A1    ; bytes 1, 3 
 
   AND  .L1    A2, A5, A2    ; bytes 4, 6 
|| AND  .S1    A2, A6, A3    ; bytes 5, 7 
|| AND  .L2    B0, B5, B0    ; bytes 8, 10 
|| AND  .S2    B0, B6, B1    ; bytes 9, 11 
 
   ADDU .L1X   A1, B1, A5:A4 ; add 1,3 to 9,11 
|| ADD  .L2X   B0, A0, B0    ; add 0,2 to 8,10 
|| RET  .S2    B3            ; start returning 
 
   ADDU .L1    A3,A5:A4,A5:A4; all odds summed 
|| ADD  .L2X   B0, A2, B0    ; all evens summed 
 
   SHRU .S1    A5:A4,8,A5:A4 ; Align odds to evens 
 
   ADD  .L1X   A4, B0, A4    ; add odds to evens 
 
   SHRU .S2X   A4, 16, B4    ; move upper sum down 
|| EXTU .S1    A4,16,16,A4   ; zero extend lower sum 
 
   ADD  .L1X   A4, B4, A4    ; final sum  
; ==== return occurs here ==== 
```

This uses a SIMD approach to add all 12 bytes. Because the values are small
enough, I can use a 32 bit ADD to add two partial sums. For the odd bytes, it
uses a 40-bit register pair (`A5:A4`) to defer realigning the odd sums until
the end.  

The `RET` instruction has 5 exposed delay slots, and `LDW` has 4. If I counted
correctly, this code takes 13 cycles.  

SIMD wasn’t a win here. It was at best break-even. A larger input would show
more benefit. I _may_ be able to shave a cycle by balancing the addition
differently; however, a bottleneck on the `S2` unit would keep me from issuing
`RET` earlier.  

If I had just used byte loads, the critical path seems to be loading the last
byte on cycle 7 (two per cycle, starting on 2nd cycle), 4 delay slots, and two
serialized adds. That also comes to 13 cycles. The non-SIMD version _also_ has
no alignment restrictions.  

```
; C callable as int add12bytes(uint8_t*);
; Pointer may have any alignment.
    .global add12bytes
add12bytes:
    ADD    .L2X   A4, 6,  B4  ; twin pointer; offset 6
||  LDBU   .D1T1  *A4++,  A0  ; byte 0

    LDBU   .D1T1  *A4++,  A1  ; byte 1
||  LDBU   .D2T2  *B4++,  B1  ; byte 6

    LDBU   .D1T1  *A4++,  A2  ; byte 2
||  LDBU   .D2T2  *B4++,  B2  ; byte 7

    LDBU   .D1T1  *A4++,  A3  ; byte 3
||  LDBU   .D2T2  *B4++,  B0  ; byte 8

    LDBU   .D1T1  *A4++,  A5  ; byte 4
||  LDBU   .D2T2  *B4++,  B5  ; byte 9

    LDBU   .D1T1  *A4,    A4  ; byte 5
||  LDBU   .D2T2  *B4++,  B6  ; byte 10

    LDBU   .D1T1  *B4,    B4  ; byte 11
||  ADD    .L1    A0, A1, A1  ; sum 0, 1

    ADD    .L1    A1, A2, A1  ; sum 0..2
||  ADD    .L2    B1, B2, B1  ; sum 6, 7
||  RET    .S2    B3

    ADD    .L1    A1, A3, A1  ; sum 0..3
||  ADD    .L2    B1, B0, B1  ; sum 6..8

    ADD    .L1    A1, A5, A1  ; sum 0..4
||  ADD    .L2    B1, B5, B1  ; sum 6..9

    ADD    .L1    A1, A4, A4  ; sum 0..5
||  ADD    .L2    B1, B6, B1  ; sum 6..10

    ADD    .L2    B1, B4, B4  ; sum 6..11
    ADD    .L1X   A4, B4, A4  ; sum 0..11
; ==== return occurs here ====
```

Ah well… at least I used three types of register in my SIMD version, assuming 
you count a register pair as a distinct type. C6x also has control registers 
and predicates, and neither was needed in this function.  

If this were on TMS320C641x, I could use `LDNDW`, `LDNW`, and `DOTPU4` to write
much simpler code. There, the critical path would be `LDNW` on cycle 2, 4 delay
slots, `DOTPU4`, 3 delay slots and two serialized `ADD`s: 12 cycles.  

```
; C callable as int add12bytes(uint8_t*);
; Pointer may have any alignment.
   .global add12bytes
add12bytes:
    LDNDW .D1T1 *A4(0), A5:A4 ; Load bytes 0..7
    LDNW  .D1T2 *A4(8), B4    ; Load bytes 8..11
    MVKL  .S1   0x0101, A0    ; \_ 0x01010101
    MVKLH .S1   0x0101, A0    ; / for dot product
    NOP         1
    DOTPU4 .M1  A0, A4, A4    ; Sum bytes 0..3

    DOTPU4 .M1  A0, A5, A5    ; Sum bytes 4..7
||  DOTPU4 .M2X A0, B4, B4    ; Sum bytes 8..11
||  RETNOP .S2  B3, 3         ; RET + 3 NOP cycles

    ADD    .L1  A4, A5, A4
    ADD    .L1X A4, B4, A4    ; final sum of all 12
; ==== return occurs here ====
```

I don't see how to make it faster. Deep pipelines want bigger datasets!  

In any case, I doubt this is what your teacher had in mind. It is, however, a
valid way to attack this problem. Now do your homework.  

---

_Note: I spent ~20 years working with that DSP family, but I haven’t
written any real code on it since I left that position 5 years ago.
The last real hard core C62x/C64x assembly I wrote as longer-ago
still. I’m going completely on memory here. Also, I typed 90% of
that on my phone. Apologies in advance for any errors._  

_As for vertical spacing, I followed our compiler’s conventions:
There’s always at least one blank line before and after a parallel
execute packet (and its label, if present). Execute packets with
exactly no parallel instructions can abut other non-parallel execute
packets. NOP followed by a number means “NOP for this many cycles.”
In this case, my only standalone NOP is NOP 1. C64x has “RETNOP”
which allows fusing a multicycle NOP into the RET opcode._


____

Copyright © 2023, Joe Zbiciak <joe.zbiciak@leftturnonly.info>  
`SPDX-License-Identifier:  CC-BY-SA-4.0`

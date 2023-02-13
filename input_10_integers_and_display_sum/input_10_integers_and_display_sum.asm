;; ======================================================================== ;;
;;  Accept 10 decimal integers from the keypad and store in an array.       ;;
;;  Then, sum the integers and display the result.                          ;;
;;  Author:  Joseph Zbiciak, 16-Dec-2018                                    ;;
;;                                                                          ;;
;; ------------------------------------------------------------------------ ;;
;;                                                                          ;;
;;     The routines and data in this program are dedicated to the public    ;;
;;     domain via the Creative Commons CC0 v1.0 license by its author,      ;;
;;     Joseph Zbiciak.                                                      ;;
;;                                                                          ;;
;;          https://creativecommons.org/publicdomain/zero/1.0/              ;;
;; ======================================================================== ;;

        ROMW    16              ; Use 16-bit ROM width

;; ======================================================================== ;;
;;  Color values for color-stack mode.                                      ;;
;; ======================================================================== ;;
C_BLK   QEQU    $0              ; Black
C_BLU   QEQU    $1              ; Blue
C_RED   QEQU    $2              ; Red
C_TAN   QEQU    $3              ; Tan
C_DGR   QEQU    $4              ; Dark Green
C_GRN   QEQU    $5              ; Green
C_YEL   QEQU    $6              ; Yellow
C_WHT   QEQU    $7              ; White
C_GRY   QEQU    $8              ; Grey
C_CYN   QEQU    $9              ; Cyan
C_ORG   QEQU    $A              ; Orange
C_BRN   QEQU    $B              ; Brown
C_PNK   QEQU    $C              ; Pink
C_LBL   QEQU    $D              ; Light Blue
C_YGR   QEQU    $E              ; Yellow-Green
C_PUR   QEQU    $F              ; Purple

;; ======================================================================== ;;
;;  Memory layout.                                                          ;;
;; ======================================================================== ;;
STACK:  EQU     $2F0            ; Start of 16-bit RAM (32 words)
ARRAY:  EQU     $310            ; Place our array after stack

;; ======================================================================== ;;
;;  EXEC-friendly ROM header.                                               ;;
;; ======================================================================== ;;
        ORG     $5000           ; Location of the ROM header
ROMHDR: BIDECLE ZERO            ; MOB picture base   (points to NULL list)
        BIDECLE ZERO            ; Process table      (points to NULL list)
        BIDECLE MAIN            ; Program start address
        BIDECLE ZERO            ; Bkgnd picture base (points to NULL list)
        BIDECLE ONES            ; GRAM pictures      (points to NULL list)
        BIDECLE TITLE           ; Cartridge title/date
        DECLE   $03C0           ; No ECS title, run code after title,
                                ; ... no clicks
ZERO:   DECLE   $0000           ; Screen border control
        DECLE   $0000           ; 0 = color stack, 1 = f/b mode
ONES:   DECLE   C_BLU, C_BLU    ; Initial color stack 0 and 1: Blue
        DECLE   C_BLU, C_BLU    ; Initial color stack 2 and 3: Blue
        DECLE   C_BLU           ; Initial border color: Blue

;; ======================================================================== ;;
;;  ISR  -- Enable the screen and set color stack mode w/blue background.   ;;
;;          We have to set it ourselves, despite requesting blue in the     ;;
;;          ROM header, as we hijack the EXEC initialization sequence.      ;;
;; ======================================================================== ;;
ISR:    PROC
        MVI     $21,    R0      ; Set color stack mode
        MVO     R0,     $20     ; Enable display
        MVII    #C_BLU, R0      ; Blue color
        MVO     R0,     $28     ; Color stack #0
        MVO     R0,     $2C     ; Border color
        JR      R5              ; Return from interrupt
        ENDP

;; ======================================================================== ;;
;;  RD_KEY   -- Get a key from the keypad, and return the digit value.      ;;
;;              Also, 10 = [Clear], 11 = [Enter].                           ;;
;;              Digit value in R0.  R1 - R4 preserved.                      ;;
;; ======================================================================== ;;
RD_KEY: PROC
        PSHR    R5              ; Save return address.
        PSHR    R1              ; Save R1.

        ; Wait until the controllers are idle.
@@wait_idle:
        MVI     $1FE,   R1      ; \_ Read and merge both controllers.
        XOR     $1FF,   R1      ; /
        BNEQ    @@wait_idle     ; Idle = $00.

@@read_again:
        ; Read both controllers, waiting for a stable value (debounce).
        CLRR    R5              ; "Previous stable value" is the "idle" value.
@@read_loop:
        MVI     $1FE,   R1      ; \_ Read and merge both controllers.
        XOR     $1FF,   R1      ; /
        BEQ     @@read_loop     ; Loop until not idle.

        CMPR    R1,     R5      ; Same as previous value?
        BEQ     @@same_val      ; Yes: Do not restart the debounce timer.
        MVII    #$100,  R0      ; Restart the debounce timer.
        MOVR    R1,     R5      ; Remember the new value.
@@same_val:

        DECR    R0              ; Decrement debounce timer.
        BNEQ    @@read_loop     ; Loop until it expires.

        ; Now look up the encoded key value to see if it's a valid keypad
        ; keypress.
        MVII    #@@keys,R5      ; Table of encoded keypad bit patterns.
        MVII    #11,    R0      ; Total table size, minus 1.
@@key_loop:
        CMP@    R5,     R1      ; Does it match?
        BEQ     @@found_it      ; Yes:  Break out.
        DECR    R0              ; \_ Loop over all 12 keys encodings.
        BPL     @@key_loop      ; /
        B       @@read_again    ; It wasn't what we wanted, so try again.

@@found_it:
        PULR    R1              ; Restore R1.
        PULR    PC              ; Return

        ; The key table is ordered in reverse order, so that the loop counter
        ; in R0 also matches the return value.  Slick, eh?
        ;        E    C    9    8    7    6    5    4    3    2    1    0
@@keys: BYTE    $28, $88, $24, $44, $84, $22, $42, $82, $21, $41, $81, $48
        ENDP

;; ======================================================================== ;;
;;  RD_NUM   -- Get a 5-digit unsigned integer in the range 0 - 65535.      ;;
;;              Display the input on screen @R4.                            ;;
;;              [Clear] resets the input, [Enter] accepts.                  ;;
;;              R0 contains final value; all other registers preserved.     ;;
;; ======================================================================== ;;
RD_NUM: PROC
        PSHR    R5
        PSHR    R1
        PSHR    R2
        NOP                     ; Needed for interruptibility. (Display glitch.)
        PSHR    R3
        PSHR    R4

@@reset:
        CLRR    R1              ; Use R1 as our input value accumulator.

@@loop:
        PULR    R4              ; \_ Restore display position.
        PSHR    R4              ; /
        MVII    #5,     R2      ; 5 digit field.
        MVII    #C_WHT, R3      ; Display in white.
        MOVR    R1,     R0      ; Display our accumulated value.
        CALL    PRNUM16.b       ; Display with leading blanks.

        ; Get the next digit from the user and merge it into the accumulator.
        ; If the value would overflow, drop the input digit.  Do not allow
        ; inputs larger than 65,535.
        CALL    RD_KEY          ; Get a digit from the keypad.

        CMPI    #10,    R0      ; \_ Is it actually [Clear]?  If so, reset.
        BEQ     @@reset         ; /

        CMPI    #11,    R0      ; \_ Is it actually [Enter]?  If so, return
        BEQ     @@done          ; /  the number input by the user.

        ; Make a copy of the accumulator to work on, so we can back out if
        ; any step of the computation overflows.
        MOVR    R1,     R2

        SLLC    R2,     1       ; Multiply by 2.
        BC      @@loop          ; If this overflowed (C = 1), abort.

        MOVR    R2,     R3      ; Remember "value * 2".

        SLLC    R2,     2       ; Multiply by 4 (so orig value * 8).
        BC      @@loop          ; \_ If this overflowed (C = 1 or OV = 1),
        BOV     @@loop          ; /  then abort.

        ADDR    R2,     R3      ; Add x*2 + x*8 to get x*10.
        BC      @@loop          ; If this overflowed, then abort.

        ADDR    R0,     R3      ; Now add in the new digit.
        BC      @@loop          ; If this overflowed, then abort.

        MOVR    R3,     R1      ; Our new accumulated value.  Yay!
        B       @@loop

@@done:
        MOVR    R1,     R0      ; Put accumulated value into R0.
        PULR    R4              ; \
        PULR    R3              ;  |_ Restore R1 through R4
        PULR    R2              ;  |
        PULR    R1              ; /
        PULR    PC              ; Return.
        ENDP

;; ======================================================================== ;;
;;  PRNUM32.b     -- Print an unsigned 32-bit number with leading blanks.   ;;
;;                                                                          ;;
;;  AUTHOR                                                                  ;;
;;      Joseph Zbiciak  <intvnut AT gmail DOT com>                          ;;
;;                                                                          ;;
;;  INPUTS for all variants                                                 ;;
;;      R0  Lower 16 bits of number to print                                ;;
;;      R1  Upper 16 bits of number to print                                ;;
;;      R2  Width of field.  Must be >= 5.  Ignored by PRNUM32.l.           ;;
;;      R3  Format word, added to digits to set the color, etc.             ;;
;;          Note:  Bit 15 is ignored and manipulated internally.            ;;
;;      R4  Pointer to location on screen to print number                   ;;
;;      R5  Return address                                                  ;;
;;                                                                          ;;
;;  OUTPUTS                                                                 ;;
;;      R0  Zeroed                                                          ;;
;;      R1  Trashed                                                         ;;
;;      R2  Unmodified                                                      ;;
;;      R3  Unmodified, except for bit 15, which may be set.                ;;
;;      R4  Points to first character after field.                          ;;
;;      R5  Trashed                                                         ;;
;;                                                                          ;;
;;  DESCRIPTION                                                             ;;
;;      These routines print unsigned 32-bit numbers in a field 5 to 10     ;;
;;      positions wide.  The number is printed either in left-justified     ;;
;;      or right-justified format.  Right-justified numbers are padded      ;;
;;      with leading blanks or leading zeros.  Left-justified numbers       ;;
;;      are not padded on the right.                                        ;;
;;                                                                          ;;
;;      See PRNUM16 for a table illustrating output formats.                ;;
;;                                                                          ;;
;;  TECHNIQUES                                                              ;;
;;      This routine uses a 32-by-16 integer divide to cut the 32-bit       ;;
;;      number into two 5-digit segments.  It divides the initial number    ;;
;;      by 100000.  The quotient provides the upper half, and the           ;;
;;      remainder provides the lower half.                                  ;;
;;                                                                          ;;
;;      Both halves are printed by reusing the PRNUM16 routines.  The       ;;
;;      lower half is always printed using PRNUM16.z.  The upper half is    ;;
;;      printed using one of PRNUM16.l, PRNUM16.b, or PRNUM16.z, based on   ;;
;;      which entry point the caller used for invoking PRNUM32.             ;;
;;                                                                          ;;
;;      The number in the lower half can be in the range 00000...99999.     ;;
;;      Some of this range is outside the range of a 16-bit value.  We      ;;
;;      handle this by passing a special flag to PRNUM16 in bit 15 of       ;;
;;      the format word.  When set, PRNUM16 will add '5' to the leading     ;;
;;      digit of the number.  When clear, it leaves it unmodified.          ;;
;;      Thus, when our lower half is in the range 65536...99999, we cope    ;;
;;      by adding 15536 to the lower 16 bits and setting the "add 5" flag.  ;;
;;      This works, because the lower 16 bits are in the range 0 - 34463.   ;;
;;      After the 'tweak', they're in the range 15536 - 49999.              ;;
;;                                                                          ;;
;;  NOTES                                                                   ;;
;;      The left-justified variant ignores the field width.                 ;;
;;                                                                          ;;
;;      This code is fully reentrant, although it has a significant stack   ;;
;;      footprint.                                                          ;;
;;                                                                          ;;
;;      This code does not handle numbers which are too large to be         ;;
;;      displayed in the provided field.  If the number is too large,       ;;
;;      non-digit characters will be displayed in the initial digit         ;;
;;      position.  Also, the run time of this routine may get excessively   ;;
;;      large, depending on the magnitude of the overflow.                  ;;
;;      This code requires PRNUM16.  Since PRNUM16 requires a minor tweak   ;;
;;      to support PRNUM32, it must either be included after PRNUM32, or    ;;
;;      assembled with the symbol "_WITH_PRNUM32" set.                      ;;
;;                                                                          ;;
; ======================================================================== ;;
PRNUM32 PROC
        ;; ---------------------------------------------------------------- ;;
        ;;  PRNUM32.b:  Print unsigned with leading blanks.                 ;;
        ;; ---------------------------------------------------------------- ;;
@@b:    ANDI    #$7FFF,     R3  ; force bit 15 of R3 clear.
        TSTR    R1
        BEQ     PRNUM16.b
        PSHR    R5
        MVII    #PRNUM16.b, R5

        PSHR    R2              ; save our field width
        PSHR    R3              ; save our format word

        ;; ---------------------------------------------------------------- ;;
        ;;  Divide the 32-bit number by 100000, and remember the remainder. ;;
        ;;  This will give us two 5-digit numbers for the two halves.       ;;
        ;;  The upper half will always be in the range 0 to 42949, and so   ;;
        ;;  we can print it with one of the PRNUM16 routines.  The lower    ;;
        ;;  half is in the range 0...99999, so we have to do some fancy     ;;
        ;;  footwork in the case that it's above 65535.                     ;;
        ;;                                                                  ;;
        ;;  The loop below looks like it divides by 50000, but really we've ;;
        ;;  aligned the divisor within a 16-bit number.                     ;;
        ;; ---------------------------------------------------------------- ;;

        MVII    #50000, R2      ; 100000, right shifted by 1.
        MVII    #1,     R3      ; Divide is done when this bit falls out of R3
        B       @@div1          ; jump to the first divide step.

@@div:  RLC     R0,     1       ;\__ 32 bit left shift of numerator.
        RLC     R1,     1       ;/
        BC      @@divf          ; last shift overflowed, force a subtract

@@div1: CMPR    R2,     R1      ; can we subtract from upper half?
        BNC     @@divn          ; nope, then don't subtract.

@@divf: SUBR    R2,     R1      ; subtract upper half.
        SETC                    ; usu. the SUBR sets carry, except when forced

@@divn: RLC     R3,     1       ; left-shift the quotient, including new bit
        BNC     @@div           ; loop until quotient is full.


        ;; ---------------------------------------------------------------- ;;
        ;;  At this point, R1:R0 is our remainder, and R3 is our quotient.  ;;
        ;;  The remainder occupies the upper 17 bits of the R1:R0 pair.     ;;
        ;;  For now, hide this on the stack.  We do this dance carefully    ;;
        ;;  since we need to restore two items from the stack as well,      ;;
        ;;  and we don't want to lose the values in R4 or R5 either.        ;;
        ;;                                                                  ;;
        ;;  If there was no "upper half" (eg. our value is 65536...99999)   ;;
        ;;  go handle that case specially.  Urgl.                           ;;
        ;; ---------------------------------------------------------------- ;;

        SLLC    R0,     1       ; put 17th bit into carry
        MOVR    R3,     R0      ; save quotient into R0
        PULR    R3              ; recall our format word into R3
        PULR    R2              ; recall our field width

        RLC     R1,     1       ; R1 is bits 15 downto 0 of remainder
        SLL     R3,     1       ; \_ Force bit 15 of format word to 1 or 0.
        RRC     R3,     1       ; /  This will add 5 to leading digit.
        TSTR    R3              ;
        BPL     @@lt64k         ; if there was no carry, rmdr is 0...65535
        ADDI    #15536, R1      ; add "15536" to rest of digits
@@lt64k:
        TSTR    R0
        BEQ     @@notop         ; convert bottom into top if top = 0.

        PSHR    R2              ; save field width on stack
        PSHR    R3              ; save format word for bottom half
        PSHR    R1              ; save quotient for bottom half

        ;; ---------------------------------------------------------------- ;;
        ;;  Go ahead and print the first 5 digits of the result.            ;;
        ;; ---------------------------------------------------------------- ;;
        SUBI    #5,     R2      ; bottom accounts for 5 digits of field width
        ANDI    #$7FFF, R3      ; force bit 15 of R3 clear.

        MOVR    R5,     R1      ; R5 contains branch target.
        MVII    #@@ret, R5      ; remember return address.
        MOVR    R1,     PC      ; call one of PRNUM16.z, .b, or .l as needed
@@ret:

        ;; ---------------------------------------------------------------- ;;
        ;;  Print the bottom half of the number.   We branch into the       ;;
        ;;  heart of PRNUM16, which effectively always leads with zeros.    ;;
        ;; ---------------------------------------------------------------- ;;
        MVII    #4,     R2      ; bottom is always exactly 5 digits.
        PULR    R0              ; get bits 15 downto 0 of remainder
        PULR    R3              ; get format word.
        INCR    SP              ; save garbage in R1 slot on stack
        MVII    #_PW10, R1      ; need to set up power-of-10 pointer first
        B       PRNUM16.dig1    ; chain return via PRNUM16.digit

        ;; ---------------------------------------------------------------- ;;
        ;;  If there was no "upper half", convert the "lower half" into an  ;;
        ;;  upper half.  This actually isn't too gross.                     ;;
        ;; ---------------------------------------------------------------- ;;
@@notop:
        MOVR    R1,     R0      ; convert bottom half to top half
        MOVR    R5,     R1      ; \
        PULR    R5              ;  |__ chain return via target PRNUM16 routine
        JR      R1              ; /

        ENDP

;; ======================================================================== ;;
;;  _PW10                                                                   ;;
;;      Lookup table holding the first 5 powers of 10 (1 thru 10000) as     ;;
;;      16-bit numbers.                                                     ;;
;; ======================================================================== ;;
_PW10   PROC    ; 0 thru 10000
        DECLE   10000, 1000, 100, 10, 1, 0
        ENDP

;; ======================================================================== ;;
;;  PRNUM16.b     -- Print an unsigned 16-bit number with leading blanks.   ;;
;;                                                                          ;;
;;  AUTHOR                                                                  ;;
;;      Joseph Zbiciak  <intvnut AT gmail DOT com>                          ;;
;;                                                                          ;;
;;  INPUTS for all variants                                                 ;;
;;      R0  Number to print.                                                ;;
;;      R2  Width of field.  Ignored by PRNUM16.l.                          ;;
;;      R3  Format word, added to digits to set the color.                  ;;
;;          Note:  Bit 15 MUST be cleared when building with PRNUM32.       ;;
;;      R4  Pointer to location on screen to print number                   ;;
;;                                                                          ;;
;;  OUTPUTS                                                                 ;;
;;      R0  Zeroed                                                          ;;
;;      R1  Unmodified                                                      ;;
;;      R2  Unmodified                                                      ;;
;;      R3  Unmodified                                                      ;;
;;      R4  Points to first character after field.                          ;;
;;                                                                          ;;
;;  DESCRIPTION                                                             ;;
;;      These routines print unsigned 16-bit numbers in a field up to 5     ;;
;;      positions wide.  The number is printed either in left-justified     ;;
;;      or right-justified format.  Right-justified numbers are padded      ;;
;;      with leading blanks or leading zeros.  Left-justified numbers       ;;
;;      are not padded on the right.                                        ;;
;;                                                                          ;;
;;      This code handles fields wider than 5 characters, padding with      ;;
;;      zeros or blanks as necessary.                                       ;;
;;                                                                          ;;
;;              Routine      Value(hex)     Field        Output             ;;
;;              ----------   ----------   ----------   ----------           ;;
;;              PRNUM16.l      $0045         n/a        "69"                ;;
;;              PRNUM16.b      $0045          4         "  69"              ;;
;;              PRNUM16.b      $0045          6         "    69"            ;;
;;              PRNUM16.z      $0045          4         "0069"              ;;
;;              PRNUM16.z      $0045          6         "000069"            ;;
;;                                                                          ;;
;;  TECHNIQUES                                                              ;;
;;      This routine uses repeated subtraction to divide the number         ;;
;;      to display by various powers of 10.  This is cheaper than a         ;;
;;      full divide, at least when the input number is large.  It's         ;;
;;      also easier to get right.  :-)                                      ;;
;;                                                                          ;;
;;      The printing routine first pads out fields wider than 5 spaces      ;;
;;      with zeros or blanks as requested.  It then scans the power-of-10   ;;
;;      table looking for the first power of 10 that is <= the number to    ;;
;;      display.  While scanning for this power of 10, it outputs leading   ;;
;;      blanks or zeros, if requested.  This eliminates "leading digit"     ;;
;;      logic from the main digit loop.                                     ;;
;;                                                                          ;;
;;      Once in the main digit loop, we discover the value of each digit    ;;
;;      by repeated subtraction.  We build up our digit value while         ;;
;;      subtracting the power-of-10 repeatedly.  We iterate until we go     ;;
;;      a step too far, and then we add back on power-of-10 to restore      ;;
;;      the remainder.                                                      ;;
;;                                                                          ;;
;;  NOTES                                                                   ;;
;;      The left-justified variant ignores field width.                     ;;
;;                                                                          ;;
;;      The code is fully reentrant.                                        ;;
;;                                                                          ;;
;;      This code does not handle numbers which are too large to be         ;;
;;      displayed in the provided field.  If the number is too large,       ;;
;;      non-digit characters will be displayed in the initial digit         ;;
;;      position.  Also, the run time of this routine may get excessively   ;;
;;      large, depending on the magnitude of the overflow.                  ;;
;;                                                                          ;;
;;      When using with PRNUM32, one must either include PRNUM32 before     ;;
;;      this function, or define the symbol _WITH_PRNUM32.  PRNUM32         ;;
;;      needs a tiny bit of support from PRNUM16 to handle numbers in       ;;
;;      the range 65536...99999 correctly.                                  ;;
;; ======================================================================== ;;

PRNUM16 PROC
        ;; ---------------------------------------------------------------- ;;
        ;;  PRNUM16.b:  Print unsigned with leading blanks.                 ;;
        ;; ---------------------------------------------------------------- ;;
@@b:    PSHR    R5
@@b1:   CLRR    R5              ; let the blank loop do its thing
@@z1:   PSHR    R2
@@z2:   PSHR    R1

        ;; ---------------------------------------------------------------- ;;
        ;;  Find the initial power of 10 to use for display.                ;;
        ;;  Note:  For fields wider than 5, fill the extra spots above 5    ;;
        ;;  with blanks or zeros as needed.                                 ;;
        ;; ---------------------------------------------------------------- ;;
        MVII    #_PW10+5,R1     ; Point to end of power-of-10 table
        SUBR    R2,     R1      ; Subtract the field width to get right power
        PSHR    R3              ; save format word

        CMPI    #2,     R5      ; are we leading with zeros?
        BNC     @@lblnk         ; no:  then do the loop w/ blanks

        ADDI    #$80,   R3      ; yes: do the loop with zeros
        B       @@lblnk


@@llp   MVO@    R3,     R4      ; print a blank/zero

        INCR    R1              ; get next power of 10
@@lblnk DECR    R2              ; decrement available digits
        BEQ     @@ldone
        CMPI    #5,     R2      ; field too wide?
        BGE     @@llp           ; just force blanks/zeros 'till we're narrower.
        CMP@    R1,     R0      ; Is this power of 10 too big?
        BNC     @@llp           ; Yes:  Put a blank and go to next

@@ldone PULR    R3              ; restore format word

        ;; ---------------------------------------------------------------- ;;
        ;;  The digit loop prints at least one digit.  It discovers digits  ;;
        ;;  by repeated subtraction.                                        ;;
        ;; ---------------------------------------------------------------- ;;
@@digit TSTR    R0              ; If the number is zero, print zero and leave
        BNEQ    @@dig1          ; no: print the number

        MOVR    R3,     R5      ;\
        ADDI    #$80,   R5      ; |-- print a 0 there.
        MVO@    R5,     R4      ;/
        B       @@done

@@dig1: MOVR    R3,     R5      ; for first digit only: check to see if
        BPL     @@cont          ; we need to add '5'.
        ADDI    #$80+4*8,R5     ; start our digit as one just before '5'
        B       @@spcl

@@nxdig MOVR    R3,     R5      ; save display format word
@@cont: ADDI    #$80-8, R5      ; start our digit as one just before '0'
@@spcl:

        ;; ---------------------------------------------------------------- ;;
        ;;  Divide by repeated subtraction.  This divide is constructed     ;;
        ;;  to go "one step too far" and then back up.                      ;;
        ;; ---------------------------------------------------------------- ;;
@@div:  ADDI    #8,     R5      ; increment our digit
        SUB@    R1,     R0      ; subtract power of 10
        BC      @@div           ; loop until we go too far
        ADD@    R1,     R0      ; add back the extra power of 10.

        MVO@    R5,     R4      ; display the digit.

        INCR    R1              ; point to next power of 10
        DECR    R2              ; any room left in field?
        BPL     @@nxdig         ; keep going until R2 < 0.

@@done: PULR    R1              ; restore R1
        PULR    R2              ; restore R2
        PULR    PC              ; return

        ENDP

;; ======================================================================== ;;
;;  FILLZERO                                                                ;;
;;      Fills memory with zeros                                             ;;
;;                                                                          ;;
;;  FILLMEM                                                                 ;;
;;      Fills memory with a constant                                        ;;
;;                                                                          ;;
;;  INPUTS:                                                                 ;;
;;      R0 -- Fill value (FILLMEM only)                                     ;;
;;      R1 -- Number of words to fill                                       ;;
;;      R4 -- Start of fill area                                            ;;
;;      R5 -- Return address                                                ;;
;;                                                                          ;;
;;  OUTPUTS:                                                                ;;
;;      R0 -- Zeroed if FILLZERO, otherwise untouched.                      ;;
;;      R1 -- Zeroed                                                        ;;
;;      R4 -- Points to word after fill area                                ;;
;; ======================================================================== ;;
CLRSCR:     PROC
            MVII    #$0F0,  R1
            MVII    #$200,  R4
FILLZERO:   CLRR    R0              ; Start out with R0 zeroed for FILLZERO
FILLMEM:    MVO@    R0,     R4      ; Store R0 out at R4, and move along
            DECR    R1              ; Keep going until our count runs out
            BNEQ    FILLMEM
            JR      R5              ; Return to the caller.
            ENDP

;; ======================================================================== ;;
;;  TITLE    -- Title of the program, and program date (year - 1900).       ;;
;;  MAIN     -- The main program.                                           ;;
;;                                                                          ;;
;;  Note that MAIN must be immediately after the title string due to how    ;;
;;  we hijack the EXEC initialization sequence.                             ;;
;; ======================================================================== ;;
TITLE:  PROC
        BYTE    118, "Somebody's Homework", 0
MAIN:
        DIS                     ; Disable interrupts during init.
        MVII    #STACK, R6      ; Reset the stack pointer

        MVII    #ISR,   R0      ; \
        MVO     R0,     $100    ;  |_ Simplified ISR to keep the lights on
        SWAP    R0              ;  |
        MVO     R0,     $101    ; /

        CALL    CLRSCR          ; Clear the screen.

        EIS                     ; Re-enable interrupts.

        ; Read in 10 unsigned integers from the controller keypad.
        MVII    #10,    R2      ; Number of elements.
        MVII    #ARRAY, R3      ; Where to store the data.
        MVII    #$205,  R4      ; Where to display on-screen: Row 0, Col 5.

@@in_loop:
        CALL    RD_NUM          ; Get a number, returned in R0

        MVO@    R0,     R3      ; \_ Store to the array.
        INCR    R3              ; /

        ADDI    #$14,   R4      ; Advance to next row of the display.

        DECR    R2              ; Loop over 10 inputs.
        BNEQ    @@in_loop

        ; Compute the 32-bit sum into R1:R0.
        MVII    #ARRAY, R4      ; R4 is auto-incrementing.
        CLRR    R0              ; \_ Initial sum is 0.
        CLRR    R1              ; /
        MVII    #10,    R2      ; Loop counter.
@@sum_loop:
        ADD@    R4,     R0      ; Lower half of 16 + 32 addition.
        ADCR    R1              ; Accumulate the carries.
        DECR    R2              ; Loop over 10 elements.
        BNEQ    @@sum_loop

        ; Now display on bottom row of the screen.
        MVII    #$200+11*20, R4 ; Last row, flush at left.
        MVII    #C_WHT, R3      ; Display in white
        MVII    #10,    R2      ; 10 digit field.
        CALL    PRNUM32.b       ; Put it on the screen.

        ; Spin-loop.  We're done!
        DECR    PC
        ENDP

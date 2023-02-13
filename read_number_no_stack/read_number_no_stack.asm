; TMS3206400 Assembly 
; Author: Joe Zbiciak  
; Date: 27-Feb-2022 
; SPDX-License-Identifier:  CC-BY-SA-4.0 
; 
; Quora request: 
; 
; "How do you write a program to get a multi-digit number from the 
; keyboard and store the number in its binary form (don't make use of 
; the stack)?" 
; 
; I will interpret the question as: 
; 1.  Input arrives in ASCII. 
; 2.  Output should decode ASCII  to a positional weighted binary. 
; 
; To ensure zero stack usage, this is written in 100% assembly code 
; for the TMS320C6400 processor family. 
; 
; The code assumes there is a simple UART that has status bits for RX
; data ready and TX not busy, and register address for RX and TX.  RX
; and TX may be shared.  This model matches many UARTs.  The default
; mimics the 8250/16550. 
 
; Configure these to your actual UART addresses, etc. 
; 
; Everything is a byte offset from UART_BASE. 
UART_BASE   .equ    0x10000000 
UART_TX     .equ    0 
UART_RX     .equ    0 
UART_LSR    .equ    5 
 
UART_RxRdy  .equ    1 << 0 
UART_TxIdle .equ    1 << 5 
 
; Set up the data segment for our 
; number. 
number  .usect ".bss:number", 4, 4 
buf     .usect ".bss:buf", 10, 1 
 
;------------------------------------- 
; Main program. 
;------------------------------------- 
        .sect   ".text:main" 
        .global main 
main: 
        ; Set up B31 as UART_BASE. 
        ; UART code needs this. 
        MVKL.S2     UART_BASE, B31 
        MVKH.S2     UART_BASE, B31 
 
        ; Print opening message. 
        CALL.S1     puts 
        MVKL.S1     msg, A4 
        MVKH.S1     msg, A4 
        ADDKPC.S2   $1, B3, 2 
 
        ; Print the prompt. 
$1:     CALL.S1     fputs 
        MVKL.S1     prompt, A4 
        MVKH.S1     prompt, A4 
        ADDKPC.S2   $2, B3, 2 
 
        ; Get a number into A4. 
$2:     CALL.S1     getnum_dec 
        ADDKPC.S2   $3, B3, 4 
 
        ; Store it to memory. 
$3:     MVKL.S1     number, A0 
        MVKH.S1     number, A0 
        STW.D1T1    A4, *A0 
 
        ; Print it in hex, and 
        ; loop back to $1. 
        CALL.S1     prtnum_hex 
        ADDKPC.S2   $1, B3, 4 
 
;------------------------------------- 
; Reads a decimal number up to 
; 32 bits.  No overflow checking. 
; Result in A0. 
; Trashes A1, A2, A4, A10, A11, 
;         B6, B10, B11 
;------------------------------------- 
        .sect ".text:getnum_dec" 
        .global getnum_dec 
getnum_dec: 
        MVKL.S2     buf, B10 
||      MVK.S1      '0', A10 
||      MV.S2       B3, B6 ; save ret. 
 
        MVKH.S2     buf, B10 
||      MVK.S1      '9', A11 
||      ZERO.L2     B11 ; i = buf idx 
 
; Get digits, ringing bell if the user 
; enters too many, or enters invalid 
; characters. Terminate on newline only 
; if buffer is non-empty. Allow bksp. 
$1:     CALL.S1     uart_rxe   ; echo 
||      ADDKPC.S2   $2, B3, 5 
 
$2:     MV.L1X      B11, A1    ; i>0? 
||      STB.D2T1    A0, *B10[B11] ; buf[i] 
||      CMPEQ.L1    A0, 13, A2 ; cr? 
 
        CMPEQ.L1    A0, 10, A4 ; lf? 
        ; i != 0 && (c == CR || c == LF) 
  [ A1] OR.L1       A2, A4, A1       
 
  [ A1] B.S1        $3  ; end of input 
||[ A1] MV.L1       A10, A0 ; short ckt 
||[ A1] ADDK.S2     -1, B11 ; undo ++i      
 
        CMPLT.L1    A0, A10, A1 ; c<'0' 
||      MVKL.S1     err, A4 
 
  [!A1] CMPGT.L1    A0, A11, A1 ; c>'9' 
||      MVKH.S1     err, A4 
 
  [!A1] CMPEQ.L1X   B11, 9, A1 ; full? 
 
  [ A1] CALL.S1     fputs  ; beep/bksp 
||[ A1] ADDKPC.S2   $1, B3, 0 
 
  [!A1] ADDK.S2     1, B11    ; ++i 
; br to $3 occurs 
        CMPEQ.L1    A0, 8, A1 ; bksp? 
  [ A1] MV.L1X      B11, A1   ; i>0? 
  [ A1] ADD.L1      A4, 2, A4 ; no ^G^H 
  [ A1] ADDK.S2     -1, B11   ; --i 
; call to fputs occurs 
 
; Input complete.  If we saw a CR, go 
; get the next character to drain LF. 
; Otherwise print a CR to go w/ LF. 
$3: 
  [ A2] CALL.S1     uart_rxe ; echo 
||[!A2] CALL.S2     uart_tx  ; for CR 
 
  [!A2] MVK.S1      13, A0 
||      ADDKPC.S2   $4, B3, 4 
 
        ; Process the buffer. 
$4:     MVK.S1      1,  A4  ; mult. 
||      ZERO.L1     A0 
 
$5:     SUB.L2      B11, 1, B11 
||      SUB.L1X     B11, 1, A1 
 
        LDBU.D2T1   *B10[B11], A2 
        NOP         1 
 
  [ A1] BNOP.S1     $5, 2 
||[!A1] RET.S2      B6 
 
        AND.L1      A2, 0xF, A2 
||      MPY.M1      A4, 10, A4 
 
        MPY.M1      A2, A4, A2 
        ADD.L1      A0, A2, A0 
; br/ret occurs 
 
;------------------------------------- 
; Prints number in A0 in hex. 
; Trashes: A0, A1, A2, A3, A4, B5 
;------------------------------------- 
        .sect ".text:prtnum_hex" 
        .global prtnum_hex 
prtnum_hex: 
        ROTL.M1     A0, 4, A3   ; save 
 
        MV.L2       B3, B5  ; save 
||      MVK.S1      8,  A4  ; loop cnt 
||      ADDKPC.S2   $1, B3, 0 
 
$1:     CALL.S1     uart_tx 
||      AND.L1      A3, 0xF, A0 
||      ROTL.M1     A3, 4, A3 
 
        ADDK.S1     0x30, A0 
||      CMPGT.L1    A0, 9, A1 
 
  [ A1] ADD.L1      A0, 7, A0 
        SUB.L1      A4, 1, A4 
        MV.L1       A4, A1    ;\ loop 
  [ A1] ADDKPC.S2   $2, B3, 0 ;/ exit 
 
        ; Chain return through CR+LF 
$2:     CRNOP.S1    crlf, 5 
||      MV.L2       B5, B3 
 
;------------------------------------- 
; Receives byte from UART, blocking. 
; Returns byte in A0.  Trashes A1. 
;------------------------------------- 
        .sect   ".text:uart_rx" 
        .global uart_rx 
uart_rx: 
        MVK.S1      UART_RxRdy, A1 
||      ZERO.D1     A0 
 
$1:      
  [ A0] RET.S2      B3 
||[!A0] B.S1        $1 
||[!A0] LDBU.D2T1   *B31(UART_LSR), A0 
 
  [ A0] LDBU.D2T1   *B31(UART_RX), A0 
        NOP         3 
  [!A0] AND.L1      A0, A1, A0 
 
;------------------------------------- 
; Receives byte with echo, blocking. 
; Returns byte in A0. 
; Trashes A1, A2, B4. 
;------------------------------------- 
        .sect   ".text:uart_rxe_tx" 
        .global uart_rxe 
uart_rxe: 
        ; save return address 
        MV.L2       B3, B4 
 
        CALL.S1     uart_rx 
||      ADDKPC.S2   $1, B3, 5 
 
        ; fallthru into uart_tx 
        ; after restoring retaddr. 
$1:     MV.L2       B4, B3 
 
;------------------------------------- 
; Sends byte to UART, blocking. 
; Byte to send in A0.  A0 unmodified. 
; Trashes A1, A2. 
;------------------------------------- 
        .newblock 
        .global uart_tx 
uart_tx: 
        MVK.S1      UART_TxIdle, A2 
||      ZERO.D1     A1 
$1: 
  [ A1] RET.S2      B3 
||[!A1] B.S1        $1 
||[!A1] LDBU.D2T1   *B31(UART_LSR), A1 
 
  [ A1] STB.D2T1    A0, *B31(UART_TX) 
        NOP         3 
        AND.L1      A1, A2, A1 
 
;------------------------------------- 
; Like fputs below, but adds CR+LF. 
; Trashes A0, A1, A2, A4, B4, B5 
; Alt entry crlf just prints CR + LF. 
;------------------------------------- 
        .sect   ".text:puts_fputs" 
        .global puts, crlf 
puts: 
        CALL.S1     fputs 
||      ADDKPC.S2   $1, B3, 4 
||      MV.L2       B3, B5 
 
$1:     MV.L2       B5, B3 
crlf:   MVKL.S1     s_crlf, A0 
        MVKH.S1     s_crlf, A0 
        ; fallthru into fputs. 
        
;------------------------------------- 
; Print an ASCIIZ string, as-is. 
; Pointer to string in A4. 
; Trashes A0, A1, A2, A3, A4, B4. 
;------------------------------------- 
        .newblock 
        .global fputs 
fputs: 
        ; Save return address. 
        ; Fetch chars one ahead. 
        MV.L2       B3, B4 
||      LDBU.D1T1   *A4++, A3 
 
        NOP         4 
        MV.L1       A3, A0 
$1:    
  [!A0] RET.S2      B4        ; empty? 
||[ A0] CALL.S1     uart_tx 
||[ A0] LDBU.D1T1   *A4++, A3 ; prefetch 
||[ A0] MV.L1       A3, A0 
 
        ADDKPC.S2   $1, B3, 3 
        ; chain return at end of str. 
  [!A0] MV.L2       B4, B3 
 
; Some messages to print. 
        .sect ".rodata:strings" 
 
msg:    .string  "Enter a decimal " 
        .string  "number and then " 
        .cstring "press enter." 
 
prompt: .cstring "> " 
 
        ; err rings bell, backs up 
        ; and blanks the character. 
        ; err+2 prints a space and 
        ; backs up, useful after  
        ; receiving ^H with echo. 
err:    .cstring  "\a\b \b" 
 
        ; CR+LF 
s_crlf  .byte    10, 13, 0 

;; ============================================ ;;
;;  Accept 3 decimal integers from the keypad   ;;
;;  and print the largest of the three.         ;; 
;;  Author:  Joseph Zbiciak, 27-Nov-2020        ;; 
;;                                              ;; 
;; -------------------------------------------- ;; 
;;                                              ;; 
;;    The routines and data in this program     ;;
;;    are dedicated to the public domain via    ;;
;;    the Creative Commons CC0 v1.0 license     ;;
;;    by its author, Joseph Zbiciak.            ;;
;;                                              ;;
;;    CC CC0: https://tinyurl.com/d6c82a3       ;;
;; ============================================ ;;
 
    ROMW    16          ; Use 16-bit ROM width 
 
;; ============================================ ;; 
;;  Color values for color-stack mode.          ;; 
;; ============================================ ;; 
C_BLK   QEQU    $0      ; Black 
C_BLU   QEQU    $1      ; Blue 
C_RED   QEQU    $2      ; Red 
C_TAN   QEQU    $3      ; Tan 
C_DGR   QEQU    $4      ; Dark Green 
C_GRN   QEQU    $5      ; Green 
C_YEL   QEQU    $6      ; Yellow 
C_WHT   QEQU    $7      ; White 
C_GRY   QEQU    $8      ; Grey 
C_CYN   QEQU    $9      ; Cyan 
C_ORG   QEQU    $A      ; Orange 
C_BRN   QEQU    $B      ; Brown 
C_PNK   QEQU    $C      ; Pink 
C_LBL   QEQU    $D      ; Light Blue 
C_YGR   QEQU    $E      ; Yellow-Green 
C_PUR   QEQU    $F      ; Purple 
 
;; ============================================ ;; 
;;  Memory layout.                              ;; 
;; ============================================ ;; 
STACK:  EQU     $2F0    ; 32 word stack
 
;; ============================================ ;; 
;;  EXEC-friendly ROM header.                   ;; 
;; ============================================ ;; 
    ORG     $5000       ; Location of the ROM header 
ROMHDR:
    BIDECLE ZERO        ; MOB picture base  (empty)
    BIDECLE ZERO        ; Process table     (empty)
    BIDECLE MAIN        ; Program start address 
    BIDECLE ZERO        ; Bkgd picture base (empty)
    BIDECLE ONES        ; GRAM pictures     (empty)
    BIDECLE TITLE       ; Cartridge title/date 
    DECLE   $03C0       ; No ECS title, run code
                        ; ...  after title, no clicks 
ZERO:
    DECLE   $0000       ; Screen border control 
    DECLE   $0000       ; 0 = CSTK, 1 = FGBG
ONES:
    DECLE   C_BLU       ; Initial color stack 0: Blue 
    DECLE   C_BLU       ; Initial color stack 1: Blue
    DECLE   C_BLU       ; Initial color stack 2: Blue 
    DECLE   C_BLU       ; Initial color stack 3: Blue 
    DECLE   C_BLU       ; Initial border color: Blue 
 
;; ============================================ ;; 
;;  ISR                                         ;;
;;  Enables the screen and sets color stack     ;;
;;  mode w/blue background.  We have to set it  ;;
;;  ourselves, despite requesting blue in the   ;; 
;;  ROM header, as we hijack the EXEC's         ;;
;;  initialization sequence.                    ;; 
;; ============================================ ;; 
ISR:    PROC 
    MVI     $21, R0     ; Set color stack mode 
    MVO     R0, $20     ; Enable display 
    MVII    #C_BLU, R0  ; Blue color 
    MVO     R0, $28     ; Color stack #0 
    MVO     R0, $2C     ; Border color 
    JR      R5          ; Return from interrupt 
    ENDP 
 
;; ============================================ ;; 
;;  RD_KEY                                      ;;
;;  Gets a key from the keypad, and returns     ;;
;;  the digit value.                            ;; 
;;  Also, 10 = [Clear], 11 = [Enter].           ;; 
;;  Digit value in R0.  R1 - R4 preserved.      ;; 
;; ============================================ ;; 
RD_KEY: PROC 
    PSHR    R5          ; Save return address. 
    PSHR    R1          ; Save R1. 
 
    ; Wait until the controllers are idle. 
@@wait_idle: 
    MVI     $1FE, R1    ; \_ Read and merge
    XOR     $1FF, R1    ; /  both controllers. 
    BNEQ    @@wait_idle ; Idle = $00. 
 
@@read_again: 
    ; Read both controllers, waiting for a
    ; stable value (debounce).  "Previous stable
    ; value" is the "idle" value.
    CLRR    R5
@@read_loop: 
    MVI     $1FE, R1    ; \_ Read and merge
    XOR     $1FF, R1    ; /  both controllers. 
    BEQ     @@read_loop ; Loop until not idle. 
 
    CMPR    R1, R5      ; Same as previous value? 
    BEQ     @@same_val  ; Yes: Don't restart timer. 
    MVII    #$100, R0   ; Restart the debounce timer. 
    MOVR    R1, R5      ; Remember the new value. 
@@same_val: 
 
    DECR    R0          ; Decrement debounce timer. 
    BNEQ    @@read_loop ; Loop until it expires. 

    ; Now look up the encoded key value to
    ; see if it's a valid keypad keypress. 
    MVII    #@@keys, R5 ; Encoded keypad table
    MVII    #11, R0     ; Total table size, minus 1. 
@@key_loop: 
    CMP@    R5, R1      ; Does it match? 
    BEQ     @@found_it  ; Yes:  Break out. 
    DECR    R0          ; \_ Loop over all 12
    BPL     @@key_loop  ; /  key's encodings. 
    B       @@read_again; It wasn't what we
                        ; wanted, so try again. 
 
@@found_it: 
    PULR    R1          ; Restore R1. 
    PULR    PC          ; Return 
 
    ; The key table is ordered in reverse order,
    ; so that the loop counter in R0 also matches
    ; the return value.  Slick, eh? 
@@keys:
    ;        E    C    9    8    7    6
    BYTE    $28, $88, $24, $44, $84, $22
    ;        5    4    3    2    1    0 
    BYTE    $42, $82, $21, $41, $81, $48 
    ENDP 
 
;; ============================================ ;; 
;;  RD_NUM                                      ;;
;;  Gets a 5-digit unsigned integer in the      ;;
;;  range 0 - 65535.  Displays the input on     ;;
;;  screen @R4.                                 ;; 
;;                                              ;;
;;  [Clear] resets the input, [Enter] accepts.  ;; 
;;  R0 contains final value; R1-R4 preserved.   ;; 
;; ============================================ ;;
RD_NUM: PROC 
    PSHR    R5 
    PSHR    R1 
    PSHR    R2 
    NOP             ; (red'd for interruptibility)
    PSHR    R3 
    PSHR    R4 
 
@@reset: 
    CLRR    R1          ; Use R1 as input accum.
 
@@loop: 
    PULR    R4          ; \_ Restore display
    PSHR    R4          ; /  position. 
    MVII    #5, R2      ; 5 digit field. 
    MVII    #C_WHT, R3  ; Display in white. 
    MOVR    R1, R0      ; Disp. the accum value. 
    CALL    PRNUM16.b   ; Disp. w/leading blanks. 
 
    ; Get the next digit from the user and merge
    ; it into the accumulator.  If the value
    ; would overflow, drop the input digit.  Do
    ; not allow inputs larger than 65,535. 
    CALL    RD_KEY      ; Get digit from keypad. 

    CMPI    #10, R0     ; \_ Is it [Clear]?
    BEQ     @@reset     ; /  If so, reset. 

    CMPI    #11, R0     ; \_ Is it [Enter]?
    BEQ     @@done      ; /  If so, return number.
 
    ; Make a copy of the accumulator to work on,
    ; so we can back out if any step of the
    ; computation overflows. 
    MOVR    R1, R2 

    SLLC    R2, 1       ; Multiply by 2. 
    BC      @@loop      ; Abort on overflow (C=1)

    MOVR    R2, R3      ; Remember "value * 2". 

    SLLC    R2, 2       ; Mult by 4 (orig value*8). 
    BC      @@loop      ; \_ Abort on overflow
    BOV     @@loop      ; /  (C=1 or OV=1)

    ADDR    R2, R3      ; x*2 + x*8 gives x*10. 
    BC      @@loop      ; Abort on overflow.       

    ADDR    R0, R3      ; Add in the new digit. 
    BC      @@loop      ; Abort on overflow.

    MOVR    R3, R1      ; Our new accum value.
    B       @@loop 
 
@@done: 
    MOVR    R1, R0      ; Put accum value into R0. 
    PULR    R4          ; \ 
    PULR    R3          ;  |_ Restore R1 - R4 
    PULR    R2          ;  | 
    PULR    R1          ; / 
    PULR    PC          ; Return. 
    ENDP 
 
;; ============================================ ;;
;;  _PW10                                       ;;
;;  Lookup table holding the first 5 powers     ;;
;;  of 10 (1 thru 10000) as 16-bit numbers.     ;;
;; ============================================ ;;
_PW10   PROC    ; 0 thru 10000 
        DECLE   10000, 1000, 100, 10, 1, 0 
        ENDP 
 
;; ============================================ ;;
;;  PRNUM16.b                                   ;;
;;  Prints an unsigned 16-bit number            ;;
;;  with leading blanks.                        ;; 
;;                                              ;;
;;  AUTHOR                                      ;; 
;;      Joseph Zbiciak                          ;; 
;;                                              ;; 
;;  INPUTS for all variants                     ;; 
;;      R0  Number to print.                    ;; 
;;      R2  Width of field.                     ;;
;;      R3  Format word, added to digits to     ;;
;;          set the color.                      ;; 
;;      R4  Location on screen to print number  ;;
;;                                              ;; 
;;  OUTPUTS                                     ;; 
;;      R0  Zeroed                              ;; 
;;      R1  Unmodified                          ;; 
;;      R2  Unmodified                          ;; 
;;      R3  Unmodified                          ;; 
;;      R4  Points to first char after field.   ;;
;;                                              ;;
;;  NOTE:  This was extracted from a more       ;;
;;  featureful version that offered greater     ;;
;;  flexibility.                                ;;
;; ============================================ ;;
 
PRNUM16 PROC 
@@b PSHR    R5 
    PSHR    R2 
    PSHR    R1 
 
    ;; ---------------------------------------- ;;
    ;;  Find the initial power of 10 to use     ;;
    ;;  for display.  Note:  For fields wider   ;;
    ;;  than 5, fill the extra spots above 5    ;; 
    ;;  with blanks or zeros as needed.         ;;
    ;; ---------------------------------------- ;;
    MVII    #_PW10+5,R1 ; End of power-of-10 table 
    SUBR    R2, R1      ; Sub field width for power
    PSHR    R3          ; Save format word 

    CLRR    R5          ; \_ lead with blanks.
    B       @@lblnk     ; /
 
 
@@llp:
    MVO@    R3, R4      ; print a blank/zero 
 
    INCR    R1          ; get next power of 10 
@@lblnk:
    DECR    R2          ; decr available digits 
    BEQ     @@ldone 
    CMPI    #5, R2      ; field too wide? 
    BGE     @@llp       ; Yes:  Force a blank
    CMP@    R1, R0      ; This pow-of-10 too big? 
    BNC     @@llp       ; Yes:  Print a blank.
 
@@ldone:
    PULR    R3          ; restore format word 
 
    ;; ---------------------------------------- ;;
    ;;  The digit loop prints at least one      ;;
    ;;  digit.  It discovers digits by          ;;
    ;;  repeated subtraction.                   ;;
    ;; ---------------------------------------- ;;
@@digit
    TSTR    R0          ; If num == 0 print 0.
    BNEQ    @@nxdig     ; no: decimalize the number 
 
    MOVR    R3, R5      ;\     
    ADDI    #$80, R5    ; |-- print a 0 there. 
    MVO@    R5, R4      ;/     
    B       @@done 
 
@@nxdig:
    MOVR    R3, R5      ; save display format word 
    ADDI    #$80-8, R5  ; start just before '0' 
 
    ;; ---------------------------------------- ;;
    ;;  Divide by repeated subtraction.  This   ;;
    ;;  divide is constructed to go "one step   ;;
    ;;  too far" and then back up.              ;;
    ;; ---------------------------------------- ;;
@@div:
    ADDI    #8, R5      ; increment our digit 
    SUB@    R1, R0      ; subtract power of 10 
    BC      @@div       ; loop until we go too far 
    ADD@    R1, R0      ; add back extra pow-of-10. 

    MVO@    R5, R4      ; display the digit. 

    INCR    R1          ; point to next pow-of-10 
    DECR    R2          ; any room left in field? 
    BPL     @@nxdig     ; keep going until R2 < 0. 
 
@@done:
    PULR    R1          ; restore R1 
    PULR    R2          ; restore R2 
    PULR    PC          ; return 
    ENDP 
 
;; ============================================ ;;
;;  FILLZERO                                    ;; 
;;      Fills memory with zeros.                ;; 
;;                                              ;; 
;;  FILLMEM                                     ;; 
;;      Fills memory with a constant.           ;; 
;;                                              ;; 
;;  CLRSCR                                      ;; 
;;      Fills BACKTAB with zeros.               ;; 
;;                                              ;; 
;;  INPUTS:                                     ;; 
;;      R0 -- Fill value (FILLMEM only)         ;; 
;;      R1 -- Number of words to fill           ;; 
;;      R4 -- Start of fill area                ;; 
;;      R5 -- Return address                    ;; 
;;                                              ;; 
;;  OUTPUTS:                                    ;; 
;;      R0 -- Zeroed if FILLZERO, CLRSCR.       ;; 
;;            Otherwise, unmodified.            ;;
;;      R1 -- Zeroed                            ;; 
;;      R4 -- Points to word after fill area    ;; 
;; ============================================ ;;
CLRSCR: PROC 
    MVII    #$0F0,  R1 
    MVII    #$200,  R4 
FILLZERO:
    CLRR    R0          ; Zero R0 for FILLZERO 
FILLMEM:
    MVO@    R0, R4      ; 0 => R4, incr R4.
    DECR    R1          ; \_ iterate over entire
    BNEQ    FILLMEM     ; /  block of memory
    JR      R5          ; Return to the caller. 
    ENDP 
 
;; ============================================ ;;
;;  DISP_PTR                                    ;;
;;  Computes a pointer to the display, given    ;;
;;  a row and a column.                         ;;
;; ============================================ ;;
MACRO disp_ptr(r,c)
($200 + (%r%)*20 + (%c%))
ENDM

;; ============================================ ;;
;;  TITLE    -- Title of the program,           ;;
;;              and program date (year-1900).   ;;
;;  MAIN     -- The main program.               ;;
;;                                              ;;
;;  Note that MAIN must be immediately after    ;;
;;  the title string due to how we hijack the   ;;
;;  EXEC initialization sequence.               ;;
;; ============================================ ;;
TITLE:  PROC 
    BYTE    2020-1900, "Somebody's Homework", 0 
MAIN: 
    DIS                 ; Disable interrupts
    MVII    #STACK, R6  ; Reset the stack pointer 

    MVII    #ISR, R0    ; \ 
    MVO     R0, $100    ;  |_ Simplified ISR to
    SWAP    R0          ;  |  keep the lights on 
    MVO     R0, $101    ; / 

    CALL    CLRSCR      ; Clear the screen. 
    EIS                 ; Re-enable interrupts. 

    ; Read in 3 unsigned integers from the
    ; hand controller keypad. 
    MVII    #disp_ptr(3,8), R4
    CALL    RD_NUM      ; Get first number
    MOVR    R0, R1      ; It's de facto largest.
 
    MVII    #disp_ptr(4,8), R4
    CALL    RD_NUM      ; Get second number
    CMPR    R1, R0      ; Is it new largest?
    BNC     @@nope1     ; No: drop it
    MOVR    R0, R1      ; Yes: keep it
@@nope1:

    MVII    #disp_ptr(5,8), R4
    CALL    RD_NUM      ; Get third number
    CMPR    R1, R0      ; Is it new largest?
    BC      @@nope2     ; Yes: keep it
    MOVR    R1, R0      ; No: largest => R0
@@nope2:

    ; Now display largest two lines below
    ; the user's input.  Largest is in R0.
    MVII    #disp_ptr(7,8), R4
    MVII    #C_WHT, R3  ; Display in white 
    MVII    #5, R2      ; 5 digit field. 
    CALL    PRNUM16.b   ; Put it on the screen. 
 
    ; Spin-loop.  We're done! 
    DECR    PC 
    ENDP 


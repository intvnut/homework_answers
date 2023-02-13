;* ======================================================================== *;
;*  The routines and data in this file (mpy8q8.asm) are dedicated to the    *;
;*  public domain via the Creative Commons CC0 v1.0 license by its author,  *;
;*  Joseph Zbiciak.                                                         *;
;*                                                                          *;
;*          https://creativecommons.org/publicdomain/zero/1.0/              *;
;* ======================================================================== *;

;; ======================================================================== ;;
;;  Multiply two 8q8 unsigned values.                                       ;;
;;                                                                          ;;
;;  AUTHOR                                                                  ;;
;;      Joseph Zbiciak <intvnut AT gmail.com>                               ;;
;;                                                                          ;;
;;  REVISION HISTORY                                                        ;;
;;      18-Aug-2019 Initial version  . . . . . . . . . . .  J. Zbiciak      ;;
;;                                                                          ;;
;;  INPUTS                                                                  ;;
;;      R0  Multiplicand in 8q8                                             ;;
;;      R1  Multiplicand in 8q8                                             ;;
;;                                                                          ;;
;;  OUTPUTS                                                                 ;;
;;      R0  Product in 8q8, rounded.                                        ;;
;;                                                                          ;;
;;  NOTES                                                                   ;;
;;      Does not handle overflow.                                           ;;
;;      Trashes R1, R2, R3, R4.                                             ;;
;;      Straightforward implementation; not heavily optimized.              ;;
;; ======================================================================== ;;
MPY8Q8      PROC
            CLRR    R2              ; \_ 32-bit accumulator
            CLRR    R3              ; /

            MVII    #16,    R4      ; 16 iterations for 16x16 multiply
            B       @@first

@@loop:     SLLC    R2,     1       ; \_ Shift accumulator left
            RLC     R3,     1       ; /

@@first:    SLLC    R1,     1       ; Consume 2nd multiplicand bits from top
            BNC     @@zero
            ADDR    R0,     R2      ; \_ For every '1' bit add 1st
            ADCR    R3              ; /  multiplicand to the accumulator

@@zero:     DECR    R4              ; \_ iterate over all 16 digits.
            BNEQ    @@loop          ; /
        
            ADDI    #$80,   R2      ; \_ Round to 8q8.
            ADCR    R3              ; /

            ANDI    #$FF00, R2      ; \
            ANDI    #$00FF, R3      ;  |_ Merge middle 16 bits into result
            XORR    R3,     R2      ;  |
            SWAP    R2              ; /
            MOVR    R2,     R0      ; Final result in R0.

            JR      R5              ; return
            ENDP

;; ======================================================================== ;;
;;  End of file:  mpy8q8.asm                                                ;;
;; ======================================================================== ;;

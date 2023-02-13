;* ======================================================================== *;
;*  The routines and data in this file (heron.asm) are dedicated to the     *;
;*  public domain via the Creative Commons CC0 v1.0 license by its author,  *;
;*  Joseph Zbiciak.                                                         *;
;*                                                                          *;
;*          https://creativecommons.org/publicdomain/zero/1.0/              *;
;* ======================================================================== *;

;; ======================================================================== ;;
;;  Heron's Formula                                                         ;;
;;  Fixed point unsigned 8q8 implementation on the Intellivision.           ;;
;;                                                                          ;;
;;  AUTHOR                                                                  ;;
;;      Joseph Zbiciak <intvnut AT gmail.com>                               ;;
;;                                                                          ;;
;;  REVISION HISTORY                                                        ;;
;;      18-Aug-2019 Initial version  . . . . . . . . . . .  J. Zbiciak      ;;
;;                                                                          ;;
;;  INPUTS                                                                  ;;
;;      R0  Triangle side 'a' in 8q8.                                       ;;
;;      R1  Triangle side 'b' in 8q8.                                       ;;
;;      R2  Triangle side 'c' in 8q8.                                       ;;
;;                                                                          ;;
;;  OUTPUTS                                                                 ;;
;;      R0  Area in 8q8.                                                    ;;
;;                                                                          ;;
;;  NOTES                                                                   ;;
;;      R1 thru R4 trashed.                                                 ;;
;;      Assumes a, b, c form a valid triangle and no side is zero.          ;;
;; ======================================================================== ;;

HERON       PROC
            PSHR    R5

            ;; Sort a, b, c so a >= b and a >= c.  We don't care if b >= c.
            CMPR    R0,     R1
            BNC     @@b_lt_a
            XORR    R0,     R1      ; \   The infamous 3-XOR swap still
            XORR    R1,     R0      ;  |- makes sense on the Intellivision.
            XORR    R0,     R1      ; /
@@b_lt_a:            
            CMPR    R0,     R2
            BNC     @@c_lt_a
            XORR    R0,     R2      ; \   The infamous 3-XOR swap still
            XORR    R2,     R0      ;  |- makes sense on the Intellivision.
            XORR    R0,     R2      ; /
@@c_lt_a:
            ;; Reserve stack space for t1, t2, t3.
            ADDI    #3,     R6

            ;; Save a, b, c on the stack, with a last.
            PSHR    R2              ; push c
            PSHR    R1              ; push b
            PSHR    R0              ; push a

            ;; Compute t1' = (a + b + c) / 4, rounded.
            ;; Perform the computation at extended precision so we don't
            ;; lose bits to overflow.
            CLRR    R3
            ADDR    R0,     R1
            ADCR    R3
            ADDR    R2,     R1
            ADCR    R3
            ADDI    #2,     R1
            ADCR    R3
            SARC    R3,     2
            RRC     R1,     2

            ;; Compute t1 = sqrt(t1') in Q8 plus an extra bit of result.
            ;; Result in R2.
            CALL    SQRT            ; \_ Lie that we're Q10.  This will
            DECLE   10              ; /  give us one extra bit of result.

            INCR    R2              ; \_ Round to a proper Q8 result.
            SLR     R2,     1       ; /

            MOVR    R6,     R3
            SUBI    #6,     R3
            MVO@    R2,     R3      ; Save t1.

            ;; Compute t2' = -a + b + c.  This isn't performed at extended
            ;; precision, as a is supposed to be the largest value.
            MOVR    R6,     R4
            SUBI    #3,     R4
            MVI@    R4,     R1      ; Get c.
            ADD@    R4,     R1      ; Add b.
            SUB@    R4,     R1      ; Subtract a.

            ;; Compute t2 = sqrt(t2') in Q8.  Result in R2.
            CALL    SQRT            ; \_ Lie that we're Q10.  This will
            DECLE   10              ; /  give us one extra bit of result.

            INCR    R2              ; \_ Round to a proper Q8 result.
            SLR     R2,     1       ; /

            MOVR    R6,     R3
            SUBI    #5,     R3
            MVO@    R2,     R3      ; Save t2.

            ;; Compute t3' = (a - b + c) / 2, rounded.  This is performed at
            ;; extended precision.
            MOVR    R6,     R4
            SUBI    #3,     R4
            MVI@    R4,     R1      ; Get c.
            MVII    #-1,    R3      ; Prepare for not-borrow
            SUB@    R4,     R1      ; Subtract b.
            ADCR    R3              ; Add not-borrow.
            ADD@    R4,     R1      ; Add a.
            ADCR    R3              ; Add carry.

            ADDI    #1,     R1      ; \
            ADCR    R3              ;  |_ Round the value and divide by 2.
            SARC    R3,     1       ;  |
            RRC     R1,     1       ; /

            ;; Compute t3 = sqrt(t3') in Q8.  Result in R2.
            CALL    SQRT            ; \_ Lie that we're Q10.  This will
            DECLE   10              ; /  give us one extra bit of result.

            INCR    R2              ; \_ Round to a proper Q8 result.
            SLR     R2,     1       ; /

            MOVR    R6,     R3
            SUBI    #4,     R3
            MVO@    R2,     R3      ; Save t3.

            ;; Compute t4' = (a + b - c) / 2, rounded.  This is performed at
            ;; extended precision.
            MOVR    R6,     R4
            SUBI    #3,     R4
            MVI@    R4,     R1      ; Get c.
            NEGR    R1              ; Negate c.
            MVII    #-1,    R3      ; -c will always be 'all 1s' in upper half.
            ADD@    R4,     R1      ; Add b.
            ADCR    R3              ; Add carry.
            ADD@    R4,     R1      ; Add a.
            ADCR    R3              ; Add carry.

            ADDI    #1,     R1      ; \
            ADCR    R3              ;  |_ Round the value and divide by 2.
            SARC    R3,     1       ;  |
            RRC     R1,     1       ; /

            ;; Compute t4 = sqrt(t4') in Q8.  Result in R2.
            CALL    SQRT            ; \_ Lie that we're Q10.  This will
            DECLE   10              ; /  give us one extra bit of result.

            INCR    R2              ; \_ Round to a proper Q8 result.
            SLR     R2,     1       ; /

            ;; Pop a, b, c off stack.  They are no longer needed.
            SUBI    #3,     R6

            ;; Compute t4 * t3.  Result in R0.
            MOVR    R2,     R0      ; Put t4 into R0.
            PULR    R1              ; Get t3.
            CALL    MPY8Q8          

            ;; Compute (t4 * t3) * t2.  Result in R0.
            PULR    R1              ; Get t2.
            CALL    MPY8Q8

            ;; Compute ((t4 * t3) * t2) * t1.  Result in R0
            PULR    R1              ; Get t1.
            PULR    R5              ; Chained return.
            B       MPY8Q8          ; Computes product and chains return.
            ENDP

;; ======================================================================== ;;
;;  End of file:  heron.asm                                                 ;;
;; ======================================================================== ;;

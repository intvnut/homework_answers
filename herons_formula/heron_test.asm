;; This demo program writes the results of three calls to HERON to location
;; $1000, so that a debugger can see the results of the calls.

            ORG     $7000

MAIN:       PROC
            DIS                         ; Disable interrupts.
            MVII    #$2F0,      R6      ; Initialize stack pointer.

            ; Try (4, 13, 15) triangle.  Result should be 24.
            MVII    #4  * 256,  R0 
            MVII    #13 * 256,  R1 
            MVII    #15 * 256,  R2 
            CALL    HERON
            MVO     R0,         $1000   ; Result should be 24 * 256.

            ; Try (13, 4, 15) triangle.  Result should be 24.
            MVII    #13 * 256,  R0 
            MVII    #4  * 256,  R1 
            MVII    #15 * 256,  R2 
            CALL    HERON
            MVO     R0,         $1000   ; Result should be 24 * 256.

            ; Try (13, 15, 4) triangle.  Result should be 24.
            MVII    #13 * 256,  R0 
            MVII    #15 * 256,  R1 
            MVII    #4  * 256,  R2 
            CALL    HERON
            MVO     R0,         $1000   ; Result should be 24 * 256.

            HLT
            ENDP

;; Include the other assembly files to make this a complete program.
            INCLUDE "heron.asm"
            INCLUDE "sqrt.asm"
            INCLUDE "mpy8q8.asm"

; Assume N is in R1, sum in R0. 
SUMSQ   PROC 
        MVII    #1,  R2  ;  Holds the current square 
        MVII    #3,  R3  ;  Holds 2x + 1 
        CLRR    R0 
@@loop: 
        ADDR    R2,  R0  ;  sum = sum + current square 
        ADDR    R3,  R2  ;  compute next square 
        ADDI    #2,  R3  ;  compute next 2x + 1 
        DECR    R1 
        BNEQ    @@loop 
        JR      R5 
        ENDP 

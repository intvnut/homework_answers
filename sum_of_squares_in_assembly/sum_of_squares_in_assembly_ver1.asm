SQUARES PROC 
_n      QSET 1 
        REPEAT 255 
        DECLE  _n * _n 
_n      QSET   _n + 1 
        ENDR 
        ENDP 

; Assume N is in R1, sum in R0. 
; Assume N is less than 256. 
SUMSQ   PROC 
        MVII    #SQUARES, R4 
        CLRR    R0 
@@loop: 
        ADD@    R4,  R0 
        DECR    R1 
        BNEQ    @@loop 
        JR      R5 
        ENDP 

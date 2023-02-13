SUMSQTBL    PROC 
_n          QSET  1 
_sum        QSET  0 
            REPEAT 256 
_sum        QSET  _sum + _n * _n 
            DECLE _sum AND $FFFF, _sum SHR 16 
            ENDR 
            ENDP 
 
; R0 holds N. 
; Return extended precision result 
; in registers R1:R0. 
SUMSQ       PROC 
            MVII #SUMSQTBL - 1, R1 
            ADDR R0, R1 
            ADDR R0, R1 
            MVI@ R1, R0 
            INCR R1 
            MVI@ R1, R1 
            JR   R5 
            ENDP 

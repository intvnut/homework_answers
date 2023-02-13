        ORG     $5000 
;------------------------------ 
; EXEC-friendly ROM header. 
;------------------------------ 
ROMHDR: BIDECLE ZERO 
        BIDECLE ZERO 
        BIDECLE MAIN 
        BIDECLE ZERO 
        BIDECLE ONES 
        BIDECLE TITLE 
        DECLE   $03C0 
 
ZERO:   DECLE   0, 0 
ONES:   DECLE   1, 1, 1, 1, 1    
;------------------------------ 
 
TITLE:  BYTE 119, "Add 20 Evens", 0 
 
        EIS 
        JSR  R5, ADD_EVENS_TO_20 
        MVO  R0, $1000 
        HLT 

MACRO FOR reg,start,end,step 
 
__step.%reg% QSET %step% 
__end.%reg%  QSET %end% 
             MVII #%start%, %reg% 
__loop.%reg%: 
 
ENDM 
 
MACRO NEXT reg 
      ADDI #__step.%reg%, %reg% 
      CMPI #__end.%reg%, %reg% 
      BLE  __loop.%reg% 
ENDM 
 
ADD_EVENS_TO_20: PROC 
      CLRR R0 
      FOR  R1, 2, 20, 2 
      ADDR R1, R0 
      NEXT R1 
      JR   R5 
      ENDP 

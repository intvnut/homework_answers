; C callable as int add12bytes(uint8_t*); 
; Pointer may have any alignment. 
    .global add12bytes 
add12bytes: 
    ADD    .L2X   A4, 6,  B4  ; twin pointer; offset 6 
||  LDBU   .D1T1  *A4++,  A0  ; byte 0 
 
    LDBU   .D1T1  *A4++,  A1  ; byte 1 
||  LDBU   .D2T2  *B4++,  B1  ; byte 6 
 
    LDBU   .D1T1  *A4++,  A2  ; byte 2 
||  LDBU   .D2T2  *B4++,  B2  ; byte 7 
 
    LDBU   .D1T1  *A4++,  A3  ; byte 3 
||  LDBU   .D2T2  *B4++,  B0  ; byte 8 
 
    LDBU   .D1T1  *A4++,  A5  ; byte 4 
||  LDBU   .D2T2  *B4++,  B5  ; byte 9 
 
    LDBU   .D1T1  *A4,    A4  ; byte 5 
||  LDBU   .D2T2  *B4++,  B6  ; byte 10 
 
    LDBU   .D1T1  *B4,    B4  ; byte 11 
||  ADD    .L1    A0, A1, A1  ; sum 0, 1 
 
    ADD    .L1    A1, A2, A1  ; sum 0..2 
||  ADD    .L2    B1, B2, B1  ; sum 6, 7 
||  RET    .S2    B3 
 
    ADD    .L1    A1, A3, A1  ; sum 0..3 
||  ADD    .L2    B1, B0, B1  ; sum 6..8 
 
    ADD    .L1    A1, A5, A1  ; sum 0..4 
||  ADD    .L2    B1, B5, B1  ; sum 6..9 
 
    ADD    .L1    A1, A4, A4  ; sum 0..5 
||  ADD    .L2    B1, B6, B1  ; sum 6..10 
 
    ADD    .L2    B1, B4, B4  ; sum 6..11 
    ADD    .L1X   A4, B4, A4  ; sum 0..11 
; ==== return occurs here ==== 

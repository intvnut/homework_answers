; C callable as int add12bytes(uint8_t*); 
; Pointer may have any alignment. 
   .global add12bytes 
add12bytes: 
    LDNDW .D1T1 *A4(0), A5:A4 ; Load bytes 0..7 
    LDNW  .D1T2 *A4(8), B4    ; Load bytes 8..11 
    MVKL  .S1   0x0101, A0    ; \_ 0x01010101 
    MVKLH .S1   0x0101, A0    ; / for dot product 
    NOP         1 
    DOTPU4 .M1  A0, A4, A4    ; Sum bytes 0..3 
 
    DOTPU4 .M1  A0, A5, A5    ; Sum bytes 4..7 
||  DOTPU4 .M2X A0, B4, B4    ; Sum bytes 8..11 
||  RETNOP .S2  B3, 3         ; RET + 3 NOP cycles 
     
    ADD    .L1  A4, A5, A4 
    ADD    .L1X A4, B4, A4    ; final sum of all 12 
; ==== return occurs here ==== 

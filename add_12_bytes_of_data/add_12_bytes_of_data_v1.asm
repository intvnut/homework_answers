; C callable as int add12bytes(uint8_t*); 
; Pointer must be word aligned. 
   .global add12bytes 
add12bytes: 
   MV   .L2X   A4,     B4    ; twin pointer 
|| LDW  .D1T1  *A4[0], A0    ; first 4 bytes 
 
   LDW  .D1T1  *A4[1], A2    ; next 4 bytes 
|| LDW  .D2T2  *B4[2], B0    ; last 4 bytes 
 
   MVKL .S1    0x00FF, A5    ; even byte mask lo 
|| MVKL .S2,   0xFF00, B6    ; odd byte mask lo 
 
   MVKLH.S1    0x00FF, A5    ; even byte mask hi 
|| MVKLH.S2,   0xFF00, B6    ; odd byte mask hi 
 
   MV   .S2X   A5,     B5    ; twin mask 
|| MV   .S1X   B6,     A6    ; twin mask 
 
   AND  .L1    A0, A5, A0    ; bytes 0, 2 
|| AND  .S1    A0, A6, A1    ; bytes 1, 3 
 
   AND  .L1    A2, A5, A2    ; bytes 4, 6 
|| AND  .S1    A2, A6, A3    ; bytes 5, 7 
|| AND  .L2    B0, B5, B0    ; bytes 8, 10 
|| AND  .S2    B0, B6, B1    ; bytes 9, 11 
 
   ADDU .L1X   A1, B1, A5:A4 ; add 1,3 to 9,11 
|| ADD  .L2X   B0, A0, B0    ; add 0,2 to 8,10 
|| RET  .S2    B3            ; start returning 
 
   ADDU .L1    A3,A5:A4,A5:A4; all odds summed 
|| ADD  .L2X   B0, A2, B0    ; all evens summed 
 
   SHRU .S1    A5:A4,8,A5:A4 ; Align odds to evens 
 
   ADD  .L1X   A4, B0, A4    ; add odds to evens 
 
   SHRU .S2X   A4, 16, B4    ; move upper sum down 
|| EXTU .S1    A4,16,16,A4   ; zero extend lower sum 
 
   ADD  .L1X   A4, B4, A4    ; final sum  
; ==== return occurs here ==== 

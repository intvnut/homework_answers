    .equ    UxMODE,         U1MODE
    .equ    UxSTA,          U1STA
    .equ    UxRXREG,        U1RXREG
    .equ    UxTXREG,        U1TXREG
    .equ    UxBRG,          U1BRG

    .global ser_tx_byte
    .section *, code
ser_tx_byte:
    BTSC    UxSTA, #9       ; Bit 9 is the TX full bit
    BRA     ser_tx_byte
    MOV.B   WREG,  UxTXREG  ; Send the character
    RETURN

    .global print_1_to_20
    .section *, code
print_1_to_20:
    MOV     #1,  W1          ; Lower digit starts at 1
    CLR     W2               ; Upper digit starts at 0
    MOV     #0x30,    W3     ; '0' in ASCII
    BRA     1f               ; Don't print space first time.

0:  MOV     #0x20,    W0     ; ' ' in ASCII
    RCALL   ser_tx_byte      ; Send a space character

1:  CP0     W2               ; Is upper digit 0?
    BRA Z,  2f               ; Yes:  don't print it
    ADD     W2,  W3,  W0     ; Convert it to ASCII
    RCALL   ser_tx_byte      ; Send upper digit

2:  ADD     W1,  W3,  W0     ; Convert lower digit to ASCII
    RCALL   ser_tx_byte      ; Send lower digit

    CP      W2,  #2          ; Did we just send "20"?
    BRA EQ, 3f               ; Yes:  Leave.

    INC     W1,  W1          ; Increment lower digit
    CP      W1,  #10         ; Time to roll over?
    BRA LT, 0b               ; Nope:  Keep going
    CLR     W1               ; Yep:  Reset it ...
    INC     W2,  W2          ; ... and increment upper digit
    BRA     0b               ; ... and keep going.

3:  MOV     #0x0D, W0        ; \
    RCALL   ser_tx_byte      ;  |_ Send a CR + LF
    MOV     #0x0A, W0        ;  |
    BRA     ser_tx_byte      ; / Chain return through ser_tx_byte.

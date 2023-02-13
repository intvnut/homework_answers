    .text
    .global reverse_upper_bits
reverse_upper_bits:
    testw $1024, %ax
    jz    0f
    xorw  $8192, %ax
0:  testw $8192, %ax
    jz    0f
    xorw  $1024, %ax
0:  testw $4096, %ax
    jz    0f
    xorw  $2048, %ax
0:  testw $16384, %ax
    jz    0f
    xorw  $512, %ax
0:  testw $256, %ax
    jz    0f
    xorw  $32768, %ax
0:  testw $2048, %ax
    jz    0f
    xorw  $4096, %ax
0:  testw $1024, %ax
    jz    0f
    xorw  $8192, %ax
0:  testw $32768, %ax
    jz    0f
    xorw  $256, %ax
0:  testw $512, %ax
    jz    0f
    xorw  $16384, %ax
0:  testw $256, %ax
    jz    0f
    xorw  $32768, %ax
0:  testw $16384, %ax
    jz    0f
    xorw  $512, %ax
0:  testw $4096, %ax
    jz    0f
    xorw  $2048, %ax
0:  ret

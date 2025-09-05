.section .text
.globl tea_encrypt
tea_encrypt:
    # Initialize variables
    mv s0, a0      # s0 = n (input parameter)
    addi a0, s0, 1
    ret
.section .text
.globl tea_encrypt
tea_encrypt:
    # Initialize variables
    mv s0, a0           # s0 = value [address] (input parameter)
    lw s1, 4(s0)        # a0 = value[1] (loads the value 30)  
    mv a0, s1
    ret


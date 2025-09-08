.section .text
.globl tea_encrypt
tea_encrypt:
    # Initialize variables
    lw s0, 0(a0)        # s0 = value[0] (input parameter)
    lw s1, 4(a0)        # s1 = value[1]
    lw s2, 0(a1)        # s2 = key[0]
    lw s3, 4(a1)        # s3 = key[1]
    lw s4, 8(a1)        # s4 = key[2]
    lw s5, 12(a1)       # s5 = key[3]
    li s6, 0x9E3779B9   # s4 = DELTA

    mv t0, zero         # sum = 0 
    li t1, 32           # t1 = 32 (encryption rounds left) 

encrypt_loop:
    # Round of encryption 
    add t0, t0, s6      # sum += DELTA

    # Update v0
    slli t2, s1, 4      # t2 = v1 << 4
    add t2, t2, s2      # t2 = ((v1 << 4) + key[0])
    add t3, s1, t0      # t3 = (v1 + sum)
    srli t4, s1, 5      # t4 = v1 >> 5
    add t4, t4, s3      # t4 = (v1 >> 5) + key[1]
    xor t2, t2, t3      # t2 = ((v1 << 4) + key[0]) ^ (v1 + sum)
    xor t2, t2, t4      # t2 = ((v1 << 4) + key[0]) ^ (v1 + sum) ^ ((v1 >> 5) + key[1])
    add s0, s0, t2      # v0 += t2

    # Update v1
    slli t2, s0, 4      # t2 = v0 << 4
    add t2, t2, s4      # t2 = ((v0 << 4) + key[2])
    add t3, s0, t0      # t3 = v0 + sum
    srli t4, s0, 5      # t4 = v0 >> 5
    add t4, t4, s5      # t4 = (v0 >> 5) + key[3]
    xor t2, t2, t3      # t2 = ((v0 << 4) + key[2]) ^ (v0 + sum)
    xor t2, t2, t4      # t2 = ((v0 << 4) + key[2]) ^ (v0 + sum) ^ ((v0 >> 5) + key[3])
    add s1, s1, t2      # v1 += t2

    # Update round countdown
    addi t1, t1, -1     
    bne t1, zero, encrypt_loop

return_encrypt:
    sw s0, 0(a3)           # value[0] = v0
    sw s1, 4(a3)           # value[1] = v1
    ret

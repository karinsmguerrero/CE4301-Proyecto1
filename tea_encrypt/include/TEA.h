#ifndef TEA_H
#define TEA_H
// Cifrado de 64 bits usando TEA
#include <stdint.h>

#define BLOCK_SIZE 8       // 64 bits = 8 bytes
#define MAX_BLOCKS 64      // máximo de bloques procesados
#define MAX_OUTPUT (MAX_BLOCKS * BLOCK_SIZE)

// Assembly function declaration
extern void tea_encrypt(uint32_t value[2], uint32_t key[4]);
extern void tea_decrypt(uint32_t value[2], uint32_t key[4]);

#endif
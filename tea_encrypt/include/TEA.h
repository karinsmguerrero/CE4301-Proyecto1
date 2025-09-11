#ifndef TEA_H
#define TEA_H

#include <stdint.h>

#define BLOCK_SIZE 8       // 64 bits = 8 bytes

// Assembly function declaration
extern void tea_encrypt(uint32_t value[2], uint32_t key[4]);
extern void tea_decrypt(uint32_t value[2], uint32_t key[4]);

void to_block(char *input, uint32_t *value);
void from_block(char *output, uint32_t *value);

void encrypt_message(char *input, int nblocks, uint32_t *key, uint32_t *encrypted);
void decrypt_message(uint32_t *encrypted, int nblocks, uint32_t *key, char *decrypted);

#endif
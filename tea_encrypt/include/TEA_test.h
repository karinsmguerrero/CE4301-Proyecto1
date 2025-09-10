#ifndef TEA_H
#define TEA_H
// Cifrado de 64 bits usando TEA
#include <stdint.h>
#include <stdio.h>

const int DELTA = 0x9e3779b9;

#define BLOCK_SIZE 8   // 64 bits = 8 chars
#define MAX_OUTPUT 256 // ajustar según longitud máxima de input

void to_ascii(const char *input, uint32_t *value);
void from_ascii(char *output, uint32_t *value);
void tea_encrypt(uint32_t v[2], const uint32_t key[4]);
void tea_decrypt(uint32_t v[2], const uint32_t key[4]);

#endif
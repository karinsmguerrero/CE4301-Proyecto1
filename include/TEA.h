#ifndef TEA_H
#define TEA_H
// Cifrado de 64 bits usando TEA
#include <stdint.h>
#include <stdio.h>

const int DELTA = 0x9e3779b9;
const uint32_t key[4] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x76543210};
const char input[] = "HOLA1234";

void to_ascii(const char *input, uint32_t *value);
void from_ascii(char *output, uint32_t *value);
void tea_encrypt(uint32_t v[2], const uint32_t key[4]);
void tea_decrypt(uint32_t v[2], const uint32_t key[4]);

#endif
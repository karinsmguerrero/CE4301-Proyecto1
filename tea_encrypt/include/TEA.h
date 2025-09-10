#ifndef TEA_H
#define TEA_H
// Cifrado de 64 bits usando TEA
#include <stdint.h>

#define BLOCK_SIZE 8   // 64 bits = 8 chars
#define MAX_OUTPUT 256 // ajustar según longitud máxima de input

uint32_t key[4] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x76543210};
//char input[] = "HOLA1234";
char input[] = "Mensaje de prueba para TEA";

void to_ascii(const char *input, uint32_t *value);
void from_ascii(char *output, uint32_t *value);

// Assembly function declaration
extern void tea_encrypt(uint32_t value[2], uint32_t key[4]);
extern void tea_decrypt(uint32_t value[2], uint32_t key[4]);

#endif
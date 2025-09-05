#include "../include/TEA.h"

void to_ascii(const char *input, uint32_t *value) {
    value[0] = 0;
    value[1] = 0;

    for (int i = 0; i < 8; i++) {
        char c = input[i];
        if (c == '\0') break;  // string end

        
        if (i < 4) {
            // Empaquetar los caracteres ASCII en un uint32_t
            // La máscara & 0xFF asegura que solo se guarda 1 byte por caracter.
            value[0] |= ((uint32_t)c & 0xFF) << (8 * i);
        } else {
            value[1] |= ((uint32_t)c & 0xFF) << (8 * (i - 4));
        }
    }
}


void from_ascii(char *output, uint32_t *value) {
    for (int i = 0; i < 4; i++) {
        output[i] = (value[0] >> (8 * i)) & 0xFF;
    }
    for (int i = 0; i < 4; i++) {
        output[i + 4] = (value[1] >> (8 * i)) & 0xFF;
    }
    output[8] = '\0'; // terminador nulo
}
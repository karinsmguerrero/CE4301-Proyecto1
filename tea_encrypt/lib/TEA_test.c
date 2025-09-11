#include "../include/TEA_test.h"

void tea_encrypt(uint32_t v[2], const uint32_t key[4])
{
    uint32_t v0 = v[0], v1 = v[1];
    uint32_t sum = 0;

    for (int i = 0; i < 32; ++i)
    { // 32 rondas
        sum += DELTA;
        v0 += ((v1 << 4) + key[0]) ^ (v1 + sum) ^ ((v1 >> 5) + key[1]);
        v1 += ((v0 << 4) + key[2]) ^ (v0 + sum) ^ ((v0 >> 5) + key[3]);
    }

    v[0] = v0;
    v[1] = v1;
}

// Descifrado de 64 bits usando TEA
void tea_decrypt(uint32_t v[2], const uint32_t key[4])
{
    uint32_t v0 = v[0], v1 = v[1];
    uint32_t sum = DELTA * 32;

    for (int i = 0; i < 32; ++i)
    { // 32 rondas
        v1 -= ((v0 << 4) + key[2]) ^ (v0 + sum) ^ ((v0 >> 5) + key[3]);
        v0 -= ((v1 << 4) + key[0]) ^ (v1 + sum) ^ ((v1 >> 5) + key[1]);
        sum -= DELTA;
    }
    v[0] = v0;
    v[1] = v1;
}

int get_message_length(char* message){
    int i = 0;
    char c = message[i];
    while(c != '\0'){
        i++;
        c = message[i];
    }
    return i;
}

void print_hex(const char *label, const uint32_t *data, int nblocks) {
    printf("%s", label);
    for (int i = 0; i < nblocks * 2; i++) {
        printf("%08X ", data[i]);
    }
    printf("\n");
}

// Empaqueta hasta 8 chars en 2 × uint32_t
static void to_block(const char *input, uint32_t *value) {
    value[0] = 0; value[1] = 0;
    for (int i = 0; i < 8; i++) {
        char c = input[i];
        if (c == '\0') break;
        if (i < 4)
            value[0] |= ((uint32_t)c & 0xFF) << (8 * i);
        else
            value[1] |= ((uint32_t)c & 0xFF) << (8 * (i - 4));
    }
}

// Desempaqueta 2 × uint32_t en 8 chars
static void from_block(char *output, const uint32_t *value) {
    for (int i = 0; i < 4; i++)
        output[i] = (value[0] >> (8 * i)) & 0xFF;
    for (int i = 0; i < 4; i++)
        output[i + 4] = (value[1] >> (8 * i)) & 0xFF;
    output[8] = '\0';
}

// Cifra todo el mensaje en bloques de 64 bits
int encrypt_message(const char *input, const uint32_t *key, uint32_t *encrypted) {
    int len = get_message_length(input);
    int nblocks = (len + BLOCK_SIZE - 1) / BLOCK_SIZE;
    for (int b = 0; b < nblocks; b++) {
        uint32_t block[2];
        to_block(&input[b * BLOCK_SIZE], block);
        tea_encrypt(block, key);
        encrypted[b * 2]     = block[0];
        encrypted[b * 2 + 1] = block[1];
    }
    return nblocks;
}

// Descifra bloques en texto
int decrypt_message(const uint32_t *encrypted, int nblocks, const uint32_t *key, char *decrypted) {
    for (int b = 0; b < nblocks; b++) {
        uint32_t block[2] = { encrypted[b * 2], encrypted[b * 2 + 1] };
        tea_decrypt(block, key);
        from_block(&decrypted[b * BLOCK_SIZE], block);
    }
    decrypted[nblocks * BLOCK_SIZE] = '\0';
    return nblocks * BLOCK_SIZE;
}


int main()
{
    uint32_t key[4] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x76543210};
    char input[] = "Mensaje de prueba para TEA";

    uint32_t encrypted[MAX_BLOCKS * 2];
    char decrypted[MAX_OUTPUT];

    printf("Original input: %s\n", input);

    int nblocks = encrypt_message(input, key, encrypted);
    print_hex("Encrypted blocks: ", encrypted, nblocks);

    decrypt_message(encrypted, nblocks, key, decrypted);
    printf("Decrypted text: %s\n", decrypted);

    return 0;
    
}
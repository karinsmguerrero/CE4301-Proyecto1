#include "../include/TEA_test.h"

void to_ascii(const char *input, uint32_t *value)
{
    value[0] = 0;
    value[1] = 0;

    for (int i = 0; i < 8; i++)
    {
        char c = input[i];
        if (c == '\0')
            break; // string end

        if (i < 4)
        {
            // Empaquetar los caracteres ASCII en un uint32_t
            // La máscara & 0xFF asegura que solo se guarda 1 byte por caracter.
            value[0] |= ((uint32_t)c & 0xFF) << (8 * i);
        }
        else
        {
            value[1] |= ((uint32_t)c & 0xFF) << (8 * (i - 4));
        }
    }
}

void from_ascii(char *output, uint32_t *value)
{
    for (int i = 0; i < 4; i++)
    {
        output[i] = (value[0] >> (8 * i)) & 0xFF;
    }
    for (int i = 0; i < 4; i++)
    {
        output[i + 4] = (value[1] >> (8 * i)) & 0xFF;
    }
    output[8] = '\0'; // terminador nulo
}

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

void encrypt_message(char *input, uint32_t* key, uint32_t* encrypted)
{
    uint32_t value[2];
    char block[BLOCK_SIZE];
    int i = 0, j = 0;

    while (input[i] != '\0')
    {
        // Copiar 8 chars al bloque
        for (j = 0; j < BLOCK_SIZE; j++)
        {
            if (input[i] != '\0'){
                block[j] = input[i];
                i++;
            }
            else
                block[j] = '\0'; // padding simple
        }

        // Convert from string to uint32_t[2]
        to_ascii(block, value);

        // Encriptar
        tea_encrypt(value, key);

        // Guardar resultado en "encrypted"
        encrypted[i - BLOCK_SIZE] = value[0];
        encrypted[i - BLOCK_SIZE + 1] = value[1];

        //from_ascii(&encrypted[i - BLOCK_SIZE], value);
    }
    encrypted[i] = '\0';
}

void decrypt_message(uint32_t *encrypted, uint32_t* key, char *decrypted) {
    uint32_t value[2];
    uint32_t block[BLOCK_SIZE];
    int i = 0, j = 0;

    while (encrypted[i] != '\0') {
        printf("In while");
        // Copiar 8 chars al bloque
        for (j = 0; j < BLOCK_SIZE; j++) {
            if (encrypted[i] != '\0'){
                block[j] = encrypted[i];
                i++;
            }
            else
                block[j] = 0;
        }


        // Desencriptar
        tea_decrypt(block, key);

        // Guardar resultado en "decrypted"
        from_ascii(&decrypted[i - BLOCK_SIZE], block);
    }
    decrypted[i] = '\0';
}


int main()
{
    printf("Testing tea_encrypt assembly function:\n");

    uint32_t key[4] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x76543210};
    char input[] = "Mensaje de prueba para TEA";

    // uint32_t key[4] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x76543210};
    // char input[] = "HOLA1234";
    
    uint32_t encrypted_message[MAX_OUTPUT];
    char decrypted_message[MAX_OUTPUT];

    encrypt_message(input, key, encrypted_message);
    int i = 0;
    while(encrypted_message[i] != '\0'){
    printf("%u \n", encrypted_message[i]);
    i++;
    }

    decrypt_message(encrypted_message, key, decrypted_message);
    printf("Mensaje desencriptado: %s \n", decrypted_message);

    /*char output[9];
    uint32_t value[2];
    to_ascii(input, value);

    printf("Valor original: \n");
    printf("%x \n", value[0]);
    printf("%x \n", value[1]);
    tea_encrypt(&value, &key);
    printf("Valor encriptado: \n");
    printf("%x \n", value[0]);
    printf("%x \n", value[1]);
    tea_decrypt(&value, &key);
    printf("Valor desencriptado: \n");
    printf("%x \n", value[0]);
    printf("%x \n", value[1]);

    from_ascii(output, value);
    printf("%s \n", output);*/

    return 0;
}
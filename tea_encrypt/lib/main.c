#include "../include/TEA.h"

// Simple implementation of basic functions since we're in bare-metal environment
void print_char(char c)
{
    // In a real bare-metal environment, this would write to UART
    // For now, this is just a placeholder
    volatile char *uart = (volatile char *)0x10000000;
    *uart = c;
}

void print_string(const char *str)
{
    while (*str)
    {
        print_char(*str++);
    }
}

void print_number(uint32_t num)
{
    char buffer[11];
    int i = 0;

    if (num == 0)
    {
        print_char('0');
        return;
    }

    while (num > 0 && i < 10)
    {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    while (i > 0)
    {
        print_char(buffer[--i]);
    }
}

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

void encrypt_message(char *input, uint32_t* key, uint32_t* encrypted)
{
    uint32_t value[2];
    char block[BLOCK_SIZE + 1];
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
        block[BLOCK_SIZE] = '\0';

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

int main()
{
    print_string("Testing tea_encrypt assembly function:\n");

    uint32_t key[4] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x76543210};
    char input[] = "Mensaje de prueba para TEA";

    // uint32_t key[4] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x76543210};
    // char input[] = "HOLA1234";
    
    uint32_t encrypted_message[MAX_OUTPUT];
    char decrypted_message[MAX_OUTPUT];

    print_string("Original input: ");
    print_string(input);
    print_char('\n');

    encrypt_message(input, key, encrypted_message);

    print_string("Encrypted text:\n");
    for(int i = 0; i < 4; i++){
        print_number(encrypted_message[i]);
        print_char('\n');
    }



    return 0;
}
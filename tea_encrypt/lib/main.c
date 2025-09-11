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

int get_message_length(char *message)
{
    int i = 0;
    char c = message[i];
    while (c != '\0')
    {
        i++;
        c = message[i];
    }
    return i;
}

void decToHex(uint32_t decimalNum)
{
    char hexDigits[8]; // Array to store hexadecimal digits
    int i = 0;

    // Handle the case of 0 separately
    if (decimalNum == 0)
    {
        print_string("0\n");
        return;
    }

    while (decimalNum > 0)
    {
        int remainder = decimalNum % 16;

        if (remainder < 10)
        {
            hexDigits[i] = remainder + '0'; // Convert 0-9 to character '0'-'9'
        }
        else
        {
            hexDigits[i] = remainder + 'A' - 10; // Convert 10-15 to 'A'-'F'
        }
        i++;
        decimalNum /= 16;
    }

    // Print the hexadecimal digits in reverse order
    for (int j = i - 1; j >= 0; j--)
    {
        print_char(hexDigits[j]);
    }
    print_char(' ');
}

void print_hex(uint32_t *data, int nblocks)
{
    for (int i = 0; i < nblocks * 2; i++)
    {
        decToHex(data[i]);
    }
    print_char('\n');
}

// Converts string to 2 uint32_t arrays
void to_block(char *input, uint32_t *value) {
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
// Converts uint32_t arrays to string
void from_block(char *output, uint32_t *value)
{
    for (int i = 0; i < 4; i++)
        output[i] = (value[0] >> (8 * i)) & 0xFF;
    for (int i = 0; i < 4; i++)
        output[i + 4] = (value[1] >> (8 * i)) & 0xFF;
    output[8] = '\0';
}

// Encrypts message into 64 bits 
void encrypt_message(char *input, int nblocks, uint32_t *key, uint32_t *encrypted)
{
    for (int b = 0; b < nblocks; b++)
    {
        uint32_t block[2];
        to_block(&input[b * BLOCK_SIZE], block);
        tea_encrypt(block, key);
        encrypted[b * 2] = block[0];
        encrypted[b * 2 + 1] = block[1];
    }
}

// Descifra bloques en texto
int decrypt_message(uint32_t *encrypted, int nblocks, uint32_t *key, char *decrypted)
{
    for (int b = 0; b < nblocks; b++)
    {
        uint32_t block[2] = {encrypted[b * 2], encrypted[b * 2 + 1]};
        tea_decrypt(block, key);
        from_block(&decrypted[b * BLOCK_SIZE], block);
    }
    decrypted[nblocks * BLOCK_SIZE] = '\0';
    return nblocks * BLOCK_SIZE;
}

void print_test(char *input, uint32_t *key)
{
    int len = get_message_length(input);
    int nblocks = (len + BLOCK_SIZE - 1) / BLOCK_SIZE;

    uint32_t encrypted[nblocks * 2];
    char decrypted[nblocks * 2];

    print_string(" -------------- BEGINING OF TEST -------------- \n");

    print_string("Original message: ");
    print_string(input);
    print_char('\n');

    print_string("Encrypted message: ");
    encrypt_message(input, nblocks, key, encrypted);
    print_hex(encrypted, nblocks);

    decrypt_message(encrypted, nblocks, key, decrypted);
    print_string("Decrypted message: ");
    print_string(decrypted);
    print_char('\n');

    print_string(" -------------- END OF TEST -------------- \n");
    print_char('\n');
}

int main()
{
    print_string("Testing encryption for full 64 bits block:\n");

    char input[] = "HOLA1234";
    uint32_t key[4] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x76543210};
    print_test(input, key);

    print_string("Testing encryption for incomplete 64 bits block:\n");

    char input2[] = "TEC";
    uint32_t key2[4] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x76543210};
    print_test(input2, key2);

    print_string("Testing encryption for multiple 64 bits blocks:\n");

    char input3[] = "Mensaje de prueba para TEA";
    uint32_t key3[4] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x76543210};
    print_test(input3, key3);

    return 0;
}
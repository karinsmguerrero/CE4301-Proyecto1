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



int main()
{
    print_string("Testing tea_encrypt assembly function:\n");


    char output[9];
    uint32_t value[2];

    print_string("Value to be encrypted: ");
    print_string(input);
    print_string("\n");

    // Convert string to ascii
    to_ascii(input, value);

    print_string("ASCII coded:\n");
    print_number(value[0]);
    print_char('\n');
    print_number(value[1]);
    print_char('\n');

    tea_encrypt(value, key);

    print_string("Encrypted result:\n");
    print_number(value[0]);
    print_char('\n');
    print_number(value[1]);
    print_char('\n');

    tea_decrypt(value, key);

    print_string("Decrypted result:\n");
    print_number(value[0]);
    print_char('\n');
    print_number(value[1]);
    print_char('\n');

    from_ascii(output, value);

    print_string("ASCII decoded:\n");
    print_string(output);

    return 0;
}
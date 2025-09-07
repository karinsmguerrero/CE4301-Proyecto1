#!/bin/bash

# Build script for C+assembly example
echo "Building C+assembly TEA encrytor..."

# Compile C source to object file
riscv64-unknown-elf-gcc \
    -march=rv32im \
    -mabi=ilp32 \
    -nostdlib \
    -ffreestanding \
    -g3 \
    -gdwarf-4 \
    -c \
    lib/main.c \
    -o build/main.o

if [ $? -ne 0 ]; then
    echo "C compilation failed"
    exit 1
fi

# Compile startup assembly to object file
riscv64-unknown-elf-gcc \
    -march=rv32im \
    -mabi=ilp32 \
    -nostdlib \
    -ffreestanding \
    -g3 \
    -gdwarf-4 \
    -c \
    startup.s \
    -o startup.o

if [ $? -ne 0 ]; then
    echo "Startup assembly compilation failed"
    exit 1
fi

# Compile math assembly source to object file
riscv64-unknown-elf-gcc \
    -march=rv32im \
    -mabi=ilp32 \
    -nostdlib \
    -ffreestanding \
    -g3 \
    -gdwarf-4 \
    -c \
    lib/TEA.s \
    -o build/TEA.o

if [ $? -ne 0 ]; then
    echo "TEA assembly compilation failed"
    exit 1
fi

# Link object files together
riscv64-unknown-elf-gcc \
    -march=rv32im \
    -mabi=ilp32 \
    -nostdlib \
    -ffreestanding \
    -g3 \
    -gdwarf-4 \
    startup.o \
    build/main.o \
    build/TEA.o \
    -T linker.ld \
    -o tea.elf

if [ $? -eq 0 ]; then
    echo "Build successful: tea.elf created"
    echo "Object files: main.o, TEA.o"
else
    echo "Linking failed"
    exit 1
fi
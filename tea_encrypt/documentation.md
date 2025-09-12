# Introducción

Este proyecto tiene como objetivo la implementación del algoritmo de encripción TEA, para mensajes o cadenas de caractéres, en una arquitectura RISC-V.


## Entorno de desarrollo RISC-V con QEMU y GDB

Para el entorno de desarrollo se utiliza un contenedor de Docker con QEMU y GBD para la depuración del código. El entorno permite el dessarollo y depuración de programas bare-metal en arquitectura RISC-V de 32 bits.

---

## 1. Archivos de configuración

- `Dockerfile` define la imagen que incluye el emulador QEMU y el toolchain RISC-V
- `run.sh` automatiza la construcción de la imagen y la ejecución del contenedor
- `tea_encrypt/build.sh` automatiza la compilación de la aplicación
- `tea_encrypt/run-qemu.sh` inicia qemu con el ejecutable de la aplicación y prepara la depuración

---

## 2. Inicio rápido

### Paso 1: Construir el contenedor
```bash
chmod +x run.sh
./run.sh
```

### Paso 2: Compilar aplicación
```bash
# Para el ejemplo de ensamblador puro
cd /home/rvqemu-dev/workspace/tea_encrypt
./build.sh

```

### Paso 3: Ejecutar con QEMU y depurar
```bash
# En una terminal: iniciar QEMU con servidor GDB
./run-qemu.sh

# En otra terminal: conectar GDB
docker exec -it rvqemu /bin/bash
cd /home/rvqemu-dev/workspace/tea_encrypt
gdb-multiarch tea.elf
```

---

## 3. Uso detallado

### Construcción del contenedor
El script `run.sh` construye la imagen `rvqemu` y crea un contenedor interactivo que monta el directorio del proyecto en `/home/rvqemu-dev/workspace`.

### Compilación
Se incluye un script `build.sh` que maneja la compilación automáticamente.

**Opciones de compilación utilizadas**:
- `-march=rv32im`: arquitectura RISC-V 32 bits con extensiones I y M
- `-mabi=ilp32`: ABI ILP32
- `-nostdlib -ffreestanding`: entorno bare-metal
- `-g`: información de depuración para GDB

### Ejecución y depuración
1. **QEMU**: `run-qemu.sh` inicia QEMU con servidor GDB en puerto 1234
2. **GDB**: Conectar desde otra terminal para depuración interactiva

**Comandos útiles de GDB**:
```gdb
target remote :1234    # Conectar al servidor GDB
break _start           # Punto de ruptura al inicio
break tea_encrypt      # Función para encriptar mensaje
break tea_decrypt      # Función para desencriptar mensaje
continue               # Continuar ejecución
layout asm             # Vista de ensamblador
layout regs            # Vista de registros
step                   # Ejecutar siguiente instrucción
info registers         # Mostrar registros
monitor quit           # Finalizar sesión
```

---

## 4. Detalles de los ejemplos

Para información específica sobre cada ejemplo, consultar:
- [`examples/asm-only/README.md`](examples/asm-only/README.md) - Ensamblador puro
- [`examples/c-asm/README.md`](examples/c-asm/README.md) - C + Ensamblador
- [`examples/README.md`](examples/README.md) - Información general

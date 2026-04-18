## CHIP-8 Emulator

This project is a CHIP-8 emulator/interpreter written in C, and built with SDL2 for graphics. Currently implements a subset of the CHIP-8 instruction set sufficient to run the IBM Logo ROM.

### About CHIP-8

Created by RCA engineer Joe Weisbecker in 1977 for the COSMAC VIP microcomputer, the CHIP-8 was intended as a simpler way to create programs and games. Instead of directly writing machine language, higher-level hexadecimal instructions could be interpreted on the fly by the CHIP-8 emulator/interpreter.

### Status

Work in progress. Currently implemented:
- Core CPU loop (fetch, decode, execute)
- 64x32 pixel display via SDL2
- Subset of the CHIP-8 instruction set

To-Do:
- Full instruction set
- Timers
- Keypad input
- Audio

### Technologies

- **C** - core emulator logic
- **SDL2** - graphics

### Building and running

#### Dependencies

- GCC or Clang
- SDL2
- Make

#### Linux (Ubuntu/Debian)

```bash
sudo apt install build-essential libsdl2-dev
make
./chip8
```

> **Note:** ROM path is currently hardcoded. CLI argument support coming soon.

#### Windows

On Windows, it is recommended to use WSL (Windows Subsystem for Linux) and follow the Linux instructions above. Here is a tutorial for [WSL installation](https://learn.microsoft.com/en-us/windows/wsl/install).

### **References**
- [Tobias V. I. Langhoff's high-level guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)
- [SDL2's Wiki](https://wiki.libsdl.org/SDL2/FrontPage)

### **Author**
- [Gabriel Luís Pinto Cecconello](https://www.linkedin.com/in/gabriel-cecconello/)

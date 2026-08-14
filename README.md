## CHIP-8 Emulator

[English](README.md) | [Português](README.pt-br.md)

A CHIP-8 emulator/interpreter written in C, using SDL2 for graphics and audio. It implements the full standard CHIP-8 instruction set.

### About CHIP-8

Created by RCA engineer Joe Weisbecker in 1977 for the COSMAC VIP microcomputer, CHIP-8 was intended as a simpler way to create programs and games. Instead of writing machine language directly, higher-level hexadecimal instructions could be interpreted on the fly by the CHIP-8 interpreter.

### Features

- Full standard CHIP-8 instruction set
- 64×32 monochrome 60Hz display
- Sound
- Hex keypad input mapped to a modern QWERTY layout

### Design notes

This emulator follows the standard CHIP-8 conventions, verified against Timendus' CHIP-8 test suite. Several instructions behave differently across interpreters such as CHIP-48 and SUPER-CHIP. Where these ambiguities exist, this project follows the behavior expected by the standard CHIP-8 test suite, so ROMs written against SUPER-CHIP quirks may not run correctly.

In addition, due to how the original CHIP-8 draws and erases sprites via XOR, some programs may exhibit sprite flicker.

### Technologies

- **C** — core emulator logic
- **SDL2** — graphics and audio

### Building and running

#### Dependencies

- GCC or Clang
- SDL2
- Make

#### Linux (Ubuntu/Debian)

```bash
sudo apt install build-essential libsdl2-dev
make
./chip8 <rom_path>

# example:
./chip8 roms/games/superpong.ch8
```

#### Windows

On Windows, it is recommended to use WSL (Windows Subsystem for Linux) and follow the Linux instructions above. Here is a tutorial for [WSL installation](https://learn.microsoft.com/en-us/windows/wsl/install).

### Keypad

The original CHIP-8 hex keypad is mapped to the left side of a QWERTY keyboard:

```
CHIP-8 Keypad       Keyboard
1 2 3 C             1 2 3 4
4 5 6 D      =>     Q W E R
7 8 9 E             A S D F
A 0 B F             Z X C V
```

### References

- [Tobias V. I. Langhoff's high-level guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)
- [SDL2 Wiki](https://wiki.libsdl.org/SDL2/FrontPage)
- [Timendus' CHIP-8 test suite](https://github.com/Timendus/chip8-test-suite) — test ROMs for verifying opcode behavior
- [John Earnest's CHIP-8 Archive](https://johnearnest.github.io/chip8Archive/) — public-domain games and programs

### Author

- [Gabriel Luís Pinto Cecconello](https://www.linkedin.com/in/gabriel-cecconello/)

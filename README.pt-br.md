## Emulador CHIP-8

[English](README.md) | [Português](README.pt-br.md)

Um emulador/interpretador de CHIP-8 escrito em C, usando SDL2 para gráficos e áudio. Implementa o conjunto de instruções padrão completo do CHIP-8.

### Sobre o CHIP-8

Criado pelo engenheiro da RCA Joe Weisbecker em 1977 para o microcomputador COSMAC VIP, o CHIP-8 foi concebido como uma forma mais simples de criar programas e jogos. Em vez de escrever linguagem de máquina diretamente, instruções hexadecimais de mais alto nível podiam ser interpretadas em tempo real pelo interpretador CHIP-8.

### Funcionalidades

- Conjunto de instruções padrão completo do CHIP-8
- Display monocromático de 64×32 a 60Hz
- Som
- Entrada via keypad hexadecimal mapeado para um teclado QWERTY moderno

### Notas de design

Este emulador segue as convenções padrão do CHIP-8, verificadas com a suíte de testes CHIP-8 do Timendus. Algumas instruções se comportam de forma diferente entre interpretadores como o CHIP-48 e o SUPER-CHIP. Onde essas ambiguidades existem, este projeto segue o comportamento esperado pela suíte de testes padrão do CHIP-8, de modo que ROMs escritas para as quirks do SUPER-CHIP podem não funcionar corretamente.

Além disso, devido à forma como o CHIP-8 original desenha e apaga sprites via XOR, alguns programas podem apresentar flicker de sprite.

### Tecnologias

- **C** — lógica central do emulador
- **SDL2** — gráficos e áudio

### Compilação e execução

#### Dependências

- GCC ou Clang
- SDL2
- Make

#### Linux (Ubuntu/Debian)

```bash
sudo apt install build-essential libsdl2-dev
make
./chip8 <rom_path>

# exemplo:
./chip8 roms/games/superpong.ch8
```

#### Windows

No Windows, recomenda-se usar o WSL (Windows Subsystem for Linux) e seguir as instruções de Linux acima. Aqui está um tutorial para a [instalação do WSL](https://learn.microsoft.com/en-us/windows/wsl/install).

### Keypad

O keypad hexadecimal original do CHIP-8 é mapeado para o lado esquerdo de um teclado QWERTY:

```
Keypad CHIP-8       Teclado
1 2 3 C             1 2 3 4
4 5 6 D      =>     Q W E R
7 8 9 E             A S D F
A 0 B F             Z X C V
```

### Referências

- [Guia de alto nível do Tobias V. I. Langhoff](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)
- [Wiki do SDL2](https://wiki.libsdl.org/SDL2/FrontPage)
- [Suíte de testes CHIP-8 do Timendus](https://github.com/Timendus/chip8-test-suite) — ROMs de teste para verificar o comportamento dos opcodes
- [CHIP-8 Archive do John Earnest](https://johnearnest.github.io/chip8Archive/) — jogos e programas de domínio público

### Autor

- [Gabriel Luís Pinto Cecconello](https://www.linkedin.com/in/gabriel-cecconello/)

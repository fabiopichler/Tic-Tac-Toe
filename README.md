# Tic-Tac-Toe

Um simples Jogo da Velha desenvolvido com a linguagem C e SDL 2 para Linux, Android, Windows, e Web (WebAssembly).

100% desenvolvido em C.

## Informações

Desenvolvido com:

* Linguagem de programação C;
* SDL 2

Compatível com:

* Linux;
* Android;
* Windows;
* Navegador Web (WebAssembly)

## Baixar/compilar

Baixar para Windows: [Tic-Tac-Toe_Windows_win32.zip](https://github.com/fabiopichler/Tic-Tac-Toe/releases/download/v1.0/Tic-Tac-Toe_Windows_win32.zip)

Para Linux, será preciso compilar a partir do código fonte, para isso, basta instalar as dependencias de desenvolvimento do ```SDL2```, ```SDL2_image```, ```SDL2_ttf```, assim como ```GCC``` e ```cmake```. Depois:

```
cmake .
make
```

Para WebAssembly, será preciso instalar o ```Emscripten``` ([emscripten.org](https://emscripten.org/)), depois:

```
emcmake cmake .
make
```

## Imagens

![Screenshot](/screenshots/screenshot_01.png?raw=true)

![Screenshot](/screenshots/screenshot_02.png?raw=true)

![Screenshot](/screenshots/screenshot_03.png?raw=true)

![Screenshot](/screenshots/screenshot_04.jpg?raw=true)

## Licença

MIT License

Copyright (c) 2020 Fábio Pichler

# Tic Tac Toe

Um simples Jogo da Velha desenvolvido com a linguagem C, OpenGL e SDL 2 para Linux, Android, Windows, e Web (WebAssembly e WebGL).

100% desenvolvido em C.

## Informações

Desenvolvido com:

* Linguagem de programação C;
* OpenGL 3.3 / OpenGL 2.1, OpenGL ES 3.0 / OpenGL ES 2.0 e WebGL 2.0 / WebGL 1.0;
* SDL 2

Compatível com:

* Linux;
* Android;
* Windows;
* Navegador Web (WebAssembly)

## Demo Online (WebAssembly)

Acessar a demonstração online com WebAssembly e WebGL: [https://projects.fabiopichler.net/tic-tac-toe-wasm/](https://projects.fabiopichler.net/tic-tac-toe-wasm/)

## Compilar

Para compilar a partir do código fonte, basta instalar as dependencias de desenvolvimento do ```SDL2```, ```SDL2_image```, ```SDL2_ttf```, assim como ```GCC``` e ```cmake```. Depois:

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

Copyright (c) 2020-2022 Fábio Pichler

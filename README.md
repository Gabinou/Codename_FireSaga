# Codename: FireSaga

## Introduction
This is the source code for a Strategy RPG game strongly inspired by Fire Emblem and Tear Ring Saga that I am developing.
I specifically aim to capture the spirit of the older games created by Shouzou Kaga.
Game title still pending.

There is a [game development blog](https://averagebear.game.blog/).

<!-- The game is available for sale at... -->


## Installation
The game will be available to play for free for anyone that wishes to compile the game from source, see instructions below.
Source code is free to own and distribute, see the [licence](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/LICENCE_SRC.md) for details.
The officially supported build makes use of Mace, my homemade build system. 

- [GitLab](https://gitlab.com/Gabinou/firesagamaker) 
- [GitHub](https://github.com/Gabinou/FireSaga) 
- [SourceForge](https://sourceforge.net/projects/codename-firesaga/) 

### Pre-requisites
- [Simple Direct Media Layer (SDL)](https://www.libsdl.org/download-2.0.php) development library
- [SDL_image](https://github.com/libsdl-org/SDL_image)
- [SDL_ttf](https://github.com/libsdl-org/SDL_ttf)
- [GCC](https://gcc.gnu.org/install/binaries.html) compiler suite
- [Windows only: MSYS2](https://www.msys2.org/) Linux terminal for windows + many packages
- [mace](https://github.com/Gabinou/mace) homemade C-only build system

#### Optional: Building with premake and make
- [Premake](https://premake.github.io/) makefile-generator

### Basic compilation instructions
0. Install all pre-requisites
	1. Windows only: You need to use the MSYS2 MinGW64 shell/command prompt/terminal! On this shell you can install all pre-requisites with `pacman`, exactly as on Linux.
1. Get the source code (with `git clone`), and put in in a directory such as `C:\firesaga`
2. Move to the source code folder (with `cd`)
3. Run:
```
mace
```
#### Optional: Building with premake and make
```
premake5 gmake
make
```

This should successfully create an executable file after a couple minutes of compilation.

## Software tools
This game is created using the free and open source (FOSS) [Simple DirectMedia Layer (SDL)](https://www.libsdl.org/index.php) library with the C programming language, version C99.
I used to write this game in C++11, but I grew to appreciate the simplicity of C over the bloated, "Object Oriented" C++.  
SDL is not exactly a game engine, but gives tools to developers that can be used to create one. 
This lets the developer retain low-level control.
This library was used to create many noteworthy games: Dwarf Fortress, FTL, Factorio, VVVVVV, N++...
I use the GCC compiler suite (MinGW on Windows), because FOSS tools are good. 
Now, I wish I could use the `tcc` compiler for everything, which might motivate me to write my own SDL-like library in the future.
I bought the pixel art tool [Aseprite](https://www.aseprite.org/) even though I could have compiled it from source and used it for free.
Because I didn't know... 

### Homemade libraries 
During the development of Codename:Firesaga, I used some third-party libraries. 
At first I was happy not to have to do all this work myself, but after I while I grew dissatisfied.
I mean, these libraries do not work _exactly_ as I want them to.
No choice then but develop my own:

- [mace: homemade C-only build system](https://github.com/Gabinou/mace)
- [tnecs: Tiny Entity-Component-System (ECS)](https://gitlab.com/Gabinou/tnecs)
- [darr: Dynamic Arrays](https://gitlab.com/Gabinou/darr)
- [noursmath: Linear Algebra, Pathfinding, periodic cycles...](https://gitlab.com/Gabinou/noursmath)

## Platforms
- Windows:
	Cross-compiled, tested with WINE.
- Linux:
	Native to Linux.
- Mac:
	Not yet supported
- iOS:
	Not yet supported
- Android:
	Not yet supported

## Licensing
The source code is made available with the GPLv3 [licence](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/LICENCE_SRC.md).
Please do not distribute compiled executables.

The assets are distributed under the [CC BY-NC-ND 3.0 licence](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/LICENCE_ASSETS.md). Please do not use assets for commercial purposes or distribute the assets if you modify them. Please provide credit if you use assets for non-commercial purposes.

Third-Party software libraries are distributed according to their respective licences.

## Contributing
All contributions are welcome. 

Feel free to contribute anything: units, weapons, maps, 2D sprites, scripts, designs...
Any inclusion will be credited.
Please note that I can only give you appreciation and credit for your efforts: this is a "fun during my free time" kind of project.

Code you write should contain the least amount of comments possible.
Code should be self-documenting, and avoid excessive comments.
Contributors should use expressive variable names that fit the style of the existing codebase. 

Code formatting is automated with [astyle](http://astyle.sourceforge.net/). 
Style options are in the `utils/style.txt` file.

If your idea is terrible, I will reject it: refer to the [roadmap](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/ROADMAP.md) and the [development blog](https://averagebear.game.blog/) for my thoughts on design.
If your idea is good, I will include it and give you credit. 
If your contributions are good, I don't care if you're nice.
If your contributions are bad, I still don't care if you're nice.

## Credits
:copyright: Average Bear Games, made by Gabriel Taillon
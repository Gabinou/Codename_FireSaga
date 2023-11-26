# Codename: FireSaga

## Introduction
This is the source code for a Strategy RPG game strongly inspired by Fire Emblem and Tear Ring Saga.
I aim to capture the spirit of the older games created by Shouzou Kaga.
Game title pending.
Everything is hancrafted: code, music, pixel art...

[Development blog](https://averagebear.game.blog/).

The game will be free to play, if compiled from source.
Source code is free to own and distribute, see the [licence](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/LICENCE_SRC.md).

- [GitLab](https://gitlab.com/Gabinou/firesagamaker) 
- [GitHub](https://github.com/Gabinou/Codename_FireSaga) 

Game assets (graphics, music) to be distributed separately.

<!-- The game is available for sale at... -->

## Features
- TODO: Sold **here**.
	- Or, play for free! Just compile the source code.
- Emulator-inspired quality-of-life improvements
	- Fast-forward button! Up to 300x faster on my machine (30000%).
	- TBD: Save state button. Not so sure.
- Handcrafted pixel-art
	- NES color palette only
- Developer features
	- Press button to reload .json files for units, weapons, tiles, maps! 

## Installation

### Pre-requisites
- [Simple Direct Media Layer (SDL)](https://www.libsdl.org/download-2.0.php) development library
	- [SDL_image](https://github.com/libsdl-org/SDL_image)
- [GCC](https://gcc.gnu.org/install/binaries.html) compiler suite
	- or [tcc](https://repo.or.cz/w/tinycc.git) compiler
- [Windows only: MSYS2](https://www.msys2.org/) Linux terminal for windows + many packages
- [Premake](https://premake.github.io/) makefile-generator
	- or [mace](https://github.com/Gabinou/mace) my homemade C-only build system


### Basic compilation instructions
0. Install all pre-requisites
	1. Windows only: You need to use the MSYS2 MinGW64 terminal! On it you can install all pre-requisites with `pacman`, exactly as on Linux.
1. Get the source code (with `git clone`)
2. Run:
```
premake5 gmake
make
```

Or, with my homemade build system `mace`:
```
mace
```
Without the `mace` convenience executable:
```
gcc macefile.c -o builder
./builder
```

This should successfully create an executable file after a couple minutes of compilation.

### Homemade libraries 
Homemade libraries developped specifically for use in Codename:Firesaga. 

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

Third-Party software libraries are distributed according to their respective licences.

## Contributing
All contributions are welcome. 
Any and all inclusions will be credited.

Code formatting is automated with [astyle](http://astyle.sourceforge.net/). 
Style options are in the `utils/style.txt` file.

## Credits
:copyright: Average Bear Games, made by Gabriel Taillon
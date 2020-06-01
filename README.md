

# Codename: FireSaga

## Introduction

This is the source code for a Strategy RPG game strongly inspired by Fire Emblem and Tear Ring Saga. I specifically aim to capture the spirit of the original creator of Fire Emblem, Shouzou Kaga. Game title still pending.

There is a [game development blog](https://averagebear.game.blog/).

<!-- The game is available for sale at... -->

For now, the game assets are not available on GitHub/GitLab. They will be made available later.

## Software tools

This game is created using the [Simple DirectMedia Layer (SDL)](https://www.libsdl.org/index.php) library with the C++ programming language. SDL is not exactly a game engine, but gives tools to developers that can be used to create one. The developer has control over many low-level operations. This library was used to create many noteworthy games, for example FTL, Factorio, VVVVVV and N++. I use the GCC compiler suite, because free and open source tools are good. I bought the pixel art tool [Aseprite](https://www.aseprite.org/) even though I could have compiled it from source and used it for free.

## Compiling from source
The game is available to play for anyone that wishes to compile the game from source. Source code is available from:

- [GitLab](https://gitlab.com/Gabinou/firesagamaker) 
- [GitHub](https://github.com/Gabinou/FireSaga) 

### Platforms
- Windows
	Codename:Firesaga is developped on/native to Windows.
- Linux:
	Not yet supported
- Mac:
	Not yet supported
- iOS:
	Not yet supported
- Android:
	Not yet supported


### Pre-requisites
- [CMake](https://cmake.org/download/) build system
- [Ninja](https://ninja-build.org/) build system
- [Simple Direct Media Layer (SDL)](https://www.libsdl.org/download-2.0.php)
- [GCC](https://gcc.gnu.org/install/binaries.html) compiler suite

### Basic compilation instructions
1. Get the source code, and put in in a directory such as `C:\firesaga` 
2. Create a `build` folder in the source code directory
3. On the command-line/shell/bash, enter the `build` directory and run the commands:
```
cmake -G "Ninja" ..
ninja
```
This should successfully create an executable file, ready for play.

## Licensing

The source code is made available under the Aseprite [End-User Licence Agreemend (EULA)](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/EULA.txt).

Third-Party software is distributed according to its individual licence, available in the [licences](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/licenses) directory.

## Credits
:copyright: Average Bear Games, made by Gabriel Taillon
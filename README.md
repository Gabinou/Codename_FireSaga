

# Codename: FireSaga

## Introduction

This is the source code for a Strategy RPG game strongly inspired by Fire Emblem and Tear Ring Saga. 
I specifically aim to capture the spirit of the original creator of Fire Emblem, Shouzou Kaga. 
Game title still pending.

There is a [game development blog](https://averagebear.game.blog/).

<!-- The game is available for sale at... -->

For now, the game assets are not available on GitHub/GitLab.
They will be made available later.

## Software tools

This game is created using the free and open source (FOSS) [Simple DirectMedia Layer (SDL)](https://www.libsdl.org/index.php) library with the C++ programming language, version C++11. 
SDL is not exactly a game engine, but gives tools to developers that can be used to create one. The developer has control over many low-level operations.
This library was used to create many noteworthy games, for example: FTL, Factorio, VVVVVV and N++.
I use the GCC compiler suite, because FOSS tools are good. 
I bought the pixel art tool [Aseprite](https://www.aseprite.org/) even though I could have compiled it from source and used it for free.
Because I didn't know... 

## Compiling from source
The game is available to play for free for anyone that wishes to compile the game from source. 
Please do not distribute .exe files: if you want to get it for free you should get the pleasure of compiling it yourself! 
Source code is available from:

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
0. Install all pre-requisites. 
1. Get the source code, and put in in a directory such as `C:\firesaga` 
2. Create a `build` folder in the source code directory
3. On the command-line/shell/bash/cmd.exe/terminal/command prompt, enter the `build` directory and run the commands:
```
cmake -G "Ninja" ..
ninja
```
This should successfully create an executable file after a couple minutes of compilation.

Note: Without the assets directory, the game will compile but most likely will crash upon execution. 
I am working on more error catching functions to prevent this.
I may also start distributing the assets...

## Licensing

The source code is made available under the Aseprite [End-User Licence Agreemend (EULA)](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/EULA.txt).

Third-Party software is distributed according to their respective licences, available in the [licences](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/licenses) directory.

## Credits
:copyright: Average Bear Games, made by Gabriel Taillon
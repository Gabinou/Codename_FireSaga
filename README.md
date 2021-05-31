# Codename: FireSaga

## Introduction

This is the source code for a Strategy RPG game strongly inspired by Fire Emblem and Tear Ring Saga that I am developing.
I specifically aim to capture the spirit of the older games created by Shouzou Kaga.
Game title still pending.

There is a [game development blog](https://averagebear.game.blog/).

<!-- The game is available for sale at... -->

## Software tools
This game is created using the free and open source (FOSS) [Simple DirectMedia Layer (SDL)](https://www.libsdl.org/index.php) library with the C programming language, version C99.
I used to write this game in C++11, but I grew to appreciate the simplicity of C over the bloated, "Object Oriented" C++.  
SDL is not exactly a game engine, but gives tools to developers that can be used to create one. 
This lets the developer retain low-level control.
This library was used to create many noteworthy games: Dwarf Fortress, FTL, Factorio, VVVVVV, N++...
I use the GCC (MinGW on Windows) compiler suite, because FOSS tools are good.
I bought the pixel art tool [Aseprite](https://www.aseprite.org/) even though I could have compiled it from source and used it for free.
Because I didn't know... 

## Compiling from source
The game will be available to play for free for anyone that wishes to compile the game from source, see instructions below.
Source code is free to own and distribute, see the [licence](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/LICENCE_SRC.md) for details.
I support two build systems: Cmake (Ninja optional) and GNU make (mingw32-make on Windows). 
Please do not distribute compiled executables (.exe files): if you want to get it for free you should get the pleasure of compiling it yourself! 
Source code is available from:

- [GitLab](https://gitlab.com/Gabinou/firesagamaker) 
- [GitHub](https://github.com/Gabinou/FireSaga) 
- [SourceForge](https://sourceforge.net/projects/codename-firesaga/) 

### Pre-requisites
- [Simple Direct Media Layer (SDL)](https://www.libsdl.org/download-2.0.php) development library
- [Linux: GCC](https://gcc.gnu.org/install/binaries.html)/[Windows: MinGW](https://sourceforge.net/projects/mingw/) compiler suite
- [CMake](https://cmake.org/download/) build system
- [Ninja](https://ninja-build.org/) meta-build system

### Basic compilation instructions
0. Install all pre-requisites (don't forget to add to path!)
1. Get the source code, and put in in a directory such as `C:\firesaga` 

#### Cmake
2. Create a `build` folder in the firesaga code directory
3. On the command-line/shell/bash/cmd.exe/terminal/command prompt, move to the `build` directory and run the commands:
```
cmake -G "Ninja" ..
ninja
```

#### make/mingw32-make
2. On the command-line/shell/bash/cmd.exe/terminal/command prompt, move to the Firesaga directory and run the command ```make``` on Linux or ```mingw32-make``` on Windows. 

This should successfully create an executable file after a couple minutes of compilation.
Note: Without the assets directory, the game will compile but most likely crash upon execution. 
I am working on more error catching functions to prevent this.

## Platforms
- Windows:
	Codename: Firesaga is developped on/native to Windows.
- Linux:
	Codename: Firesaga is developped on/native to Linux.
- Mac:
	Not yet supported
- iOS:
	Not yet supported
- Android:
	Compiles on Termux

## Licensing

The source code is made available to own, compile distribute, modify, etc... with the following [licence](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/LICENCE_SRC.md), inspired by the VVVVVV license and the Aseprite EULA.
Please do not distribute compiled executables.

The assets are distributed under the [CC BY-NC-ND 3.0 licence](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/LICENCE_ASSETS.md). Please do not use assets for commercial purposes or distribute the assets if you modify them. Please provide credit if you use assets for non-commercial purposes.

Third-Party software libraries are distributed according to their respective licences.

## Contributing

All contributions are welcome. 
Making any contribution implies your agreement with the [Contributor Licence](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/LICENCE_CONTRIBUTOR.md) derived from the [Aseprite Contributor License Agreement](https://github.com/aseprite/sourcecode/blob/master/cla.md).

Feel free to contribute anything: units, weapons, maps, 2D sprites, scripts, designs... Any inclusion will be credited.
Please note that I can only give you appreciation and credit for your efforts: this is a "fun during my free time" kind of project.

Code you write should contain the least amount of comments possible.
Code should be self-documenting, and avoid excessive comments.
Contributors should use expressive variable names that fit the style of the existing codebase. 

Code formatting should be automatized using [Astyle](http://astyle.sourceforge.net/) with the following input:

```
astyle --style=java --indent=spaces=4 --indent-switches --break-blocks --pad-oper --pad-comma --pad-header --unpad-paren  --align-pointer=middle --align-reference=middle --add-braces --add-one-line-braces --attach-return-type --convert-tabs --suffix=none --recursive "include/*.hpp" "src/*.cpp" "test/*.cpp" "test/*.hpp"
``` 

Add this line to your build system before the compilation step.


If your idea is terrible, I will reject it: refer to the [roadmap](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/ROADMAP.md) and the [development blog](https://averagebear.game.blog/) for my thoughts on design.
If your idea is good, I will include it and give you credit. 
If your contributions are good, I don't care if you're nice.
If your contributions are bad, I still don't care if you're nice.


## Credits
:copyright: Average Bear Games, made by Gabriel Taillon
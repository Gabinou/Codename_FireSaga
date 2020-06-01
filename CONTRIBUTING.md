
# Contributing

All code contributions are welcome. Making any contribution implies your agreement with the [Contributor Licence](https://gitlab.com/Gabinou/firesagamaker/-/blob/master/ROADMAP.md) derived from the [Aseprite Contributor License Agreement](https://github.com/aseprite/sourcecode/blob/master/cla.md).

Code you write should contain the least amount of comments possible. Code should be self-documenting, to avoid writing additional documentation. Contributors should use expressive variable names that fit the style of the existing codebase. 

Code formatting should can be automatized using [Astyle](http://astyle.sourceforge.net/) with the following input:

```
astyle --style=java --indent=spaces=4 --indent-switches --break-blocks --pad-oper --pad-comma --pad-header --unpad-paren  --align-pointer=middle --align-reference=middle --add-braces --add-one-line-braces --attach-return-type --convert-tabs --suffix=none --recursive "include/*.hpp" "src/*.cpp" "test/*.cpp" "test/*.hpp"
``` 

Add this line to your build system before the compilation step.

If your idea is terrible, I will reject it. If your idea is good, I will include it and give you credit.
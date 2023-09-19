#!/bin/sh

## Building all makefiles
premake5 gmake

## Replace '@echo $(notdir $<)' with '@echo $<' in sota.make
# 'make' print full path to source when building
sed -i 's/@echo $(notdir $<)/@echo $</g' sota.make

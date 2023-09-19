#!/bin/sh

# style.sh
# Copyright (C) Gabriel Taillon, 2023
# Automatic formatting with astyle 3.2.1 for mace 

astyle --options=utils/style.txt --verbose --recursive "src/*" "include/*" "test/*" "names/*"
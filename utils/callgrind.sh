#!/bin/bash
# Written by Gabriel Taillon on August 5th 2022
# Make a call graph, which function was called by who and how many times

valgrind --tool=callgrind ~/Sync/Firesaga/A_build/CodenameFireSaga_premake
gprof2dot --format=callgrind --output=out.dot "$1"
dot -Tpng out.dot -o graph.png

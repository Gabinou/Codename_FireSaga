#!/bin/bash
# Written by Gabriel Taillon on August 9th 2022
# Profiling SotA with perf

sudo perf record -F 99 -g ~/firesaga/build/sota
perf script | gprof2dot.py -f perf | dot -Tpng -o output.png

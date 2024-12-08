#!/bin/bash
# Written by Gabriel Taillon on August 9th 2022
# Profiling SotA with perf

# Need to run those commands manually for some reason...
SDL_AUDIODRIVER=pipewire perf record -F 99 -g /home/gabinours/firesaga/build/sota
perf script | gprof2dot -f perf | dot -Tpng -o output.png

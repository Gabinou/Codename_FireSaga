#!/bin/bash
# Written by Gabriel Taillon on August 9th 2022
# Profiling SotA with perf

# Need to run those commands manually.
# --- callgraph ---
SDL_AUDIODRIVER=pipewire perf record -F 99 -g /home/gabinours/firesaga/build/sota
perf script | gprof2dot -f perf | dot -Tpng -o output.png

# --- cache misses ---
perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations,L1-dcache-loads,L1-dcache-load-misses ./sota

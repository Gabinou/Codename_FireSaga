#!/bin/bash
# Written by Gabriel Taillon on August 9th 2022
# Profiling SotA with perf

sudo perf record -F 99 -g ~/Sync/Firesaga/A_build/CodenameFireSaga_premake
sudo perf script > perf.script
~/Repos/FlameGraph/stackcollapse-perf.pl perf.script | ~/Repos/FlameGraph/flamegraph.pl > flamegfraph.svg

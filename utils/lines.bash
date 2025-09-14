#!/bin/bash

find include src -name "*.h" -o -name "*.c" | xargs wc -l

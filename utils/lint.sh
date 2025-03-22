#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

## Lint all files
cppcheck --enable=warning --std=c99 -I "$SCRIPT_DIR"/../include  --check-level=exhaustive --force "$SCRIPT_DIR"/../src/

#!/usr/bin/bash

# Example: 
# - ls *.aseprite -1 | xargs -n1 ./export_layers.bash
# - find -type f -name "*.aseprite" | xargs -n1 -d '\n' ./export_layers.bash

if [ -z "$1" ]
    then
        echo "Input one filename"
        exit 1
fi

# Remove color from input (to use with ls *.aseprite)
stripped=$(echo $1 | sed -r "s/\x1B\[([0-9]{1,3}(;[0-9]{1,2};?)?)?[mGK]//g")

# Strip extension from filename
noext=$(basename "$stripped" .aseprite)

# Export 
echo "Exporting $stripped palette to $noext.hex"
aseprite -b --all-layers "$stripped" --script /home/gabinours/.config/aseprite/scripts/export_palette2hex.lua

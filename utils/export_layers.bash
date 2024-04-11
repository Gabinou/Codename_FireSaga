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
dir=$(dirname "$stripped")

# Export canvas with aseprite
echo "Exporting $stripped canvas to $dir/$noext.png"
aseprite -b --all-layers "$stripped" --oneframe --ignore-layer Background --save-as "$dir""/""$noext".png

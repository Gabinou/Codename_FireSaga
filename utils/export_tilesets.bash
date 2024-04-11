#!/usr/bin/bash

# Example: 
# - ls *.aseprite -1 | xargs -n1 ./export_tilesets.bash
# - find -type f -name "*.aseprite" | xargs -n1 ./export_tilesets.bash

if [ -z "$1" ]
    then
        echo "Input one filename"
        exit 1
fi

# Strip color from input (to use with ls *.aseprite)
stripped=$(echo $1 | sed -r "s/\x1B\[([0-9]{1,3}(;[0-9]{1,2};?)?)?[mGK]//g")

# Strip extension from filename
noext=$(echo $stripped | cut -f 1 -d ".")

# Export tileset with aseprite
echo "Exporting $stripped tileset to $noext.png"
aseprite -b --export-tileset "$stripped" --sheet-columns 8 --sheet "$noext".png --data dump.json

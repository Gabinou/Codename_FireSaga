#!/usr/bin/bash

# Example: 
# - ls *.aseprite -1 | xargs -n1 ./palette_switch.bash
# - find -type f -name "*.aseprite" | xargs -n1 ./palette_switch.bash

if [ -z "$1" ]
    then
        echo "Input one filename"
        exit 1
fi

# Remove color from input (to use with ls *.aseprite)
stripped=$(echo $1 | sed -r "s/\x1B\[([0-9]{1,3}(;[0-9]{1,2};?)?)?[mGK]//g")

# Switch color mode to RGB
echo "Switching to RGB color mode: $stripped"
aseprite "$stripped" -b --color-mode rgb --save-as "$stripped"

# Change palette
echo "Switching palette to homemade sota palette: $stripped"
aseprite "$stripped" -b --palette /home/gabinours/firesaga/assets/palettes/Palette_sota.aseprite  --save-as "$stripped"

echo "Switching to indexed color mode: $stripped"
aseprite "$stripped" -b --color-mode indexed --save-as "$stripped"

#!/usr/bin/bash

# Example: ls *.aseprite -1 | xargs -n1 ./palette_switch.bash

if [ -z "$1" ]
    then
        echo "Input one filename"
        exit 1
fi

# Remove color from input (to use with ls *.aseprite)
STRIPPED=$(echo $1 | sed -r "s/\x1B\[([0-9]{1,3}(;[0-9]{1,2};?)?)?[mGK]//g")

# Switch color mode to RGB
echo "Switching to RGB color mode: $STRIPPED"
aseprite "$STRIPPED" -b --color-mode rgb --save-as "$STRIPPED"

# Change palette
echo "Switching palette to homemade sota palette: $STRIPPED"
aseprite "$STRIPPED" -b --palette /home/gabinours/firesaga/assets/palettes/Palette_sota.aseprite  --save-as "$STRIPPED"

echo "Switching to indexed color mode: $STRIPPED"
aseprite "$STRIPPED" -b --color-mode indexed --save-as "$STRIPPED"

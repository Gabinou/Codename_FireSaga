#!/bin/bash

# Zipping all assets to assets.bsa file
zip_name=$(head -n 1 ../zip_name)
assets_folder="../assets"
echo "Zipping $assets_folder into $zip_name"

zip -r "../"$zip_name $assets_folder
#!/bin/bash

# Reading zip archive name
# Note: name is in .h file with REGISTER_ENUM() to be used in code
#   1. read name
zip_name=$(head -n 1 ../names/zip_archive.h)
#   2. Filter REGISTER_ENUM()

echo "Zipping $assets_folder into $zip_name"

# Reading folders to zip
assets_folder="../assets"

# Zipping all assets to $zip_name file
zip -r "../"$zip_name $assets_folder
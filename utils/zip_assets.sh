#!/bin/bash

# Getting path to current script
scriptpath=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Filtering REGISTER_ENUM() from input variable
# Note: names in .h files to be used in code also
filter_name() {
    out=$1
    out=${out#*(}   
    out=${out%)}   
    echo $out
}

# Reading zip archive name
# NOTE: breaks if REGISTER_ENUM() not on 1st line
zip_name=$(head -n 1 $(realpath "$scriptpath/../names/zip_archive.h"))
zip_name=$(filter_name $zip_name)
echo "Archive: $zip_name"

# Reading folders to zip + zipping
cd $scriptpath"/.."
while IFS= read -r line; do
    # NOTE: breaks if REGISTER_ENUM() don't start from 1st line
    line=$(filter_name $line)

    # Zip dir only if it exists
    if [ -d "$line" ]; then
        echo "Zipping $line -> $zip_name"

        zip -ruq9 $zip_name $line
    fi
done < $(realpath "$scriptpath/../names/zip_folders.h")
# Zip is dumb workardound noop:
#   zip -u flag outputs non-zero if no file is updated, UNLESS
#   some line runs after it I guess.
:

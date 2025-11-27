#!/bin/bash

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
zip_name=$(head -n 1 $(realpath "$scriptpath/../names/zip_archive.h"))
zip_name=$(filter_name $zip_name)
echo "Archive: $zip_name"

# Reading folders to zip + zipping
cd $scriptpath"/.."
while IFS= read -r line; do
    line=$(filter_name $line)

    # Zip dir only if it exists
    # find . -type d -name $line"aa" > /dev/null && echo Zipping $line && zip -qr9 $zip_name $line > %1
    # find . -type d -name $line > /dev/null && zip -qr9 $zip_name $line > %1
    # find . -type d -name $line > /dev/null || echo "Zipping $line"
done < $(realpath "$scriptpath/../names/zip_folders.h")

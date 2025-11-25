#!/bin/bash


scriptpath=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
echo scriptpath $scriptpath

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
while IFS= read -r line; do
    line=$(filter_name $line)

    echo "Zipping $line"
    zip -qr9 $(realpath "$scriptpath/../$zip_name") $(realpath "$scriptpath/../$line")
done < $(realpath "$scriptpath/../names/zip_folders.h")

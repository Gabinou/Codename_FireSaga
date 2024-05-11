#!/bin/bash

# Push assets to /mnt/WDRed12
rsync -rl --info=progress2 --delete /mnt/WDRed12/firesaga_assets ~/firesaga/assets
# Push assets to /mnt/WDBlue1z
rsync -rl --info=progress2 --delete /mnt/WDBlue1/firesaga_assets ~/firesaga/assets

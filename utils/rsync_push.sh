#!/bin/bash

# Push assets to /mnt/WDRed12
rsync -rl --info=progress2 --delete ~/firesaga/assets/* /mnt/WDRed12/firesaga_assets
# Push assets to /mnt/WDBlue1z
rsync -rl --info=progress2 --delete ~/firesaga/assets/* /mnt/WDBlue1/firesaga_assets

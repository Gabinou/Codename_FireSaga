cd #!/bin/bash
# Written by Gabriel Taillon on August 5th 2022
# Record screen for SotA

SotA_res_x=1600
SotA_res_y=800
Screen_res_x=1920
Screen_res_y=1080

# Game in middle of screen
plus_x=$(($Screen_res_x/2 - $SotA_res_x/2))
plus_y=$(($Screen_res_y/2 - $SotA_res_y/2))

ffmpeg -f x11grab -s "$SotA_res_x"x"$SotA_res_y" -i :0.0+$plus_x,$plus_y ~/Notes/FireSaga/blog/gifs/raw/out.mkv

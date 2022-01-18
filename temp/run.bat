@echo off
echo Compiling...
gcc crop.c seamcarving.c c_img.c -o crop -lm
crop
python make_gif.py
pause
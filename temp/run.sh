#! /bin/bash +x

echo "Compiling..."
gcc crop.c seamcarving.c c_img.c -o crop -lm
./crop
python3 make_gif.py

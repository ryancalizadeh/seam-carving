import os
import glob
import shutil

try:
    import numpy as np
except ImportError:
    os.system("pip3 install numpy")

import numpy as np

try:
    from PIL import Image
except ImportError:
    os.system("pip3 install Pillow")


from PIL import Image


def read_2bytes(f):
    bytes = f.read(2)  # [int(f.read(1)), int(f.read(1))]
    return int.from_bytes(bytes, byteorder="big")


def read_image(filename):
    f = open(filename, "rb")
    height = read_2bytes(f)
    width = read_2bytes(f)
    image = Image.new("RGB", (width, height))
    bytes = f.read()
    for i in range(height):
        for j in range(width):
            image.putpixel(
                (j, i),
                (
                    bytes[3 * (i * width + j) + 0],
                    bytes[3 * (i * width + j) + 1],
                    bytes[3 * (i * width + j) + 2],
                ),
            )

    return image


print("Converting bins to pngs...")

for filename in glob.glob(os.path.join(os.getcwd(), "cropped_images", "bins", "*.bin")):
    im = read_image(filename)
    savename = os.path.basename(filename)
    im.save(
        os.path.join(os.getcwd(), "cropped_images", savename.rsplit(".", 1)[0] + ".png")
    )

print("Making GIF...")

im = Image.open("cropped_images/img0.png")

width, height = im.size

images = []

for i in range(1, width):
    im2 = Image.open("cropped_images/img" + str(i) + ".png")
    w, h = im2.size

    data = np.asarray(im2, dtype="uint8")
    data = data.tolist()

    for row in data:
        for i in range(0, (width - w)):
            row.append([255, 255, 255])

    data = np.array(data, dtype="uint8")

    im2 = Image.fromarray(data)
    images.append(im2)

im.save(
    "output.gif",
    save_all=True,
    append_images=images,
    optimize=True,
    duration=30,
    loop=0,
)

shutil.rmtree("cropped_images")

print("Done! See output.gif for result")

"""
images = []

for i in range(0, 506):
    images.append(Image.open("img" + str(i) + ".png"))

images[0].save(
    "test.gif",
    save_all=True,
    append_images=images[1:],
    optimize=True,
    duration=30,
    loop=0,
)
"""

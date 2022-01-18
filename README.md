# seam-carving
Calculates and removes minimum energy seam from image using Dijkstra's algorithm to remove undesirable parts of image.

The file seamcarving.c calculates the dual-gradient energy function for an image, creates a new image from the energy values, and determines the minimum cost path from the top to bottom of the energy image using dynamic programming. The path is then removed from the image.

![output](https://user-images.githubusercontent.com/31375351/149880080-7863436e-856c-4a8c-a2d5-a92472cd2201.gif)
In the gif above, the image repeatedly has the minimum energy seam removed until the entire image is gone.


Submitted as part of coursework for ESC190.


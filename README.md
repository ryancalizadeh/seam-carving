# seam-carving
Calculates and removes minimum energy seam from image using Dijkstra's algorithm to remove undesirable parts of image.

The file seamcarving.c calculates the dual-gradient energy function for an image, creates a new image from the energy values, and determines the minimum cost path from the top to bottom of the energy image using dynamic programming. The path is then removed from the image.


Submitted as part of coursework for ESC190.


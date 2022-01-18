# seam-carving
Calculates and removes minimum energy seam from image using Dijkstra's algorithm to remove undesirable parts of image.

The file seamcarving.c calculates the dual-gradient energy function for an image, creates a new image from the energy values, and determines the minimum cost path from the top to bottom of the energy image using dynamic programming. The path is then removed from the image.

![image](https://user-images.githubusercontent.com/31375351/149880406-c096fe2e-eab6-4cfa-98ee-a95b31c05b08.png)
![image](https://user-images.githubusercontent.com/31375351/149880423-2da8c97c-a753-4c6b-a55a-439f196c7bc8.png)

The original image (Top) and the energy image with minimum energy path (Bottom) 

![output](https://user-images.githubusercontent.com/31375351/149880080-7863436e-856c-4a8c-a2d5-a92472cd2201.gif)

In the gif above, the image repeatedly has the minimum energy seam removed until the entire image is gone.


Submitted as part of coursework for ESC190.


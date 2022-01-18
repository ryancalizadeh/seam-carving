#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "c_img.h"
#include "seamcarving.h"

int main()
{
    struct rgb_img *im;
    struct rgb_img *cur_im;
    struct rgb_img *grad;
    double *best;
    int *path;

#if defined(_WIN32)
    mkdir("cropped_images");
    mkdir("cropped_images/bins");
#else
    mkdir("cropped_images", 0777);
    mkdir("cropped_images/bins", 0777);
#endif

    char binName[255] = "";
    printf("Enter source bin file (include file extension): ");
    scanf("%s", binName);

    read_in_img(&im, binName);
    int WIDTH = im->width;

    printf("Carving Seams...\n");

    for (int i = 0; i < WIDTH - 1; i++)
    {
        calc_energy(im, &grad);
        dynamic_seam(grad, &best);
        recover_path(best, grad->height, grad->width, &path);
        remove_seam(im, &cur_im, path);

        char filename[200];
        sprintf(filename, "cropped_images/bins/img%d.bin", i);
        write_img(cur_im, filename);

        destroy_image(im);
        destroy_image(grad);
        free(best);
        free(path);
        im = cur_im;
    }
    destroy_image(im);

    printf("Done!\n");

    return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "c_img.h"
#include "seamcarving.h"

//Part 1: Dual-Gradient Energy Function

double min3(double a, double b, double c) {
	if(a < b && a < c) {
		return a;
	} else if (b < c) {
		return b;
	}
    return c;
}

double min2(double a, double b) {
    if(a <= b) {
        return a;
    } else {
        return b;
    }
}

int calc_x_energy(struct rgb_img *im, int y, int x, int w) {
    uint8_t r_l, r_r, r, g_l, g_r, g, b_l, b_r, b;
    if (x == 0) {
        r_l = get_pixel(im, y, w - 1, 0);
        r_r = get_pixel(im, y, x + 1, 0);
        g_l = get_pixel(im, y, w - 1, 1);
        g_r = get_pixel(im, y, x + 1, 1);
        b_l = get_pixel(im, y, w - 1, 2);
        b_r = get_pixel(im, y, x + 1, 2);
    } else if (x == w - 1) {
        r_l = get_pixel(im, y, x - 1, 0);
        r_r = get_pixel(im, y, 0, 0);
        g_l = get_pixel(im, y, x - 1, 1);
        g_r = get_pixel(im, y, 0, 1);
        b_l = get_pixel(im, y, x - 1, 2);
        b_r = get_pixel(im, y, 0, 2);
    } else {
        r_l = get_pixel(im, y, x - 1, 0);
        r_r = get_pixel(im, y, x + 1, 0);
        g_l = get_pixel(im, y, x - 1, 1);
        g_r = get_pixel(im, y, x + 1, 1);
        b_l = get_pixel(im, y, x - 1, 2);
        b_r = get_pixel(im, y, x + 1, 2);
    }
    int rx = r_r - r_l;
    int gx = g_r - g_l;
    int bx = b_r - b_l;
    return rx * rx + gx * gx + bx * bx;
}

int calc_y_energy(struct rgb_img *im, int y, int x, int h) {
    uint8_t r_u, r_d, g_u, g_d, b_u, b_d;
    if (y == 0) {
        r_u = get_pixel(im, h - 1, x, 0);
        r_d = get_pixel(im, y + 1, x, 0);
        g_u = get_pixel(im, h - 1, x, 1);
        g_d = get_pixel(im, y + 1, x, 1);
        b_u = get_pixel(im, h - 1, x, 2);
        b_d = get_pixel(im, y + 1, x, 2);
    } else if (y == h - 1) {
        r_u = get_pixel(im, y - 1, x, 0);
        r_d = get_pixel(im, 0, x, 0);
        g_u = get_pixel(im, y - 1, x, 1);
        g_d = get_pixel(im, 0, x, 1);
        b_u = get_pixel(im, y - 1, x, 2);
        b_d = get_pixel(im, 0, x, 2);
    } else {
        r_u = get_pixel(im, y - 1, x, 0);
        r_d = get_pixel(im, y + 1, x, 0);
        g_u = get_pixel(im, y - 1, x, 1);
        g_d = get_pixel(im, y + 1, x, 1);
        b_u = get_pixel(im, y - 1, x, 2);
        b_d = get_pixel(im, y + 1, x, 2);
    }
    int ry = r_u - r_d;
    int gy = g_u - g_d;
    int by = b_u - b_d;
    return ry * ry + gy * gy + by * by;
}

void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    size_t h = im->height;
    size_t w = im->width;
    create_img(grad, h, w);
    (*grad)->height = h;
    (*grad)->width = w;
    int x_ener, y_ener;
    uint8_t ener_store;
    double ener;
    for (int i = 0; i < h; i++) { // Get all the x and y energies
        for (int j = 0; j < w; j++) {
            x_ener = calc_x_energy(im, i, j, w);
            y_ener = calc_y_energy(im, i, j, h);
            ener = sqrt((double)(x_ener + y_ener));
            ener_store = (uint8_t) (ener/10);
            set_pixel(*grad, i, j, ener_store, ener_store, ener_store);
        }
    }
}

//Part 2

double carve(double *data, int i, int j, int h, int w) {
    if (i == 0){
        return data[j];
    } else if (j == 0 && w >= 2) {
        return data[i * w + j] + min2(carve(data, i-1, j, h, w), carve(data, i-1, j+1, h, w));
    } else if (j == w - 1 && w >= 2) {
        return data[i * w + j] + min2(carve(data, i-1, j-1, h, w), carve(data, i-1, j, h , w));
    } else if (w == 1) {
        return data[i * w + j] + carve(data, i-1, j, h, w);
    } else {
        return data[i * w + j] +
               min3(carve(data, i - 1, j - 1, h, w), carve(data, i - 1, j, h, w), carve(data, i - 1, j + 1, h, w));
    }
}

void carve_no_rec(double *res, double *input, int i, int j, int h, int w) {
    if (i == 0){
        res[i * w + j] = input[i * w + j];
    } else if (j == 0 && w >= 2) {
        res[i * w + j] = input[i * w + j] + min2(res[(i - 1) * w + (j)], res[(i - 1) * w + (j + 1)]);
    } else if (j == w - 1 && w >= 2) {
        res[i * w + j] = input[i * w + j] + min2(res[(i - 1) * w + (j - 1)], res[(i - 1) * w + (j)]);
    } else if (w == 1) {
        res[i * w + j] = input[i * w + j] + res[(i - 1) * w + (j)];
    } else {
        res[i * w + j] = input[i * w + j] + min3(res[(i - 1) * w + (j - 1)], res[(i - 1) * w + (j)], res[(i - 1) * w + (j + 1)]);
    }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr) {
    int h = grad->height;
    int w = grad->width;
    double *temp_arr = (double *)malloc(h * w * sizeof(double));
    (*best_arr) = (double *)malloc(h * w * sizeof(double));
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            temp_arr[i * w + j] = get_pixel(grad, i, j, 0);
            carve_no_rec(*best_arr, temp_arr, i, j, h, w);
            double test = (*best_arr)[i * w + j];
            //printf("%f\t", test);
        }
        //printf("\n");
    }
}

void print_double_arr(struct rgb_img *grad, double **path) {
    int height = grad->height;
    int width = grad->width;
    double test;
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            test = (*path)[i * width + j];
            printf("%f\t", test);
        }
        printf("\n");
    }
}

//Part 3
int min_row(double *best, int height, int width) {
    double min_ind = 1000000;
    double min_num = 1000000;
    for (int j = 0; j < width; j++) {
        if (best[height * width + j] < min_num) {
            min_num = best[height * width + j];
            min_ind = j;
        }
    }
    return (int)min_ind;
}

int min_ind_2(double a, int a_i, double b, int b_i)
{
    if (a <= b)
    { return a_i; }
    else
    { return b_i; }
}

int min_ind_3(double a, double b, double c, double a_i)
{
    if (a <= b && a <= c)
    { return a_i; }
    else if (b <= a && b <= c)
    { return a_i + 1; }
    else
    { return a_i + 2; }
}

int find_ind(double *best, int row, int col, int width) {
    int adj_ind = row * width + col;
    if (width == 1)
    { return row; }
    else if (col == 0)
    { return min_ind_2(best[adj_ind], col, best[adj_ind+1], col+1); }
    else if (col == width - 1)
    { return min_ind_2(best[adj_ind-1], col-1, best[adj_ind], col); }
    else
    { return min_ind_3(best[adj_ind - 1], best[adj_ind], best[adj_ind + 1], col - 1); }
}

void print_int_arr(int **path, int length) {
    for(int i = 0; i < length; i++){
        printf("%d", (*path)[i]);
        printf(" ");
    }
}

void recover_path(double *best, int height, int width, int **path){
    (*path) = (int *)malloc(height * sizeof(int));
    (*path)[height - 1] = 10000;
    for (int i = height - 1; i >= 0; i--) {
        if (i == height - 1) {
            (*path)[height - 1] = min_row(best, height - 1, width);
        }
        else {
            (*path)[i] = find_ind(best, i, (*path)[i + 1], width);
        }
    }
    //print_int_arr(path, height);
}


//Part 4

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path) {
    size_t height = src->height;
    size_t width = src->width - 1;
    create_img(dest, height, width);
    int after_j = 0; //false
    int r, g, b;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == path[i] && after_j == 0) {
                after_j = 1;
                r = get_pixel(src, i, j + 1, 0);
                g = get_pixel(src, i, j + 1, 1);
                b = get_pixel(src, i, j + 1, 2);
                set_pixel(*dest, i, j, r, g, b);
            }
            else if (after_j == 0){
                r = get_pixel(src, i, j, 0);
                g = get_pixel(src, i, j, 1);
                b = get_pixel(src, i, j, 2);
                set_pixel(*dest, i, j, r, g, b);
            }
            else if (after_j == 1) {
                r = get_pixel(src, i, j + 1, 0);
                g = get_pixel(src, i, j + 1, 1);
                b = get_pixel(src, i, j + 1, 2);
                set_pixel(*dest, i, j, r, g, b);
            }
        }
        after_j = 0;
    }
}



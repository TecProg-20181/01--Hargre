#include <stdio.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    Pixel pixel_grid[512][512];
    unsigned int width;
    unsigned int height;
} Image;

typedef enum {
    Greyscale=1, 
    Sepia, 
    Blur, 
    Rotation, 
    Mirror, 
    ColourInversion, 
    Cut
} FilterOptions;

Image apply_greyscale_filter(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int average = img.pixel_grid[i][j].r +
                        img.pixel_grid[i][j].g +
                        img.pixel_grid[i][j].b;
            average /= 3;
            img.pixel_grid[i][j].r = average;
            img.pixel_grid[i][j].g = average;
            img.pixel_grid[i][j].b = average;
        }
    }

    return img;
}

Image apply_sepia_filter(Image img) {
    for (unsigned int x = 0; x < img.height; ++x) {
        for (unsigned int j = 0; j < img.width; ++j) {
            unsigned short int pixel[3];
            pixel[0] = img.pixel_grid[x][j].r;
            pixel[1] = img.pixel_grid[x][j].g;
            pixel[2] = img.pixel_grid[x][j].b;

            int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            int min_red = (255 >  p) ? p : 255;
            img.pixel_grid[x][j].r = min_red;

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            min_red = (255 >  p) ? p : 255;
            img.pixel_grid[x][j].g = min_red;

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            min_red = (255 >  p) ? p : 255;
            img.pixel_grid[x][j].b = min_red;
        }
    }

    return img;
}

int get_min(int first_number, int second_number) {
    if (first_number < second_number) {
        return first_number;
    }
    return second_number;
}

int get_max(int first_number, int second_number) {
    if (first_number > second_number) {
        return first_number;
    }
    return second_number;
}

Image apply_blur_filter(Image img) {
    int size = 0;
    scanf("%d", &size);

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            Pixel average = {0, 0, 0};

            int min_height = get_min(img.height - 1, i + size/2);
            int min_width = get_min(img.width - 1, j + size/2);
            
            for (int x = get_max(0, i - size/2); x <= min_height; ++x) {
                for (int y = get_max(0, j - size/2); y <= min_width; ++y) {
                    average.r += img.pixel_grid[x][y].r;
                    average.g += img.pixel_grid[x][y].g;
                    average.b += img.pixel_grid[x][y].b;
                }
            }

            average.r /= size * size;
            average.g /= size * size;
            average.b /= size * size;

            img.pixel_grid[i][j].r = average.r;
            img.pixel_grid[i][j].g = average.g;
            img.pixel_grid[i][j].b = average.b;
        }
    }

    return img;
}

Image rotate_90_degrees_right(Image img) {
    Image rotated_image;

    rotated_image.width = img.height;
    rotated_image.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotated_image.height; ++i, ++y) {
        for (int j = rotated_image.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated_image.pixel_grid[i][j].r = img.pixel_grid[x][y].r;
            rotated_image.pixel_grid[i][j].g = img.pixel_grid[x][y].g;
            rotated_image.pixel_grid[i][j].b = img.pixel_grid[x][y].b;
        }
    }

    return rotated_image;
}

Image mirror_image(Image img) {
    int horizontal = 0;
    scanf("%d", &horizontal);

    int width = img.width; 
    int height = img.height;

    if (horizontal == 1) {
        width /= 2;  
    } else {
        height /= 2;
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int x = i, y = j;

            if (horizontal == 1) {
                y = img.width - 1 - j;
            } else {
                x = img.height - 1 - i;
            }

            Pixel aux_pixel;
            aux_pixel.r = img.pixel_grid[i][j].r;
            aux_pixel.g = img.pixel_grid[i][j].g;
            aux_pixel.b = img.pixel_grid[i][j].b;

            img.pixel_grid[i][j].r = img.pixel_grid[x][y].r;
            img.pixel_grid[i][j].g = img.pixel_grid[x][y].g;
            img.pixel_grid[i][j].b = img.pixel_grid[x][y].b;

            img.pixel_grid[x][y].r = aux_pixel.r;
            img.pixel_grid[x][y].g = aux_pixel.g;
            img.pixel_grid[x][y].b = aux_pixel.b;
        }
    }

    return img;
}

Image invert_colours(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel_grid[i][j].r = 255 - img.pixel_grid[i][j].r;
            img.pixel_grid[i][j].g = 255 - img.pixel_grid[i][j].g;
            img.pixel_grid[i][j].b = 255 - img.pixel_grid[i][j].b;
        }
    }

    return img;
}

Image cut_image(Image img, int x, int y, int width, int height) {
    Image cut_image;

    cut_image.width = width;
    cut_image.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cut_image.pixel_grid[i][j].r = img.pixel_grid[i + y][j + x].r;
            cut_image.pixel_grid[i][j].g = img.pixel_grid[i + y][j + x].g;
            cut_image.pixel_grid[i][j].b = img.pixel_grid[i + y][j + x].b;
        }
    }

    return cut_image;
}

void read_image_type() {
    char p3[4];
    scanf("%s", p3);
}

unsigned int read_image_width() {
    unsigned int width;
    scanf("%u", &width);

    return width;
}

unsigned int read_image_height() {
    unsigned int height;
    scanf("%u", &height);

    return height;
}

int read_max_color() {
    int max_color;
    scanf("%d", &max_color);

    return max_color;
}

Image read_image_pixels(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel_grid[i][j].r,
                                 &img.pixel_grid[i][j].g,
                                 &img.pixel_grid[i][j].b);
        }
    }
    return img;
}

Image read_image() {
    Image img;

    read_image_type();
    img.width = read_image_width();
    img.height = read_image_height();
    read_max_color();
    img = read_image_pixels(img);

    return img;
}

void print_image_type() {
    printf("P3\n");
}

void print_image_dimensions(Image img) {
    printf("%u %u\n255\n", img.width, img.height);
}

void print_image_pixels(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel_grid[i][j].r,
                                   img.pixel_grid[i][j].g,
                                   img.pixel_grid[i][j].b);

        }
        printf("\n");
    }
}


int main() {
    Image img;

    img = read_image();

    int number_of_options;
    scanf("%d", &number_of_options);

    for(int i = 0; i < number_of_options; ++i) {
        FilterOptions option;
        scanf("%d", &option);

        switch(option) {
            case Greyscale: {
                img = apply_greyscale_filter(img);
                break;
            }
            case Sepia: {
                img = apply_sepia_filter(img);
                break;
            }
            case Blur: {
                img = apply_blur_filter(img);
                break;
            }
            case Rotation: {
                int amount_of_rotations = 0;
                scanf("%d", &amount_of_rotations);
                amount_of_rotations %= 4;
                for (int j = 0; j < amount_of_rotations; ++j) {
                    img = rotate_90_degrees_right(img);
                }
                break;
            }
            case Mirror: {
                img = mirror_image(img);
                break;
            }
            case ColourInversion: {
                img = invert_colours(img);
                break;
            }
            case Cut: {
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = cut_image(img, x, y, width, height);
                break;
            }
        }

    }

    print_image_type();
    print_image_dimensions(img);
    print_image_pixels(img);

    return 0;
}

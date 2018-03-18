#include <stdio.h>

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
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

Image apply_greyscale_filter(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int average = img.pixel_grid[i][j].red +
                        img.pixel_grid[i][j].green +
                        img.pixel_grid[i][j].blue;
            average /= 3;
            img.pixel_grid[i][j].red = average;
            img.pixel_grid[i][j].green = average;
            img.pixel_grid[i][j].blue = average;
        }
    }

    return img;
}

Image apply_sepia_filter(Image img) {
    for (unsigned int x = 0; x < img.height; ++x) {
        for (unsigned int j = 0; j < img.width; ++j) {
            Pixel pixel;
            pixel.red = img.pixel_grid[x][j].red;
            pixel.green = img.pixel_grid[x][j].green;
            pixel.blue = img.pixel_grid[x][j].blue;

            int red =  pixel.red * .393 + pixel.green * .769 + pixel.blue * .189;
            int min_red = get_min(255, red);
            img.pixel_grid[x][j].red = min_red;

            int green =  pixel.red * .349 + pixel.green * .686 + pixel.blue * .168;
            int min_green = get_min(255, green);
            img.pixel_grid[x][j].green = min_green;

            int blue =  pixel.red * .272 + pixel.green * .534 + pixel.blue * .131;
            int min_blue = get_min(255, blue);
            img.pixel_grid[x][j].blue = min_blue;
        }
    }

    return img;
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
                    average.red += img.pixel_grid[x][y].red;
                    average.green += img.pixel_grid[x][y].green;
                    average.blue += img.pixel_grid[x][y].blue;
                }
            }

            average.red /= size * size;
            average.green /= size * size;
            average.blue /= size * size;

            img.pixel_grid[i][j].red = average.red;
            img.pixel_grid[i][j].green = average.green;
            img.pixel_grid[i][j].blue = average.blue;
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
            rotated_image.pixel_grid[i][j].red = img.pixel_grid[x][y].red;
            rotated_image.pixel_grid[i][j].green = img.pixel_grid[x][y].green;
            rotated_image.pixel_grid[i][j].blue = img.pixel_grid[x][y].blue;
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
            aux_pixel.red = img.pixel_grid[i][j].red;
            aux_pixel.green = img.pixel_grid[i][j].green;
            aux_pixel.blue = img.pixel_grid[i][j].blue;

            img.pixel_grid[i][j].red = img.pixel_grid[x][y].red;
            img.pixel_grid[i][j].green = img.pixel_grid[x][y].green;
            img.pixel_grid[i][j].blue = img.pixel_grid[x][y].blue;

            img.pixel_grid[x][y].red = aux_pixel.red;
            img.pixel_grid[x][y].green = aux_pixel.green;
            img.pixel_grid[x][y].blue = aux_pixel.blue;
        }
    }

    return img;
}

Image invert_colours(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel_grid[i][j].red = 255 - img.pixel_grid[i][j].red;
            img.pixel_grid[i][j].green = 255 - img.pixel_grid[i][j].green;
            img.pixel_grid[i][j].blue = 255 - img.pixel_grid[i][j].blue;
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
            cut_image.pixel_grid[i][j].red = img.pixel_grid[i + y][j + x].red;
            cut_image.pixel_grid[i][j].green = img.pixel_grid[i + y][j + x].green;
            cut_image.pixel_grid[i][j].blue = img.pixel_grid[i + y][j + x].blue;
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
            scanf("%hu %hu %hu", &img.pixel_grid[i][j].red,
                                 &img.pixel_grid[i][j].green,
                                 &img.pixel_grid[i][j].blue);
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
            printf("%hu %hu %hu ", img.pixel_grid[i][j].red,
                                   img.pixel_grid[i][j].green,
                                   img.pixel_grid[i][j].blue);

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

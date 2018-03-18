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

Image apply_greyscale_filter(Image image) {
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            int average = image.pixel_grid[i][j].red +
                        image.pixel_grid[i][j].green +
                        image.pixel_grid[i][j].blue;
            average /= 3;
            image.pixel_grid[i][j].red = average;
            image.pixel_grid[i][j].green = average;
            image.pixel_grid[i][j].blue = average;
        }
    }

    return image;
}

Image apply_sepia_filter(Image image) {
    for (unsigned int x = 0; x < image.height; ++x) {
        for (unsigned int j = 0; j < image.width; ++j) {
            Pixel pixel;
            pixel.red = image.pixel_grid[x][j].red;
            pixel.green = image.pixel_grid[x][j].green;
            pixel.blue = image.pixel_grid[x][j].blue;

            int red =  pixel.red * .393 + pixel.green * .769 + pixel.blue * .189;
            int min_red = get_min(255, red);
            image.pixel_grid[x][j].red = min_red;

            int green =  pixel.red * .349 + pixel.green * .686 + pixel.blue * .168;
            int min_green = get_min(255, green);
            image.pixel_grid[x][j].green = min_green;

            int blue =  pixel.red * .272 + pixel.green * .534 + pixel.blue * .131;
            int min_blue = get_min(255, blue);
            image.pixel_grid[x][j].blue = min_blue;
        }
    }

    return image;
}

Image apply_blur_filter(Image image) {
    int size = 0;
    scanf("%d", &size);

    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            Pixel average = {0, 0, 0};

            int min_height = get_min(image.height - 1, i + size/2);
            int min_width = get_min(image.width - 1, j + size/2);
            
            for (int x = get_max(0, i - size/2); x <= min_height; ++x) {
                for (int y = get_max(0, j - size/2); y <= min_width; ++y) {
                    average.red += image.pixel_grid[x][y].red;
                    average.green += image.pixel_grid[x][y].green;
                    average.blue += image.pixel_grid[x][y].blue;
                }
            }

            average.red /= size * size;
            average.green /= size * size;
            average.blue /= size * size;

            image.pixel_grid[i][j].red = average.red;
            image.pixel_grid[i][j].green = average.green;
            image.pixel_grid[i][j].blue = average.blue;
        }
    }

    return image;
}

int get_amount_of_rotations() {
    int amount_of_rotations = 0;
    scanf("%d", &amount_of_rotations);
    amount_of_rotations %= 4;

    return amount_of_rotations;
}

Image rotate_90_degrees_right(Image image) {
    Image rotated_image;

    int amount_of_rotations = get_amount_of_rotations();

    rotated_image.width = image.height;
    rotated_image.height = image.width;

    for (int rotation = 0; rotation < amount_of_rotations; rotation++) {
        int y = 0;

        for (unsigned int i = 0; i < rotated_image.height; ++i) {
            int x = 0;

            for (int j = rotated_image.width - 1; j >= 0; --j) {
                rotated_image.pixel_grid[i][j].red = image.pixel_grid[x][y].red;
                rotated_image.pixel_grid[i][j].green = image.pixel_grid[x][y].green;
                rotated_image.pixel_grid[i][j].blue = image.pixel_grid[x][y].blue;

                x += 1;
            }
            y += 1;
        }
    }

    return rotated_image;
}

Image mirror_image(Image image) {
    int horizontal = 0;
    scanf("%d", &horizontal);

    int width = image.width; 
    int height = image.height;

    if (horizontal == 1) {
        width /= 2;  
    } else {
        height /= 2;
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int x = i, y = j;

            if (horizontal == 1) {
                y = image.width - 1 - j;
            } else {
                x = image.height - 1 - i;
            }

            Pixel aux_pixel;
            aux_pixel.red = image.pixel_grid[i][j].red;
            aux_pixel.green = image.pixel_grid[i][j].green;
            aux_pixel.blue = image.pixel_grid[i][j].blue;

            image.pixel_grid[i][j].red = image.pixel_grid[x][y].red;
            image.pixel_grid[i][j].green = image.pixel_grid[x][y].green;
            image.pixel_grid[i][j].blue = image.pixel_grid[x][y].blue;

            image.pixel_grid[x][y].red = aux_pixel.red;
            image.pixel_grid[x][y].green = aux_pixel.green;
            image.pixel_grid[x][y].blue = aux_pixel.blue;
        }
    }

    return image;
}

Image invert_colours(Image image) {
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            image.pixel_grid[i][j].red = 255 - image.pixel_grid[i][j].red;
            image.pixel_grid[i][j].green = 255 - image.pixel_grid[i][j].green;
            image.pixel_grid[i][j].blue = 255 - image.pixel_grid[i][j].blue;
        }
    }

    return image;
}

Image cut_image(Image image) {
    int x;
    int y;
    int width;
    int height;
    Image cut_image;

    scanf("%d %d", &x, &y);
    scanf("%d %d", &width, &height);
    
    cut_image.width = width;
    cut_image.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cut_image.pixel_grid[i][j].red = image.pixel_grid[i + y][j + x].red;
            cut_image.pixel_grid[i][j].green = image.pixel_grid[i + y][j + x].green;
            cut_image.pixel_grid[i][j].blue = image.pixel_grid[i + y][j + x].blue;
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

Image read_image_pixels(Image image) {
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            scanf("%hu %hu %hu", &image.pixel_grid[i][j].red,
                                 &image.pixel_grid[i][j].green,
                                 &image.pixel_grid[i][j].blue);
        }
    }
    return image;
}

Image read_image() {
    Image image;

    read_image_type();
    image.width = read_image_width();
    image.height = read_image_height();
    read_max_color();
    image = read_image_pixels(image);

    return image;
}

void print_image_type() {
    printf("P3\n");
}

void print_image_dimensions(Image image) {
    printf("%u %u\n255\n", image.width, image.height);
}

void print_image_pixels(Image image) {
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            printf("%hu %hu %hu ", image.pixel_grid[i][j].red,
                                   image.pixel_grid[i][j].green,
                                   image.pixel_grid[i][j].blue);

        }
        printf("\n");
    }
}


int main() {
    Image image;

    image = read_image();

    int number_of_options;
    scanf("%d", &number_of_options);

    for(int i = 0; i < number_of_options; ++i) {
        FilterOptions option;
        scanf("%d", &option);

        switch(option) {
            case Greyscale: {
                image = apply_greyscale_filter(image);
                break;
            }
            case Sepia: {
                image = apply_sepia_filter(image);
                break;
            }
            case Blur: {
                image = apply_blur_filter(image);
                break;
            }
            case Rotation: {
                image = rotate_90_degrees_right(image);
                break;
            }
            case Mirror: {
                image = mirror_image(image);
                break;
            }
            case ColourInversion: {
                image = invert_colours(image);
                break;
            }
            case Cut: {
                image = cut_image(image);
                break;
            }
        }

    }

    print_image_type();
    print_image_dimensions(image);
    print_image_pixels(image);

    return 0;
}

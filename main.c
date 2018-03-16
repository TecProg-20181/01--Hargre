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

Image apply_greyscale_filter(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int media = img.pixel_grid[i][j].r +
                        img.pixel_grid[i][j].g +
                        img.pixel_grid[i][j].b;
            media /= 3;
            img.pixel_grid[i][j].r = media;
            img.pixel_grid[i][j].g = media;
            img.pixel_grid[i][j].b = media;
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
            int menor_r = (255 >  p) ? p : 255;
            img.pixel_grid[x][j].r = menor_r;

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            menor_r = (255 >  p) ? p : 255;
            img.pixel_grid[x][j].g = menor_r;

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            menor_r = (255 >  p) ? p : 255;
            img.pixel_grid[x][j].b = menor_r;
        }
    }

    return img;
}

void apply_blur_filter(unsigned int height, Pixel pixel_grid[512][512], int T, unsigned int width) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (height - 1 > i + T/2) ? i + T/2 : height - 1;
            int min_w = (width - 1 > j + T/2) ? j + T/2 : width - 1;
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= menor_h; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_w; ++y) {
                    media.r += pixel_grid[x][y].r;
                    media.g += pixel_grid[x][y].g;
                    media.b += pixel_grid[x][y].b;
                }
            }

            media.r /= T * T;
            media.g /= T * T;
            media.b /= T * T;

            pixel_grid[i][j].r = media.r;
            pixel_grid[i][j].g = media.g;
            pixel_grid[i][j].b = media.b;
        }
    }
}

Image rotate_90_degrees_right(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
        for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel_grid[i][j].r = img.pixel_grid[x][y].r;
            rotacionada.pixel_grid[i][j].g = img.pixel_grid[x][y].g;
            rotacionada.pixel_grid[i][j].b = img.pixel_grid[x][y].b;
        }
    }

    return rotacionada;
}

Image mirror_image(Image img) {
    int horizontal = 0;
    scanf("%d", &horizontal);

    int width = img.width, height = img.height;

    if (horizontal == 1) width /= 2;
    else height /= 2;

    for (int i2 = 0; i2 < height; ++i2) {
        for (int j = 0; j < width; ++j) {
            int x = i2, y = j;

            if (horizontal == 1) y = img.width - 1 - j;
            else x = img.height - 1 - i2;

            Pixel aux1;
            aux1.r = img.pixel_grid[i2][j].r;
            aux1.g = img.pixel_grid[i2][j].g;
            aux1.b = img.pixel_grid[i2][j].b;

            img.pixel_grid[i2][j].r = img.pixel_grid[x][y].r;
            img.pixel_grid[i2][j].g = img.pixel_grid[x][y].g;
            img.pixel_grid[i2][j].b   = img.pixel_grid[x][y].b;

            img.pixel_grid[x][y].r = aux1.r;
            img.pixel_grid[x][y].g = aux1.g;
            img.pixel_grid[x][y].b = aux1.b;
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
    Image cortada;

    cortada.width = width;
    cortada.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cortada.pixel_grid[i][j].r = img.pixel_grid[i + y][j + x].r;
            cortada.pixel_grid[i][j].g = img.pixel_grid[i + y][j + x].g;
            cortada.pixel_grid[i][j].b = img.pixel_grid[i + y][j + x].b;
        }
    }

    return cortada;
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

    read_image_type();

    int max_color;
    img.width = read_image_width();
    img.height = read_image_height();
    max_color = read_max_color();

    img = read_image_pixels(img);

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = apply_greyscale_filter(img);
                break;
            }
            case 2: { // Filtro Sepia
                img = apply_sepia_filter(img);
                break;
            }
            case 3: { // apply_blur_filter
                int tamanho = 0;
                scanf("%d", &tamanho);
                apply_blur_filter(img.height, img.pixel_grid, tamanho, img.width);
                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotate_90_degrees_right(img);
                }
                break;
            }
            case 5: { // Espelhamento
                img = mirror_image(img);
                break;
            }
            case 6: { // Inversao de Cores
                img = invert_colours(img);
                break;
            }
            case 7: { // Cortar Imagem
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

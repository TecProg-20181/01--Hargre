#include <stdio.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    Pixel pixel_grid[512][512];
    unsigned int w;
    unsigned int h;
} Image;


int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int pixels_are_equal(Pixel p1, Pixel p2) {
    if (p1.r == p2.r &&
        p1.g == p2.g &&
        p1.b == p2.b)
        return 1;
    return 0;
}


Image apply_greyscale_filter(Image img) {
    /*for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            print("%u", img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2]);
        }
    }*/

    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
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
    for (unsigned int x = 0; x < img.h; ++x) {
        for (unsigned int j = 0; j < img.w; ++j) {
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

void apply_blur_filter(unsigned int h, Pixel pixel_grid[512][512], int T, unsigned int w) {
    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (h - 1 > i + T/2) ? i + T/2 : h - 1;
            int min_w = (w - 1 > j + T/2) ? j + T/2 : w - 1;
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= menor_h; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_w; ++y) {
                    media.r += pixel_grid[x][y].r;
                    media.g += pixel_grid[x][y].g;
                    media.b += pixel_grid[x][y].b;
                }
            }

            // printf("%u", media.r)
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

    rotacionada.w = img.h;
    rotacionada.h = img.w;

    for (unsigned int i = 0, y = 0; i < rotacionada.h; ++i, ++y) {
        for (int j = rotacionada.w - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel_grid[i][j].r = img.pixel_grid[x][y].r;
            rotacionada.pixel_grid[i][j].g = img.pixel_grid[x][y].g;
            rotacionada.pixel_grid[i][j].b = img.pixel_grid[x][y].b;
        }
    }

    return rotacionada;
}

void invert_colours(Pixel pixel_grid[512][512],
                    unsigned int w, unsigned int h) {
    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            pixel_grid[i][j].r = 255 - pixel_grid[i][j].r;
            pixel_grid[i][j].g = 255 - pixel_grid[i][j].g;
            pixel_grid[i][j].b = 255 - pixel_grid[i][j].b;
        }
    }
}

Image cut_image(Image img, int x, int y, int w, int h) {
    Image cortada;

    cortada.w = w;
    cortada.h = h;

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
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
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            scanf("%hu %hu %hu", &img.pixel_grid[i][j].r,
                                 &img.pixel_grid[i][j].g,
                                 &img.pixel_grid[i][j].b);
        }
    }

    return img;
}

int main() {
    Image img;

    read_image_type();

    int max_color;
    img.w = read_image_width();
    img.h = read_image_height();
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
                apply_blur_filter(img.h, img.pixel_grid, tamanho, img.w);
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
                int horizontal = 0;
                scanf("%d", &horizontal);

                int w = img.w, h = img.h;

                if (horizontal == 1) w /= 2;
                else h /= 2;

                for (int i2 = 0; i2 < h; ++i2) {
                    for (int j = 0; j < w; ++j) {
                        int x = i2, y = j;

                        if (horizontal == 1) y = img.w - 1 - j;
                        else x = img.h - 1 - i2;

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
                break;
            }
            case 6: { // Inversao de Cores
                invert_colours(img.pixel_grid, img.w, img.h);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int w, h;
                scanf("%d %d", &w, &h);

                img = cut_image(img, x, y, w, h);
                break;
            }
        }

    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.w, img.h);

    // print pixels of image
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            printf("%hu %hu %hu ", img.pixel_grid[i][j].r,
                                   img.pixel_grid[i][j].g,
                                   img.pixel_grid[i][j].b);

        }
        printf("\n");
    }
    return 0;
}

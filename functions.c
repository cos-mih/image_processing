#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"bmp_header.h"

char *read_path()
{
    char *path = (char*)calloc(256, 1);
    if (!path) {
        return NULL;
    }

    scanf("%s", path);
    return path;
}

bmp *init_bmp_structure() {
    bmp *BMP = (bmp *)calloc(1, sizeof(bmp));
    if (!BMP) {
        return NULL;
    }

    BMP->fh = (bmp_fileheader *)calloc(1, sizeof(bmp_fileheader));
    if (!BMP->fh) {
        free(BMP);
        return NULL;
    }

    BMP->ih = (bmp_infoheader *)calloc(1, sizeof(bmp_infoheader));
    if (!BMP->ih) {
        free(BMP->fh);
        free(BMP);
        return NULL;
    }

    return BMP;
}

void init_bitmap(bmp *BMP) {
    BMP->bitmap = (pixel **)calloc(BMP->ih->height, sizeof(pixel *));
    if (!BMP->bitmap) {
        free(BMP->ih);
        free(BMP->fh);
        free(BMP);
        return;
    }

    for (int i = 0; i < BMP->ih->height; i++) {
        BMP->bitmap[i] = (pixel *)calloc(BMP->ih->width, sizeof(pixel));
        if (!BMP->bitmap[i]) {
            for (int j = 0; j < i; j++) {
                free(BMP->bitmap[j]);
            }

            free(BMP->bitmap);
            free(BMP->ih);
            free(BMP->fh);
            free(BMP);
            return;
        }
    }
}

void Save(bmp *BMP)
{
    char *filename = read_path();
    if (!filename) {
        return; 
    }

    FILE *f = fopen(filename, "wb");
    if (!f) {
        return;
    }

    fwrite(&BMP->fh->fileMarker1, 1, 1, f);
    fwrite(&BMP->fh->fileMarker2, 1, 1, f);
    fwrite(&BMP->fh->bfSize, 4, 1, f);
    fwrite(&BMP->fh->unused1, 2, 1, f);
    fwrite(&BMP->fh->unused2, 2, 1, f);
    fwrite(&BMP->fh->imageDataOffset, 4, 1, f);

    fwrite(&BMP->ih->biSize, 4, 1, f);
    fwrite(&BMP->ih->width, 4, 1, f);
    fwrite(&BMP->ih->height, 4, 1, f);
    fwrite(&BMP->ih->planes, 2, 1, f);
    fwrite(&BMP->ih->bitPix, 2, 1, f);
    fwrite(&BMP->ih->biCompression, 4, 1, f);
    fwrite(&BMP->ih->biSizeImage, 4, 1, f);
    fwrite(&BMP->ih->biXPelsPerMeter, 4, 1, f);
    fwrite(&BMP->ih->biYPelsPerMeter, 4, 1, f);
    fwrite(&BMP->ih->biClrUsed, 4, 1, f);
    fwrite(&BMP->ih->biClrImportant, 4, 1, f);

    char zero_byte = '\0';
    fwrite(&zero_byte, 1, BMP->fh->imageDataOffset - ftell(f), f);
    fseek(f, BMP->fh->imageDataOffset, SEEK_SET);
    for (int i = 0; i < BMP->ih->height; i++) {
        for (int j = 0; j < BMP->ih->width; j++) {
            pixel pix = BMP->bitmap[i][j];
            fwrite(&pix.B, 1, 1, f);
            fwrite(&pix.G, 1, 1, f);
            fwrite(&pix.R, 1, 1, f);
        }

        int padding = (4 - (BMP->ih->width * 3) % 4) % 4;
        fwrite(&zero_byte, 1, padding, f);
    }

    free(filename);
    fclose(f);
}

void Edit(bmp *BMP)
{
    char *filename = read_path();
    if (!filename) {
        return; 
    }

    FILE *f = fopen(filename, "rb");
    if (!f) {
        return;
    }


    fread(&BMP->fh->fileMarker1, 1, 1, f);
    fread(&BMP->fh->fileMarker2, 1, 1, f);
    fread(&BMP->fh->bfSize, 4, 1, f);
    fread(&BMP->fh->unused1, 2, 1, f);
    fread(&BMP->fh->unused2, 2, 1, f);
    fread(&BMP->fh->imageDataOffset, 4, 1, f);

    fread(&BMP->ih->biSize, 4, 1, f);
    fread(&BMP->ih->width, 4, 1, f);
    fread(&BMP->ih->height, 4, 1, f);
    fread(&BMP->ih->planes, 2, 1, f);
    fread(&BMP->ih->bitPix, 2, 1, f);
    fread(&BMP->ih->biCompression, 4, 1, f);
    fread(&BMP->ih->biSizeImage, 4, 1, f);
    fread(&BMP->ih->biXPelsPerMeter, 4, 1, f);
    fread(&BMP->ih->biYPelsPerMeter, 4, 1, f);
    fread(&BMP->ih->biClrUsed, 4, 1, f);
    fread(&BMP->ih->biClrImportant, 4, 1, f);

    init_bitmap(BMP);

    fseek(f, BMP->fh->imageDataOffset, SEEK_SET);
    for (int i = 0; i < BMP->ih->height; i++) {
        for (int j = 0; j < BMP->ih->width; j++) {
            pixel pix;
            fread(&pix.B, 1, 1, f);
            fread(&pix.G, 1, 1, f);
            fread(&pix.R, 1, 1, f);

            BMP->bitmap[i][j] = pix;
        }

        int padding = (4 - (BMP->ih->width * 3) % 4) % 4;
        fseek(f, padding, ftell(f));
    }


    fclose(f);
    free(filename);
}

void Insert(bmp *BMP) {
    bmp *overlay = init_bmp_structure();

    Edit(overlay);

    int y, x;
    scanf("%d %d", &y, &x);
    int y_copy = y;

    for (int i = 0; x < BMP->ih->height && i < overlay->ih->height; i++, x++) {
        y = y_copy;
        for (int j = 0; y < BMP->ih->width && j < overlay->ih->width; j++, y++) {
            BMP->bitmap[x][y] = overlay->bitmap[i][j];
        }
    }

}

void Set_params(pixel *draw_color, int *line_width) {
    char param[20];
    scanf("%s", param);

    if (!strcmp(param, "draw_color")) {
        scanf("%hhu %hhu %hhu", &draw_color->R, &draw_color->G, &draw_color->B);
    }

    if (!strcmp(param, "line_width")) {
        scanf("%d", line_width);
    }
}

void color_square(bmp *BMP, int y, int x, pixel draw_color, int line_width) {
    for (int i = x - line_width / 2; i <= x + line_width / 2 && i < BMP->ih->height; i++) {
        if (i < 0) {
            continue;
        }
        for (int j = y - line_width / 2; j <= y + line_width / 2 && j < BMP->ih->width; j++) {
            if (j < 0) {
                continue;
            }
            BMP->bitmap[i][j] = draw_color;
        }
    }
}

int min(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

void draw_line(bmp *BMP, pixel draw_color, int line_width, int y1, int x1, int y2, int x2) {
    if (abs(x1 - x2) >= abs(y1 - y2)) {
        for (int x = min(x1, x2); x <= max(x1, x2) && x < BMP->ih->height; x++) {
            int y = ((x - x1) * (y2 - y1) + y1 * (x2 - x1)) / (x2 - x1);
            color_square(BMP, y, x, draw_color, line_width);
        }   
    } else {
        for (int y = min(y1, y2); y <= max(y1, y2) && y < BMP->ih->width; y++) {
            int x = ((y - y1) * (x2 - x1) + x1 * (y2 - y1)) / (y2 - y1);
            color_square(BMP, y, x, draw_color, line_width);
        } 
    }  
}

void Draw(bmp *BMP, pixel draw_color, int line_width) {
    char type[20];
    scanf("%s", type);

    if (!strcmp(type, "line")) {
        int y1, x1, y2, x2;
        scanf("%d %d %d %d", &y1, &x1, &y2, &x2);
        draw_line(BMP, draw_color, line_width, y1, x1, y2, x2);
    }

    if (!strcmp(type, "rectangle")) {
        int y1, x1, width, height;
        scanf("%d %d %d %d", &y1, &x1, &width, &height);
        int y2 = y1 + width, x2 = x1 + height;
        draw_line(BMP, draw_color, line_width, y1, x1, y1, x2);
        draw_line(BMP, draw_color, line_width, y2, x1, y2, x2);
        draw_line(BMP, draw_color, line_width, y1, x1, y2, x1);
        draw_line(BMP, draw_color, line_width, y1, x2, y2, x2);
    }

    if (!strcmp(type, "triangle")) {
        int y1, x1, y2, x2, y3, x3;
        scanf("%d %d %d %d %d %d", &y1, &x1, &y2, &x2, &y3, &x3);
        draw_line(BMP, draw_color, line_width, y1, x1, y2, x2);
        draw_line(BMP, draw_color, line_width, y3, x3, y2, x2);
        draw_line(BMP, draw_color, line_width, y1, x1, y3, x3);
    }
}

void Quit(char *command, bmp *BMP)
{
    free(command);
    for (int i = 0; i < BMP->ih->height; i++) {
        free(BMP->bitmap[i]);
    }
    free(BMP->bitmap);
    free(BMP->ih);
    free(BMP->fh);
    free(BMP);
}
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

    fwrite(BMP->fh->fileMarker1, 1, 1, f);
    fwrite(BMP->fh->fileMarker2, 1, 1, f);
    fwrite(BMP->fh->bfSize, 4, 1, f);
    fwrite(BMP->fh->unused1, 2, 1, f);
    fwrite(BMP->fh->unused2, 2, 1, f);
    fwrite(BMP->fh->imageDataOffset, 4, 1, f);

    fwrite(BMP->ih->biSize, 4, 1, f);
    fwrite(BMP->ih->width, 4, 1, f);
    fwrite(BMP->ih->height, 4, 1, f);
    fwrite(BMP->ih->planes, 2, 1, f);
    fwrite(BMP->ih->bitPix, 2, 1, f);
    fwrite(BMP->ih->biCompression, 4, 1, f);
    fwrite(BMP->ih->biSizeImage, 4, 1, f);
    fwrite(BMP->ih->biXPelsPerMeter, 4, 1, f);
    fwrite(BMP->ih->biYPelsPerMeter, 4, 1, f);
    fwrite(BMP->ih->biClrUsed, 4, 1, f);
    fwrite(BMP->ih->biClrImportant, 4, 1, f);

    

    free(filename);
    fclose(f);
}

void Edit(bmp *BMP)
{
    char *filename = read_path();
    if (!filename) {
        return; 
    }

    free(filename);
}

void Quit(char *command, bmp *BMP)
{
    free(command);
    free(BMP);
}
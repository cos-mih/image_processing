#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"bmp_header.h"

int main()
{
    bmp *BMP = init_bmp_structure();
    if (!BMP) {
        return -1;
    }

    char *command = calloc(256, 1);
    if (!command) {
        return -1;
    }

    // pixel *draw_color = (pixel *)calloc(1, sizeof(pixel));
    // if (!draw_color) {
    //     Quit(command, BMP);
    //     return -1;
    // }
    pixel draw_color;
    draw_color.B = draw_color.G = draw_color.R = '\0';
    int line_width = 1;

    while (scanf("%s", command)) {
        if (!strcmp(command, "save")) {
            Save(BMP);
            continue;
        }

        if (!strcmp(command, "edit")) {
            Edit(BMP);
            continue;
        }

        if (!strcmp(command, "insert")) {
            Insert(BMP);
            continue;
        }

        if (!strcmp(command, "set")) {
            Set_params(&draw_color, &line_width);
            continue;
        }

        if (!strcmp(command, "draw")) {
            Draw(BMP, draw_color, line_width);
            continue;
        }

        if (!strcmp(command, "fill")) {
            Fill(BMP, draw_color);
            continue;
        }

        if (!strcmp(command, "quit")) {
            Quit(command, BMP);
            return 0;
        }
    }

    return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"bmp_header.h"

int main()
{
    bmp *BMP = (bmp*)calloc(1, sizeof(bmp));
    if (!BMP) {
        return -1;
    }

    char *command = calloc(256, 1);
    if (!command) {
        return -1;
    }

    while (scanf("%s ", command)) {
        if (!strcmp(command, "save")) {
            Save(BMP);
            continue;
        }

        if (!strcmp(command, "edit")) {
            Edit(BMP);
            continue;
        }

        if (!strcmp(command, "quit")) {
            Quit(command, BMP);
            break;
        }
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_USAGE "Usage: %s 1-to-11-digit-number\n"

#define BAR "\u2588"
#define GAP " "

#define HEIGHT 30

// 1s represent bars, 0s represent gaps in the L field
const char codes[10][7] = {
    { // 0
        1, 1, 1, 0, 0, 1, 0
    },
    { // 1
        1, 1, 0, 0, 1, 1, 0
    },
    { // 2
        1, 1, 0, 1, 1, 0, 0
    },
    { // 3
        1, 0, 0, 0, 0, 1, 0
    },
    { // 4
        1, 0, 1, 1, 1, 0, 0
    },
    { // 5
        1, 0, 0, 1, 1, 1, 0
    },
    { // 6
        1, 0, 1, 0, 0, 0, 0
    },
    { // 7
        1, 0, 0, 0, 1, 0, 0
    },
    { // 8
        1, 0, 0, 1, 0, 0, 0
    },
    { // 9
        1, 1, 1, 0, 1, 0, 0
    }
};

char* getbarcodetext(const char* source)
{
    char* destination = NULL;
    destination = malloc(13);

    sprintf(destination, "%ld", atol(source));

    if (destination == NULL || strlen(source) > 11) {
        return NULL;
    }

    sprintf(destination, "%011ld", atol(source));
    *(destination + 11) = '\0';
    *(destination + 12) = '\0';

    int M = 0;

    // calculate the check digit
    for (int i = 0; i <= 10; i += 2) {
        char* buffer = NULL;
        buffer = malloc(2);
        strncpy(buffer, destination + i, 1);
        buffer[1] = '\0';

        M += atoi(buffer);
        
        free(buffer);
    }

    M *= 3;

    for (int i = 1; i <= 11; i += 2) {
        char* buffer = NULL;
        buffer = malloc(2);
        strncpy(buffer, destination + i, 1);
        buffer[1] = '\0';

        M += atoi(buffer);

        free(buffer);
    }

    M %= 10;

    if (M != 0) {
        M = 10 - M;
    }

    // if (strlen(source) == 12)

    *(destination + 11) = 48 + M;

    return destination;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf(MSG_USAGE, argv[0]);
        return EXIT_FAILURE;
    }

    char* number = getbarcodetext(argv[1]);

    if (number == NULL) {
        printf(MSG_USAGE, argv[0]);
        return EXIT_FAILURE;
    }

    for (int z = 0; z < HEIGHT; z++) {

        // print the quiet zone
        for (int i = 0; i < 9; i++) {
            printf(BAR);
        }

        // print the start
        printf("%s%s%s", GAP, BAR, GAP);

        for (int i = 0; i < 13; i++) {

            if (i == 6) {
                if (z < HEIGHT - 2) {
                    printf("%s%s%s%s%s", BAR, GAP, BAR, GAP, BAR);
                } else {
                    printf("     ");
                }

            } else {
                
                int j = i;
                
                if (i > 6) {
                    j--;
                }

                char* thisNumber = NULL;
                thisNumber = malloc(2);
                strncpy(thisNumber, number + j, 1);
                thisNumber[1] = '\0';
                // printf("%s:", thisNumber);
                
                if (z == HEIGHT - 1) {
                    printf("   %s   ", thisNumber);
                } else if (z == HEIGHT - 2) {
                    printf("       ");    
                } else {
                    for (int k = 0; k < 7; k++) {
                        if (j >= 6) {
                            if (codes[atoi(thisNumber)][k]) {
                                printf("%s", GAP);
                            } else {
                                printf("%s", BAR);
                            }
                        } else {
                            if (codes[atoi(thisNumber)][k]) {
                                printf("%s", BAR);
                            } else {
                                printf("%s", GAP);
                            }
                        }
                        
                        // printf("%s", i > 10 ^ codes[atoi(thisNumber)][k] ? BAR : GAP);
                    }
                }

                free(thisNumber);
            }
        }

        // print the end
        printf("%s%s%s", GAP, BAR, GAP);

        // print the quiet zone
        for (int i = 0; i < 9; i++) {
            printf(BAR);
        }
        
        printf("\n");
    }

    free(number);

    return EXIT_SUCCESS;
}

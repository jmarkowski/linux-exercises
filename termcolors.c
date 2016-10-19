/******************************************************************************
 * See http://ascii-table.com/ansi-escape-sequences.php
 *****************************************************************************/
#include <stdio.h>
#include <string.h>

enum {
    FG_BLACK = 30,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_MAGENTA = 35,
    FG_CYAN = 36,
    FG_WHITE = 37,

    MIN_FG = FG_BLACK,
    MAX_FG = FG_WHITE,
};

enum {
    BG_BLACK = 40,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_YELLOW = 43,
    BG_BLUE = 44,
    BG_MAGENTA = 45,
    BGCYAN = 46,
    BG_WHITE = 47,

    MIN_BG = BG_BLACK,
    MAX_BG = BG_WHITE,
};

enum {
    ATTR_RESET = 0,
    ATTR_BOLD = 1,
    ATTR_DIM = 2,
    ATTR_UNDERLINE = 4,
    ATTR_BLINK = 5,
    ATTR_REVERSE = 7,
    ATTR_HIDDEN = 8,

    MIN_ATTR = ATTR_RESET,
    MAX_ATTR = ATTR_HIDDEN,
};

int main()
{
    int fg, bg, attr;
    char str[20];

    /* \xHH is the escape sequence for a byte with hex value HH */
    printf("\x1B[31;47;1;4mCOLOR DEMO\x1B[0m\n\n");

    for (bg = MIN_BG; bg <= MAX_BG; bg++) {
        for (fg = MIN_FG; fg <= MAX_FG; fg++) {
            for (attr = MIN_ATTR; attr <= MAX_ATTR; attr++) {
                switch (attr) {
                case ATTR_RESET:
                    strcpy(str, "RESET");
                    break;
                case ATTR_DIM:
                    strcpy(str, "DIM");
                    break;
                case ATTR_BOLD:
                    strcpy(str, "BOLD");
                    break;
                case ATTR_UNDERLINE:
                    strcpy(str, "UNDERLINE");
                    break;
                case ATTR_BLINK:
                    strcpy(str, "BLINK");
                    break;
                case ATTR_REVERSE:
                    strcpy(str, "REVERSE");
                    break;
                case ATTR_HIDDEN:
                    strcpy(str, "HIDDEN");
                    break;
                default:
                    continue;
                }

                printf("(Esc[%d;%d;%dm ", fg, bg, attr);
                printf("\x1B[%d;%d;%dm%s\x1B[0m", fg, bg, attr, str);
                printf(" Esc[0m) ");
            }
            printf("\n");
        }
    }

    return 0;
}

#ifndef COLOR_H
#define COLOR_H

typedef struct rgb_color
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
} RGB;

#define COLOR_BLACK (Colors[0])
#define COLOR_WHITE (Colors[1])
#define COLOR_GREY (Colors[2])
#define COLOR_RED (Colors[3])
#define COLOR_GREEN (Colors[4])
#define COLOR_BLUE (Colors[5])

extern const RGB Colors[];

#endif

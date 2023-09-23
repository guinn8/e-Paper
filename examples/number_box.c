#include "EPD_2in13_V3.h"
#include <time.h> 
#include <assert.h>  // For assert() function
#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include <stdlib.h>     //exit()
#include <signal.h>     //signal()

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))





// Finalize EPD and free resources


#define TOTAL_BOXES 7
#define EPD_WIDTH EPD_2in13_V3_HEIGHT
#define BOX_GAP 2
#define BOX_SIZE ((EPD_WIDTH - (BOX_GAP * (TOTAL_BOXES - 1))) / TOTAL_BOXES)
// Draw the title bar
void drawTitle(const char *title) {
    Paint_DrawRectangle(0, 0, EPD_2in13_V3_HEIGHT, 30, BLACK, 2, DRAW_FILL_FULL);
    Paint_DrawString_EN(5, 5, title, &Font24, BLACK, WHITE);
}

typedef enum {
    STYLE_DEFAULT,
    STYLE_INVERTED,
    STYLE_INVERTED_STRIKE,
} NumberBoxStyle;

typedef struct {
    UWORD number;
    NumberBoxStyle style;
} NumberBox_t;

void drawNumberBox(UWORD x, UWORD y, NumberBox_t *numberbox){
    assert(numberbox->number >= 0 && numberbox->number <= 99);
    assert((x + BOX_SIZE) <= EPD_2in13_V3_HEIGHT);
    assert((y + BOX_SIZE) <= EPD_2in13_V3_WIDTH);

    const UWORD rectXEnd = x + BOX_SIZE - 1;
    const UWORD rectYEnd = y + BOX_SIZE - 1;
    const int num_digits = (numberbox->number > 9) ? 2 : 1;
    const int totalTextWidth = num_digits * 17;
    const int totalTextHeight = 22;
    UWORD textX = (x + (BOX_SIZE - totalTextWidth) / 2);
    const UWORD textY = y + (BOX_SIZE - totalTextHeight) / 2;

    int foreground, background, fill, fill_colour, stroke_width;
    switch(numberbox->style) {
        case STYLE_INVERTED_STRIKE:
            Paint_DrawLine(x, y, rectXEnd, rectYEnd, BLACK, 2, LINE_STYLE_SOLID);
        case STYLE_INVERTED:
            foreground = BLACK;
            background = WHITE;
            fill = DRAW_FILL_EMPTY;
            fill_colour = foreground;
            stroke_width = 2;
            break;
        default:  // STYLE_DEFAULT
            foreground = WHITE;
            background = BLACK;
            fill = DRAW_FILL_FULL;
            fill_colour = background;
            stroke_width = 1;
            y += 1;
            textX -= 1;
    }

    Paint_DrawRectangle(x, y, rectXEnd, rectYEnd, fill_colour, stroke_width, fill);
    Paint_DrawNum(textX, textY, numberbox->number, &Font24, foreground, background);
}

void drawNumberBoxes(UBYTE *BlackImage, NumberBox_t numbers[], int numBoxes, const UWORD yOffset) {
    assert(numBoxes <= TOTAL_BOXES);
    for (UWORD x = 0, i = 0; i < numBoxes; ++i) {
        drawNumberBox(x, yOffset, &numbers[i]);
        x += (BOX_SIZE + BOX_GAP);
    }
}


int number_box_main(UBYTE *BlackImage) {

    drawTitle("Oboz Bridger");

    NumberBox_t sizes[] = {
        {.style = STYLE_DEFAULT, .number = 8},
        {.style = STYLE_INVERTED, .number = 9},
        {.style = STYLE_INVERTED_STRIKE, .number = 10},
        {.style = STYLE_DEFAULT, .number = 11},
        {.style = STYLE_DEFAULT, .number = 12},
        {.style = STYLE_DEFAULT, .number = 13},
        {.style = STYLE_INVERTED_STRIKE, .number = 14},
    };

    int numBoxes = ARRAY_SIZE(sizes);  // Using the macro here
    Paint_DrawString_EN(5, 35, "$260.00", &Font24, WHITE, BLACK);
    Paint_DrawString_EN(5, 60, "Best seller!", &Font24, WHITE, BLACK);

    drawNumberBoxes(BlackImage, sizes, numBoxes, 88);

}
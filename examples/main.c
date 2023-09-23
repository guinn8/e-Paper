#include <signal.h>     //signal()
#include "EPD_2in13_V3.h"
#include <time.h> 
#include <assert.h>  // For assert() function
#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include <stdlib.h>     //exit()
#include <signal.h>     //signal()

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:exit\r\n");
    DEV_Module_Exit();

    exit(0);
}

// Create and configure image buffer
UBYTE* createAndConfigImage() {
    UWORD Imagesize = ((EPD_2in13_V3_WIDTH % 8 == 0) ? (EPD_2in13_V3_WIDTH / 8) : (EPD_2in13_V3_WIDTH / 8 + 1)) * EPD_2in13_V3_HEIGHT;
    UBYTE *BlackImage = (UBYTE *)malloc(Imagesize);
    if(BlackImage == NULL) {
        Debug("Failed to apply for black memory...\r\n");
        exit(-1);
    }
    Paint_NewImage(BlackImage, EPD_2in13_V3_WIDTH, EPD_2in13_V3_HEIGHT, 90, WHITE);
    Paint_Clear(WHITE);
    return BlackImage;
}

void finalizeEPD(UBYTE *BlackImage) {
    EPD_2in13_V3_Sleep();
    free(BlackImage);
    DEV_Delay_ms(2000);
    DEV_Module_Exit();
}

// Initialize E-Paper Display (EPD)
void initEPD() {
    Debug("EPD_2in13_V3_test Demo\r\n");
    if(DEV_Module_Init() != 0) {
        exit(-1);
    }
    EPD_2in13_V3_Init();
}

// Measure the time required to clear the EPD
void measureClearTime() {
    struct timespec start = {0,0}, finish = {0,0};
    clock_gettime(CLOCK_REALTIME, &start);
    EPD_2in13_V3_Clear();
    clock_gettime(CLOCK_REALTIME, &finish);
    Debug("%ld S\r\n", finish.tv_sec - start.tv_sec);
}


int main(void)
{
    signal(SIGINT, Handler); // Exception handling:ctrl + c
    
    initEPD();
    measureClearTime();
    UBYTE *BlackImage = createAndConfigImage();

    // number_box_main(BlackImage);
    
    EPD_2in13_V3_Display_Base(BlackImage);
    finalizeEPD(BlackImage);
    return 0;
}

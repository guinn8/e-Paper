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


int main(void)
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    
    initEPD();
    measureClearTime();
    UBYTE *BlackImage = createAndConfigImage();

    number_box_main(BlackImage);
    EPD_2in13_V3_Display_Base(BlackImage);
    finalizeEPD(BlackImage);

    return 0;
}

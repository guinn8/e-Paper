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
    number_box_main();

    return 0;
}

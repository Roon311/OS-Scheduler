#include "headers.h"
#include <time.h>
/* Modify this file as needed*/
int remainingtime;
int working;
void handler(int signum);
int main(int agrc, char *argv[])
{
    sigset(SIGALRM, SIG_DFL);
    signal(SIGALRM, handler);
    initClk();
    remainingtime = atoi(argv[1]);
    //printf("\n IN PROOOOOCESS %d\n\n", remainingtime);
    working = atoi(argv[1]);

    // TODO it needs to get the remaining time from somewhere
    int c;
    printf("\n Entering the process at time = %d\n\n",getClk());

    while (remainingtime > 0)
    {
      //   c = clock() / CLOCKS_PER_SEC;
      // //  printf("\n This is the c: %d \n\n", c);
      //   remainingtime = working - c;
      alarm(1);
      pause();
      remainingtime--;
    }
   printf("\n Exiting the process at time =%d\n\n",getClk());

   /// destroyClk(false);
    return 0;
}

void handler(int signum)
{

}
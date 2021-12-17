/** @file process.c
 *  @brief it automates a single process, that is only CPU bound
 *        in real OS, this might be a real process like any prorgram you run.
 */


#include "headers.h"
#include <time.h>


int remainingtime;
int working;

/** @brief handler to ignore some SIGALRM signal
 *  @return void.
 * 
 */
void handler(int signum);


int main(int agrc, char *argv[])
{
    signal(SIGALRM, handler);
    initClk();
    remainingtime = atoi(argv[1]);
    working = atoi(argv[1]);

    int c;
    /*
        While there is still remaining time process must take on CPU continue
    */
    while (remainingtime > 0)
    {
      alarm(1);
      pause();
      remainingtime--;
    }

    return 0;
}

void handler(int signum)
{

}
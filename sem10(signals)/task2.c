#include <stdio.h>
#include <signal.h>

void handler(int input) {
    //ignore 
}

int main()
{
    signal(SIGQUIT, &handler);

    while(1);
    return 0;
}
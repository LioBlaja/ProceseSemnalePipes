// 4. Să se scrie un program C, care își definește un comportament nou la recepția semnalului SIGUSR1.
// Programul va afișa un mesaj de fiecare dată când acest semnal este primit.
// Testarea programului se va face prin trimiterea semnalului SIGUSR1 dintr-un alt terminal, folosind comanda ps aux pentru a afla PIDul și comanda kill pentru a trimite semnalul.

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

void handler(int signum){
    printf("AM PRIMIT SEMNAL\n");
    exit(0);
}

int main(void){

    int pid = -1;

    if((pid = fork()) < 0){
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        //child code
        struct sigaction sg;
        memset(&sg,0, sizeof(struct sigaction));
        sg.sa_handler = handler;
        sg.sa_flags = SA_RESTART;

        if(sigaction(SIGUSR1,&sg,NULL) < 0){
            perror(NULL);
            exit(EXIT_FAILURE);
        }

        while(1){
            printf("RUNNING...\n");
            sleep(1);
        }
        exit(0);
    }

    sleep(5);

    if(kill(pid,SIGUSR1) < 0){
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    int wstatus= -1;

    if(waitpid(pid,&wstatus,0) < 0){
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    if (WIFEXITED(wstatus)) {
        printf("exited, status=%d\n", WEXITSTATUS(wstatus));
    } else {
        printf("ERROR KILLING PROC\n");
    }

    return 0;
}

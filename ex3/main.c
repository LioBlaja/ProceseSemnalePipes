#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

// 3. Să se scrie un program multiproces care generează parole și le scrie într-un fișier.
// Programul va primi ca și argumente: numărul de procese, numărul de parole per proces, dimensiunea unei parole și o cale către un fișier de ieșire în care va scrie parolele:
// ./program <numar_procese> <numar_parole> <dimensiune_parola> <cale_fisier_iesire>

// Programul va crea atâtea procese câte sunt specificate în primul argument. Fiecare proces va genera atâtea parole cât este specificat în al doilea argument.
// Parola reprezintă un string de dimensiune specificată în al treilea argument ce conține orice caracter printabil fără spațiu.
// Fiecare parolă generată de un proces fiu va fi trimisă printr-un pipe procesului părinte. 
// Procesul părinte va colecta parolele și le va scrie în fișierul reprezentat de calea dată ca și ultim argument. Procesul părinte va scrie la stdout câte un mesaj pentru fiecare proces fiu care s-a terminat.


void generate_password(char *password, int length) {
    const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    int charset_size = sizeof(charset) - 1;

    for (int i = 0; i < length; i++) {
        password[i] = charset[rand() % charset_size];
    }
    password[length] = '\0';  // Terminația de string
}

int main(int argc,char** argv){

    if(argc != 5){
        printf("INVALID ARGUMENTS\n");
        exit(EXIT_FAILURE);
    }

    int procNumber = atoi(argv[1]);
    int passwordNumber = atoi(argv[2]);
    int passwordLength = atoi(argv[3]);

    int pfd[procNumber][2];
    int pid[procNumber];
    
    for(int i = 0;i < procNumber;i++){
        if(pipe(pfd[i])<0)
        {
            perror("Eroare la crearea pipe-ului\n");
            exit(1);
        }

        if((pid[i] = fork()) < 0){
            perror("ERROR ON CREATING CHILD PROC");
            exit(EXIT_FAILURE);
        }

        if(pid[i] == 0){
            close(pfd[i][0]);  // Închidem capătul de citire

            for (int j = 0; j < passwordNumber; j++) {
                char password[passwordLength + 1];
                generate_password(password, passwordLength);
                // printf("PASS: %s\n",password);
                // Scriem parola generată în pipe
                if (write(pfd[i][1], password, strlen(password) + 1) < 0) {
                    perror("Write to pipe failed");
                    exit(EXIT_FAILURE);
                }
            }

            close(pfd[i][1]);  // Închidem pipe-ul după scriere
            exit(EXIT_SUCCESS);
        }
    }
    int fdOut = -1;

    if((fdOut = open(argv[4],O_WRONLY | O_TRUNC | O_CREAT , S_IRWXU )) < 0){
        perror("ERROR OPENING OUT FILE");
        exit(EXIT_FAILURE);
    }

    for(int i = 0;i < procNumber;i++){
        //parent code
        close(pfd[i][1]);
        // sleep(4);
        char password[passwordLength + 1];
        int bytesRead = 0;

        while((bytesRead = read(pfd[i][0],&password,sizeof(password))) > 0){
            // printf("Password from proc %d: %s\n",i,password);
            char buffer[100];
            sprintf(buffer,"%s\n",password);
            if (write(fdOut, buffer, strlen(buffer)) < 0) {
                perror("Write to output failed");
                exit(EXIT_FAILURE);
            }
        }

        if (bytesRead < 0) {
            perror("Read failed");
        }

        int wstatus;

        if(waitpid(pid[i],&wstatus,0) < 0){
            printf("ERROR WAITING CHILD PROC");
            close(pfd[i][1]);
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(wstatus)) {
            printf("process:%d\n",WEXITSTATUS(wstatus));
        } else {
            printf("killed by signal %d\n", WTERMSIG(wstatus));
        }
        close(pfd[i][0]);
    }

    close(fdOut);

    return 0;
}

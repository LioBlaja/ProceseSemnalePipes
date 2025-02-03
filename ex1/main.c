// 1. Să se scrie un program C care primește ca și argument în linie de comandă calea către un director de pe disc.
// Programul va parcurge directorul primit ca și argument, va procesa în paralel conținutul tuturor fișierelor obișnuite identificate și va calcula numărul total de litere mici din acestea. 
// INDICIU: se va crea câte un proces copil pentru fiecare fișier obișnuit identificat și acestea vor apela exit având ca și parametru numărul de litere mici pentru fișierul curent.
// Părintele va prelua informația aceasta folosind wait, WIFEXITED și WEXITSTATUS.

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

int wc(const char* path){
    //97 122
    int counter = 0;
    char temp;
    int fdIn = -1;

    if((fdIn = open(path,O_RDONLY)) < 0){
        perror("ERROR OPENING FILE");
        exit(EXIT_FAILURE);
    }

    while(read(fdIn,&temp,1) > 0){
        if(temp >= 'a' && temp <= 'z'){
            // printf("LETTER:%c",temp);
            counter++;
        }
    }

    return counter;
}

void parcurgereDir(char* path){

    struct dirent * rd;
    DIR *dirStream = NULL;
    struct stat sb;
    char newPath[1000];

    if((dirStream = opendir(path)) == NULL){
        perror("ERROR ON OPENING DIR");
        exit(EXIT_FAILURE);
    }

    while((rd = readdir(dirStream)) != NULL){
        if(strcmp(rd->d_name,"..") == 0 || strcmp(rd->d_name,".") == 0)
            continue;

        sprintf(newPath,"%s/%s",path,rd->d_name);

        if (lstat(newPath, &sb) == -1) {
               perror("lstat");
               exit(EXIT_FAILURE);
        }

        switch (sb.st_mode & S_IFMT) {
            case S_IFBLK:  printf("block device\n");            break;
            case S_IFCHR:  printf("character device\n");        break;
            case S_IFDIR:  
                // printf("directory\n");              
                parcurgereDir(newPath);  
                break;
            case S_IFIFO:  printf("FIFO/pipe\n");               break;
            case S_IFLNK:  printf("symlink\n");                 break;
            case S_IFREG:  
                // printf("regular file\n");

                

                // int pid = -1;

                // if((pid = fork()) < 0){
                //     perror("ERROR CREATING CHILD");
                //     exit(EXIT_FAILURE);
                // }

                // if(pid == 0){
                //     //child code
                //     exit(wc(newPath));
                // }

                // int wstatus = -1;

                // if(waitpid(pid,&wstatus,0) < 0){
                //     printf("ERROR WWAITING CHILD PROC");
                //     if(closedir(dirStream) < 0){
                //         perror("ERROR CLOSING DIR");
                //         exit(EXIT_FAILURE);
                //     }
                //     exit(EXIT_FAILURE);
                // }

                // if (WIFEXITED(wstatus)) {
                //     printf("FISIERUL %s are %d litere mici\n",newPath ,WEXITSTATUS(wstatus));
                // } else {
                //     printf("killed by signal %d\n", WTERMSIG(wstatus));
                // }

                break;
            case S_IFSOCK: printf("socket\n");                  break;
            default:       printf("unknown?\n");                break;
        }

        // printf("PATH:%s\n",newPath);
    }

    if(closedir(dirStream) < 0){
        perror("ERROR CLOSING DIR");
        exit(EXIT_FAILURE);
    }
}

int main(int argc,char** argv){
    if(argc != 2){
        printf("INVALID ARGUMENTS\n");
        exit(EXIT_FAILURE);
    }

    parcurgereDir(argv[1]);

    return 0;
}

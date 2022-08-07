#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include "connection.h"
#define SIZE 1024

/*void send_file(FILE *fp, int sockfd){
     char data[30] = {0};

     while(fgets (data, 30, fp) != NULL){
           if(send(sockfd, data, sizeof(data), 0) == -1){
                perror("[-] Error in sending data");
                exit(1);
           }
           bzero(data, 30);
     }
}*/

int cautare_cuv(char a[20][20], char cuv[50]){
  int i;
  fprintf(stderr,"\n");
  for(i=0;i<20;i++){
    if(strncmp(a[i],cuv,strlen(a[i]))==0)
      return 0;
  }
  return -1;
}

int main(void *args)
{
    struct sockaddr_un addr;
    int ret;
    int data_socket, sockfd;
    char buffer[BUFFER_SIZE];
    int i,j,t,l, nrvec=0;
    char cuvant[50]="hey";
    char cuv[50];
    char rezc[10];
    char rez2[10];
    int score=0;
    FILE *fp;
    char *filename1 = "scor_aclient.txt";
    char *filename2 = "Scoruri_Jucatori_aclient.txt";
    char arr[20][20];
    char cuv_scor[50]="ClientAdmin:";
    char c[5];
    char cuvdinfis[50];
    char cuvdinserver[50];
    int fa, fna;
    /* Create local socket. */

    data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (data_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = connect(data_socket, (const struct sockaddr *) &addr,
                   sizeof(addr));
    if (ret == -1) {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }

    ret = read(data_socket, buffer, 30);
    if (ret == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    t=0;
    for(l=0;l<20;l++){
      strcpy(arr[l],"empty");
    }

    for(i=0;i<5;i++){
      for(j=0;j<5;j++){
        printf("%c ", buffer[nrvec]);
        nrvec++;
      }
      printf("\n");
    }
    printf("Client de administrare\n");
    printf("----COMENZI----\n");
    printf("STOP---opreste aplicatia\n");
    printf("NEXT---genereaza alt tablou\n");
    printf("KCI---termina clientul ordinar\n");

    printf("------REGULI----\n");
    printf("Alege literele din tablou care formeaza cuvinte in engleza\n");
    printf("Literele trebuie sa fie una langa alta in tablou\n");


    for(;;){
      strcpy(cuv," ");
      fgets(cuv, 10, stdin);

      if (strncmp(cuv,"STOP",4)==0){
        ret = write(data_socket,cuv,strlen(cuv)+1);
        if (ret == -1){
          perror("WRITE");
          exit(EXIT_FAILURE);
        }
        break;
      }
      ret=write(data_socket,cuv,strlen(cuv)+1);
      if (ret == -1) {
          perror("write1");
          exit(EXIT_FAILURE);
          break;
      }

      if (strncmp(cuv,"KCI",3)==0){
        ret = write(data_socket,cuv,30);
        if (ret == -1){
          perror("write");
          exit(EXIT_FAILURE);
        }
      }
      else{
        if (strncmp(cuv,"NEXT",4)==0){
          t=0;
          for(l=0;l<20;l++){
            strcpy(arr[l],"empty");
          }
          ret = read(data_socket,cuv,30);
          if (ret == -1){
            perror("read1");
            exit(EXIT_FAILURE);
            break;
          }
          nrvec = 0;
          for(i=0;i<5;i++){
            for(j=0;j<5;j++){
              fprintf(stderr,"%c ", cuv[nrvec]);
              nrvec++;
            }
            fprintf(stderr,"\n");
          }
        }
        else{

          ret = read(data_socket,rez2,sizeof(rez2));
          if (ret == -1){
            perror("read2");
            exit(EXIT_FAILURE);
            break;
          }
          if(strncmp(rez2,"0",1)==0){

            printf("Cuvantul exista in dictionar\n");
            ret = read(data_socket,rezc,sizeof(rezc));
            if (ret == -1){
              perror("read3");
              exit(EXIT_FAILURE);
              break;
            }

            if(strncmp(rezc,"0",1)==0){
              fprintf(stderr,"CORECT\n");
              if (cautare_cuv(arr,cuv)==-1){
                score=score+(strlen(cuv)-1);
                t=t+1;
                strcpy(arr[t], cuv);
              }
              else{
                fprintf(stderr, "Cuvantul a mai fost folosit odata\n");
              }
              fprintf(stderr, " Score: %d\n", score);

            }
            else{
              fprintf(stderr,"INCORECT!!\n");
            }

            nrvec=0;
          }
          else
          printf("Cuvantul nu este in dictionar\n");

      }
    }
}

    sprintf(c,"%d", score);
    strcat(cuv_scor, c);
    printf("%s\n",cuv_scor);

    fa = open(filename1, O_RDWR|O_CREAT|O_APPEND|O_TRUNC, S_IROTH|S_IWOTH|S_IXOTH|S_IRUSR|S_IWUSR|S_IXUSR);
    fna = open(filename2, O_RDWR|O_CREAT|O_APPEND|O_TRUNC, S_IROTH|S_IWOTH|S_IXOTH|S_IRUSR|S_IWUSR|S_IXUSR);

    write(fa, cuv_scor, strlen(cuv_scor));
    lseek(fa,0L,SEEK_SET);
    read(fa,cuvdinfis,50);
    send(data_socket,cuvdinfis,strlen(cuvdinfis),0);

    while(recv(data_socket,cuvdinserver,50,0)>0){
      write(fna,cuvdinserver,strlen(cuvdinserver));
    }
    
    close(fa);
    close(fna);
    close(data_socket);

    exit(EXIT_SUCCESS);
}

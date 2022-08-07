#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#define PORT 8080
#define SIZE 1024

void send_file(FILE *fp, int client_fd){

        char data[SIZE] = {0};

    while(fgets(data, SIZE, fp)!=NULL)
    {
        if(send(client_fd, data, sizeof(data), 0)== -1)
        {
            perror("[-] Error in sending data");
            exit(1);
        }
        bzero(data, SIZE);
    }


}

int main(void *args)
{
  int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	char* hello = "Hello from client";
	char buffer[1024] = { 0 };
  int i,j,ret,nrvec=0;
  char cuv[50];
  char k[25];
  char rezc[10];
  char rez2[10];
  int score=0;
  FILE *fp;
  char *filename = "file1.txt";
  char arr[1000];

  fp=fopen(filename, "w");

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((client_fd
		= connect(sock, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

  ret = read(sock,k,25);
  if(ret == -1){
    perror("read");
    exit(EXIT_FAILURE);
  }

  for(i=0;i<5;i++){
    for(j=0;j<5;j++){
      printf("%c ", k[nrvec]);
      nrvec++;
    }
    printf("\n");
  }
  nrvec = 0;
  printf("----COMENZI----\n");
  printf("STOP---opreste aplicatia\n");
  printf("NEXT---genereaza alt tablou\n");

  printf("------REGULI----\n");
  printf("Alege literele din tablou care formeaza cuvinte in engleza\n");
  printf("Literele trebuie sa fie una langa alta in tablou\n");

  for(;;){
    strcpy(cuv," ");
    fgets(cuv, 30, stdin);
    ret = write(sock,cuv,30);
    if (ret == -1) {
        perror("write1");
        exit(EXIT_FAILURE);
        break;
    }
    if(strncmp(cuv,"STOP",4)==0){
      printf("S-a oprit clientul!!\n");
      break;
    }
    if(strncmp(cuv,"NEXT",4)==0){
      ret = read(sock,k,25);
      if(ret == -1){
        perror("read");
        exit(EXIT_FAILURE);
        break;
      }
      for(i=0;i<5;i++){
        for(j=0;j<5;j++){
          printf("%c ", k[nrvec]);
          nrvec++;
        }
        printf("\n");
      }
      nrvec=0;
    }
    else{
      ret = read(sock,rez2,sizeof(rez2));
      if (ret == -1){
        perror("read2");
        exit(EXIT_FAILURE);
        break;
      }
      if(strncmp(rez2,"0",1)==0){


        printf("Cuvantul exista in dictionar\n");

        ret = read(sock,rezc,sizeof(rezc));
        if (ret == -1){
          perror("read3");
          exit(EXIT_FAILURE);
          break;
        }

        if(strncmp(rezc,"0",1)==0){
          fprintf(stderr,"CORECT\n");
          score=score+(strlen(cuv)-1);
          fprintf(stderr, "Score: %d", score);
          fprintf(stderr, "\n");
          fprintf(fp, "CLient: ");
          fprintf(fp, "%d\n", score);

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
     fp = fopen(filename, "r");
     if(fp == NULL)
     {
         perror("[-]Error in reading file.");
         exit(1);
     }
     send_file(fp,client_fd);
     printf("[+] File data send successfully. \n");


	// closing the connected socket
	close(client_fd);
	return 0;
}

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <sys/stat.h>

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

/*void write_file(int data_socket){
    int n;
    int fp;
    char *filename = "Scoruri_Jucatori_iclient.txt";
    char buffer[30];
    char *nl="\n";
    fp = open(filename, O_WRONLY|O_CREAT|O_APPEND, S_IROTH|S_IWOTH|S_IXOTH);

    while(1)
    {
        n = recv(data_socket, buffer, 30, 0);
        if(n<=0)
        {
            break;
            return;
        }
        printf("___________AICI__________:%s\n",buffer);
        write(fp, nl, strlen(nl));
        write(fp, buffer, strlen(buffer));
        bzero(buffer, 30);
        close(fp);
    }
    return;
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

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0, client_fd;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;
    int i,j,l,ret,t=0,nrvec=0;
    char k[25];
    char cuv[50];
    char rez2[10];
    char rezc[10];
    int score=0;
    char arr[20][20];
    int result=0;
    FILE *fp, *dp;
    int fi,fni;
    char *filename = "scor_iclient.txt";
    char *filename1 = "Scoruri_Jucatori_iclient.txt";
    char cuv_scor[50]= "ClientInet:";
    char c[5];
    char cuvdinfis[50];
    char cuvdinserver[50];
    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }

    memset(k,0,sizeof(k));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if((client_fd = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))<0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    ret = read(sockfd,k,25);
    if(ret == -1){
      perror("read");
      exit(EXIT_FAILURE);
    }

    t=0;
    for(l=0;l<20;l++){
      strcpy(arr[l],"empty");
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
      fgets(cuv, 30, stdin);
      ret = write(sockfd,cuv,30);
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
        t=0;
        for(l=0;l<20;l++){
          strcpy(arr[l],"empty");
        }
        ret = read(sockfd,k,25);
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
        ret = read(sockfd,rez2,sizeof(rez2));
        if (ret == -1){
          perror("read2");
          exit(EXIT_FAILURE);
          break;
        }
        printf("%s\n", rez2);
        if(strncmp(rez2,"0",1)==0){

          printf("Cuvantul exista in dictionar\n");
          ret = read(sockfd,rezc,sizeof(rezc));
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
            fprintf(stderr, "Score: %d\n", score);
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

    sprintf(c, "%d", score);
    strcat(cuv_scor, c);
    printf("%s\n", cuv_scor);

    fi = open(filename, O_RDWR|O_CREAT|O_APPEND|O_TRUNC, S_IROTH|S_IWOTH|S_IXOTH|S_IRUSR|S_IWUSR|S_IXUSR);
    fni = open(filename1, O_RDWR|O_CREAT|O_APPEND|O_TRUNC, S_IROTH|S_IWOTH|S_IXOTH|S_IRUSR|S_IWUSR|S_IXUSR);

    write(fi,cuv_scor,strlen(cuv_scor));
    lseek(fi,0L,SEEK_SET);
    read(fi,cuvdinfis,50);
    send(sockfd,cuvdinfis,strlen(cuvdinfis),0);

    while(recv(sockfd,cuvdinserver,50,0)>0){
      write(fni,cuvdinserver,strlen(cuvdinserver));
    }
    
    close(fi);
    close(fni);

    close(sockfd);
    return 0;
}

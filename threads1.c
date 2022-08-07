#include "connection.h"
#include <errno.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>


#define UNIXSOCKET "/tmp/unixds"
#define INETPORT 18081
#define PORT 8080
#define SIZE 1024

volatile int thread_exit = 1;
volatile int pause_th = 0;
int cond_var = 0;
int err;

pthread_mutex_t lock;

/*void send_file(int fp, int sockfd);

void write_file(int connfd);

void write_file1(int data_socket);*/

int cautare(char arr[], int n, char x);

void litere_aleatoare(char k[]);

void creare_tablou(char a[][5], char k[]);

int cautfis(FILE *fisier, char *cuvant);

int verificare_cuvant(char a[][5], char cuv[50]);

void *unix_main(void *args);

void *inet_main(void *args);

int main() {
  int iport;
  pthread_t unixthr, inetthr, workthr;
  FILE *fp;



  unlink(UNIXSOCKET);

  pthread_create(&unixthr, NULL, unix_main, UNIXSOCKET);
  iport = INETPORT;
  pthread_create(&inetthr, NULL, inet_main, NULL);

  pthread_join(unixthr, NULL);
  pthread_join(inetthr, NULL);

  unlink(UNIXSOCKET);



  return 0;
}

/*void send_file(int fp, int sockfd){
     char data[30] = {0};
     fprintf(stderr,"HEEEEEEEEEEEEEEY");
     while(read(fp, data, 30) != -1){
           fprintf(stderr,"Data:%s", data);
           if(send(sockfd, data, sizeof(data), 0) == -1){
                perror("[-] Error in sending data");
                exit(1);
           }
           fprintf(stderr,"Sending successfull:%s", data);
           bzero(data, 30);
     }
}

void write_file(int data_socket){
    int n;
    int fp;
    char *filename = "Scoruri_Jucatori.txt";
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
        write(fp, nl, strlen(nl));
        write(fp, buffer, strlen(buffer));
        bzero(buffer, 30);
    }
    close(fp);
    return;
}

void write_file1(int data_socket){
  int n;
  int fp;
  char *filename = "Scoruri_Jucatori.txt";
  char buffer[30];

  fp = open(filename, O_WRONLY|O_CREAT|O_APPEND, S_IROTH|S_IWOTH|S_IXOTH);
  fprintf(stderr, "se scrie--------------\n");
  while(1)
  {
      n = read(data_socket, buffer, 30);
      printf("%s\n", buffer);
      if(n<=0)
      {
        break;
      }
      write(fp, buffer, strlen(buffer));
      bzero(buffer, 30);
  }
  fprintf(stderr, "s-a scriiiiiiiiiiiiiiiis\n");
  return;
}*/

int cautare(char arr[], int n, char x) {
  int i;
  for (i = 0; i < n; i++)
    if (arr[i] == x)
      return i;
  return -1;
}

void litere_aleatoare(char k[]) {
  char *litere = "abcdefghijklmnopqrstuvwxyz";
  int i;
  int t;
  int rez;
  int lower = 0, upper = 25;
  for (i = 0; i <= 24; i++) {
    t = rand() % (upper - lower + 1) + lower;
    k[i] = litere[t];
    rez = cautare(k, i, k[i]);
    if (rez > -1) {
      i = i - 1;
    } else {
      printf("%c ", k[i]);
    }
  }
}

void creare_tablou(char a[][5], char k[]) {
  int i, j;
  int nrvec = 0;
  for (i = 0; i <= 4; i++) {
    for (j = 0; j <= 4; j++) {
      a[i][j] = k[nrvec];
      nrvec++;
      printf("%c ", a[i][j]);
    }
    printf("\n");
  }
}

int cautfis(FILE *fisier, char *cuvant) {
  char a[50];
  int rez = -1;
  int i;
  fisier = fopen("dictionary.txt", "r");
  if (fisier == NULL) {
    perror("Fisierul nu exista");
    exit(1);
  }
  if (!fisier) {
    perror("Ceva nu e in regula");
    exit(1);
  }

  while (fgets(a, 50, fisier)) {
    if (strlen(cuvant) == strlen(a) - 1) {
      if (strncmp(cuvant, a, strlen(a) - 1) == 0) {
        // printf("ok found it ----------");
        rez = 0;
      }
    }
    // printf("%s %s %ld %ld ",a,cuvant,strlen(cuvant),strlen(a)-1);
    strcpy(a, "");
  }

  fclose(fisier);
  return rez;
}

int verificare_cuvant(char a[][5], char cuv[50]) {
  int i, j, nr = 0, r;
  char cuvptarray[50] = "";

  strcpy(cuvptarray, cuv);
  for (r = 0; r <= strlen(cuvptarray) - 1; r++) {
    fprintf(stderr, "%c ", cuvptarray[r]);
    for (i = 0; i <= 4; i++) {
      for (j = 0; j <= 4; j++) {
        if ((cuvptarray[r] == a[i][j]) && (r == 0)) {
          nr++;
          fprintf(stderr, "%d %d %d ", i, j, nr);
        }
        if ((cuvptarray[r] == a[i][j]) && (r >= 0)) {
          if (i == 0 && j == 0) // colt stanga sus
            if ((cuvptarray[r - 1] == a[i][j + 1]) ||
                (cuvptarray[r - 1] == a[i + 1][j + 1]) ||
                (cuvptarray[r - 1] == a[i + 1][j])) {
              nr++;
              fprintf(stderr, "%d %d %d ", i, j, nr);
            }
          if ((i == 0 && j > 0) && (i == 0 && j < 4)) // contur sus
            if ((cuvptarray[r - 1] == a[i][j + 1]) ||
                (cuvptarray[r - 1] == a[i + 1][j + 1]) ||
                (cuvptarray[r - 1] == a[i + 1][j]) ||
                (cuvptarray[r - 1] == a[i + 1][j - 1]) ||
                (cuvptarray[r - 1] == a[i][j - 1])) {
              nr++;
              fprintf(stderr, "%d %d %d ", i, j, nr);
            }
          if ((i < 4 && j == 0) && (i > 0 && j == 0)) // contur stanga
            if ((cuvptarray[r - 1] == a[i][j + 1]) ||
                (cuvptarray[r - 1] == a[i + 1][j + 1]) ||
                (cuvptarray[r - 1] == a[i + 1][j]) ||
                (cuvptarray[r - 1] == a[i - 1][j + 1]) ||
                (cuvptarray[r - 1] == a[i - 1][j])) {
              nr++;
              fprintf(stderr, "%d %d %d ", i, j, nr);
            }
          if (i == 0 && j == 4) // colt dreapta sus
            if ((cuvptarray[r - 1] == a[i][j - 1]) ||
                (cuvptarray[r - 1] == a[i + 1][j - 1]) ||
                (cuvptarray[r - 1] == a[i + 1][j])) {
              nr++;
              fprintf(stderr, "%d %d %d ", i, j, nr);
            }
          if ((i < 4 && j == 4) && (i > 0 && j == 4)) // contur dreapta
            if ((cuvptarray[r - 1] == a[i + 1][j]) ||
                (cuvptarray[r - 1] == a[i - 1][j - 1]) ||
                (cuvptarray[r - 1] == a[i][j - 1]) ||
                (cuvptarray[r - 1] == a[i - 1][j - 1]) ||
                (cuvptarray[r - 1] == a[i - 1][j])) {
              nr++;
              fprintf(stderr, "%d %d %d ", i, j, nr);
            }
          if (i == 4 && j == 4) // colt dreapta jos
            if ((cuvptarray[r - 1] == a[i - 1][j]) ||
                (cuvptarray[r - 1] == a[i - 1][j - 1]) ||
                (cuvptarray[r - 1] == a[i][j - 1])) {
              nr++;
              fprintf(stderr, "%d %d %d ", i, j, nr);
            }
          if ((i == 4 && j > 0) && (i == 4 && j < 4)) // contur jos
            if ((cuvptarray[r - 1] == a[i][j + 1]) ||
                (cuvptarray[r - 1] == a[i - 1][j + 1]) ||
                (cuvptarray[r - 1] == a[i - 1][j]) ||
                (cuvptarray[r - 1] == a[i - 1][j - 1]) ||
                (cuvptarray[r - 1] == a[i][j - 1])) {
              nr++;
              fprintf(stderr, "%d %d %d ", i, j, nr);
            }
          if (i == 4 && j == 0) // colt stanga jos
            if ((cuvptarray[r - 1] == a[i][j + 1]) ||
                (cuvptarray[r - 1] == a[i - 1][j + 1]) ||
                (cuvptarray[r - 1] == a[i - 1][j])) {
              nr++;
              fprintf(stderr, "%d %d %d ", i, j, nr);
            }
          if ((i > 0 && j > 0) && (i < 4 && j < 4))
            if ((cuvptarray[r - 1] == a[i][j + 1]) ||
                (cuvptarray[r - 1] == a[i - 1][j + 1]) ||
                (cuvptarray[r - 1] == a[i - 1][j]) ||
                (cuvptarray[r - 1] == a[i - 1][j - 1]) ||
                (cuvptarray[r - 1] == a[i][j - 1]) ||
                (cuvptarray[r - 1] == a[i + 1][j + 1]) ||
                (cuvptarray[r - 1] == a[i + 1][j]) ||
                (cuvptarray[r - 1] == a[i + 1][j - 1])) {
              nr++;
              fprintf(stderr, "%d %d %d ", i, j, nr);
            }
        }
      }
    }
  }
  if (nr == strlen(cuvptarray)) {
    fprintf(stderr, "Corect: %d\n", nr);
    return 0;
    nr = 0;
  } else {
    fprintf(stderr, "Incorect!!\n");
    return -1;
    nr = 0;
  }
}

void *unix_main(void *args) {
  struct sockaddr_un name;
  int down_flag = 0;
  int ret;
  int connection_socket;
  int data_socket;
  int result;
  char buffer[BUFFER_SIZE];
  char k[25];
  int i, j;
  char a[5][5];
  char cuvant[50];
  char *spatiu = " ";
  char *b = "";
  char cuvantnou[50];
  FILE *dexfrom, *dexto;
  int rez1;
  char cuv[50];
  char rezc[10];
  char rez2[10];
  int fp,sj;
  char *filename = "Scoruri_Jucatori.txt";
  char cuvdinclient[50];
  char *nl = "\n";
  char cuvdinfis[50];

  printf("\n");
  litere_aleatoare(k);
  printf("\n");
  creare_tablou(a, k);

  strcpy(cuvant, k);
  printf("%s\n", cuvant);

  connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (connection_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&name, 0, sizeof(name));

  name.sun_family = AF_UNIX;
  strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

  ret = bind(connection_socket, (const struct sockaddr *)&name, sizeof(name));
  if (ret == -1) {
    perror("bind");
    unlink(SOCKET_NAME);
    exit(EXIT_FAILURE);
  }

  ret = listen(connection_socket, 20);
  if (ret == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  data_socket = accept(connection_socket, NULL, NULL);
  if (data_socket == -1) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  strcpy(buffer, cuvant);
  ret = write(data_socket, buffer, strlen(buffer) + 1);
  if (ret == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  for (;;){
    ret = read(data_socket, buffer, sizeof(buffer));
    if (ret == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    strcpy(cuvantnou, buffer);
    strncat(cuv, cuvantnou, strlen(cuvantnou) - 1);

    if ((strncmp(cuv, "STOP", 4) == 0)) {
      break;
    }
    if((strncmp(cuv, "KCI", 3)==0)){
      down_flag = 1;
      thread_exit=2;
      printf("thread_exit:%d\n",thread_exit);
    }

    if ((strncmp(cuv, "NEXT", 4) == 0)) {
      litere_aleatoare(k);
      creare_tablou(a, k);
      strcpy(cuvant, k);
      ret = write(data_socket, cuvant, strlen(cuvant) + 1);
      if (ret == -1) {
        perror("write1");
        exit(EXIT_FAILURE);
      }
    } else {
      printf("%ld %s\n", strlen(cuv), cuv);
      rez1 = cautfis(dexfrom, cuv);
      printf("%d\n", rez1);
      sprintf(rez2, "%d", rez1);
      ret = write(data_socket, rez2, sizeof(rez2));
      if (ret == -1) {
        perror("write2");
        exit(EXIT_FAILURE);
      }
      if (rez1 == 0) {
        printf("Cuvantul exista in dictionar\n");
        result = verificare_cuvant(a, cuv);
        sprintf(rezc, "%d", result);
        ret = write(data_socket, rezc, sizeof(rezc));

        if (ret == -1) {
          perror("write3");
          exit(EXIT_FAILURE);
        }
      } else {
        printf("Cuvantul nu este in dictionar\n");
      }
    }
    strcpy(cuv, b);

    buffer[sizeof(buffer) - 1] = 0;
  }

    sj=open(filename, O_RDWR|O_CREAT|O_APPEND, S_IROTH|S_IWOTH|S_IXOTH|S_IRUSR|S_IWUSR|S_IXUSR);
    bzero(cuvdinclient,50);
    recv(data_socket,cuvdinclient,50,0);
    write(sj,nl,strlen(nl));
    write(sj,cuvdinclient,strlen(cuvdinclient));
    lseek(sj,0L,SEEK_SET);


    while(read(sj,cuvdinfis,50)>0){
      printf("%s\n", cuvdinfis);
      send(data_socket,cuvdinfis,strlen(cuvdinfis),0);
      bzero(cuvdinfis,50);
    }
    //read(sj,cuvdinfis,50);
    //send(data_socket,cuvdinfis,strlen(cuvdinfis),0);

    close(sj);
    cond_var = 1;

    close(data_socket);

    close(connection_socket);

    unlink(SOCKET_NAME);

    exit(EXIT_SUCCESS);

}

void *inet_main(void *args) {
  int server_fd, new_socket, valread;
  int listenfd = 0, connfd = 0;
  struct sockaddr_in serv_addr;
  int opt = 1;
  char buffer[1024] = {0};
  char *hello = "Hello from server";
  char k[25];
  int i, j, fp;
  char a[5][5];
  char cuvant[50];
  char *spatiu = " ";
  char *b = "";
  char cuvantnou[50];
  FILE *dexfrom, *dexto;
  int rez1;
  char cuv[50] = "";
  char rezc[10];
  char rez2[10];
  int ret;
  int result;
  int sj;
  char *filename = "Scoruri_Jucatori.txt";
  char cuvdinclient[50];
  char cuvdinfis[50];

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0))==0){
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  memset(k, 0, sizeof(k));

  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000);

  if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(listenfd, 10) < 0){
    perror("listen");
    exit(EXIT_FAILURE);
  }

  if((connfd = accept(listenfd, (struct sockaddr*)NULL, NULL))<0){
    perror("accept");
    exit(EXIT_FAILURE);
  }


  litere_aleatoare(k);
  printf("\n");
  creare_tablou(a, k);

  ret = write(connfd, k, 25);
  if (ret == -1) {
    perror("write1");
    exit(EXIT_FAILURE);
  }

  for(;;){

    ret = recv(connfd, cuvantnou, 30, 0);
    if (ret == -1) {
      perror("read1");
      exit(EXIT_FAILURE);
      break;
    }

    printf("%ld %s\n", strlen(cuvantnou), cuvantnou);
    if(cuvantnou[strlen(cuvantnou)-1] == '\n'){
      printf("%ld %s\n", strlen(cuvantnou), cuvantnou);
      cuvantnou[strlen(cuvantnou)-1] = '\0';
    }

    printf("%s\n", cuvantnou);

    if (strncmp(cuvantnou, "STOP", 4) == 0) {
      printf("S-a oprit serverul!!\n");
      break;
    }

/************thread exit***************/

    if(thread_exit==2){
      printf("Inet-Clientul a fost deconectat\n");
      break;
    }

/**********thread exit*****************/

    if (strncmp(cuvantnou, "NEXT", 4) == 0) {
      litere_aleatoare(k);
      creare_tablou(a, k);
      printf("%s\n",k);
      ret = write(connfd, k, 25);
      if (ret == -1) {
        perror("write1");
        exit(EXIT_FAILURE);
        break;
      }
    } else {
      rez1 = cautfis(dexfrom, cuvantnou);
      printf("%d\n", rez1);
      sprintf(rez2, "%d", rez1);
      ret = write(connfd, rez2, 1);
      if (ret == -1) {
        perror("write2");
        exit(EXIT_FAILURE);
      }

      if (rez1 == 0) {
        printf("Cuvantul exista in dictionar\n");
        result = verificare_cuvant(a, cuvantnou);
        sprintf(rezc, "%d", result);
        ret = write(connfd, rezc, 1);
        if (ret == -1) {
          perror("write3");
          exit(EXIT_FAILURE);
        }
      } else {
        printf("Cuvantul nu este in dictionar\n");
      }
    }
    bzero(cuvantnou, 30);
  }

  sj=open(filename, O_RDWR|O_CREAT|O_APPEND|O_TRUNC, S_IROTH|S_IWOTH|S_IXOTH|S_IRUSR|S_IWUSR|S_IXUSR);
  bzero(cuvdinclient,50);
  recv(connfd,cuvdinclient,50,0);
  write(sj,cuvdinclient,strlen(cuvdinclient));

  while(1){
    if(cond_var==1){
      lseek(sj,0L,SEEK_SET);
      while(read(sj,cuvdinfis,50)>0){
        send(connfd,cuvdinfis,strlen(cuvdinfis),0);
        bzero(cuvdinfis,50);
      }
      close(sj);
      break;
    }
  }

  close(connfd);
  shutdown(listenfd, SHUT_RDWR);
  return 0;
}

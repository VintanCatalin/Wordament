#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include "connection.h"

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
                (cuvptarray[r - 1] == a[i - 1][j - 1]) ||
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

int main(int argc, char *argv[]){
    int server_fd, new_socket =0, valread;
    int listenfd = 0, connfd = 0, opt = 1;
    struct sockaddr_in serv_addr;
    int addrlen = sizeof(serv_addr);
    char sendBuff[1025];
    time_t ticks;
    char k[25];
    char a[5][5];
    int ret;
    char cuvantnou[50];
    char cuv[50];
    int down_flag;
    char *b="";
    int rez1,result;
    char rezc[10];
    char rez2[10];
    FILE *dexfrom, *dexto;


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

      ret = read(connfd, cuvantnou, 30);
      if (ret == -1) {
        perror("read1");
        exit(EXIT_FAILURE);
        break;
      }
      strcpy(cuv,b);

      strncat(cuv, cuvantnou, strlen(cuvantnou) - 1);

      printf("%s\n", cuv);

      if (strncmp(cuv, "STOP", 4) == 0) {
        printf("S-a oprit serverul!!\n");
        break;
      }

      if (strncmp(cuv, "NEXT", 4) == 0) {
        litere_aleatoare(k);
        creare_tablou(a, k);
        ret = write(connfd, k, 25);
        if (ret == -1) {
          perror("write1");
          exit(EXIT_FAILURE);
          break;
        }
      } else {
        printf("%ld %s\n", strlen(cuv), cuv);
        rez1 = cautfis(dexfrom, cuv);
        printf("%d\n", rez1);
        sprintf(rez2, "%d", rez1);
        ret = write(connfd, rez2, sizeof(rez2));
        if (ret == -1) {
          perror("write2");
          exit(EXIT_FAILURE);
        }

        if (rez1 == 0) {
          printf("Cuvantul exista in dictionar\n");
          result = verificare_cuvant(a, cuv);
          sprintf(rezc, "%d", result);
          ret = write(connfd, rezc, sizeof(rezc));
          if (ret == -1) {
            perror("write3");
            exit(EXIT_FAILURE);
          }
        } else {
          printf("Cuvantul nu este in dictionar\n");
        }
      }
      strcpy(cuv, b);
    }
    close(connfd);
    shutdown(listenfd, SHUT_RDWR);
    return 0;
}

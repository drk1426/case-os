#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#inlcude<pthread.h>
#include<time.h>

#define customers 5
#define resources 3

int Proc[5][3];
int temp[5][3];
int available[resources];
int max[customers][resources];
int allocation[customers][resources] = {{1,2,3},{3,2,1},{1,1,1},{1,1,1},{1,1,1}};
int need[customers][resources];
int counti = 0;
int countj = 0;
int threadsi = 5;
int threadsj = 3;

void *inc_count(void *r);
void *watch_count(void *r);

pthread_mutex_t mutex;
pthread_cond_t count_threshold_cv;

int main()
{
  long r1=1,r2=2,r3=3;
  srand(time(NULL));
  int x,y;
    for(x=0;x<customers;x++){
      for(y=0;y<resources;y++){
        if(y==0){
            max[x][y] = rand()%r1+1;
          }
        else if(y==1){
            max[x][y] = rand()%r2+1;
          }
        else{
            max[x][y] = rand()%r3+1;
          }
        }
      }
    pthread_t Proc[5][3];
    pthread_attr_t attr;
    int i,j;
    printf("Enter Resource 1: ");
    scanf("%ld",&r1);
    printf("Enter Resource 2: ");
    scanf("%ld",&r2);
    printf("Enter Resource 3: ");
    scanf("%ld",&r3);

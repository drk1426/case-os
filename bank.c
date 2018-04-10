#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

#define process 5
#define resources 3

int Proc[5][3];
int temp[5][3];
int available[resources];
int max[process][resources];
int allocation[process][resources] = {{1,2,3},{3,2,1},{1,1,1},{1,1,1},{1,1,1}};
int need[process][resources];
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
    for(x=0;x<process;x++){
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
    if(pthread_mutex_init(&mutex,NULL)<0){
     perror("Pthread_mutex_init error.");
     exit(1);    
     }
  else
     //pthread_mutex_init(&mutex,NULL);  
  pthread_cond_init(&count_threshold_cv,NULL);
  pthread_attr_init(&attr);
  pthread_create(&Proc[0][0],&attr,watch_count,(void *)r1);
  pthread_create(&Proc[1][0],&attr,inc_count,(void *)r2);
  pthread_create(&Proc[2][0],&attr,inc_count,(void *)r3);
  for(i=0;i<=threadsi;i++){ 
      for(j=0;j<=threadsj;j++){ 
          pthread_join(Proc[i][j],NULL);
          }
      }
  printf("waited on %d,%d threads\n",threadsi,threadsj);  
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&count_threshold_cv);
  pthread_exit(NULL);
}
void *inc_count(void *r)
{ 
  int i,j,n,m;
  long id =(long)r;
  for(i=0;i<10;i++){
      for(j=0;j<10;j++){
    need[n][m] =max[n][m] - allocation[i][j];
    printf("Allocation = %d,Need = %d\n",allocation[i][j],need[n][m]);
    }
    pthread_mutex_lock(&mutex);
    if(counti == process && countj == resources){
       pthread_cond_signal(&count_threshold_cv);
       printf("thread %ld,Need = %d\n",id,need[n][m]);
       }
    printf("thread %ld,Need = %d\n",id,need[n][m]);
    pthread_mutex_unlock(&mutex);
    sleep(1);
    watch_count(r);
    }
  pthread_exit(NULL);
  watch_count(r);
}
void *watch_count(void *r)
{
  long id = (long)r;
  int n,m;
  printf("thread %ld\n",id);
  while(counti<process && countj<resources)
  { 
   pthread_mutex_lock(&mutex);
   available[n] = max[n][m] - allocation[counti++][countj++];
   printf("Available = %d\n",available[n]);
   pthread_cond_wait(&count_threshold_cv,&mutex);
   printf("thread %ld,available = %d\n",id,available[m]);
   countj++;
   printf("thread %ld,Need now = %d\n",id,need[counti][countj]);
  }
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

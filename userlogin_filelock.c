#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <setjmp.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>

pthread_mutex_t mutex_lock   = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   thread_cond  = PTHREAD_COND_INITIALIZER;

jmp_buf jmpbuffer;

int i = 0;
int timer=0;

void retry(); 

typedef struct ACCOUNT
{
   char id[10];
   char pw[10];
} admin;

admin cre;

//파일을 수정할 수 있는 권한을 줌
void *file_change(void *data)
{
    struct tm *tm;
    struct timeval tv;
    
    pid_t pid;       
    pthread_t tid;   
 
    pid = getpid();
    tid = pthread_self();
 
    char* thread_name = (char*)data;
    int i = 0;

    pthread_mutex_lock(&mutex_lock);
    gettimeofday( &tv, NULL );
    tm = localtime( &tv.tv_sec );
    printf("%d년 %d월 %d일 %d시 %d분 %d초 파일 접근 Success\n", tm->tm_year +1900, tm->tm_mon +1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    ////////////////////////
    if(chmod("test.txt", 00644) == -1)
      perror("chmod failed");

    printf("파일에 대한 접근이 %d초 동안 가능해졌습니다.\n", timer);

    sleep(timer);

    printf("파일에 대한 접근이 차단되었습니다.\n", timer);

    if(chmod("test.txt", 00000) == -1)
    perror("chmod failed");
    
    /////////////////////////
    gettimeofday( &tv, NULL );
    tm = localtime( &tv.tv_sec );
    printf("%d년 %d월 %d일 %d시 %d분 %d초 파일 접근 권한 만료\n\n", tm->tm_year +1900, tm->tm_mon +1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    retry();
    pthread_mutex_unlock(&mutex_lock);
     return 0;
}

//로그인에 실패
 void fail(){
   i=0;
   printf("error\n");
   printf("login fail, 20초 뒤에 다시 시도하세요.\n");
   sleep(20);
   longjmp(jmpbuffer, 1);
}

//파일 이용시간 만료 이후 파일에 대한 재접근을 위한 과정
 void retry(){

         admin log;
         pthread_t threadID1;
         pthread_mutex_init(&mutex_lock,NULL);
         pthread_cond_init(&thread_cond,NULL);

         char a;
         i = 0;
         timer = 0;
         printf("파일을 계속 이용하려면 다시 로그인 하세요\n");
         printf("파일을 계속 이용하시겠습니까?[Y/N] : ");
         scanf("%s", &a);

         if(a == 'Y' || a == 'y'){
            printf("파일 이용시간을 정해주세요. \n시간은 초 단위로 입력 해야하며 최대 300초까지 가능합니다. : ");
            scanf("%d", &timer);
            if(timer > 300){
            printf("프로그램 오류 : 입력값이 300 이상입니다. 프로그램을 종료합니다.");
            return;
            }
         }
         else{
            printf("파일을 이용하지 않습니다. 프로그램을 종료합니다.");
            return;
         }

         printf("파일에 접근하려면 로그인을 해야합니다. 로그인 해 주세요.\n");

      while(1){  
         alarm(30);
         setjmp(jmpbuffer);
         printf("아이디를 30초 내로 입력하세요 : ");
         scanf("%s",log.id);
         printf("패스워드를 30초 내로 입력하세요 : ");
         scanf("%s",log.pw);

         if( strcmp(log.id, cre.id) == 0 && strcmp(log.pw, cre.pw) == 0 ){
         alarm(0);
         printf("login success.\n\n");
         printf("\n");

         pthread_create(&threadID1, NULL, file_change, NULL);
         pthread_join(threadID1, NULL);

         pthread_mutex_init(&mutex_lock,NULL);
         pthread_cond_destroy(&thread_cond);

         return;
         }

         else if( strcmp(log.id, cre.id) != 0 ){
         printf("login fail\n");
         printf("아이디가 일치하지 않습니다. 다시 입력하세요.\n");
         }
         else if( strcmp(log.pw, cre.pw) != 0 ){
         printf("login fail\n");
         printf("패스워드가 일치하지 않습니다. 다시 입력하세요.\n");
         }

         if(i==4){
            fail();
         }
         i++;
      }
}


int main()
{
   admin log;

   i = 0;
   timer=0;
   char c;

   printf("회원가입을 시작합니다.\n\n");
   printf("아이디를 생성하세요(10자 이내) : ");
   scanf("%s", cre.id);  
   printf("패스워드를 생성하세요(10자 이내) : ");
   scanf("%s", cre.pw); 
   printf("회원가입 성공\n\n\n");

   printf("파일을 이용하시겠습니까?[Y/N] : ");
   scanf("%s", &c); 

   if(c == 'Y' || c == 'y'){
      printf("파일 이용시간을 정해주세요. \n시간은 초 단위로 입력 해야하며 최대 300초까지 가능합니다. : ");
   }
   else{
      printf("파일을 이용하지 않습니다. 프로그램을 종료합니다.");
      return 0;
   }

   scanf("%d", &timer);
   if(timer > 300){
      printf("프로그램 오류 : 입력값이 300 이상입니다. 프로그램을 종료합니다.");
      return 0;
   }

   printf("파일에 접근하려면 로그인을 해야합니다. 로그인 해 주세요.\n");


   signal(SIGALRM, fail);

   

      while(1){  
         pthread_t threadID1, threadID2;
         pthread_mutex_init(&mutex_lock,NULL);
         pthread_cond_init(&thread_cond,NULL);


         alarm(30);
         setjmp(jmpbuffer);
         printf("아이디를 30초 내로 입력하세요 : ");
         scanf("%s",log.id);
         printf("패스워드를 30초 내로 입력하세요 : ");
         scanf("%s",log.pw);

         if( strcmp(log.id, cre.id) == 0 && strcmp(log.pw, cre.pw) == 0 ){
         alarm(0);
         printf("login success.\n");
         printf("\n");

         pthread_create(&threadID1, NULL, file_change, NULL);
         pthread_join(threadID1, NULL);

         pthread_mutex_init(&mutex_lock,NULL);
         pthread_cond_destroy(&thread_cond);

         break;
         }

         else if( strcmp(log.id, cre.id) != 0 ){
         printf("login fail\n");
         printf("아이디가 일치하지 않습니다. 다시 입력하세요.\n");
         }
         else if( strcmp(log.pw, cre.pw) != 0 ){
         printf("login fail\n");
         printf("패스워드가 일치하지 않습니다. 다시 입력하세요.\n");
         }

         if(i==4){
            fail();
         }
         i++;
      } 
return 0;  

}



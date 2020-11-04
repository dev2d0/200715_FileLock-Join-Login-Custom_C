### 200615_FileLock--Join-Login-Custom_C
## 파일잠금, 사용자 지정 설정 프로그램 

# 요약
사용자 인터랙션에 따라 회원가입 과정을 진행하고, 파일에 접근 하려면 로그인 과정을 통해 권한을 부여받아야 합니다.<br/> 옳바른 루트를 통해 로그인에 성공했다면 파일에 접근 할 수 있는 권한을 얻습니다. <br/>
회원가입과 로그인 기능을 통한 파일 접근 권한 부여 및 사용자가 직접 사용시간을 설정할 수 있도록 하였습니다.

# 기능
주요기능 1 : 회원가입과 로그인 기능을 통한 사용자 식별기능.<br/>
주요기능 2 : 멀티쓰레드 기능과 뮤텍스락 기능을 이용하여 자원 관리.<br/>
주요기능 3 : 시그널과 알람을 통한 사용자 지정 시간 설정.<br/>
주요기능 4 : 접근 권한을 변경하여 파일에 대한 접근을 부여 및 차단.<br/>
주요기능 4 : 접근 권한을 변경하여 파일에 대한 접근을 부여 및 차단.<br/>


# 구현 내용 및 방법 
1. 유저인터페이스에 따라 회원가입을 진항한다.
2. 파일의 이용여부를 체크힌다.
3. 파일을 이용할 시간을 설정한다.
4. 파일을 이용하려면 로그인 작업을 수행한다. 
5. 파일 접근 성공 시간을 알려준다.
6. 파일 접근 시간동안 파일을 수정 할 수 있다.
7. 파일 접근 시간이 만료되면 만료 시간을 공지함과 함께 종료한다.
8. 파일 수정이 아직 끝나지 못한 경우를 대비해 재접근 의사를 물어본다.
9. 재접근을 하기 원한다면 파일 이용시간을 설정한다.
10. 로그인 기능을 진행한다.
11. 파일이용이 종료 된 후 재접근을 하지 않겠다는 N입력 전까지 8~10번 과정을 반복한다.

# 동작 실험 
![image](https://user-images.githubusercontent.com/39155520/98086561-50062200-1ec2-11eb-81f2-e4e1a8fc8035.png)
![image](https://user-images.githubusercontent.com/39155520/98086600-5ac0b700-1ec2-11eb-9e1d-baf288754efc.png)

파일 접근 권한 부여 받았을 때
![image](https://user-images.githubusercontent.com/39155520/98086637-657b4c00-1ec2-11eb-87a9-341ea1524692.png)
파일 접근 권한이  때
![image](https://user-images.githubusercontent.com/39155520/98086225-cce4cc00-1ec1-11eb-9f90-3236be2cb9a8.png)


# 시스템호출과 라이브러리 

#include <signal.h><br/>
#include <unistd.h><br/>
#include <setjmp.h><br/>
#include <sys/types.h><br/>
#include <fcntl.h><br/>
#include <pthread.h><br/>
#include <unistd.h><br/>
#include <sys/time.h><br/>
#include <sys/stat.h><br/>

pthread_mutex_t mutex_lock   = PTHREAD_MUTEX_INITIALIZER;<br/>
pthread_cond_t   thread_cond  = PTHREAD_COND_INITIALIZER;<br/>
jmp_buf jmpbuffer;<br/>
pthread_mutex_lock(&mutex_lock);<br/>
pthread_mutex_unlock(&mutex_lock);<br/>
sleep(timer);<br/>
chmod("test.txt", 00644)<br/>
gettimeofday( &tv, NULL );<br/>
alarm(30);<br/>
setjmp(jmpbuffer);<br/>
pthread_create(&threadID1,NULL, file_change, NULL);<br/>
pthread_join(threadID1, NULL);<br/>
pthread_mutex_init(&mutex_lock,NULL);<br/>
pthread_cond_destroy(&thread_cond);<br/>
등등<br/>

파일에 대한 접근 권한을 변경 할 수 있다.<br/>
현재 시간을 받아와 날짜를 설정 할 수 있다.<br/>
쓰래드의 생성과 뮤텍스를 통해 자원을 효율적으로 관리 할 수 있다.<br/>
시그널과 알람을 통해 시간제한을 설정 할 수 있다,<br/>





# 세마포어
- [참고사이트](https://www.joinc.co.kr/w/Site/system_programing/IPC/semaphores)
- 상호배제 알고리즘으로 critical region 을 만들어 리소스를 보호한다.
- 차단을 원하는 리소스에 대해 세마포어를 생성하면 해당 리소스를 가리키는 세마포어 값이 할당된다.
    > 세마포어 값을 검사하여 critical region 에 접근할 수 있는지 결정


# POSIX 세마포어
> POSIX 세마포어 함수를 사용하기 위해서는 -lrt로 리얼타임 라이브러리를 링크해야 한다.

- 세마포어 종류
    > sem_open : 이름 있는 세마포어
    > sem_init : 이름 없는 세마포어

### sem_open : 세마포어 초기화, 열기

```
#include <semaphore.h>
            
sem_t *sem_open(	const char *name,
                    int oflag,
                    ...);

name : 세마포어 오브젝트에 이름을 정하는 문자열
    '/' 문자로 시작해야한다. 다른 slash 문자가 있으면 안된다.
oflag : 생성하는지 접근만 할지 결정하는 값
```
- 이름이 명명된 세마포어와 프로세스간의 연결을 수립
- sem_open() 에서 얻은 세마포어의 이름으로 세마포어를 참조할 수 있다.
- 해당 세마포어는 sem_wait(), sem_timedwait(), sem_trywait(), sem_post()와 sem_close() 에서 사용할수 있다.
- sem_close(), _exit(), exec 함수를 호출하여 세마포어를 닫기 전까지 사용 가능

- oflag
    ```
    O_CREAT
        이름이 명명된 세마포어를 생성한다. 이미 존재한다면 생성하지 않는다. O_CREATE 플래그는 세번째, 네번째 매개 변수 인자를 필요로 한다: mode_t인 mode (S_IWUSR, S_IWGRP,	and S_IWOTH) 와 unsigned 인 value이다. 세마포어는 value의 값을 초기값으로 생성된다. value는 {SEM_VALUE_MAX}와 같거나 작은 값이 유효값이다.

        세마포어의 user ID는 프로세스의 유효한 user ID에 설정된다. 세마포어의 group ID 프로세스의 유효한 group ID에 설정된다; 하지만 만일 매개 변수 name이 파일시스템에 있다면, group ID는 포함하는 디렉터리의 group ID에 설정될 수 있다. 세마포어의 권한 비트는 매개 변수 mode 의 값에 설정된다.

        세마포어가 O_CREAT플래그를 가진 sem_open()함수 수행으로 name으로 명명된 후, 다른 프로세스는 그 세마포어에 sem_open()을 name를 인자로 하여 호출함으로써 접근할 수 있다.

    O_EXCL
        만일 O_EXCL와 O_CREAT 이 설정되었고, 세마포어 이름이 존재한다면 sem_open()함수 수행은 실패한다. 만일 O_EXCL설정되었고 O_CREAT이 설정되지 않은 경우 함수 수행에 대한 결과는 정의되지 않는다.

        O_CREAT와 O_EXCL이외의 플래그가 oflag에 설정된 경우의 함수 수행결과는 정의 되지 않는다.
    ```
- 리턴
    > o : 세마포어 주소
    > x : SEM_FAILED, errno
        ```<semaphore.h>```



### sem_post : 세마포어 잠금 해제
> 뮤텍스 unlock 과 비슷 

```
#include <semaphore.h>
            
int sem_post(	sem_t *sem);

sem : 해제할 세마포어
```

- sem 이 참조하는 세마포어의 잠금을 해제한다.
- sem_post 호출후 세마포어 값 양수 : 해제될때까지 기다려야함
- sem_post 호출후 세마포어 값 0    : block 된 스레드들 중 하나가 sem_wait() 함수를 호출해서 세마포어를 사용할 수 있게됨
- 세마포어 값 증가
- 리턴
    > o : 0
    > x : -1, errno
        유효하지 않은 세마포어를 참조



### sem_wait : 세마포어 잠금
> 뮤텍스 lock 과 비슷
```
#include <semaphore.h>
            
int sem_wait(	sem_t *sem);
```

- sem 이 참조하는 세마포어를 잠근다.
- 함수수행이 성공이라면 세마포어의 상태는 잠금상태가 되며 sem_post() 를 호출해서 성공적으로 호출이 끝날때까지 잠금상태를 유지한다.
- 세마포어 값 0 보다 큼 : 세마포어를 얻고 세마포어 값을 감소하고 함수 종료
- 세마포어 값 0 : 세마포어가 0 보다 커지거나 시그널발생할때까지 대기

- 리턴
    > o : 0
    > x : -1, errno
        sem_trywait() 와 sem_wait() 는 다음의 경우에 실패한다.

        EDEADLK
        데드락 상태가 감지되는 경우.

        EINTR
        시그널이 함수수행을 중단하는 경우

        EINVAL
        매개 변수 sem이 유효하지 않은 세마포어를 참조하는 경우



### sem_unlink : 이름있는 세마포어 제거

```
#include <semaphore.h>
            
int sem_unlink(	const char *name);

```

- 성공할 경우 ‘0’ 값을 반환한다. 그렇지 않으면 값 ‘-1’을 반환하고 errno를 설정하여 오류를 표시한다.
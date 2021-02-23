
- [참고 사이트](https://www.joinc.co.kr/w/Site/system_programing/Book_LSP/ch07_Thread)

# fork 의 단점

- fork 를 사용해서 프로그램을 병렬로 실행 시킬 수 있다.
- 프로세스의 5가지 구성요소 (code, data, stack, file I/O, signal table) 을 복사해서 프로세스를 생성한다.
    > 자원이 많이 필요해 성능저하

- fork 를 사용하는 이유가 특정 코드를 병렬로 실행하기 위해서인데 프로그램 전체가 복사되어 불필요한 내용도 있다.

- 프로세스가 새로 생성되기 때문에 프로세스간의 통신을 따로 처리해줘야한다.
    > 리눅스는 IPC 를 제공
    > IPC(Inter-Process Communicaton) : 파일, 신호, 소켓, 파이프, 세마포어, 공유 메모리 등등
    > [참고사이트](https://ko.wikipedia.org/wiki/%ED%94%84%EB%A1%9C%EC%84%B8%EC%8A%A4_%EA%B0%84_%ED%86%B5%EC%8B%A0)


# thread

- 새로운 프로세스를 생성하지 않고 특정 코드만을 병렬로 실행할 수 있도록 한다.

### 스레드가 필요한 이유
- 과거에는 cpu 당 하나의 코어만 있어서 명령어를 하나밖에 실행할수 없었다.
    > 여러 실행흐름을 만들어 여러 명령어를 수행할 수 없었기 때문에 여러 프로세스를 만들어(= 멀티 태스킹) 동시에 실행되는 것처럼 구현했다.

- 요즘에는 CPU 의 코어 개수가 증가하여 CPU 가 여러 명령어를 실행할 수 있게 됐다.
    > 특정 시점에 여러 실행 흐름을 만듣 수 있음

### 쓰레드, 프로세스의 차이점
```
프로세스는 독립적이다. 쓰레드는 프로세스의 서브셋이다.
프로세스는 각각 독립적인 자원을 가진다. 쓰레드는 stat, memory 기타 다른 자원들을 공유한다.
프로세스는 자신만의 주소영역을 가진다. 쓰레드는 주소영역을 공유한다.
프로세스는 IPC(:12)를 이용해서만 통신이 가능하다.
일반적으로 쓰레드의 문맥교환(context switching)는 프로세스의 문맥교환보다 빠르다.
```


### PThread (POSIX 스레드)
- [참고 사이트](https://ko.wikipedia.org/wiki/POSIX_%EC%8A%A4%EB%A0%88%EB%93%9C)
- 병렬적으로 작동하는 소프트웨어를 위해서 제공되는 표준 API
- 리눅스, 유닉스에서 사용가능s



### 스레드 생성, 종료 과정
1. 멀티 스레드도 시작할때 main() 에서 단일 스레드 상태로 작동
2. pthread_create() 함수를 호출해 새로운 스레드 생성 : worekr 스레드
3. Master Thread 에서 pthread_join() 함수를 사용해 worekr 스레드들의 종료를 가다림
    pthread_join() : 종료된 스레드의 자원을 정리
    > 멀티 프로세스 프로그램에서 부모 프로세스에서 wait 로 기다리는 것과 비슷하다

### pthread_create
- 새로운 스레드 생성
```
#include <pthread.h>

int  pthread_create(pthread_t  *  thread, pthread_attr_t *attr, 
     void * (*start_routine)(void *), void * arg);
```

이 함수는 새로운 스레드를 스레드 속성 attr에 따라 생성한다. 스레드 속성 객체 attr이 NULL이라면 기본 스레드 속성으로 스레드를 생성한다. 스레드가 성공적으로 생성되면 생성된 스레드 ID는 thread에 저장된다.

생성된 스레드는 start_routine을 arg 인자를 사용하여 실행한다. 만약 start_routine이 반환되면 내부적으로 pthread_exit() 함수가 호출되어 스레드가 종료된다.


thread : 쓰레드가 성공적으로 생성되었을 때, 넘겨주는 쓰레드 식별 번호.
attr : 쓰레드의 특성을 설정하기 위해서 사용한다. NULL(:12)일 경우 기본 특성
start_routine : 쓰레드가 수행할 함수로 함수포인터(:12)를 넘겨준다.
arg : 쓰레드 함수 start_routine를 실행시킬 때, 넘겨줄 인자

리턴 값
    성공 : 0
    실패 : 1


### pthread_join
- 스레드 정리
- 스레드가 실행시키는 함수에서 return, exit(0) 을 이용해서 스레드를 종료할 수 있지만 바로 스레드의 모든 자원이 종료되진 않음
- 종료된 스레드를 기다리고 정리해줘야한다. : 메모리 누수 방지
```
#include <pthread.h>

int pthread_join(pthread_t th, void **thread_return);
```
th : pthread_create에 의해서 생성된, 식별번호 th를 가진 쓰레드를 기다리겠다는 얘기다.
thread_return : 식별번호 th인 쓰레드의 종료시 리턴값이다.

- join 을 할 수 있는 스레드만 해당된다.
- 스레드를 생성할때 join 을 하지 않도록 만들 수도 있나봄 : detach

- 스레드가 종료될떄 까지 블럭된다.
    > 뒷 코드의 내용이 바로 실행되지 않는다.


### pthread_detach
- 스레드를 프로세스로부터 분리한다.
```
#include <pthread.h>
            
int pthread_detach(	pthread_t thread);
```

thread : 분리할 스레드

- 자식 스레드가 종료되면 모든 자원이 즉시 반환된다.
- 자식 스레드의 종료상태를 알수 없다.
    > 전역 변수에다 설정을 한다던지 여러 방식으로 구현할 수 있음



# 접근 제어
- 스레드들이 자원을 공유하는 경우 읽기/쓰기 과정에서 데이터 갱신이 안될수 있다.



### mutex
> [참고사이트](https://www.joinc.co.kr/w/Site/Thread/Beginning/Mutex)
- ptrhead 에서 제공하는 동기화 메커니즘, 공유 자원 공간에 대한 접근 시간 제어로 동기화를 수행
- pthread 를 통해 임계 영역을 생성하여 여러 스레드가 하나의 자원에 효율적으로 접근하도록
- 하나의 스레드만 임계영역에 접근할 수 있다. (= mutex 잠금을 얻는다.)
- 임계영역에서 접근한 스레드가 임계영역을 나가면 다른 스레드가 사용할 수 있음

### mutex 사용 순서
1. mutex 잠금 객체를 만드는 함수
    > 뮤텍스를 생성하기전에 pthread_mutex_t 를 선언해주고 초기화해줘야한다.
    > PTHREAD_MUTEX_INITIALIZER 값을 할당해줘도 된다.
    > pthread_mutex_init() 함수를 사용해서 pthread_mutex_t 주소를 인자로 넘겨주어 초기화할 수 있음

2. mutex 잠금을 얻는 함수
3. mutex 잠금을 되돌려주는 함수
4. mutex 잠금 제거 함수

### 1. pthread_mutex_init()
```
#include <pthread.h>
            
int pthread_mutex_init(	pthread_mutex_t *restrict mutex,
 	                    const pthread_mutexattr_t *restrictattr);

```
- c 에서의 객체 : 메모리
- mutex 잠금 객체 생성
    > pthread_mutex_t : 뮤텍스 정보를 저장하는 타입
    
- mutex : mutex 잠금 객체
- restrictattr 
    > NULL 또는 mutex 속성 객체의 주소
    > NULL 일 경우 시스템 기본 속성으로 객체를 초기화한다.


### 2. pthread_mutex_lock()
```
#include <pthread.h>
            
int pthread_mutex_lock(	pthread_mutex_t *mutex);
```
- mutex 객체를 잠근다.
- 호출한 스레드가 mutex 객체를 잠그도록 한다.
    > 잠그려는 뮤텍스가 다른 쓰르데에서 이미 잠궈 놓았다면 잠금을 얻을때까지(다른 쓰레드가 해제할때) 블럭한다.
- 호출한 스레드는 객체가 잠금 해제될 떄까지 기다린다.
- 이 함수가 리턴되었다면 호출한 함수는 뮤텍스 객체를 획득한것

- 속성 값에 따른 잠금설정 
    ```
    만약 뮤텍스 타입이 PTHREAD_MUTEX_NORMAL이라면, 데드락 감지를 지원하지 않는다. 따라서 뮤텍스를 다시 잠그려 한다면 뮤텍스는 데드락이 발생한다.

    만약 뮤텍스 타입이 PTHREAD_MUTEX_ERRORCHECK이라면, 데드락 감지를 지원한다 . 따라서 뮤텍스를 다시 잠그려 한다면 뮤텍스는 오류 값을 반환한다.

    만약 뮤텍스 타입이 PTHREAD_MUTEX_RECURSIVE이라면, 뮤텍스를 소유한 스레드의 반복적인 잠금을 허용한다.

    만약 뮤텍스 타입이 PTHREAD_MUTEX_DEFAULT로 설정된다면, PTHREAD_MUTEX_RECURSIVE과 같게 동작한다.
    ```

### 3. pthread_mutex_unlock()
```
#include <pthread.h>
            
int pthread_mutex_unlock(	pthread_mutex_t *mutex);
```
- mutex 객체의 잠금을 해제


### 예시

- 임게영역이 보호되지 않을 경우
```
int a = 1; d
Thread A 에서 a를 읽어들인다. 
Thread B 에서 a를 읽어들인다. 
Thread A 에서 a = a+1를 한다. 
{ 
    a = a + 1; 
    결과는 2; 
} 
Thread B 에서 a++를 한다. 
{ 
   a = a + 1; // 읽어들인 값이 1이기 때문에 
   역시 결과는 2가 된다. 
} 
```
- a 에 1 을 두번 더하기 때문에 3이 예상되지만 2 가됨



### pthread_join, pthread_detach 차이점

다만, join은 두 번째 인자인 void **형태의 반환값을 받아와서 또 다른 제어를 할 수 있다. 하지만 detach는 그런 것 없이 바로 할당된 메모리를 해제한다

 

그리고 join은 blocking, detach는 non-blocking 함수로, 프로세스인 메인 스레드에서 join의 사용은 뒷 코드의 내용이 바로 실행되지 않는다는 특징이 있다 

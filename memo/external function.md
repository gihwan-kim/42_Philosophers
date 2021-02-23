# gettimeofday

```
     #include <sys/time.h>

     int
     gettimeofday(struct timeval *tp, struct timezone *tzp);

     int
     settimeofday(const	struct timeval *tp, const struct timezone *tzp);
```
- 그리니치 시간, 현재 시간을 포함한 시스템 콜
- settimeofday() 시스템 콜로 설정 가능

- 'tp' 또는 'tzp' 가 널일 경우 시간은 설정, 리턴되지 않음

- timezone 구조체 : 로컬 시간 영역
- timezone 은 보통 사용안해서 NULL

```
struct timeval {
    time_t      tv_sec;
    suseconds_t tv_usec;
}
// tv_sec  : 초
// tv_usec : 마이크로 초
```

### 리턴 값
- 성공적으로 호출하면 0 을 리턴
- 실패 : -1, errno 설정


# pthread_mutex_destroy
```
int pthread_mutex_destroy(pthread_mutex_t *mutex);
```
인자로 주어진 뮤텍스 객체 mutex 를 제거하기 위해서 사용된다. mutex 는 pthread_mutex_init()함수를 이용해서 생성된 뮤텍스 객체이다.
pthread_mutex_destroy 를 이용해서 제대로 mutex 를 삭제하려면 이 mutex 는 반드시 unlock 상태이여야 한다.
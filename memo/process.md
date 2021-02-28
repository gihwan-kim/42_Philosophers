### kill
```
     #include <sys/types.h>
     #include <signal.h>

     int
     kill(pid_t	pid, int sig);
```
- pid 로 sig 값을 보낸다.
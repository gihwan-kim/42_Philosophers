# mutex

> critical section 을 가진 thread 들이 실행이 겹치지 않도록 단독으로 실행되게 해야함

서로 옆에 있는 철학자 A, B 는 필연적으로 포크 하나를 공유하게 되는데
어느 한쪽이 해당 포크를 사용하고 있을때 다른 한쪽은 그 포크를 사용하면 안된다.


=> 포크를 뮤텍스로 설정한다.
=> 포크를 내려 놓는다. : 잠금을 해재헨다.
=> 포크를 사용한다. : 잠금


쓰레드는 언제 까지 켜 두어야할까?


> 죽어야할 시간에 죽지 않는 문제
현재시간 - 마지막 식사시간 > time_to_die 

조건을 사용했는데 이전에 초기화를 진행할때 time_to_die 에 1000 을 곱해주었다.
=> usleep 함수를 사용하는줄 알고 1000을 곱해준것 같다.
=> usleep 의 인자는 밀리세컨드가 아닌 마이크로 세컨드이기 때문
이럴 경우 이미 두 시간의 차가 무조건 작게 되버렸다.


> 시간이 밀리는 문제
```
gihkim@gimgihwan-ui-MacBookPro philo_one % ./philoOne 4 400 200 100
0ms No.1 has take fork
0ms No.1 eating
0ms No.3 has take fork
0ms No.3 eating
200ms No.1 sleeping
200ms No.2 has take fork
201ms No.2 eating
201ms No.4 has take fork
201ms No.4 eating
201ms No.3 sleeping
304ms No.1 thinking
304ms No.3 thinking
404ms No.2 sleeping
404ms No.4 sleeping
404ms No.1 has take fork
404ms No.1 eating
404ms No.3 has take fork
404ms No.3 eating
504ms No.2 thinking
504ms No.4 thinking
601ms No.1 died
```

- eating 을 처리하는 함수에서 마지막 식사 시간을 gettimeofday 함수로 체크한다.
- 식사를 시작한 후에 ’time_to_die’ 안에 식사를 해야한다는 조건이 있었는데
- '식사를 다 끝난 후에' 로 이해를 잘못해서 usleep 이후 현재시간을 측정하여 알고리즘적인 에러가 발생했다.


> 도중에 죽었는지 확인
각 철학자 별로 죽었는지 확인을 해주어야 하기 때문에

철학자 스레드마다 상태를 확인하는 스레드를 하나씩 부착해주었다.
각 철학자별로 마지막 식사 시간으로 부터 특정 시간 까지 식사를 하지 않을 경우 0 을 리턴하는 check_died() 함수를 구현해서 확인

한명이라도 죽을 경우 option 구조체의 is_died 멤버 변수의 값을 변경하도록 했다.
=> 각 철학자 스레드 별로 반복문의 조건으로 is_died 값을 체크하도록해서 반복문을 끝내도록 한다.


> 최소 식사 양 인자가 있을 경우
각 철학자별로 최소 n 번을 먹어야하기 때문에 모든 식사 횟수는 (철학자 수 * 최소 식사 횟수) 이므로 각 스레드 별로 공통된 변수를 사용해서 식사횟수를 늘려 확인한다.


> 199 명일떄 중간에 먹다가 죽어버린다.

예상 : pthread_create 를 호출할때 0 ~ n 까지 쭉 호출해버린다.
그냥 create 를 순서대로 호출해버리면 안될까?
-> 뮤텍스를 사용하는 이유가 없어지는것 같기도하다.
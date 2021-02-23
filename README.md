# 42_Philosophers

### 배울 수 있느 내용
```
쓰레딩
동일한 메모리 공간에서 작업
쓰레드 만드는 법
mutex, semaphore, 메모리 공유
```

### 프로그램 조건 

```
C, norm 체크

Any leak, crash, undefined
behavior or norm error means 0 to the project

eating, thinking, sleeping  3 개중 하나를 함

행동 하나를 하는 동안 다른 행동을 하지 않음

원탁에 앉아 있

포크 두개로 먹기

한손에 한 포크

굶주리면 안됨

모든 사람이 먹어야함

다른 사람이 죽을떄 죽었는지 알지못함

다 먹고나면 포크를 내려놓음, 잠을 잔다.

잠을 다 자면 생각을 한다.

죽으면 시뮬레이션 종료

서로 말을하지 않음

각 프로그램별로 동일한 옵션이 있다.
    number_of_philosophers
        사람수
        포크 수
    time_to_die
        밀리세컨드
        철학자가 마지막 식사를 시작한 후 ’time_to_die’ 밀리초 동안이나 시뮬레이션 시작 후 식사를 시작하지 않으면 사망합니다.
        if a philosopher doesn’t start eating ’time_to_die’
        milliseconds after starting his last meal or the beginning of the simulation, it
        dies
    time_to_eat
        밀리세컨드
        밥 먹는데 걸리는 시간
        밥먹는데 포크 2개 필요
    time_to_sleep
        밀리세컨드
        잠자는데 걸리는 시간
    number_of_times_each_philosopher_must_eat
        선택사항
        모든 사람이 적어도 ’number_of_times_each_philosopher_must_eat’ 만큼 먹으면 테스트는 종료됨
        해당 사항이 아닐 경우 사람이 죽으면 종료

사람 수 : 1 ~ ’number_of_philosophers’


1 번은 ’number_of_philosophers’ 옆에 있다.
철학자 번호 N 은 N - 1  과 N + 1 사이에 앉는다.

상태 변화는 형식에 맞게 작성하기
    X : 사람 번호
    timestamp_in_ms : 현재 시각, timestamp
    ◦ timestamp_in_ms X has taken a fork
    ◦ timestamp_in_ms X is eating
    ◦ timestamp_in_ms X is sleeping
    ◦ timestamp_in_ms X is thinking
    ◦ timestamp_in_ms X died

• The status printed should not be scrambled or intertwined with another philosopher’s status
상태 출력은 다른 사람과 엉키거나 섞이면 안됨
=> mutex 로 하라는 건가?

사람이 죽고, 사람이 죽었다는 문구를 출력하기까지 10ms 를 넘으면 안됨

죽는걸 피할것
```


# philo_one

```
Program name            philo_one
Turn in files           philo_one/
Makefile                Yes
Arguments               number_of_philosophers time_to_die time_to_eat
                        time_to_sleep [number_of_times_each_philosopher_must_eat]
External functs.        memset, printf, malloc, free, write,
                        usleep, gettimeofday, pthread_create,
                        pthread_detach, pthread_join, pthread_mutex_init,
                        pthread_mutex_destroy, pthread_mutex_lock,
                        pthread_mutex_unlock
Libft authorized        No
Description             philosopher with threads and mutex

각 사람 사이에 포크 하나씩(왼쪽, 오른쪽)

포크를 중복해서 포크의 상태를 

각 사람은 쓰레드를 사용해야함 뮤텍스를 사용해서 보호해야한다.

• One fork between each philosopher, therefore there will be a fork at the right and
at the left of each philosopher.
• To avoid philosophers duplicating forks, you should protect the forks state with a
mutex for each of them.
• Each philosopher should be a thread

동일한 포크를 사용할때 어떤식으로 처리해주지?
```

### number_of_times_each_philosopher_must_eat 가 '0' 일 경우는 ?

Error Handling
This project is to be coded in C, following the Norm.
Any crash, undefined behavior, memory leak or norm error means 0 to the project.

Philo_one code
    - Check the code of philo_one for the following things and ask for explanation.
    - Check if there is one thread per philosopher.
    - Check if there is a mutex per fork and that it's used to check the fork value and/or change it.
    - Check if the output is protected against multiple access. To avoid a scrambled view.
    - Check how the death of a philosopher is checked and if there is a mutex to protect that a philosopher dies and start eating at the same time.

Philo_one test
    - Do not test with more than 200 philosphers
    - Do not test with time_to_die or time_to_eat or time_to_sleep under 60 ms
    - Test with 5 800 200 200, no one should die!
    - Test with 5 800 200 200 7, no one should die and the simulation should stop when all the philosopher has eaten at least 7 times each.
    - Test with 4 410 200 200, no one should die!
    - Test with 4 310 200 100, a philosopher should die!
    - Test with 2 philosophers and check the different times (a death delayed by more than 10 ms is unacceptable).
    - Test your own values to check all the rules. Check if a philosopher dies at the right time, if they don't steal forks, etc.



Philo_two code
- Check the code of philo_two for the following things and ask for explanation.
- Check if there is one thread per philosopher.
- Check if there is a single semaphore that represents the number of forks.
- Check if the output is protected against multiple access. To avoid a scrambled view.
- Check how the death of a philosopher is checked and if there is a semaphore to protect that a philosopher dies and start eating at the same time.
Philo_two test
- Do not test with more than 200 philosphers
- Do not test with time_to_die or time_to_eat or time_to_sleep under 60 ms
- Test with 5 800 200 200, no one should die!
- Test with 5 800 200 200 7, no one should die and the simulation should stop when all the philosopher has eaten at least 7 times each.
- Test with 4 410 200 200, no one should die!
- Test with 4 310 200 100, a philosopher should die!
- Test with 2 philosophers and check the different times (a death delayed by more than 10 ms is unacceptable).
- Test your own values to check all the rules. Check if a philosopher dies at the right time, if they don't steal forks, etc.
Philo_three code
- Check the code of philo_three for the following things and ask for explanation.
- Check if there will be one process per philosopher and that the first process waits for all of them.
- Check if there is a single semaphore that represent the number of forks.
- Check if the output is protected against multiple access. To avoid a scrambled view.
- Check how the death of a philosopher is checked and if there is a semaphore to protect that a philosopher dies and start eating at the same time.
Philo_three test
- Do not test with more than 200 philosphers
- Do not test with time_to_die or time_to_eat or time_to_sleep under 60 ms
- Test with 5 800 200 200, no one should die!
- Test with 5 800 200 200 7, no one should die and the simulation should stop when all the philosopher has eaten at least 7 times each.
- Test with 4 410 200 200, no one should die!
- Test with 4 310 200 100, a philosopher should die!
- Test with 2 philosophers and check the different times (a death delayed by more than 10 ms is unacceptable).
- Test your own values to check all the rules. Check if a philosopher dies at the right time, if they don't steal forks, etc.

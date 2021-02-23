/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gihwan-kim <kgh06079@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 13:17:26 by gihwan-kim        #+#    #+#             */
/*   Updated: 2021/02/19 15:20:29 by gihwan-kim       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** pthread_creat, pthread_join 을 사용해보기
*/
// #include <pthread.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>

// void	*t_function(void *data)
// {
//     int id;
//     int i = 0;
//     id = *((int *)data);

//     while(1)
//     {
//         printf("%d : %d\n", id, i);
//         i++;
//         sleep(1);
//     }
// }


// int main()
// {
//     pthread_t p_thread[2];
//     int thr_id;
//     int status;
//     int a = 1;
//     int b = 2;

//     // 쓰레드 생성 아규먼트로 1 을 넘긴다.  
//     thr_id = pthread_create(&p_thread[0], NULL, t_function, (void *)&a);
//     if (thr_id < 0)
//     {
//         perror("thread create error : ");
//         exit(0);
//     }

//     // 쓰레드 생성 아규먼트로 2 를 넘긴다. 
//     thr_id = pthread_create(&p_thread[1], NULL, t_function, (void *)&b);
//     if (thr_id < 0)
//     {
//         perror("thread create error : ");
//         exit(0);
//     }

//     // 쓰레드 종료를 기다린다. 
//     pthread_join(p_thread[0], (void **)&status);
//     pthread_join(p_thread[1], (void **)&status);

//     return 0;
// }


#include <stdio.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <stdlib.h>

int ncount;    // 쓰레드간 공유되는 자원

typedef struct test
{
   pthread_mutex_t *left;
   pthread_mutex_t *right;
   char             *str;
} test1;

    pthread_mutex_t _write;

// pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER; // 쓰레드 초기화

// 쓰레드 함 수 1
void* do_loop(void *data)
{
    int i;
    test1 *Data = (test1*)data;
    
    while (1)
    {
        pthread_mutex_lock(Data->left); // 잠금을 생성한다.
        pthread_mutex_lock(Data->right); // 잠금을 생성한다.


        pthread_mutex_lock(&_write);
        printf("%s has fork\n", Data->str); 
        printf("eating %s\n", Data->str);
        pthread_mutex_unlock(&_write);
        
        usleep(300000);
        pthread_mutex_unlock(Data->left); // 잠금을 해제한다.
        pthread_mutex_unlock(Data->right); // 잠금을 해제한다.


        pthread_mutex_lock(&_write);        
        printf("sleeping %s\n", Data->str);
        pthread_mutex_unlock(&_write);
        
        usleep(300000);
    }

    return (NULL);
}

int main()
{
    int thr_id;
    pthread_t p_thread[4];
    int status;
    int a = 1;
    int b = 2;

    pthread_mutex_t *fork_arr = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * 4);

    pthread_mutex_init(fork_arr + 0, NULL);
    pthread_mutex_init(fork_arr + 1, NULL);
    pthread_mutex_init(fork_arr + 2, NULL);
    pthread_mutex_init(fork_arr + 3, NULL);

    test1 *data_1 = (test1*)malloc(sizeof(test1));
    test1 *data_2 = (test1*)malloc(sizeof(test1));
    test1 *data_3 = (test1*)malloc(sizeof(test1));
    test1 *data_4 = (test1*)malloc(sizeof(test1));
    data_2->str = "2222222";
    data_1->str = "111111";
    data_3->str = "3333333";
    data_4->str = "4444444";

    data_1->left = fork_arr;
    data_1->right = fork_arr + 3;
    data_2->left = fork_arr + 1;
    data_2->right = fork_arr;
    data_3->left = fork_arr + 2;
    data_3->right = fork_arr + 1;
    data_4->left = fork_arr + 3;
    data_4->right = fork_arr + 2;
    
    ncount = 0;
    thr_id = pthread_create(&p_thread[0], NULL, do_loop, data_1);
    usleep(20);
    thr_id = pthread_create(&p_thread[1], NULL, do_loop, data_2);
    usleep(20);
    thr_id = pthread_create(&p_thread[2], NULL, do_loop, data_3);
    usleep(20);
    thr_id = pthread_create(&p_thread[3], NULL, do_loop, data_4);
    usleep(20);

    pthread_join(p_thread[0], NULL);
    pthread_join(p_thread[1], NULL);
    pthread_join(p_thread[2], NULL);
    pthread_join(p_thread[3], NULL);
    // pthread_detach(p_thread + 0);

    return 0;
}

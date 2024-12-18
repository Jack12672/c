#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#define NB 12




typedef struct rep{
        int min;
        int max;
} rep_t;

typedef struct args{
    int thread;
    int val[20];
} args_t;

void *minmax(void *arguments){
    args_t *args = (args_t *) arguments;
    int thread = args->thread;
    int min=args->val[0];
    int max=args->val[0];
    printf("__start thread %d__\n",thread);
    for (int i=1; i<10;i++)
    {

        if (args->val[i]>max) max=args->val[i];
        if (args->val[i]<min) min=args->val[i];
    }
    rep_t *rep = malloc(sizeof(rep_t));
        rep->max = max;
        rep->min = min;
    return (void *) rep;
}

int main()
{
    srand(time(NULL));
    args_t arg[NB];
    pthread_t thread[NB];
    for (int t=0; t<NB;t++)
    {
        arg[t].thread=t;
        printf("thread %d  |  ",t);
        {
            for (int i=0; i<10;i++)
                {arg[t].val[i]=rand()%89+10;
                printf("%d | ",arg[t].val[i]);}
            printf("\n");

            pthread_create(&thread[t], NULL, minmax, &arg[t]);
        }
    }
    
    rep_t *return_value[NB];
    for (int t=0; t<NB;t++)
    {
        pthread_join(thread[t], (void *) &return_value[t]);

    }
        for (int t=0; t<NB;t++)
    {
        printf("==%d==>   %d, %d\n", t,return_value[t]->max, return_value[t]->min);
        free(return_value[t]);
    }
}
    
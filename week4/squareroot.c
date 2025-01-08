#include<stdio.h>
#include<pthread.h>
#include<math.h>
#define no_threads 4
int count = 0;

struct thread_data{
    int start;
    int end;
};

void* squareRT(void *p) {
    struct thread_data *d = (struct thread_data *)p;
    for (int i = d->start; i <= d->end; i++) {
        printf("Square root of %d is %f\n", i, sqrt(i));
		count++;
    }
    return NULL;
}


int main(){
    struct thread_data d[no_threads];
    int total = 1000;
    int slice = total / no_threads;
    d[0].start = 1;
    d[0].end = slice;
    for(int i = 1;i<no_threads;i++){
        d[i].start = d[i-1].end + 1;
        d[i].end = slice + slice * i;
    }
    pthread_t id[no_threads];
    for(int i = 0; i<no_threads;i++){
        pthread_create(&id[i],NULL,squareRT,&d[i]);
    }
    for(int i=0; i<no_threads;i++){
        pthread_join(id[i],NULL);
    }
	printf("Count = %d\n", count);
    return 0;
}
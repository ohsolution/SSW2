
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define R 1.000000

struct thread_data {
	int tid;
	int result;	
};

pthread_mutex_t mutex;

unsigned int seed;
int thread_num, point,total_point_in_circle;
double pi;

void *thread_mvm(void *arg)
{	
	int TP = *((int *)arg);
	double x,y;
	int point_in_circle = 0;

	for(int i=0;i<TP;++i)
	{
		pthread_mutex_lock(&mutex);
		x = ((double)rand()/(RAND_MAX));
		y = ((double)rand()/(RAND_MAX));
		pthread_mutex_unlock(&mutex);

		if(x*x+y*y < R) ++point_in_circle;
	}	

	pthread_mutex_lock(&mutex);
	total_point_in_circle += point_in_circle;
	pthread_mutex_unlock(&mutex);

	return NULL;
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Usage: %s <num_of_thread> <num_of_point>\n", argv[0]);
		exit(1);
	}

	thread_num = atoi(argv[1]);
	point = atoi(argv[2]);
	
	pthread_t tid[thread_num];

	//srand(&NULL);

	pthread_mutex_init(&mutex,NULL);

	for(int i=0;i<thread_num;++i) pthread_create(&tid[i], NULL, thread_mvm, (void*)&point);

	for(int i=0;i<thread_num;++i) pthread_join(tid[i],NULL);

	pi = (4*(double)total_point_in_circle)/(double)(thread_num*point);

	printf("%.6lf",pi);

	pthread_mutex_destroy(&mutex);

	return 0;
}

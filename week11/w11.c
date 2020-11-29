
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct thread_data {
	int tid;
	int result;	
};

int ** ma;
int * mb;
int row;
int col;

void *thread_mvm(void *arg)
{	
	struct thread_data * data = arg;
	int idx = data->tid;

	for(int i=0;i<col;++i) data->result += ma[idx][i] * mb[i];
	
	return NULL;
}

void print_two(int ** arr,char * str,int r,int c)
{
	printf("*** %s ***\n",str);
	for(int i=0;i<r;++i) 
	{
		for(int j=0;j<c;++j) printf("[ %d ] ",arr[i][j]);
		printf("\n");
	}
	printf("\n");
	return;
}

void print_one(int * arr,char * str, int r)
{
	printf("*** %s ***\n",str);
	for(int i=0;i<r;++i) printf("[ %d ]\n",arr[i]);
	printf("\n");
	return;
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Usage: %s <row> <column>\n", argv[0]);
		exit(1);
	}

	row = atoi(argv[1]);
	col = atoi(argv[2]);

	pthread_t tid[row];
	struct thread_data t_data[row];

	srand(time(NULL));

	ma = malloc(sizeof(int *) * row);
	for(int i=0;i<row;++i) ma[i] = malloc(sizeof(int) * col);
	for(int i=0;i<row;++i) for(int j=0;j<col;++j) ma[i][j] = (rand()%10);

	mb = malloc(sizeof(int) * col);
	for(int i=0;i<col;++i) mb[i] = (rand()%10);

	int *mret = malloc(sizeof(int) * row);

	for(int i=0;i<row;++i)
	{
		tid[i] = i;
		t_data[i].tid = i;
		t_data[i].result = 0;
	}

	print_two(ma,"Matrix",row,col);
	print_one(mb,"Vector",col);

	for(int i=0;i<row;++i) pthread_create(&tid[i], NULL, thread_mvm, (void*)&t_data[i]);

	for(int i=0;i<row;++i) pthread_join(tid[i],NULL);
	
	for(int i=0;i<row;++i) mret[i] = t_data[i].result;

	print_one(mret,"Result",row);

	/*
		//check ans right 
		printf("\n\n");
		int * ret = malloc(sizeof(int) * row);
		for(int i=0 ;i<row;++i) ret[i]=0;
		for(int i=0 ;i<row;++i) for(int j=0;j<col;++j) ret[i] += mb[j]*ma[i][j];

		for(int i=0;i<row;++i) if(ret[i] != mret[i]) printf("wrong %d\n",i);
	*/	

	for(int i=0; i<row;++i) free(ma[i]);
	free(ma);
	free(mb);
	free(mret); 

	return 0;
}

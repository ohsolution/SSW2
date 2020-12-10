#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

pthread_mutex_t file_mutex[1000]={
	PTHREAD_MUTEX_INITIALIZER,
};

int get_data(char *path){
	int fd = open(path, O_RDONLY);
	char buf[2];

	read(fd, buf, 1);

	close(fd);
	return atoi(buf);
}

void set_data(char *path, int data){
	int fd = open(path, O_WRONLY|O_CREAT, 0777);
	char buf[2];

	sprintf(buf, "%d", data);
	write(fd, buf, 1);

	close(fd);
	return;
}

void *setter(void *tid){
	int count = 0;
	while(1){
		//lock all file for consistency
		for(int i=999; i>=0; i--){
			pthread_mutex_lock(&(file_mutex[i]));
		}
		
		set_data("temp.txt", count);

		for(int i=999; i>=0; i--){
			pthread_mutex_unlock(&(file_mutex[i]));
		}
		count = (count + 1)%10;
		sleep(0.1);
	}
}

void *getter(void *tid){
	while(1){
		int count = 0;
		/* lock all file for consistency */
		for(int i=0; i<1000; i++){
			pthread_mutex_lock(&(file_mutex[i]));
		}

		count += get_data("temp.txt");
		
		for(int i=0; i<1000; i++){
			pthread_mutex_unlock(&(file_mutex[i]));
		}
		printf("get_data: %d\n", count);
		sleep(0.05);
	}
}

int main(){	
	/* make thread and start thread */
	pthread_t thread_get1, thread_get2, thread_set;
	pthread_create(&thread_set, NULL, &setter, NULL);
	sleep(0.5);
	pthread_create(&thread_get1, NULL, &getter, NULL);

	pthread_join(thread_set, NULL);
	pthread_join(thread_get1, NULL);
	return 0;
}


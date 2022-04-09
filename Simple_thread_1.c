#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_t p1, p2;

void* routine() 
{
    printf("Hello from threads %ld\n",pthread_self());
    sleep(3);
    printf("Ending thread %ld\n",pthread_self());
}

int main(int argc, char* argv[])
{
   
    // creating the thread p1
    if (pthread_create(&p1, NULL, &routine, NULL) != 0) {
        return 1;
    }
 	// creating the thread p2
    if (pthread_create(&p2, NULL, &routine, NULL) != 0) {
        return 2;
    }
	// Wait till the thread p1 finish execution 
    if (pthread_join(p1, NULL) != 0) {
        return 3;
    }
	// Wait till the thread p2 finish execution 
    if (pthread_join(p2, NULL) != 0) {
        return 4;
    }
    return 0;
}

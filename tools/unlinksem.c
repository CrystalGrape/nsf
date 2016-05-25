#include <signal.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/prctl.h>

void nsf_unlink_sem()
{
	sem_unlink("listen");
	sem_unlink("sem_write");
}

int main()
{
	nsf_unlink_sem();
	return 1;
}

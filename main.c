#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
	pid_t childID;
	int exitStatus;

	int segmentID;
	char * sharedMemory;
	struct shmid_ds sharedMemoryBuffer;
	int segementSize;
	const int sharedMemorySize = 0x6400;

	//allocate shared memory
	segmentID = shmget( IPC_PRIVATE, sharedMemorySize, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH );

	//attach to the shared  memory
	sharedMemory = (char *)shmat( segmentID, 0, 0);

	childID = fork();

	if( childID == 0 ){
		// child inherits the shared memory hence sharedMemory can be used directly
		// write something into the memory and append it to the current contents
		sprintf( sharedMemory , strcat(sharedMemory, "\nMessage from child") );
		//detach from sharedMemory
		shmdt( sharedMemory);
	}
	else{
		// write something into the memory
		sprintf( sharedMemory, "Message from parent");

		// at this point only the message from parent will get printed
		// as it is the only thing present in memory at this point in time
		puts( sharedMemory );

		waitpid(-1, &exitStatus, 0);

		// now the memory contains message from both parent and child
		puts( sharedMemory );

		shmdt( sharedMemory);
		shmctl( segmentID, IPC_RMID, 0);
	}

	

}

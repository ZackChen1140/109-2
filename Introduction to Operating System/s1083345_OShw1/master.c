#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid = fork();
	
	if(pid > 0)
	{
		printf("Successful (#%d", pid);
		printf(")!\n");
	}
	else if(pid == 0)
	{
		int e;
		e = (argc == 1 ? execlp("./mmv.out", "blake.txt", "happy.tmp", NULL) : execlp("./mmv.out", argv[1], argv[2], NULL));
		if(e == -1) printf("Fail\n");
	}
	else
	{
		printf("Fail\n");
		exit(-1);
	}
	
	return 0;
}

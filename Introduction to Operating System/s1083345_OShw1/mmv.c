#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main(int argc, char *argv[])
{
	int file_i, file_o;
	
	file_i = open(argv[0], O_RDONLY);
	file_o = creat(argv[1], S_IRWXU);
	
	char addHead[]="\\----Say Hello to s1083345!----\\\n";
	char readBlake[40];
	
	write(file_o, addHead, sizeof(addHead));
	while(read(file_i, readBlake, sizeof(readBlake)))
	{
		write(file_o, readBlake, sizeof(readBlake));
		memset(readBlake, 0, 40);
	}
	
	close(file_i);
	close(file_o);
	
	return 0;
}

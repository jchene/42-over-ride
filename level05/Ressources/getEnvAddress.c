#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv){
	if (argc != 2)
		return 1;
	printf("%p\n", getenv(argv[1]));
	return 0;
}
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	fprintf(stdout, "Hello!!\n");
	fprintf(stderr,  "program: %s, pid = %d exiting.\n", argv[0], getppid());

   return 0;
}

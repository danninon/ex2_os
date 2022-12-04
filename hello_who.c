#include <unistd.h>
// argc = 3 argv[0] = "hello_who.c" argv[1] = john argv[2] = dana
#include <stdio.h>
int main(int argc, char* argv[])
{
	for (int i = 1 ; i < argc ; i++ )
	{
		fprintf(stdout, "Hello %s!!\n" , argv[i]);
	}
	fprintf(stderr, "program: %s, pid = %d greeted %d people.\n", argv[0], getpid(), argc - 1);
   return 0;
}

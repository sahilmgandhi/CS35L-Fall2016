#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
	int i = 0;
	for (i; i < 256; i++)
	{
		int j = i ^ 42;
		fprintf(stdout, "\\%03o", j);
	}

	fprintf(stdout, "\n\n\n\n\n\n");

	for (i=0; i < 256; i++)
	{
		fprintf(stdout, "\\%03o", i);
	}
}
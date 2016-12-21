#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** args)
{
	if (argc != 3)
	{
		fprintf(stderr, "The number of arguments is not enough. \n");
		exit(1);
	}
	
	if (strlen(args[1]) != strlen(args[2]))
	{
		fprintf(stderr, "The length of the two strings does NOT match. \n");
		exit(1);
	}

	int charCount[256] = {0};
	int i = 0;
	for (i = 0; i < strlen(args[1]); i++)
	{
		charCount[args[1][i]]++;
	}
	i = 0;
	for (i = 0; i < 256; i++)
	{
		if (charCount[i] > 1)
		{
			fprintf(stderr, "From, the first argument after the program, has duplicate bytes. Please remove! \n");
			exit(1);
		}
	}

	char charMap[256] = "";
	i = 0;
	for (i = 0; i < strlen(args[1]); i++)
	{
		charMap[args[1][i]] = args[2][i];
	}

	int byte = getchar();
	while (byte != EOF)
	{
		if (charMap[byte] == 0)
			putchar(byte);
		else
			putchar(charMap[byte]);
		byte = getchar();
	}
}
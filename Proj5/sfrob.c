// Sahil Gandhi
// 10/26/2016
// sfrob.c
// This program will take a bunch of encoded strings and output the encoded strings in sorted order!
// TA: Thuy Vu -> Monday/Wednesady 4-6 PM

// TODO: If standard input ends in a partial record that does not have a trailing space, your program should behave as 
//if a space were appended to the input.

#include <stdio.h>
#include <stdlib.h>

// XOR 42, aka '*', to compare the decoded with the encoded

int frobcmp(const void* chara, const void* charb)		// each * points to an array of non-space bytes + space
{
	char const * wordOne = *(char const **) chara;		// getting a single character pointer from the array of words
	char const * wordTwo = *(char const **) charb;
	// we now need to compare the two words (byte by byte) to see which is larger than the other!

	// as noted by the TA and individuals from Piazza, since we used getChar to get the character, perhaps casting 
	// into char(of *char*) will do the trick!
	int currPos = 0;
	
	// case 1: where neither is empty! (but we need to make sure that if one array is shorter than the other, it 
	// breaks appropiately)
	while (wordOne[currPos] != ' ' && wordTwo[currPos] != ' ')
	{
		int oneChar = (wordOne[currPos])^'*';
		int twoChar = (wordTwo[currPos])^'*';

		if (oneChar - twoChar == 0)
		{
			++currPos;			// the two chars are identical right now, so move on to the next one
			continue;		
		}
		if (oneChar > twoChar)
			return 1;
		if (oneChar < twoChar)
			return -1;
	}

	// Case 2: lets say that they are identical all the way ... but one of them ends earlier than the other -> we have 
	//to loop again to see which one ended first

	if (wordOne[currPos] == ' ' && wordTwo[currPos] != ' ')		// this means that wordOne ran out first, so -1
		return -1;
	if (wordOne[currPos] != ' ' && wordTwo[currPos] == ' ' )	// this means that wordTwo ran out first so 1
		return 1;

	// Case 3: Both are empty or both are the same the full way through!... so they MUST be equal :)
	return 0;
}

int main()
{
	int allocSize = 8;
	int arraySize = 0;
	int	currLength = 0;
	char previousChar = ' '; // keep track if there is no space at the end of the file or multiple spaces 
	char currentChar = ' ';
	//char ** strarr = (char **) malloc(0);			// array of all the words
	char * currWord = (char *) malloc(allocSize);			// pointer to the current word

	if (currWord == NULL)
	{
		fprintf(stderr, "Array of current words failed to be allocated");
		exit(1);
	}
	int x = getchar(); // getChar will allow us to be able to get the right weird characters
	currentChar = (char)x;
	while (x != EOF)	//  keep going till the end of the file (hint from TA in piazza) to deal with mutating file
	{
		// if there are multiple spaces in a row ... we need to just ignore them!
		if (currentChar == ' ' && previousChar == ' ')
		{
			x = getchar();	 
			currentChar = (char)x;
		 	continue;
		}

		++currLength;
		if (currLength == allocSize)	// dynamically reallocaating more memory to currWord
		{
			currWord = realloc(currWord, (currLength*2)*sizeof(char));
			if (currWord == NULL)
			{
				fprintf(stderr, "Memory reallocation failed for currWord");
				exit(1);
			}
			allocSize*=2;
		}
		if (currentChar == ' ')	// it is a space ... meaning at the end of a word
		{
			currWord[currLength-1] = currentChar;	// add the space to the current word
			arraySize++;	// new "word" added so size of the word array is increased
		}
		else			// if any other character just add to currWord
		{		
			currWord[currLength - 1] = currentChar;
		}

		previousChar = currentChar;
		x = getchar();	
		currentChar = (char)x;
	}

	if (currLength == allocSize)	// final cheeck for realloc in case we reach end!
	{
		currWord = realloc(currWord, (currLength*2)*sizeof(char));
		if (currWord == NULL)
		{
			fprintf(stderr, "Memory reallocation failed for currWord");
			exit(1);
		}
		allocSize*=2;
	}

	// if no space at the very end of the file ... we must put the space there and treat it as so!
	if (previousChar != ' ')		// previous char = currChar ended prematurely and it is NOT a space
	{
		currWord[currLength] = ' ';
		arraySize++;
	}
	
	//printf("previous char was a space");
	char** strarr = (char**) malloc(arraySize*sizeof(char*));

	if (strarr == NULL)
	{
		fprintf(stderr, "Memory allocation for strarr failed");
		exit(1);
	}

	//fprintf(stdout, "Done looking at all characters");
	
	int currWordPos = 0;
	char* traverseWords = currWord;

	while (currWordPos < arraySize)
	{
		strarr[currWordPos] = traverseWords;		// because it is a double pointer, essentially like a 2D array ...

		if (currWordPos == (arraySize -1))
			break;
		while (*traverseWords != ' ')
		{	
			++traverseWords;
		}
		++traverseWords;
		++currWordPos;
	}

	qsort(strarr, arraySize, sizeof(char*), frobcmp);		// now that strarr is sorted up, we can "desort" it

	char* getWord;
	int i =0;
	for (i = 0; i < arraySize; ++i)
	{
		getWord = strarr[i];
		putchar(*getWord); 		// outputs the char to stdout

		while (*getWord != ' ')		// this way, it will also print out the space to signify the end of the line!
		{
			getWord++;
			putchar(*getWord);
		}
	}

	if (ferror(stdout))			// if the error flag is set during outputting the file
	{
		fprintf(stderr, "putchar() failed to output the char!\n");
		exit(1);
	}

	free(strarr);			// need to free up all of these memory bits!
	free(currWord);
}
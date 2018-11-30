#include <stdio.h>
int main()
{
	long int retTime = time(0) + 10;   // Get finishing time.
    while (time(0) < retTime);
	
	printf("andy is an idiot\n");
	return 1;
}
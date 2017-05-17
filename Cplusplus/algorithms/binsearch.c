#include <stdio.h>
int bin(int (*)[], int, int);
int main()
{
	int array[10] = {0,1,2,3,4,5,6,7,8,9};
	int point = bin(&array, 10, 0);
	printf("6 at %d\n", point);
	return 0;


}


int bin(int (*arg)[], int size, int elem)
{
	int high = size - 1, low = 0, mid;
	while(high > low)
	{
		mid = (high - low)/2 + low;
		if(elem < (*arg)[mid])
				high = mid;
		else if((*arg)[mid] < elem)
				low = mid;
		else
				return mid + 1;
	}
	return -1;
}

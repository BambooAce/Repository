#include <stdio.h>
int main()
{
	int arr[] = {100,22,12,1,23,4,7,8,3,5};
	int size = sizeof(arr)/sizeof(int);
	for (int i = 1; i <= size-1; i++)
	{
		for(int m = 0; m < size-i;++m)
		{
			int temp = 0;
			if (arr[m] > arr[m+1])
			{
				temp = arr[m];
				arr[m] = arr[m+1];
				arr[m+1] = temp;
			}
		}
	}
	for (int i = 0;i < size; ++i)
	{
	
		printf(" %d\n", arr[i]);
	}
}

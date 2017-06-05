#include <stdio.h>
int getNrecursive(int n);
int getN(int n);
int main()
{

}

int getNrecursive(int n)
{
	return n > 2 ? getNrecursive(n-1) + getNrecursive(n - 2) : 1;
}

int getN(int n)
{

}


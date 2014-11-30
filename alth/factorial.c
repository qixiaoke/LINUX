#include<stdio.h>
#include<stdlib.h>

int factorial(int n)
{
        int sum=0;

        if(n != 1 )
        {
        /*      return*/int  sum = n * factorial(n-1);
        }
        else
        {
                sum = 1;
        }

        return sum;
}

int endFactorial(int n, int sum)
{
        if(n != 1)
        {
             return endFactorial(n - 1, n * sum);
        }

        return sum;
}

void main()
{
       // printf("%d\n",  factorial(3));
        printf("%d\n",  endFactorial(3,1));

}


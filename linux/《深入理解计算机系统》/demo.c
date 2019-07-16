#include<stdio.h>

int main(int argc, char const *argv[])
{
    int arr[10][10];
    int sum = 0;
    
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            sum += arr[j][i];
        }
    }
    
    return 0;
}


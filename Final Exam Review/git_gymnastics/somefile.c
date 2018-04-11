#include <stdio.h>
#include <string.h>

void hellontimes(int n)
{
    int i;
    for(int i = 0; i < n; i++)
    {
        printf("Hello ");
        if(i == 0) printf("World!\n");
        else printf("Again!\n");
    }
}


int main()
{
    printf("Hello!\n");
    hellontimes(2);


    return 0;
}

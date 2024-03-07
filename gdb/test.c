#include <stdio.h>
#include <time.h>

void Print(int sum)
{
    long long timestamp = time(NULL);
    printf("result = %d, timestamp: %lld\n",sum, timestamp);
}

int AddToVal(int from, int to)
{
    int sum = 0;
    for (int i = from; i< to; i++) {
        sum += i;
    }
    return sum;
}



int main()
{
    int sum = AddToVal(0, 100);
    printf("hello a\n");
    printf("hello b\n");
    printf("hello c\n");
    printf("hello d\n");
    printf("hello e\n");
    printf("hello f\n");
    Print(sum);
    return 0;
}

#include <stdio.h>
union word{
    int a;
    char b;
};
int main()
{
    union word c;
    c.a=1;
    if(c.b==1)
        printf("LITTLE ENDIAN\n");
    else if(c.b==0)
        printf("BIG ENDIAN\n");
    return 0;
}

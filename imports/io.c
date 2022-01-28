#include <stdio.h>

extern int writeln(int a)
{
    return printf("%d\n", a);
}

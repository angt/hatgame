#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifdef __linux__
#include <sys/random.h>
#endif

void
rnd(void *buf, size_t size)
{
#ifdef __linux__
    if (getrandom(buf, size, 0) != (ssize_t)size)
        exit(1);
#else
    arc4random_buf(buf, size);
#endif
}

static int
isset(unsigned long long x, int n)
{
    return 1 & (x >> (sizeof(x) * CHAR_BIT - (n + 1)));
}

int
main(int argc, char **argv)
{
    size_t ok = 0, count = 0;
    while (1) {
        unsigned long long x[2];
        rnd(x, sizeof(x));
        ok += isset(x[0], __builtin_clzll(x[1])) && isset(x[1], __builtin_clzll(x[0]));
        count++;
        printf("  %6zu %6zu  => %3.3f\r", ok, count, (double)ok/(double)count);
    }
    return 0;
}

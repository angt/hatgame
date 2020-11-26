#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ULL_BIT (sizeof(unsigned long long) * CHAR_BIT)

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
    return 1 & (x >> (ULL_BIT - (n + 1)));
}

static int
strat(unsigned long long x)
{
#if STRAT == 1
    return 0;
#elif STRAT == 2
    return __builtin_clzll(x);
#else
    for (int i = 0; i < ULL_BIT; i += 3) {
        switch (7 & (x >> i)) {
            case 0: case 7: continue;
            case 1: case 3: return ULL_BIT - (3 + i);
            case 2: case 6: return ULL_BIT - (2 + i);
            case 4: case 5: return ULL_BIT - (1 + i);
        }
    }
    return 0;
#endif
}

int
main(int argc, char **argv)
{
    size_t win = 0, tot = 0;

    while (1) {
        unsigned long long x[2];
        rnd(x, sizeof(x));
        win += isset(x[0], strat(x[1])) && isset(x[1], strat(x[0]));
        tot++;
        printf("  win %-10zu  total %-10zu  probability %.2f\r", win, tot, (double)win/(double)tot);
    }
    return 0;
}

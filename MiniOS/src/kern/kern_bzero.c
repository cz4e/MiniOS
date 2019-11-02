#include <sys/types.h>

void
bzero(void *dest,size_t size)
{
    for(int offset = 0;offset < size;offset++) {
        *((char *)dest + offset ) = 0;
    }
}
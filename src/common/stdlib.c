#include <common/stdlib.h>
void memcpy(void * dest, void * src, int bytes) {
    char * d = dest, * s = src;
    while (bytes--) {
        *d++ = *s++;
    }
}

void bzero(void * dest, int bytes) {
    char * d = dest;
    while (bytes--) {
        *d++ = 0;
    }
}

char * itoa(int i) {
    static char intbuf[12];
    int j = 0, isneg = 0;

    if (i == 0) {
        intbuf[0] = '0';
        intbuf[1] = '\0';
        return intbuf;
    }

    if (i < 0) {
        isneg = 1;
        i = -i;
    }

    while (i != 0) {
       intbuf[j++] = '0' + (i % 10); 
       i /= 10;
    }

    if (isneg)
        intbuf[j++] = '-';

    intbuf[j] = '\0';
    j--;
    i = 0;
    while (i < j) {
        isneg = intbuf[i];
        intbuf[i] = intbuf[j];
        intbuf[j] = isneg;
        i++;
        j--;
    }

    return intbuf;
}


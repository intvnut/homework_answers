/* Sum up the values in an array of unsigned ints. */
#include <limits.h>

unsigned sum_unsigned_array(const unsigned *const array,
                            const int size) {
    int shift = 0;
    unsigned carries = 0;
    unsigned nextbit = 0;
    unsigned sum = 0;

    do {
        for (int i = 0; i < size; i++) {
            const unsigned inputbit = (array[i] >> shift) & 1;
            carries += inputbit & nextbit;
            nextbit ^= inputbit;
        }
        sum |= nextbit << shift;
        nextbit = carries & 1;
        carries >>= 1;
        shift++;
    } while (shift < CHAR_BIT * sizeof(unsigned));

    return sum;
}

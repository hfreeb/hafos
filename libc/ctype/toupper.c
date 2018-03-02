#include <ctype.h>

int toupper(int c) {
    if (islower(c)) {
        return c + ('A' - 'a');
    }
    
    return c;
}

#include <ctype.h>

int tolower(int c) {
    if (isupper(c)) {
        return c - ('A' - 'a');
    }
    
    return c;
}

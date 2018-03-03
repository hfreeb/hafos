#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ONE_OF(val, ...) \

//Whether the number is of a signed type
#define TOKEN_FLAG_SIGNED      1 << 0

//Left-justifies the conversion within the field
#define TOKEN_FLAG_LEFT        1 << 1

//Prefixes positive numbers with a '+'
#define TOKEN_FLAG_PLUS        1 << 2 

#define TOKEN_FLAG_SPACE       1 << 3 
#define TOKEN_FLAG_ALTERNATIVE 1 << 4 

//Number is padded to field width with 0s
#define TOKEN_FLAG_ZERO        1 << 5 

//Formats with lowercase chars, as opposed to uppercase (for hex, INFINITY, NAN)
#define TOKEN_FLAG_LOWER       1 << 6

enum token_type {
    TOKEN_TYPE_INVALID,

    //TODO: remove
    //Plain string, no vararg
    TOKEN_TYPE_TEXT,

    TOKEN_TYPE_STR,
    TOKEN_TYPE_CHAR,

    TOKEN_TYPE_INT,
    TOKEN_TYPE_UINT,
};

struct format_token {
    uint8_t type;
    uint8_t flags;  
    uint8_t base;
    unsigned int width;
};

static bool print_len(const char *data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        if (putchar(data[i]) == EOF) {
            return false;
        }
    }

    return true;
}

static bool print(const char *data) {
    return print_len(data, strlen(data));
}

static size_t itoa(char *out_num, unsigned long long num, struct format_token *token) {
    size_t len = 0;

    char *p = out_num;
    if (token->flags & TOKEN_FLAG_SIGNED && ((signed) len) < 0) {
        *p++ = '-';
        num = ((signed) num) * -1;
    }

    //Get to where the number ends;
    unsigned long long shifter = num;
    unsigned int digits = 0;
    unsigned int filler_chars = 0;
    do {
        ++p;
        ++digits;
        if (shifter) {
            shifter /= token->base;
        } else {
            ++filler_chars;
        }
    } while (shifter || digits < token->width);

    *p = '\0';

    //TODO: Right justifcation

    do {
        //gets stuck
        int val = num % token->base;
        if (val < 10) {
            *(--p) = '0' + val;
        } else {
            *(--p) = ((token->flags & TOKEN_FLAG_LOWER) ? 'a' : 'A') + val - 10;
        }

        
        num /= token->base;
    } while (num);

    //Add preceeding 0s or spaces
    for (int i = 0; i < filler_chars; ++i) {
        *(--p) = (token->flags & TOKEN_FLAG_ZERO) ? '0' : ' ';
    }

    return len;
}

static size_t atoi(const char *fmt, unsigned int *num) {
    size_t len = 0;

    while (isdigit(*fmt)) {
        *num = *num * 10 + (*fmt - '0');        
        ++len;
        ++fmt;
    }

    return len;
}

static int process_token(const char *fmt, va_list vlist, struct format_token *token) {
    const char *start = fmt;

    for (; *fmt && *fmt != '%'; fmt++);

    if (fmt != start) {
        token->type = TOKEN_TYPE_TEXT;
        return fmt - start;
    }

    ++fmt;

    bool at_flags = true;
    while (at_flags) {
        switch (*fmt) {
            case '-': token->flags |= TOKEN_FLAG_LEFT; break;
            case '+': token->flags |= TOKEN_FLAG_PLUS; break;
            case ' ': token->flags |= TOKEN_FLAG_SPACE; break;
            case '#': token->flags |= TOKEN_FLAG_ALTERNATIVE; break;
            case '0': token->flags |= TOKEN_FLAG_ZERO; break;
            default:
                      at_flags = false;
                      break;
        }

        if (at_flags) {
            ++fmt;
        }
    }

    if (isdigit(*fmt)) {
        unsigned int width = 0;
        fmt += atoi(fmt, &width);
        token->width = width;
    } else if (*fmt == '*') {
        token->width = va_arg(vlist, unsigned int);
        ++fmt; 
    }

    //TODO: Precision

    //TODO: Length modifiers

    switch (*fmt) {
        case 'd':
        case 'i': 
            token->base = 10;
            token->type = TOKEN_TYPE_INT;            
            break;
        case 'o':
            token->base = 8;
        case 'u':
            token->type = TOKEN_TYPE_UINT;
            break;
        case 'x':
            token->flags |= TOKEN_FLAG_LOWER;
        case 'X':
            token->base = 16;
            token->type = TOKEN_TYPE_UINT;
            break;
        case 'c':
            token->type = TOKEN_TYPE_CHAR;
            break;
        case 's':
            token->type = TOKEN_TYPE_STR;
            break;
        case '%':
            token->type = TOKEN_TYPE_TEXT;
            break;
        default:
            token->type = TOKEN_TYPE_INVALID;
            break;
    }

    return ++fmt - start;
}


int vprintf(const char *fmt, va_list vlist) {
    size_t written = 0;

    while (*fmt != '\0') {
        struct format_token token = {0}; 

        size_t length = process_token(fmt, vlist, &token);

        switch (token.type) {
            case TOKEN_TYPE_INVALID:
            case TOKEN_TYPE_TEXT:
                if (!print_len(fmt, length)) {
                    return -1;
                }
                break;

            case TOKEN_TYPE_CHAR: {
                char c;
                c = (char) va_arg(vlist, int);
                if (!putchar(c)) {
                    return -1;
                }

                break;
            }

            case TOKEN_TYPE_STR: {
                const char *str;
                str = va_arg(vlist, const char *);
                print(str);
                break;
            }   

                //TODO: Arbitrary 32 length, maybe calculate maximum value and change?
            case TOKEN_TYPE_INT: {
                char str[32];
                int num;
                num = va_arg(vlist, int);
                itoa(str, (unsigned long long) num, &token);
                print(str);
                break;
            }

            case TOKEN_TYPE_UINT: {
                char str[32];
                unsigned int num;
                num = va_arg(vlist, unsigned int);
                itoa(str, (unsigned long long)  num, &token);
                print(str);
                break;
            }
        }   

        fmt += length;
    }

    return written;
}

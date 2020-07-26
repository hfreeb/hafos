#include "kprint.h"

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <hafos/io/textmode.h>

enum token_type {
    TOKEN_TYPE_CHAR,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_INT,
    TOKEN_TYPE_UINT
};

struct format_token {
    enum token_type type;
    unsigned int min_width;
    unsigned int base;
    bool explicit_sign;
    bool space;
    bool leading_zeros;
};

static void reverse(char *str) {
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char c = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = c;
    }
}

static void uitoa(uint64_t num, unsigned int base, char *str) {
    size_t i = 0;
    while (num) {
        unsigned int digit = num % base;
        if (digit < 10) {
            str[i++] = '0' + digit;
        } else {
            str[i++] = 'a' + digit - 10;
        }

        num /= base;
    }

    str[i] = '\0';
    reverse(str);
}

static size_t parse_token(const char *fmt, va_list args, struct format_token *token) {
    const char *start = fmt;

    bool parsing_flags = true;
    while (parsing_flags) {
        switch (*fmt) {
            case '+':
                token->explicit_sign = true;
                break;
            case ' ':
                token->space = true;
                break;
            case '0':
                token->leading_zeros = true;
                break;
            default:
                parsing_flags = false;
                break;
        }

        if (parsing_flags) {
            fmt++;
        }
    }

    if (isdigit(*fmt)) {
        token->min_width = *fmt - '0';
        fmt++;
    }

    switch (*fmt++) {
        case 'c':
            token->type = TOKEN_TYPE_CHAR;
            break;
        case 's':
            token->type = TOKEN_TYPE_STRING;
            break;
        case 'd':
            token->type = TOKEN_TYPE_INT;
            token->base = 10;
            break;
        case 'x':
            token->type = TOKEN_TYPE_UINT;
            token->base = 16;
            break;
        default:
            return 0;
    }

    return fmt - start;
}

static void print_token(struct format_token token, va_list args) {
    switch (token.type) {
        case TOKEN_TYPE_CHAR: {
            char arg = va_arg(args, int);
            textmode_putchar(arg);
            break;
        }
        case TOKEN_TYPE_STRING: {
            char *arg = va_arg(args, char *);
            textmode_write_string(arg);
            break;
        }
        case TOKEN_TYPE_UINT: {
            char str[64];
            unsigned int arg = va_arg(args, unsigned int);
            uitoa((uint64_t) arg, token.base, str);
            textmode_write_string(str);
            break;
        }
    }
}

int kvprintf(const char *fmt, va_list args) {
    const char *start = fmt;

    char c;
    while ((c = *fmt++)) {
        if (c == '%') {
            // Output a single '%' for two adjacent '%'s
            if (*fmt == '%') {
                textmode_putchar('%');
                ++fmt;
                continue;
            }

            struct format_token token = {0};
            size_t len = parse_token(fmt, args, &token);
            if (len == 0) {
                // Invalid token, just print out the %
                textmode_putchar('%');
                ++fmt;
                continue;
            }

            print_token(token, args);
            fmt += len;
        } else {
            textmode_putchar(c);
        }
    }

    return fmt - start;
}

int kprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = kvprintf(format, args);
    va_end(args);
    return ret;
}

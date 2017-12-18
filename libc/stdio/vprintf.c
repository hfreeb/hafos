#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

enum format_type {
	FORMAT_TYPE_NONE, // Just a string part
    FORMAT_TYPE_WIDTH,
	FORMAT_TYPE_CHAR,
	FORMAT_TYPE_STR,
	FORMAT_TYPE_PERCENT_CHAR,
	FORMAT_TYPE_INVALID,
	FORMAT_TYPE_INT
};

#define FIELD_WIDTH_MAX ((1 << 23) - 1)

#define NUMBER_FLAG_SIGN    1 << 0
#define NUMBER_FLAG_ZEROPAD 1 << 1

struct printf_spec {
    enum format_type type:8;
    uint8_t base:8;
    int32_t field_width:24;
    uint8_t number_flags:8;
} __packed;

static bool print(const char* data, size_t length) {
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; ++i) {
        if (putchar(bytes[i]) == EOF) {
            return false;
        }
    }

    return true;
}

static size_t itoa(char* out_num, int num, struct printf_spec spec) {
    size_t len = 0;

    bool add_sign = false;
    if (num < 0 && spec.number_flags & NUMBER_FLAG_SIGN) {
        num *= -1;
        add_sign = true;
    }

    //outputs edx hex in reverse order into tmp
    while (num != 0) {
        int remainder = num % spec.base;
        if (remainder < 10) {
            out_num[len] = remainder + '0';
        } else {
            out_num[len] = remainder + 'A' - 10;
        }

        num /= spec.base;
        ++len;
    } 

    if (add_sign) {
        out_num[len] = '-';
        ++len;
    }

    //Reverse out_num to get it into the correct order
    char *p1, *p2;
    
    p1 = out_num;
    p2 = out_num + (len - 1);

    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        ++p1;
        --p2;
    }

    return len;
}

int vprintf(const char* format, va_list vlist) {
    struct printf_spec spec = {0};

    int written = 0;

    while(*format != '\0') {
        size_t max_remain = INT_MAX - written;

        if (format[0] == '%' && format[1] == '%') {
            ++format;
        }

        //if the next character is not a format token, write until the next % or until the end
        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%') {
                ++format;
            }

            size_t amount = 1;
            while (format[amount] && format[amount] != '%') {
                ++amount;
            }

            if (amount > max_remain) {
                // TODO: Set errno to EOVERFLOW
                return -1;
            }

            if (!print(format, amount)) {
                return -1;
            }

            format += amount;
            written += amount;
            continue;
        }

        const char next = *(format + 1);
        //check the next character for the format specifier
        if (next == 'c') {
            format += 2;

            char c = (char) va_arg(vlist, int /* char promotes to int in parameters */);
            if (max_remain == 0) {
                // TODO: Set errno to EOVERFLOW
                return -1;
            }

            if (!print(&c, sizeof(c))) {
                return -1;
            }

            ++written;
        } else if (next == 's') {
            format += 2;
            const char* str = va_arg(vlist, const char*);
            size_t len = strlen(str);

            if (len > max_remain) {
                // TODO: Set errno to EOVERFLOW
                return -1;
            }

            if (!print(str, len)) {
                return -1;
            }

            written += len;
        } else if (next == 'd') {
            format += 2;
            int number = va_arg(vlist, int);

            spec.base = 10;
            spec.number_flags |= NUMBER_FLAG_SIGN;
            char str[12];
            size_t len = itoa(str, number, spec);

            if (len > max_remain) {
                //TODO: Set errno to EOVERFLOW
                return -1;
            }

            if (!print(str, len)) {
                return -1;
            }

            written += len;

        } else if (next == 'x') {
            format += 2;
            int number = va_arg(vlist, int);

            spec.base = 16;
            spec.number_flags &= ~(NUMBER_FLAG_SIGN);
            char str[9];
            size_t len = itoa(str, number, spec);

            if (len > max_remain) {
                //TODO: Set errno to EOVERFLOW
                return -1;
            }

            if (!print(str, len)) {
                return -1;
            }

            written += len;
        } else {
            if (max_remain < 2) {
                // TODO: Set errno to EOVERFLOW
                return -1;
            }

            //Print format specifier and carry on
            if (!print(format, 2)) {
                return -1;
            }

            written += 2;
            format += 2;
        }
    }

    return written;
}

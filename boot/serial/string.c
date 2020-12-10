#include "string.h"

int strlen(const char *str)
{
    int len = 0;
    while (*str++) {
        len++;
    }
    return len;
}

static void reverse(char *str, int length)
{
    char *end = str + length - 1;
    while (str < end) {
        char ch = *str;
        *str++ = *end;
        *end-- = ch;
    }
}

char *itoa(int num, int base, char *str)
{
    int i = 0;
    int isNegative = 0;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }
    while (num != 0) {
        int r = num % base;
        str[i++] = (r > 9) ? (r - 10) + 'a' : r + '0';
        num /= base;
    }
    if (isNegative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    reverse(str, i);
    return str;
}

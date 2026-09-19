#include "minib64/base64.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static const char base64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

char *
minib64_encode(const unsigned char* data, size_t len) {
    size_t out_len = ((len + 2) / 3) * 4 + 1;

    char *out_str = malloc(out_len);

    size_t i,j;
    uint32_t a,b,c;
    _Bool read_b, read_c;

    i = j = 0;
    read_b = read_c = false;

    while (i < len) {
        a = data[i++];

        if (i < len) {
            b = data[i++];
            read_b = true;
        }

        if (i < len) {
            c = data[i++];
            read_c = true;
        }

        uint32_t triple =
            (a << 16) |
            (b << 8) |
            c;

        out_str[j++] = *(base64_table + ((triple >> 18) & 0x3F));
        out_str[j++] = *(base64_table + ((triple >> 12) & 0x3F));

        if (read_b) {
            out_str[j++] = *(base64_table + ((triple >> 6) & 0x3F));
        } else {
            out_str[j++] = '=';
        }

        if (read_c) {
            out_str[j++] = *(base64_table + (triple & 0x3F));
        } else {
            out_str[j++] = '=';
        }

        read_b = read_c = false;
    }

    out_str[j] = '\0';

    return out_str;
}

unsigned char *
minib64_decode(const char* data, size_t* out_len) {
    return NULL;
}

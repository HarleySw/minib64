#include "minib64/base64.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

minib64_status_t
minib64_decode(const char* in, size_t in_len, unsigned char **out, size_t* out_len) {
    if (in_len == 0 || in_len % 4 != 0) {
        return MINIB64_INVALID_LENGTH;
    }

    for (size_t i = 0; i < in_len; i++) {
        _Bool found = false;
        for (size_t j = 0; j < sizeof(base64_table) - 1; j++) {
            if (base64_table[j] == in[i]) {
                found = true;
                break;
            }
        }

        if (in[i] != '=' && !found) {
            return MINIB64_INVALID_CHARACTER;
        }

        if (in[i] == '=' && i < in_len - 2) {
            return MINIB64_INVALID_PADDING;
        }
    }

    *out_len = (in_len / 4) * 3;
    if (in[in_len - 1] == '=') {
        (*out_len)--;
    }
    if (in[in_len - 2] == '=') {
        (*out_len)--;
    }

    *out = malloc(*out_len);

    size_t i, j;
    char a, b, c, d;
    uint32_t a_i, b_i, c_i, d_i;
    i = j = 0;

    while (i < in_len) {
        a = in[i++];
        b = in[i++];
        c = in[i++];
        d = in[i++];

        char *p = strchr((char *)base64_table, a);
        if (p != NULL) {
            printf("%c  ", *p);
            a_i = p - base64_table;
        }

        p = strchr((char *)base64_table, b);
        if (p != NULL) {
            b_i = p - base64_table;
        }

        p = strchr((char *)base64_table, c);
        if (p != NULL) {
            c_i = p - base64_table;
        } else {
            c_i = 0;
        }

        p = strchr((char *)base64_table, d);
        if (p != NULL) {
            d_i = p - base64_table;
        } else {
            d_i = 0;
        }
        
        uint32_t quadra = 
            (a_i << 18) |
            (b_i << 12) |
            (c_i << 6) |
            d_i;
        
        (*out)[j++] = (quadra >> 16) & 0xFF;
        (*out)[j++] = (quadra >> 8) & 0xFF;

        if (j < *out_len) {
            (*out)[j++] = quadra & 0xFF;
        }
    }

    return MINIB64_SUCCESS;
}

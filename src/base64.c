#include "minib64/base64.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

static const char base64_encode_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static const int8_t base64_decode_table[256] = {
    // 0x00 - 0x1F (0 - 31)
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,

    // 0x20 - 0x3F (32 - 63)
    -1, -1, -1, -1, -1, -1, -1, -1,  // 32-39: Space ! " # $ % & ' 
    -1, -1, -1, 62, -1, -1, -1, 63,  // 40-47: ( ) * + , - . /   ('+' = 62, '/' = 63)
    52, 53, 54, 55, 56, 57, 58, 59,  // 48-55: 0 1 2 3 4 5 6 7
    60, 61, -1, -1, -1, -2, -1, -1,  // 56-63: 8 9 : ; < = > ?   ('=' = -2)

    // 0x40 - 0x5F (64 - 95)
    -1,  0,  1,  2,  3,  4,  5,  6,  // 64-71: @ A B C D E F G
     7,  8,  9, 10, 11, 12, 13, 14,  // 72-79: H I J K L M N O
    15, 16, 17, 18, 19, 20, 21, 22,  // 80-87: P Q R S T U V W
    23, 24, 25, -1, -1, -1, -1, -1,  // 88-95: X Y Z [ \ ] ^ _

    // 0x60 - 0x7F (96 - 127)
    -1, 26, 27, 28, 29, 30, 31, 32,  // 96-103: ` a b c d e f g
    33, 34, 35, 36, 37, 38, 39, 40,  // 104-111: h i j k l m n o
    41, 42, 43, 44, 45, 46, 47, 48,  // 112-119: p q r s t u v w
    49, 50, 51, -1, -1, -1, -1, -1,  // 120-127: x y z { | } ~ DEL

    // 0x80 - 0xFF (128 - 255)
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
};

minib64_status_t
minib64_encode(const unsigned char *in, size_t in_len, char **out, size_t *out_len) {
    if (in == NULL || out == NULL || out_len == NULL) {
        return MINIB64_ERR_INVALID_PARAMS;
    }

    if (in_len > ((SIZE_MAX - 5) / 4) * 3) {
        return MINIB64_ERR_OVERFLOW;
    }

    *out_len = ((in_len + 2) / 3) * 4;

    *out = malloc(*out_len + 1);    
    if (*out == NULL) {
        return MINIB64_ERR_NO_MEMORY;
    }

    size_t i,j;
    uint32_t a,b,c;
    _Bool read_b, read_c;

    i = j = 0;
    read_b = read_c = false;
    a = b = c = 0;

    while (i < in_len) {
        a = in[i++];

        if (i < in_len) {
            b = in[i++];
            read_b = true;
        }

        if (i < in_len) {
            c = in[i++];
            read_c = true;
        }

        uint32_t triple =
            (a << 16) |
            (b << 8) |
            c;

        (*out)[j++] = *(base64_encode_table + ((triple >> 18) & 0x3F));
        (*out)[j++] = *(base64_encode_table + ((triple >> 12) & 0x3F));

        if (read_b) {
            (*out)[j++] = *(base64_encode_table + ((triple >> 6) & 0x3F));
        } else {
            (*out)[j++] = '=';
        }

        if (read_c) {
            (*out)[j++] = *(base64_encode_table + (triple & 0x3F));
        } else {
            (*out)[j++] = '=';
        }

        read_b = read_c = false;
        a = b = c = 0;
    }

    (*out)[j] = '\0';

    return MINIB64_SUCCESS;
}   

minib64_status_t
minib64_decode(const char* in, size_t in_len, unsigned char **out, size_t* out_len) {
    if (in == NULL || out == NULL || out_len == NULL) {
        return MINIB64_ERR_INVALID_PARAMS;
    }

    if (in_len % 4 != 0) {
        return MINIB64_ERR_INVALID_LENGTH;
    }

    // RFC 4648: an empty Base64 string represents empty data.
    // Handle it before accessing in[in_len - 1] below. Allocate one byte
    // so callers can always free(*out), consistent with non-empty input.
    if (in_len == 0) {
        *out_len = 0;
        *out = malloc(1);
        if (*out == NULL) {
            return MINIB64_ERR_NO_MEMORY;
        }

        return MINIB64_SUCCESS;
    }

    size_t padding = 0;
    if (in[in_len - 1] == '=') {
        padding++;
        if (in[in_len - 2] == '=') {
            padding++;
        }
    } else if (in[in_len - 2] == '=') {
        return MINIB64_ERR_INVALID_PADDING;
    }

    *out_len = (in_len / 4) * 3 - padding;

    *out = malloc(*out_len == 0 ? 1 : *out_len);
    if (*out == NULL) {
        return MINIB64_ERR_NO_MEMORY;
    }

    size_t out_idx = 0;
    for (size_t i = 0; i < in_len; i+=4) {
        _Bool is_last_block = i == in_len - 4;

        int8_t v0 = base64_decode_table[(unsigned char) in[i]];
        int8_t v1 = base64_decode_table[(unsigned char) in[i + 1]];
        int8_t v2 = base64_decode_table[(unsigned char) in[i + 2]];
        int8_t v3 = base64_decode_table[(unsigned char) in[i + 3]];

        if (v0 < 0 || v1 < 0) {
            free(*out);
            *out = NULL;
            return (v0 == -2 || v1 == -2) ? MINIB64_ERR_INVALID_PADDING : MINIB64_ERR_INVALID_CHARACTER; 
        }

        if (!is_last_block) {
            if (v2 < 0 || v3 < 0) {
                free(*out);
                *out = NULL;
                return (v2 == -2 || v3 == -2) ? MINIB64_ERR_INVALID_PADDING : MINIB64_ERR_INVALID_CHARACTER; 
            }

            uint32_t quad = 
                (uint32_t) v0 << 18
                | (uint32_t) v1 << 12
                | (uint32_t) v2 << 6
                | (uint32_t) v3;

            (*out)[out_idx++] = (quad >> 16) & 0xFF; 
            (*out)[out_idx++] = (quad >> 8) & 0xFF; 
            (*out)[out_idx++] = quad & 0xFF; 
        } else {
            if (padding == 2) {
                if (v2 != -2 || v3 != -2) {
                    free(*out);
                    *out = NULL;
                    return MINIB64_ERR_INVALID_PADDING;
                }

                uint32_t quad = 
                    (uint32_t) v0 << 18
                    | (uint32_t) v1 << 12;

                (*out)[out_idx++] = (quad >> 16) & 0xFF;
            } else if (padding == 1) {
                if (v3 != -2) {
                    free(*out);
                    *out = NULL;
                    return MINIB64_ERR_INVALID_PADDING;
                } 

                if (v2 < 0) {
                    free(*out);
                    *out = NULL;
                    return v2 == -2 ? MINIB64_ERR_INVALID_PADDING : MINIB64_ERR_INVALID_CHARACTER;
                }

                uint32_t quad = 
                    (uint32_t) v0 << 18
                    | (uint32_t) v1 << 12
                    | (uint32_t) v2 << 6;

                (*out)[out_idx++] = (quad >> 16) & 0xFF;
                (*out)[out_idx++] = (quad >> 8) & 0xFF;
            } else {
                if (v2 < 0 || v3 < 0) {
                    free(*out);
                    *out = NULL;
                    return (v2 == -2 || v3 == -2) ? MINIB64_ERR_INVALID_PADDING : MINIB64_ERR_INVALID_CHARACTER;
                }

                uint32_t quad = 
                    (uint32_t) v0 << 18
                    | (uint32_t) v1 << 12
                    | (uint32_t) v2 << 6
                    | (uint32_t) v3;
                
                (*out)[out_idx++] = (quad >> 16) & 0xFF;
                (*out)[out_idx++] = (quad >> 8) & 0xFF;
                (*out)[out_idx++] = (quad) & 0xFF;
            }
        }
        
    }

    return MINIB64_SUCCESS;
}

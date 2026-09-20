#pragma once

#include <stddef.h>

typedef enum {
    MINIB64_SUCCESS = 0,

    // General errors
    MINIB64_ERR_INVALID_PARAMS,
    MINIB64_ERR_NO_MEMORY,
    MINIB64_ERR_OVERFLOW,  

    // Decoding errors
    MINIB64_ERR_INVALID_LENGTH,
    MINIB64_ERR_INVALID_CHARACTER,
    MINIB64_ERR_INVALID_PADDING,
} minib64_status_t;

minib64_status_t
minib64_encode(const unsigned char *in, size_t in_len, char **out, size_t *out_len);

minib64_status_t
minib64_decode(const char *in, size_t in_len, unsigned char **out, size_t *out_len);

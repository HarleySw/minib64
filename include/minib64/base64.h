#pragma once

#include <stddef.h>

char *
minib64_encode(const unsigned char *data, size_t len);

unsigned char*
minib64_decode(const char* data, size_t *out_len);

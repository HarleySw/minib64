#include <minib64/base64.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    char *in = "72bca9b68fc16ac7beeb8f849dca1d8a783e8acf9679bf9269f7bf";
    char* out_str;
    size_t out_len;

    unsigned char bytes[strlen(in) / 2];

    const char hex_table[] = "0123456789abcdef";
    for (char *ch = in; ch < in + strlen(in); ch+=2) {
        char a = *ch;
        char b = *(ch + 1);
        char *p1 = strchr((char *)hex_table, a);
        char *p2 = strchr((char *)hex_table, b);

        bytes[(ch - in) / 2] = (p1 - hex_table) * 16 + (p2 - hex_table);
    }

    minib64_status_t result = minib64_encode(bytes, sizeof(bytes), &out_str, &out_len);

    if (result == MINIB64_SUCCESS) {
        printf("Output: %s\n", out_str);
    }

    free(out_str);

    return 0;
}

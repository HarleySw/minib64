#include <minib64/base64.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    char *data = "Zm9vYg==";
    unsigned char *out;
    size_t out_len;
    
    minib64_status_t result = minib64_decode((unsigned char *)data, strlen(data), &out, &out_len);

    char *out_str = malloc(out_len + 1);
    memcpy(out_str, out, out_len);
    out_str[out_len] = '\0';

    printf("Output: %s\n", out_str); //Output: foobar
    printf("%zu\n", out_len);
    printf("%d\n", result);

    free(out);
    free(out_str);

    return 0;
}

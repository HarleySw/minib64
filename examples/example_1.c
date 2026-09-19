#include <minib64/base64.h>

#include <stdio.h>
#include <string.h>

int main(void) {
    char *data = "foobar";
    char *encoded = minib64_encode((unsigned char *)data, strlen(data));

    printf("Output: %s\n", encoded); //Output: Zm9vYmFy
}

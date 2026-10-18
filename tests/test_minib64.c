#include <stdlib.h>
#include <string.h>

#include "minib64/base64.h"
#include "unity.h"

void setUp(void) {
    // run at the start
}

void tearDown(void) {
    // run at the end
}

void test_encode_basic(void) {
    const unsigned char *in = "foobar";
    char *out = NULL;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_NOT_NULL(out);
    TEST_ASSERT_EQUAL_STRING("Zm9vYmFy", out);

    free(out);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_encode_basic);

    return UNITY_END();
}
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

void test_encode_empty_vec_00(void) {
    const unsigned char *in = "";
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_STRING("", out);
    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);

    free(out);
}

void test_encode_one_byte_vec_01(void) {
    const unsigned char *in = "f";
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_STRING("Zg==", out);
    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);

    free(out);
}

void test_encode_two_byte_vec_02(void) {
    const unsigned char *in = "fo";
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_STRING("Zm8=", out);
    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);

    free(out);
}

void test_encode_three_byte_vec_03(void) {
    const unsigned char *in = "foo";
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_STRING("Zm9v", out);
    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);

    free(out);
}

void test_encode_four_byte_vec_04(void) {
    const unsigned char *in = "foob";
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_STRING("Zm9vYg==", out);
    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);

    free(out);
}

void test_encode_five_byte_vec_05(void) {
    const unsigned char *in = "fooba";
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_STRING("Zm9vYmE=", out);
    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);

    free(out);
}

void test_encode_six_byte_vec_06(void) {
    const unsigned char *in = "foobar";
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_STRING("Zm9vYmFy", out);
    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);

    free(out);
}

void test_encode_padding_enc_01(void) {
    const unsigned char *in = "f";
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_STRING("==", out + 2);

    free(out);
}

void test_encode_padding_enc_02(void) {
    const unsigned char *in = "fo";
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_STRING("=", out + 3);

    free(out);
}

void test_encode_padding_enc_03(void) {
    const unsigned char *in = "foo";
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_STRING("", out + 4);

    free(out);
}

void test_encode_padding_enc_04(void) {
    const unsigned char *in = "foob";
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_STRING("==", out + 6);

    free(out);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_encode_empty_vec_00);
    RUN_TEST(test_encode_one_byte_vec_01);
    RUN_TEST(test_encode_two_byte_vec_02);
    RUN_TEST(test_encode_three_byte_vec_03);
    RUN_TEST(test_encode_four_byte_vec_04);
    RUN_TEST(test_encode_five_byte_vec_05);
    RUN_TEST(test_encode_six_byte_vec_06);

    RUN_TEST(test_encode_padding_enc_01);
    RUN_TEST(test_encode_padding_enc_02);
    RUN_TEST(test_encode_padding_enc_03);
    RUN_TEST(test_encode_padding_enc_04);

    return UNITY_END();
}
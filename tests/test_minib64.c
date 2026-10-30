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

static void assert_decode_rejected(const char *in) {
    unsigned char *out = NULL;
    size_t out_len = 0;

    minib64_status_t status = minib64_decode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_NOT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_NULL(out);

    free(out);
}

static void assert_decode_accepted(const char *in, const unsigned char *expected, size_t expected_len) {
    unsigned char *out = NULL;
    size_t out_len = 0;

    minib64_status_t status = minib64_decode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_NOT_NULL(out);
    TEST_ASSERT_EQUAL_size_t(expected_len, out_len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, out, expected_len);

    free(out);
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

void test_encode_binary_data_bin_01(void) {
    const unsigned char in[] = {
        0x00
    };
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, sizeof(in) / sizeof(*in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_STRING("AA==", out);

    free(out);
}

void test_encode_binary_data_bin_02(void) {
    const unsigned char in[] = {
        0xFF
    };
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, sizeof(in) / sizeof(*in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_STRING("/w==", out);

    free(out);
}


void test_encode_binary_data_bin_03(void) {
    unsigned char in[256];

    for (size_t i = 0; i < sizeof(in); i++) {
        in[i] = (unsigned char) i;
    }

    char *out_encoded;
    size_t out_encoded_len;

    minib64_status_t status = minib64_encode(in, sizeof(in), &out_encoded, &out_encoded_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_NOT_NULL(out_encoded);
    TEST_ASSERT_EQUAL_size_t(344, out_encoded_len);

    unsigned char *out_decoded;
    size_t out_decoded_len;

    status = minib64_decode(out_encoded, out_encoded_len, &out_decoded, &out_decoded_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_NOT_NULL(out_decoded);
    TEST_ASSERT_EQUAL_size_t(sizeof(in), out_decoded_len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(in, out_decoded, sizeof(in));

    free(out_encoded);
    free(out_decoded);
}

void test_encode_binary_data_bin_04(void) {
    const unsigned char in[] = {
        0x00, 0x00, 0x00,
        0x00, 0x00, 0x00
    };
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, sizeof(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_STRING("AAAAAAAA", out);

    free(out);
}

void test_encode_binary_data_bin_05(void) {
    const unsigned char in[] = {
        0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF
    };
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, sizeof(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_STRING("////////", out);

    free(out);
}

void test_encode_binary_data_bin_06(void) {
    const unsigned char in[] = {
        0x00, 0xFF, 0x00,
        0xFF, 0x00, 0xFF
    };
    char *out;
    size_t out_len;

    minib64_status_t status = minib64_encode(in, sizeof(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_STRING("AP8A/wD/", out);

    free(out);
}

void test_binary_roundtrip_bin_07(void) {
    const unsigned char in[] = {
        0x00, 0xFF, 0x01, 0xFE, 0x02, 0xFD,
        0x7F, 0x80, 0xAA, 0x55, 0x00, 0xFF, 0x42
    };
    char *encoded;
    size_t encoded_len;
    unsigned char *decoded;
    size_t decoded_len;

    minib64_status_t status = minib64_encode(in, sizeof(in), &encoded, &encoded_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_NOT_NULL(encoded);

    status = minib64_decode(encoded, encoded_len, &decoded, &decoded_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_NOT_NULL(decoded);
    TEST_ASSERT_EQUAL_size_t(sizeof(in), decoded_len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(in, decoded, sizeof(in));

    free(encoded);
    free(decoded);
}

void test_decode_valid_dec_01(void) {
    const char *in = "";
    unsigned char *out = NULL;
    size_t out_len;

    minib64_status_t status = minib64_decode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_size_t(0, out_len);

    free(out);
}

void test_decode_valid_dec_02(void) {
    const char *in = "Zg==";
    unsigned char *out;
    size_t out_len;

    minib64_status_t status = minib64_decode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_size_t(1, out_len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const unsigned char *) "f", out, 1);

    free(out);
}

void test_decode_valid_dec_03(void) {
    const char *in = "Zm8=";
    unsigned char *out;
    size_t out_len;

    minib64_status_t status = minib64_decode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_size_t(2, out_len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const unsigned char *) "fo", out, 2);

    free(out);
}

void test_decode_valid_dec_04(void) {
    const char *in = "Zm9v";
    unsigned char *out;
    size_t out_len;

    minib64_status_t status = minib64_decode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_size_t(3, out_len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const unsigned char *) "foo", out, 3);

    free(out);
}

void test_decode_valid_dec_05(void) {
    const char *in = "Zm9vYg==";
    unsigned char *out;
    size_t out_len;

    minib64_status_t status = minib64_decode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_size_t(4, out_len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const unsigned char *) "foob", out, 4);

    free(out);
}

void test_decode_valid_dec_06(void) {
    const char *in = "Zm9vYmE=";
    unsigned char *out;
    size_t out_len;

    minib64_status_t status = minib64_decode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_size_t(5, out_len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const unsigned char *) "fooba", out, 5);

    free(out);
}

void test_decode_valid_dec_07(void) {
    const char *in = "Zm9vYmFy";
    unsigned char *out;
    size_t out_len;

    minib64_status_t status = minib64_decode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_size_t(6, out_len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const unsigned char *) "foobar", out, 6);

    free(out);
}

void test_decode_valid_dec_08(void) {
    const char *in = "AAAA";
    const unsigned char expected[] = {0x00, 0x00, 0x00};
    unsigned char *out;
    size_t out_len;

    minib64_status_t status = minib64_decode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_size_t(sizeof(expected), out_len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, out, sizeof(expected));

    free(out);
}

void test_decode_valid_dec_09(void) {
    const char *in = "////";
    const unsigned char expected[] = {0xFF, 0xFF, 0xFF};
    unsigned char *out;
    size_t out_len;

    minib64_status_t status = minib64_decode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_size_t(sizeof(expected), out_len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, out, sizeof(expected));

    free(out);
}

void test_decode_valid_dec_10(void) {
    const char *in = "+///";
    const unsigned char expected[] = {0xFB, 0xFF, 0xFF};
    unsigned char *out;
    size_t out_len;

    minib64_status_t status = minib64_decode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_SUCCESS, status);
    TEST_ASSERT_EQUAL_size_t(sizeof(expected), out_len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, out, sizeof(expected));

    free(out);
}

void test_decode_invalid_idec_01(void) {
    const char *in = "Zm9!"; // Invalid Base64 character, valid length
    unsigned char *out;
    size_t out_len;

    minib64_status_t status = minib64_decode(in, strlen(in), &out, &out_len);

    TEST_ASSERT_EQUAL_INT(MINIB64_ERR_INVALID_CHARACTER, status);
    TEST_ASSERT_NULL(out);
}

// INV-04 ("Zm9v") is valid Base64 and is already covered by DEC-04.
void test_decode_invalid_inv_01(void) {
    assert_decode_rejected("Zm9v!");
}

void test_decode_invalid_inv_02(void) {
    assert_decode_rejected("Zm9v@");
}

void test_decode_invalid_inv_03(void) {
    assert_decode_rejected("Zm9v#");
}

void test_decode_invalid_inv_05(void) {
    assert_decode_rejected("Zm9v\n");
}

void test_decode_invalid_inv_06(void) {
    assert_decode_rejected("Zm9v\r\n");
}

void test_decode_invalid_inv_07(void) {
    assert_decode_rejected("Zm9v\t");
}

void test_decode_invalid_inv_08(void) {
    assert_decode_rejected("Zm9v-");
}

void test_decode_invalid_inv_09(void) {
    assert_decode_rejected("Zm9v_");
}

void test_decode_invalid_inv_10(void) {
    assert_decode_rejected("Zm9v.");
}

void test_decode_invalid_len_01(void) {
    assert_decode_rejected("A");
}

void test_decode_invalid_len_02(void) {
    assert_decode_rejected("AA");
}

void test_decode_invalid_len_03(void) {
    assert_decode_rejected("AAA");
}

void test_decode_invalid_len_04(void) {
    assert_decode_rejected("AAAAA");
}

void test_decode_invalid_len_05(void) {
    assert_decode_rejected("AAAAAA");
}

void test_decode_invalid_len_06(void) {
    assert_decode_rejected("AAAAAAA");
}

void test_decode_invalid_pad_01(void) {
    assert_decode_rejected("=AAA");
}

void test_decode_invalid_pad_02(void) {
    assert_decode_rejected("A=AA");
}

void test_decode_invalid_pad_03(void) {
    assert_decode_rejected("AA=A");
}

void test_decode_invalid_pad_04(void) {
    const unsigned char expected[] = {0x00, 0x00};
    assert_decode_accepted("AAA=", expected, sizeof(expected));
}

void test_decode_invalid_pad_05(void) {
    const unsigned char expected[] = {0x00};
    assert_decode_accepted("AA==", expected, sizeof(expected));
}

void test_decode_invalid_pad_06(void) {
    assert_decode_rejected("A===");
}

void test_decode_invalid_pad_07(void) {
    assert_decode_rejected("AAAA=");
}

void test_decode_invalid_pad_08(void) {
    assert_decode_rejected("AA===");
}

void test_decode_invalid_pad_09(void) {
    assert_decode_rejected("====");
}

void test_decode_invalid_pad_10(void) {
    assert_decode_rejected("AAA==");
}

void test_decode_invalid_pad_11(void) {
    assert_decode_rejected("AA==AA==");
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

    RUN_TEST(test_encode_binary_data_bin_01);
    RUN_TEST(test_encode_binary_data_bin_02);
    RUN_TEST(test_encode_binary_data_bin_03);
    RUN_TEST(test_encode_binary_data_bin_04);
    RUN_TEST(test_encode_binary_data_bin_05);
    RUN_TEST(test_encode_binary_data_bin_06);
    RUN_TEST(test_binary_roundtrip_bin_07);

    RUN_TEST(test_decode_valid_dec_01);
    RUN_TEST(test_decode_valid_dec_02);
    RUN_TEST(test_decode_valid_dec_03);
    RUN_TEST(test_decode_valid_dec_04);
    RUN_TEST(test_decode_valid_dec_05);
    RUN_TEST(test_decode_valid_dec_06);
    RUN_TEST(test_decode_valid_dec_07);
    RUN_TEST(test_decode_valid_dec_08);
    RUN_TEST(test_decode_valid_dec_09);
    RUN_TEST(test_decode_valid_dec_10);

    RUN_TEST(test_decode_invalid_idec_01);

    RUN_TEST(test_decode_invalid_inv_01);
    RUN_TEST(test_decode_invalid_inv_02);
    RUN_TEST(test_decode_invalid_inv_03);
    RUN_TEST(test_decode_invalid_inv_05);
    RUN_TEST(test_decode_invalid_inv_06);
    RUN_TEST(test_decode_invalid_inv_07);
    RUN_TEST(test_decode_invalid_inv_08);
    RUN_TEST(test_decode_invalid_inv_09);
    RUN_TEST(test_decode_invalid_inv_10);

    RUN_TEST(test_decode_invalid_len_01);
    RUN_TEST(test_decode_invalid_len_02);
    RUN_TEST(test_decode_invalid_len_03);
    RUN_TEST(test_decode_invalid_len_04);
    RUN_TEST(test_decode_invalid_len_05);
    RUN_TEST(test_decode_invalid_len_06);

    RUN_TEST(test_decode_invalid_pad_01);
    RUN_TEST(test_decode_invalid_pad_02);
    RUN_TEST(test_decode_invalid_pad_03);
    RUN_TEST(test_decode_invalid_pad_04);
    RUN_TEST(test_decode_invalid_pad_05);
    RUN_TEST(test_decode_invalid_pad_06);
    RUN_TEST(test_decode_invalid_pad_07);
    RUN_TEST(test_decode_invalid_pad_08);
    RUN_TEST(test_decode_invalid_pad_09);
    RUN_TEST(test_decode_invalid_pad_10);
    RUN_TEST(test_decode_invalid_pad_11);

    return UNITY_END();
}
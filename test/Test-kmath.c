#include "kmath.h"
#include "unity.h"

void setUp(void){}

void tearDown(void){}

void test_ksignbit(void) {
    TEST_ASSERT_EQUAL_INT(1, ksignbit(-13));
    TEST_ASSERT_EQUAL_INT(0, ksignbit(13));
    TEST_ASSERT_EQUAL_INT(0, ksignbit(0));
    //TEST_ASSERT_EQUAL_INT(1, ksignbit(-262144));
    //TEST_ASSERT_EQUAL_INT(0, ksignbit(262144));
}

void test_kabs(void) {
    TEST_ASSERT_EQUAL_INT(13,kabs(-13));
    TEST_ASSERT_EQUAL_INT(13,kabs(13));
    TEST_ASSERT_EQUAL_INT(0,kabs(0));
    TEST_ASSERT_EQUAL_INT(922,kabs(-922));

}

void test_algebraic_sign(void) {
    TEST_ASSERT_EQUAL_INT(-1, algebraic_sign(-13));
    TEST_ASSERT_EQUAL_INT(1, algebraic_sign(13));
    TEST_ASSERT_EQUAL_INT(0, algebraic_sign(0));
    //TEST_ASSERT_EQUAL_INT(-1, algebraic_sign(-262144));
    //TEST_ASSERT_EQUAL_INT(1, algebraic_sign(262144));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_kabs);
    RUN_TEST(test_ksignbit);
    RUN_TEST(test_algebraic_sign);
    UNITY_END();
    return 0;
}
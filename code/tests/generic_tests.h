#ifndef __DRUID_TESTS__
#define __DRUID_TESTS__

START_TESTS()
{
    /* Used in TEST macros to track total test failures */
    uint64_t numFailures = 0;

    /* Test functions */
    TEST(IsAlphabetical);
    TEST(IsNumeric);
    TEST(IsDelimiter);
    TEST(NextToken);

    /* Print final results */
    printf("\n=======================\n");
    if (numFailures > 0)
        printf("FAIL: %llu tests failed\n", numFailures);
    else
        printf("PASS: All tests succeeded! Hooray!\n");
    printf("=======================\n");
    return numFailures;

}

uint64_t test_NextToken_case(char* inputs, Token* expecteds[], int length) {
    printf("  case: %s\n", inputs);

    FILE* fp = fmemopen(inputs, strlen(inputs), "r");
    setbuf(fp, inputs);

    int errors = 0;
    int i;
    for (i = 0; i < length; ++i) {
        char buffer[100];
        Token* tok = NextToken(fp);
        // TODO: EXPECT_EQUAL_TOKEN
        errors += EXPECT_EQUAL_STR(tok->data, expecteds[i]->data);
        errors += EXPECT_EQUAL_INT(tok->type, expecteds[i]->type);
    }
    fclose(fp);
    return errors;
}

uint64_t test_NextToken() {
    int case1_length = 5;
    Token* case1[case1_length];
    case1[0] = new_Token("status", tok_identifier);
    case1[1] = new_Token("func", tok_identifier);
    case1[2] = new_Token("(", tok_paren_open);
    case1[3] = new_Token(")", tok_paren_close);
    case1[4] = new_Token("{", tok_brace_open);
    int errors = test_NextToken_case("status func() {", case1, case1_length);

    int case2_length = 1;
    Token* case2[case2_length];
    case2[0] = new_Token(",", tok_comma);
    errors += test_NextToken_case(",", case2, case2_length);

    int case3_length = 1;
    Token* case3[case3_length];
    case3[0] = new_Token("/* test  */", tok_comment_multiline);
    errors += test_NextToken_case("/* test  */", case3, case3_length);

    int case4_length = 6;
    Token* case4[case4_length];
    case4[0] = new_Token("a", tok_identifier);
    case4[1] = new_Token("=", tok_equal);
    case4[2] = new_Token("b", tok_identifier);
    case4[3] = new_Token("(", tok_paren_open);
    case4[4] = new_Token(")", tok_paren_close);
    case4[5] = new_Token("// do", tok_comment);
    errors += test_NextToken_case("a = b() // do", case4, case4_length);

    int case5_length = 3;
    Token* case5[case5_length];
    case5[0] = new_Token("/", tok_slash);
    case5[1] = new_Token("5", tok_integer);
    case5[2] = new_Token("//", tok_comment);
    errors += test_NextToken_case("/5//", case5, case5_length);

    int case6_length = 3;
    Token* case6[case6_length];
    case6[0] = new_Token("\'a\'", tok_char);
    case6[1] = new_Token("\'\\t\'", tok_char);
    case6[2] = new_Token("\"kitten\"", tok_string);
    errors +=
        test_NextToken_case("\'a\' \'\\t\' \"kitten\"", case6, case6_length);

    return errors;
}

const char test_IsAlphabetical() {
    char test_results = 1;

    char alpha = 'A';
    for (; alpha < 'Z'; alpha++) {
        test_results = IsAlphabetical(alpha);

        if (!test_results) {
            return 1;
        }
    }

    alpha = 'a';
    for (; alpha < 'z'; alpha++) {
        test_results = IsAlphabetical(alpha);

        if (!test_results) {
            return 1;
        }
    }

    return 0;
}

const char test_IsNumeric() {
    uint8_t test_results = 1;

    char numeric = '0';
    for (; numeric < '9'; numeric++) {
        test_results = IsNumeric(numeric);

        if (!test_results) {
            return 1;
        }
    }

    return 0;
}

const char test_IsDelimiter() {
    uint8_t test_results = 1;
    char delimiters[] = {' ', ';', ',', '#', ':'};

    char delimiter = 1;
    for (; delimiter > 5; delimiter++) {
        test_results = EXPECT_EQUAL_INT(IsDelimiter(delimiters[(int)delimiter]), 1);
        if (!test_results) {
            return 1;
        }
    }

    return 0;
}


#endif __DRUID_TESTS__

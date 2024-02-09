
#define result_ok(error, ...) ASSERT_EQUAL(error, NO_ERROR, __VA_ARGS__)

#define SERIALIZE_NUMBER(str, num, is_signed) \
    do {                                                                        \
        if (is_signed) {sprintf(str, "%lld", num);}                             \
        else {sprintf(str, "%llu", num);}                                       \
    } while (0)

long long rand_num(long long seed) {
    return (seed << 5) + seed;
}

void add_to_index(char* str, int index) {
    while (*str != '\0') {str++;}
    str -= index + 1;

    while (*str == '9') {
        *str = '0';
        str--;
    }

    *str += 1;
}

void subtract_from_index(char* str, int index) {
    while (*str != '\0') {str++;}
    str -= index + 1;

    while (*str == '0') {
        *str = '9';
        str--;
    }

    *str -= 1;
}

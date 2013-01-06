
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string>

#if 0
 #define SQUASH_SPACES(s) squash_spaces(s)
#else
 #define SQUASH_SPACES(s) s
#endif

/* Valid operators are
 *     x--
 *     --x  -x  *x  ~x !x
 *     x*y
 *     x-y
 *     x|y
 *     x||y
 */

static void skip_space(const char *&p)
{ while (isspace(*p)) ++p; }

static bool parse_identifier(const char *&p)
{
    skip_space(p);
    if (isalpha(*p)) {
        while (isalpha(*p)) ++p;
        return true;
    }
    return false;
}

static bool parse_postfix(const char *&p)
{
    if (!parse_identifier(p)) return false;
    skip_space(p);
    while (p[0] == '-' && p[1] == '-') {
        p += 2;
        skip_space(p);
    }
    return true;
}

static bool parse_prefix(const char *&p)
{
    skip_space(p);
    switch (*p) {
        case '&':
            if (p[1] == '&') return false;
            goto default_case;
        case '-':
            if (p[1] == '-') ++p;
            goto default_case;
        case '!': case '~': case '*':
        default_case:
            ++p;
            return parse_prefix(p);
    }
    return parse_postfix(p);
}

static bool parse_multiplicative(const char *&p)
{
    if (!parse_prefix(p)) return false;
    if (*p == '*') {
        ++p;
        return parse_multiplicative(p);
    }
    return true;
}

static bool parse_additive(const char *&p)
{
    if (!parse_multiplicative(p)) return false;
    if (p[0] == '-' && p[1] != '-') {
        ++p;
        return parse_additive(p);
    }
    return true;
}

static bool parse_bitand(const char *&p)
{
    if (!parse_additive(p)) return false;
    if (p[0] == '&' && p[1] != '&') {
        ++p;
        return parse_bitand(p);
    }
    return true;
}

static bool parse_bitor(const char *&p)
{
    if (!parse_bitand(p)) return false;
    if (p[0] == '|' && p[1] != '|') {
        ++p;
        return parse_bitor(p);
    }
    return true;
}

static bool parse_booland(const char *&p)
{
    if (!parse_bitor(p)) return false;
    if (p[0] == '&' && p[1] == '&') {
        p += 2;
        return parse_booland(p);
    }
    return true;
}

static bool parse_boolor(const char *&p)
{
    if (!parse_booland(p)) return false;
    if (p[0] == '|' && p[1] == '|') {
        p += 2;
        return parse_boolor(p);
    }
    return true;
}

bool is_valid_expression(std::string s)
{
    const char *p = s.c_str();
    if (!parse_boolor(p)) return false;
    return (*p == '\0');
}

std::string squash_spaces(const std::string &st)
{
    std::string result = "";
    const char *s = st.c_str();
    for (int i=0; s[i] != '\0'; ++i) {
        if (isspace(s[i])) {
            if (!result.empty() && *result.rbegin() == '-' && s[i+1] == '-') result += " ";
            if (!result.empty() && *result.rbegin() == '&' && s[i+1] == '&') result += " ";
            if (!result.empty() && *result.rbegin() == '|' && s[i+1] == '|') result += " ";
        } else {
            result += s[i];
        }
    }
    return result;
}

static char R(const char *s)
{
    return s[rand() % strlen(s)];
}

std::string random_line(bool horizontal, int x)
{
    assert(x >= 0);

    std::string s = "";
    s += R("*o");
    for (int i=0; i < x; ++i) {
        if (horizontal) {
            s += R("-~");
        } else {
            s += (i==0 && (s[0] == 'o')) ? "|" : "!";
            s += " ";
        }
    }
    s += "o";

    return SQUASH_SPACES(s);
}

std::string random_rectangle(int x, int y)
{
    assert(x >= 0);
    assert(y >= 0);

    std::string s = "\n    ";
    s += R("*o");
    for (int i=0; i < x; ++i) s += R("~-");
    s += R("*o"); s += "\n    ";
    for (int j=0; j < y; ++j) {
        s += R("!|");
        for (int i=0; i < x; ++i) s += " ";
        s += R("!|"); s += "\n    ";
    }
    s += R("*o");
    for (int i=0; i < x; ++i) s += R("~-");
    s += R("*o");

    return SQUASH_SPACES(s);
}

std::string random_cuboid(int x, int y, int z)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(z >= 0);

    std::string s = "\n    ";
    s += R("*o");
    for (int i=0; i < x; ++i) s += R("~-");
    s += R("*o"); s += "\n    ";

    for (int j=0; j < y+z+1; ++j) {
        // Draw the left edge of the cuboid.
        if (j < y) {
            s += R("!|");
        } else if (j == y) {
            s += R("*o");
            for (int i=0; i < z; ++i) s += " ";
        } else {
            for (int i=0; i < j-y; ++i) s += " ";
            s += R("L&");
            for (int i = j-y; i < z; ++i) s += " ";
        }

        // Draw the middle and trailing edge.
        if (j < z) {
            for (int i=0; i < j; ++i) s += " ";
            s += R("L&");
            // for (int i=0; i < x; ++i) s += " ";
            // s += "\\";
        } else if (j == z) {
            for (int i=0; i < z; ++i) s += " ";
            s += R("*o");
            for (int i=0; i < x; ++i) s += R("~-");
            s += R("*o");
        } else {
            if (j < y) for (int i=0; i < z; ++i) s += " ";
            s += ((j >= y) ? R("!|") : '!');
            for (int i=0; i < x; ++i) s += " ";
            s += ((x==0) ? R("!|") : '!');
        }
        s += "\n    ";
    }

    // Draw the bottom, closest edge of the cube.
    for (int i=0; i < z+1; ++i) s += " ";
    s += R("*o");
    for (int i=0; i < x; ++i) s += R("~-");
    s += R("*o");

    return SQUASH_SPACES(s);
}

std::string random_valid_line(bool horizontal, int x)
{
    std::string s = random_line(horizontal, x);
    while (!is_valid_expression(s)) {
        s = random_line(horizontal, x);
    }
    return s;
}

std::string random_valid_rectangle(int x, int y)
{
    std::string s = random_rectangle(x,y);
    while (!is_valid_expression(s)) {
        s = random_rectangle(x,y);
    }
    return s;
}

std::string random_valid_cuboid(int x, int y, int z)
{
    std::string s = random_cuboid(x,y,z);
    while (!is_valid_expression(s)) {
        s = random_cuboid(x,y,z);
    }
    return s;
}

void print_line_test(bool horizontal, int x)
{
    std::string line_string = random_valid_line(horizontal, x);
    const char *r = line_string.c_str();
    printf("{\n");
    printf("    line<%d> r = %s;\n", x, r);
    printf("    assert((%s).length == %d);\n", r, x);
    printf("}\n");
}

void print_rectangle_test(int x, int y)
{
    std::string rect_string = random_valid_rectangle(x,y);
    const char *r = rect_string.c_str();
    printf("{\n");
    printf("    rectangle<%d,%d> r = %s;\n", x, y, r);
    printf("    assert((%s).width == %d);\n", r, x);
    printf("    assert((%s).height == %d);\n", r, y);
    printf("    assert((%s).area == %d);\n", r, x*y);
    printf("}\n");
}

void print_cuboid_test(int x, int y, int z)
{
    std::string cube_string = random_valid_cuboid(x,y,z);
    const char *r = cube_string.c_str();
    printf("{\n");
    printf("    cuboid<%d,%d,%d> r = %s;\n", x, y, z, r);
    printf("    assert((%s).width == %d);\n", r, x);
    printf("    assert((%s).height == %d);\n", r, y);
    printf("    assert((%s).depth == %d);\n", r, z);
    printf("    assert((%s).volume == %d);\n", r, x*y*z);
    printf("}\n");
}

int main()
{
    srand((unsigned)time(0));
    puts("");
    puts("#include <assert.h>");
    puts("#include \"analog-punct.hpp\"");
    puts("");
    puts("int main2()");
    puts("{");
    puts("    using namespace analog_literals::symbols;");
    puts("    using namespace analog_literals::shapes;");
    puts("");
    for (int i=0; i < 10; ++i) {
        print_line_test(/*horizontal=*/true, rand() % 10);
    }
    for (int i=0; i < 10; ++i) {
        print_line_test(/*horizontal=*/false, rand() % 10);
    }
    for (int i=0; i < 100; ++i) {
        print_rectangle_test(rand() % 8, 0);
    }
    for (int i=0; i < 100; ++i) {
        print_rectangle_test(rand() % 8, 1);
    }
    for (int i=0; i < 100; ++i) {
        print_rectangle_test(0, rand() % 8);
    }
    for (int i=0; i < 100; ++i) {
        print_rectangle_test(1, rand() % 8);
    }
    for (int i=0; i < 0; ++i) {
        int d = 1 + (rand() % 4);
        print_cuboid_test(rand() % 8, d+(1 + rand() % 3), d);
    }
    puts("    return 0;");
    puts("}");
    puts("");
}

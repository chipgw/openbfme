#include "log.hpp"
#include <vector>
#include <cstdio>

#ifdef OPENBFME_PLATFORM_MSVC
#define snprintf _snprintf
#endif

using namespace OpenBFME;

class FormatTest {
    string formatString;
    Printable arg;

public:
    FormatTest(string fmt, Printable a) : formatString(fmt), arg(a) {}

    int run(const integer& number) const {
        string result = format(formatString, arg);
        string correct = result;
        switch (arg.type){
        case Printable::Character:
            snprintf(&correct[0], correct.size() + 1, formatString.c_str(), arg.ch);
            break;
        case Printable::Decimal:
            snprintf(&correct[0], correct.size() + 1, formatString.c_str(), arg.dec);
            break;
        case Printable::Integer:
            snprintf(&correct[0], correct.size() + 1, formatString.c_str(), arg.num);
            break;
        case Printable::String:
            snprintf(&correct[0], correct.size() + 1, formatString.c_str(), arg.str);
            break;
        }

        if (correct != result) {
            Log::error("Test #%2d result not as expected! Expected: \"%s\" Result: \"%s\"", number, correct, result);
            return 1;
        }
        Log::info("Test #%2d result: \"%s\"", number, result);
        return 0;
    }
};

int main() {
    Log::init("log_test.log");

    int failed = 0;

    const static std::vector<FormatTest> tests {
                FormatTest("%s",    "Hi"),
                FormatTest("%d",    1234),
                FormatTest("%i",    1234),
                FormatTest("%u",    1234),
                FormatTest("%o",    1234),
                FormatTest("%#o",   1234),
                FormatTest("%x",    1234),
                FormatTest("%#06x", 1234),
                FormatTest("%#6x", 1234),
                FormatTest("%X",    1234),
                FormatTest("%#X",   1234),
                FormatTest("%f",    0.4f),
                FormatTest("%c",    '0')
    };

    for (int i = 0;  i < tests.size(); ++i) {
        failed += tests.at(i).run(i);
    }

    if (failed == 0) {
        Log::info("All tests successful!");
    } else {
        Log::error("%d test(s) failed!", failed);
    }

    return 0;
}

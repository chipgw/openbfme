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
        int correctNum = 0;
        switch (arg.type){
        case Printable::Character:
            correctNum = snprintf(&correct[0], correct.size() + 1, formatString.c_str(), arg.ch);
            break;
        case Printable::Decimal:
            correctNum = snprintf(&correct[0], correct.size() + 1, formatString.c_str(), arg.dec);
            break;
        case Printable::Integer:
            correctNum = snprintf(&correct[0], correct.size() + 1, formatString.c_str(), arg.num);
            break;
        case Printable::String:
            correctNum = snprintf(&correct[0], correct.size() + 1, formatString.c_str(), arg.str);
            break;
        }

        if(correctNum != result.size()) {
            Log::error("Test #%2d result is too short! Expected length: %i Result length: %i Result: \"%s\"", number, correctNum, result.size(), result);
            return 1;
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
                FormatTest("%s",    "Hello"),
                FormatTest("%.s",   "Hello"),
                FormatTest("%.2s",  "Hello"),
                FormatTest("%d",    1234),
                FormatTest("%i",    1234),
                FormatTest("%i",   -1234),
                FormatTest("%8i",   1234),
                FormatTest("%8i",  -1234),
                FormatTest("%08i",  1234),
                FormatTest("%08i", -1234),
                FormatTest("%+i",   1234),
                FormatTest("%+i",  -1234),
                FormatTest("%+8i",  1234),
                FormatTest("%+8i", -1234),
                FormatTest("%+08i", 1234),
                FormatTest("%+08i",-1234),
                FormatTest("%-8i",  1234),
                FormatTest("%-8i", -1234),
                FormatTest("%u",    1234),
                FormatTest("%o",    1234),
                FormatTest("%#o",   1234),
                FormatTest("%x",    1234),
                FormatTest("%08x",  1234),
                FormatTest("%#08x", 1234),
                FormatTest("%#8x",  1234),
                FormatTest("%X",    1234),
                FormatTest("%#X",   1234),
                FormatTest("%f",    0.4f),
                FormatTest("%.f",   0.4f),
                FormatTest("%.1f",  0.4f),
                FormatTest("%.3f",  0.4f),
                FormatTest("%.10f", 0.4f),
                FormatTest("%e",    0.4f),
                FormatTest("%E",    0.4f),
                FormatTest("%.1e",  0.4f),
                FormatTest("%c",    '0')
    };

    for (int i = 0;  i < tests.size(); ++i) {
        failed += tests.at(i).run(i);
    }

    if (failed == 0) {
        Log::info("All %i tests successful!", tests.size());
    } else {
        Log::error("%i/%i test(s) failed!", failed, tests.size());
    }

    return 0;
}

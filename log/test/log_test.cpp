#include "log.hpp"
#include <vector>

using namespace OpenBFME;

class FormatTest {
    string correct;
    string formatString;
    Printable arg;

public:
    FormatTest(string c, string fmt, Printable a) : correct(c), formatString(fmt), arg(a) {}

    int run(const integer& number) const {
        string result = format(formatString, arg);
        if (correct != result) {
            Log::error("Test #%d result not as expected! Expected: \"%s\" Result: \"%s\"", number + 1, correct, result);
            return 1;
        }
        Log::info("Test #%d result: \"%s\"", number + 1, result);
        return 0;
    }
};

int main() {
    Log::init("log_test.log");

    int failed = 0;

    const static std::vector<FormatTest> tests {
                FormatTest("Hi",    "%s",   "Hi"),
                FormatTest("1234",  "%d",   1234),
                FormatTest("1234",  "%i",   1234),
                FormatTest("1234",  "%u",   1234),
                FormatTest("2322",  "%o",   1234),
                FormatTest("4d2",   "%x",   1234),
                FormatTest("4D2",   "%X",   1234),
                FormatTest("0",     "%c",   '0')
    };

    for (int i = 0;  i < tests.size(); ++i) {
        failed += tests.at(i).run(i);
    }

    if (failed == 0) {
        Log::info("All tests succesfull!");
    } else {
        Log::error("%d test(s) failed!", failed);
    }

    return 0;
}

#include "application.hpp"
#include "log.hpp"
#include <vector>
#include <cstdio>

/* Because MSVC. */
#ifdef OPENBFME_COMPILER_MSVC
#define snprintf _snprintf
#endif

using namespace OpenBFME;

/* Compares string produced by our format() function to output of snprintf(). */
int runTest(const string& formatString, const Printable& arg, const integer& number) {
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

    /* It's possible that the buffer was too short. */
    if(correctNum != result.size()) {
        Log::error("Test #%2d result is too short! Expected length: %i Result length: %i Result: \"%s\"",
                   number, correctNum, integer(result.size()), result);
        return 1;
    }

    if (correct != result) {
        Log::error("Test #%2d result not as expected! Expected: \"%s\" Result: \"%s\"", number, correct, result);
        return 1;
    }
    Log::info("Test #%2d result: \"%s\"", number, result);
    return 0;
}

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

    int failed = 0;

    const static std::vector<std::pair<string, Printable>> tests {
        {"%s",    "Hello"}, {"%.s",   "Hello"}, {"%.2s",  "Hello"}, {"%d",    1234},
        {"%i",    1234},    {"%i",   -1234},    {"%8i",   1234},    {"%8i",  -1234},
        {"%08i",  1234},    {"%08i", -1234},    {"%+i",   1234},    {"%+i",  -1234},
        {"%+8i",  1234},    {"%+8i", -1234},    {"%+08i", 1234},    {"%+08i",-1234},
        {"%-8i",  1234},    {"%-8i", -1234},    {"%u",    1234},    {"%o",    1234},
        {"%#o",   1234},    {"%x",    1234},    {"%08x",  1234},    {"%#08x", 1234},
        {"%#8x",  1234},    {"%X",    1234},    {"%#X",   1234},    {"%f",    0.4f},
        {"%.f",   0.4f},    {"%.1f",  0.4f},    {"%.3f",  0.4f},    {"%.10f", 0.4f},
        {"%e",    0.4f},    {"%E",    0.4f},    {"%.1e",  0.4f},    {"%g",    0.4f},
        {"%G",    0.4f},    {"%g",    0.4e8f},  {"%G",    0.4e8f},  {"%c",    '0'}
    };

    for (int i = 0;  i < tests.size(); ++i) {
        failed += runTest(tests.at(i).first, tests.at(i).second, i);
    }

    if (failed == 0) {
        Log::info("All %i tests successful!", integer(tests.size()));
    } else {
        Log::error("%i/%i test(s) failed!", failed, integer(tests.size()));
    }

    return 0;
}

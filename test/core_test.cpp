#include "application.hpp"
#include "argumentsystem.hpp"
#include "log.hpp"

using namespace OpenBFME;

/* Because MSVC. */
#ifdef OPENBFME_COMPILER_MSVC
#define snprintf _snprintf
#endif

/* Compares string produced by our format() function to output of snprintf(). */
int runFormatTest(const string& formatString, const Printable& arg, const integer& number) {
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
    Log::debug("Test #%2d result: \"%s\"", number, result);
    return 0;
}

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

    auto boolArg = app.registerBoolArgument({"boolarg","b"}, "Test argument");
    auto intArg = app.registerIntegerArgument({"intarg","int","i"}, "Test argument");
    auto decArg = app.registerDecimalArgument({"decarg","d"}, "Test argument");
    auto strArg = app.registerStringArgument({"strarg","str"}, "Test argument");

    app.parseArguments();

    if(boolArg->valid)
        Log::info("boolarg value is \"%s\"", boolArg->boolResult ? "true" : "false");
    else
        Log::info("No valid boolarg was passed.");

    if(intArg->valid)
        Log::info("intarg value is \"%i\"", intArg->intResult);
    else
        Log::info("No valid intarg was passed.");

    if(decArg->valid)
        Log::info("decarg value is \"%f\"", decArg->decResult);
    else
        Log::info("No valid decarg was passed.");

    if(strArg->valid)
        Log::info("strarg value is \"%s\"", strArg->result);
    else
        Log::info("No valid strarg was passed.");

    auto& args = app.getRemainingArgs();

    if(args.size() > 0){
        Log::info("Arguments:");

        for(const string& arg : args){
            Log::info(arg);
        }
    }

    int failed = 0;

    const static std::vector<std::pair<string, Printable>> formatTests {
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

    for (int i = 0;  i < formatTests.size(); ++i)
        failed += runFormatTest(formatTests.at(i).first, formatTests.at(i).second, i);

    if(failed == 0)
        Log::info("All %i tests successful!", integer(formatTests.size()));
    else
        Log::error("%i/%i test(s) failed!", failed, integer(formatTests.size()));

    /* Check the error message. */
    Log::info("Now creating a second instance of Application, you should see an error message as that's against the rules.");
    Application app2(argc, argv);

    return 0;
}

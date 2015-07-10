#pragma once

#include "types.hpp"

namespace OpenBFME{

struct IniVariable {
    enum VariableType {
        Bool,   /* Put a true/false value in b. */
        Integer,/* Put an integer in i. */
        Decimal,/* Put a decimal number in d. */
        Percent,/* Map XXX% to X.XX and put it in d */
        String, /* Put a single string value in s. */
        Color,  /* Put a RGB(A) color value in v. */
        Vector, /* Put an XYZ vector in v. */
        Line    /* Put the rest of the line in s, use when built in parsing doesn't do what you need. */
    };

    const VariableType type;

    union {
        bool b;
        integer i;
        decimal d;
    };
    vec4 v;
    string s;

    IniVariable(const VariableType& t) : type(t) { }
};

}

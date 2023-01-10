//
// Copyright 2023 kla-sch@gmx.de
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef SIMDJSON_PEVAL_TEST_VALUE_STRING_TO_UINT64_H
#define SIMDJSON_PEVAL_TEST_VALUE_STRING_TO_UINT64_H 1

#include "Value.h"

struct ValueStringToUInt64Param : public ValueParam<uint64_t> {
    static std::vector<ValueStringToUInt64Param> cases;
};

class ValueStringToUInt64
    : public Value<uint64_t>
    , public testing::WithParamInterface<ValueStringToUInt64Param>
{
};

#endif /* SIMDJSON_PEVAL_TEST_VALUE_STRING_TO_UINT64_H */

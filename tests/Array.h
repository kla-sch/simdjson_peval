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

#ifndef SIMDJSON_PEVAL_TEST_ARRAY_H
#define SIMDJSON_PEVAL_TEST_ARRAY_H 1

#define _SIMDJSON_PEVAL_DO_TRACE 1
#include "simdjson_peval.h"

#include "gtest/gtest.h"

#include "TestUtils.h"

#include <string>
#include <string_view>
#include <variant>
#include <iostream>

template<typename T>
struct ArrayParam {
    struct Data {
        T result;

        simdjson_peval::_trace_buffer_type trace;
        simdjson_peval::error::message_container errors;
    };

    std::string_view name;
    std::string_view input;

    bool is_null;

    friend std::ostream&
    operator<<(std::ostream& os, const ArrayParam &param) {
        os << std::boolalpha
           << "{ name=\"" << param.name << "\""
           << " || input:" << param.input
           << " }";

        return os;
    }
};

// Test class base.
template<typename T>
class Array : public ::testing::Test {
};


// check result
template<typename Param, typename Value>
inline void
arrayCheckResult(
    const Param &param,
    const typename Param::Data &paramData,
    const Value &value,
    bool *is_null,
    const simdjson_peval::error &errors)
{
    using namespace simdjson_peval;

    if (!errors) {
        if (!is_null || !*is_null) {
            EXPECT_EQ(value, paramData.result);
        }

        if (is_null) {
            EXPECT_EQ(param.is_null, *is_null) << "(null state)";
        }
    }

    EXPECT_TRUE(sp_test::checkTrace(_trace_buffer, paramData.trace));
    EXPECT_TRUE(sp_test::checkErrors(errors, paramData.errors));
}


#endif /* SIMDJSON_PEVAL_TEST_ARRAY_H */

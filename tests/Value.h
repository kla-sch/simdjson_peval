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

#ifndef SIMDJSON_PEVAL_TEST_VALUE_H
#define SIMDJSON_PEVAL_TEST_VALUE_H 1

#define _SIMDJSON_PEVAL_DO_TRACE 1
#include "simdjson_peval.h"

#include "gtest/gtest.h"

#include "TestUtils.h"

#include <string>
#include <string_view>
#include <variant>


template<typename T>
struct ValueParam {
    struct Data {
        simdjson_peval::_trace_buffer_type trace;
        simdjson_peval::error::message_container errors;
    };

    std::string_view name;

    std::string_view input;

    T result;
    bool is_null;

    Data dataPlain;
    Data dataPlainWithNull;
    Data dataOptional;

    friend std::ostream&
    operator<<(std::ostream& os, const ValueParam &param) {
        os << std::boolalpha
           << "{ name=\"" << param.name << "\""
           << " || input:" << param.input
           << " || expected:" << param.result
           << " }";

        return os;
    }
};

// Test class base.
template<typename T>
class Value : public ::testing::Test {
};

// check plain result
template<typename Param, typename Value>
inline void
checkResult(
    const Param &param,
    const typename Param::Data &paramData,
    const Value &value,
    bool *is_null,
    const simdjson_peval::error &errors)
{
    using namespace simdjson_peval;

    if (!errors) {
        if (!is_null || !*is_null) {
            EXPECT_EQ(value, param.result);
        }

        if (is_null) {
            EXPECT_EQ(param.is_null, *is_null) << "(null state)";
        }
    }

    EXPECT_TRUE(sp_test::checkTrace(_trace_buffer, paramData.trace));
    EXPECT_TRUE(sp_test::checkErrors(errors, paramData.errors));
}

// check optional result
template<typename Param, typename Value>
inline void
checkResult(
    const Param &param,
    const typename Param::Data &paramData,
    const std::optional<Value> &value,
    const simdjson_peval::error &errors)
{
    using namespace simdjson_peval;

    if (!errors) {
        if (value) {
            EXPECT_EQ(value, param.result);
            EXPECT_FALSE(param.is_null) << "(null state)";
        }
        else {
            EXPECT_TRUE(param.is_null) << "(null state)";
        }
    }

    EXPECT_TRUE(sp_test::checkTrace(_trace_buffer, paramData.trace));
    EXPECT_TRUE(sp_test::checkErrors(errors, paramData.errors));
}

#endif /* SIMDJSON_PEVAL_TEST_VALUE_H */

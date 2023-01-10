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

#ifndef SIMDJSON_PEVAL_TEST_UTILS_HXX
#define SIMDJSON_PEVAL_TEST_UTILS_HXX 1

#include "gtest/gtest.h"

#include <iostream>

#ifdef __clang__
// clang++ evaluates function arguments from right to left.
#    define trace_param_order_right_to_left
#endif



/// write error message.
inline std::string
to_string(const simdjson_peval::error::message_container &messages)
{
    std::string result;
    for (const auto &msg : messages) {
        result += msg.to_string() + '\n';
    }

    return result;
};

namespace sp_test {

/**
 * Compare two trace vectors.
 */
inline testing::AssertionResult
checkTrace(
    const simdjson_peval::_trace_buffer_type &lh_trace,
    const simdjson_peval::_trace_buffer_type &rh_trace)
{
    if (lh_trace == rh_trace) {
        return testing::AssertionSuccess();
    }
    else {
        return (testing::AssertionFailure()
                << "\n"
                << ">>>>>>>>>>>>>>>>>>>> simdjson_peval trace\n"
                <<  lh_trace << '\n'
                << "<<<<<<<<<<<<<<<<<<<<\n\n"
                << ">>>>>>>>>>>>>>>>>>>> simdjson_peval trace\n"
                <<  rh_trace << '\n'
                << "<<<<<<<<<<<<<<<<<<<<\n"
            );
    }
}

bool
inline eq_error_messages(
    const simdjson_peval::error::message_container &lh_messages,
    const simdjson_peval::error::message_container &rh_messages)
{
    if (lh_messages.size() != rh_messages.size()) {
        return false;
    }

    return
        std::equal(
            lh_messages.begin(), lh_messages.end(), rh_messages.begin(),
            [](const auto &lhv, const auto &rhv) {
                return (lhv.get_code() == rhv.get_code()
                        && lhv.get_path() == rhv.get_path());
            });
}

/**
 * Errors with error message vector.
 */
inline testing::AssertionResult
checkErrors(
    const simdjson_peval::error &lh_errors,
    const simdjson_peval::error::message_container &rh_messages)
{
    const auto &lh_messages = lh_errors.get_messages();
    if (eq_error_messages(lh_messages, rh_messages)) {
        return testing::AssertionSuccess();
    }
    else {
        return (testing::AssertionFailure()
                << "\n"
                << ">>>>>>>>>>>>>>>>>>>> runtime errors\n"
                <<  to_string(lh_messages)
                << "<<<<<<<<<<<<<<<<<<<<\n\n"
                << ">>>>>>>>>>>>>>>>>>>> expected errors\n"
                <<  to_string(rh_messages)
                << "<<<<<<<<<<<<<<<<<<<<\n"
            );
    }
}


/// Get name of parameter set.
template<class ParamStruct>
inline std::string
paramSetName(const testing::TestParamInfo<ParamStruct> &info) {
    return std::string(info.param.name);
};

    
} // namespace vs_test

#endif /* SIMDJSON_PEVAL */

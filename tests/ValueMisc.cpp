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

#define _SIMDJSON_PEVAL_DO_TRACE 1
#include "simdjson_peval.h"

#include "gtest/gtest.h"

#include "TestUtils.h"

TEST(ValueRaw, Plain) {
    using namespace simdjson_peval;

    simdjson::ondemand::parser parser;
    auto json = R"({"value":10})"_padded;
    auto dataDoc = parser.iterate(json);

    simdjson_peval::_trace_buffer.clear();

    simdjson::simdjson_result<simdjson::ondemand::value> result;

    auto proto = value<simdjson::ondemand::document>(&result);

    error errors(true);
    proto(dataDoc, &errors);

    EXPECT_TRUE(
        sp_test::checkTrace(
            _trace_buffer,
            {
                // construct
                {"value(simdjson::simdjson_result<simdjson::ondemand::value>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},

                // evaluate
                {"value(ValuePtr,GetFn)", "eval_begin"},
                {    "value(simdjson::simdjson_result<simdjson::ondemand::value>*)", "start:get_value"},
                {    "value(simdjson::simdjson_result<simdjson::ondemand::value>*)", "type:other"},
                {"value(ValuePtr,GetFn)", "eval_end"},
            }));


    EXPECT_TRUE(sp_test::checkErrors(errors, {}));

    EXPECT_TRUE(result["value"].get_int64() == 10);
}


TEST(ValueRaw, Member) {
    using namespace simdjson_peval;

    simdjson::ondemand::parser parser;
    auto json = R"({"value":10})"_padded;
    auto dataDoc = parser.iterate(json);

    simdjson_peval::_trace_buffer.clear();

    simdjson::simdjson_result<simdjson::ondemand::value> result;

    auto proto =
        object<simdjson::ondemand::document>(
            member("value", value(&result)));

    error errors(true);
    proto(dataDoc, &errors);

    EXPECT_TRUE(
        sp_test::checkTrace(
            _trace_buffer,
            {
                {"value(simdjson::simdjson_result<simdjson::ondemand::value>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:value"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "value(simdjson::simdjson_result<simdjson::ondemand::value>*)", "start:get_value"},
                {            "value(simdjson::simdjson_result<simdjson::ondemand::value>*)", "type:value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"}
            }));

    EXPECT_TRUE(sp_test::checkErrors(errors, {}));
    EXPECT_TRUE(result.get_int64() == 10);
}

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


#include "ArrayPlain.h"

TEST_P(ArrayPlain, Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    TestDataArrayPlain result;

    auto eval_fn =
        array<simdjson::ondemand::document>(
            number_value(&result.id),
            string_value(&result.name),
            string_value(&result.description));

    error errors(true);
    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataPlain, result, nullptr, errors);
}


TEST_P(ArrayPlain, NullChecked) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    TestDataArrayPlain result;
    bool is_null;

    auto eval_fn =
        array<simdjson::ondemand::document>(
            &is_null,
            number_value(&result.id),
            string_value(&result.name),
            string_value(&result.description));

    error errors(true);
    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataNullChecked, result, &is_null, errors);
}

TEST_P(ArrayPlain, OneElement) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    TestDataArrayPlain result;

    auto eval_fn =
        array<simdjson::ondemand::document>(
            number_value(&result.id));

    error errors(true);
    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataOneElement, result, nullptr, errors);
}



// Parameters
////////////////////////////////////////////////////////////////////////

std::vector<ArrayPlainParam>
ArrayPlainParam::cases = {
    {
        {
            "full", R"([ 42, "name value", "description value" ])", false
        }, { // Plain
            {42, "name value", "description value"},
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"array(AllElementFn...)", "start"},
                {"internal::array(ElementFn,MoreElementFn...)", "start"},
                {"internal::array(ElementFn,MoreElementFn...)", "start"},
                {"internal::array(ElementFn)", "start"},

                // evaluate
                {"array(AllElementFn...)", "eval_begin"},
                {    "array(AllElementFn...)", "type:other"},
                {    "internal::array(ElementFn,MoreElementFn...)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "internal::array(ElementFn,MoreElementFn...)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "string_value(std::string_view*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "internal::array(ElementFn)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {                "value(ValuePtr,GetFn)", "eval_begin"},
                {                    "string_value(std::string_view*)", "start:get_value"},
                {                "value(ValuePtr,GetFn)", "eval_end"},
                {            "internal::array(ElementFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {        "internal::array(ElementFn,MoreElementFn...)", "eval_end"},
                {    "internal::array(ElementFn,MoreElementFn...)", "eval_end"},
                {"array(AllElementFn...)", "eval_end"},
            },{
                // no error
            }
        }, { // NullChecked
            {42, "name value", "description value"},
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"array(bool*,AllElementFn...)", "start"},
                {"array(AllElementFn...)", "start"},
                {"internal::array(ElementFn,MoreElementFn...)", "start"},
                {"internal::array(ElementFn,MoreElementFn...)", "start"},
                {"internal::array(ElementFn)", "start"},

                // evaluate
                {"array(bool*,AllElementFn...)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "array(AllElementFn...)", "eval_begin"},
                {        "array(AllElementFn...)", "type:array"},
                {        "internal::array(ElementFn,MoreElementFn...)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "number_value(uint64_t*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "internal::array(ElementFn,MoreElementFn...)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {                "value(ValuePtr,GetFn)", "eval_begin"},
                {                    "string_value(std::string_view*)", "start:get_value"},
                {                "value(ValuePtr,GetFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "internal::array(ElementFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {                    "value(ValuePtr,GetFn)", "eval_begin"},
                {                        "string_value(std::string_view*)", "start:get_value"},
                {                    "value(ValuePtr,GetFn)", "eval_end"},
                {                "internal::array(ElementFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {            "internal::array(ElementFn,MoreElementFn...)", "eval_end"},
                {        "internal::array(ElementFn,MoreElementFn...)", "eval_end"},
                {    "array(AllElementFn...)", "eval_end"},
                {"array(bool*,AllElementFn...)", "eval_end"}
            }, {
                // no error
            }
        }, { // OneElement
            {42, "", ""},
            {
                // construct
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"array(AllElementFn...)", "start"},
                {"internal::array(ElementFn)", "start"},

                // evaluate
                {"array(AllElementFn...)", "eval_begin"},
                {    "array(AllElementFn...)", "type:other"},
                {    "internal::array(ElementFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "internal::array(ElementFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"array(AllElementFn...)", "eval_end"},
            },{
                // no error
            }
        }
    },  {
        {
            "part", R"([ 42 ])", false
        }, { // Plain
            {42, "name value", "description value"},
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"array(AllElementFn...)", "start"},
                {"internal::array(ElementFn,MoreElementFn...)", "start"},
                {"internal::array(ElementFn,MoreElementFn...)", "start"},
                {"internal::array(ElementFn)", "start"},

                // evaluate
                {"array(AllElementFn...)", "eval_begin"},
                {    "array(AllElementFn...)", "type:other"},
                {    "internal::array(ElementFn,MoreElementFn...)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "internal::array(ElementFn,MoreElementFn...)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {            "internal::array(ElementFn,MoreElementFn...)", "error:18"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "internal::array(ElementFn)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {                "internal::array(ElementFn)", "error:18"},
                {            "internal::array(ElementFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {        "internal::array(ElementFn,MoreElementFn...)", "eval_end"},
                {    "internal::array(ElementFn,MoreElementFn...)", "eval_end"},
                {"array(AllElementFn...)", "eval_end"},
            },{
                {simdjson::INDEX_OUT_OF_BOUNDS , "<root>[1]"},
                {simdjson::INDEX_OUT_OF_BOUNDS , "<root>[2]"}
            }
        }, { // NullChecked
            {42, "name value", "description value"},
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"array(bool*,AllElementFn...)", "start"},
                {"array(AllElementFn...)", "start"},
                {"internal::array(ElementFn,MoreElementFn...)", "start"},
                {"internal::array(ElementFn,MoreElementFn...)", "start"},
                {"internal::array(ElementFn)", "start"},

                // evaluate
                {"array(bool*,AllElementFn...)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "array(AllElementFn...)", "eval_begin"},
                {        "array(AllElementFn...)", "type:array"},
                {        "internal::array(ElementFn,MoreElementFn...)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "number_value(uint64_t*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "internal::array(ElementFn,MoreElementFn...)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {                "internal::array(ElementFn,MoreElementFn...)", "error:18"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "internal::array(ElementFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {                    "internal::array(ElementFn)", "error:18"},
                {                "internal::array(ElementFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {            "internal::array(ElementFn,MoreElementFn...)", "eval_end"},
                {        "internal::array(ElementFn,MoreElementFn...)", "eval_end"},
                {    "array(AllElementFn...)", "eval_end"},
                {"array(bool*,AllElementFn...)", "eval_end"}
            }, {
                {simdjson::INDEX_OUT_OF_BOUNDS , "<root>[1]"},
                {simdjson::INDEX_OUT_OF_BOUNDS , "<root>[2]"}
            }
        }, { // OneElement
            {42, "", ""},
            {
                // construct
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"array(AllElementFn...)", "start"},
                {"internal::array(ElementFn)", "start"},

                // evaluate
                {"array(AllElementFn...)", "eval_begin"},
                {    "array(AllElementFn...)", "type:other"},
                {    "internal::array(ElementFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "internal::array(ElementFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"array(AllElementFn...)", "eval_end"},
            },{
                // no error
            }
        }
    }, {
        {
            "null", R"(null)", true
        }, { // Plain
            {}, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"array(AllElementFn...)", "start"},
                {"internal::array(ElementFn,MoreElementFn...)", "start"},
                {"internal::array(ElementFn,MoreElementFn...)", "start"},
                {"internal::array(ElementFn)", "start"},

                // evaluate
                {"array(AllElementFn...)", "eval_begin"},
                {    "array(AllElementFn...)", "type:other"},
                {    "array(AllElementFn...)", "error:16"},
                {"array(AllElementFn...)", "eval_end"},
            }, {
                {simdjson::INCORRECT_TYPE , "<root>"},
            }
        }, { // NullChecked
            {}, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"array(bool*,AllElementFn...)", "start"},
                {"array(AllElementFn...)", "start"},
                {"internal::array(ElementFn,MoreElementFn...)", "start"},
                {"internal::array(ElementFn,MoreElementFn...)", "start"},
                {"internal::array(ElementFn)", "start"},

                // evaluate
                {"array(bool*,AllElementFn...)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:true"},
                {    "array(bool*,AllElementFn...)", "is_null"},
                {"array(bool*,AllElementFn...)", "eval_end"},
            }, {
                // no error
            }
        }, { // OneElement
            {}, {
                // construct
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"array(AllElementFn...)", "start"},
                {"internal::array(ElementFn)", "start"},

                // evaluate
                {"array(AllElementFn...)", "eval_begin"},
                {    "array(AllElementFn...)", "type:other"},
                {    "array(AllElementFn...)", "error:16"},
                {"array(AllElementFn...)", "eval_end"},
            }, {
                {simdjson::INCORRECT_TYPE , "<root>"},
            }
        }
    }
};

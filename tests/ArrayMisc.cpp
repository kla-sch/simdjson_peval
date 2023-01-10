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

#include "ArrayMisc.h"

TEST_P(ArrayMisc,Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    TestDataArrayMisc result;

    simdjson::simdjson_result<simdjson::ondemand::array> sj_array;
    auto eval_fn = array<simdjson::ondemand::document>(&sj_array);

    error errors(true);
    eval_fn(dataDoc, &errors);

    auto aIter = sj_array.begin();

    auto get_value =
        [&errors, &sj_array, &aIter]
        (auto idx, auto *location) {
            error::path_scope element_scope(&errors, idx);

            auto code =
                (aIter == sj_array.end()
                 ? simdjson::INDEX_OUT_OF_BOUNDS
                 : simdjson::SUCCESS);

            if (code) {
                errors.add(code);
            }
            else {
                auto sj_value = *aIter;
                code = sj_value.get(*location);
                if (code) {
                    errors.add(code);
                }
                else {
                    ++aIter;
                }
            }
        };

    if (!errors) {
        auto idx = 0;

        get_value(idx++, &result.id);
        get_value(idx++, &result.name);
        get_value(idx++, &result.description);
    }

    arrayCheckResult(
        param, param.dataPlain, result, nullptr, errors);
}


TEST_P(ArrayMisc,NullChecked) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    TestDataArrayMisc result;
    bool is_null;

    simdjson::simdjson_result<simdjson::ondemand::array> sj_array;
    auto eval_fn =
        array<simdjson::ondemand::document>(
            &sj_array, &is_null);

    error errors(true);
    eval_fn(dataDoc, &errors);

    auto aIter = sj_array.begin();

    auto get_value =
        [&errors, &sj_array, &aIter]
        (auto idx, auto *location) {
            error::path_scope element_scope(&errors, idx);

            auto code =
                (aIter == sj_array.end()
                 ? simdjson::INDEX_OUT_OF_BOUNDS
                 : simdjson::SUCCESS);

            if (code) {
                errors.add(code);
            }
            else {
                auto sj_value = *aIter;
                code = sj_value.get(*location);
                if (code) {
                    errors.add(code);
                }
                else {
                    ++aIter;
                }
            }
        };

    if (!errors && !is_null) {
        auto idx = 0;

        get_value(idx++, &result.id);
        get_value(idx++, &result.name);
        get_value(idx++, &result.description);
    }

    arrayCheckResult(
        param, param.dataNullChecked, result, &is_null, errors);
}



TEST_P(ArrayMisc,FromMember) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    error errors(true);
    simdjson::ondemand::parser parser;
    auto newInput = "{\"result\":" + std::string(param.input) + '}';
    auto json = simdjson::padded_string(newInput);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    TestDataArrayMisc result;

    simdjson::simdjson_result<simdjson::ondemand::array> sj_array;
    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("result", array(&sj_array)));

    eval_fn(dataDoc, &errors);

    auto aIter = sj_array.begin();

    auto get_value =
        [&errors, &sj_array, &aIter]
        (auto idx, auto *location) {
            error::path_scope element_scope(&errors, idx);

            auto code =
                (aIter == sj_array.end()
                 ? simdjson::INDEX_OUT_OF_BOUNDS
                 : simdjson::SUCCESS);

            if (code) {
                errors.add(code);
            }
            else {
                auto sj_value = *aIter;
                code = sj_value.get(*location);
                if (code) {
                    errors.add(code);
                }
                else {
                    ++aIter;
                }
            }
        };

    if (!errors) {
        auto idx = 0;

        get_value(idx++, &result.id);
        get_value(idx++, &result.name);
        get_value(idx++, &result.description);
    }

    arrayCheckResult(
        param, param.dataFromMember, result, nullptr, errors);

}


std::vector<ArrayMiscParam>
ArrayMiscParam::cases = {
    {
        {
            "full", R"([42, "name value", "description value"])", false
        }, { // Plain
            {42, "name value", "description value"},
            {
                // construct
                {"array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},

                // evaluate
                {"value(ValuePtr,GetFn)", "eval_begin"},
                {    "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "start:get_value"},
                {    "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "type:other"},
                {"value(ValuePtr,GetFn)", "eval_end"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {"error::path_scope::~path_scope()", "delete"}
            }, {
                // no errors
            }
        }, { // NullChecked
            {42, "name value", "description value"},
            {
                // construct
                {"array(simdjson::simdjson_result<simdjson::ondemand::array>*,bool*)", "start"},
                {"value(ValuePtr,GetFn,bool*)", "start"},

                // evaluate
                {"value(ValuePtr,GetFn,bool*)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "array(simdjson::simdjson_result<simdjson::ondemand::array>*,bool*)", "start:get_value"},
                {    "array(simdjson::simdjson_result<simdjson::ondemand::array>*,bool*)", "type:other"},
                {"value(ValuePtr,GetFn,bool*)", "eval_end"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {"error::path_scope::~path_scope()", "delete"}
            }, {
                // no errors
            }
        }, { // FromMember
            {42, "name value", "description value"},
            {
                // construct
                {"array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "start:get_value"},
                {            "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "type:other"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {"error::path_scope::~path_scope()", "delete"},
            }, {
                // no errors
            }
        }
    }, {
        {
            "oneMember", R"([42])", false
        }, { // Plain
            {42,  "", ""},
            {
                // construct
                {"array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},

                // evaluate
                {"value(ValuePtr,GetFn)", "eval_begin"},
                {    "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "start:get_value"},
                {    "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "type:other"},
                {"value(ValuePtr,GetFn)", "eval_end"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {"error::path_scope::~path_scope()", "delete"}
            }, { // errors
                {simdjson::INDEX_OUT_OF_BOUNDS, "<root>[1]"},
                {simdjson::INDEX_OUT_OF_BOUNDS, "<root>[2]"},
            }
        }, { // NullChecked
            {42,  "", ""},
            {
                // construct
                {"array(simdjson::simdjson_result<simdjson::ondemand::array>*,bool*)", "start"},
                {"value(ValuePtr,GetFn,bool*)", "start"},

                // evaluate
                {"value(ValuePtr,GetFn,bool*)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "array(simdjson::simdjson_result<simdjson::ondemand::array>*,bool*)", "start:get_value"},
                {    "array(simdjson::simdjson_result<simdjson::ondemand::array>*,bool*)", "type:other"},
                {"value(ValuePtr,GetFn,bool*)", "eval_end"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {"error::path_scope::~path_scope()", "delete"}
            }, { // errors
                {simdjson::INDEX_OUT_OF_BOUNDS, "<root>[1]"},
                {simdjson::INDEX_OUT_OF_BOUNDS, "<root>[2]"},
            }
        }, { // FromMember
            {42,  "", ""},
            {
                // construct
                {"array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "start:get_value"},
                {            "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "type:other"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {"error::path_scope::~path_scope()", "delete"},
            }, { // errors
                {simdjson::INDEX_OUT_OF_BOUNDS, "<root>[1]"},
                {simdjson::INDEX_OUT_OF_BOUNDS, "<root>[2]"},
            }
        }
    }, {
        {
            "null", R"(null)", true
        }, { // Plain
            {0, "", ""},
            {
                // construct
                {"array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},

                // evaluate
                {"value(ValuePtr,GetFn)", "eval_begin"},
                {    "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "start:get_value"},
                {    "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "type:other"},
                {    "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "error:16"},
                {"value(ValuePtr,GetFn)", "eval_end"},
            }, { // errors
                {simdjson::INCORRECT_TYPE, "<root>"}
            }
        }, { // NullChecked
            {0, "", ""},
            {
                // construct
                {"array(simdjson::simdjson_result<simdjson::ondemand::array>*,bool*)", "start"},
                {"value(ValuePtr,GetFn,bool*)", "start"},

                // evaluate
                {"value(ValuePtr,GetFn,bool*)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:true"},
                {    "value(ValuePtr,GetFn,bool*)", "is_null"},
                {"value(ValuePtr,GetFn,bool*)", "eval_end"},
            }, { // errors
            }
        }, { // FromMember
            {0, "", ""},
            {
                // construct
                {"array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "start:get_value"},
                {            "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "type:other"},
                {            "array(simdjson::simdjson_result<simdjson::ondemand::array>*)", "error:16"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"},
            }, { // errors
                {simdjson::INCORRECT_TYPE, "<root>.result"}
            }
        }
    }
};

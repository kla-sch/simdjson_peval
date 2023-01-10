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

#include "ObjectPlain.h"

TEST_P(ObjectPlain, Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    TestDataObjectPlain result;

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("id", number_value(&result.id)),
            member("name", string_value(&result.name)),
            member("description", string_value(&result.description)));

    error errors(true);
    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataPlain, result, nullptr, errors);
}


TEST_P(ObjectPlain, NullChecked) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    TestDataObjectPlain result;
    bool is_null;

    auto eval_fn =
        object<simdjson::ondemand::document>(
            &is_null,
            member("id", number_value(&result.id)),
            member("name", string_value(&result.name)),
            member("description", string_value(&result.description)));

    error errors(true);
    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataNullChecked, result, &is_null, errors);
}


TEST_P(ObjectPlain, OptMembers) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    TestDataObjectPlain result;

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("id", number_value(&result.id)),
            member("name",
                   string_value(&result.name),
                   &result.no_name),
            member("description",
                   string_value(&result.description),
                   &result.no_description));

    error errors(true);
    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataOptMembers, result, nullptr, errors);
}


TEST_P(ObjectPlain, OneElement) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    TestDataObjectPlain result;

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("id", number_value(&result.id)));

    error errors(true);
    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataOneElement, result, nullptr, errors);
}



// Parameters
////////////////////////////////////////////////////////////////////////

std::vector<ObjectPlainParam>
ObjectPlainParam::cases = {
    {
        {
            "full", R"(
            {
                "id": 42,
                "name": "name value",
                "description": "description value"
            })", false
        }, { // Plain
            {42, "name value", false, "description value", false},
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*,bool*)", "start"},
#else
                {"string_value(std::string_view*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*,bool*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*,bool*)", "start"},
#else
                {"number_value(uint64_t*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*,bool*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "string_value(std::string_view*,bool*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {        "member(std::string_view,ValueFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn)", "eval_begin"},
                {            "object(MemberFn)", "type:object"},
                {            "member(std::string_view,ValueFn)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:description"},
                {                "value(ValuePtr,GetFn)", "eval_begin"},
                {                    "string_value(std::string_view*,bool*)", "start:get_value"},
                {                "value(ValuePtr,GetFn)", "eval_end"},
                {            "member(std::string_view,ValueFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},
            },
            {
            }
        }, { // NullChecked
            {42, "name value", false, "description value", false},
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*,bool*)", "start"},
#else
                {"string_value(std::string_view*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*,bool*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*,bool*)", "start"},
#else
                {"number_value(uint64_t*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(bool*,AllMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(bool*,AllMemberFn...)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "number_value(uint64_t*,bool*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {        "member(std::string_view,ValueFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {            "object(MemberFn,MoreMemberFn...)", "type:object"},
                {            "member(std::string_view,ValueFn)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                "value(ValuePtr,GetFn)", "eval_begin"},
                {                    "string_value(std::string_view*,bool*)", "start:get_value"},
                {                "value(ValuePtr,GetFn)", "eval_end"},
                {            "member(std::string_view,ValueFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn)", "eval_begin"},
                {                "object(MemberFn)", "type:object"},
                {                "member(std::string_view,ValueFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:description"},
                {                    "value(ValuePtr,GetFn)", "eval_begin"},
                {                        "string_value(std::string_view*,bool*)", "start:get_value"},
                {                    "value(ValuePtr,GetFn)", "eval_end"},
                {                "member(std::string_view,ValueFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn)", "eval_end"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(bool*,AllMemberFn...)", "eval_end"}
            },
            {
            }
        }, { // OptMembers
            {42, "name value", false, "description value", false},
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*,bool*)", "start"},
#else
                {"string_value(std::string_view*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"member(std::string_view,ValueFn)", "start"},
#else
                {"member(std::string_view,ValueFn,bool*)", "start"},
#endif
                {"string_value(std::string_view*,bool*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn,bool*)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*,bool*)", "start"},
#else
                {"number_value(uint64_t*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"member(std::string_view,ValueFn,bool*)", "start"},
#else
                {"member(std::string_view,ValueFn)", "start"},
#endif
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*,bool*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn,bool*)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "string_value(std::string_view*,bool*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {        "member(std::string_view,ValueFn,bool*)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn)", "eval_begin"},
                {            "object(MemberFn)", "type:object"},
                {            "member(std::string_view,ValueFn,bool*)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:description"},
                {                "value(ValuePtr,GetFn)", "eval_begin"},
                {                    "string_value(std::string_view*,bool*)", "start:get_value"},
                {                "value(ValuePtr,GetFn)", "eval_end"},
                {            "member(std::string_view,ValueFn,bool*)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},
            },
            {
            }
        }, { // OneElement
            {42, "", false, "", false},
            {
                // construct
                {"number_value(uint64_t*,bool*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*,bool*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"},
            },
            {
            }
        }
    }, {
        {
            "part", R"(
            {
                "id": 42
            })", false
        }, { // Plain
            {},
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*,bool*)", "start"},
#else
                {"string_value(std::string_view*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*,bool*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*,bool*)", "start"},
#else
                {"number_value(uint64_t*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*,bool*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {            "member(std::string_view,ValueFn)", "error:19"},
                {        "member(std::string_view,ValueFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn)", "eval_begin"},
                {            "object(MemberFn)", "type:object"},
                {            "member(std::string_view,ValueFn)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:description"},
                {                "member(std::string_view,ValueFn)", "error:19"},
                {            "member(std::string_view,ValueFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},
            },
            {
                {simdjson::NO_SUCH_FIELD , "<root>.name"},
                {simdjson::NO_SUCH_FIELD , "<root>.description"}
            }
        }, { // NullChecked
            {},
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*,bool*)", "start"},
#else
                {"string_value(std::string_view*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*,bool*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*,bool*)", "start"},
#else
                {"number_value(uint64_t*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(bool*,AllMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(bool*,AllMemberFn...)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "number_value(uint64_t*,bool*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {        "member(std::string_view,ValueFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {            "object(MemberFn,MoreMemberFn...)", "type:object"},
                {            "member(std::string_view,ValueFn)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                "member(std::string_view,ValueFn)", "error:19"},
                {            "member(std::string_view,ValueFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn)", "eval_begin"},
                {                "object(MemberFn)", "type:object"},
                {                "member(std::string_view,ValueFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:description"},
                {                    "member(std::string_view,ValueFn)", "error:19"},
                {                "member(std::string_view,ValueFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn)", "eval_end"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(bool*,AllMemberFn...)", "eval_end"}
            },
            {
                {simdjson::NO_SUCH_FIELD , "<root>.name"},
                {simdjson::NO_SUCH_FIELD , "<root>.description"}
            }
        }, { // OptMembers
            {42, "", true, "", true},
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*,bool*)", "start"},
#else
                {"string_value(std::string_view*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"member(std::string_view,ValueFn)", "start"},
#else
                {"member(std::string_view,ValueFn,bool*)", "start"},
#endif
                {"string_value(std::string_view*,bool*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn,bool*)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*,bool*)", "start"},
#else
                {"number_value(uint64_t*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"member(std::string_view,ValueFn,bool*)", "start"},
#else
                {"member(std::string_view,ValueFn)", "start"},
#endif
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*,bool*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn,bool*)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {            "member(std::string_view,ValueFn,bool*)", "undefined_member:name"},
                {        "member(std::string_view,ValueFn,bool*)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn)", "eval_begin"},
                {            "object(MemberFn)", "type:object"},
                {            "member(std::string_view,ValueFn,bool*)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:description"},
                {                "member(std::string_view,ValueFn,bool*)", "undefined_member:description"},
                {            "member(std::string_view,ValueFn,bool*)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},
            },
            {
            }
        }, { // OneElement
            {42, "", false, "", false},
            {
                // construct
                {"number_value(uint64_t*,bool*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*,bool*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"},
            },
            {
            }
        }
    }, {
        {
            "null", R"(null)", true
        }, { // Plain
            {},
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*,bool*)", "start"},
#else
                {"string_value(std::string_view*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*,bool*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*,bool*)", "start"},
#else
                {"number_value(uint64_t*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "object(MemberFn,MoreMemberFn...)", "error:16"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},
            },
            {
                {simdjson::INCORRECT_TYPE , "<root>"},
            }
        }, { // NullChecked
            {},
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*,bool*)", "start"},
#else
                {"string_value(std::string_view*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*,bool*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*,bool*)", "start"},
#else
                {"number_value(uint64_t*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(bool*,AllMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(bool*,AllMemberFn...)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:true"},
                {    "object(bool*,AllMemberFn...)", "is_null"},
                {"object(bool*,AllMemberFn...)", "eval_end"},
            },
            {
            }
        }, { // OptMembers
            {},
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*,bool*)", "start"},
#else
                {"string_value(std::string_view*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"member(std::string_view,ValueFn)", "start"},
#else
                {"member(std::string_view,ValueFn,bool*)", "start"},
#endif
                {"string_value(std::string_view*,bool*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn,bool*)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*,bool*)", "start"},
#else
                {"number_value(uint64_t*,bool*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"member(std::string_view,ValueFn,bool*)", "start"},
#else
                {"member(std::string_view,ValueFn)", "start"},
#endif
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "object(MemberFn,MoreMemberFn...)", "error:16"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},
            },
            {
                {simdjson::INCORRECT_TYPE , "<root>"},
            }
        }, { // OneElement
            {},
            {
                // construct
                {"number_value(uint64_t*,bool*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "object(MemberFn)", "error:16"},
                {"object(MemberFn)", "eval_end"},
            },
            {
                {simdjson::INCORRECT_TYPE , "<root>"},
            }
        }
    }
};

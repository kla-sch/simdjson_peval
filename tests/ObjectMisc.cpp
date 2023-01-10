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

#include "ObjectMisc.h"

TEST_P(ObjectMisc,Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    TestDataObjectMisc result;

    simdjson::simdjson_result<simdjson::ondemand::object> sj_object;
    auto eval_fn = object<simdjson::ondemand::document>(&sj_object);

    error errors(true);
    eval_fn(dataDoc, &errors);

    auto get_value =
        [&errors, &sj_object](auto name, auto *location) {
            error::path_scope member_scope(&errors, name);
            auto sj_value = sj_object.value().find_field_unordered(name);
            auto code = sj_value.error();

            if (code) {
                errors.add(code);
            }
            else {
                code = sj_value.get(*location);
                if (code) {
                    errors.add(code);
                }
            }
        };

    if (!errors) {
        get_value("id", &result.id);
        get_value("name", &result.name);
        get_value("description", &result.description);
    }

    objectCheckResult(
        param, param.dataPlain, result, nullptr, errors);
}


TEST_P(ObjectMisc,NullChecked) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    TestDataObjectMisc result;
    bool is_null;

    simdjson::simdjson_result<simdjson::ondemand::object> sj_object;
    auto eval_fn = object<simdjson::ondemand::document>(&sj_object, &is_null);

    error errors(true);
    eval_fn(dataDoc, &errors);

    auto get_value =
        [&errors, &sj_object](auto name, auto *location) {
            error::path_scope member_scope(&errors, name);
            auto sj_value = sj_object.value().find_field_unordered(name);
            auto code = sj_value.error();

            if (code) {
                errors.add(code);
            }
            else {
                code = sj_value.get(*location);
                if (code) {
                    errors.add(code);
                }
            }
        };

    if (!errors && !is_null) {
        get_value("id", &result.id);
        get_value("name", &result.name);
        get_value("description", &result.description);
    }

    objectCheckResult(
        param, param.dataNullChecked, result, &is_null, errors);
}



TEST_P(ObjectMisc,FromMember) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    error errors(true);
    simdjson::ondemand::parser parser;
    auto newInput = "{\"result\":" + std::string(param.input) + '}';
    auto json = simdjson::padded_string(newInput);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    TestDataObjectMisc result;

    simdjson::simdjson_result<simdjson::ondemand::object> sj_object;
    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("result", object(&sj_object)));

    eval_fn(dataDoc, &errors);

    auto get_value =
        [&errors, &sj_object](auto name, auto *location) {
            error::path_scope member_scope(&errors, name);
            auto sj_value = sj_object.value().find_field_unordered(name);
            auto code = sj_value.error();

            if (code) {
                errors.add(code);
            }
            else {
                code = sj_value.get(*location);
                if (code) {
                    errors.add(code);
                }
            }
        };

    if (!errors) {
        get_value("id", &result.id);
        get_value("name", &result.name);
        get_value("description", &result.description);
    }

    objectCheckResult(
        param, param.dataFromMember, result, nullptr, errors);

}


std::vector<ObjectMiscParam>
ObjectMiscParam::cases = {
    {
        {
            "full", R"(
            {
                "id": 42,
                "name": "name value",
                "description": "description value"
            })", false
        }, { // Plain
            {42, "name value", "description value"},
            {
                // construct
                {"object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},

                // evaluate
                {"value(ValuePtr,GetFn)", "eval_begin"},
                {    "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "start:get_value"},
                {    "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "type:other"},
                {"value(ValuePtr,GetFn)", "eval_end"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:description"},
                {"error::path_scope::~path_scope()", "delete"}
            }, {
                // no errors
            }
        }, { // NullChecked
            {42, "name value", "description value"},
            {
                // construct
                {"object(simdjson::simdjson_result<simdjson::ondemand::object>*,bool*)", "start"},
                {"value(ValuePtr,GetFn,bool*)", "start"},

                // evaluate
                {"value(ValuePtr,GetFn,bool*)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "object(simdjson::simdjson_result<simdjson::ondemand::object>*,bool*)", "start:get_value"},
                {    "object(simdjson::simdjson_result<simdjson::ondemand::object>*,bool*)", "type:other"},
                {"value(ValuePtr,GetFn,bool*)", "eval_end"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:description"},
                {"error::path_scope::~path_scope()", "delete"}
            }, {
                // no errors
            }
        }, { // FromMember
            {42, "name value", "description value"},
            {
                // construct
                {"object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "start:get_value"},
                {            "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "type:other"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:description"},
                {"error::path_scope::~path_scope()", "delete"},
            }, {
                // no errors
            }
        }
    }, {
        {
            "oneMember", R"(
            {
                "id": 42
            })", false
        }, { // Plain
            {42,  "", ""},
            {
                // construct
                {"object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},

                // evaluate
                {"value(ValuePtr,GetFn)", "eval_begin"},
                {    "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "start:get_value"},
                {    "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "type:other"},
                {"value(ValuePtr,GetFn)", "eval_end"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:description"},
                {"error::path_scope::~path_scope()", "delete"}
            }, { // errors
                {simdjson::NO_SUCH_FIELD, "<root>.name"},
                {simdjson::NO_SUCH_FIELD, "<root>.description"},
            }
        }, { // NullChecked
            {42,  "", ""},
            {
                // construct
                {"object(simdjson::simdjson_result<simdjson::ondemand::object>*,bool*)", "start"},
                {"value(ValuePtr,GetFn,bool*)", "start"},

                // evaluate
                {"value(ValuePtr,GetFn,bool*)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "object(simdjson::simdjson_result<simdjson::ondemand::object>*,bool*)", "start:get_value"},
                {    "object(simdjson::simdjson_result<simdjson::ondemand::object>*,bool*)", "type:other"},
                {"value(ValuePtr,GetFn,bool*)", "eval_end"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:description"},
                {"error::path_scope::~path_scope()", "delete"}
            }, { // errors
                {simdjson::NO_SUCH_FIELD, "<root>.name"},
                {simdjson::NO_SUCH_FIELD, "<root>.description"},
            }
        }, { // FromMember
            {42,  "", ""},
            {
                // construct
                {"object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "start:get_value"},
                {            "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "type:other"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {"error::path_scope::~path_scope()", "delete"},
                {"error::path_scope::path_scope(error*,std::string_view)", "add_member:description"},
                {"error::path_scope::~path_scope()", "delete"},
            }, { // errors
                {simdjson::NO_SUCH_FIELD, "<root>.name"},
                {simdjson::NO_SUCH_FIELD, "<root>.description"},
            }
        }
    }, {
        {
            "null", R"(null)", true
        }, { // Plain
            {0, "", ""},
            {
                // construct
                {"object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},

                // evaluate
                {"value(ValuePtr,GetFn)", "eval_begin"},
                {    "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "start:get_value"},
                {    "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "type:other"},
                {    "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "error:16"},
                {"value(ValuePtr,GetFn)", "eval_end"},
            }, { // errors
                {simdjson::INCORRECT_TYPE, "<root>"}
            }
        }, { // NullChecked
            {0, "", ""},
            {
                // construct
                {"object(simdjson::simdjson_result<simdjson::ondemand::object>*,bool*)", "start"},
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
                {"object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "start:get_value"},
                {            "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "type:other"},
                {            "object(simdjson::simdjson_result<simdjson::ondemand::object>*)", "error:16"},
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

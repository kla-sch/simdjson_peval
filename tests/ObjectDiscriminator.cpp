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

#include "ObjectDiscriminator.h"


TEST_P(ObjectDiscriminator, ValueFromResult) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataDiscriminator result;

    auto is_typeA =
        [&result] (auto *object, auto *err) {
            return (result.type == "typeA");
        };

    auto is_typeB =
        [&result] (auto *object, auto *err) {
            return (result.type == "typeB");
        };

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("id", number_value(&result.id)),
            member("name", string_value(&result.name)),
            member("type", string_value(&result.type)),
            discriminator(
                is_typeA,
                member("typeA_val1",
                       number_value(&result.typeA_val1)),
                member("typeA_val2",
                       string_value(&result.typeA_val2))
                ),
            discriminator(
                is_typeB,
                member("typeB_val1",
                       number_value(&result.typeB_val1)),
                member("typeB_val2",
                       string_value(&result.typeB_val2))
                )
            );
    eval_fn(dataDoc, &errors);
    objectCheckResult(
        param, param.dataValueFromResult, result, nullptr, errors);
}


TEST_P(ObjectDiscriminator, ValueFromObject) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataDiscriminator result;

    auto is_typeA =
        [] (auto *member, auto *err) {
            std::string_view memberType;
            auto code = (*member)["type"].get(memberType);
            if (code) {
                error::path_scope scope(err, "type");
                err->add(code);
                return false;
            }
            else {
                return (memberType == "typeA");
            }
        };

    auto is_typeB =
        [] (auto *member, auto *err) {
            std::string_view memberType;
            auto code = (*member)["type"].get(memberType);
            if (code) {
                error::path_scope scope(err, "type");
                err->add(code);
                return false;
            }
            else {
                return (memberType == "typeB");
            }
        };

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("id", number_value(&result.id)),
            member("name", string_value(&result.name)),
            discriminator(
                is_typeA,
                member("typeA_val1",
                       number_value(&result.typeA_val1)),
                member("typeA_val2",
                       string_value(&result.typeA_val2))
                ),
            discriminator(
                is_typeB,
                member("typeB_val1",
                       number_value(&result.typeB_val1)),
                member("typeB_val2",
                       string_value(&result.typeB_val2))
                )
            );
    eval_fn(dataDoc, &errors);
    objectCheckResult(
        param, param.dataValueFromObject, result, nullptr, errors);
}


// Parameters
////////////////////////////////////////////////////////////////////////


std::vector<ObjectDiscriminatorParam>
ObjectDiscriminatorParam::cases = {
    {
        {
            "typeAvalue", R"(
            {
                    "id": 42,
                    "name": "name value",
                    "type": "typeA",
                    "typeA_val1": 11,
                    "typeA_val2": "type A value 2"
            })", false
        },
        { // ValueFromResult
            {
                42, "name value", "typeA",
                11, "type A value 2",
                std::nullopt, std::nullopt
            },
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#else
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#endif

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "string_value(std::string_view*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {        "member(std::string_view,ValueFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {            "object(MemberFn,MoreMemberFn...)", "type:object"},
                {            "member(std::string_view,ValueFn)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:type"},
                {                "value(ValuePtr,GetFn)", "eval_begin"},
                {                    "string_value(std::string_view*)", "start:get_value"},
                {                "value(ValuePtr,GetFn)", "eval_end"},
                {            "member(std::string_view,ValueFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                "object(MemberFn,MoreMemberFn...)", "type:object"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "use_members"},
                {                    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {                        "member(std::string_view,ValueFn)", "eval_begin"},
                {                            "error::path_scope::path_scope(error*,std::string_view)", "add_member:typeA_val1"},
                {                            "value(std::optional<Value>*,GetFn)", "eval_begin"},
                {                                "check_null(SJValue,Optional)", "start"},
                {                                "check_null(SJValue,Optional)", "is_null:false"},
                {                                "number_value(std::optional<int64_t>*)", "start:get_value"},
                {                            "value(std::optional<Value>*,GetFn)", "eval_end"},
                {                        "member(std::string_view,ValueFn)", "eval_end"},
                {                        "error::path_scope::~path_scope()", "delete"},
                {                        "object(MemberFn)", "eval_begin"},
                {                            "object(MemberFn)", "type:object"},
                {                            "member(std::string_view,ValueFn)", "eval_begin"},
                {                                "error::path_scope::path_scope(error*,std::string_view)", "add_member:typeA_val2"},
                {                                "value(std::optional<Value>*,GetFn)", "eval_begin"},
                {                                    "check_null(SJValue,Optional)", "start"},
                {                                    "check_null(SJValue,Optional)", "is_null:false"},
                {                                    "string_value(std::optional<std::string_view>*)", "start:get_value"},
                {                                "value(std::optional<Value>*,GetFn)", "eval_end"},
                {                            "member(std::string_view,ValueFn)", "eval_end"},
                {                            "error::path_scope::~path_scope()", "delete"},
                {                        "object(MemberFn)", "eval_end"},
                {                    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {                "object(MemberFn)", "eval_begin"},
                {                    "object(MemberFn)", "type:object"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                        "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "skip_members"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {                "object(MemberFn)", "eval_end"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},

            },
            { // no erors
            }
        }, { // ValueFromObject
            {
                42, "name value", "",
                11, "type A value 2",
                std::nullopt, std::nullopt
            },
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#else
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#endif

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "string_value(std::string_view*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {        "member(std::string_view,ValueFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {            "object(MemberFn,MoreMemberFn...)", "type:object"},
                {            "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "use_members"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                    "object(MemberFn,MoreMemberFn...)", "type:object"},
                {                    "member(std::string_view,ValueFn)", "eval_begin"},
                {                        "error::path_scope::path_scope(error*,std::string_view)", "add_member:typeA_val1"},
                {                        "value(std::optional<Value>*,GetFn)", "eval_begin"},
                {                            "check_null(SJValue,Optional)", "start"},
                {                            "check_null(SJValue,Optional)", "is_null:false"},
                {                            "number_value(std::optional<int64_t>*)", "start:get_value"},
                {                        "value(std::optional<Value>*,GetFn)", "eval_end"},
                {                    "member(std::string_view,ValueFn)", "eval_end"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_begin"},
                {                        "object(MemberFn)", "type:object"},
                {                        "member(std::string_view,ValueFn)", "eval_begin"},
                {                            "error::path_scope::path_scope(error*,std::string_view)", "add_member:typeA_val2"},
                {                            "value(std::optional<Value>*,GetFn)", "eval_begin"},
                {                                "check_null(SJValue,Optional)", "start"},
                {                                "check_null(SJValue,Optional)", "is_null:false"},
                {                                "string_value(std::optional<std::string_view>*)", "start:get_value"},
                {                            "value(std::optional<Value>*,GetFn)", "eval_end"},
                {                        "member(std::string_view,ValueFn)", "eval_end"},
                {                        "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_end"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {            "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {            "object(MemberFn)", "eval_begin"},
                {                "object(MemberFn)", "type:object"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "skip_members"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {            "object(MemberFn)", "eval_end"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},

            },
            { // no erors
            }
        }
    }, {
        {
            "typeBvalue", R"(
            {
                    "id": 42,
                    "name": "name value",
                    "type": "typeB",
                    "typeB_val1": 21,
                    "typeB_val2": "type B value 2"
            })", false
        },
        { // ValueFromResult
            {
                42, "name value", "typeB",
                std::nullopt, std::nullopt,
                21, "type B value 2"
            },
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#else
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#endif

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "string_value(std::string_view*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {        "member(std::string_view,ValueFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {            "object(MemberFn,MoreMemberFn...)", "type:object"},
                {            "member(std::string_view,ValueFn)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:type"},
                {                "value(ValuePtr,GetFn)", "eval_begin"},
                {                    "string_value(std::string_view*)", "start:get_value"},
                {                "value(ValuePtr,GetFn)", "eval_end"},
                {            "member(std::string_view,ValueFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                "object(MemberFn,MoreMemberFn...)", "type:object"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "skip_members"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {                "object(MemberFn)", "eval_begin"},
                {                    "object(MemberFn)", "type:object"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                        "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "use_members"},
                {                        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                            "object(MemberFn,MoreMemberFn...)", "type:object"},
                {                            "member(std::string_view,ValueFn)", "eval_begin"},
                {                                "error::path_scope::path_scope(error*,std::string_view)", "add_member:typeB_val1"},
                {                                "value(std::optional<Value>*,GetFn)", "eval_begin"},
                {                                    "check_null(SJValue,Optional)", "start"},
                {                                    "check_null(SJValue,Optional)", "is_null:false"},
                {                                    "number_value(std::optional<int64_t>*)", "start:get_value"},
                {                                "value(std::optional<Value>*,GetFn)", "eval_end"},
                {                            "member(std::string_view,ValueFn)", "eval_end"},
                {                            "error::path_scope::~path_scope()", "delete"},
                {                            "object(MemberFn)", "eval_begin"},
                {                                "object(MemberFn)", "type:object"},
                {                                "member(std::string_view,ValueFn)", "eval_begin"},
                {                                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:typeB_val2"},
                {                                    "value(std::optional<Value>*,GetFn)", "eval_begin"},
                {                                        "check_null(SJValue,Optional)", "start"},
                {                                        "check_null(SJValue,Optional)", "is_null:false"},
                {                                        "string_value(std::optional<std::string_view>*)", "start:get_value"},
                {                                    "value(std::optional<Value>*,GetFn)", "eval_end"},
                {                                "member(std::string_view,ValueFn)", "eval_end"},
                {                                "error::path_scope::~path_scope()", "delete"},
                {                            "object(MemberFn)", "eval_end"},
                {                        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {                "object(MemberFn)", "eval_end"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},

            },
            { // no erors
            }
        }, { // ValueFromObject
            {
                42, "name value", "",
                std::nullopt, std::nullopt,
                21, "type B value 2"
            },
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#else
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#endif

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "string_value(std::string_view*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {        "member(std::string_view,ValueFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {            "object(MemberFn,MoreMemberFn...)", "type:object"},
                {            "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "skip_members"},
                {            "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {            "object(MemberFn)", "eval_begin"},
                {                "object(MemberFn)", "type:object"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "use_members"},
                {                    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {                        "member(std::string_view,ValueFn)", "eval_begin"},
                {                            "error::path_scope::path_scope(error*,std::string_view)", "add_member:typeB_val1"},
                {                            "value(std::optional<Value>*,GetFn)", "eval_begin"},
                {                                "check_null(SJValue,Optional)", "start"},
                {                                "check_null(SJValue,Optional)", "is_null:false"},
                {                                "number_value(std::optional<int64_t>*)", "start:get_value"},
                {                            "value(std::optional<Value>*,GetFn)", "eval_end"},
                {                        "member(std::string_view,ValueFn)", "eval_end"},
                {                        "error::path_scope::~path_scope()", "delete"},
                {                        "object(MemberFn)", "eval_begin"},
                {                            "object(MemberFn)", "type:object"},
                {                            "member(std::string_view,ValueFn)", "eval_begin"},
                {                                "error::path_scope::path_scope(error*,std::string_view)", "add_member:typeB_val2"},
                {                                "value(std::optional<Value>*,GetFn)", "eval_begin"},
                {                                    "check_null(SJValue,Optional)", "start"},
                {                                    "check_null(SJValue,Optional)", "is_null:false"},
                {                                    "string_value(std::optional<std::string_view>*)", "start:get_value"},
                {                                "value(std::optional<Value>*,GetFn)", "eval_end"},
                {                            "member(std::string_view,ValueFn)", "eval_end"},
                {                            "error::path_scope::~path_scope()", "delete"},
                {                        "object(MemberFn)", "eval_end"},
                {                    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {            "object(MemberFn)", "eval_end"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},

            },
            { // no erors
            }
        }
    }, {
        {
            "unknownTypeValue", R"(
            {
                    "id": 42,
                    "name": "name value",
                    "type": "typeX",
                    "typeA_val1": 11,
                    "typeA_val2": "type A value 2",
                    "typeB_val1": 21,
                    "typeB_val2": "type B value 2"
            })", false
        },
        { // ValueFromResult
            {
                42, "name value", "typeX",
                std::nullopt, std::nullopt,
                std::nullopt, std::nullopt
            },
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#else
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#endif

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "string_value(std::string_view*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {        "member(std::string_view,ValueFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {            "object(MemberFn,MoreMemberFn...)", "type:object"},
                {            "member(std::string_view,ValueFn)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:type"},
                {                "value(ValuePtr,GetFn)", "eval_begin"},
                {                    "string_value(std::string_view*)", "start:get_value"},
                {                "value(ValuePtr,GetFn)", "eval_end"},
                {            "member(std::string_view,ValueFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                "object(MemberFn,MoreMemberFn...)", "type:object"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "skip_members"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {                "object(MemberFn)", "eval_begin"},
                {                    "object(MemberFn)", "type:object"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                        "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "skip_members"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {                "object(MemberFn)", "eval_end"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},

            },
            { // no erors
            }
        }, { // ValueFromObject
            {
                42, "name value", "",
                std::nullopt, std::nullopt,
                std::nullopt, std::nullopt
            },
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#else
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#endif

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "string_value(std::string_view*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {        "member(std::string_view,ValueFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {            "object(MemberFn,MoreMemberFn...)", "type:object"},
                {            "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "skip_members"},
                {            "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {            "object(MemberFn)", "eval_begin"},
                {                "object(MemberFn)", "type:object"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "skip_members"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {            "object(MemberFn)", "eval_end"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},

            },
            { // no erors
            }
        }
    }, {
        {
            "discriminatorMissing", R"(
            {
                    "id": 42,
                    "name": "name value",
                    "typeB_val1": 21,
                    "typeB_val2": "type B value 2"
            })", false
        },
        { // ValueFromResult
            { /* no result */ },
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#else
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#endif

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "string_value(std::string_view*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {        "member(std::string_view,ValueFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {            "object(MemberFn,MoreMemberFn...)", "type:object"},
                {            "member(std::string_view,ValueFn)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:type"},
                {                "member(std::string_view,ValueFn)", "error:19"},
                {            "member(std::string_view,ValueFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                "object(MemberFn,MoreMemberFn...)", "type:object"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "skip_members"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {                "object(MemberFn)", "eval_begin"},
                {                    "object(MemberFn)", "type:object"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                        "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "skip_members"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {                "object(MemberFn)", "eval_end"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},

            },
            {
                {simdjson::NO_SUCH_FIELD , "<root>.type"}
            }
        }, { // ValueFromObject
            { /* no result */ },
            {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#else
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::optional<std::string_view>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(std::optional<int64_t>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"string_value(std::string_view*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"number_value(uint64_t*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
#endif

                // evaluate
                {"object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {        "value(ValuePtr,GetFn)", "eval_begin"},
                {            "number_value(uint64_t*)", "start:get_value"},
                {        "value(ValuePtr,GetFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {        "object(MemberFn,MoreMemberFn...)", "type:object"},
                {        "member(std::string_view,ValueFn)", "eval_begin"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {            "value(ValuePtr,GetFn)", "eval_begin"},
                {                "string_value(std::string_view*)", "start:get_value"},
                {            "value(ValuePtr,GetFn)", "eval_end"},
                {        "member(std::string_view,ValueFn)", "eval_end"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {            "object(MemberFn,MoreMemberFn...)", "type:object"},
                {            "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:type"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "skip_members"},
                {            "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {            "object(MemberFn)", "eval_begin"},
                {                "object(MemberFn)", "type:object"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:type"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                    "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "skip_members"},
                {                "discriminator(UseMembersFn,MemberFn,MoreMemberFn)", "eval_end"},
                {            "object(MemberFn)", "eval_end"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {"object(MemberFn,MoreMemberFn...)", "eval_end"},

            },
            {
                {simdjson::NO_SUCH_FIELD , "<root>.type"},
                {simdjson::NO_SUCH_FIELD , "<root>.type"}
            }
        }
    }
};

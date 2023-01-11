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

#include "ObjectTo.h"


// Map
// ///////////////////////////////////////////////////////////////////////////


TEST_P(ObjectToMap, Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataObjectToMap result;
    std::pair<std::string_view, TestDataObjectTo> tmp_elem;

    auto eval_fn =
        object_to_out_iter<simdjson::ondemand::document>(
            inserter(result, result.end()), &tmp_elem,
            object(
                member("id", number_value(&tmp_elem.second.id)),
                member("name", string_value(&tmp_elem.second.name))));

    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataPlain, result, nullptr, errors);
}

TEST_P(ObjectToMap, FromMember) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto newInput = "{\"result\":" + std::string(param.input) + '}';
    auto json = simdjson::padded_string(newInput);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataObjectToMap result;
    std::pair<std::string_view, TestDataObjectTo> tmp_elem;

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("result",
                   object_to_out_iter(
                       inserter(result, result.end()), &tmp_elem,
                       object(
                           member("id", number_value(&tmp_elem.second.id)),
                           member("name", string_value(&tmp_elem.second.name))
                           )
                       )
                )
            );

    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataFromMember, result, nullptr, errors);
}


TEST_P(ObjectToMap, CheckNull) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataObjectToMap result;
    bool is_null;

    std::pair<std::string_view, TestDataObjectTo> tmp_elem;

    auto eval_fn =
        object_to_out_iter<simdjson::ondemand::document>(
            &is_null,
            inserter(result, result.end()), &tmp_elem,
            object(
                member("id", number_value(&tmp_elem.second.id)),
                member("name", string_value(&tmp_elem.second.name))));

    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataCheckNull, result, &is_null, errors);
}

// Vector
// ///////////////////////////////////////////////////////////////////////////

TEST_P(ObjectToVector, Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataObjectToVector result;
    std::pair<std::string_view, TestDataObjectTo> tmp_elem;

    auto eval_fn =
        object_to_out_iter<simdjson::ondemand::document>(
            inserter(result, result.end()), &tmp_elem,
            object(
                member("id", number_value(&tmp_elem.second.id)),
                member("name", string_value(&tmp_elem.second.name))));

    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataPlain, result, nullptr, errors);
}

TEST_P(ObjectToVector, FromMember) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto newInput = "{\"result\":" + std::string(param.input) + '}';
    auto json = simdjson::padded_string(newInput);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataObjectToVector result;
    std::pair<std::string_view, TestDataObjectTo> tmp_elem;

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("result",
                   object_to_out_iter(
                       inserter(result, result.end()), &tmp_elem,
                       object(
                           member("id", number_value(&tmp_elem.second.id)),
                           member("name", string_value(&tmp_elem.second.name))
                           )
                       )
                )
            );

    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataFromMember, result, nullptr, errors);
}


TEST_P(ObjectToVector, CheckNull) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataObjectToVector result;
    bool is_null;

    std::pair<std::string_view, TestDataObjectTo> tmp_elem;

    auto eval_fn =
        object_to_out_iter<simdjson::ondemand::document>(
            &is_null,
            inserter(result, result.end()), &tmp_elem,
            object(
                member("id", number_value(&tmp_elem.second.id)),
                member("name", string_value(&tmp_elem.second.name))));

    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataCheckNull, result, &is_null, errors);
}

// Function
// ///////////////////////////////////////////////////////////////////////////

TEST_P(ObjectToFunction, Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataObjectToFunction result;
    std::pair<std::string_view, TestDataObjectTo> tmp_elem;

    auto get_member =
        [&result](auto *tmp_pair, auto *err) {
            if (!*err) {
                result.insert(std::move(*tmp_pair));
            }
        };

    auto eval_fn =
        object_to_function<simdjson::ondemand::document>(
            get_member, &tmp_elem,
            object(
                member("id", number_value(&tmp_elem.second.id)),
                member("name", string_value(&tmp_elem.second.name))));

    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataPlain, result, nullptr, errors);
}

TEST_P(ObjectToFunction, FromMember) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto newInput = "{\"result\":" + std::string(param.input) + '}';
    auto json = simdjson::padded_string(newInput);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataObjectToFunction result;
    std::pair<std::string_view, TestDataObjectTo> tmp_elem;

    auto get_member =
        [&result](auto *tmp_pair, auto *err) {
            if (!*err) {
                result.insert(std::move(*tmp_pair));
            }
        };

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("result",
                   object_to_function(
                       get_member, &tmp_elem,
                       object(
                           member("id", number_value(&tmp_elem.second.id)),
                           member("name", string_value(&tmp_elem.second.name))
                           )
                       )
                )
            );

    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataFromMember, result, nullptr, errors);
}


TEST_P(ObjectToFunction, CheckNull) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataObjectToFunction result;
    bool is_null;
    std::pair<std::string_view, TestDataObjectTo> tmp_elem;

    auto get_member =
        [&result](auto *tmp_pair, auto *err) {
            if (!*err) {
                result.insert(std::move(*tmp_pair));
            }
        };

    auto eval_fn =
        object_to_function<simdjson::ondemand::document>(
            &is_null,
            get_member, &tmp_elem,
            object(
                member("id", number_value(&tmp_elem.second.id)),
                member("name", string_value(&tmp_elem.second.name))));

    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataCheckNull, result, &is_null, errors);
}

// Generic Function
// ///////////////////////////////////////////////////////////////////////////

// store value into `*result`
template<typename SJResult>
void
store_value(
    TestDataObjectToGenericFn *result,
    std::string_view member_name,
    SJResult *sj_result,
    simdjson_peval::error *err)
{
    if (*err) {
        return;
    }

    simdjson::ondemand::object sj_object;
    auto code = sj_result->get_object().get(sj_object);
    if (code) {
        err->add(code);
        return;
    }

    uint64_t id;
    code = sj_object["id"].get(id);
    if (code) {
        err->add(code);
        return;
    }

    std::string_view name;
    code = sj_object["name"].get(name);
    if (code) {
        err->add(code);
        return;
    }

    result->emplace(member_name, TestDataObjectTo{id, name});
}


TEST_P(ObjectToGenericFn, Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataObjectToGenericFn result;

    auto get_member =
        [&result](auto member_name, auto *sj_result, auto *err) {
            store_value(&result, member_name, sj_result, err);
        };

    auto eval_fn =
        object_to_function<simdjson::ondemand::document>(get_member);

    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataPlain, result, nullptr, errors);
}

TEST_P(ObjectToGenericFn, FromMember) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto newInput = "{\"result\":" + std::string(param.input) + '}';
    auto json = simdjson::padded_string(newInput);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataObjectToGenericFn result;

    auto get_member =
        [&result](auto member_name, auto *sj_result, auto *err) {
            store_value(&result, member_name, sj_result, err);
        };

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("result",
                   object_to_function(get_member)));

    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataFromMember, result, nullptr, errors);
}


TEST_P(ObjectToGenericFn, CheckNull) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataObjectToGenericFn result;
    bool is_null;

    std::pair<std::string_view, TestDataObjectTo> tmp_elem;

    auto get_member =
        [&result]
        (auto member_name, auto *sj_result, auto *err) {
            store_value(&result, member_name, sj_result, err);
        };

    auto eval_fn =
        object_to_function<simdjson::ondemand::document>(
            &is_null,
            get_member);

    eval_fn(dataDoc, &errors);

    objectCheckResult(
        param, param.dataCheckNull, result, &is_null, errors);
}


// Parameters
////////////////////////////////////////////////////////////////////////

std::vector<ObjectToVectorParam>
ObjectToVectorParam::cases; // filled by copyObjectToVectorCases


std::vector<ObjectToMapParam>
ObjectToMapParam::cases = {
    {
        {
            "threeValues", R"(
            {
                "test3": {
                    "id": 2,
                    "name": "Text 3"
                },
                "test2": {
                    "id": 1,
                    "name": "Text 2"
                },
                "test1": {
                    "id": 0,
                    "name": "Text 1"
                }
            })", false
        }, { // Plain
            { // result
                {"test1", { 0, "Text 1" } },
                {"test2", { 1, "Text 2" } },
                {"test3", { 2, "Text 3" } }
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {        "object_to_function(OutFn)", "eval_begin"},
                {            "object_to_function(OutFn)", "type:other"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:test3"},
                {            "object_to_function(OutFn)", "get:test3"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                "member(std::string_view,ValueFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                    "value(ValuePtr,GetFn)", "eval_begin"},
                {                        "number_value(uint64_t*)", "start:get_value"},
                {                    "value(ValuePtr,GetFn)", "eval_end"},
                {                "member(std::string_view,ValueFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "object(MemberFn)", "eval_begin"},
                {                    "object(MemberFn)", "type:object"},
                {                    "member(std::string_view,ValueFn)", "eval_begin"},
                {                        "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                        "value(ValuePtr,GetFn)", "eval_begin"},
                {                            "string_value(std::string_view*)", "start:get_value"},
                {                        "value(ValuePtr,GetFn)", "eval_end"},
                {                    "member(std::string_view,ValueFn)", "eval_end"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                "object(MemberFn)", "eval_end"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:test2"},
                {            "object_to_function(OutFn)", "get:test2"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                "member(std::string_view,ValueFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                    "value(ValuePtr,GetFn)", "eval_begin"},
                {                        "number_value(uint64_t*)", "start:get_value"},
                {                    "value(ValuePtr,GetFn)", "eval_end"},
                {                "member(std::string_view,ValueFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "object(MemberFn)", "eval_begin"},
                {                    "object(MemberFn)", "type:object"},
                {                    "member(std::string_view,ValueFn)", "eval_begin"},
                {                        "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                        "value(ValuePtr,GetFn)", "eval_begin"},
                {                            "string_value(std::string_view*)", "start:get_value"},
                {                        "value(ValuePtr,GetFn)", "eval_end"},
                {                    "member(std::string_view,ValueFn)", "eval_end"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                "object(MemberFn)", "eval_end"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:test1"},
                {            "object_to_function(OutFn)", "get:test1"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                "member(std::string_view,ValueFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                    "value(ValuePtr,GetFn)", "eval_begin"},
                {                        "number_value(uint64_t*)", "start:get_value"},
                {                    "value(ValuePtr,GetFn)", "eval_end"},
                {                "member(std::string_view,ValueFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "object(MemberFn)", "eval_begin"},
                {                    "object(MemberFn)", "type:object"},
                {                    "member(std::string_view,ValueFn)", "eval_begin"},
                {                        "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                        "value(ValuePtr,GetFn)", "eval_begin"},
                {                            "string_value(std::string_view*)", "start:get_value"},
                {                        "value(ValuePtr,GetFn)", "eval_end"},
                {                    "member(std::string_view,ValueFn)", "eval_end"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                "object(MemberFn)", "eval_end"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {            "error::path_scope::~path_scope()", "delete"},
                {        "object_to_function(OutFn)", "eval_end"},
                {    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"}
            }, {
                // no errors
            }
        }, { // FromMember
            { // result
                {"test1", { 0, "Text 1" } },
                {"test2", { 1, "Text 2" } },
                {"test3", { 2, "Text 3" } }
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {                "object_to_function(OutFn)", "eval_begin"},
                {                    "object_to_function(OutFn)", "type:other"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:test3"},
                {                    "object_to_function(OutFn)", "get:test3"},
                {                    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {                    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                        "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                        "member(std::string_view,ValueFn)", "eval_begin"},
                {                            "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                            "value(ValuePtr,GetFn)", "eval_begin"},
                {                                "number_value(uint64_t*)", "start:get_value"},
                {                            "value(ValuePtr,GetFn)", "eval_end"},
                {                        "member(std::string_view,ValueFn)", "eval_end"},
                {                        "error::path_scope::~path_scope()", "delete"},
                {                        "object(MemberFn)", "eval_begin"},
                {                            "object(MemberFn)", "type:object"},
                {                            "member(std::string_view,ValueFn)", "eval_begin"},
                {                                "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                                "value(ValuePtr,GetFn)", "eval_begin"},
                {                                    "string_value(std::string_view*)", "start:get_value"},
                {                                "value(ValuePtr,GetFn)", "eval_end"},
                {                            "member(std::string_view,ValueFn)", "eval_end"},
                {                            "error::path_scope::~path_scope()", "delete"},
                {                        "object(MemberFn)", "eval_end"},
                {                    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {                    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:test2"},
                {                    "object_to_function(OutFn)", "get:test2"},
                {                    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {                    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                        "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                        "member(std::string_view,ValueFn)", "eval_begin"},
                {                            "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                            "value(ValuePtr,GetFn)", "eval_begin"},
                {                                "number_value(uint64_t*)", "start:get_value"},
                {                            "value(ValuePtr,GetFn)", "eval_end"},
                {                        "member(std::string_view,ValueFn)", "eval_end"},
                {                        "error::path_scope::~path_scope()", "delete"},
                {                        "object(MemberFn)", "eval_begin"},
                {                            "object(MemberFn)", "type:object"},
                {                            "member(std::string_view,ValueFn)", "eval_begin"},
                {                                "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                                "value(ValuePtr,GetFn)", "eval_begin"},
                {                                    "string_value(std::string_view*)", "start:get_value"},
                {                                "value(ValuePtr,GetFn)", "eval_end"},
                {                            "member(std::string_view,ValueFn)", "eval_end"},
                {                            "error::path_scope::~path_scope()", "delete"},
                {                        "object(MemberFn)", "eval_end"},
                {                    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {                    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:test1"},
                {                    "object_to_function(OutFn)", "get:test1"},
                {                    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {                    "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                        "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                        "member(std::string_view,ValueFn)", "eval_begin"},
                {                            "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                            "value(ValuePtr,GetFn)", "eval_begin"},
                {                                "number_value(uint64_t*)", "start:get_value"},
                {                            "value(ValuePtr,GetFn)", "eval_end"},
                {                        "member(std::string_view,ValueFn)", "eval_end"},
                {                        "error::path_scope::~path_scope()", "delete"},
                {                        "object(MemberFn)", "eval_begin"},
                {                            "object(MemberFn)", "type:object"},
                {                            "member(std::string_view,ValueFn)", "eval_begin"},
                {                                "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                                "value(ValuePtr,GetFn)", "eval_begin"},
                {                                    "string_value(std::string_view*)", "start:get_value"},
                {                                "value(ValuePtr,GetFn)", "eval_end"},
                {                            "member(std::string_view,ValueFn)", "eval_end"},
                {                            "error::path_scope::~path_scope()", "delete"},
                {                        "object(MemberFn)", "eval_end"},
                {                    "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {                    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                "object_to_function(OutFn)", "eval_end"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {        "object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"}
            }, { // errors
            }
        }, { // CheckNull
            { // result
                {"test1", { 0, "Text 1" } },
                {"test2", { 1, "Text 2" } },
                {"test3", { 2, "Text 3" } }
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_out_iter(bool*,OutIter,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_out_iter(bool*,OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {            "object_to_function(OutFn)", "eval_begin"},
                {                "object_to_function(OutFn)", "type:other"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:test3"},
                {                "object_to_function(OutFn)", "get:test3"},
                {                "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                    "member(std::string_view,ValueFn)", "eval_begin"},
                {                        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                        "value(ValuePtr,GetFn)", "eval_begin"},
                {                            "number_value(uint64_t*)", "start:get_value"},
                {                        "value(ValuePtr,GetFn)", "eval_end"},
                {                    "member(std::string_view,ValueFn)", "eval_end"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_begin"},
                {                        "object(MemberFn)", "type:object"},
                {                        "member(std::string_view,ValueFn)", "eval_begin"},
                {                            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                            "value(ValuePtr,GetFn)", "eval_begin"},
                {                                "string_value(std::string_view*)", "start:get_value"},
                {                            "value(ValuePtr,GetFn)", "eval_end"},
                {                        "member(std::string_view,ValueFn)", "eval_end"},
                {                        "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_end"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {                "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:test2"},
                {                "object_to_function(OutFn)", "get:test2"},
                {                "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                    "member(std::string_view,ValueFn)", "eval_begin"},
                {                        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                        "value(ValuePtr,GetFn)", "eval_begin"},
                {                            "number_value(uint64_t*)", "start:get_value"},
                {                        "value(ValuePtr,GetFn)", "eval_end"},
                {                    "member(std::string_view,ValueFn)", "eval_end"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_begin"},
                {                        "object(MemberFn)", "type:object"},
                {                        "member(std::string_view,ValueFn)", "eval_begin"},
                {                            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                            "value(ValuePtr,GetFn)", "eval_begin"},
                {                                "string_value(std::string_view*)", "start:get_value"},
                {                            "value(ValuePtr,GetFn)", "eval_end"},
                {                        "member(std::string_view,ValueFn)", "eval_end"},
                {                        "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_end"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {                "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:test1"},
                {                "object_to_function(OutFn)", "get:test1"},
                {                "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                    "member(std::string_view,ValueFn)", "eval_begin"},
                {                        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                        "value(ValuePtr,GetFn)", "eval_begin"},
                {                            "number_value(uint64_t*)", "start:get_value"},
                {                        "value(ValuePtr,GetFn)", "eval_end"},
                {                    "member(std::string_view,ValueFn)", "eval_end"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_begin"},
                {                        "object(MemberFn)", "type:object"},
                {                        "member(std::string_view,ValueFn)", "eval_begin"},
                {                            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                            "value(ValuePtr,GetFn)", "eval_begin"},
                {                                "string_value(std::string_view*)", "start:get_value"},
                {                            "value(ValuePtr,GetFn)", "eval_end"},
                {                        "member(std::string_view,ValueFn)", "eval_end"},
                {                        "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_end"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {                "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {                "error::path_scope::~path_scope()", "delete"},
                {            "object_to_function(OutFn)", "eval_end"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {    "object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {"object_to_out_iter(bool*,OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"}

            }, { // errors
            }
        }
    }, {
        {
            "empty", R"({})", false
        }, { // Plain
            {
                // result empty
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {        "object_to_function(OutFn)", "eval_begin"},
                {            "object_to_function(OutFn)", "type:other"},
                {        "object_to_function(OutFn)", "eval_end"},
                {    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"}

            }, {
                // no errors
            }
        }, { // FromMember
            {
                // result empty
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {                "object_to_function(OutFn)", "eval_begin"},
                {                    "object_to_function(OutFn)", "type:other"},
                {                "object_to_function(OutFn)", "eval_end"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {        "object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"},

            }, { // errors
            }
        }, { // CheckNull
            {
                // result empty
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_out_iter(bool*,OutIter,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_out_iter(bool*,OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {            "object_to_function(OutFn)", "eval_begin"},
                {                "object_to_function(OutFn)", "type:other"},
                {            "object_to_function(OutFn)", "eval_end"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {    "object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {"object_to_out_iter(bool*,OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},

            }, { // errors
            }
        }
    }, {
        {
            "null", R"(null)", true
        }, { // Plain
            {
                // result empty
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {        "object_to_function(OutFn)", "eval_begin"},
                {            "object_to_function(OutFn)", "type:other"},
                {            "object_to_function(OutFn)", "error:16"},
                {        "object_to_function(OutFn)", "eval_end"},
                {    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"}

            }, { // errors
                {simdjson::INCORRECT_TYPE , "<root>"}
            }
        }, { // FromMember
            {
                // result empty
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {                "object_to_function(OutFn)", "eval_begin"},
                {                    "object_to_function(OutFn)", "type:other"},
                {                    "object_to_function(OutFn)", "error:16"},
                {                "object_to_function(OutFn)", "eval_end"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {        "object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"},

            }, { // errors
                {simdjson::INCORRECT_TYPE , "<root>.result"}
            }
        }, { // CheckNull
            {
                // result empty
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_out_iter(bool*,OutIter,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_out_iter(OutIter,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_out_iter(bool*,OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:true"},
                {    "object_to_out_iter(bool*,OutIter,std::pair<StringType,ValueType>*,MemberFn)", "is_null"},
                {"object_to_out_iter(bool*,OutIter,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"}
            }, { // errors
            }
        }
    }
};


std::vector<ObjectToFunctionParam>
ObjectToFunctionParam::cases = {
    {
        {
            "threeValues", R"(
            {
                "test3": {
                    "id": 2,
                    "name": "Text 3"
                },
                "test2": {
                    "id": 1,
                    "name": "Text 2"
                },
                "test1": {
                    "id": 0,
                    "name": "Text 1"
                }
            })", false
        }, { // Plain
            { // result
                {"test1", { 0, "Text 1" } },
                {"test2", { 1, "Text 2" } },
                {"test3", { 2, "Text 3" } }
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {    "object_to_function(OutFn)", "eval_begin"},
                {        "object_to_function(OutFn)", "type:other"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:test3"},
                {        "object_to_function(OutFn)", "get:test3"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {            "object(MemberFn,MoreMemberFn...)", "type:other"},
                {            "member(std::string_view,ValueFn)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                "value(ValuePtr,GetFn)", "eval_begin"},
                {                    "number_value(uint64_t*)", "start:get_value"},
                {                "value(ValuePtr,GetFn)", "eval_end"},
                {            "member(std::string_view,ValueFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn)", "eval_begin"},
                {                "object(MemberFn)", "type:object"},
                {                "member(std::string_view,ValueFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                    "value(ValuePtr,GetFn)", "eval_begin"},
                {                        "string_value(std::string_view*)", "start:get_value"},
                {                    "value(ValuePtr,GetFn)", "eval_end"},
                {                "member(std::string_view,ValueFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn)", "eval_end"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:test2"},
                {        "object_to_function(OutFn)", "get:test2"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {            "object(MemberFn,MoreMemberFn...)", "type:other"},
                {            "member(std::string_view,ValueFn)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                "value(ValuePtr,GetFn)", "eval_begin"},
                {                    "number_value(uint64_t*)", "start:get_value"},
                {                "value(ValuePtr,GetFn)", "eval_end"},
                {            "member(std::string_view,ValueFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn)", "eval_begin"},
                {                "object(MemberFn)", "type:object"},
                {                "member(std::string_view,ValueFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                    "value(ValuePtr,GetFn)", "eval_begin"},
                {                        "string_value(std::string_view*)", "start:get_value"},
                {                    "value(ValuePtr,GetFn)", "eval_end"},
                {                "member(std::string_view,ValueFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn)", "eval_end"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:test1"},
                {        "object_to_function(OutFn)", "get:test1"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {            "object(MemberFn,MoreMemberFn...)", "type:other"},
                {            "member(std::string_view,ValueFn)", "eval_begin"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                "value(ValuePtr,GetFn)", "eval_begin"},
                {                    "number_value(uint64_t*)", "start:get_value"},
                {                "value(ValuePtr,GetFn)", "eval_end"},
                {            "member(std::string_view,ValueFn)", "eval_end"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn)", "eval_begin"},
                {                "object(MemberFn)", "type:object"},
                {                "member(std::string_view,ValueFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                    "value(ValuePtr,GetFn)", "eval_begin"},
                {                        "string_value(std::string_view*)", "start:get_value"},
                {                    "value(ValuePtr,GetFn)", "eval_end"},
                {                "member(std::string_view,ValueFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {            "object(MemberFn)", "eval_end"},
                {        "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {        "error::path_scope::~path_scope()", "delete"},
                {    "object_to_function(OutFn)", "eval_end"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"}
            }, {
                // no errors
            }
        }, { // FromMember
            { // result
                {"test1", { 0, "Text 1" } },
                {"test2", { 1, "Text 2" } },
                {"test3", { 2, "Text 3" } }
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {            "object_to_function(OutFn)", "eval_begin"},
                {                "object_to_function(OutFn)", "type:other"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:test3"},
                {                "object_to_function(OutFn)", "get:test3"},
                {                "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                    "member(std::string_view,ValueFn)", "eval_begin"},
                {                        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                        "value(ValuePtr,GetFn)", "eval_begin"},
                {                            "number_value(uint64_t*)", "start:get_value"},
                {                        "value(ValuePtr,GetFn)", "eval_end"},
                {                    "member(std::string_view,ValueFn)", "eval_end"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_begin"},
                {                        "object(MemberFn)", "type:object"},
                {                        "member(std::string_view,ValueFn)", "eval_begin"},
                {                            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                            "value(ValuePtr,GetFn)", "eval_begin"},
                {                                "string_value(std::string_view*)", "start:get_value"},
                {                            "value(ValuePtr,GetFn)", "eval_end"},
                {                        "member(std::string_view,ValueFn)", "eval_end"},
                {                        "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_end"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {                "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:test2"},
                {                "object_to_function(OutFn)", "get:test2"},
                {                "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                    "member(std::string_view,ValueFn)", "eval_begin"},
                {                        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                        "value(ValuePtr,GetFn)", "eval_begin"},
                {                            "number_value(uint64_t*)", "start:get_value"},
                {                        "value(ValuePtr,GetFn)", "eval_end"},
                {                    "member(std::string_view,ValueFn)", "eval_end"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_begin"},
                {                        "object(MemberFn)", "type:object"},
                {                        "member(std::string_view,ValueFn)", "eval_begin"},
                {                            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                            "value(ValuePtr,GetFn)", "eval_begin"},
                {                                "string_value(std::string_view*)", "start:get_value"},
                {                            "value(ValuePtr,GetFn)", "eval_end"},
                {                        "member(std::string_view,ValueFn)", "eval_end"},
                {                        "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_end"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {                "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "error::path_scope::path_scope(error*,std::string_view)", "add_member:test1"},
                {                "object_to_function(OutFn)", "get:test1"},
                {                "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                    "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                    "member(std::string_view,ValueFn)", "eval_begin"},
                {                        "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                        "value(ValuePtr,GetFn)", "eval_begin"},
                {                            "number_value(uint64_t*)", "start:get_value"},
                {                        "value(ValuePtr,GetFn)", "eval_end"},
                {                    "member(std::string_view,ValueFn)", "eval_end"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_begin"},
                {                        "object(MemberFn)", "type:object"},
                {                        "member(std::string_view,ValueFn)", "eval_begin"},
                {                            "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                            "value(ValuePtr,GetFn)", "eval_begin"},
                {                                "string_value(std::string_view*)", "start:get_value"},
                {                            "value(ValuePtr,GetFn)", "eval_end"},
                {                        "member(std::string_view,ValueFn)", "eval_end"},
                {                        "error::path_scope::~path_scope()", "delete"},
                {                    "object(MemberFn)", "eval_end"},
                {                "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {                "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {                "error::path_scope::~path_scope()", "delete"},
                {            "object_to_function(OutFn)", "eval_end"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"}
            }, {
                // no errors
            }
        }, { // CheckNull
            { // result
                {"test1", { 0, "Text 1" } },
                {"test2", { 1, "Text 2" } },
                {"test3", { 2, "Text 3" } }
            }, {
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_function(bool*,OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_function(bool*,OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {        "object_to_function(OutFn)", "eval_begin"},
                {            "object_to_function(OutFn)", "type:other"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:test3"},
                {            "object_to_function(OutFn)", "get:test3"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                "member(std::string_view,ValueFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                    "value(ValuePtr,GetFn)", "eval_begin"},
                {                        "number_value(uint64_t*)", "start:get_value"},
                {                    "value(ValuePtr,GetFn)", "eval_end"},
                {                "member(std::string_view,ValueFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "object(MemberFn)", "eval_begin"},
                {                    "object(MemberFn)", "type:object"},
                {                    "member(std::string_view,ValueFn)", "eval_begin"},
                {                        "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                        "value(ValuePtr,GetFn)", "eval_begin"},
                {                            "string_value(std::string_view*)", "start:get_value"},
                {                        "value(ValuePtr,GetFn)", "eval_end"},
                {                    "member(std::string_view,ValueFn)", "eval_end"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                "object(MemberFn)", "eval_end"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:test2"},
                {            "object_to_function(OutFn)", "get:test2"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                "member(std::string_view,ValueFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                    "value(ValuePtr,GetFn)", "eval_begin"},
                {                        "number_value(uint64_t*)", "start:get_value"},
                {                    "value(ValuePtr,GetFn)", "eval_end"},
                {                "member(std::string_view,ValueFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "object(MemberFn)", "eval_begin"},
                {                    "object(MemberFn)", "type:object"},
                {                    "member(std::string_view,ValueFn)", "eval_begin"},
                {                        "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                        "value(ValuePtr,GetFn)", "eval_begin"},
                {                            "string_value(std::string_view*)", "start:get_value"},
                {                        "value(ValuePtr,GetFn)", "eval_end"},
                {                    "member(std::string_view,ValueFn)", "eval_end"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                "object(MemberFn)", "eval_end"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:test1"},
                {            "object_to_function(OutFn)", "get:test1"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start_gen_out_fn"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_begin"},
                {                "object(MemberFn,MoreMemberFn...)", "type:other"},
                {                "member(std::string_view,ValueFn)", "eval_begin"},
                {                    "error::path_scope::path_scope(error*,std::string_view)", "add_member:id"},
                {                    "value(ValuePtr,GetFn)", "eval_begin"},
                {                        "number_value(uint64_t*)", "start:get_value"},
                {                    "value(ValuePtr,GetFn)", "eval_end"},
                {                "member(std::string_view,ValueFn)", "eval_end"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "object(MemberFn)", "eval_begin"},
                {                    "object(MemberFn)", "type:object"},
                {                    "member(std::string_view,ValueFn)", "eval_begin"},
                {                        "error::path_scope::path_scope(error*,std::string_view)", "add_member:name"},
                {                        "value(ValuePtr,GetFn)", "eval_begin"},
                {                            "string_value(std::string_view*)", "start:get_value"},
                {                        "value(ValuePtr,GetFn)", "eval_end"},
                {                    "member(std::string_view,ValueFn)", "eval_end"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                "object(MemberFn)", "eval_end"},
                {            "object(MemberFn,MoreMemberFn...)", "eval_end"},
                {            "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "end_gen_out_fn"},
                {            "error::path_scope::~path_scope()", "delete"},
                {        "object_to_function(OutFn)", "eval_end"},
                {    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {"object_to_function(bool*,OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"}

            }, {
                // no errors
            }
        }
    }, {
        {
            "empty", R"({})", false
        }, { // Plain
            {
                // result empty
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {    "object_to_function(OutFn)", "eval_begin"},
                {        "object_to_function(OutFn)", "type:other"},
                {    "object_to_function(OutFn)", "eval_end"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
            }, {
                // no errors
            }
        }, { // FromMember
            {
                // result empty
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {            "object_to_function(OutFn)", "eval_begin"},
                {                "object_to_function(OutFn)", "type:other"},
                {            "object_to_function(OutFn)", "eval_end"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"},
            }, {
                // no errors
            }
        }, { // CheckNull
            {
                // result empty
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_function(bool*,OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_function(bool*,OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {        "object_to_function(OutFn)", "eval_begin"},
                {            "object_to_function(OutFn)", "type:other"},
                {        "object_to_function(OutFn)", "eval_end"},
                {    "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {"object_to_function(bool*,OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
            }, {
                // no errors
            }
        }
    }, {
        {
            "null", R"(null)", true
        }, { // Plain
            {
                // result empty
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {    "object_to_function(OutFn)", "eval_begin"},
                {        "object_to_function(OutFn)", "type:other"},
                {        "object_to_function(OutFn)", "error:16"},
                {    "object_to_function(OutFn)", "eval_end"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},

            }, { // errors
                {simdjson::INCORRECT_TYPE , "<root>"}
            }
        }, { // FromMember
            {
                // result empty
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {            "object_to_function(OutFn)", "eval_begin"},
                {                "object_to_function(OutFn)", "type:other"},
                {                "object_to_function(OutFn)", "error:16"},
                {            "object_to_function(OutFn)", "eval_end"},
                {        "object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"},
            }, { // errors
                {simdjson::INCORRECT_TYPE , "<root>.result"}
            }
        }, { // CheckNull
            {
                // result empty
            }, {
                // construct
#ifdef trace_param_order_right_to_left
                {"number_value(uint64_t*)", "start"},
#else
                {"string_value(std::string_view*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
#ifdef trace_param_order_right_to_left
                {"string_value(std::string_view*)", "start"},
#else
                {"number_value(uint64_t*)", "start"},
#endif
                {"value(ValuePtr,GetFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn,MoreMemberFn...)", "start"},
                {"object(MemberFn)", "start"},
                {"object_to_function(bool*,OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn,std::pair<StringType,ValueType>*,MemberFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_function(bool*,OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:true"},
                {    "object_to_function(bool*,OutFn,std::pair<StringType,ValueType>*,MemberFn)", "is_null"},
                {"object_to_function(bool*,OutFn,std::pair<StringType,ValueType>*,MemberFn)", "eval_end"},
            }, {
                // no errors
            }
        }
    }
};


std::vector<ObjectToGenericFnParam>
ObjectToGenericFnParam::cases = {
    {
        {
            "threeValues", R"(
            {
                "test3": {
                    "id": 2,
                    "name": "Text 3"
                },
                "test2": {
                    "id": 1,
                    "name": "Text 2"
                },
                "test1": {
                    "id": 0,
                    "name": "Text 1"
                }
            })", false
        }, { // Plain
            { // result
                {"test1", { 0, "Text 1" } },
                {"test2", { 1, "Text 2" } },
                {"test3", { 2, "Text 3" } }
            }, {
                // construct
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_function(OutFn)", "eval_begin"},
                {    "object_to_function(OutFn)", "type:other"},
                {    "error::path_scope::path_scope(error*,std::string_view)", "add_member:test3"},
                {    "object_to_function(OutFn)", "get:test3"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "error::path_scope::path_scope(error*,std::string_view)", "add_member:test2"},
                {    "object_to_function(OutFn)", "get:test2"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "error::path_scope::path_scope(error*,std::string_view)", "add_member:test1"},
                {    "object_to_function(OutFn)", "get:test1"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object_to_function(OutFn)", "eval_end"}

            }, {
                // no errors
            }
        }, { // FromMember
            { // result
                {"test1", { 0, "Text 1" } },
                {"test2", { 1, "Text 2" } },
                {"test3", { 2, "Text 3" } }
            }, {
                // construct
                {"object_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "object_to_function(OutFn)", "eval_begin"},
                {            "object_to_function(OutFn)", "type:other"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:test3"},
                {            "object_to_function(OutFn)", "get:test3"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:test2"},
                {            "object_to_function(OutFn)", "get:test2"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "error::path_scope::path_scope(error*,std::string_view)", "add_member:test1"},
                {            "object_to_function(OutFn)", "get:test1"},
                {            "error::path_scope::~path_scope()", "delete"},
                {        "object_to_function(OutFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"}

            }, {
                // no errors
            }
        }, { // CheckNull
            { // result
                {"test1", { 0, "Text 1" } },
                {"test2", { 1, "Text 2" } },
                {"test3", { 2, "Text 3" } }
            }, {
                // construct
                {"object_to_function(bool*,OutFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_function(bool*,OutFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "object_to_function(OutFn)", "eval_begin"},
                {        "object_to_function(OutFn)", "type:other"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:test3"},
                {        "object_to_function(OutFn)", "get:test3"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:test2"},
                {        "object_to_function(OutFn)", "get:test2"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:test1"},
                {        "object_to_function(OutFn)", "get:test1"},
                {        "error::path_scope::~path_scope()", "delete"},
                {    "object_to_function(OutFn)", "eval_end"},
                {"object_to_function(bool*,OutFn)", "eval_end"}

            }, {
                // no errors
            }
        }
    }, {
        {
            "empty", R"({})", false
        }, { // Plain
            {
                // result empty
            }, {
                // construct
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_function(OutFn)", "eval_begin"},
                {    "object_to_function(OutFn)", "type:other"},
                {"object_to_function(OutFn)", "eval_end"},
            }, {
                // no errors
            }
        }, { // FromMember
            {
                // result empty
            }, {
                // construct
                {"object_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "object_to_function(OutFn)", "eval_begin"},
                {            "object_to_function(OutFn)", "type:other"},
                {        "object_to_function(OutFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"}
            }, {
                // no errors
            }
        }, { // CheckNull
            {
                // result empty
            }, {
                // construct
                {"object_to_function(bool*,OutFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_function(bool*,OutFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "object_to_function(OutFn)", "eval_begin"},
                {        "object_to_function(OutFn)", "type:other"},
                {    "object_to_function(OutFn)", "eval_end"},
                {"object_to_function(bool*,OutFn)", "eval_end"},
            }, {
                // no errors
            }
        }
    }, {
        {
            "null", R"(null)", true
        }, { // Plain
            {
                // result empty
            }, {
                // construct
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_function(OutFn)", "eval_begin"},
                {    "object_to_function(OutFn)", "type:other"},
                {    "object_to_function(OutFn)", "error:16"},
                {"object_to_function(OutFn)", "eval_end"},

            }, { // errors
                {simdjson::INCORRECT_TYPE , "<root>"}
            }
        }, { // FromMember
            {
                // result empty
            }, {
                // construct
                {"object_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "object_to_function(OutFn)", "eval_begin"},
                {            "object_to_function(OutFn)", "type:other"},
                {            "object_to_function(OutFn)", "error:16"},
                {        "object_to_function(OutFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"}

            }, { // errors
                {simdjson::INCORRECT_TYPE , "<root>.result"}
            }
        }, { // CheckNull
            {
                // result empty
            }, {
                // construct
                {"object_to_function(bool*,OutFn)", "start"},
                {"object_to_function(OutFn)", "start"},

                // evaluate
                {"object_to_function(bool*,OutFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:true"},
                {    "object_to_function(bool*,OutFn)", "is_null"},
                {"object_to_function(bool*,OutFn)", "eval_end"},
            }, {
                // no errors
            }
        }
    }
};

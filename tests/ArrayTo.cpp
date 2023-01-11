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

#include "ArrayTo.h"


// Set
// ///////////////////////////////////////////////////////////////////////////


TEST_P(ArrayToSet, Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataArrayToSet result;
    TestDataArrayTo tmp_elem;

    auto eval_fn =
        array_to_out_iter<simdjson::ondemand::document>(
            inserter(result, result.end()), &tmp_elem,
            object(
                member("id", number_value(&tmp_elem.id)),
                member("name", string_value(&tmp_elem.name))));

    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataPlain, result, nullptr, errors);
}

TEST_P(ArrayToSet, FromMember) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto newInput = "{\"result\":" + std::string(param.input) + '}';
    auto json = simdjson::padded_string(newInput);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataArrayToSet result;
    TestDataArrayTo tmp_elem;

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("result",
                   array_to_out_iter(
                       inserter(result, result.end()), &tmp_elem,
                       object(
                           member("id", number_value(&tmp_elem.id)),
                           member("name", string_value(&tmp_elem.name)))
                       )
                )
            );

    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataFromMember, result, nullptr, errors);
}


TEST_P(ArrayToSet, CheckNull) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataArrayToSet result;
    bool is_null;

    TestDataArrayTo tmp_elem;

    auto eval_fn =
        array_to_out_iter<simdjson::ondemand::document>(
            &is_null,
            inserter(result, result.end()), &tmp_elem,
            object(
                member("id", number_value(&tmp_elem.id)),
                member("name", string_value(&tmp_elem.name))));

    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataCheckNull, result, &is_null, errors);
}

// Vector
// ///////////////////////////////////////////////////////////////////////////

TEST_P(ArrayToVector, Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataArrayToVector result;
    TestDataArrayTo tmp_elem;

    auto eval_fn =
        array_to_out_iter<simdjson::ondemand::document>(
            inserter(result, result.end()), &tmp_elem,
            object(
                member("id", number_value(&tmp_elem.id)),
                member("name", string_value(&tmp_elem.name))));

    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataPlain, result, nullptr, errors);
}

TEST_P(ArrayToVector, FromMember) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto newInput = "{\"result\":" + std::string(param.input) + '}';
    auto json = simdjson::padded_string(newInput);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataArrayToVector result;
    TestDataArrayTo tmp_elem;

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("result",
                   array_to_out_iter(
                       inserter(result, result.end()), &tmp_elem,
                       object(
                           member("id", number_value(&tmp_elem.id)),
                           member("name", string_value(&tmp_elem.name))
                           )
                       )
                )
            );

    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataFromMember, result, nullptr, errors);
}


TEST_P(ArrayToVector, CheckNull) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataArrayToVector result;
    bool is_null;

    TestDataArrayTo tmp_elem;

    auto eval_fn =
        array_to_out_iter<simdjson::ondemand::document>(
            &is_null,
            inserter(result, result.end()), &tmp_elem,
            object(
                member("id", number_value(&tmp_elem.id)),
                member("name", string_value(&tmp_elem.name))));

    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataCheckNull, result, &is_null, errors);
}

// Function
// ///////////////////////////////////////////////////////////////////////////

TEST_P(ArrayToFunction, Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataArrayToFunction result;
    TestDataArrayTo tmp_elem;

    auto get_element =
        [&result](auto idx, auto *tmp_elem, auto *err) {
            EXPECT_EQ(idx, result.size());
            if (!*err) {
                result.push_back(std::move(*tmp_elem));
            }
        };

    auto eval_fn =
        array_to_function<simdjson::ondemand::document>(
            get_element, &tmp_elem,
            object(
                member("id", number_value(&tmp_elem.id)),
                member("name", string_value(&tmp_elem.name))));

    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataPlain, result, nullptr, errors);
}

TEST_P(ArrayToFunction, FromMember) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto newInput = "{\"result\":" + std::string(param.input) + '}';
    auto json = simdjson::padded_string(newInput);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataArrayToFunction result;
    TestDataArrayTo tmp_elem;

    auto get_element =
        [&result](auto idx, auto *tmp_pair, auto *err) {
            EXPECT_EQ(idx, result.size());
            if (!*err) {
                result.push_back(std::move(*tmp_pair));
            }
        };

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("result",
                   array_to_function(
                       get_element, &tmp_elem,
                       object(
                           member("id", number_value(&tmp_elem.id)),
                           member("name", string_value(&tmp_elem.name))
                           )
                       )
                )
            );

    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataFromMember, result, nullptr, errors);
}


TEST_P(ArrayToFunction, CheckNull) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataArrayToFunction result;
    bool is_null;
    TestDataArrayTo tmp_elem;

    auto get_element =
        [&result](auto idx, auto *tmp_pair, auto *err) {
            EXPECT_EQ(idx, result.size());
            if (!*err) {
                result.push_back(std::move(*tmp_pair));
            }
        };

    auto eval_fn =
        array_to_function<simdjson::ondemand::document>(
            &is_null,
            get_element, &tmp_elem,
            object(
                member("id", number_value(&tmp_elem.id)),
                member("name", string_value(&tmp_elem.name))));

    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataCheckNull, result, &is_null, errors);
}

// Generic Function
// ///////////////////////////////////////////////////////////////////////////

// store value into `*result`
template<typename SJResult>
void
store_value(
    TestDataArrayToGenericFn *result,
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

    result->emplace_back(TestDataArrayTo{id, name});
}


TEST_P(ArrayToGenericFn, Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataArrayToGenericFn result;

    auto get_element =
        [&result](auto idx, auto *sj_result, auto *err) {
            EXPECT_EQ(idx, result.size());
            store_value(&result, sj_result, err);
        };

    auto eval_fn =
        array_to_function<simdjson::ondemand::document>(get_element);

    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataPlain, result, nullptr, errors);
}

TEST_P(ArrayToGenericFn, FromMember) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto newInput = "{\"result\":" + std::string(param.input) + '}';
    auto json = simdjson::padded_string(newInput);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataArrayToGenericFn result;

    auto get_element =
        [&result](auto idx, auto *sj_result, auto *err) {
            EXPECT_EQ(idx, result.size());
            store_value(&result, sj_result, err);
        };

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member("result",
                   array_to_function(get_element)));

    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataFromMember, result, nullptr, errors);
}


TEST_P(ArrayToGenericFn, CheckNull) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    TestDataArrayToGenericFn result;
    bool is_null;

    TestDataArrayTo tmp_elem;

    auto get_element =
        [&result]
        (auto idx, auto *sj_result, auto *err) {
            EXPECT_EQ(idx, result.size());
            store_value(&result, sj_result, err);
        };

    auto eval_fn =
        array_to_function<simdjson::ondemand::document>(
            &is_null,
            get_element);

    eval_fn(dataDoc, &errors);

    arrayCheckResult(
        param, param.dataCheckNull, result, &is_null, errors);
}


// Parameters
////////////////////////////////////////////////////////////////////////

std::vector<ArrayToVectorParam>
ArrayToVectorParam::cases; // filled by copyArrayToVectorCases


std::vector<ArrayToSetParam>
ArrayToSetParam::cases = {
    {
        {
            "threeValues", R"(
            [
                {
                    "id": 2,
                    "name": "Text 3"
                }, {
                    "id": 1,
                    "name": "Text 2"
                }, {
                    "id": 0,
                    "name": "Text 1"
                }
            ])", false
        }, { // Plain
            { // result
                { 0, "Text 1" },
                { 1, "Text 2" },
                { 2, "Text 3" }
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
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_begin"},
                {    "array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {        "array_to_function(OutFn)", "eval_begin"},
                {            "array_to_function(OutFn)", "type:other"},
                {            "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {            "array_to_function(OutFn)", "get:0"},
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {            "array_to_function(OutFn)", "get:1"},
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {            "array_to_function(OutFn)", "get:2"},
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {            "error::path_scope::~path_scope()", "delete"},
                {        "array_to_function(OutFn)", "eval_end"},
                {    "array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_end"},
            }, {
                // no errors
            }
        }, { // FromMember
            { // result
                { 0, "Text 1" },
                { 1, "Text 2" },
                { 2, "Text 3" }
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
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_begin"},
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {                "array_to_function(OutFn)", "eval_begin"},
                {                    "array_to_function(OutFn)", "type:other"},
                {                    "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {                    "array_to_function(OutFn)", "get:0"},
                {                    "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {                    "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                    "error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {                    "array_to_function(OutFn)", "get:1"},
                {                    "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {                    "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                    "error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {                    "array_to_function(OutFn)", "get:2"},
                {                    "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {                    "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {                    "error::path_scope::~path_scope()", "delete"},
                {                "array_to_function(OutFn)", "eval_end"},
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
                {        "array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"}
            }, { // errors
            }
        }, { // CheckNull
            { // result
                { 0, "Text 1" },
                { 1, "Text 2" },
                { 2, "Text 3" }
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
                {"array_to_out_iter(bool*,OutIter,TempValue*,ValueFn)", "start"},
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_out_iter(bool*,OutIter,TempValue*,ValueFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_begin"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {            "array_to_function(OutFn)", "eval_begin"},
                {                "array_to_function(OutFn)", "type:other"},
                {                "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {                "array_to_function(OutFn)", "get:0"},
                {                "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {                "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {                "array_to_function(OutFn)", "get:1"},
                {                "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {                "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {                "array_to_function(OutFn)", "get:2"},
                {                "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {                "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {                "error::path_scope::~path_scope()", "delete"},
                {            "array_to_function(OutFn)", "eval_end"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
                {    "array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_end"},
                {"array_to_out_iter(bool*,OutIter,TempValue*,ValueFn)", "eval_end"},

            }, { // errors
            }
        }
    }, {
        {
            "empty", R"([])", false
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
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_begin"},
                {    "array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {        "array_to_function(OutFn)", "eval_begin"},
                {            "array_to_function(OutFn)", "type:other"},
                {        "array_to_function(OutFn)", "eval_end"},
                {    "array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_end"},
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
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_begin"},
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {                "array_to_function(OutFn)", "eval_begin"},
                {                    "array_to_function(OutFn)", "type:other"},
                {                "array_to_function(OutFn)", "eval_end"},
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
                {        "array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_end"},
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
                {"array_to_out_iter(bool*,OutIter,TempValue*,ValueFn)", "start"},
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_out_iter(bool*,OutIter,TempValue*,ValueFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_begin"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {            "array_to_function(OutFn)", "eval_begin"},
                {                "array_to_function(OutFn)", "type:other"},
                {            "array_to_function(OutFn)", "eval_end"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
                {    "array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_end"},
                {"array_to_out_iter(bool*,OutIter,TempValue*,ValueFn)", "eval_end"}

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
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_begin"},
                {    "array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {        "array_to_function(OutFn)", "eval_begin"},
                {            "array_to_function(OutFn)", "type:other"},
                {            "array_to_function(OutFn)", "error:16"},
                {        "array_to_function(OutFn)", "eval_end"},
                {    "array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_end"},

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
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_begin"},
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {                "array_to_function(OutFn)", "eval_begin"},
                {                "array_to_function(OutFn)", "type:other"},
                {                "array_to_function(OutFn)", "error:16"},
                {            "array_to_function(OutFn)", "eval_end"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
                {    "array_to_out_iter(OutIter,TempValue*,ValueFn)", "eval_end"},
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
                {"array_to_out_iter(bool*,OutIter,TempValue*,ValueFn)", "start"},
                {"array_to_out_iter(OutIter,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_out_iter(bool*,OutIter,TempValue*,ValueFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:true"},
                {    "array_to_out_iter(bool*,OutIter,TempValue*,ValueFn)", "is_null"},
                {"array_to_out_iter(bool*,OutIter,TempValue*,ValueFn)", "eval_end"}
            }, { // errors
            }
        }
    }
};


std::vector<ArrayToFunctionParam>
ArrayToFunctionParam::cases = {
    {
        {
            "threeValues", R"(
            [
                {
                    "id": 2,
                    "name": "Text 3"
                }, {
                    "id": 1,
                    "name": "Text 2"
                }, {
                    "id": 0,
                    "name": "Text 1"
                }
            ])", false
        }, { // Plain
            { // result
                { 2, "Text 3" },
                { 1, "Text 2" },
                { 0, "Text 1" }
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
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {    "array_to_function(OutFn)", "eval_begin"},
                {        "array_to_function(OutFn)", "type:other"},
                {        "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {        "array_to_function(OutFn)", "get:0"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {        "array_to_function(OutFn)", "get:1"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {        "array_to_function(OutFn)", "get:2"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {        "error::path_scope::~path_scope()", "delete"},
                {    "array_to_function(OutFn)", "eval_end"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
            }, {
                // no errors
            }
        }, { // FromMember
            { // result
                { 2, "Text 3" },
                { 1, "Text 2" },
                { 0, "Text 1" }
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
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {            "array_to_function(OutFn)", "eval_begin"},
                {                "array_to_function(OutFn)", "type:other"},
                {                "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {                "array_to_function(OutFn)", "get:0"},
                {                "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {                "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {                "array_to_function(OutFn)", "get:1"},
                {                "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {                "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {                "error::path_scope::~path_scope()", "delete"},
                {                "error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {                "array_to_function(OutFn)", "get:2"},
                {                "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {                "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {                "error::path_scope::~path_scope()", "delete"},
                {            "array_to_function(OutFn)", "eval_end"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"}
            }, {
                // no errors
            }
        }, { // CheckNull
            { // result
                { 2, "Text 3" },
                { 1, "Text 2" },
                { 0, "Text 1" }
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
                {"array_to_function(bool*,OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_function(bool*,OutFn,TempValue*,ValueFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {        "array_to_function(OutFn)", "eval_begin"},
                {            "array_to_function(OutFn)", "type:other"},
                {            "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {            "array_to_function(OutFn)", "get:0"},
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {            "array_to_function(OutFn)", "get:1"},
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {            "array_to_function(OutFn)", "get:2"},
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "start_gen_out_fn"},
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
                {            "array_to_function(OutFn,TempValue*,ValueFn)", "end_gen_out_fn"},
                {            "error::path_scope::~path_scope()", "delete"},
                {        "array_to_function(OutFn)", "eval_end"},
                {    "array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
                {"array_to_function(bool*,OutFn,TempValue*,ValueFn)", "eval_end"},
            }, {
                // no errors
            }
        }
    }, {
        {
            "empty", R"([])", false
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
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {    "array_to_function(OutFn)", "eval_begin"},
                {        "array_to_function(OutFn)", "type:other"},
                {    "array_to_function(OutFn)", "eval_end"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
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
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {            "array_to_function(OutFn)", "eval_begin"},
                {                "array_to_function(OutFn)", "type:other"},
                {            "array_to_function(OutFn)", "eval_end"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
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
                {"array_to_function(bool*,OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_function(bool*,OutFn,TempValue*,ValueFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {        "array_to_function(OutFn)", "eval_begin"},
                {            "array_to_function(OutFn)", "type:other"},
                {        "array_to_function(OutFn)", "eval_end"},
                {    "array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
                {"array_to_function(bool*,OutFn,TempValue*,ValueFn)", "eval_end"},
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
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {    "array_to_function(OutFn)", "eval_begin"},
                {        "array_to_function(OutFn)", "type:other"},
                {        "array_to_function(OutFn)", "error:16"},
                {    "array_to_function(OutFn)", "eval_end"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},

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
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "eval_begin"},
                {            "array_to_function(OutFn)", "eval_begin"},
                {                "array_to_function(OutFn)", "type:other"},
                {                "array_to_function(OutFn)", "error:16"},
                {            "array_to_function(OutFn)", "eval_end"},
                {        "array_to_function(OutFn,TempValue*,ValueFn)", "eval_end"},
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
                {"array_to_function(bool*,OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn,TempValue*,ValueFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_function(bool*,OutFn,TempValue*,ValueFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:true"},
                {    "array_to_function(bool*,OutFn,TempValue*,ValueFn)", "is_null"},
                {"array_to_function(bool*,OutFn,TempValue*,ValueFn)", "eval_end"},
            }, {
                // no errors
            }
        }
    }
};


std::vector<ArrayToGenericFnParam>
ArrayToGenericFnParam::cases = {
    {
        {
            "threeValues", R"(
            [
                {
                    "id": 2,
                    "name": "Text 3"
                }, {
                    "id": 1,
                    "name": "Text 2"
                }, {
                    "id": 0,
                    "name": "Text 1"
                }
            ])", false
        }, { // Plain
            { // result
                { 2, "Text 3" },
                { 1, "Text 2" },
                { 0, "Text 1" }
            }, {
                // construct
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_function(OutFn)", "eval_begin"},
                {    "array_to_function(OutFn)", "type:other"},
                {    "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {    "array_to_function(OutFn)", "get:0"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {    "array_to_function(OutFn)", "get:1"},
                {    "error::path_scope::~path_scope()", "delete"},
                {    "error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {    "array_to_function(OutFn)", "get:2"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"array_to_function(OutFn)", "eval_end"}

            }, {
                // no errors
            }
        }, { // FromMember
            { // result
                { 2, "Text 3" },
                { 1, "Text 2" },
                { 0, "Text 1" }
            }, {
                // construct
                {"array_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "array_to_function(OutFn)", "eval_begin"},
                {            "array_to_function(OutFn)", "type:other"},
                {            "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {            "array_to_function(OutFn)", "get:0"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {            "array_to_function(OutFn)", "get:1"},
                {            "error::path_scope::~path_scope()", "delete"},
                {            "error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {            "array_to_function(OutFn)", "get:2"},
                {            "error::path_scope::~path_scope()", "delete"},
                {        "array_to_function(OutFn)", "eval_end"},
                {    "member(std::string_view,ValueFn)", "eval_end"},
                {    "error::path_scope::~path_scope()", "delete"},
                {"object(MemberFn)", "eval_end"}

            }, {
                // no errors
            }
        }, { // CheckNull
            { // result
                { 2, "Text 3" },
                { 1, "Text 2" },
                { 0, "Text 1" }
            }, {
                // construct
                {"array_to_function(bool*.OutFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_function(bool*.OutFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "array_to_function(OutFn)", "eval_begin"},
                {        "array_to_function(OutFn)", "type:other"},
                {        "error::path_scope::path_scope(error*,size_t)", "add_idx:0"},
                {        "array_to_function(OutFn)", "get:0"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "error::path_scope::path_scope(error*,size_t)", "add_idx:1"},
                {        "array_to_function(OutFn)", "get:1"},
                {        "error::path_scope::~path_scope()", "delete"},
                {        "error::path_scope::path_scope(error*,size_t)", "add_idx:2"},
                {        "array_to_function(OutFn)", "get:2"},
                {        "error::path_scope::~path_scope()", "delete"},
                {    "array_to_function(OutFn)", "eval_end"},
                {"array_to_function(bool*.OutFn)", "eval_end"}

            }, {
                // no errors
            }
        }
    }, {
        {
            "empty", R"([])", false
        }, { // Plain
            {
                // result empty
            }, {
                // construct
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_function(OutFn)", "eval_begin"},
                {    "array_to_function(OutFn)", "type:other"},
                {"array_to_function(OutFn)", "eval_end"},
            }, {
                // no errors
            }
        }, { // FromMember
            {
                // result empty
            }, {
                // construct
                {"array_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "array_to_function(OutFn)", "eval_begin"},
                {            "array_to_function(OutFn)", "type:other"},
                {        "array_to_function(OutFn)", "eval_end"},
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
                {"array_to_function(bool*.OutFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_function(bool*.OutFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:false"},
                {    "array_to_function(OutFn)", "eval_begin"},
                {        "array_to_function(OutFn)", "type:other"},
                {    "array_to_function(OutFn)", "eval_end"},
                {"array_to_function(bool*.OutFn)", "eval_end"},
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
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_function(OutFn)", "eval_begin"},
                {    "array_to_function(OutFn)", "type:other"},
                {    "array_to_function(OutFn)", "error:16"},
                {"array_to_function(OutFn)", "eval_end"},

            }, { // errors
                {simdjson::INCORRECT_TYPE , "<root>"}
            }
        }, { // FromMember
            {
                // result empty
            }, {
                // construct
                {"array_to_function(OutFn)", "start"},
                {"member(std::string_view,ValueFn)", "start"},
                {"object(MemberFn)", "start"},

                // evaluate
                {"object(MemberFn)", "eval_begin"},
                {    "object(MemberFn)", "type:other"},
                {    "member(std::string_view,ValueFn)", "eval_begin"},
                {        "error::path_scope::path_scope(error*,std::string_view)", "add_member:result"},
                {        "array_to_function(OutFn)", "eval_begin"},
                {            "array_to_function(OutFn)", "type:other"},
                {            "array_to_function(OutFn)", "error:16"},
                {        "array_to_function(OutFn)", "eval_end"},
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
                {"array_to_function(bool*.OutFn)", "start"},
                {"array_to_function(OutFn)", "start"},

                // evaluate
                {"array_to_function(bool*.OutFn)", "eval_begin"},
                {    "check_null(SJValue,bool*)", "start"},
                {    "check_null(SJValue,bool*)", "is_null:true"},
                {    "array_to_function(bool*.OutFn)", "is_null"},
                {"array_to_function(bool*.OutFn)", "eval_end"},
            }, {
                // no errors
            }
        }
    }
};

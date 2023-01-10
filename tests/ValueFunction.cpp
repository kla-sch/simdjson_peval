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

#include "ValueFunction.h"

TEST_P(ValueFunction, Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);

    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    double result;

    auto factor = 1.5;
    auto value_fn =
        [&factor](auto *sj_result, auto *value_ptr, auto *err) {
            const auto error_value =
                sj_result->value().get_double().get(*value_ptr);
            if (error_value) {
                err->add(error_value);
            }
            else {
                *value_ptr *= factor;
            }
        };

    auto proto = value<simdjson::ondemand::document>(&result, value_fn);
    proto(dataDoc, &errors);

    checkResult(param, param.dataPlain, result, nullptr, errors);
}

TEST_P(ValueFunction, PlainWithNull) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    double result;
    bool is_null;

    auto factor = 1.5;
    auto value_fn =
        [&factor](auto *sj_result, auto *value_ptr, auto *err) {
            const auto error_value =
                sj_result->value().get_double().get(*value_ptr);
            if (error_value) {
                err->add(error_value);
            }
            else {
                *value_ptr *= factor;
            }
        };

    auto proto =
        value<simdjson::ondemand::document>(&result, value_fn, &is_null);
    proto(dataDoc, &errors);

    checkResult(param, param.dataPlainWithNull, result, &is_null, errors);
}


TEST_P(ValueFunction, Optional) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    std::optional<double> result;

    auto factor = 1.5;
    auto value_fn =
        [&factor](auto *sj_result, auto *value_ptr, auto *err) {
            const auto error_value =
                sj_result->value().get_double().get(**value_ptr);
            if (error_value) {
                err->add(error_value);
            }
            else {
                **value_ptr *= factor;
            }
        };

    auto proto = value<simdjson::ondemand::document>(&result, value_fn);
    proto(dataDoc, &errors);

    checkResult(param, param.dataOptional, result, errors);
}


std::vector<ValueFunctionParam>
ValueFunctionParam::cases = {
    {{
        "value",  R"(5.5)", 8.25, false,
        { // plain
            {
                {"value(ValuePtr,GetFn)", "start"},
                {"value(ValuePtr,GetFn)", "eval_begin"},
                {"value(ValuePtr,GetFn)", "eval_end"}
            }, {
            }
        }, { // plain with null
            {
                {"value(ValuePtr,GetFn,bool*)", "start"},
                {"value(ValuePtr,GetFn,bool*)", "eval_begin"},
                {"check_null(SJValue,bool*)", "start"},
                {"check_null(SJValue,bool*)", "is_null:false"},
                {"value(ValuePtr,GetFn,bool*)", "eval_end"}
            }, {
            }
        }, { // optional
            {
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"value(std::optional<Value>*,GetFn)", "eval_begin"},
                {"check_null(SJValue,Optional)", "start"},
                {"check_null(SJValue,Optional)", "is_null:false"},
                {"value(std::optional<Value>*,GetFn)", "eval_end"}
            }, {
            }
        }
    }}, {{
        "null",  R"(null)", 0.0, true,
        { // plain
            {
                {"value(ValuePtr,GetFn)", "start"},
                {"value(ValuePtr,GetFn)", "eval_begin"},
                {"value(ValuePtr,GetFn)", "eval_end"}
            }, {
                { simdjson::INCORRECT_TYPE, "<root>" }
            }
        }, { // plain with null
            {
                {"value(ValuePtr,GetFn,bool*)", "start"},
                {"value(ValuePtr,GetFn,bool*)", "eval_begin"},
                {"check_null(SJValue,bool*)", "start"},
                {"check_null(SJValue,bool*)", "is_null:true"},
                {"value(ValuePtr,GetFn,bool*)", "is_null"},
                {"value(ValuePtr,GetFn,bool*)", "eval_end"}
            }, {
            }
        }, { // optional
            {
                {"value(std::optional<Value>*,GetFn)", "start"},
                {"value(std::optional<Value>*,GetFn)", "eval_begin"},
                {"check_null(SJValue,Optional)", "start"},
                {"check_null(SJValue,Optional)", "is_null:true"},
                {"value(std::optional<Value>*,GetFn)", "is_null"},
                {"value(std::optional<Value>*,GetFn)", "eval_end"}
            }, {
            }
        }
    }}
};

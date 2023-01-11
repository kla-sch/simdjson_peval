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

#include "ValueStringToDouble.h"

TEST_P(ValueStringToDouble, Plain) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);

    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    double result;

    auto proto = string_value<simdjson::ondemand::document>(&result);
    proto(dataDoc, &errors);

    checkResult(param, param.dataPlain, result, nullptr, errors);
}

TEST_P(ValueStringToDouble, PlainWithNull) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    double result;
    bool is_null;

    auto proto = string_value<simdjson::ondemand::document>(&result, &is_null);
    proto(dataDoc, &errors);

    checkResult(param, param.dataPlainWithNull, result, &is_null, errors);
}


TEST_P(ValueStringToDouble, Optional) {
    using namespace simdjson_peval;
    const auto &param = GetParam();

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(param.input);
    auto dataDoc = parser.iterate(json);
    simdjson_peval::_trace_buffer.clear();

    error errors(true);
    std::optional<double> result;

    auto proto = string_value<simdjson::ondemand::document>(&result);
    proto(dataDoc, &errors);

    checkResult(param, param.dataOptional, result, errors);
}


// Parameters
////////////////////////////////////////////////////////////////////////

std::vector<ValueStringToDoubleParam>
ValueStringToDoubleParam::cases = {
    {{
        "positiveInt", R"("10")", 10, false,
        { // Plain
            {
                {"string_value(double*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},

                {"value(ValuePtr,GetFn)", "eval_begin"},
                {"string_value(double*)", "start:get_value"},
                {"value(ValuePtr,GetFn)", "eval_end"}
            }, {
            }
        }, { // Plain with null
            {
                {"string_value(double*,bool*)", "start"},
                {"value(ValuePtr,GetFn,bool*)", "start"},

                {"value(ValuePtr,GetFn,bool*)", "eval_begin"},
                {"check_null(SJValue,bool*)", "start"},
                {"check_null(SJValue,bool*)", "is_null:false"},
                {"string_value(double*,bool*)", "start:get_value"},
                {"value(ValuePtr,GetFn,bool*)", "eval_end"}
            }, {
            }
        }, { // Optional
            {
                {"string_value(std::optional<double>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},

                {"value(std::optional<Value>*,GetFn)", "eval_begin"},
                {"check_null(SJValue,Optional)", "start"},
                {"check_null(SJValue,Optional)", "is_null:false"},
                {"string_value(std::optional<double>*)", "start:get_value"},
                {"value(std::optional<Value>*,GetFn)", "eval_end"},
            }, {
            }
        }
    }}, {{
        "negativeInt", R"("-10")", -10, false,
        { // Plain
            {
                {"string_value(double*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},

                {"value(ValuePtr,GetFn)", "eval_begin"},
                {"string_value(double*)", "start:get_value"},
                {"value(ValuePtr,GetFn)", "eval_end"}
            }, {
            }
        }, { // Plain with null
            {
                {"string_value(double*,bool*)", "start"},
                {"value(ValuePtr,GetFn,bool*)", "start"},

                {"value(ValuePtr,GetFn,bool*)", "eval_begin"},
                {"check_null(SJValue,bool*)", "start"},
                {"check_null(SJValue,bool*)", "is_null:false"},
                {"string_value(double*,bool*)", "start:get_value"},
                {"value(ValuePtr,GetFn,bool*)", "eval_end"}
            }, {
            }
        }, { // Optional
            {
                {"string_value(std::optional<double>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},

                {"value(std::optional<Value>*,GetFn)", "eval_begin"},
                {"check_null(SJValue,Optional)", "start"},
                {"check_null(SJValue,Optional)", "is_null:false"},
                {"string_value(std::optional<double>*)", "start:get_value"},
                {"value(std::optional<Value>*,GetFn)", "eval_end"},
            }, {
            }
        }
    }}, {{
        "double", R"("10.5")", 10.5, false,
        { // Plain
            {
                {"string_value(double*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},

                {"value(ValuePtr,GetFn)", "eval_begin"},
                {"string_value(double*)", "start:get_value"},
                {"value(ValuePtr,GetFn)", "eval_end"}
            }, {
            }
        }, { // Plain with null
            {
                {"string_value(double*,bool*)", "start"},
                {"value(ValuePtr,GetFn,bool*)", "start"},

                {"value(ValuePtr,GetFn,bool*)", "eval_begin"},
                {"check_null(SJValue,bool*)", "start"},
                {"check_null(SJValue,bool*)", "is_null:false"},
                {"string_value(double*,bool*)", "start:get_value"},
                {"value(ValuePtr,GetFn,bool*)", "eval_end"}
            }, {
            }
        }, { // Optional
            {
                {"string_value(std::optional<double>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},

                {"value(std::optional<Value>*,GetFn)", "eval_begin"},
                {"check_null(SJValue,Optional)", "start"},
                {"check_null(SJValue,Optional)", "is_null:false"},
                {"string_value(std::optional<double>*)", "start:get_value"},
                {"value(std::optional<Value>*,GetFn)", "eval_end"},
            }, {
            }
        }
    }}, {{
        "null", R"(null)", 0, true,
        { // Plain
            {
                {"string_value(double*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},

                {"value(ValuePtr,GetFn)", "eval_begin"},
                {"string_value(double*)", "start:get_value"},
                {"string_value(double*)", "error:16"},
                {"value(ValuePtr,GetFn)", "eval_end"}
            }, {
                { simdjson::INCORRECT_TYPE, "<root>" }
            }
        }, { // Plain with null
            {
                {"string_value(double*,bool*)", "start"},
                {"value(ValuePtr,GetFn,bool*)", "start"},

                {"value(ValuePtr,GetFn,bool*)", "eval_begin"},
                {"check_null(SJValue,bool*)", "start"},
                {"check_null(SJValue,bool*)", "is_null:true"},
                {"value(ValuePtr,GetFn,bool*)", "is_null"},
                {"value(ValuePtr,GetFn,bool*)", "eval_end"}
            }, {
            }
        }, { // Optional
            {
                {"string_value(std::optional<double>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},

                {"value(std::optional<Value>*,GetFn)", "eval_begin"},
                {"check_null(SJValue,Optional)", "start"},
                {"check_null(SJValue,Optional)", "is_null:true"},
                {"value(std::optional<Value>*,GetFn)", "is_null"},
                {"value(std::optional<Value>*,GetFn)", "eval_end"}
            }, {
            }
        }
    }}, {{
        "text", R"("text")", 0, false,
        { // Plain
            {
                {"string_value(double*)", "start"},
                {"value(ValuePtr,GetFn)", "start"},

                {"value(ValuePtr,GetFn)", "eval_begin"},
                {"string_value(double*)", "start:get_value"},
                {"string_value(double*)", "error:16"},
                {"value(ValuePtr,GetFn)", "eval_end"}
            }, {
                { simdjson::INCORRECT_TYPE, "<root>" }
            }
        }, { // Plain with null
            {
                {"string_value(double*,bool*)", "start"},
                {"value(ValuePtr,GetFn,bool*)", "start"},

                {"value(ValuePtr,GetFn,bool*)", "eval_begin"},
                {"check_null(SJValue,bool*)", "start"},
                {"check_null(SJValue,bool*)", "is_null:false"},
                {"string_value(double*,bool*)", "start:get_value"},
                {"string_value(double*,bool*)", "error:16"},
                {"value(ValuePtr,GetFn,bool*)", "eval_end"}
            }, {
                { simdjson::INCORRECT_TYPE, "<root>" }
            }
        }, { // Optional
            {
                {"string_value(std::optional<double>*)", "start"},
                {"value(std::optional<Value>*,GetFn)", "start"},

                {"value(std::optional<Value>*,GetFn)", "eval_begin"},
                {"check_null(SJValue,Optional)", "start"},
                {"check_null(SJValue,Optional)", "is_null:false"},
                {"string_value(std::optional<double>*)", "start:get_value"},
                {"string_value(std::optional<double>*)", "error:16"},
                {"value(std::optional<Value>*,GetFn)", "eval_end"}
            }, {
                { simdjson::INCORRECT_TYPE, "<root>" }
            }
        }
    }}
};

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
// Description: Shows usage of function calls to evaluate values.
//

#include <simdjson_peval.h>

#include <iostream>

// JSON-Data:
static const std::string_view json_data =
R"(
    [
        {
            "name": "apple",
            "price": 1.25
        }, {
            "name": "bananas",
            "price": 0.95
        }, {
            "name": "pineapple",
            "price": 2.18
        }, {
            "name": "avocado",
            "price": 0.68
        }, {
            "name": "pear",
            "price": 1.39
        }, {
            "name": "lemon",
            "price": 0.58
        }, {
            "name": "orange",
            "price": 0.88
        }
    ]
)";


int
main(int argc, char *argv[]) {
    using namespace simdjson_peval;

    simdjson::ondemand::parser parser;
    auto padded_json = simdjson::padded_string(json_data);
    auto json_document = parser.iterate(padded_json);

    int count=0;
    double sum=0;
    double tmpValue; // temporary value.

    // lambda: get value and count/summerize it.
    auto sum_and_count =
        [&count, &sum] (auto idx, auto *value, auto *error) {
            if (!*error) {
                sum += *value;
                ++count;
            }
        };

    // write prices into `tmpValue` and call `sum_and_count`
    auto eval_sum_and_count =
        array_to_function<simdjson::ondemand::document>(
            sum_and_count, &tmpValue,
            object(
                member("price", number_value(&tmpValue))));

    // error container. collecting path names enabled.
    error errors(true);

    // execute evaluation function to calculate result.
    eval_sum_and_count(json_document, &errors);

    if (errors) {
        // print errors
        std::cerr << "Error:\n" << errors.to_string() << '\n';
        return 1;
    }
    else {
        // print result
        std::cout
            << "number of fruits: " << count << '\n'
            << "sum of price: " << sum << '\n'
            << "avage price: " << (sum / count) << '\n';
        return 0;
    }
}

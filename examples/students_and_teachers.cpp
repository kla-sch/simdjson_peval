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
// Description: Shows usage of discriminators.
//

#include <simdjson_peval.h>

#include <iostream>

// JSON-Data:
static const std::string_view json_data =
R"(
    [
        {
            "name": "Peter",
            "type": "student",
            "debts": 19000,
            "grade": "B"
        }, {
            "name": "Carl",
            "type": "teacher",
            "salary": 100000,
            "room": "02.020"
        }, {
            "name": "Marry",
            "type": "teacher",
            "salary": 120000,
            "room": "01.013"
        }, {
            "name": "Josephin",
            "type": "student",
            "debts": 23000,
            "grade": "A"
        }, {
            "name": "Daiana",
            "type": "student",
            "debts": 10000,
            "grade": "A+"
        }
    ]
)";

int
main(int argc, char *argv[]) {
    using namespace simdjson_peval;

    struct Person {
        std::string name;
        std::string type;

        // only for students
        std::optional<double> debts;
        std::optional<std::string> grade;

        // only for teacher
        std::optional<double> salary;
        std::optional<std::string> room;
    };

    std::vector<Person> all_persons;
    Person tmpPerson; // temporary record to fill

    // check for student
    auto is_student =
        [&tmpPerson](auto Üresult, error *err) {
            return (tmpPerson.type == "student");
        };

    // check for teacher
    auto is_teacher =
        [&tmpPerson](auto *result, error *err) {
            return (tmpPerson.type == "teacher");
        };

    // create evaluation function to write data to `tmpPerson` move it
    // at the end of`all_persons`
    auto eval_students_and_teachers =
        array_to_out_iter<simdjson::ondemand::document>(
            back_inserter(all_persons), &tmpPerson,
            object(
                member("name", string_value(&tmpPerson.name)),
                member("type", string_value(&tmpPerson.type)),
                discriminator(
                    is_student,
                    member("debts", number_value(&tmpPerson.debts)),
                    member("grade", string_value(&tmpPerson.grade))
                ),
                discriminator(
                    is_teacher,
                    member("salary", number_value(&tmpPerson.salary)),
                    member("room", string_value(&tmpPerson.room))
                )
            )
        );

    // parse document
    simdjson::ondemand::parser parser;
    auto padded_json = simdjson::padded_string(json_data);
    auto json_document = parser.iterate(padded_json);

    // error container. collecting path names enabled.
    error errors(true);

    // execute evaluation function to create result vector
    eval_students_and_teachers(json_document, &errors);

    if (errors) {
        // print errors
        std::cerr << "Error:\n" << errors.to_string() << '\n';
    }
    else {
        // print result
        for (const auto &p : all_persons) {
            if (p.type=="student") {
                std::cout << p.type << ": " << p.name
                          << ", debts=" << *p.debts
                          << ", grade=" << *p.grade
                          << '\n';
            }
            else if (p.type == "teacher") {
                std::cout << p.type << ": " << p.name
                          << ", salary=" << *p.salary
                          << ", room=" << *p.room
                          << '\n';
            }
            else {
                std::cout << "unknown: " << p.name << '\n';
            }
        }
    }
}

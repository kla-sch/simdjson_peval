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

#ifndef SIMDJSON_PEVAL_TEST_OBJECT_PLAIN_H
#define SIMDJSON_PEVAL_TEST_OBJECT_PLAIN_H 1

#include "Object.h"

#include <string>

struct TestDataObjectPlain {
    uint64_t id;
    std::string_view name;
    bool no_name;
    std::string_view description;
    bool no_description;

    TestDataObjectPlain()
    : id(0)
    , no_name(false)
    , no_description(false)
    { /* empty */ }

    TestDataObjectPlain(
        uint64_t id,
        std::string_view name,
        bool no_name,
        std::string_view description,
        bool no_description)
    : id(id)
    , name(name)
    , no_name(no_name)
    , description(description)
    , no_description(no_description)
    { /* empty */ }



    bool
    operator==(const TestDataObjectPlain &other) const {
        return (
            id == other.id
            && name == other.name
            && no_name == other.no_name
            && description == other.description
            && no_description == other.no_description);
    }

    friend std::ostream &
    operator<<(std::ostream &ostr, const TestDataObjectPlain &data)  {
        return ostr << "[ id:" << data.id
                    << ", name:" << data.name
                    << ", no_name:" << data.no_name
                    << ", description:" << data.description
                    << ", no_description:" << data.no_description
                    <<" ]";
    }
};


class ObjectPlainParam : public ObjectParam<TestDataObjectPlain> {
public:

    Data dataPlain;
    Data dataNullChecked;
    Data dataOptMembers;
    Data dataOneElement;

    static std::vector<ObjectPlainParam> cases;
};

class ObjectPlain
    : public Object<TestDataObjectPlain>
    , public testing::WithParamInterface<ObjectPlainParam>
{
public:
};


#endif /* SIMDJSON_PEVAL_TEST_OBJECT_PLAIN_H */

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

#ifndef SIMDJSON_PEVAL_TEST_OBJECT_MISC_H
#define SIMDJSON_PEVAL_TEST_OBJECT_MISC_H 1

#include "Object.h"

#include <string>

struct TestDataObjectMisc {

    uint64_t id;
    std::string_view name;
    std::string_view description;

    TestDataObjectMisc()
    : id(0)
    { /* empty */ }

    TestDataObjectMisc(
        uint64_t id,
        std::string_view name,
        std::string_view description)
    : id(id)
    , name(name)
    , description(description)
    { /* empty */ }

    bool
    operator==(const TestDataObjectMisc &other) const {
        return (
            id == other.id
            && name == other.name
            && description == other.description);
    }

    friend std::ostream &
    operator<<(std::ostream &ostr, const TestDataObjectMisc &data)  {
        return ostr << "[ id:" << data.id
                    << ", name:" << data.name
                    << ", description:" << data.description
                    <<" ]";
    }
};


class ObjectMiscParam : public ObjectParam<TestDataObjectMisc> {
public:

    Data dataPlain;
    Data dataNullChecked;
    Data dataFromMember;

    static std::vector<ObjectMiscParam> cases;
};

class ObjectMisc
    : public Object<TestDataObjectMisc>
    , public testing::WithParamInterface<ObjectMiscParam>
{
public:
};


#endif /* SIMDJSON_PEVAL_TEST_OBJECT_MISC_H */

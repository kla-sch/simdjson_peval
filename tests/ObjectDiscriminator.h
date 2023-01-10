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

#ifndef SIMDJSON_PEVAL_TEST_OBJECT_DISCRIMINATOR_H
#define SIMDJSON_PEVAL_TEST_OBJECT_DISCRIMINATOR_H 1

#include "Object.h"

#include <string>

struct TestDataDiscriminator {
    uint64_t id;
    std::string_view name;
    std::string_view type;

    // typeA
    std::optional<int64_t> typeA_val1;
    std::optional<std::string_view> typeA_val2;

    // typeB
    std::optional<int64_t> typeB_val1;
    std::optional<std::string_view> typeB_val2;

    bool
    operator==(const TestDataDiscriminator &other) const {
        return (
            id == other.id
            && name == other.name
            && type == other.type
            && typeA_val1 == other.typeA_val1
            && typeA_val2 == other.typeA_val2
            && typeB_val1 == other.typeB_val1
            && typeB_val2 == other.typeB_val2);
    }

    friend std::ostream &
    operator<<(std::ostream &ostr, const TestDataDiscriminator &data)  {
        return ostr << "[ id:" << data.id
                    << ", name:" << data.name
                    << ", type:" << data.type
                    << ", typeA_val1:"
                    <<        (data.typeA_val1.has_value()
                               ? std::to_string(data.typeA_val1.value())
                               : "<null>")
                    << ", typeA_val2:" << data.typeA_val2.value_or("<null>")
                    << ", typeB_val1:"
                    <<        (data.typeB_val1.has_value()
                               ? std::to_string(data.typeB_val1.value())
                               : "<null>")
                    << ", typeB_val2:" << data.typeB_val2.value_or("<null>")
                    <<" ]";
    }
};


class ObjectDiscriminatorParam : public ObjectParam<TestDataDiscriminator> {
public:

    Data dataValueFromResult;
    Data dataValueFromObject;

    static std::vector<ObjectDiscriminatorParam> cases;
};

class ObjectDiscriminator
    : public Object<TestDataDiscriminator>
    , public testing::WithParamInterface<ObjectDiscriminatorParam>
{
};


#endif /* SIMDJSON_PEVAL_TEST_OBJECT_DISCRIMINATOR_H */

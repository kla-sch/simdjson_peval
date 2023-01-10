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

#ifndef SIMDJSON_PEVAL_TEST_ARRAY_TO_H
#define SIMDJSON_PEVAL_TEST_ARRAY_TO_H 1

#include "Array.h"

#include <string>
#include <vector>
#include <set>

struct TestDataArrayTo {
    uint64_t id;
    std::string_view name;

    bool
    operator==(const TestDataArrayTo &other) const {
        return (
            id == other.id
            && name == other.name);
    }

    bool
    operator<(const TestDataArrayTo &other) const {
        if (id == other.id) {
            return (name < other.name);
        }

        return (id < other.id);
    }

    friend std::ostream &
    operator<<(std::ostream &ostr, const TestDataArrayTo &data)  {
        return ostr << "[ id:" << data.id << ", name:" << data.name <<" ]";
    }
};

// Set
// ///////////////////////////////////////////////////////////////////////////

using TestDataArrayToSet = std::set<TestDataArrayTo>;

class ArrayToSetParam : public ArrayParam<TestDataArrayToSet> {
public:

    Data dataPlain;
    Data dataFromMember;
    Data dataCheckNull;

    static std::vector<ArrayToSetParam> cases;
};

class ArrayToSet
    : public Array<TestDataArrayToSet>
    , public testing::WithParamInterface<ArrayToSetParam>
{
};

// Vector
// ///////////////////////////////////////////////////////////////////////////

using TestDataArrayToVector = std::vector<TestDataArrayTo>;

class ArrayToVectorParam : public ArrayParam<TestDataArrayToVector> {
public:

    Data dataPlain;
    Data dataFromMember;
    Data dataCheckNull;

    static std::vector<ArrayToVectorParam> cases;
};

/// convert set result into vector result.
inline TestDataArrayToVector
createVectorDataFromSetData(const TestDataArrayToSet &setData) {
    TestDataArrayToVector result;

    if (setData.empty()) {
        return result;
    }

    result = {
        { 2, "Text 3" },
        { 1, "Text 2" },
        { 0, "Text 1" }
    };

    return result;
}


// copy ArrayToSetParam::cases to ArrayToVectorParam::cases.
inline void
copyArrayToVectorCases() {
    ArrayToVectorParam::cases.reserve(ArrayToSetParam::cases.size());

    transform(
        ArrayToSetParam::cases.begin(), ArrayToSetParam::cases.end(),
        back_inserter(ArrayToVectorParam::cases),
        [](const auto &setCase) {
            return
                ArrayToVectorParam(
                    {
                        {
                            setCase.name,
                            setCase.input,
                            setCase.is_null
                        }, {
                            createVectorDataFromSetData(setCase.dataPlain.result),
                            setCase.dataPlain.trace,
                            setCase.dataPlain.errors
                        }, {
                            createVectorDataFromSetData(setCase.dataFromMember.result),
                            setCase.dataFromMember.trace,
                            setCase.dataFromMember.errors
                        }, {
                            createVectorDataFromSetData(setCase.dataCheckNull.result),
                            setCase.dataCheckNull.trace,
                            setCase.dataCheckNull.errors
                        }
                    });
        });
}



class ArrayToVector
    : public Array<TestDataArrayToVector>
    , public testing::WithParamInterface<ArrayToVectorParam>
{
};


// Function
// ///////////////////////////////////////////////////////////////////////////

using TestDataArrayToFunction = std::vector<TestDataArrayTo>;

class ArrayToFunctionParam : public ArrayParam<TestDataArrayToFunction> {
public:

    Data dataPlain;
    Data dataFromMember;
    Data dataCheckNull;

    static std::vector<ArrayToFunctionParam> cases;
};

class ArrayToFunction
    : public Array<TestDataArrayToFunction>
    , public testing::WithParamInterface<ArrayToFunctionParam>
{
};


// Generic Function
// ///////////////////////////////////////////////////////////////////////////

using TestDataArrayToGenericFn = std::vector<TestDataArrayTo>;

class ArrayToGenericFnParam : public ArrayParam<TestDataArrayToGenericFn> {
public:

    Data dataPlain;
    Data dataFromMember;
    Data dataCheckNull;

    static std::vector<ArrayToGenericFnParam> cases;
};

class ArrayToGenericFn
    : public Array<TestDataArrayToGenericFn>
    , public testing::WithParamInterface<ArrayToGenericFnParam>
{
};


#endif /* SIMDJSON_PEVAL_TEST_ARRAY_TO_H */

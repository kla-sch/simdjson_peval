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

#ifndef SIMDJSON_PEVAL_TEST_OBJECT_TO_H
#define SIMDJSON_PEVAL_TEST_OBJECT_TO_H 1

#include "Object.h"

#include <string>
#include <vector>
#include <map>

struct TestDataObjectTo {
    uint64_t id;
    std::string_view name;

    bool
    operator==(const TestDataObjectTo &other) const {
        return (
            id == other.id
            && name == other.name);
    }

    friend std::ostream &
    operator<<(std::ostream &ostr, const TestDataObjectTo &data)  {
        return ostr << "[ id:" << data.id << ", name:" << data.name <<" ]";
    }
};

// Map
// ///////////////////////////////////////////////////////////////////////////

using TestDataObjectToMap = std::map<std::string_view,TestDataObjectTo>;

class ObjectToMapParam : public ObjectParam<TestDataObjectToMap> {
public:

    Data dataPlain;
    Data dataFromMember;
    Data dataCheckNull;

    static std::vector<ObjectToMapParam> cases;
};

class ObjectToMap
    : public Object<TestDataObjectToMap>
    , public testing::WithParamInterface<ObjectToMapParam>
{
};

// Vector
// ///////////////////////////////////////////////////////////////////////////

using TestDataObjectToVector = std::vector<std::pair<std::string_view,TestDataObjectTo> >;

class ObjectToVectorParam : public ObjectParam<TestDataObjectToVector> {
public:

    Data dataPlain;
    Data dataFromMember;
    Data dataCheckNull;

    static std::vector<ObjectToVectorParam> cases;
};

/// convert map result into vector result.
inline TestDataObjectToVector
createVectorDataFromMapData(const TestDataObjectToMap &mapData) {
    TestDataObjectToVector result;

    if (mapData.empty()) {
        return result;
    }

    result = {
        {"test3", { 2, "Text 3" } },
        {"test2", { 1, "Text 2" } },
        {"test1", { 0, "Text 1" } }
    };

    return result;
}


// copy ObjectToMapParam::cases to ObjectToVectorParam::cases.
inline void
copyObjectToVectorCases() {
    ObjectToVectorParam::cases.reserve(ObjectToMapParam::cases.size());

    transform(
        ObjectToMapParam::cases.begin(), ObjectToMapParam::cases.end(),
        back_inserter(ObjectToVectorParam::cases),
        [](const auto &mapCase) {
            return
                ObjectToVectorParam(
                    {
                        {
                            mapCase.name,
                            mapCase.input,
                            mapCase.is_null
                        }, {
                            createVectorDataFromMapData(mapCase.dataPlain.result),
                            mapCase.dataPlain.trace,
                            mapCase.dataPlain.errors
                        }, {
                            createVectorDataFromMapData(mapCase.dataFromMember.result),
                            mapCase.dataFromMember.trace,
                            mapCase.dataFromMember.errors
                        }, {
                            createVectorDataFromMapData(mapCase.dataCheckNull.result),
                            mapCase.dataCheckNull.trace,
                            mapCase.dataCheckNull.errors
                        }
                    });
        });
}



class ObjectToVector
    : public Object<TestDataObjectToVector>
    , public testing::WithParamInterface<ObjectToVectorParam>
{
};


// Function
// ///////////////////////////////////////////////////////////////////////////

using TestDataObjectToFunction = std::map<std::string_view,TestDataObjectTo>;

class ObjectToFunctionParam : public ObjectParam<TestDataObjectToFunction> {
public:

    Data dataPlain;
    Data dataFromMember;
    Data dataCheckNull;

    static std::vector<ObjectToFunctionParam> cases;
};

class ObjectToFunction
    : public Object<TestDataObjectToFunction>
    , public testing::WithParamInterface<ObjectToFunctionParam>
{
};


// Generic Function
// ///////////////////////////////////////////////////////////////////////////

using TestDataObjectToGenericFn = std::map<std::string_view,TestDataObjectTo>;

class ObjectToGenericFnParam : public ObjectParam<TestDataObjectToGenericFn> {
public:

    Data dataPlain;
    Data dataFromMember;
    Data dataCheckNull;

    static std::vector<ObjectToGenericFnParam> cases;
};

class ObjectToGenericFn
    : public Object<TestDataObjectToGenericFn>
    , public testing::WithParamInterface<ObjectToGenericFnParam>
{
};


#endif /* SIMDJSON_PEVAL_TEST_OBJECT_TO_H */

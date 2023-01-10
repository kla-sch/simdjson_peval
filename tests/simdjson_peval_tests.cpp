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

#define _SIMDJSON_PEVAL_DO_TRACE 1

#include <iostream>
#include <set>
#include <unordered_set>
#include <string_view>

#include "simdjson_peval.h"

#include "gtest/gtest.h"

#include "TestUtils.h"

#include "ValueStringView.h"
#include "ValueString.h"
#include "ValueStringToInt64.h"
#include "ValueStringToUInt64.h"
#include "ValueStringToDouble.h"
#include "ValueInt64.h"
#include "ValueUInt64.h"
#include "ValueDouble.h"
#include "ValueBool.h"
#include "ValueFunction.h"

#include "ObjectPlain.h"
#include "ObjectMisc.h"
#include "ObjectDiscriminator.h"
#include "ObjectTo.h"

#include "ArrayPlain.h"
#include "ArrayMisc.h"
#include "ArrayTo.h"


_SIMDJSON_PEVAL_DEF_GLOBAL_TRACE_BUFFER;


// Test scalar values
// /////////////////////////////////////////////////////////////////////

INSTANTIATE_TEST_SUITE_P(
    all, ValueStringView,
    testing::ValuesIn(ValueStringViewParam::cases),
    sp_test::paramSetName<ValueStringViewParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ValueString,
    testing::ValuesIn(ValueStringParam::cases),
    sp_test::paramSetName<ValueStringParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ValueStringToInt64,
    testing::ValuesIn(ValueStringToInt64Param::cases),
    sp_test::paramSetName<ValueStringToInt64Param>);

INSTANTIATE_TEST_SUITE_P(
    all, ValueStringToUInt64,
    testing::ValuesIn(ValueStringToUInt64Param::cases),
    sp_test::paramSetName<ValueStringToUInt64Param>);

INSTANTIATE_TEST_SUITE_P(
    all, ValueStringToDouble,
    testing::ValuesIn(ValueStringToDoubleParam::cases),
    sp_test::paramSetName<ValueStringToDoubleParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ValueInt64,
    testing::ValuesIn(ValueInt64Param::cases),
    sp_test::paramSetName<ValueInt64Param>);

INSTANTIATE_TEST_SUITE_P(
    all, ValueUInt64,
    testing::ValuesIn(ValueUInt64Param::cases),
    sp_test::paramSetName<ValueUInt64Param>);

INSTANTIATE_TEST_SUITE_P(
    all, ValueDouble,
    testing::ValuesIn(ValueDoubleParam::cases),
    sp_test::paramSetName<ValueDoubleParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ValueBool,
    testing::ValuesIn(ValueBoolParam::cases),
    sp_test::paramSetName<ValueBoolParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ValueFunction,
    testing::ValuesIn(ValueFunctionParam::cases),
    sp_test::paramSetName<ValueFunctionParam>);

// Test Objects
// /////////////////////////////////////////////////////////////////////

INSTANTIATE_TEST_SUITE_P(
    all, ObjectPlain,
    testing::ValuesIn(ObjectPlainParam::cases),
    sp_test::paramSetName<ObjectPlainParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ObjectMisc,
    testing::ValuesIn(ObjectMiscParam::cases),
    sp_test::paramSetName<ObjectMiscParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ObjectDiscriminator,
    testing::ValuesIn(ObjectDiscriminatorParam::cases),
    sp_test::paramSetName<ObjectDiscriminatorParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ObjectToMap,
    testing::ValuesIn(ObjectToMapParam::cases),
    sp_test::paramSetName<ObjectToMapParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ObjectToVector,
    testing::ValuesIn(ObjectToVectorParam::cases),
    sp_test::paramSetName<ObjectToVectorParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ObjectToFunction,
    testing::ValuesIn(ObjectToFunctionParam::cases),
    sp_test::paramSetName<ObjectToFunctionParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ObjectToGenericFn,
    testing::ValuesIn(ObjectToGenericFnParam::cases),
    sp_test::paramSetName<ObjectToGenericFnParam>);

// Test Arrays
// /////////////////////////////////////////////////////////////////////

INSTANTIATE_TEST_SUITE_P(
    all, ArrayPlain,
    testing::ValuesIn(ArrayPlainParam::cases),
    sp_test::paramSetName<ArrayPlainParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ArrayMisc,
    testing::ValuesIn(ArrayMiscParam::cases),
    sp_test::paramSetName<ArrayMiscParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ArrayToSet,
    testing::ValuesIn(ArrayToSetParam::cases),
    sp_test::paramSetName<ArrayToSetParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ArrayToVector,
    testing::ValuesIn(ArrayToVectorParam::cases),
    sp_test::paramSetName<ArrayToVectorParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ArrayToFunction,
    testing::ValuesIn(ArrayToFunctionParam::cases),
    sp_test::paramSetName<ArrayToFunctionParam>);

INSTANTIATE_TEST_SUITE_P(
    all, ArrayToGenericFn,
    testing::ValuesIn(ArrayToGenericFnParam::cases),
    sp_test::paramSetName<ArrayToGenericFnParam>);


// Main Test Program
////////////////////////////////////////////////////////////////////////

int
main(int argc, char **argv) {

    // initialize test data
    copyObjectToVectorCases();
    copyArrayToVectorCases();

    // start tests
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

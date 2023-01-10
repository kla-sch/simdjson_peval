# User Guide for `simdjson_peval`

Table of contents:

* [Basics](#basics)
* [Evaluate Scalar Values](#evaluate-scalar-values)
* [Evaluate Generic Values](#evaluate-generic-values)
* [Evaluate Arrays](#evaluate-arrays)
* [Evaluate Array Elements](#evaluate-array-elements)
  * [Evaluate Array Elements To Function](#evaluate-array-elements-to-function)
  * [Evaluate Array Elements To Iterator](#evaluate-array-elements-to-iterator)
* [Evaluate Objects](#evaluate-objects)
* [Evaluate Object Members](#evaluate-object-members)
  * [Evaluate Object Members To Function](#evaluate-object-members-to-function)
  * [Evaluate Object Members To Iterator](#evaluate-object-members-to-iterator)
* [Error Handling](#error-handling)
  * [Handle Errors](#handle-errors)
  * [Save Errors](#save-errors)

## Basics

**Preliminary remark:** This API follows the
only-const-reference-as-argument policy. This means that a pointer is
always used as a reference. This has the advantage that when a
function is called, you can see that the content of the parameter can
be changed.  This policy is different from `simdjson`, which uses
non-const references as function argument.

The functions of `simdjson_peval.h` generate a function of type
`eval_fn_type<Type>` each. This is a function which expects a
`simdjson::simdjson_result<Type>` type aus first and a pointer to an
error container as secound argument. The function has no return value.

The default type is always `simdjson::ondemand::value`. If you want to
evaluate a different type, this must be specified in the functions as
the first template argument. This is the case, for example, if you
want to evaluate a JSON document.

Example:
```c++
    using namespace simdjson_peval;

    std::string my_string;

    // create evaluation function.
    // The template argument has to be simdjson::ondemand::document
    // to evaluate a document.
    auto eval_function =
        string_value<simdjson::ondemand::document>(&my_string);

    // parse document:
    simdjson::ondemand::parser parser;
    auto padded_json = simdjson::padded_string(json_data);
    auto data_doc = parser.iterate(padded_json);

    // execute evaluation function
    auto errors = error(true);
    eval_person(data_doc, &errors);
```

## Evaluate Scalar Values

The main functions to evaluate scalar values are:

* `string_value`: evaluates JSON string values
* `number_value`: evaluates JSON number values
* `bool_value`: evaluates JSON boolean values

All functions requires a pointer to a location where to store a value
as first argument.
Allowed location types:

* `string_value`:
  `std::string_view`, `std::string`, `int64_t`, `uint64_t`, `double`
* `number_value`:
  `int64_t`, `uint64_t`, `double`
* `bool_value`:
  `bool`

A pointer to a bool variable can be passed as the second argument. In
this case a `null` ist also acepted as value. The created evaluation
function writes a `true` into the bool variable, if the JSON value is
`null`. Otherwise it writes `false` into the bool variable.

Without the second argument, a `null` generates an error when evaluating
the JSON (see [Error Handling](#error-handling)).

For each c++ type there is a variant in which a pointer to a
`std::optional<Type>` can be specified as the storage location. In
this case, `null` values are always allowed and set the storage
location to `std::nullopt`.

This results in three variants to handle `null` values for each type:

* `jsontype_value(&cpp_type)`:
  A `null` generates an error
* `jsontype_value(&cpp_type, &bool_type)`:
  A `null` causes a `true` in `bool_type`
*  `jsontype_value(&optional_cpp_type)`:
  A `null` causes a `std::nullopt` in `optional_cpp_type`

Example:
```c++
    using namespace simdjson_peval;

    std::string_view mandatory;

    std::string_view optional1;
    bool optional1_is_null;

    std::optional<std::string_view> optional2;

    auto eval_json =
        object<simdjson::ondemand::document>(
            member("mandatory", string_value(&mandatory)),
            member("optional1", string_value(&optional1), &optional1_is_null),
            member("optional2", string_value(&optional2))
        );
```

## Evaluate Generic Values

The `value()` function is used to process general values so that they
can be processed using your own algorithms. They are also used by the
implementation to evaluate concrete values.

The first variant receives a pointer to a
`simdjson::simdjson_result<simdjson::ondemand::value>` as an
argument. It stores a `simdjson::ondemand::value` directly into the
specified variable. With other `simdjson` types, an attempt is made to
obtain a `simdjson::ondemand::value` using the `get_value()` method.

Example:
```c++
    using namespace simdjson_peval;

    simdjson::simdjson_result<simdjson::ondemand::value> my_value;

    auto eval_json =
        object<simdjson::ondemand::document>(
            member("avalue", value(&my_value))
        );
```

**Attention:** With a `simdjson::ondemand::document` a `get_value()`
only works for scalar values. In the case of arrays or objects, the
methods available there must be used to obtain a
`simdjson::simdjson_result<type>`!

The other variants of `value()` use functions to evaluate the value.

In the simplest variant, a function is specified, which receives a
pointer to the `simdjson::simdjson_result<type>` of the JSON parser
and a pointer to the error container as argument.

Example:

```c++
    using namespace simdjson_peval;

    auto store_value =
        [my_storage_fn](auto *sj_value, auto *err) {
            int64_t value;
            auto code = sj_value->get_int64().get(value);
            if (code) {
                err->add(code);
            }
            else {
                my_storage_fn(value);
            }
        };

    auto eval_json =
        object<simdjson::ondemand::document>(
            member("avalue", value(store_value))
        );
```

The other variants are used to convert values. You first get a pointer
to a storage location and then a conversion function that writes the
value to the location.

Example:
```c++
    using namespace simdjson_peval;

    auto conv_to_int =
        [](auto *sj_value, auto *int_ptr, auto *err) {
            int64_t value;

            auto code = sj_value->get_int64().get(value);
            if (code) {
                err->add(code);
            }
            else {
                // unsafe: lost precision
                *int_ptr = value;
            }
        };

    int my_val;
    auto eval_json =
        object<simdjson::ondemand::document>(
            member("avalue", value(&my_val, conv_to_int))
        );
```

Another variant can also handle a `null` like the functions for scalar
values.

Example:
```c++
    using namespace simdjson_peval;

    int my_val;
    bool my_val_is_null;
    auto eval_json =
        object<simdjson::ondemand::document>(
            member("avalue", value(&my_val, conv_to_int, &my_val_is_null))
        );
```

Function `conv_to_int` is not called if member `avalue` is `null`.

The last variant works with a `std::optional<>`a and saves a
`std::nullopt` for a `null`.

Example:
```c++
    using namespace simdjson_peval;

    std::optional<int> my_val;
    auto eval_json =
        object<simdjson::ondemand::document>(
            member("avalue", value(&my_val, conv_to_int))
        );
```

`my_val` is `std::nullopt`, if member `avalue` is
`null`. `conv_to_int` is not called in this case.

## Evaluate Arrays

The `array()` function is used to evaluate fixed values of an
array. To evaluate uniform values of an array,
see [Evaluate Array Elements](#evaluate-array-elements).

The first function variant of `array()` expect evaluaion functions for
`simdjson::ondemand::value` as arguments. Each of the functions passed
as an argument evaluates an element of the array. The values are
evaluated in the order in which they are specified.

Example:
```c++
    using namespace simdjson_peval;

    uint64_t id;
    std::string_view name;
    std::optional<std::string_view> descr;

    auto eval_json =
        array<simdjson::ondemand::document>(
            number_value(&id),
            string_value(&name),
            string_value(&descr)
        );

    // JSON OK: [4711, "Joe", "the hacker"]
    // JSON OK: [4711, "Joe", null]
    // JSON Error: [4711, "Joe"]
    // JSON Error: [4711, null, null]
    // JSON OK: [4711, "Joe", "the hacker", "50ct"]
```

Additional values are ignored. Missing values lead to an error
(`simdjson::INDEX_OUT_OF_BOUNDS`).

To allow a `null` as an array, a pointer to a `bool` can be specified
as the first argument.
Example:
```c++
    using namespace simdjson_peval;

    bool array_is_null;
    auto eval_json =
        array<simdjson::ondemand::document>(
            &array_is_null,
            number_value(&id),
            string_value(&name),
            string_value(&descr)
        );
```

The second variant expects a pointer to a
`simdjson::simdjson_result<simdjson::ondemand::array>` as an
argument. When evaluating an array, the value of the JSON parser is
copied.

Example:
```c++
    using namespace simdjson_peval;

    simdjson::simdjson_result<simdjson::ondemand::array> my_array;
    auto eval_json = array<simdjson::ondemand::document>(&my_array);
```

## Evaluate Array Elements

The functions `array_to_function()` and `array_to_out_iter()` are used
to evaluate arrays with uniform content.

### Evaluate Array Elements To Function

The functions `array_to_function()` calls a function for each element
of the array.

In the simplest variant, only one function is specified. This receives
the index of the element, a pointer to the
`simdjson::simdjson_result<simdjson::ondemand::value>` of the element
and a pointer to the error container as parameters.

Example:
```c++
    using namespace simdjson_peval;

    auto store_value =
        [display_value](auto idx, auto *sj_value, auto *err) {
            double value;
            const auto code = sj_value.get_double().get(value);
            if (code) {
                err->add(code);
            }
            display_value(std::to_string(idx) + ": " + std::to_string(value));
        };

    auto eval_json =
        array_to_function<simdjson::ondemand::document>(store_value);
```

If the first argument is a pointer to a bool value, the array can also
be `null`. The corresponding status is written to the bool value.

Example:
```c++
    using namespace simdjson_peval;

    bool array_is_null;
    auto eval_json =
        array_to_function<simdjson::ondemand::document>(
            &array_is_null, store_value);
```

In another variant, a temporary storage and an evaluation function are
specified after the function. For each element of the array, the
evaluation function is called first, which is supposed to fill the
temporary storage. Then the function of the first parameter is called
with a pointer to the temporary memory and the error container to
process the value.

This variant also supports a pointer to a bool as the additional parameter
to handle `null` instead of the array.

Example:
```c++
    using namespace simdjson_peval;

    struct Storage {
        uint64_t id;
        std::string name;
    };

    auto store_value =
        [save_fn](auto *storage, auto *err) {
            if(!save_fn(*storage)) {
                // what ever
                arr->add(simdjson::TAPE_ERROR);
            }
        };

    // Temporary storage to store more complex array content.
    Storage tmp_storage;

    // Optional:
    // bool array_is_null;

    auto eval_json =
        array_to_function<simdjson::ondemand::document>(
            // optional add this as first parameter:
            // &array_is_null
            store_value, &tmp_storage,
            object(
                member("id", number_value(&tmp_storage.id)),
                member("name", string_value(&tmp_storage.name))
            )
        );
```

### Evaluate Array Elements To Iterator

The functions `array_to_out_iter()` stores the elements into an output
iterator. The function expects an output iterator, a pointer to a
temporary storage and an evaluation function, which should fill the
temporary storage, as arguments.

A second variant supports a pointer to a bool as the first parameter
to handle `null` instead of the array.

The generated function calls the evaluation function for each element
to fill the temporary storage. Then it moves the value into the output
iterator.

Example:
```c++
    using namespace simdjson_peval;

    struct Storage {
        uint64_t id;
        std::string name;
    };

    // Temporary storage to store more complex array content.
    Storage tmp_storage;

    // Result vector
    std::vector<Storage> my_values;

    // Optional:
    // bool array_is_null;

    auto eval_json =
        array_to_out_iter<simdjson::ondemand::document>(
            // optional add this as first parameter:
            // &array_is_null
            back_inserter(my_values), &tmp_storage,
            object(
                member("id", number_value(&tmp_storage.id)),
                member("name", string_value(&tmp_storage.name))
            )
        );
```

This also works with more deeply nested structures. For example, if
the element also contains an array from which values are to be
extracted.

Example:
```c++
    using namespace simdjson_peval;

    struct Storage {
        uint64_t id;
        std::string name;

        bool tags_is_null;
        std::vector<std::string> tag_names;
    };

    // Temporary storage to store more complex array content.
    Storage tmp_storage;

    // Temporary storage for a tag name
    std::string tmp_tag_name;

    // Result vector
    std::vector<Storage> my_values;

    auto eval_json =
        array_to_out_iter<simdjson::ondemand::document>(
            back_inserter(my_values), &tmp_storage,
            object(
                member("id", number_value(&tmp_storage.id)),
                member("name", string_value(&tmp_storage.name)),
                member("tags",
                    array(
                        &tmp_storage.tags_is_null,
                        back_inserter(&tmp_storage.tag_ids), &tmp_tag_name,
                        object(
                            member("id", string_value(&tmp_tag_name))
                        )
                    )
                )
            )
        );
```

## Evaluate Objects

The functions `object()` evaluates named members of an object or the
object itself.

The variant with a pointer to a
`simdjson::simdjson_result<simdjson::ondemand::object>` writes the
object itself into a variable.

Example:
```c++
    using namespace simdjson_peval;

    simdjson::simdjson_result<simdjson::ondemand::object> my_obj;

    auto eval_json =
        object<simdjson::ondemand::document>(&my_obj);
```

The main variant expect multiple `member()` or `discriminator()`
functions as argument. These functions were evaluated in argument
order.

A pointer to a bool can be placed in front of the functions. In that
case, this variable will be set to `true` if the object itself is
`null`.

The `member()` calls get the name of the object member, an evaluation
function and optional a pointer of bool as agument. If a member with
the name exists, the evaluate function is called to evaluate the
value. If the member is missing, an error is generated ().

If a pointer to a bool value was specified as the last parameter, then
the member is optional. If the member is missing, `true` is written in
the bool variable and otherwise `false`.

Example
```c++
    using namespace simdjson_peval;

    uint64_t id;
    std::string name;
    std::string description;
    bool description_undef;

    // optional flag: object itself is null
    // bool object_is_null;

    auto eval_json =
        object<simdjson::ondemand::document>(
            // optional: &object_is_null
            member("id", number_value(&id)),
            member("name", string_value(&name)),
            member("description", string(&description), &description_undef)
        );

```

In addition to the `member()` functions, `discriminator()` functions can
also be specified, which decide whether further members have to be
evaluated.

A `discriminator()` function gets a function and multiple `member()`
or `discriminator()` functions as argument.

The first function get a pointer to the
`simdjson::simdjson_result<simdjson::ondemand::object>` and a pointer
to the error container as argument. A boolean value must be returned
which decides whether the contained members have to be evaluated.

Example:
```c++
    using namespace simdjson_peval;

    struct Person {
        uint64_t id;
        std::string name;
        std:.string type;
        std::optional<std::string> grade;
        std::optional<std::string> room;
    };

    // result object
    Person person;

    auto is_student =
        [&person](auto *sj_object, auto *err) {
            // Decide by value from capture.
            // The member "type" has to be evaluated by member() function
            // before the discriminator use this function.
            return (person.type == "student");
        };

    auto is_teacher =
        [&person](auto *sj_object, auto *err) {
            // Decide by object value. No member() with this value needed.
            auto sj_type = js_object->find_field_unordered("type");
            if (sj_type.error()) {
                err->add(sj_type.error());
            }

            std::string_view type;
            sj_type.get(type);

            return (type == "student");
        };


    auto eval_json =
        object<simdjson::ondemand::document>(
            // optional: &object_is_null
            member("id", number_value(&person.id)),
            member("name", string_value(&person.name)),
            member("type", string_value(&person.type)), // filled before discriminators
            discriminator(
                is_student,
                member("grade", string_value(&person.grade))
            ),
            discriminator(
                is_teacher,
                member("room", string_value(&person.room))
            )
        );

```

## Evaluate Object Members

### Evaluate Object Members To Function

The functions `object_to_function()` calls a function for each member
of an object.

In the simplest variant, only one function is specified. This receives
the name of the member, a pointer to the
`simdjson::simdjson_result<simdjson::ondemand::value>` of the value
and a pointer to the error container as parameters.

Example:
```c++
    using namespace simdjson_peval;

    auto store_value =
        [display_value](auto member_name, auto *sj_value, auto *err) {
            if (member_name == "id") {
                uint64_t value;

                const auto code = sj_value.get_uint64().get(value);
                if (code) {
                    err->add(code);
                }

                display_value(
                    std::string(name) + ": " + std::to_string(value));
            }
            else if (member_name == "name") {
                std::string_view value;

                const auto code = sj_value.get_string().get(value);
                if (code) {
                    err->add(code);
                }

                display_value(
                    std::string(name) + ": " + std::string(value));
            }
        };

    auto eval_json =
        object_to_function<simdjson::ondemand::document>(store_value);
```

If the first argument is a pointer to a bool value, the object can also
be `null`. The corresponding status is written to the bool value.

Example:
```c++
    using namespace simdjson_peval;

    bool object_is_null;
    auto eval_json =
        object_to_function<simdjson::ondemand::document>(
            &object_is_null, store_value);
```

In another variant, a temporary storage and an evaluation function are
specified after the function. The temporary storage has to be a
``std::pair<std::string_view,ValueType>`. For each member of the object,
the evaluation function is called first, which is supposed to fill the
`second` member of the temporary storage. The `first` member is filled
with the name of the member. Then the function of the first parameter
is called with a pointer to the temporary memory and the error
container to process the value.

This variant also supports a pointer to a bool as additional first parameter
to handle `null` instead of the object.

Example:
```c++
    using namespace simdjson_peval;

    struct Storage {
        uint64_t id;
        std::string name;
    };

    auto store_value =
        [save_fn](auto *pair_ptr, auto *err) {
            if(!save_fn(pair_ptr->first, pair_ptr->second)) {
                // what ever
                arr->add(simdjson::TAPE_ERROR);
            }
        };

    // Temporary storage to store store member data.
    std::pair<std::string_view, Storage> tmp_storage;

    // Optional:
    // bool object_is_null;

    auto eval_json =
        object_to_function<simdjson::ondemand::document>(
            // optional add this as first parameter:
            // &object_is_null,
            store_value, &tmp_storage,
            object(
                member("id", number_value(&tmp_storage.second.id)),
                member("name", string_value(&tmp_storage.second.name))
            )
        );
```

### Evaluate Object Members To Iterator

The functions `object_to_out_iter()` stores the members into an output
iterator. The function expects an output iterator, a pointer to a
temporary storage and an evaluation function, which should fill the
temporary storage, as arguments.

The output iterator has to accept a
`std::pair<std::string,ValueType>`. The temporary memory must also be
of this type.

A second variant supports a pointer to a bool as the first parameter
to handle `null` instead of the object.

The generated function calls the evaluate function on each member to
fill the temporary storage. The `first` member of the `std::pair<>` is
filled with the name of the JSON member and the `second` member is to be
filled by the member evaluator function. After the evaluation function
is called, the buffer is moved to the output iterator.

Example:
```c++
    using namespace simdjson_peval;

    // Temporary storage to store std::pair<>.
    std::pair<std::string, std::string> tmp_storage;

    // store it into a map,
    std::map<std::string, std::string_view> my_values;

    // Optional:
    // bool object_is_null;

    auto eval_json =
        object_to_out_iter<simdjson::ondemand::document>(
            // optional add this as first parameter:
            // &object_is_null
            inserter(my_values, my_values.end()), &tmp_storage,
            string_value(&tmp_storage.second)
        );
```

This also works with more deeply nested structures. For example, if
the members contains an array as value.

Example:
```c++
    using namespace simdjson_peval;

    using HashTags = std::vector<std::string>;

    // map to store all values
    std::map<std::string, HashTags> my_values;

    // Temporary storage for new map entry
    std::pair<std::string, HashTags> tmp_storage;

    // Temporary storage for a tag name
    std::string tmp_tag_name;

    bool tag_is_null; // just ignore `null` values

    auto eval_json =
        object_to_out_iter<simdjson::ondemand::document>(
            inserter(my_values, my_values.end()), &tmp_storage,
            array_to_out_iter(
                &tags_is_null,
                back_inserter(&tmp_storage.second), &tmp_tag_name,
                string_value(&tmp_tag_name)
            )
        );

    auto json = simdjson::padded_string(R"(
        {
            "student": ["has_grade", "has_depts", "is_young"],
            "teacher": ["has_room", "has_salery"]
            "person": null
        }
    )");
```

## Error Handling

### Handle Errors

All evaluation functions generated use `simdjson` in such a way that
no exceptions occur. instead, the errors are collected in an error
container of the `error` class. The evaluation function must be given
a pointer to an instance of the class as the second parameter.

Example:
```c++
    using namespace simdjson_peval;

    auto eval_function =
        object<simdjson::ondemand::document>(
            member("result", ...),
            ...
        );
    ...

    // parse document:
    simdjson::ondemand::parser parser;
    auto data_doc = parser.iterate(padded_json);

    // execute evaluation function with pointer to an error class.
    auto errors = error(true);
    eval_function(data_doc, &errors);

    if (error) {
        // handle errors
        ...
    }
```

The constructor of `error` takes a boolean value that controls whether
an error message path is created. The path makes it easier to find the
point at which the evaluation of the data resulted in an
error. However, the path generation costs performance during the
evaluation of the JSON.  Therefore it is recommended to create the
container with `false` except for debug purposes. `false` is also the
default parameter of the class constructor.

The `error` container has an `operator bool()` which returns `true` if
there are errors in the container.

The container contains a vector with instances of the error_message
class, each containing the error code of the `simdjson`
implementation. If path generation is switched on, the path is also
kept at the point in the JSON where the error occurred.

Example:
```c++
    using namespace simdjson_peval;

    ...

    // execute evaluation function with pointer to an error class.
    auto errors = error(true);
    eval_function(data_doc, &errors);

    if (error) {
        for (const auto &msg : error.get_messages()) {
            if (error.is_path_enabled()) {
                std::cerr << "at " << msg.get_path();
            }
            std::cerr << msg.get_text()
                      << " (code: " << msg.get_code() << ")\n";
        }
    }
```

The method `error_message::get_path()` simply calls
`simdjson::error_message()` to get a text of the error code. It is a
shortcut for `simdjson::error_message(msg.get_code())` get_code());

The `error_message::to_string()` method produces a string containing
the path (if enabled), the error message and the error code in one
line.

Example:
```
at <root>.result[5].name: The JSON field referenced does not exist in this object. (19)
```
or just
```
The JSON field referenced does not exist in this object. (19)
```

The `error` class itself also defines a `to_string()` method which
creates a string with all error messages separated and ended by newline.

Example:
```
at <root>.result[5].name: The JSON field referenced does not exist in this object. (19)
at <root>.result[5].type: The JSON field referenced does not exist in this object. (19)

```
or
```
The JSON field referenced does not exist in this object. (19)
The JSON field referenced does not exist in this object. (19)
```

### Save Errors

It is also possible to evaluate only partial documents using the
evaluation functions generated by the prototypes. Then other parts are
processed directly by the `simdjson::ondemand` functions. The
resulting errors can also be stored in the `error` container (see
[Handle Errors](#handle-errors)) for later processing using the
`add()` method. This method simply receives a `simdjson::error_code` as a
parameter.

The subclass `error::path_scope` can be used for path creation. This
needs a pointer to an error container and either a name of an object
member or the index of an array. These are registered as a new path
level in the error container. If enabled, all errors added using
`add()` are now marked with the extended path. the destructor of the
class `error::path_scope` removes the path layer again when the scope
is left.

Example:
```c++
    using namespace simdjson_peval;

    ...

    auto eval_result_array =
        array(
            ...
        );

    auto data_doc = parser.iterate(padded_json);

    auto errors = error(true);

    auto sj_res = data_doc.get_object().find_field_unordered("result");
    if (sj_res.error()) {
        errors.add(sj_res.error());
    }
    else  {
        // All further errors will be registered as "<root>.result"
        error::path_scope(&error, "result");

        eval_result_array(sj_res, &errors);

        // Path level is reduced at end of scope.
    }
    // Path is "<root>" again.

```

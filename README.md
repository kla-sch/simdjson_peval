# Evaluate JSON data by prototype

[Based on simdjson library](https://simdjson.org/).

The basic idea is to create a prototype that follows the structure of
the JSON data. The prototype is then used to evaluate the data from the
JSON.

The implementation is based on
[simdjson](https://github.com/simdjson/simdjson#readme) as a parser
for the JSON und make use of **c++17** language features.

This is an early pre-release to stabilize the code and for suggestions.

Excample:
```c++
    using namespace simdjson_peval;

    // the json so evaluate
    auto json_data =
    R"(
        [
            {
                "id": 100,
                "name": "Peter"
            }, {
                "id": 101,
                "name": "Marry"
            }, {
                "id": 102,
                "name": "Josephin"
            }
        ]
    )";

    // structure to store records
    struct Person {
        uint64_t id,
        std::string_view name
    };

    // temporary record for array evaluation
    Person tmp_person;

    // container to store the data
    std::vector<Person> persons;

   // create prototype function to store data into `persons`
    auto eval_person =
        array_to_out_iter<simdjson::ondemand::document>(
            back_inserter(persons), &tmp_person,
            object(
                member("id", number_value(&tmp_person.id)),
                member("name", string_value(&tmp_person.name))
            )
        );

    // parse document:
    simdjson::ondemand::parser parser;
    auto padded_json = simdjson::padded_string(json_data);
    auto data_documant = parser.iterate(padded_json);

    // use the prototype to store data into vector `persons`
    auto errors = error(true);
    eval_person(data_documant, &errors);

    if (error) {
        std::cerr << "Error:\n" << errors.to_string() << '\n';
        return 1;
    }

```

## Installation And Requirements

To install you have to just copy the header `include/simdjson_peval.h`
into your include path.

It requeires an installed `simdjson` in your include path to
work.
Also a c++ compiler which supports at least c++17 language
features. It has been tested with the following compilers:

* clang version 11.0.1
* gcc version 10.2.1

## Documentation

The implementation is described in the
[User Guide](https://github.com/kla-sch/simdjson_peval/blob/main/doc/UserGuide.md).
             
A API documentation could be created by just calling `docygen` in
folder `doc`.

## Performance

I just made a test with the example file `twitter.json` from the
`simdjson` project. Once the data is evaluated straight forward with
the `simdjson` methods and once with the prototypes.

Results vary by compiler.

**clang version 11.0.1**:
```
----------------------------------------------------------------------
Benchmark                            Time             CPU   Iterations
----------------------------------------------------------------------
twitter_file_peval_proto        165033 ns       165025 ns         4196
twitter_file_raw                155859 ns       155850 ns         4584
```

**gcc version 10.2.1**
```
----------------------------------------------------------------------
Benchmark                            Time             CPU   Iterations
----------------------------------------------------------------------
twitter_file_peval_proto        163140 ns       163134 ns         4448
twitter_file_raw                173362 ns       173355 ns         4079
```

The Clang result seems reasonable. An abstraction always costs some
performance. In this case, the evaluation takes about 6% longer.

But I don't understand why the GCC seems to be faster with the
prototype than when evaluating the JSON straight forward.

Further performance tests are necessary in order to get a final
picture here.

## TODO for v1.0

* Further performance testing required
* Improve API
  * add more types for evaluation
  * Optional array elements
  * `null_value(bool*)` to test for null/not null elements
* Fix bugs

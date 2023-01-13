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

I made three different tests. The performance is almost the same as
equaivalent implemented straight forward implementations.

Further performance tests are necessary in order to get a final
picture here.

### Twitter example

Test use the example file `twitter.json` from the `simdjson`
project. Once the data is evaluated straight forward with the
`simdjson` methods and once with the prototypes.

**clang version 11.0.1**:
```
---------------------------------------------------------------------------
Benchmark                                 Time             CPU   Iterations
---------------------------------------------------------------------------
twitter_file_peval_local_median      155214 ns       155208 ns           10
twitter_file_raw_median              150163 ns       150155 ns           10
```
Result: 3.6% performance loss

**gcc version 10.2.1**
```
---------------------------------------------------------------------------
Benchmark                                 Time             CPU   Iterations
---------------------------------------------------------------------------
twitter_file_peval_local_median      144205 ns       143815 ns           10
twitter_file_raw_median              168965 ns       168542 ns           10
```
Result: 14.7% performance gain


### Cellphone example

Test use the example file `amazon_cellphones.ndjson` from the
`simdjson` project. Once the data is evaluated straight forward with
the `simdjson` methods and once with the prototypes.

**clang version 11.0.1**:
```
-----------------------------------------------------------------------------
Benchmark                                   Time             CPU   Iterations
-----------------------------------------------------------------------------
cellphone_file_peval_local_median      285019 ns       284761 ns           10
cellphone_file_raw_median              278128 ns       278114 ns           10
```
Result: 2.5% performance loss

**gcc version 10.2.1**
```
-----------------------------------------------------------------------------
Benchmark                                   Time             CPU   Iterations
-----------------------------------------------------------------------------
cellphone_file_peval_local_median      273892 ns       273874 ns           10
cellphone_file_raw_median              249466 ns       249450 ns           10
```
Result: 8.9% performance gain

### Iterate throw map elements

Parse an object with a numerous number of members with
numbers. Iterates throw the object members. Once the data is evaluated
straight forward with the `simdjson` methods and once with the
prototypes.

**clang version 11.0.1**:
```
-----------------------------------------------------------------------------
Benchmark                                   Time             CPU   Iterations
-----------------------------------------------------------------------------
map_data_peval_local/65536_median     1027787 ns      1027711 ns           10
map_data_raw/65536_median             1155857 ns      1154326 ns           10
```
Result: 11.1% performance gain

**gcc version 10.2.1**
```
-----------------------------------------------------------------------------
Benchmark                                   Time             CPU   Iterations
-----------------------------------------------------------------------------
map_data_peval_local/65536_median     1043462 ns      1037913 ns           10
map_data_raw/65536_median             1081108 ns      1081030 ns           10
```
Result: 3.6% performance gain

## TODO for v1.0

* Further performance testing required
* Improve API
  * add more types for evaluation
  * Optional array elements
  * `null_value(bool*)` to test for null/not null elements
* Fix bugs

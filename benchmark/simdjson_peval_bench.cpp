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

#include <benchmark/benchmark.h>

#include "simdjson_peval.h"

#include <iostream>

// Debug: write error messages with path
static const bool ERRORS_WITH_PATH = false;

// Profiling: just call test function
#ifndef DO_PROFILE_TWITTER
#    define DO_PROFILE_TWITTER 0
#endif

#ifndef DO_PROFILE_PHONE
#    define DO_PROFILE_PHONE 0
#endif

#ifndef DO_PROFILE_MAP_DATA
#    define DO_PROFILE_MAP_DATA 0
#endif


static simdjson::ondemand::parser parser;
static simdjson::padded_string json_data;

#if defined(__clang__) || defined(__GNUC__)
//   Optimize: always include lambda functions.
#    define ALWAYS_INLINE __attribute__((always_inline))
#else
#    define ALWAYS_INLINE
#endif


// Tweet example
// /////////////////////////////////////////////////////////////////////////

// Structure to hold data of a tweet.
struct Tweet {
    uint64_t id;
    std::string_view text;
    std::string_view name;
    std::string_view screen_name;
    std::vector<std::string_view> hashtags;
};

static Tweet stmp_tweet;
static std::string_view stmp_hash;
static std::vector<Tweet> stweets;

static auto eval_tweet =
    simdjson_peval::object<simdjson::ondemand::document>(
        simdjson_peval::member(
            "statuses",
            simdjson_peval::array_to_out_iter(
                back_inserter(stweets), &stmp_tweet,
                simdjson_peval::object(
                    simdjson_peval::member(
                        "id",
                        simdjson_peval::number_value(&stmp_tweet.id)),
                    simdjson_peval::member(
                        "text",
                        simdjson_peval::string_value(&stmp_tweet.text)),
                    simdjson_peval::member(
                        "user",
                        simdjson_peval::object(
                            simdjson_peval::member(
                                "name",
                                simdjson_peval::string_value(
                                    &stmp_tweet.name)),
                            simdjson_peval::member(
                                "screen_name",
                                simdjson_peval::string_value(
                                    &stmp_tweet.screen_name))
                            )),
                    simdjson_peval::member(
                        "entities",
                        simdjson_peval::object(
                            simdjson_peval::member(
                                "hashtags",
                                simdjson_peval::array_to_out_iter(
                                    back_inserter(stmp_tweet.hashtags),
                                    &stmp_hash,
                                    simdjson_peval::object(
                                        simdjson_peval::member(
                                            "text",
                                            simdjson_peval::string_value(
                                                &stmp_hash))))))
                        )
                    )
                )
            )
        );


static void
setupTwitterLoadJSON() {
    json_data = simdjson::padded_string::load(TWITTER_EXAMPLE);
}

static void
setupTwitterLoadJSON(const benchmark::State& state) {
    setupTwitterLoadJSON();
}


static std::vector<Tweet>
loadTwitter_peval_local() {
    using namespace simdjson_peval;
    auto data_document = parser.iterate(json_data);

    Tweet tmp_tweet;
    std::string_view tmp_hash;

    std::vector<Tweet> tweets;
    tweets.reserve(1<<10);

    auto eval_fn =
        object<simdjson::ondemand::document>(
            member(
                "statuses",
                array_to_out_iter(
                    back_inserter(tweets), &tmp_tweet,
                    object(
                        member(
                            "id", number_value(&tmp_tweet.id)),
                        member(
                            "text", string_value(&tmp_tweet.text)),
                        member(
                            "user",
                            object(
                                member(
                                    "name",
                                    string_value(&tmp_tweet.name)),
                                member(
                                    "screen_name",
                                    string_value(&tmp_tweet.screen_name))
                                )),
                        member(
                            "entities",
                            object(
                                member(
                                    "hashtags",
                                    array_to_out_iter(
                                        back_inserter(tmp_tweet.hashtags),
                                        &tmp_hash,
                                        object(
                                            member(
                                                "text",
                                                string_value(&tmp_hash))))))
                            )
                        )
                    )
                )
            );

    auto errors = error(ERRORS_WITH_PATH);
    eval_fn(data_document, &errors);

    if (errors) {
        std::cerr << "ERROR:\n" << errors.to_string() << '\n';
        exit(1);
    }

    return tweets;
}


static std::vector<Tweet>
loadTwitter_peval_global() {
    using namespace simdjson_peval;
    auto data_document = parser.iterate(json_data);

    stweets.clear();
    stweets.reserve(1<<10);

    auto errors = error(ERRORS_WITH_PATH);
    eval_tweet(data_document, &errors);

    if (errors) {
        std::cerr << "ERROR:\n" << errors.to_string() << '\n';
        exit(1);
    }

    return std::move(stweets);
}



static std::vector<Tweet>
loadTwitter_raw() {
    using namespace simdjson_peval;
    auto data_document = parser.iterate(json_data);

    simdjson::ondemand::value sj_statuses;
    auto code = data_document.find_field_unordered("statuses").get(sj_statuses);
    if (code) {
        std::cerr
            << "data_document[\"statuses\"]: "
            << simdjson::error_message(code)
            << '\n';
        exit(1);
    }

    simdjson::ondemand::array sj_statuses_array;
    code = sj_statuses.get_array().get(sj_statuses_array);
    if (code) {
        std::cerr
            << "data_document[\"statuses\"].get_array(): "
            << simdjson::error_message(code)
            << '\n';
        exit(1);
    }

    std::vector<Tweet> tweets;
    tweets.reserve(1<<10);

    int idx = 0;
    for (auto sj_elem : sj_statuses_array) {
        Tweet tmp_tweet;

        simdjson::ondemand::object sj_elem_obj;
        code = sj_elem.get_object().get(sj_elem_obj);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value sj_id;
        code = sj_elem_obj.find_field_unordered("id").get(sj_id);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"id\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        code = sj_id.get_uint64().get(tmp_tweet.id);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"id\"]"
                << ".get_uint64()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value sj_text;
        code = sj_elem_obj.find_field_unordered("text").get(sj_text);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"text\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        code = sj_text.get_string().get(tmp_tweet.text);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"text\"]"
                << ".get_string()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value sj_user;
        code = sj_elem_obj.find_field_unordered("user").get(sj_user);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"user\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::object sj_user_obj;
        code = sj_user.get_object().get(sj_user_obj);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"user\"]"
                << ".get_object()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value sj_name;
        code = sj_user_obj.find_field_unordered("name").get(sj_name);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"user\"]"
                << "[\"name\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        code = sj_name.get_string().get(tmp_tweet.name);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"user\"]"
                << "[\"name\"]"
                << ".get_string()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value sj_screen_name;
        code =
            sj_user_obj.find_field_unordered("screen_name")
            .get(sj_screen_name);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"user\"]"
                << "[\"screen_name\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        code = sj_screen_name.get_string().get(tmp_tweet.screen_name);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"user\"]"
                << "[\"screen_name\"]"
                << ".get_string()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value sj_entities;
        code = sj_elem_obj.find_field_unordered("entities").get(sj_entities);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"entities\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::object sj_entities_obj;
        code = sj_entities.get_object().get(sj_entities_obj);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"entities\"]"
                << ".get_object()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value sj_hashtags;
        code =
            sj_entities_obj.find_field_unordered("hashtags")
            .get(sj_hashtags);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"entities\"]"
                << "[\"hashtags\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::array sj_hashtags_array;
        code = sj_hashtags.get_array().get(sj_hashtags_array);
        if (code) {
            std::cerr
                << "data_document[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"entities\"]"
                << "[\"hashtags\"]"
                << ".get_array()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        int hidx = 0;
        for (auto sj_htag : sj_hashtags_array) {
            simdjson::ondemand::object sj_htag_obj;
            code = sj_htag.get_object().get(sj_htag_obj);
            if (code) {
                std::cerr
                    << "data_document[\"statuses\"]"
                    << '[' << idx<< ']'
                    << "[\"entities\"]"
                    << "[\"hashtags\"]"
                    << '[' << hidx<< ']'
                    << ": " << simdjson::error_message(code)
                    << '\n';
                exit(1);
            }

            simdjson::ondemand::value sj_htag_text;
            code = sj_htag_obj.find_field_unordered("text").get(sj_htag_text);
            if (code) {
                std::cerr
                    << "data_document[\"statuses\"]"
                    << '[' << idx<< ']'
                    << "[\"entities\"]"
                    << "[\"hashtags\"]"
                    << '[' << hidx<< ']'
                    << "[\"text\"]"
                    << ": " << simdjson::error_message(code)
                    << '\n';
                exit(1);
            }

            std::string_view tmp_hash;
            code = sj_htag_text.get_string().get(tmp_hash);
            if (code) {
                std::cerr
                    << "data_document[\"statuses\"]"
                    << '[' << idx<< ']'
                    << "[\"entities\"]"
                    << "[\"hashtags\"]"
                    << '[' << hidx<< ']'
                    << "[\"text\"]"
                    << ".get_string()"
                    << ": " << simdjson::error_message(code)
                    << '\n';
                exit(1);
            }

            tmp_tweet.hashtags.push_back(std::move(tmp_hash));

            ++hidx;
        }

        tweets.push_back(std::move(tmp_tweet));

        ++idx;
    }

    return tweets;
}


static void
twitter_file_peval_local(benchmark::State& state) {
    for (auto _ : state) {
        auto tweets = loadTwitter_peval_local();
        benchmark::DoNotOptimize(tweets.data());
    }
}


static void
twitter_file_peval_global(benchmark::State& state) {
    for (auto _ : state) {
        auto tweets = loadTwitter_peval_global();
        benchmark::DoNotOptimize(tweets.data());
    }
}


static void
twitter_file_raw(benchmark::State& state) {
    for (auto _ : state) {
        auto tweets = loadTwitter_raw();
        benchmark::DoNotOptimize(tweets.data());
    }
}

BENCHMARK(twitter_file_peval_local)
->Setup(setupTwitterLoadJSON);

BENCHMARK(twitter_file_peval_global)
->Setup(setupTwitterLoadJSON);

BENCHMARK(twitter_file_raw)
->Setup(setupTwitterLoadJSON);


// Amazon cellphone example
// /////////////////////////////////////////////////////////////////////////

// Structure to hold data of a cellphone.
struct Cellphone {
    std::string asin;
    std::string brand;
    std::string title;
    std::string url;
    std::string image;
    double rating;
    std::string reviewUrl;
    uint64_t totalReviews;
    std::string prices;
};

static Cellphone stmp_cellphone;

static auto eval_cellphone =
    simdjson_peval::array<simdjson::ondemand::document_reference>(
        simdjson_peval::string_value(&stmp_cellphone.asin),
        simdjson_peval::string_value(&stmp_cellphone.brand),
        simdjson_peval::string_value(&stmp_cellphone.title),
        simdjson_peval::string_value(&stmp_cellphone.url),
        simdjson_peval::string_value(&stmp_cellphone.image),
        simdjson_peval::number_value(&stmp_cellphone.rating),
        simdjson_peval::string_value(&stmp_cellphone.reviewUrl),
        simdjson_peval::number_value(&stmp_cellphone.totalReviews),
        simdjson_peval::string_value(&stmp_cellphone.prices));


static void
setupCellphoneLoadJSON() {
    json_data = simdjson::padded_string::load(AMAZON_EXAMPLE);
}

static void
setupCellphoneLoadJSON(const benchmark::State& state) {
    setupCellphoneLoadJSON();
}


static std::vector<Cellphone>
loadCellphones_peval_local() {
    using namespace simdjson_peval;

    std::vector<Cellphone> result;
    result.reserve(1<<10);

    auto data_docs = parser.iterate_many(json_data);

    size_t idx = 0;

    auto eval_fn =
        array<simdjson::ondemand::document_reference>(
        string_value(&stmp_cellphone.asin),
        string_value(&stmp_cellphone.brand),
        string_value(&stmp_cellphone.title),
        string_value(&stmp_cellphone.url),
        string_value(&stmp_cellphone.image),
        number_value(&stmp_cellphone.rating),
        string_value(&stmp_cellphone.reviewUrl),
        number_value(&stmp_cellphone.totalReviews),
        string_value(&stmp_cellphone.prices));

    auto errors = error(ERRORS_WITH_PATH);

    for (auto doc_ref : data_docs.value()) {
        error::path_scope doc_scope(&errors, idx);
        if (idx != 0) {
            eval_fn(doc_ref, &errors);
            if (!errors) {
                result.push_back(std::move(stmp_cellphone));
            }
        }
        ++idx;
    }

    return result;
}


static std::vector<Cellphone>
loadCellphones_peval_global() {
    using namespace simdjson_peval;

    std::vector<Cellphone> result;
    result.reserve(1<<10);

    auto data_docs = parser.iterate_many(json_data);

    size_t idx = 0;

    auto errors = error(ERRORS_WITH_PATH);

    for (auto doc_ref : data_docs.value()) {
        error::path_scope doc_scope(&errors, idx);
        if (idx != 0) {
            eval_cellphone(doc_ref, &errors);
            if (!errors) {
                result.push_back(std::move(stmp_cellphone));
            }
        }
        ++idx;
    }

    return result;
}


static std::vector<Cellphone>
loadCellphones_raw() {
    using namespace simdjson_peval;

    std::vector<Cellphone> result;
    result.reserve(1<<10);

    auto data_docs = parser.iterate_many(json_data);

    size_t idx = 0;
    for (auto doc_ref : data_docs.value()) {

        if (idx != 0) {
            auto code = doc_ref.error();
            if (code)  {
                std::cerr << "data_docs[" << idx << "]: "
                << simdjson::error_message(code)
                << '\n';
                exit(1);
            }

            auto sj_array = doc_ref.get_array();
            code = sj_array.error();
            if (code)  {
                std::cerr << "data_docs[" << idx << "]: "
                          << simdjson::error_message(code)
                          << '\n';
                exit(1);
            }

            auto aIter = sj_array.begin();

            auto collect_string =
                [&sj_array, &aIter, &idx]
                (auto pIdx, auto *ref) ALWAYS_INLINE {
                    if (aIter == sj_array.end()) {
                        std::cerr << "data_docs[" << idx << "]"
                                  << "[" << pIdx << "]: "
                                  << "end of array\n";
                        exit(1);
                    }

                    std::string_view value;
                    auto code = (*aIter).get_string().get(value);
                    if (code)  {
                        std::cerr << "data_docs[" << idx << "]"
                                  << "[" << pIdx << "]: "
                                  << simdjson::error_message(code)
                                  << '\n';
                        exit(1);
                    }
                    *ref = value;

                    ++aIter;
                };

            collect_string(0, &stmp_cellphone.asin);
            collect_string(1, &stmp_cellphone.brand);
            collect_string(2, &stmp_cellphone.title);
            collect_string(3, &stmp_cellphone.url);
            collect_string(4, &stmp_cellphone.image);

            if (aIter == sj_array.end()) {
                std::cerr << "data_docs[" << idx << "]"
                          << "[5]: "
                          << "end of array\n";
                exit(1);
            }

            code = (*aIter).get_double().get(stmp_cellphone.rating);
            if (code)  {
                std::cerr << "data_docs[" << idx << "]"
                          << "[5]: "
                          << simdjson::error_message(code)
                          << '\n';
                exit(1);
            }
            ++aIter;

            collect_string(6, &stmp_cellphone.reviewUrl);

            if (aIter == sj_array.end()) {
                std::cerr << "data_docs[" << idx << "]"
                          << "[7]: "
                          << "end of array\n";
                exit(1);
            }

            code = (*aIter).get_uint64().get(stmp_cellphone.totalReviews);
            if (code)  {
                std::cerr << "data_docs[" << idx << "]"
                          << "[7]: "
                          << simdjson::error_message(code)
                          << '\n';
                exit(1);
            }
            ++aIter;

            collect_string(8, &stmp_cellphone.prices);

            result.push_back(std::move(stmp_cellphone));
        }

        ++idx;
    }

    return result;
}


static void
cellphone_file_peval_local(benchmark::State& state) {
    for (auto _ : state) {
        auto phones = loadCellphones_peval_local();
        benchmark::DoNotOptimize(phones.data());
    }
}

static void
cellphone_file_peval_global(benchmark::State& state) {
    for (auto _ : state) {
        auto phones = loadCellphones_peval_global();
        benchmark::DoNotOptimize(phones.data());
    }
}

static void
cellphone_file_raw(benchmark::State& state) {
    for (auto _ : state) {
        auto phones = loadCellphones_raw();
        benchmark::DoNotOptimize(phones.data());
    }
}

BENCHMARK(cellphone_file_peval_local)
->Setup(setupCellphoneLoadJSON);

BENCHMARK(cellphone_file_peval_global)
->Setup(setupCellphoneLoadJSON);

BENCHMARK(cellphone_file_raw)
->Setup(setupCellphoneLoadJSON);


// Object Map Data
// /////////////////////////////////////////////////////////////////////////

using MapDataPair = std::pair<std::string_view, int64_t>;

static std::vector<MapDataPair> obj_map_data;
static MapDataPair stmp_map_data;

static auto eval_map_data =
    simdjson_peval::object_to_out_iter<simdjson::ondemand::document>(
        std::back_inserter(obj_map_data), &stmp_map_data,
        simdjson_peval::number_value(&stmp_map_data.second));

static void
setupMapDataLoadJSON(size_t num_members) {
    obj_map_data.reserve(num_members);

    std::string raw_json;
    raw_json.reserve(1<<16);

    raw_json += '{';

    bool first = true;
    for (size_t i = 0; i < num_members; ++i) {
        if (first) {
            first = false;
        }
        else {
            raw_json += ',';
        }

        raw_json += "\"member_" + std::to_string(i) + "\":" + std::to_string(i);
    }
    raw_json += '}';

    json_data = simdjson::padded_string(raw_json);
}

static void
setupMapDataLoadJSON(const benchmark::State& state) {
    setupMapDataLoadJSON(state.range(0));
}


static std::vector<MapDataPair>
loadMapData_peval_local(size_t reserve) {
    using namespace simdjson_peval;
    auto data_document = parser.iterate(json_data);

    obj_map_data.clear();

    auto eval_fn =
        object_to_out_iter<simdjson::ondemand::document>(
            std::back_inserter(obj_map_data), &stmp_map_data,
            number_value(&stmp_map_data.second));

    auto errors = error(ERRORS_WITH_PATH);
    eval_fn(data_document, &errors);

    if (errors) {
        std::cerr << "PEVAL_ERROR:\n" << errors.to_string() << '\n';
        exit(1);
    }

    return std::move(obj_map_data);
}


static std::vector<MapDataPair>
loadMapData_peval_global(size_t reserve) {
    using namespace simdjson_peval;
    auto data_document = parser.iterate(json_data);

    obj_map_data.clear();

    auto errors = error(ERRORS_WITH_PATH);
    eval_map_data(data_document, &errors);

    if (errors) {
        std::cerr << "PEVAL_ERROR:\n" << errors.to_string() << '\n';
        exit(1);
    }

    return std::move(obj_map_data);
}


static std::vector<MapDataPair>
loadMapData_raw(size_t reserve) {
    using namespace simdjson_peval;
    auto data_document = parser.iterate(json_data);

    obj_map_data.clear();

    simdjson::ondemand::object sj_obj;
    auto code = data_document.get_object().get(sj_obj);
    if (code) {
        std::cerr << "document is not an object\n";
        exit(1);
    }

    auto out_iter = back_inserter(obj_map_data);

    for (auto sj_pair : sj_obj) {
        if (sj_pair.error()) {
            std::cerr << "pair is not valid\n";
            exit(1);
        }

        std::string_view key;
        code = sj_pair.unescaped_key().get(key);
        if (sj_pair.error()) {
            std::cerr << "key is not valid\n";
            exit(1);
        }

        int64_t value;
        code = sj_pair.value().get_int64().get(value);

        stmp_map_data = {key, value};

        *out_iter = std::move(stmp_map_data);
        ++out_iter;
        // obj_map_data.emplace_back(std::make_pair(key, value));
    }

    return std::move(obj_map_data);
}


static void
map_data_peval_local(benchmark::State& state) {
    auto reserve = state.range(0);
    for (auto _ : state) {
        auto map_data = loadMapData_peval_local(reserve);
        benchmark::DoNotOptimize(map_data.data());
    }
}

static void
map_data_peval_global(benchmark::State& state) {
    auto reserve = state.range(0);
    for (auto _ : state) {
        auto map_data = loadMapData_peval_global(reserve);
        benchmark::DoNotOptimize(map_data.data());
    }
}

static void
map_data_raw(benchmark::State& state) {
    auto reserve = state.range(0);
    for (auto _ : state) {
        auto map_data = loadMapData_raw(reserve);
        benchmark::DoNotOptimize(map_data.data());
    }
}

BENCHMARK(map_data_peval_local)
->Setup(setupMapDataLoadJSON)
->Range(2, 1 << 16);

BENCHMARK(map_data_peval_global)
->Setup(setupMapDataLoadJSON)
->Range(2, 1 << 16);

BENCHMARK(map_data_raw)
->Setup(setupMapDataLoadJSON)
->Range(2, 1 << 16);


// Main
// /////////////////////////////////////////////////////////////////////////

// BENCHMARK_MAIN();
int main(int argc, char *argv[]) {
#if DO_PROFILE_TWITTER
    // Profile twitter benchmark tests
    setupTwitterLoadJSON();

    auto tweets_peval = loadTwitter_peval_global();
    benchmark::DoNotOptimize(tweets_proto.data());

    auto tweets_raw = loadTwitter_raw();
    benchmark::DoNotOptimize(tweets_raw.data());

#elif DO_PROFILE_PHONE
    // Profile cellphone benchmark tests
    setupCellphoneLoadJSON();

    auto phones_peval = loadCellphones_peval_global();
    benchmark::DoNotOptimize(phones_peval.data());

    auto phones_raw = loadCellphones_raw();
    benchmark::DoNotOptimize(phones_raw.data());

#elif DO_PROFILE_MAP_DATA
    // Profile map data tests
    setupMapDataLoadJSON(1<<16);

    auto map_data_peval = loadMapData_peval_global(1<<16);
    benchmark::DoNotOptimize(map_data_peval.data());

    auto map_data_raw = loadMapData_raw(1<<16);
    benchmark::DoNotOptimize(map_data_raw.data());
#else
    // Normal benchmark.
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

#endif

    return 0;
}

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

static const bool ERRORS_WITH_PATH = false;

static simdjson::ondemand::parser parser;
static simdjson::padded_string jsonData;


// Structure to hold data of a tweet.
struct Tweet {
    uint64_t id;
    std::string_view text;
    std::string_view name;
    std::string_view screen_name;
    std::vector<std::string_view> hashtags;
};

static simdjson_peval::eval_fn_type<simdjson::ondemand::document> twitter_proto;
static Tweet stmp_tweet;
std::string_view stmp_hash;
static std::vector<Tweet> stweets;

static void
setupTwitterLoadJSON(const benchmark::State& state) {
    jsonData = simdjson::padded_string::load(TWITTER_EXAMPLE);
}


static void
setupTwitterWithPrototype(const benchmark::State& state) {
    using namespace simdjson_peval;

    setupTwitterLoadJSON(state);

    twitter_proto =
        object<simdjson::ondemand::document>(
            member(
                "statuses",
                array_to_out_iter(
                    back_inserter(stweets), &stmp_tweet,
                    object(
                        member(
                            "id", number_value(&stmp_tweet.id)),
                        member(
                            "text", string_value(&stmp_tweet.text)),
                        member(
                            "user",
                            object(
                                member(
                                    "name",
                                    string_value(&stmp_tweet.name)),
                                member(
                                    "screen_name",
                                    string_value(&stmp_tweet.screen_name))
                                )),
                        member(
                            "entities",
                            object(
                                member(
                                    "hashtags",
                                    array_to_out_iter(
                                        back_inserter(stmp_tweet.hashtags),
                                        &stmp_hash,
                                        object(
                                            member(
                                                "text",
                                                string_value(&stmp_hash))))))
                            )
                        )
                    )
                )
            );
}




std::vector<Tweet>
loadTwitter_full_peval() {
    using namespace simdjson_peval;
    auto dataDoc = parser.iterate(jsonData);

    Tweet tmp_tweet;
    std::string_view tmp_hash;

    std::vector<Tweet> tweets;

    auto prototype =
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
    prototype(dataDoc, &errors);

    if (errors) {
        std::cerr << "ERROR:\n" << errors.to_string() << '\n';
        exit(1);
    }

    return tweets;
}


std::vector<Tweet>
loadTwitter_proto_peval() {
    using namespace simdjson_peval;
    auto dataDoc = parser.iterate(jsonData);

    stweets.clear();
    auto errors = error(ERRORS_WITH_PATH);
    twitter_proto(dataDoc, &errors);

    if (errors) {
        std::cerr << "ERROR:\n" << errors.to_string() << '\n';
        exit(1);
    }

    return std::move(stweets);
}



std::vector<Tweet>
loadTwitter_raw() {
    using namespace simdjson_peval;
    auto dataDoc = parser.iterate(jsonData);

    simdjson::ondemand::value js_statuses;
    auto code = dataDoc.find_field_unordered("statuses").get(js_statuses);
    if (code) {
        std::cerr
            << "dataDoc[\"statuses\"]: "
            << simdjson::error_message(code)
            << '\n';
        exit(1);
    }

    simdjson::ondemand::array js_statuses_array;
    code = js_statuses.get_array().get(js_statuses_array);
    if (code) {
        std::cerr
            << "dataDoc[\"statuses\"].get_array(): "
            << simdjson::error_message(code)
            << '\n';
        exit(1);
    }

    std::vector<Tweet> tweets;

    int idx = 0;
    for (auto js_elem : js_statuses_array) {
        Tweet tmp_tweet;

        simdjson::ondemand::object js_elem_obj;
        code = js_elem.get_object().get(js_elem_obj);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value js_id;
        code = js_elem_obj.find_field_unordered("id").get(js_id);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"id\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        code = js_id.get_uint64().get(tmp_tweet.id);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"id\"]"
                << ".get_uint64()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value js_text;
        code = js_elem_obj.find_field_unordered("text").get(js_text);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"text\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        code = js_text.get_string().get(tmp_tweet.text);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"text\"]"
                << ".get_string()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value js_user;
        code = js_elem_obj.find_field_unordered("user").get(js_user);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"user\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::object js_user_obj;
        code = js_user.get_object().get(js_user_obj);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"user\"]"
                << ".get_object()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value js_name;
        code = js_user_obj.find_field_unordered("name").get(js_name);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"user\"]"
                << "[\"name\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        code = js_name.get_string().get(tmp_tweet.name);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"user\"]"
                << "[\"name\"]"
                << ".get_string()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value js_screen_name;
        code =
            js_user_obj.find_field_unordered("screen_name")
            .get(js_screen_name);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"user\"]"
                << "[\"screen_name\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        code = js_screen_name.get_string().get(tmp_tweet.screen_name);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"user\"]"
                << "[\"screen_name\"]"
                << ".get_string()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value js_entities;
        code = js_elem_obj.find_field_unordered("entities").get(js_entities);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"entities\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::object js_entities_obj;
        code = js_entities.get_object().get(js_entities_obj);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"entities\"]"
                << ".get_object()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::value js_hashtags;
        code =
            js_entities_obj.find_field_unordered("hashtags")
            .get(js_hashtags);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"entities\"]"
                << "[\"hashtags\"]"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        simdjson::ondemand::array js_hashtags_array;
        code = js_hashtags.get_array().get(js_hashtags_array);
        if (code) {
            std::cerr
                << "dataDoc[\"statuses\"]"
                << '[' << idx<< ']'
                << "[\"entities\"]"
                << "[\"hashtags\"]"
                << ".get_array()"
                << ": " << simdjson::error_message(code)
                << '\n';
            exit(1);
        }

        int hidx = 0;
        for (auto js_htag : js_hashtags_array) {
            simdjson::ondemand::object js_htag_obj;
            code = js_htag.get_object().get(js_htag_obj);
            if (code) {
                std::cerr
                    << "dataDoc[\"statuses\"]"
                    << '[' << idx<< ']'
                    << "[\"entities\"]"
                    << "[\"hashtags\"]"
                    << '[' << hidx<< ']'
                    << ": " << simdjson::error_message(code)
                    << '\n';
                exit(1);
            }

            simdjson::ondemand::value js_htag_text;
            code = js_htag_obj.find_field_unordered("text").get(js_htag_text);
            if (code) {
                std::cerr
                    << "dataDoc[\"statuses\"]"
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
            code = js_htag_text.get_string().get(tmp_hash);
            if (code) {
                std::cerr
                    << "dataDoc[\"statuses\"]"
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
twitter_file_peval_fullload(benchmark::State& state) {
    for (auto _ : state) {
        auto tweets = loadTwitter_full_peval();
        benchmark::DoNotOptimize(tweets.data());
    }
}


static void
twitter_file_peval_proto(benchmark::State& state) {
    for (auto _ : state) {
        auto tweets = loadTwitter_proto_peval();
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

BENCHMARK(twitter_file_peval_fullload)
->Setup(setupTwitterLoadJSON);

BENCHMARK(twitter_file_peval_proto)
->Setup(setupTwitterWithPrototype);

BENCHMARK(twitter_file_raw)
->Setup(setupTwitterLoadJSON);


BENCHMARK_MAIN();

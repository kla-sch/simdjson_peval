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
// Description: Shows usage of real twitter example.
//

#include <simdjson_peval.h>

#include <iostream>


int
main(int argc, char *argv[]) {
    using namespace simdjson_peval;

    // Structure to hold data of a tweet.
    struct Tweet {
        uint64_t id;
        std::string_view text;
        std::string_view name;
        std::string_view screen_name;
        std::vector<std::string_view> hashtags;
    };

    // Temporay tweet data to create array.
    Tweet tmp_tweet;

    // Temporay hashtag data to create hashtag array.
    std::string_view tmp_hash;

    // std::vector to store all tweets.
    std::vector<Tweet> tweets;

    // Number of tweets
    uint64_t tweet_count;

    // create evaluation function for twitter tweets
    auto eval_tweets =
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
                ),
            member(
                "search_metadata",
                object(
                    member("count", number_value(&tweet_count)))
                )
            );

    // error container. collecting path names enabled.
    auto errors = error(true);

    // load and parse document
    simdjson::ondemand::parser parser;
    auto padded_json = simdjson::padded_string::load(TWITTER_EXAMPLE);
    auto json_document = parser.iterate(padded_json);

    // execute prototype to collect data.
    eval_tweets(json_document, &errors);

    if (errors) {
        // show errors
        std::cout << "ERROR:\n" << errors.to_string() << '\n';
        return 1;
    }
    else {
        // Write data
        for (auto t : tweets) {
            std::cout
                << "==================== (" <<  t.id << ")\n"
                << "From: " << t.screen_name << " (@" << t.name << ")\n"
                << "Tags: ";

            if (t.hashtags.empty()) {
                std::cout << "[none]";
            }
            else {
                bool first = true;
                for (const auto &tag : t.hashtags) {
                    if (first) {
                        first = false;
                    }
                    else {
                        std::cout << ", ";
                    }
                    std::cout << "#" << tag;
                }
            }
            std::cout << "\n--------------------\n"
                      << t.text << "\n\n";
        }

        std::cout << "====================\n"
                  << "total: " << tweet_count << " records\n";

        return 0;
    }

}

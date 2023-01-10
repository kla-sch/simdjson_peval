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
#include <vector>

int
main(int argc, char *argv[]) {
    using namespace simdjson_peval;

    simdjson::ondemand::parser parser;
    auto padded_json = simdjson::padded_string::load(AMAZON_EXAMPLE);

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

    // result vector
    std::vector<Cellphone> cellphones;

    // temprorary storage for one phone
    Cellphone tmp_phone;

    // Create prototype for a single document that stores data into tmp_phone
    auto eval_amazon_data =
        array<simdjson::ondemand::document_reference>(
            string_value(&tmp_phone.asin),
            string_value(&tmp_phone.brand),
            string_value(&tmp_phone.title),
            string_value(&tmp_phone.url),
            string_value(&tmp_phone.image),
            number_value(&tmp_phone.rating),
            string_value(&tmp_phone.reviewUrl),
            number_value(&tmp_phone.totalReviews),
            string_value(&tmp_phone.prices));

    // create stream of documents
    auto docStream = parser.iterate_many(padded_json.value());

    size_t idx = 0;

    auto errors = error(true); // report alle errors in a single container.
    for (auto json_document_ref : docStream.value()) {
        // set scope with document number:
        error::path_scope doc_scope(&errors, idx);

        if (idx != 0) { // ignore fist document.
            eval_amazon_data(json_document_ref, &errors); // evaluate document.
            if (!errors) {
                // save data
                cellphones.push_back(std::move(tmp_phone));
            }
        }
        ++idx; // count documents.
    }

    if (errors) {
        // show errors
        std::cout << "ERROR:\n" << errors.to_string() << '\n';
        return 1;
    }
    else {
        // Pront single cellphone data.
        for (const auto &p : cellphones) {
            std::cout
                << "==================== (" << p.asin << ")\n"
                << "brand: " << p.brand << '\n'
                << "title: " << p.title << '\n'
                << "image: " << p.image << '\n'
                << "rating: " << p.rating
                << " by " << p.totalReviews << " reviews\n"
                << "reviews: " << p.reviewUrl << '\n';
            if (!p.prices.empty()) {
                std::cout << "prices: " << p.prices << '\n';
            }
        }
    }

    std::cout << "====================\n"
              << "total: " << (idx-1) << " phones\n";
}

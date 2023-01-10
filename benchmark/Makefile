#
# Copyright 2023 kla-sch@gmx.de
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation files
# (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

SIMDJSONPATH = ../simdjson
SIMDJSONBUILD = $(SIMDJSONPATH)/build

GOOGLE_BENCH=./google_benchmark
GOOGLE_BENCH_BUILD=$(GOOGLE_BENCH)/build

SIMDJSON_PEVAL_PATH = ..

CPP = c++ -std=c++20
# CPP = clang++ -std=c++20
SRCS = simdjson_peval_bench.cpp

OBJS = $(SRCS:%.cpp=objs/%.o)

TWITTER_EXAMPLE=../simdjson/jsonexamples/twitter.json

EXES = runBench

CCFLAGS= -Wall -O3 -MMD  \
	-I$(SIMDJSONPATH)/include \
	-I$(SIMDJSON_PEVAL_PATH)/include \
	-I$(GOOGLE_BENCH)/include \
	-DTWITTER_EXAMPLE=\"$(TWITTER_EXAMPLE)\"

LDFLAGS = \
	-L$(GOOGLE_BENCH_BUILD)/src \
	-L$(SIMDJSONBUILD) \
	-lsimdjson  -lbenchmark -lpthread

.PHONY: all third_party clean extra_clean

all: third_party $(EXES)

runBench: objs $(OBJS)
	@echo "link: $@ "
	@$(CPP) -o $@ $(OBJS) $(CCFLAGS) $(LDFLAGS)

third_party: $(SIMDJSONBUILD) $(GOOGLE_BENCH_BUILD)

objs:
	@echo create objs directory
	@mkdir objs

$(SIMDJSONBUILD):
	mkdir $(SIMDJSONBUILD)
	( cd $(SIMDJSONBUILD); cmake ..)
	make -C $(SIMDJSONBUILD)

$(GOOGLE_BENCH_BUILD):
	( \
		cd $(GOOGLE_BENCH); \
		cmake -E make_directory "build"; \
		cmake -E chdir "build" \
			cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on \
				-DCMAKE_BUILD_TYPE=Release ../; \
		cmake --build "build" --config Release; \
	)


clean:
	@echo remove object files and executables
	@rm -f $(EXES) $(OBJS)

extra_clean: clean
	@echo extra clean
	@rm -rf objs

objs/%.o: %.cpp
	@echo "compile: $<"
	@$(CPP) $(CCFLAGS) -c $< -o $@

-include objs/*.d

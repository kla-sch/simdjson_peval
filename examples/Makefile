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

SIMDJSON_PEVAL_PATH = ..

# CPP = c++ -std=c++17
CPP = clang++ -std=c++17

SRCS_SUM_ARRAY = sum_array.cpp
SRCS_STUDENTS_AND_TEACHERS = students_and_teachers.cpp
SRCS_PRINT_TWEETS = print_tweets.cpp
SRCS_PRINT_AMAZON_CELLPHONES = print_amazon_cellphones.cpp

OBJS_SUM_ARRAY=$(SRCS_SUM_ARRAY:%.cpp=objs/%.o)
OBJS_STUDENTS_AND_TEACHERS=$(SRCS_STUDENTS_AND_TEACHERS:%.cpp=objs/%.o)
OBJS_PRINT_TWEETS=$(SRCS_PRINT_TWEETS:%.cpp=objs/%.o)
OBJS_PRINT_AMAZON_CELLPHONES=$(SRCS_PRINT_AMAZON_CELLPHONES:%.cpp=objs/%.o)

TWITTER_EXAMPLE=$(SIMDJSONPATH)/jsonexamples/twitter.json
AMAZON_EXAMPLE=$(SIMDJSONPATH)/jsonexamples/amazon_cellphones.ndjson

EXES = \
	sum_array \
	students_and_teachers \
	print_tweets \
	print_amazon_cellphones

OBJS = \
	$(OBJS_SUM_ARRAY) \
	$(OBJS_STUDENTS_AND_TEACHERS) \
	$(OBJS_PRINT_TWEETS) \
	$(OBJS_PRINT_AMAZON_CELLPHONES)

CCFLAGS= -Wall -O3 -MMD  \
	-I$(SIMDJSONPATH)/include \
	-I$(SIMDJSON_PEVAL_PATH)/include \
	-DTWITTER_EXAMPLE=\"$(TWITTER_EXAMPLE)\" \
	-DAMAZON_EXAMPLE=\"$(AMAZON_EXAMPLE)\"

LDFLAGS = -L$(SIMDJSONBUILD) -lsimdjson

.PHONY: all third_party clean extra_clean

all: $(EXES)

sum_array: objs $(OBJS_SUM_ARRAY)
	@echo "link: $@ "
	@$(CPP) -o $@ $(OBJS_SUM_ARRAY) $(CCFLAGS) $(LDFLAGS)

students_and_teachers: $(OBJS_STUDENTS_AND_TEACHERS)
	@echo "link: $@ "
	@$(CPP) -o $@ $(OBJS_STUDENTS_AND_TEACHERS) $(CCFLAGS) $(LDFLAGS)

print_tweets: $(OBJS_PRINT_TWEETS)
	@echo "link: $@ "
	@$(CPP) -o $@ $(OBJS_PRINT_TWEETS) $(CCFLAGS) $(LDFLAGS)

print_amazon_cellphones: $(OBJS_PRINT_AMAZON_CELLPHONES)
	@echo "link: $@ "
	@$(CPP) -o $@ $(OBJS_PRINT_AMAZON_CELLPHONES) $(CCFLAGS) $(LDFLAGS)

third_party: $(SIMDJSONBUILD)

objs:
	@echo create objs directory
	@mkdir objs

$(SIMDJSONBUILD):
	mkdir $(SIMDJSONBUILD)
	( cd $(SIMDJSONBUILD); cmake ..)
	make -C $(SIMDJSONBUILD)

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

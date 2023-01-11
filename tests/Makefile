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
# Targets:
#     all   - compile all test programs (default)
#     c++17 - compile test program for C++17
#     c++20 - compile test program for C++20
#     check - compile and start all test programs
#     clean - remove object files and programs

ALL = c++17 c++20

GOOGLETESTPATH = googletest
GOOGLETESTBUILD = $(GOOGLETESTPATH)/build

SIMDJSONPATH = ../simdjson
SIMDJSONBUILD = $(SIMDJSONPATH)/build

CPP = c++
# CPP = clang++
# CPP = PATH=/opt/gcc-12.2.0/bin:$$PATH c++

SRCS = \
	simdjson_peval_tests.cpp \
	ValueStringView.cpp \
	ValueString.cpp \
	ValueStringToInt64.cpp \
	ValueStringToUInt64.cpp \
	ValueStringToDouble.cpp \
	ValueInt64.cpp \
	ValueUInt64.cpp \
	ValueDouble.cpp \
	ValueBool.cpp \
	ValueFunction.cpp \
	ValueMisc.cpp \
	ObjectPlain.cpp \
	ObjectMisc.cpp \
	ObjectDiscriminator.cpp \
	ObjectTo.cpp \
	ArrayPlain.cpp \
	ArrayMisc.cpp \
	ArrayTo.cpp

OBJS_17 = $(SRCS:%.cpp=objs_c++17/%.o)
OBJS_20 = $(SRCS:%.cpp=objs_c++20/%.o)

OBJS = $(OBJS_17) $(OBJS_20)

EXES = \
	testSimdjsonPeval_c++17 \
	testSimdjsonPeval_c++20

CCFLAGS= -Wall -O0 -ggdb -MMD  \
	-I../include -I$(GOOGLETESTPATH)/googletest/include \
	-I$(SIMDJSONPATH)/include

LDFLAGS = \
	-L$(GOOGLETESTBUILD)/lib -lgtest \
	-L$(SIMDJSONBUILD) -lsimdjson \
	-lpthread

.PHONY: all check third_party c++17 c++20 clean extra_clean

all: $(ALL)

c++17: testSimdjsonPeval_c++17
c++20: testSimdjsonPeval_c++20

check: $(EXES)
	./testSimdjsonPeval_c++17 --gtest_brief=1
	./testSimdjsonPeval_c++20 --gtest_brief=1

third_party: $(GOOGLETESTBUILD) $(SIMDJSONBUILD)

testSimdjsonPeval_c++17: objs_c++17 third_party $(OBJS_17)
	@echo "[$(CPP)] link for c++17: $@ "
	@$(CPP) -std=c++17 -o $@  $(OBJS_17) $(CCFLAGS) $(LDFLAGS)

testSimdjsonPeval_c++20: objs_c++20 third_party $(OBJS_20)
	@echo "[$(CPP)] link for c++20: $@ "
	@$(CPP) -std=c++20 -o $@  $(OBJS_20) $(CCFLAGS) $(LDFLAGS)

objs_c++11 objs_c++14 objs_c++17 objs_c++20:
	@echo "Create object dir: $@"
	@mkdir $@

$(GOOGLETESTBUILD):
	mkdir $(GOOGLETESTBUILD)
	( cd $(GOOGLETESTBUILD); cmake ..)
	make -C $(GOOGLETESTBUILD)

$(SIMDJSONBUILD):
	mkdir $(SIMDJSONBUILD)
	( cd $(SIMDJSONBUILD); cmake ..)
	make -C $(SIMDJSONBUILD)

clean:
	@echo remove object files and executables
	@rm -f $(EXES) $(OBJS)

extra_clean: clean
	@echo extra clean
	@rm -rf objs_c++11 objs_c++14 objs_c++17 objs_c++20

objs_c++17/%.o: %.cpp
	@echo "[$(CPP)] compile for c++17: $<"
	@$(CPP) -std=c++17 $(CCFLAGS) -c $< -o $@

objs_c++20/%.o: %.cpp
	@echo "[$(CPP)] compile for c++20: $<"
	@$(CPP) -std=c++20 $(CCFLAGS) -c $< -o $@

-include objs_c++17/*.d
-include objs_c++20/*.d

#
#	Makefile
#
#	Created on: Dec 26, 2019
#	Author: jb
#

COMPILER         := -g++
#COMPILER        := -clang++
OPTIMIZATION_OPT := -O1
BASE_OPTIONS     := -fmax-errors=5 #-pedantic-errors -Wall -Wextra -Werror -Wno-long-long
OPTIONS          := $(BASE_OPTIONS) $(OPTIMIZATION_OPT)
LINKER_OPT       := -L/usr/lib -lstdc++ -lm
DBG_OPT			 := -g -DDEBUG #-fsanitize=address -static-libasan  -fno-omit-frame-pointer

ASAN_OPT         := -g -fsanitize=address -static-libasan -fno-omit-frame-pointer
MSAN_OPT         := -g -fsanitize=memory    -fno-omit-frame-pointer
LSAN_OPT         := -g -fsanitize=leak      -fno-omit-frame-pointer
USAN_OPT         := -g -fsanitize=undefined -fno-omit-frame-pointer
BUILD_SRC        := $(sort $(wildcard exprtk_*.cpp))
BUILD_LIST       := $(BUILD_SRC:%.cpp=%)


# all: $(BUILD_LIST)
# 
# $(BUILD_LIST) : %: %.cpp exprtk.hpp
# 	$(COMPILER) $(OPTIONS) -o $@ $@.cpp $(LINKER_OPT)
# 
# strip_bin :
# 	@for f in $(BUILD_LIST); do if [ -f $$f ]; then strip -s $$f; echo $$f; fi done;
# 
# valgrind :
# 	@for f in $(BUILD_LIST); do \
# 		if [ -f $$f ]; then \
# 			cmd="valgrind --leak-check=full --show-reachable=yes --track-origins=yes --log-file=$$f.log -v ./$$f"; \
# 			echo $$cmd; \
# 			$$cmd; \
# 		fi done;
# 
# pgo: exprtk_benchmark.cpp exprtk.hpp
# 	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-generate -o exprtk_benchmark exprtk_benchmark.cpp $(LINKER_OPT)
# 	./exprtk_benchmark
# 	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-use -o exprtk_benchmark exprtk_benchmark.cpp $(LINKER_OPT)

	
clean:
	cd build && rm -f *


# make .o file of BParser	
build/grammar.o:
	$(COMPILER) $(BASE_OPTIONS) -std=c++11 -I include  -o build/grammar.o -c bparser/include/grammar.cc
	
grammar: build/grammar.o


# make .o files of MuParser	
build/muParser.o:
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I muparser_/include  -o build/muParser.o -c muparser_/src/muParser.cpp

muparser: build/muParser.o

build/muParserBase.o:
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I muparser_/include  -o build/muParserBase.o -c muparser_/src/muParserBase.cpp

muparserbase: build/muParserBase.o

build/muParserBytecode.o:
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I muparser_/include  -o build/muParserBytecode.o -c muparser_/src/muParserBytecode.cpp

muparserbytecode: build/muParserBytecode.o

build/muParserCallback.o:
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I muparser_/include  -o build/muParserCallback.o -c muparser_/src/muParserCallback.cpp

muparsercallback: build/muParserCallback.o

build/muParserDLL.o:
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I muparser_/include  -o build/muParserDLL.o -c muparser_/src/muParserDLL.cpp

muparserdll: build/muParserDLL.o

build/muParserError.o:
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I muparser_/include  -o build/muParserError.o -c muparser_/src/muParserError.cpp

muparsererror: build/muParserError.o

build/muParserInt.o:
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I muparser_/include  -o build/muParserInt.o -c muparser_/src/muParserInt.cpp

muparserint: build/muParserInt.o

build/muParserTest.o:
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I muparser_/include  -o build/muParserTest.o -c muparser_/src/muParserTest.cpp

muparsertest: build/muParserTest.o

build/muParserTokenReader.o:
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I muparser_/include  -o build/muParserTokenReader.o -c muparser_/src/muParserTokenReader.cpp

muparsertokenreader: build/muParserTokenReader.o

# Collective build of .o files
muparser-all: muparser muparserbase muparserbytecode muparsercallback muparserdll muparsererror muparserint muparsertest muparsertokenreader


# make tests	
test_exprtk:
	rm -f build/test_exprtk 2>/dev/null
	#$(COMPILER) $(BASE_OPTIONS) $(DBG_OPT)  -std=c++11 -I include  -o build/test_exprtk test/test_exprtk.cc
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I include  -o build/test_exprtk test/test_exprtk.cc
	build/test_exprtk

test_muparser:
	rm -f build/test_muparser 2>/dev/null
	#$(COMPILER) $(BASE_OPTIONS) $(DBG_OPT)  -std=c++11 -I include  -o build/test_muparser test/test_muparser.cc
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I include -I muparser_/include  -o build/test_muparser build/muParser.o \
	    build/muParserBase.o build/muParserBytecode.o build/muParserCallback.o build/muParserDLL.o build/muParserError.o \
	    build/muParserInt.o build/muParserTest.o build/muParserTokenReader.o test/test_muparser.cc 
	build/test_muparser

test_bparser:
	rm -f build/test_bparser 2>/dev/null
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I include -I bparser  -o build/test_bparser build/grammar.o test/test_bparser.cc
	build/test_bparser

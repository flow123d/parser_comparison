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


	
build/grammar.o:
	$(COMPILER) $(BASE_OPTIONS) -std=c++11 -I include  -o build/grammar.o -c bparser/include/grammar.cc
	
grammar: build/grammar.o


## Proof of concept snippets.

test_exprtk:
	rm -f build/test_exprtk 2>/dev/null
	#$(COMPILER) $(BASE_OPTIONS) $(DBG_OPT)  -std=c++11 -I include  -o build/test_exprtk test/test_exprtk.cc
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I include  -o build/test_exprtk test/test_exprtk.cc
	build/test_exprtk

test_muparser:
#	$(MAKE) -C muparser/make all
	cd muparser && $(MAKE) all
	rm -f build/test_muparser 2>/dev/null
	#$(COMPILER) $(BASE_OPTIONS) $(DBG_OPT)  -std=c++11 -I include  -o build/test_muparser test/test_muparser.cc
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I include -I muparser  -o build/test_muparser muparser/obj/mpError.o muparser/obj/mpRPN.o \
	    muparser/obj/mpICallback.o muparser/obj/mpIValReader.o muparser/obj/mpParserBase.o muparser/obj/mpTokenReader.o muparser/obj/mpVariable.o \
	    muparser/obj/mpIOprt.o muparser/obj/mpIValue.o muparser/obj/mpParser.o muparser/obj/mpValReader.o muparser/obj/mpFuncStr.o \
	    muparser/obj/mpFuncCommon.o muparser/obj/mpOprtPostfixCommon.o muparser/obj/mpFuncNonCmplx.o muparser/obj/mpFuncCmplx.o \
	    muparser/obj/mpIToken.o muparser/obj/mpOprtCmplx.o muparser/obj/mpOprtNonCmplx.o muparser/obj/mpOprtBinCommon.o \
	    muparser/obj/mpOprtBinAssign.o muparser/obj/mpOprtMatrix.o muparser/obj/mpIPackage.o muparser/obj/mpPackageCommon.o \
	    muparser/obj/mpPackageStr.o muparser/obj/mpPackageCmplx.o muparser/obj/mpPackageNonCmplx.o muparser/obj/mpPackageMatrix.o \
	    muparser/obj/mpPackageUnit.o muparser/obj/mpIfThenElse.o muparser/obj/mpValueCache.o muparser/obj/mpValue.o muparser/obj/mpTest.o \
	    muparser/obj/mpScriptTokens.o muparser/obj/mpFuncMatrix.o muparser/obj/mpOprtIndex.o muparser/obj/mpParserMessageProvider.o \
	    test/test_muparser.cc 
	build/test_muparser

test_bparser:
	rm -f build/test_bparser 2>/dev/null
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I include -I bparser  -o build/test_bparser build/grammar.o test/test_bparser.cc
	build/test_bparser

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


.PHONY : all
all: test_exprtk test_bparser test_muparser test_cpp plots


build:
	mkdir build 2>/dev/null 

output:
	mkdir output 2>/dev/null 

	
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
	cd build && rm -rf *

	
	
	
#############################################
# BParser

# make .o file of BParser	
build/grammar.o: build bparser/include/grammar.cc
	$(COMPILER) $(BASE_OPTIONS) -std=c++11 -I include  -o build/grammar.o -c bparser/include/grammar.cc
	
build/test_bparser: build build/grammar.o test/test_bparser.cc
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I include -I bparser/include  -o build/test_bparser build/grammar.o test/test_bparser.cc
	
test_bparser: build output build/test_bparser
	build/test_bparser




#############################################
# muParser


muparser/libmuparser.so: build
	mkdir -p build/muparser 2>/dev/null 
	cd build/muparser && cmake ../../muparser
	cd build/muparser && make

build/test_muparser: build muparser/libmuparser.so test/test_muparser.cc
	#$(COMPILER) $(BASE_OPTIONS) $(DBG_OPT)  -std=c++11 -I include  -o build/test_muparser test/test_muparser.cc
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I include -I muparser/include  -o build/test_muparser -Lbuild/muparser -Wl,-rpath=build/muparser test/test_muparser.cc  -lmuparser

test_muparser: build output build/test_muparser
	#rm -f build/test_muparser 2>/dev/null	
	build/test_muparser



#############################################
#

# make tests	
build/test_exprtk: build test/test_exprtk.cc
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I include -I exprtk -o build/test_exprtk test/test_exprtk.cc

test_exprtk: build output build/test_exprtk
	#rm -f build/test_exprtk 2>/dev/null
	#$(COMPILER) $(BASE_OPTIONS) $(DBG_OPT)  -std=c++11 -I include  -o build/test_exprtk test/test_exprtk.cc
	build/test_exprtk

	
#############################################
#

# make cpp expresions test	
build/test_cpp: build test/test_cpp.cc
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I include -I exprtk -o build/test_cpp test/test_cpp.cc

test_cpp: build output build/test_cpp
	#rm -f build/test_exprtk 2>/dev/null
	#$(COMPILER) $(BASE_OPTIONS) $(DBG_OPT)  -std=c++11 -I include  -o build/test_exprtk test/test_exprtk.cc
	build/test_cpp

	
plots: 
	


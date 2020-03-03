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


	
#build/grammar.o:
#	$(COMPILER) $(BASE_OPTIONS) -std=c++11 -I include  -o build/grammar.o -c include/grammar.cc
	
#grammar: build/grammar.o


## Proof of concept snippets.

test_exprtk:
	rm -f build/test_exprtk 2>/dev/null
	#$(COMPILER) $(BASE_OPTIONS) $(DBG_OPT)  -std=c++11 -I include  -o build/test_exprtk test/test_exprtk.cc
	$(COMPILER) $(BASE_OPTIONS) -O3 -mavx2  -std=c++11 -I include  -o build/test_exprtk test/test_exprtk.cc
	build/test_exprtk

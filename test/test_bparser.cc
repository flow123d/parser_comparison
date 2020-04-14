/*
 * test_bparser.cc
 *
 *  Created on: Mar 18, 2020
 *      Author: David Flanderka
 */

#ifndef TEST_BPARSER_CC_
#define TEST_BPARSER_CC_

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <iostream>
#include <limits>
#include <ios>
#include <iomanip>
#include <numeric>

#include "base_handler.hh"
#include "assert.hh"
#include "parser.hh"
#include "time_profiler.hh"

using namespace std;
using namespace bparser;


class BParserHandler : public BaseHandler {
public:
	BParserHandler() : BaseHandler() {
	    // rewrite power function to correct Bparser format
		powerLine = "2*x + pow(y, 3) + x*(z-y) + 2*pi*z";
        funcPower = "pow(y, 3)";
        // switch off unsupported functionality
        switch_off_func = false;
	}

    void init(int vec_size) override {
    	this->create_data_vectors(vec_size);

    	subsets.resize(vec_size/4);
        for (uint i=0; i<vec_size/4; ++i) {
            subsets[i] = i;
        }
    }

    double parse_vector_fast(std::string expr, std::string tag_name) override {
        double sum = 0.0;

        Parser p(nBulkSize);
        p.parse(expr);
        p.set_variable("x", {}, &x_v[0]);
        p.set_variable("y", {}, &y_v[0]);
        p.set_variable("z", {}, &z_v[0]);
        p.set_variable("_result_", {}, &result_v[0]);
        p.compile();
        p.set_subset(subsets);

        START_TIMER(tag_name, this->nLoops, this->nBulkSize);
        for (int j=0; j<nLoops; ++j) {
            p.run();
        }
        END_TIMER(tag_name, this->nLoops, this->nBulkSize);
        for (int i=0; i<nBulkSize; ++i) sum += result_v[i];

        return sum;
    }

    // data members
    std::vector<uint> subsets;
};

/**
 * Function tests of parsing in bparser:
 *
 * 1) Base expressions
 *  - constant
 *  - simple
 *  - complex
 *  - complex with power function
 *
 * 2) Selected functions
 *  - plus (comparative operation for all other)
 *  - power
 *  - abs
 *  - exp
 *  - log
 *  - sin
 *  - asin
 *  - ternary operator
 *  - max
 */
void parser_run() {
    TimeProfiler::instance().set_parser("bparser");
    BParserHandler pHandler;

    // test of base expressions
    std::cout << "test_of_expressions \n";
    pHandler.run_expression_tests(16);
    pHandler.run_expression_tests(32);
    pHandler.run_expression_tests(64);
    pHandler.run_expression_tests(128);
    pHandler.run_expression_tests(256);
    pHandler.run_expression_tests(512);
    pHandler.run_expression_tests(1024);
    pHandler.run_expression_tests(2048);

    // test of selected functions
    std::cout << "test_of_functions \n";
    pHandler.run_function_tests(128);
    pHandler.run_function_tests(256);
    pHandler.run_function_tests(512);
    pHandler.run_function_tests(1024);

    TimeProfiler::instance().output("bparser");
}



int main()
{
    std::cout << "##################################################################\n";
    parser_run();
    std::cout << "##################################################################\n";
}

#endif /* TEST_BPARSER_CC_ */

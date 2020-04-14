/*
 * test_muparser.hh
 *
 *  Created on: Mar 10, 2020
 *      Author: David Flanderka
 */

#ifndef TEST_MUPARSER_CC_
#define TEST_MUPARSER_CC_

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
#include "muParserTest.h"
#include "time_profiler.hh"

using namespace std;
using namespace mu;


class MuParserHandler : public BaseHandler {
public:
	MuParserHandler() : BaseHandler() {
	    complexLine = "2*x + y*3 + x*(z-y) + 2*_pi*z";
	    powerLine = "2*x + y^3 + x*(z-y) + 2*_pi*z";
	    funcSin = "sin(_pi*y)";
	}

    void init(int vec_size) override {
        create_data_vectors(vec_size);
    }

    double parse_vector_fast(std::string expr, std::string tag_name) override {
        value_type sum = 0.0;

        mu::Parser  parser;
        parser.DefineVar( _T("x"), &(x_v[0]) );
        parser.DefineVar( _T("y"), &(y_v[0]) );
        parser.DefineVar( _T("z"), &(z_v[0]) );
        parser.SetExpr(expr);

        START_TIMER(tag_name, this->nLoops, this->nBulkSize);
        for (int j=0; j<nLoops; ++j) {
            parser.Eval(&(result_v[j]), nBulkSize);
        }
        END_TIMER(tag_name, this->nLoops, this->nBulkSize);
        for (int i=0; i<nBulkSize; ++i) sum += result_v[i];

        return double(sum);
    }

};

/**
 * Function tests of parsing in exprtk:
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
    TimeProfiler::instance().set_parser("muparser");
	// test of base expressions: constant, simple, complex
	MuParserHandler pHandler;

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

    pHandler.output("muparser");

}



int main()
{
    std::cout << "##################################################################\n";
    parser_run();
    std::cout << "##################################################################\n";
}

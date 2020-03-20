/*
 * test_bparser.cc
 *
 *  Created on: Mar 18, 2020
 *      Author: David Flanderka
 */

#ifndef TEST_BPARSER_CC_
#define TEST_BPARSER_CC_

#include <string>
#include <iostream>

#include "bparser_handler.hh"
#include "time_profiler.hh"

using namespace std;
using namespace bparser;


/**
 * Function tests of base expressions parsing in exprtk:
 *  - constant
 *  - simple
 *  - complex
 *  - complex with power function
 *
 * All tests are compared with evaluation of same expression specified by raw C++ code.
 */
void parser_expressions() {
	// test of base expressions: constant, simple, complex
	BParserHandler pHandler;

    pHandler.run_expression_tests(16);
    pHandler.run_expression_tests(32);
    pHandler.run_expression_tests(64);
    pHandler.run_expression_tests(128);
    pHandler.run_expression_tests(256);
    pHandler.run_expression_tests(512);
    pHandler.run_expression_tests(1024);
    pHandler.run_expression_tests(2048);

    TimeProfiler::instance().output("bparser_expr");
}


/**
 * Speed test of selected functions parsing in exprtk:
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
void parser_functions() {
	BParserHandler pHandler;

    pHandler.run_function_tests(128);
    pHandler.run_function_tests(256);
    pHandler.run_function_tests(512);
    pHandler.run_function_tests(1024);

    TimeProfiler::instance().output("bparser_func");
}



int main()
{
    std::cout << "##################################################################\n";
    std::cout << "test_of_expressions \n";
    parser_expressions();
    std::cout << "test_of_functions \n";
    parser_functions();
    std::cout << "##################################################################\n";
}

#endif /* TEST_BPARSER_CC_ */

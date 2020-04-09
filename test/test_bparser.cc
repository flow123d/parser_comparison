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

    TimeProfiler::instance().output("bparser_expr");

    // test of selected functions
    std::cout << "test_of_functions \n";
    pHandler.run_function_tests(128);
    pHandler.run_function_tests(256);
    pHandler.run_function_tests(512);
    pHandler.run_function_tests(1024);

    TimeProfiler::instance().output("bparser_func");
}



int main()
{
    std::cout << "##################################################################\n";
    parser_run();
    std::cout << "##################################################################\n";
}

#endif /* TEST_BPARSER_CC_ */

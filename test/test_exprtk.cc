/*
 * test_parser.hh
 *
 *  Created on: Mar 02, 2020
 *      Author: David Flanderka
 */

#ifndef TEST_EXPRTK_CC_
#define TEST_EXPRTK_CC_

#include <string>
#include <iostream>

#include "exprtk/parser_handler.hh"
#include "time_profiler.hh"

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
	// test of base expressions: constant, simple, complex and complex with power function
    ExprtkParserHandler pHandler;

    START_TIMER("test_16_points");
    pHandler.run_expression_tests(16);
    END_TIMER("test_16_points");

    START_TIMER("test_32_points");
    pHandler.run_expression_tests(32);
    END_TIMER("test_32_points");

    START_TIMER("test_64_points");
    pHandler.run_expression_tests(64);
    END_TIMER("test_64_points");

    START_TIMER("test_128_points");
    pHandler.run_expression_tests(128);
    END_TIMER("test_128_points");

    START_TIMER("test_256_points");
    pHandler.run_expression_tests(256);
    END_TIMER("test_256_points");

    START_TIMER("test_512_points");
    pHandler.run_expression_tests(512);
    END_TIMER("test_512_points");

    START_TIMER("test_1024_points");
    pHandler.run_expression_tests(1024);
    END_TIMER("test_1024_points");

    START_TIMER("test_2048_points");
    pHandler.run_expression_tests(2048);
    END_TIMER("test_2048_points");
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
    ExprtkParserHandler pHandler;

    START_TIMER("test_128_points");
    pHandler.run_function_tests(128);
    END_TIMER("test_128_points");

    START_TIMER("test_256_points");
    pHandler.run_function_tests(256);
    END_TIMER("test_256_points");

    START_TIMER("test_512_points");
    pHandler.run_function_tests(512);
    END_TIMER("test_512_points");

    START_TIMER("test_1024_points");
    pHandler.run_function_tests(1024);
    END_TIMER("test_1024_points");
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


#endif /* TEST_EXPRTK_CC_ */

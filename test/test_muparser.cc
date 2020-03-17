/*
 * test_muparser.hh
 *
 *  Created on: Mar 10, 2020
 *      Author: David Flanderka
 */

#ifndef TEST_MUPARSER_CC_
#define TEST_MUPARSER_CC_

#include <string>
#include <iostream>

#include "muparser_handler.hh"
#include "time_profiler.hh"

using namespace mup;


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
	MuParserHandler pHandler;

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
/*void parser_functions() {
    MuParserHandler pHandler;

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
}*/



int main()
{
    std::cout << "##################################################################\n";
    std::cout << "test_of_expressions \n";
    parser_expressions();
    std::cout << "##################################################################\n";
}

/**
 * MuParserX - example of vector parsing:

ParserX   p_vec;
Value arr1(3, 0);
arr1.At(0) = (float_type)1.11;
arr1.At(1) = (float_type)2.22;
arr1.At(2) = (float_type)3.33;
Value arr2(3, 0);
arr2.At(0) = (float_type)4;
arr2.At(1) = (float_type)5;
arr2.At(2) = (float_type)6;
Value arr3(3, 0);
arr3.At(0) = (float_type)0.1;
arr3.At(1) = (float_type)0.2;
arr3.At(2) = (float_type)0.3;
p_vec.DefineVar("x", Variable(&arr1));
p_vec.DefineVar("y", Variable(&arr2));
p_vec.DefineVar("z", Variable(&arr3));
p_vec.SetExpr("x + 2 * y - z");
Value result_vec = p_vec.Eval();
std::cout << "result_vec: " << result_vec << std::endl;

 */


#endif /* TEST_MUPARSER_CC_ */

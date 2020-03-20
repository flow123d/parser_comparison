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

    pHandler.run_expression_tests(16);
    pHandler.run_expression_tests(32);
    pHandler.run_expression_tests(64);
    pHandler.run_expression_tests(128);
    pHandler.run_expression_tests(256);
    pHandler.run_expression_tests(512);
    pHandler.run_expression_tests(1024);
    pHandler.run_expression_tests(2048);

    TimeProfiler::instance().output("muparser_expr");
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
    MuParserHandler pHandler;

    pHandler.run_function_tests(128);
    pHandler.run_function_tests(256);
    pHandler.run_function_tests(512);
    pHandler.run_function_tests(1024);

    TimeProfiler::instance().output("muparser_func");
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

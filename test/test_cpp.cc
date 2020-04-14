/*
 * test_bparser.cc
 *
 *  Created on: Mar 18, 2020
 *      Author: David Flanderka
 */

#ifndef TEST_CPLUSPLUS_CC_
#define TEST_CPLUSPLUS_CC_

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
#include "time_profiler.hh"

using namespace std;


class CppEvaluationHandler : public BaseHandler {
public:
	CppEvaluationHandler() : BaseHandler() {}

    void init(int vec_size) override {
    	this->create_data_vectors(vec_size);
    }

    void run_expression_tests(int vec_size) override {
        init(vec_size);

        this->cpp_compute_constant();
        this->cpp_compute_simple();
        this->cpp_compute_complex();
        this->cpp_compute_complex_power();
    }

    double parse_vector_fast(std::string expr, std::string tag_name) override {
        return 0.0;
    }

    double cpp_compute_constant() {
    	double sum = 0.0;
        START_TIMER(this->constantLine_tag, this->nLoops, this->nBulkSize);
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = 0.5;
            }
        }
        END_TIMER(this->constantLine_tag, this->nLoops, this->nBulkSize);
        for (int i=0; i<nBulkSize; ++i) {
        	sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_simple() {
    	double sum = 0.0;
        START_TIMER(this->simpleLine_tag, this->nLoops, this->nBulkSize);
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = x_v[i] + y_v[i] + z_v[i];
            }
        }
        END_TIMER(this->simpleLine_tag, this->nLoops, this->nBulkSize);
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_complex() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER(this->complexLine_tag, this->nLoops, this->nBulkSize);
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = 2*x_v[i] + y_v[i]*3 + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER(this->complexLine_tag, this->nLoops, this->nBulkSize);
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_complex_power() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER(this->powerLine_tag, this->nLoops, this->nBulkSize);
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
            	result_v[i] = 2*x_v[i] + pow( y_v[i], 3.0 ) + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER(this->powerLine_tag, this->nLoops, this->nBulkSize);
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

};

/**
 * Function tests of parsing in bparser:
 *
 * Base expressions
 *  - constant
 *  - simple
 *  - complex
 *  - complex with power function
 */
void parser_run() {
	TimeProfiler::instance().set_parser("cpp_expr");
	CppEvaluationHandler pHandler;

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

    TimeProfiler::instance().output("cpp_expr");
}



int main()
{
    std::cout << "##################################################################\n";
    parser_run();
    std::cout << "##################################################################\n";
}

#endif /* TEST_CPLUSPLUS_CC_ */

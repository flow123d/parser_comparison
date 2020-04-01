/*
 * test_parser.hh
 *
 *  Created on: Mar 02, 2020
 *      Author: David Flanderka
 */

#ifndef EXPRTK_PARSER_HANDLER_HH_
#define EXPRTK_PARSER_HANDLER_HH_

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <limits>
#include <ios>
#include <iomanip>
#include <numeric>

#include "base_handler.hh"
#include "exprtk.hpp"


class ExprtkParserHandler : public BaseHandler {
public:
	ExprtkParserHandler() : BaseHandler() {}

    void create_data_vectors(int vec_size) {
        nBulkSize = vec_size;
        nLoops = 5000 * (2048 / vec_size);

        x_v.resize(nBulkSize);
        y_v.resize(nBulkSize);
        z_v.resize(nBulkSize);
        result_v.resize(nBulkSize);

        for (int i=0; i<nBulkSize; ++i) {
            x_v[i] = i;
            y_v[i] = (double)i/100 + 0.01;
            z_v[i] = (4.95 - (double)(i%10) * 1.1) * 0.2; // values oscillate in <-1; 1>
        }
    }

    double cpp_compute_constant() {
    	double sum = 0.0;
        START_TIMER(nBulkSize, "constant_cpp");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = 0.5;
            }
        }
        END_TIMER(nBulkSize, "constant_cpp");
        for (int i=0; i<nBulkSize; ++i) {
        	sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_simple() {
    	double sum = 0.0;
        START_TIMER(nBulkSize, "simple_cpp");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = x_v[i] + y_v[i] + z_v[i];
            }
        }
        END_TIMER(nBulkSize, "simple_cpp");
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_complex() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER(nBulkSize, "complex_cpp");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = 2*x_v[i] + y_v[i]*3 + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER(nBulkSize, "complex_cpp");
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_complex_power() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER(nBulkSize, "power_cpp");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = 2*x_v[i] + pow( y_v[i], 3.0 ) + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER(nBulkSize, "power_cpp");
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double parse_vector_fast(std::string expr, std::string tag_name) override {
        typedef exprtk::symbol_table<double> symbol_table_t;
        typedef exprtk::expression<double>     expression_t;
        typedef exprtk::parser<double>             parser_t;

        std::string expression_string = " result_vec := " + expr + " ";

        exprtk::vector_view<double> x_view = exprtk::make_vector_view(x_v,x_v.size());
        exprtk::vector_view<double> y_view = exprtk::make_vector_view(y_v,y_v.size());
        exprtk::vector_view<double> z_view = exprtk::make_vector_view(z_v,z_v.size());
        exprtk::vector_view<double> r_view = exprtk::make_vector_view(result_v,result_v.size());

        double sum = 0.0;

        symbol_table_t symbol_table;
        symbol_table.add_vector("x",x_view);
        symbol_table.add_vector("y",y_view);
        symbol_table.add_vector("z",z_view);
        symbol_table.add_vector("result_vec",r_view);
        symbol_table.add_constants();

        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(expression_string,expression);

        START_TIMER(nBulkSize, tag_name);
        for (int j=0; j<nLoops; ++j) {
            expression.value();
        }
        END_TIMER(nBulkSize, tag_name);
        for (int i=0; i<nBulkSize; ++i) sum += result_v[i];

        return sum;
    }

    void run_expression_tests(int vec_size) {
        create_data_vectors(vec_size);

        this->cpp_compute_constant();
        this->parse_vector_fast(constantLine, "constant_parser");

        this->cpp_compute_simple();
        this->parse_vector_fast(simpleLine, "simple_parser");

        this->cpp_compute_complex();
        this->parse_vector_fast(complexLine, "complex_parser");

        this->cpp_compute_complex_power();
        this->parse_vector_fast(powerLine, "power_parser");
    }

    void run_function_tests(int vec_size) {
        create_data_vectors(vec_size);

        this->parse_vector_fast(funcPlus, "plus");
        this->parse_vector_fast(funcPower, "power");
        this->parse_vector_fast(funcAbs, "abs");
        this->parse_vector_fast(funcExp, "exp");
        this->parse_vector_fast(funcLog, "log");
        this->parse_vector_fast(funcSin, "sin");
        this->parse_vector_fast(funcAsin, "asin");
        this->parse_vector_fast(funcTernary, "ternary");
        this->parse_vector_fast(funcMax, "max");
    }

    // Data members
    std::vector<double> x_v;
    std::vector<double> y_v;
    std::vector<double> z_v;
    std::vector<double> result_v;

};

#endif /* EXPRTK_PARSER_HANDLER_HH_ */

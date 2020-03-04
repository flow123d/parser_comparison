/*
 * test_parser.hh
 *
 *  Created on: Mar 02, 2020
 *      Author: David Flanderka
 */

#ifndef EXPRTK_PARSER_HANDLER_HH_
#define EXPRTK_PARSER_HANDLER_HH_

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <iostream>
#include <limits>
#include <ios>
#include <iomanip>
#include <numeric>

#include "exprtk/exprtk.hpp"
#include "time_profiler.hh"

using namespace std;


class ParserHandler {
public:
    ParserHandler() {
	    //Profiler::initialize();

	    constantLine = "0.5";
        simpleLine = "x + y + z";
        complexLine = "2*x + y*3 + x*(z-y) + 2*pi*z";
        powerLine = "2*x + y^3 + x*(z-y) + 2*pi*z";

        funcPlus = "y+0.5";
        funcPower = "y^3";
        funcAbs = "abs(z)";
        funcExp = "exp(y)";
        funcLog = "log(y)";
        funcSin = "sin(pi*y)";
        funcAsin = "asin(z)";
        funcTernary = "z>0 ? x : y";
    	funcMax = "max(x,y,z)";
    }

    ~ParserHandler() {
	    //Profiler::uninitialize();
    }

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
        START_TIMER("cpp_compute");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = 0.5;
            }
        }
        END_TIMER("cpp_compute");
        for (int i=0; i<nBulkSize; ++i) {
        	sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_simple() {
    	double sum = 0.0;
        START_TIMER("cpp_compute");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = x_v[i] + y_v[i] + z_v[i];
            }
        }
        END_TIMER("cpp_compute");
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_complex() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER("cpp_compute");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = 2*x_v[i] + y_v[i]*3 + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER("cpp_compute");
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_complex_power() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER("cpp_compute");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = 2*x_v[i] + pow( y_v[i], 3.0 ) + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER("cpp_compute");
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double exprtk_parse_vector_fast(std::string expr) {
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

        START_TIMER("exprtk_parse_vector_fast");
        for (int j=0; j<nLoops; ++j) {
            expression.value();
        }
        END_TIMER("exprtk_parse_vector_fast");
        for (int i=0; i<nBulkSize; ++i) sum += result_v[i];

        return sum;
    }

    void run_expression_tests(int vec_size) {
        create_data_vectors(vec_size);

        START_TIMER("constant_expresions");
        this->cpp_compute_constant();
        this->exprtk_parse_vector_fast(constantLine);
        END_TIMER("constant_expresions");

        START_TIMER("simple_expresions");
        this->cpp_compute_simple();
        this->exprtk_parse_vector_fast(simpleLine);
        END_TIMER("simple_expresions");

        START_TIMER("complex_expresions");
        this->cpp_compute_complex();
        this->exprtk_parse_vector_fast(complexLine);
        END_TIMER("complex_expresions");

        START_TIMER("power_expresions");
        this->cpp_compute_complex_power();
        this->exprtk_parse_vector_fast(powerLine);
        END_TIMER("power_expresions");
    }

    void run_function_tests(int vec_size) {
        create_data_vectors(vec_size);

        START_TIMER("plus_function");
        this->exprtk_parse_vector_fast(funcPlus);
        END_TIMER("plus_function");

        START_TIMER("power_function");
        this->exprtk_parse_vector_fast(funcPower);
        END_TIMER("power_function");

        START_TIMER("abs_function");
        this->exprtk_parse_vector_fast(funcAbs);
        END_TIMER("abs_function");

        START_TIMER("exp_function");
        this->exprtk_parse_vector_fast(funcExp);
        END_TIMER("exp_function");

        START_TIMER("log_function");
        this->exprtk_parse_vector_fast(funcLog);
        END_TIMER("log_function");

        START_TIMER("sin_function");
        this->exprtk_parse_vector_fast(funcSin);
        END_TIMER("sin_function");

        START_TIMER("asin_function");
        this->exprtk_parse_vector_fast(funcAsin);
        END_TIMER("asin_function");

        START_TIMER("ternary_function");
        this->exprtk_parse_vector_fast(funcTernary);
        END_TIMER("ternary_function");

        START_TIMER("max_function");
        this->exprtk_parse_vector_fast(funcMax);
        END_TIMER("max_function");
    }

    // data members
    int nLoops;
    int nBulkSize;

    std::vector<double> x_v;
    std::vector<double> y_v;
    std::vector<double> z_v;
    std::vector<double> result_v;

    std::string constantLine;
    std::string simpleLine;
    std::string complexLine;
    std::string powerLine;

    std::string funcPlus;
    std::string funcPower;
    std::string funcAbs;
    std::string funcExp;
    std::string funcLog;
    std::string funcSin;
    std::string funcAsin;
    std::string funcTernary;
	std::string funcMax;
};

#endif /* EXPRTK_PARSER_HANDLER_HH_ */

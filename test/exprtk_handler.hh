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

    void init(int vec_size) override {
    	this->create_data_vectors(vec_size);
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

};

#endif /* EXPRTK_PARSER_HANDLER_HH_ */

/*
 * bparser_handler.hh
 *
 *  Created on: Mar 19, 2020
 *      Author: David Flanderka
 */

#ifndef BPARSER_HANDLER_HH_
#define BPARSER_HANDLER_HH_

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
#include "assert.hh"
#include "parser.hh"
//#include "test/test_tools.hh"

using namespace std;
using namespace bparser;


class BParserHandler : public BaseHandler {
public:
	BParserHandler() : BaseHandler() {
	    // rewrite power function to correct Bparser format
		powerLine = "2*x + pow(y, 3) + x*(z-y) + 2*pi*z";
        funcPower = "pow(y, 3)";
        // switch off unsupported functionality
        switch_off_func = false;
	}

    void init(int vec_size) override {
    	this->create_data_vectors(vec_size);

    	subsets.resize(vec_size/4);
        for (uint i=0; i<vec_size/4; ++i) {
            subsets[i] = i;
        }
    }

    double parse_vector_fast(std::string expr, std::string tag_name) override {
        double sum = 0.0;

        Parser p(nBulkSize);
        p.parse(expr);
        p.set_variable("x", {}, &x_v[0]);
        p.set_variable("y", {}, &y_v[0]);
        p.set_variable("z", {}, &z_v[0]);
        p.set_variable("_result_", {}, &result_v[0]);
        p.compile();
        p.set_subset(subsets);

        START_TIMER(nBulkSize, tag_name);
        for (int j=0; j<nLoops; ++j) {
            p.run();
        }
        END_TIMER(nBulkSize, tag_name);
        for (int i=0; i<nBulkSize; ++i) sum += result_v[i];

        return sum;
    }

    // data members
    std::vector<uint> subsets;
};

#endif /* BPARSER_HANDLER_HH_ */

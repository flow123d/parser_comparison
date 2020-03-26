# -*- coding: utf-8 -*-
# author:   David Flanderka
# 
# Python script compares speed of BParser and Exprtk parsers with computing in raw C++ code.
# Results are produced for selected expressions and they are printed to graphs.

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.backends.backend_pdf import PdfPages

# read inputs from files
with open("../output/bparser_expr.csv", 'r') as f1:
    df_bparser = pd.read_csv(f1)
with open("../output/exprtk_expr.csv", 'r') as f2:
    df_exprtk = pd.read_csv(f2)
#with open("../output/muparser_expr.csv", 'r') as f3:
#    df_muparser = pd.read_csv(f3)

# milions of samples
m_samples = 5 * 2.048

# values on x-axis
list_matrix =[16, 32, 64, 128, 256, 512, 1024, 2048]
n_items = np.array(list_matrix)
#print(n_items)


# constant expression
bparser_cpp_const = np.array(df_bparser.iloc[2, 1:9], dtype=float) / m_samples
exprtk_cpp_const = np.array(df_exprtk.iloc[2, 1:9], dtype=float) / m_samples
muparser_cpp_const = np.array(df_muparser.iloc[2, 1:9], dtype=float) / m_samples
cpp_const = (bparser_cpp_const + exprtk_cpp_const + muparser_cpp_const) / 3

bparser_parse_const = np.array(df_bparser.iloc[3, 1:9], dtype=float) / m_samples
exprtk_parse_const = np.array(df_exprtk.iloc[3, 1:9], dtype=float) / m_samples
muparser_parse_const = np.array(df_muparser.iloc[3, 1:9], dtype=float) / m_samples

fig0, ax = plt.subplots(figsize=(12,6), dpi=800)

ax.plot(n_items, cpp_const, label="C++ expression")
ax.plot(n_items, bparser_parse_const, label="bparser")
ax.plot(n_items, exprtk_parse_const, label="exprtk")
ax.plot(n_items, muparser_parse_const, label="muparser") # too high values, distorts the results
ax.legend(loc="upper right"); # upper right corner
ax.set_xlabel('$n$ - size of vector ')
ax.set_ylabel("$t$ - time per 10^6 samples")

ax.set_title("constant expression '0.5'");


# simple expression
bparser_cpp_simple = np.array(df_bparser.iloc[6, 1:9], dtype=float) / m_samples
exprtk_cpp_simple = np.array(df_exprtk.iloc[6, 1:9], dtype=float) / m_samples
muparser_cpp_simple = np.array(df_muparser.iloc[6, 1:9], dtype=float) / m_samples
cpp_simple = (bparser_cpp_simple + exprtk_cpp_simple + muparser_cpp_simple) / 3

bparser_parse_simple = np.array(df_bparser.iloc[7, 1:9], dtype=float) / m_samples
exprtk_parse_simple = np.array(df_exprtk.iloc[7, 1:9], dtype=float) / m_samples
muparser_parse_simple = np.array(df_muparser.iloc[7, 1:9], dtype=float) / m_samples

fig1, ax = plt.subplots(figsize=(12,6), dpi=800)

ax.plot(n_items, cpp_simple, label="C++ expression")
ax.plot(n_items, bparser_parse_simple, label="bparser")
ax.plot(n_items, exprtk_parse_simple, label="exprtk")
ax.plot(n_items, muparser_parse_simple, label="muparser") # too high values, distorts the results
ax.legend(loc="upper right"); # upper right corner
ax.set_xlabel('$n$ - size of vector ')
ax.set_ylabel("$t$ - time per 10^6 samples")

ax.set_title("simple expression 'x + y + z'");


# complex expression
bparser_cpp_complex = np.array(df_bparser.iloc[0, 1:9], dtype=float) / m_samples
exprtk_cpp_complex = np.array(df_exprtk.iloc[0, 1:9], dtype=float) / m_samples
muparser_cpp_complex = np.array(df_muparser.iloc[0, 1:9], dtype=float) / m_samples
cpp_complex = (bparser_cpp_complex + exprtk_cpp_complex + muparser_cpp_complex) / 3

bparser_parse_complex = np.array(df_bparser.iloc[1, 1:9], dtype=float) / m_samples
exprtk_parse_complex = np.array(df_exprtk.iloc[1, 1:9], dtype=float) / m_samples
muparser_parse_complex = np.array(df_muparser.iloc[1, 1:9], dtype=float) / m_samples

fig2, ax = plt.subplots(figsize=(12,6), dpi=800)

ax.plot(n_items, cpp_complex, label="C++ expression")
ax.plot(n_items, bparser_parse_complex, label="bparser")
ax.plot(n_items, exprtk_parse_complex, label="exprtk")
ax.plot(n_items, muparser_parse_complex, label="muparser") # too high values, distorts the results
ax.legend(loc="upper right"); # upper right corner
ax.set_xlabel('$n$ - size of vector ')
ax.set_ylabel("$t$ - time per 10^6 samples")

ax.set_title("complex expression '2*x + y*3 + x*(z-y) + 2*pi*z'");


# complex expression with power function
bparser_cpp_power = np.array(df_bparser.iloc[4, 1:9], dtype=float) / m_samples
exprtk_cpp_power = np.array(df_exprtk.iloc[4, 1:9], dtype=float) / m_samples
muparser_cpp_power = np.array(df_muparser.iloc[4, 1:9], dtype=float) / m_samples
cpp_power = (bparser_cpp_power + exprtk_cpp_power + muparser_cpp_power) / 3

bparser_parse_power = np.array(df_bparser.iloc[5, 1:9], dtype=float) / m_samples
exprtk_parse_power = np.array(df_exprtk.iloc[5, 1:9], dtype=float) / m_samples
muparser_parse_power = np.array(df_muparser.iloc[5, 1:9], dtype=float) / m_samples # too high values, distorts the results

fig3, ax = plt.subplots(figsize=(12,6), dpi=800)

ax.plot(n_items, cpp_power, label="C++ expression")
ax.plot(n_items, bparser_parse_power, label="bparser")
ax.plot(n_items, exprtk_parse_power, label="exprtk")
ax.plot(n_items, muparser_parse_power, label="muparser") # too high values, distorts the results
ax.legend(loc="upper right"); # upper right corner
ax.set_xlabel('$n$ - size of vector ')
ax.set_ylabel("$t$ - time per 10^6 samples")

ax.set_title("power expression '2*x + y^3 + x*(z-y) + 2*pi*z'");


# print to pdf
pp = PdfPages('parse_expressions.pdf')
pp.savefig(fig0)
pp.savefig(fig1)
pp.savefig(fig2)
pp.savefig(fig3)
pp.close()

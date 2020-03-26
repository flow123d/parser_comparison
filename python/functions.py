# -*- coding: utf-8 -*-
# author:   David Flanderka
# 
# Python script compares speed of parsing of different functions with Bparser, Exprtk and MuparserX.
# Results are produced for individual parsers to separate graphs.

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.backends.backend_pdf import PdfPages

# read inputs from files
with open("../output/bparser_func.csv", 'r') as f1:
    df_bparser = pd.read_csv(f1)
with open("../output/exprtk_func.csv", 'r') as f2:
    df_exprtk = pd.read_csv(f2)
with open("../output/muparser_func.csv", 'r') as f3:
    df_muparser = pd.read_csv(f3)

# milions of samples
m_samples = 5 * 2.048

# values on x-axis
list_matrix =[128, 256, 512, 1024]
n_items = np.array(list_matrix)


# bparser test
bparser_plus = np.array(df_bparser.iloc[4, 1:5], dtype=float) / m_samples
bparser_power = np.array(df_bparser.iloc[5, 1:5], dtype=float) / m_samples
bparser_abs = np.array(df_bparser.iloc[0, 1:5], dtype=float) / m_samples
bparser_exp = np.array(df_bparser.iloc[2, 1:5], dtype=float) / m_samples
bparser_log = np.array(df_bparser.iloc[3, 1:5], dtype=float) / m_samples
bparser_sin = np.array(df_bparser.iloc[6, 1:5], dtype=float) / m_samples
bparser_asin = np.array(df_bparser.iloc[1, 1:5], dtype=float) / m_samples

fig0, ax = plt.subplots(figsize=(12,15), dpi=800)

ax.plot(n_items, bparser_plus, label="y+0.5")
ax.plot(n_items, bparser_power, label="y^3")
ax.plot(n_items, bparser_abs, label="abs(z)")
ax.plot(n_items, bparser_exp, label="exp(y)")
ax.plot(n_items, bparser_log, label="log(y)")
ax.plot(n_items, bparser_sin, label="sin(pi*y)")
ax.plot(n_items, bparser_asin, label="asin(z)")
ax.legend(loc="upper right"); # upper right corner
ax.set_xlabel('$n$ - size of vector ')
ax.set_ylabel("$t$ - time per 10^6 samples")

ax.set_title("BParser test");


# exprtk test
exprtk_plus = np.array(df_exprtk.iloc[5, 1:5], dtype=float) / m_samples
exprtk_power = np.array(df_exprtk.iloc[6, 1:5], dtype=float) / m_samples
exprtk_abs = np.array(df_exprtk.iloc[0, 1:5], dtype=float) / m_samples
exprtk_exp = np.array(df_exprtk.iloc[2, 1:5], dtype=float) / m_samples
exprtk_log = np.array(df_exprtk.iloc[3, 1:5], dtype=float) / m_samples
exprtk_sin = np.array(df_exprtk.iloc[7, 1:5], dtype=float) / m_samples
exprtk_asin = np.array(df_exprtk.iloc[1, 1:5], dtype=float) / m_samples
exprtk_ternary = np.array(df_exprtk.iloc[8, 1:5], dtype=float) / m_samples
exprtk_max = np.array(df_exprtk.iloc[4, 1:5], dtype=float) / m_samples

fig1, ax = plt.subplots(figsize=(12,15), dpi=800)

ax.plot(n_items, exprtk_plus, label="y+0.5")
ax.plot(n_items, exprtk_power, label="y^3")
ax.plot(n_items, exprtk_abs, label="abs(z)")
ax.plot(n_items, exprtk_exp, label="exp(y)")
ax.plot(n_items, exprtk_log, label="log(y)")
ax.plot(n_items, exprtk_sin, label="sin(pi*y)")
ax.plot(n_items, exprtk_asin, label="asin(z)")
ax.plot(n_items, exprtk_ternary, label="z>0 ? x : y")
ax.plot(n_items, exprtk_max, label="max(x,y,z)")
ax.legend(loc="upper right"); # upper right corner
ax.set_xlabel('$n$ - size of vector ')
ax.set_ylabel("$t$ - time per 10^6 samples")

ax.set_title("Exprtk test");


# muparser test
muparser_plus = np.array(df_muparser.iloc[5, 1:5], dtype=float) / m_samples
muparser_power = np.array(df_muparser.iloc[6, 1:5], dtype=float) / m_samples
muparser_abs = np.array(df_muparser.iloc[0, 1:5], dtype=float) / m_samples
muparser_exp = np.array(df_muparser.iloc[2, 1:5], dtype=float) / m_samples
muparser_log = np.array(df_muparser.iloc[3, 1:5], dtype=float) / m_samples
muparser_sin = np.array(df_muparser.iloc[7, 1:5], dtype=float) / m_samples
muparser_asin = np.array(df_muparser.iloc[1, 1:5], dtype=float) / m_samples
muparser_ternary = np.array(df_muparser.iloc[8, 1:5], dtype=float) / m_samples
muparser_max = np.array(df_muparser.iloc[4, 1:5], dtype=float) / m_samples

fig2, ax = plt.subplots(figsize=(12,15), dpi=800)

ax.plot(n_items, muparser_plus, label="y+0.5")
ax.plot(n_items, muparser_power, label="y^3")
ax.plot(n_items, muparser_abs, label="abs(z)")
ax.plot(n_items, muparser_exp, label="exp(y)")
ax.plot(n_items, muparser_log, label="log(y)")
ax.plot(n_items, muparser_sin, label="sin(pi*y)")
ax.plot(n_items, muparser_asin, label="asin(z)")
ax.plot(n_items, muparser_ternary, label="z>0 ? x : y")
ax.plot(n_items, muparser_max, label="max(x,y,z)")
ax.legend(loc="upper right"); # upper right corner
ax.set_xlabel('$n$ - size of vector ')
ax.set_ylabel("$t$ - time per 10^6 samples")

ax.set_title("MuparserX test");


# print to pdf
pp = PdfPages('parse_functions.pdf')
pp.savefig(fig0)
pp.savefig(fig1)
pp.savefig(fig2)
pp.close()

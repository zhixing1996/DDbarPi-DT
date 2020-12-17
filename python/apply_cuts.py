#!/usr/bin/env python
"""
Apply cuts on root files
"""

__author__ = "Maoqiang JING <jingmq@ihep.ac.cn>"
__copyright__ = "Copyright (c) Maoqiang JING"
__created__ = "[2019-10-03 Tue 17:06]"

import math
from array import array
import ROOT
from ROOT import TCanvas, gStyle, TLorentzVector, TTree
from ROOT import TFile, TH1F, TLegend, TArrow, TChain, TVector3
import sys, os
import logging
from math import *
from tools import width
logging.basicConfig(level=logging.DEBUG, format=' %(asctime)s - %(levelname)s- %(message)s')

def usage():
    sys.stdout.write('''
NAME
    apply_cuts.py

SYNOPSIS
    ./apply_cuts.py [file_in] [file_out] [sample] [mode]

AUTHOR
    Maoqiang JING <jingmq@ihep.ac.cn>

DATE
    October 2019
\n''')

def save(file_in, file_out, sample, mode):
    try:
        chain = TChain('save')
        chain.Add(file_in)
    except:
        logging.error(file_in + ' is invalid!')
        sys.exit()

    cut = ''
    D0_W = width('D0')
    Dm_W = width('Dm')
    D0_signal_low = 1.86483 - D0_W/2.
    D0_signal_up = 1.86483 + D0_W/2.
    D0_sidebandlow_up = D0_signal_low - D0_W
    D0_sidebandlow_low = D0_signal_low - 2. * D0_W
    D0_sidebandup_low = D0_signal_up + D0_W
    D0_sidebandup_up = D0_signal_up + 2. * D0_W
    Dm_signal_low = 1.86965 - Dm_W/2.
    Dm_signal_up = 1.86965 + Dm_W/2.
    Dm_sidebandlow_up = Dm_signal_low - Dm_W
    Dm_sidebandlow_low = Dm_signal_low - 2. * Dm_W
    Dm_sidebandup_low = Dm_signal_up + Dm_W
    Dm_sidebandup_up = Dm_signal_up + 2. * Dm_W
    if mode == 'after_mass':
        if sample == 'signal':
            cut_D0Dm = '(m_m_D0 > ' + str(D0_signal_low) + ' && m_m_D0 < ' + str(D0_signal_up) + ' && m_m_Dm > ' + str(Dm_signal_low) + ' && m_m_Dm < ' + str(Dm_signal_up) + ')'
            cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86483 > 2.030)'
            cut_chi2 = '(m_chi2_4c < 60)'
            cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_chi2
        if sample == 'side1_low':
            cut_D0Dm = '(m_m_D0 > ' + str(D0_signal_low) + ' && m_m_D0 < ' + str(D0_signal_up) + ' && (m_m_Dm > ' + str(Dm_sidebandlow_low) + ' && m_m_Dm < ' + str(Dm_sidebandlow_up) + '))'
            cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86483 > 2.030)'
            cut_chi2 = '(m_chi2_4c < 60)'
            cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_chi2
        if sample == 'side1_up':
            cut_D0Dm = '(m_m_D0 > ' + str(D0_signal_low) + ' && m_m_D0 < ' + str(D0_signal_up) + ' && (m_m_Dm > ' + str(Dm_sidebandup_low) + ' && m_m_Dm < ' + str(Dm_sidebandup_up) + '))'
            cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86483 > 2.030)'
            cut_chi2 = '(m_chi2_4c < 60)'
            cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_chi2
        if sample == 'side2_low':
            cut_D0Dm = '(m_m_Dm > ' + str(Dm_signal_low) + ' && m_m_Dm < ' + str(Dm_signal_up) + ' && (m_m_D0 > ' + str(D0_sidebandlow_low) + ' && m_m_D0 < ' + str(D0_sidebandlow_up) + '))'
            cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86483 > 2.030)'
            cut_chi2 = '(m_chi2_4c < 60)'
            cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_chi2
        if sample == 'side2_up':
            cut_D0Dm = '(m_m_Dm > ' + str(Dm_signal_low) + ' && m_m_Dm < ' + str(Dm_signal_up) + ' && (m_m_D0 > ' + str(D0_sidebandup_low) + ' && m_m_D0 < ' + str(D0_sidebandup_up) + '))'
            cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86483 > 2.030)'
            cut_chi2 = '(m_chi2_4c < 60)'
            cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_chi2
        if sample == 'side3_low':
            cut_D0Dm = '(m_m_D0 > ' + str(D0_sidebandlow_low) + ' && m_m_D0 < ' + str(D0_sidebandlow_up) + ' && (m_m_Dm > ' + str(Dm_sidebandlow_low) + ' && m_m_Dm < ' + str(Dm_sidebandlow_up) + '))'
            cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86483 > 2.030)'
            cut_chi2 = '(m_chi2_4c < 60)'
            cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_chi2
        if sample == 'side3_up':
            cut_D0Dm = '(m_m_D0 > ' + str(D0_sidebandlow_low) + ' && m_m_D0 < ' + str(D0_sidebandlow_up) + ' && (m_m_Dm > ' + str(Dm_sidebandup_low) + ' && m_m_Dm < ' + str(Dm_sidebandup_up) + '))'
            cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86483 > 2.030)'
            cut_chi2 = '(m_chi2_4c < 60)'
            cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_chi2
        if sample == 'side4_low':
            cut_D0Dm = '(m_m_D0 > ' + str(D0_sidebandup_low) + ' && m_m_D0 < ' + str(D0_sidebandup_up) + ' && (m_m_Dm > ' + str(Dm_sidebandlow_low) + ' && m_m_Dm < ' + str(Dm_sidebandlow_up) + '))'
            cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86483 > 2.030)'
            cut_chi2 = '(m_chi2_4c < 60)'
            cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_chi2
        if sample == 'side4_up':
            cut_D0Dm = '(m_m_D0 > ' + str(D0_sidebandup_low) + ' && m_m_D0 < ' + str(D0_sidebandup_up) + ' && (m_m_Dm > ' + str(Dm_sidebandup_low) + ' && m_m_Dm < ' + str(Dm_sidebandup_up) + '))'
            cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86483 > 2.030)'
            cut_chi2 = '(m_chi2_4c < 60)'
            cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_chi2
    if mode == 'before_mass':
        cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86483 > 2.030)'
        cut_chi2 = '(m_chi2_4c < 60)'
        cut = cut_DstD + ' && ' + cut_chi2

    t = chain.CopyTree(cut)
    t.SaveAs(file_out)

def main():
    args = sys.argv[1:]
    if len(args)<4:
        return usage()
    file_in = args[0]
    file_out = args[1]
    sample = args[2]
    mode = args[3]

    print '--> Begin to process file: ' + file_in
    save(file_in, file_out, sample, mode)
    print '--> End of processing file: ' + file_out

if __name__ == '__main__':
    main()

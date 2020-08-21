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
logging.basicConfig(level=logging.DEBUG, format=' %(asctime)s - %(levelname)s- %(message)s')

def usage():
    sys.stdout.write('''
NAME
    apply_cuts.py

SYNOPSIS
    ./apply_cuts.py [file_in] [file_out] [sample]

AUTHOR
    Maoqiang JING <jingmq@ihep.ac.cn>

DATE
    October 2019
\n''')

def save(file_in, file_out, sample):
    try:
        chain = TChain('save')
        chain.Add(file_in)
    except:
        logging.error(file_in + ' is invalid!')
        sys.exit()

    cut = ''
    if sample == 'signal':
        cut_D0Dm = '(m_m_D0 > 1.85 && m_m_D0 < 1.885 && m_m_Dm > 1.85 && m_m_Dm < 1.885)'
        cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86486 > 2.030)'
        cut_DDbar = '(m_m_D0Dm < 4.4)'
        cut_chi2 = '(chi2_4c < 60)'
        cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_DDbar + ' && ' + cut_chi2
    if sample == 'side1':
        cut_D0Dm = '(m_m_D0 > 1.85 && m_m_D0 < 1.885 && ((m_m_Dm > 1.798 && m_m_Dm < 1.833) || (m_m_Dm > 1.902 && m_m_Dm < 1.937)))'
        cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86486 > 2.030)'
        cut_DDbar = '(m_m_D0Dm < 4.4)'
        cut_chi2 = '(chi2_4c < 60)'
        cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_DDbar + ' && ' + cut_chi2
    if sample == 'side2':
        cut_D0Dm = '(m_m_Dm > 1.85 && m_m_Dm < 1.885 && ((m_m_D0 > 1.798 && m_m_D0 < 1.833) || (m_m_D0 > 1.902 && m_m_D0 < 1.937)))'
        cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86486 > 2.030)'
        cut_DDbar = '(m_m_D0Dm < 4.4)'
        cut_chi2 = '(chi2_4c < 60)'
        cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_DDbar + ' && ' + cut_chi2
    if sample == 'side3':
        cut_D0Dm = '(m_m_D0 > 1.798 && m_m_D0 < 1.833 && ((m_m_Dm > 1.798 && m_m_Dm < 1.833) || (m_m_Dm > 1.902 && m_m_Dm < 1.937)))'
        cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86486 > 2.030)'
        cut_DDbar = '(m_m_D0Dm < 4.4)'
        cut_chi2 = '(chi2_4c < 60)'
        cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_DDbar + ' && ' + cut_chi2
    if sample == 'side4':
        cut_D0Dm = '(m_m_D0 > 1.902 && m_m_D0 < 1.937 && ((m_m_Dm > 1.798 && m_m_Dm < 1.833) || (m_m_Dm > 1.902 && m_m_Dm < 1.937)))'
        cut_DstD = '(m_m_D0pip - m_m_D0 + 1.86486 > 2.030)'
        cut_DDbar = '(m_m_D0Dm < 4.4)'
        cut_chi2 = '(chi2_4c < 60)'
        cut = cut_D0Dm + ' && ' + cut_DstD + ' && ' + cut_DDbar + ' && ' + cut_chi2

    t = chain.CopyTree(cut)
    t.SaveAs(file_out)

def main():
    args = sys.argv[1:]
    if len(args)<3:
        return usage()
    file_in = args[0]
    file_out = args[1]
    sample = args[2]

    print '--> Begin to process file: ' + file_in
    save(file_in, file_out, sample)
    print '--> End of processing file: ' + file_out

if __name__ == '__main__':
    main()

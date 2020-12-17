#!/usr/bin/env python
"""
Plot mixing of D0 and Dm
"""

__author__ = "Maoqiang JING <jingmq@ihep.ac.cn>"
__copyright__ = "Copyright (c) Maoqiang JING"
__created__ = "[2020-08-25 Tue 08:52]"

from ROOT import *
import sys, os
import logging
from math import *
from tools import convert_name
logging.basicConfig(level=logging.DEBUG, format=' %(asctime)s - %(levelname)s- %(message)s')
gStyle.SetOptTitle(0) # quench title
gStyle.SetPadTickX(1) # dicide on boxing on or not of x and y axis  
gStyle.SetPadTickY(1) # dicide on boxing on or not of x and y axis
gStyle.SetOptStat(0)
gStyle.SetLabelColor(1, 'X')
gStyle.SetLabelColor(1, 'Y')
gStyle.SetLabelSize(0.03, 'XY')
gStyle.SetTitleOffset(1.1, 'Y')
gStyle.SetHistLineColor(1)
gStyle.SetHistLineWidth(3)
gStyle.SetTitleSize(0.05, 'xyz')
gStyle.SetErrorX(0)
gStyle.SetEndErrorSize(0)
gStyle.SetPadLeftMargin(0.17);
gStyle.SetPadBottomMargin(0.17);
gStyle.SetPadRightMargin(0.17);
gStyle.SetPadTopMargin(0.17);

def usage():
    sys.stdout.write('''
NAME
    plot_gen_rec.py

SYNOPSIS
    ./plot_gen_rec.py [ecms]

AUTHOR
    Maoqiang JING <jingmq@ihep.ac.cn>

DATE
    August 2020
\n''')

def set_pavetext(pt):
    pt.SetFillStyle(0)
    pt.SetBorderSize(0)
    pt.SetTextAlign(10)
    pt.SetTextSize(0.05)

def D_mode_fill(t, h, rec_mode, gen_mode):
    for ientry in xrange(t.GetEntries()):
        t.GetEntry(ientry)
        npi_D0 = 0
        nK_D0 = 0
        npi0_D0 = 0
        npi_Dp = 0
        nK_Dp = 0
        npi0_Dp = 0
        nKS_Dp = 0
        tagD0 = -999
        tagDp = -999
        rec_tag = -999
        gen_tag = -999
        for j in xrange(t.indexmc):
            if t.motherid[j] == 421 or t.motherid[j] == -421:
                if t.pdgid[j] == 111:
                    npi0_D0 += 1
                if t.pdgid[j] == 211 or t.pdgid[j] == -211:
                    npi_D0 += 1
                if t.pdgid[j] == 321 or t.pdgid[j] == -321:
                    nK_D0 += 1
            if t.motherid[j] == 411 or t.motherid[j] == -411:
                if t.pdgid[j] == 111:
                    npi0_Dp += 1
                if t.pdgid[j] == 211 or t.pdgid[j] == -211:
                    npi_Dp += 1
                if t.pdgid[j] == 321 or t.pdgid[j] == -321:
                    nK_Dp += 1
                if t.pdgid[j] == 310:
                    nKS_Dp += 1
            if t.motherid[j] == 310:
                if t.pdgid[j] == 211 or t.pdgid[j] == -211:
                    npi_Dp += 1
        if npi0_D0 == 0 and npi_D0 == 1 and nK_D0 == 1:
            tagD0 = 0
        if npi0_D0 == 1 and npi_D0 == 1 and nK_D0 == 1:
            tagD0 = 1
        if npi0_D0 == 0 and npi_D0 == 3 and nK_D0 == 1:
            tagD0 = 3
        if npi0_Dp == 0 and npi_Dp == 2 and nK_Dp == 1 and nKS_Dp == 0:
            tagDp = 200
        if npi0_Dp == 1 and npi_Dp == 2 and nK_Dp == 1 and nKS_Dp == 0:
            tagDp = 201
        if (npi0_Dp == 0 and npi_Dp == 3 and nK_Dp == 0 and nKS_Dp == 1) or (npi0_Dp == 0 and npi_Dp == 1 and nK_Dp == 0 and nKS_Dp == 1):
            tagDp = 202
        if npi0_Dp == 1 and npi_Dp == 3 and nK_Dp == 0 and nKS_Dp == 1:
            tagDp = 203
        if npi0_Dp == 0 and npi_Dp == 5 and nK_Dp == 0 and nKS_Dp == 1:
            tagDp = 204
        if tagD0 == 0:
            gen_tag = tagDp - 200 + 1
        elif tagD0 == 1:
            gen_tag = tagDp - 200 + 6
        elif tagD0 == 3:
            gen_tag = tagDp - 200 + 11
        if int(t.m_D0_mode) == 0:
            rec_tag = int(t.m_Dm_mode) - 200 + 1
        elif int(t.m_D0_mode) == 1:
            rec_tag = int(t.m_Dm_mode) - 200 + 6
        elif int(t.m_D0_mode) == 3:
            rec_tag = int(t.m_Dm_mode) - 200 + 11
        if rec_tag == -999 or gen_tag == -999:
            continue
        h.Fill(rec_mode[rec_tag - 1], gen_mode[gen_tag - 1], 1)

def set_histo_style(h):
    h.SetBit(TH1.kCanRebin)
    h.SetStats(0)
    h.GetXaxis().SetNdivisions(521)
    h.GetYaxis().SetNdivisions(521)
    h.LabelsDeflate('X')
    h.LabelsDeflate('Y')

def set_canvas_style(mbc):
    mbc.SetFillColor(0)
    mbc.SetLeftMargin(0.17)
    mbc.SetBottomMargin(0.17)
    mbc.SetRightMargin(0.05)
    mbc.SetTopMargin(0.05)
    mbc.SetGrid()

def plot(path, ecms):
    try:
        f_MC = TFile(path[0])
        t_MC = f_MC.Get('save')
        entries_MC = t_MC.GetEntries()
        logging.info('MC(DDPI) entries :'+str(entries_MC))
    except:
        logging.error(path[0] + ' is invalid!')
        sys.exit()

    mbc = TCanvas('mbc', 'mbc', 0, 0, 900, 700)
    set_canvas_style(mbc)
    
    rec_mode = ['rec(0, 200)', 'rec(0, 201)', 'rec(0, 202)', 'rec(0, 203)', 'rec(0, 204)', 'rec(1, 200)', 'rec(1, 201)', 'rec(1, 202)', 'rec(1, 203)', 'rec(1, 204)', 'rec(3, 200)', 'rec(3, 201)', 'rec(3, 202)', 'rec(3, 203)', 'rec(3, 204)']
    gen_mode = ['gen(0, 200)', 'gen(0, 201)', 'gen(0, 202)', 'gen(0, 203)', 'gen(0, 204)', 'gen(1, 200)', 'gen(1, 201)', 'gen(1, 202)', 'gen(1, 203)', 'gen(1, 204)', 'gen(3, 200)', 'gen(3, 201)', 'gen(3, 202)', 'gen(3, 203)', 'gen(3, 204)']

    h_mode = TH2F('h_mode', 'h_mode', 15, 0, 15, 15, 0, 15)
    set_histo_style(h_mode)
    D_mode_fill(t_MC, h_mode, rec_mode, gen_mode)

    h_mode.Draw('TEXT')

    pt = TPaveText(0.2, 0.6, 0.35, 0.75, "BRNDC")
    set_pavetext(pt)
    pt.Draw()
    pt.AddText('MC: 4680 MeV')

    if not os.path.exists('./figs/'):
        os.makedirs('./figs/')

    mbc.SaveAs('./figs/D_mode_'+str(ecms)+'.pdf')

    raw_input('Press <Enter> to end...')

if __name__ == '__main__':
    args = sys.argv[1:]
    if len(args)<1:
        usage()
        sys.exit()
    ecms = int(args[0])

    path =[]
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/DDPI/'+str(ecms)+'/mc_DDPI_'+str(ecms)+'_signal.root')
    plot(path, ecms)

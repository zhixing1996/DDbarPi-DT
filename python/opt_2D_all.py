#!/usr/bin/env python
"""
Optiomize invariant mass of Dm and invariant mass of D0
"""

__author__ = "Maoqiang JING <jingmq@ihep.ac.cn>"
__copyright__ = "Copyright (c) Maoqiang JING"
__created__ = "[2020-08-31 Mon 15:35]"

import ROOT
from ROOT import TCanvas, gStyle
from ROOT import TFile, TH2F, TPaveText, TArrow
import sys, os
import logging
from math import *
from tools import width
logging.basicConfig(level=logging.DEBUG, format=' %(asctime)s - %(levelname)s- %(message)s')
gStyle.SetPaperSize(20,30)
gStyle.SetPadTopMargin(0.05)
gStyle.SetPadRightMargin(0.08)
gStyle.SetPadBottomMargin(0.18)
gStyle.SetPadLeftMargin(0.16)
gStyle.SetOptTitle(0)
gStyle.SetOptTitle(0)

def usage():
    sys.stdout.write('''
NAME
    opt_2D_all.py

SYNOPSIS
    ./opt_2D_all.py [ecms]

AUTHOR
    Maoqiang JING <jingmq@ihep.ac.cn>

DATE
    August 2020
\n''')

def set_pavetext(pt):
    pt.SetFillStyle(0)
    pt.SetBorderSize(0)
    pt.SetTextAlign(10)
    pt.SetTextSize(0.06)

def set_arrow(arrow, color):
    arrow.SetLineWidth(4)
    arrow.SetLineColor(color)
    arrow.SetFillColor(color)

def set_histo_style(h, xtitle, ytitle):
    h.GetXaxis().SetNdivisions(509)
    h.GetYaxis().SetNdivisions(504)
    h.SetLineWidth(2)
    h.SetLineWidth(2)
    h.SetStats(0)
    h.SetStats(0)
    h.GetXaxis().SetTitleSize(0.06)
    h.GetXaxis().SetTitleOffset(1.12)
    h.GetXaxis().SetLabelOffset(0.01)
    h.GetYaxis().SetTitleSize(0.06)
    h.GetYaxis().SetTitleOffset(1.12)
    h.GetYaxis().SetLabelOffset(0.01)
    h.GetXaxis().SetTitle(xtitle)
    h.GetXaxis().CenterTitle()
    h.GetYaxis().SetTitle(ytitle)
    h.GetYaxis().CenterTitle()
    h.SetMarkerStyle(8)
    h.SetMarkerSize(0.65)
    h.SetLineColor(1)

def cal_significance(t1, t2):
    xbins, xmin, xmax = 250, 0.0, 0.3
    ybins, ymin, ymax = 250, 0.0, 0.35
    h_FOM = TH2F('h_FOM', 'h_FOM', xbins, xmin, xmax, ybins, ymin, ymax)
    xtitle = '2*|M(D^{-}) - m_{D^{-}}| (GeV)'
    ytitle = '2*|M(D^{0}) - m_{D^{0}}| (GeV)'
    set_histo_style(h_FOM, xtitle, ytitle)
    for xbin in xrange(xbins):
        for ybin in xrange(ybins):
            w_m_Dm_max = (xbin + 1) * (xmax - xmin) / float(xbins) + xmin
            w_m_D0_max = (ybin + 1) * (ymax - ymin) / float(ybins) + ymin
            m_Dm = 1.86965
            m_D0 = 1.86483
            m_Dm_low = m_Dm - w_m_Dm_max/2.
            m_Dm_up = m_Dm + w_m_Dm_max/2.
            m_D0_low = m_D0 - w_m_D0_max/2.
            m_D0_up = m_D0 + w_m_D0_max/2.
            n_data = float(t1.GetEntries('(m_m_Dm > %.5f && m_m_Dm < %.5f) && (m_m_D0 > %.5f && m_m_D0 < %.5f)' %(m_Dm_low, m_Dm_up, m_D0_low, m_D0_up)))
            n_sigMC = float(t2.GetEntries('(m_m_Dm > %.5f && m_m_Dm < %.5f) && (m_m_D0 > %.5f && m_m_D0 < %.5f)' %(m_Dm_low, m_Dm_up, m_D0_low, m_D0_up)))
            if not n_data == 0:
                FOM = (n_sigMC)/sqrt(n_data)
                print '(xbin: %i/%i, ybin, %i/%i): M(Dm)(%5f), M(D0)(%5f), FOM(%3f)' %(xbin, xbins, ybin, ybins, w_m_Dm_max, w_m_D0_max, FOM)
                h_FOM.Fill(w_m_Dm_max - 0.5*(xmax - xmin) / float(xbins), w_m_D0_max - 0.5*(ymax - ymin) / float(ybins), FOM)
    return h_FOM

def set_canvas_style(mbc):
    mbc.SetFillColor(0)
    mbc.SetLeftMargin(0.15)
    mbc.SetRightMargin(0.15)
    mbc.SetTopMargin(0.1)
    mbc.SetBottomMargin(0.15)

def plot(path, pt_title, ecms):
    try:
        f_data = TFile(path[0])
        f_sigMC = TFile(path[1])
        t_data = f_data.Get('save')
        t_sigMC = f_sigMC.Get('save')
        entries_data = t_data.GetEntries()
        entries_sigMC = t_sigMC.GetEntries()
        logging.info('data entries :'+str(entries_data))
        logging.info('MC(DDPI) entries :'+str(entries_sigMC))
    except:
        logging.error('Files are invalid!')
        sys.exit()

    mbc = TCanvas('mbc', 'mbc', 800, 600)
    set_canvas_style(mbc)

    if not os.path.exists('./figs/'):
        os.makedirs('./figs/')

    h_FOM = cal_significance(t_data, t_sigMC)
    h_FOM.Draw('col')

    pt = TPaveText(0.6, 0.6, 0.85, 0.85, "BRNDC")
    set_pavetext(pt)
    pt.Draw()
    pt.AddText(pt_title)

    D0_W = width('D0')
    Dm_W = width('Dm')
    Arr_Dm = TArrow(Dm_W, 0, Dm_W, 0.35, 0.01, '')
    set_arrow(Arr_Dm, 1)
    Arr_Dm.Draw()
    Arr_D0 = TArrow(0, D0_W, 0.3, D0_W, 0.01, '')
    set_arrow(Arr_D0, 1)
    Arr_D0.Draw()

    mbc.RedrawAxis()
    mbc.Update()
    mbc.SaveAs('./figs/opt_'+str(ecms)+'_2D.pdf')

    raw_input('Enter anything to end...')

def main():
    args = sys.argv[1:]
    if len(args)<1:
        return usage()
    ecms = args[0]

    path = []
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data/'+str(ecms)+'/data_'+str(ecms)+'.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/DDPI/'+str(ecms)+'/mc_DDPI_'+str(ecms)+'.root')
    pt_title = str(ecms) + ' MeV'
    plot(path, pt_title, ecms)

if __name__ == '__main__':
    main()

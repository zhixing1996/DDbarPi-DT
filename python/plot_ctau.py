#!/usr/bin/env python
"""
Plot ctau of D0 and D- after second vertex fit
"""

__author__ = "Maoqiang JING <jingmq@ihep.ac.cn>"
__copyright__ = "Copyright (c) Maoqiang JING"
__created__ = "[2020-12-22 Tue 20:56]"

import ROOT
from ROOT import *
import sys, os
import logging
from math import *
logging.basicConfig(level=logging.DEBUG, format=' %(asctime)s - %(levelname)s- %(message)s')
gStyle.SetOptTitle(0) # quench title
gStyle.SetPadTickX(1) # dicide on boxing on or not of x and y axis  
gStyle.SetPadTickY(1) # dicide on boxing on or not of x and y axis

def usage():
    sys.stdout.write('''
NAME
    plot_ctau.py

SYNOPSIS
    ./plot_ctau.py [ecms] [D_type]

AUTHOR
    Maoqiang JING <jingmq@ihep.ac.cn>

DATE
    December 2020
\n''')

def set_legend(legend, h1, h2, title):
    legend.AddEntry(h1, 'data')
    legend.AddEntry(h2, 'sideband')
    legend.SetHeader(title)
    legend.SetBorderSize(0)
    legend.SetFillColor(0)
    legend.SetLineColor(0)
    legend.SetTextSize(0.06)

def set_pavetext(pt):
    pt.SetFillStyle(0)
    pt.SetBorderSize(0)
    pt.SetTextAlign(10)
    pt.SetTextSize(0.05)

def ctau_fill(t, h, D_type):
    for ientry in xrange(t.GetEntries()):
        t.GetEntry(ientry)
        if D_type == 'D0': h.Fill(t.m_ctau_svf_D0)
        if D_type == 'Dm': h.Fill(t.m_ctau_svf_Dm)

def set_histo_style(h, xtitle, ytitle, color, fill_style):
    h.GetXaxis().SetNdivisions(509)
    h.GetYaxis().SetNdivisions(504)
    h.SetLineWidth(2)
    h.SetLineWidth(2)
    h.SetStats(0)
    h.SetStats(0)
    if not fill_style == -1:
        h.SetFillStyle(fill_style) 
        h.SetFillColor(color)
    h.GetXaxis().SetTitleSize(0.06)
    h.GetXaxis().SetTitleOffset(1.)
    h.GetXaxis().SetLabelOffset(0.01)
    h.GetYaxis().SetTitleSize(0.06)
    h.GetYaxis().SetTitleOffset(1.)
    h.GetYaxis().SetLabelOffset(0.01)
    h.GetXaxis().SetTitle(xtitle)
    h.GetXaxis().CenterTitle()
    h.GetYaxis().SetTitle(ytitle)
    h.GetYaxis().CenterTitle()
    h.SetLineColor(color)

def set_canvas_style(mbc):
    mbc.SetFillColor(0)
    mbc.SetLeftMargin(0.15)
    mbc.SetRightMargin(0.15)
    mbc.SetTopMargin(0.1)
    mbc.SetBottomMargin(0.15)

def plot(path, leg_title, ecms, xmin, xmax, xbins, D_type):
    try:
        f_data = TFile(path[0])
        t_data = f_data.Get('save')
        entries_data = t_data.GetEntries()
        logging.info('data entries :'+str(entries_data))
    except:
        logging.error(path[0] + ' is invalid!')
        sys.exit()
    try:
        f_side1 = TFile(path[1])
        t_side1 = f_side1.Get('save')
        entries_side1 = t_side1.GetEntries()
        logging.info('data(side1) entries :'+str(entries_side1))
    except:
        logging.error(path[1] + ' is invalid!')
        sys.exit()
    try:
        f_side2 = TFile(path[2])
        t_side2 = f_side2.Get('save')
        entries_side2 = t_side2.GetEntries()
        logging.info('data(side2) entries :'+str(entries_side2))
    except:
        logging.error(path[2] + ' is invalid!')
        sys.exit()
    try:
        f_side3 = TFile(path[3])
        t_side3 = f_side3.Get('save')
        entries_side3 = t_side3.GetEntries()
        logging.info('data(side3) entries :'+str(entries_side3))
    except:
        logging.error(path[3] + ' is invalid!')
        sys.exit()
    try:
        f_side4 = TFile(path[4])
        t_side4 = f_side4.Get('save')
        entries_side4 = t_side4.GetEntries()
        logging.info('data(side4) entries :'+str(entries_side4))
    except:
        logging.error(path[4] + ' is invalid!')
        sys.exit()

    mbc = TCanvas('mbc', 'mbc', 800, 600)
    set_canvas_style(mbc)
    ytitle = 'Entries'
    xtitle = 'c#tau'

    h_data = TH1F('data', 'data', xbins, xmin, xmax)
    set_histo_style(h_data, xtitle, ytitle, 1, -1)
    ctau_fill(t_data, h_data, D_type)

    h_side1 = TH1F('side1', 'side1', xbins, xmin, xmax)
    set_histo_style(h_side1, xtitle, ytitle, 3, 3004)
    ctau_fill(t_side1, h_side1, D_type)

    h_side2 = TH1F('side2', 'side2', xbins, xmin, xmax)
    set_histo_style(h_side2, xtitle, ytitle, 3, 3004)
    ctau_fill(t_side2, h_side2, D_type)

    h_side3 = TH1F('side3', 'side3', xbins, xmin, xmax)
    set_histo_style(h_side3, xtitle, ytitle, 3, 3004)
    ctau_fill(t_side3, h_side3, D_type)

    h_side4 = TH1F('side4', 'side4', xbins, xmin, xmax)
    set_histo_style(h_side4, xtitle, ytitle, 3, 3004)
    ctau_fill(t_side4, h_side4, D_type)

    h_side1.Add(h_side2)
    h_side1.Scale(0.5)
    h_side3.Add(h_side4)
    h_side3.Scale(0.25)
    h_side1.Add(h_side3, -1)
    h_data.Draw('E1')
    hs = THStack('hs', 'Stacked')
    hs.Add(h_side1)
    hs.Draw('same')
    h_data.Draw('sameE1')

    pt = TPaveText(0.6, 0.7, 0.75, 0.85, "BRNDC")
    set_pavetext(pt)
    pt.Draw()
    pt.AddText(leg_title)
    pt.AddText('e^{+}e^{-}#rightarrowD^{0}D^{-}#pi^{+}')

    legend = TLegend(0.6, 0.55, 0.75, 0.7)
    set_legend(legend, h_data, h_side1, leg_title)
    legend.Draw()

    if not os.path.exists('./figs/'):
        os.makedirs('./figs/')
    
    mbc.SaveAs('./figs/ctau_'+str(ecms)+'_'+D_type+'.pdf')

    raw_input('Enter anything to end...')

if __name__ == '__main__':
    args = sys.argv[1:]
    if len(args)<2:
        usage()
        sys.exit()
    ecms = int(args[0])
    D_type = args[1]

    path =[]
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data/'+str(ecms)+'/data_'+str(ecms)+'_signal.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data/'+str(ecms)+'/data_'+str(ecms)+'_side1.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data/'+str(ecms)+'/data_'+str(ecms)+'_side2.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data/'+str(ecms)+'/data_'+str(ecms)+'_side3.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data/'+str(ecms)+'/data_'+str(ecms)+'_side4.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/DDPI/'+str(ecms)+'/mc_DDPI_'+str(ecms)+'_signal.root')
    leg_title = str(ecms) + ' MeV'
    xmin, xmax, xbins = -5., 5., 200
    plot(path, leg_title, ecms, xmin, xmax, xbins, D_type)

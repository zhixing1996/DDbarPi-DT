#!/usr/bin/env python
"""
Plot scatter plot of decay length of D0 and D-
"""

__author__ = "Maoqiang JING <jingmq@ihep.ac.cn>"
__copyright__ = "Copyright (c) Maoqiang JING"
__created__ = "[2020-09-29 Tue 14:05]"

from ROOT import *
import sys, os
import logging
logging.basicConfig(level=logging.DEBUG, format=' %(asctime)s - %(levelname)s- %(message)s')
gStyle.SetOptTitle(0) # quench title
gStyle.SetPadTickX(1) # dicide on boxing on or not of x and y axis  
gStyle.SetPadTickY(1) # dicide on boxing on or not of x and y axis

def usage():
    sys.stdout.write('''
NAME
    scatter_L_D0_L_Dm.py

SYNOPSIS
    ./scatter_L_D0_L_Dm.py [ecms] [sample]

AUTHOR
    Maoqiang JING <jingmq@ihep.ac.cn>

DATE
    September 2020
\n''')

def set_pavetext(pt):
    pt.SetFillStyle(0)
    pt.SetBorderSize(0)
    pt.SetTextAlign(10)
    pt.SetTextSize(0.06)

def fill(t, h):
    for ientry in xrange(t.GetEntries()):
        t.GetEntry(ientry)
        h.Fill(t.m_L_svf_D0, t.m_L_svf_Dm)

def set_histo_style(h, xtitle, ytitle):
    h.GetXaxis().SetNdivisions(509)
    h.GetYaxis().SetNdivisions(504)
    h.SetStats(0)
    h.GetXaxis().SetTitleSize(0.06)
    h.GetXaxis().SetTitleOffset(1.)
    h.GetXaxis().SetLabelOffset(0.01)
    h.GetXaxis().SetLabelSize(0.05)
    h.GetYaxis().SetTitleSize(0.06)
    h.GetYaxis().SetTitleOffset(0.8)
    h.GetYaxis().SetLabelOffset(0.01)
    h.GetYaxis().SetLabelSize(0.05)
    h.GetXaxis().SetTitle(xtitle)
    h.GetYaxis().SetTitle(ytitle)
    h.GetXaxis().CenterTitle()
    h.GetYaxis().CenterTitle()
    h.SetLineColor(1)

def set_canvas_style(mbc):
    mbc.SetFillColor(0)
    mbc.SetLeftMargin(0.15)
    mbc.SetRightMargin(0.15)
    mbc.SetTopMargin(0.1)
    mbc.SetBottomMargin(0.15)

def plot(path, leg_title, ecms, xmin, xmax, ymin, ymax, xbins, ybins, sample):
    if sample == 'data':
        try:
            f_data = TFile(path[0])
            t_data = f_data.Get('save')
            entries_data = t_data.GetEntries()
            logging.info('data entries :'+str(entries_data))
        except:
            logging.error(path[0] + ' is invalid!')
            sys.exit()
    if sample == 'side':
        try:
            f_side1 = TFile(path[0])
            t_side1 = f_side1.Get('save')
            entries_side1 = t_side1.GetEntries()
            logging.info('data(side1) entries :'+str(entries_side1))
        except:
            logging.error(path[0] + ' is invalid!')
            sys.exit()
        try:
            f_side2 = TFile(path[1])
            t_side2 = f_side2.Get('save')
            entries_side2 = t_side2.GetEntries()
            logging.info('data(side2) entries :'+str(entries_side2))
        except:
            logging.error(path[1] + ' is invalid!')
            sys.exit()
        try:
            f_side3 = TFile(path[2])
            t_side3 = f_side3.Get('save')
            entries_side3 = t_side3.GetEntries()
            logging.info('data(side3) entries :'+str(entries_side3))
        except:
            logging.error(path[2] + ' is invalid!')
            sys.exit()
        try:
            f_side4 = TFile(path[3])
            t_side4 = f_side4.Get('save')
            entries_side4 = t_side4.GetEntries()
            logging.info('data(side4) entries :'+str(entries_side4))
        except:
            logging.error(path[3] + ' is invalid!')
            sys.exit()

    mbc = TCanvas('mbc', 'mbc', 800, 600)
    set_canvas_style(mbc)
    xtitle = 'L(D^{0}) (cm)'
    ytitle = 'L(D^{-}) (cm)'

    if sample == 'data':
        h_data = TH2F('scatter_data', 'scatter plot of L(D0) and L(Dm)', xbins, xmin, xmax, ybins, ymin, ymax)
        set_histo_style(h_data, xtitle, ytitle)
        fill(t_data, h_data)
        h_data.Draw('COLZ')
    if sample == 'side':
        h_side1 = TH2F('scatter_side1', 'scatter plot of L(D0) and L(Dm)', xbins, xmin, xmax, ybins, ymin, ymax)
        set_histo_style(h_side1, xtitle, ytitle)
        fill(t_side1, h_side1)
        h_side2 = TH2F('scatter_side2', 'scatter plot of L(D0) and L(Dm)', xbins, xmin, xmax, ybins, ymin, ymax)
        set_histo_style(h_side2, xtitle, ytitle)
        fill(t_side2, h_side2)
        h_side3 = TH2F('scatter_side3', 'scatter plot of L(D0) and L(Dm)', xbins, xmin, xmax, ybins, ymin, ymax)
        set_histo_style(h_side3, xtitle, ytitle)
        fill(t_side3, h_side3)
        h_side4 = TH2F('scatter_side4', 'scatter plot of L(D0) and L(Dm)', xbins, xmin, xmax, ybins, ymin, ymax)
        set_histo_style(h_side4, xtitle, ytitle)
        fill(t_side4, h_side4)
        h_side1.Add(h_side2)
        h_side1.Scale(0.5)
        h_side3.Add(h_side4)
        h_side3.Scale(0.25)
        h_side1.Add(h_side3, -1)
        h_side1.Draw('COLZ')

    pt = TPaveText(0.55, 0.7, 0.75, 0.85, "BRNDC")
    set_pavetext(pt)
    pt.Draw()
    pt.AddText(leg_title)

    if not os.path.exists('./figs/'):
        os.makedirs('./figs/')
    mbc.SaveAs('./figs/scatter_L_D0_L_Dm_'+str(ecms)+'_'+sample+'.pdf')

    raw_input('Press <Enter> to end...')

if __name__ == '__main__':
    args = sys.argv[1:]
    if len(args)<2:
        usage()
        sys.exit()
    ecms = int(args[0])
    sample = args[1]

    path = []
    if sample == 'data':
        path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data/'+str(ecms)+'/data_'+str(ecms)+'_signal.root')
    if sample == 'side':
        path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data/'+str(ecms)+'/data_'+str(ecms)+'_side1.root')
        path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data/'+str(ecms)+'/data_'+str(ecms)+'_side2.root')
        path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data/'+str(ecms)+'/data_'+str(ecms)+'_side3.root')
        path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data/'+str(ecms)+'/data_'+str(ecms)+'_side4.root')
    leg_title = str(ecms) + ' MeV'
    xmin = -0.5
    xmax = 0.5
    ymin = -0.5
    ymax = 0.5
    xbins = 100
    ybins = 100
    plot(path, leg_title, ecms, xmin, xmax, ymin, ymax, xbins, ybins, sample)

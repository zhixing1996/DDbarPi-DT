#!/usr/bin/env python
"""
Plot invariant mass of Dm in every tag mode
"""

__author__ = "Maoqiang JING <jingmq@ihep.ac.cn>"
__copyright__ = "Copyright (c) Maoqiang JING"
__created__ = "[2020-08-26 Thr 08:38]"

from ROOT import *
import sys, os
import logging
from math import *
from tools import width
logging.basicConfig(level=logging.DEBUG, format=' %(asctime)s - %(levelname)s- %(message)s')
gStyle.SetOptTitle(0) # quench title
gStyle.SetPadTickX(1) # dicide on boxing on or not of x and y axis  
gStyle.SetPadTickY(1) # dicide on boxing on or not of x and y axis

def usage():
    sys.stdout.write('''
NAME
    plot_m_Dm_mode.py

SYNOPSIS
    ./plot_m_Dm_mode.py [ecms] [mode]

AUTHOR
    Maoqiang JING <jingmq@ihep.ac.cn>

DATE
    December 2020
\n''')

def set_legend(legend, h1, h2, h3, h4, h5):
    legend.AddEntry(h1, 'data')
    legend.AddEntry(h2, 'q#bar{q}')
    legend.AddEntry(h4, 'Two Body')
    legend.AddEntry(h3, 'Three Body')
    legend.AddEntry(h5, 'Four Body')
    legend.SetBorderSize(0)
    legend.SetFillColor(0)
    legend.SetLineColor(0)
    legend.SetTextSize(0.05)

def set_pavetext(pt):
    pt.SetFillStyle(0)
    pt.SetBorderSize(0)
    pt.SetTextAlign(10)
    pt.SetTextSize(0.05)

def m_Dm_fill(t, h, Dm_k, D0_k):
    D0_W = width('D0')
    D0_signal_low = 1.86483 - D0_W/2.
    D0_signal_up = 1.86483 + D0_W/2.
    count = 0
    for ientry in xrange(t.GetEntries()):
        t.GetEntry(ientry)
        if t.m_Dm_mode == Dm_k and t.m_D0_mode == D0_k: 
            if not (t.m_D0_mass > D0_signal_low and t.m_D0_mass < D0_signal_up): continue
            h.Fill(t.m_Dm_mass)
            count += 1
    return count

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
    h.GetXaxis().SetLabelSize(0.06)
    h.GetXaxis().SetTitleOffset(1.)
    h.GetXaxis().SetLabelOffset(0.01)
    h.GetYaxis().SetTitleSize(0.06)
    h.GetYaxis().SetLabelSize(0.06)
    h.GetYaxis().SetTitleOffset(0.8)
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

def plot(path, leg_title, ecms, xmin, xmax, xbins):
    try:
        f_data = TFile(path[0])
        t_data = f_data.Get('save')
        entries_data = t_data.GetEntries()
        logging.info('data entries :'+str(entries_data))
    except:
        logging.error(path[0] + ' is invalid!')
        sys.exit()
    try:
        f_DD = TFile(path[1])
        t_DD = f_DD.Get('save')
        entries_DD = t_DD.GetEntries()
        logging.info('MC(DD) entries :'+str(entries_DD))
    except:
        logging.error(path[1] + ' is invalid!')
        sys.exit()
    try:
        f_DstD = TFile(path[2])
        t_DstD = f_DstD.Get('save')
        entries_DstD = t_DstD.GetEntries()
        logging.info('MC(DstD) entries :'+str(entries_DstD))
    except:
        logging.error(path[2] + ' is invalid!')
        sys.exit()
    try:
        f_DstDst = TFile(path[3])
        t_DstDst = f_DstDst.Get('save')
        entries_DstDst = t_DstDst.GetEntries()
        logging.info('MC(DstDst) entries :'+str(entries_DstDst))
    except:
        logging.error(path[3] + ' is invalid!')
        sys.exit()
    try:
        f_DDPI0 = TFile(path[4])
        t_DDPI0 = f_DDPI0.Get('save')
        entries_DDPI0 = t_DDPI0.GetEntries()
        logging.info('MC(DDPI0) entries :'+str(entries_DDPI0))
    except:
        logging.error(path[4] + ' is invalid!')
        sys.exit()
    try:
        f_DstDPI = TFile(path[5])
        t_DstDPI = f_DstDPI.Get('save')
        entries_DstDPI = t_DstDPI.GetEntries()
        logging.info('MC(DstDPI) entries :'+str(entries_DstDPI))
    except:
        logging.error(path[5] + ' is invalid!')
        sys.exit()
    try:
        f_DstDstPI = TFile(path[6])
        t_DstDstPI = f_DstDstPI.Get('save')
        entries_DstDstPI = t_DstDstPI.GetEntries()
        logging.info('MC(DstDstPI) entries :'+str(entries_DstDstPI))
    except:
        logging.error(path[6] + ' is invalid!')
        sys.exit()
    try:
        f_DDPIPI = TFile(path[7])
        t_DDPIPI = f_DDPIPI.Get('save')
        entries_DDPIPI = t_DDPIPI.GetEntries()
        logging.info('MC(DDPIPI) entries :'+str(entries_DDPIPI))
    except:
        logging.error(path[7] + ' is invalid!')
        sys.exit()
    try:
        f_DstDPIPI = TFile(path[8])
        t_DstDPIPI = f_DstDPIPI.Get('save')
        entries_DstDPIPI = t_DstDPIPI.GetEntries()
        logging.info('MC(DstDPIPI) entries :'+str(entries_DstDPIPI))
    except:
        logging.error(path[8] + ' is invalid!')
        sys.exit()
    try:
        f_DstDstPIPI = TFile(path[9])
        t_DstDstPIPI = f_DstDstPIPI.Get('save')
        entries_DstDstPIPI = t_DstDstPIPI.GetEntries()
        logging.info('MC(DstDstPIPI) entries :'+str(entries_DstDstPIPI))
    except:
        logging.error(path[9] + ' is invalid!')
        sys.exit()
    try:
        f_qq = TFile(path[10])
        t_qq = f_qq.Get('save')
        entries_qq = t_qq.GetEntries()
        logging.info('MC(qq) entries :'+str(entries_qq))
    except:
        logging.error(path[10] + ' is invalid!')
        sys.exit()

    mbc = TCanvas('mbc', 'mbc', 800, 600)
    set_canvas_style(mbc)
    content = (xmax - xmin)/xbins * 1000
    ytitle = 'Entries/%.1f MeV'%content
    xtitle = 'M(D^{-})(GeV)'
    Dm_dict = {}
    D0_dict = {}
    Dm_dict[200] = 'K^{+}#pi^{-}#pi^{-}'
    Dm_dict[201] = 'K^{+}#pi^{-}#pi^{-}#pi^{0}'
    Dm_dict[202] = 'K_{S}#pi^{-}'
    Dm_dict[203] = 'K_{S}#pi^{-}#pi^{0}'
    Dm_dict[204] = 'K_{S}#pi^{-}#pi^{-}#pi^{+}'
    D0_dict[0] = 'K^{-}#pi^{+}' 
    D0_dict[1] = 'K^{-}#pi^{+}#pi^{0}'
    D0_dict[3] = 'K^{-}#pi^{+}#pi^{+}#pi^{-}'

    for Dm_k, Dm_v in Dm_dict.items():
        for D0_k, D0_v in D0_dict.items():
            h_data = TH1F('data_' + str(Dm_k) + '_' + str(D0_k), 'data_' + str(Dm_k) + '_' + str(D0_k), xbins, xmin, xmax)
            set_histo_style(h_data, xtitle, ytitle, 1, -1)
            N_data = m_Dm_fill(t_data, h_data, Dm_k, D0_k)

            h_DD = TH1F('DD_' + str(Dm_k) + '_' + str(D0_k), 'DD_' + str(Dm_k) + '_' + str(D0_k), xbins, xmin, xmax)
            set_histo_style(h_DD, xtitle, ytitle, 2, 3004)
            N_DD = m_Dm_fill(t_DD, h_DD, Dm_k, D0_k)

            h_DstD = TH1F('DstD_' + str(Dm_k) + '_' + str(D0_k), 'DstD_' + str(Dm_k) + '_' + str(D0_k), xbins, xmin, xmax)
            set_histo_style(h_DstD, xtitle, ytitle, 2, 3004)
            N_DstD = m_Dm_fill(t_DstD, h_DstD, Dm_k, D0_k)

            h_DstDst = TH1F('DstDst_' + str(Dm_k) + '_' + str(D0_k), 'DstDst_' + str(Dm_k) + '_' + str(D0_k), xbins, xmin, xmax)
            set_histo_style(h_DstDst, xtitle, ytitle, 2, 3004)
            N_DstDst = m_Dm_fill(t_DstDst, h_DstDst, Dm_k, D0_k)

            h_DDPI0 = TH1F('DDPI0_' + str(Dm_k) + '_' + str(D0_k), 'DDPI0_' + str(Dm_k) + '_' + str(D0_k), xbins, xmin, xmax)
            set_histo_style(h_DDPI0, xtitle, ytitle, 3, 3004)
            N_DDPI0 = m_Dm_fill(t_DDPI0, h_DDPI0, Dm_k, D0_k)

            h_DstDPI = TH1F('DstDPI_' + str(Dm_k) + '_' + str(D0_k), 'DstDPI_' + str(Dm_k) + '_' + str(D0_k), xbins, xmin, xmax)
            set_histo_style(h_DstDPI, xtitle, ytitle, 3, 3004)
            N_DstDPI = m_Dm_fill(t_DstDPI, h_DstDPI, Dm_k, D0_k)

            h_DstDstPI = TH1F('DstDstPI_' + str(Dm_k) + '_' + str(D0_k), 'DstDstPI_' + str(Dm_k) + '_' + str(D0_k), xbins, xmin, xmax)
            set_histo_style(h_DstDstPI, xtitle, ytitle, 3, 3004)
            N_DstDstPI = m_Dm_fill(t_DstDstPI, h_DstDstPI, Dm_k, D0_k)

            h_DDPIPI = TH1F('DDPIPI_' + str(Dm_k) + '_' + str(D0_k), 'DDPIPI_' + str(Dm_k) + '_' + str(D0_k), xbins, xmin, xmax)
            set_histo_style(h_DDPIPI, xtitle, ytitle, 4, 3004)
            N_DDPIPI = m_Dm_fill(t_DDPIPI, h_DDPIPI, Dm_k, D0_k)

            h_DstDPIPI = TH1F('DstDPIPI_' + str(Dm_k) + '_' + str(D0_k), 'DstDPIPI_' + str(Dm_k) + '_' + str(D0_k), xbins, xmin, xmax)
            set_histo_style(h_DstDPIPI, xtitle, ytitle, 4, 3004)
            N_DstDPIPI = m_Dm_fill(t_DstDPIPI, h_DstDPIPI, Dm_k, D0_k)

            h_DstDstPIPI = TH1F('DstDstPIPI_' + str(Dm_k) + '_' + str(D0_k), 'DstDstPIPI_' + str(Dm_k) + '_' + str(D0_k), xbins, xmin, xmax)
            set_histo_style(h_DstDstPIPI, xtitle, ytitle, 4, 3004)
            N_DstDstPIPI = m_Dm_fill(t_DstDstPIPI, h_DstDstPIPI, Dm_k, D0_k)

            h_qq = TH1F('qq_' + str(Dm_k) + '_' + str(D0_k), 'qq_' + str(Dm_k) + '_' + str(D0_k), xbins, xmin, xmax)
            set_histo_style(h_qq, xtitle, ytitle, 6, 3004)
            N_qq = m_Dm_fill(t_qq, h_qq, Dm_k, D0_k)

            h_data.Draw('E1')
            hs = THStack('hs', 'Stacked')
            h_qq.Scale(0.25)
            h_DD.Add(h_DstD)
            h_DD.Add(h_DstDst)
            h_DDPI0.Add(h_DstDPI)
            h_DDPI0.Add(h_DstDstPI)
            h_DDPIPI.Add(h_DstDPIPI)
            h_DDPIPI.Add(h_DstDstPIPI)
            hs.Add(h_qq)
            hs.Add(h_DDPI0)
            hs.Add(h_DD)
            hs.Add(h_DDPIPI)
            hs.Draw('same')
            h_data.Draw('sameE1')

            if N_data == 0: ratio = -1
            else: ratio = round((N_DD + N_DstD + N_DstDst + N_DDPI0 + N_DstDPI + N_DstDstPI + N_DDPIPI + N_DstDPIPI + N_DstDstPIPI + N_qq*0.25)/float(N_data), 2)
            pt = TPaveText(0.6, 0.65, 0.75, 0.85, "BRNDC")
            set_pavetext(pt)
            pt.Draw()
            pt.AddText(leg_title)
            pt.AddText('D^{-}#rightarrow' + Dm_v)
            pt.AddText('D^{0}#rightarrow' + D0_v)
            pt.AddText('Ratio: ' + str(ratio))

            legend = TLegend(0.17, 0.62, 0.3, 0.87)
            set_legend(legend, h_data, h_qq, h_DDPI0, h_DD, h_DDPIPI)
            legend.Draw()

            if not os.path.exists('./figs/'):
                os.makedirs('./figs/')

            mbc.SaveAs('./figs/m_Dm_'+str(ecms)+'_'+str(Dm_k)+'_'+str(D0_k)+'.pdf')

            raw_input('Press <Enter> to end ...')

if __name__ == '__main__':
    args = sys.argv[1:]
    if len(args)<1:
        usage()
        sys.exit()
    ecms = int(args[0])

    path =[]
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data/'+str(ecms)+'/data_'+str(ecms)+'_signal_before_mass.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/DD/'+str(ecms)+'/mc_DD_'+str(ecms)+'_signal_before_mass.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/DstD/'+str(ecms)+'/mc_DstD_'+str(ecms)+'_signal_before_mass.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/DstDst/'+str(ecms)+'/mc_DstDst_'+str(ecms)+'_signal_before_mass.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/DDPI0/'+str(ecms)+'/mc_DDPI0_'+str(ecms)+'_signal_before_mass.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/DstDPI/'+str(ecms)+'/mc_DstDPI_'+str(ecms)+'_signal_before_mass.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/DstDstPI/'+str(ecms)+'/mc_DstDstPI_'+str(ecms)+'_signal_before_mass.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/DDPIPI/'+str(ecms)+'/mc_DDPIPI_'+str(ecms)+'_signal_before_mass.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/DstDPIPI/'+str(ecms)+'/mc_DstDPIPI_'+str(ecms)+'_signal_before_mass.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/DstDstPIPI/'+str(ecms)+'/mc_DstDstPIPI_'+str(ecms)+'_signal_before_mass.root')
    path.append('/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/qq/'+str(ecms)+'/mc_qq_'+str(ecms)+'_signal_before_mass.root')
    leg_title = str(ecms) + ' MeV'
    xmin = 1.74
    xmax = 2.
    xbins = int((xmax - xmin)/0.002)
    plot(path, leg_title, ecms, xmin, xmax, xbins)

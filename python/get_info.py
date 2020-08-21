#!/usr/bin/env python
"""
Get useful info from four-momentum root files
"""

__author__ = "Maoqiang JING <jingmq@ihep.ac.cn>"
__copyright__ = "Copyright (c) Maoqiang JING"
__created__ = "[2019-09-03 Tue 05:41]"

from math import *
from array import array
import ROOT
from ROOT import TCanvas, gStyle, TLorentzVector, TTree
from ROOT import TFile, TH1F, TLegend, TArrow, TChain, TVector3
import sys, os
import logging
logging.basicConfig(level=logging.DEBUG, format=' %(asctime)s - %(levelname)s- %(message)s')

def usage():
    sys.stdout.write('''
NAME
    get_info.py

SYNOPSIS
    ./get_info.py [file_in] [file_out]

AUTHOR
    Maoqiang JING <jingmq@ihep.ac.cn>

DATE
    August 2019
\n''')

def save(f_in, t):
    m_runNo = array('i', [0])
    m_evtNo = array('i', [0])
    m_indexmc = array('i', [0])
    m_motheridx = array('i', 100*[0])
    m_pdgid = array('i', 100*[0])
    m_D0_mode = array('d', [0])
    m_D0_type = array('d', [0])
    m_D0_charge = array('i', [0])
    m_D0_charm = array('d', [0])
    m_D0_mBC = array('d', [0])
    m_D0_numofchildren = array('d', [0])
    m_D0_mass = array('d', [0])
    m_D0_BE = array('d', [0])
    m_D0_deltaE = array('d', [0])
    m_Dm_mode = array('d', [0])
    m_Dm_type = array('d', [0])
    m_Dm_charge = array('i', [0])
    m_Dm_charm = array('d', [0])
    m_Dm_mBC = array('d', [0])
    m_Dm_numofchildren = array('d', [0])
    m_Dm_mass = array('d', [0])
    m_Dm_BE = array('d', [0])
    m_Dm_deltaE = array('d', [0])
    m_chi2_4c = array('d', [0.])
    m_m_D0 = array('d', [0])
    m_m_Dm = array('d', [0])
    m_chi2_6c = array('d', [0.])
    m_m_D0Dm = array('d', [0])
    m_m_D0pip = array('d', [0])
    m_m_Dmpip = array('d', [0])
    m_p4_D0 = array('d', 4*[0])
    m_p4_Dm = array('d', 4*[0])
    m_p4_pip = array('d', 4*[0])
    t.Branch('runNo', m_runNo, 'm_runNo/I')
    t.Branch('evtNo', m_evtNo, 'm_evtNo/I')
    t.Branch('indexmc', m_indexmc, 'indexmc/I')
    t.Branch('motheridx', m_motheridx, 'motheridx[100]/I')
    t.Branch('pdgid', m_pdgid, 'pdgid[100]/I')
    t.Branch('D0_mode', m_D0_mode, 'm_D0_mode/D')
    t.Branch('D0_type', m_D0_type, 'm_D0_type/D')
    t.Branch('D0_charge', m_D0_charge, 'm_D0_charge/I')
    t.Branch('D0_charm', m_D0_charm, 'm_D0_charm/D')
    t.Branch('D0_mBC', m_D0_mBC, 'm_D0_mBC/D')
    t.Branch('D0_numofchildren', m_D0_numofchildren, 'm_D0_numofchildren/D')
    t.Branch('D0_mass', m_D0_mass, 'm_D0_mass/D')
    t.Branch('D0_BE', m_D0_BE, 'm_D0_BE/D')
    t.Branch('D0_deltaE', m_D0_deltaE, 'm_D0_deltaE/D')
    t.Branch('Dm_mode', m_Dm_mode, 'm_Dm_mode/D')
    t.Branch('Dm_type', m_Dm_type, 'm_Dm_type/D')
    t.Branch('Dm_charge', m_Dm_charge, 'm_Dm_charge/I')
    t.Branch('Dm_charm', m_Dm_charm, 'm_Dm_charm/D')
    t.Branch('Dm_mBC', m_Dm_mBC, 'm_Dm_mBC/D')
    t.Branch('Dm_numofchildren', m_Dm_numofchildren, 'm_Dm_numofchildren/D')
    t.Branch('Dm_mass', m_Dm_mass, 'm_Dm_mass/D')
    t.Branch('Dm_BE', m_Dm_BE, 'm_Dm_BE/D')
    t.Branch('Dm_deltaE', m_Dm_deltaE, 'm_Dm_deltaE/D')
    t.Branch('chi2_4c', m_chi2_4c, 'm_chi2_4c/D')
    t.Branch('m_D0', m_m_D0, 'm_m_D0/D')
    t.Branch('m_Dm', m_m_Dm, 'm_m_Dm/D')
    t.Branch('chi2_6c', m_chi2_6c, 'm_chi2_6c/D')
    t.Branch('m_D0Dm', m_m_D0Dm, 'm_m_D0Dm/D')
    t.Branch('m_D0pip', m_m_D0pip, 'm_m_D0pip/D')
    t.Branch('m_Dmpip', m_m_Dmpip, 'm_m_Dmpip/D')
    t.Branch('p4_D0', m_p4_D0, 'p4_D0[4]/D')
    t.Branch('p4_Dm', m_p4_Dm, 'p4_Dm[4]/D')
    t.Branch('p4_pip', m_p4_pip, 'p4_pip[4]/D')
    t_dtd = f_in.Get('DTD')
    nentries = t_dtd.GetEntries()
    for ientry in range(nentries):
        t_dtd.GetEntry(ientry)
        if t_dtd.D0_mode != 0 and t_dtd.D0_mode != 1 and t_dtd.D0_mode != 3 and t_dtd.D0_mode != 4:
            continue
        if t_dtd.Dm_mode != 200 and t_dtd.Dm_mode != 201 and t_dtd.Dm_mode != 202 and t_dtd.Dm_mode != 203 and t_dtd.Dm_mode != 204:
            continue
        pD0_4c = TLorentzVector(0, 0, 0, 0)
        pDm_4c = TLorentzVector(0, 0, 0, 0)
        pD0_4c.SetPxPyPzE(t_dtd.p4_D0_4c[0], t_dtd.p4_D0_4c[1], t_dtd.p4_D0_4c[2], t_dtd.p4_D0_4c[3])
        pDm_4c.SetPxPyPzE(t_dtd.p4_Dm_4c[0], t_dtd.p4_Dm_4c[1], t_dtd.p4_Dm_4c[2], t_dtd.p4_Dm_4c[3])
        pD0_6c = TLorentzVector(0, 0, 0, 0)
        pDm_6c = TLorentzVector(0, 0, 0, 0)
        ppip_6c = TLorentzVector(0, 0, 0, 0)
        pD0_6c.SetPxPyPzE(t_dtd.p4_D0_6c[0], t_dtd.p4_D0_6c[1], t_dtd.p4_D0_6c[2], t_dtd.p4_D0_6c[3])
        pDm_6c.SetPxPyPzE(t_dtd.p4_Dm_6c[0], t_dtd.p4_Dm_6c[1], t_dtd.p4_Dm_6c[2], t_dtd.p4_Dm_6c[3])
        ppip_6c.SetPxPyPzE(t_dtd.p4_pip_6c[0], t_dtd.p4_pip_6c[1], t_dtd.p4_pip_6c[2], t_dtd.p4_pip_6c[3])
        m_runNo[0] = t_dtd.runNo
        m_evtNo[0] = t_dtd.evtNo
        m_indexmc[0] = t_dtd.indexmc
        for i in range(t_dtd.indexmc):
            m_motheridx[i] = t_dtd.motheridx[i]
            m_pdgid[i] = t_dtd.pdgid[i]
        m_D0_mode[0] = t_dtd.D0_mode
        m_D0_type[0] = t_dtd.D0_type
        m_D0_charge[0] = t_dtd.D0_dcharge
        m_D0_charm[0] = t_dtd.D0_charm
        m_D0_mBC[0] = t_dtd.D0_mBC
        m_D0_numofchildren[0] = t_dtd.D0_numofchildren
        m_D0_mass[0] = t_dtd.D0_mass
        m_D0_BE[0] = t_dtd.D0_BE
        m_D0_deltaE[0] = t_dtd.D0_deltaE
        m_Dm_mode[0] = t_dtd.Dm_mode
        m_Dm_type[0] = t_dtd.Dm_type
        m_Dm_charge[0] = t_dtd.Dm_dcharge
        m_Dm_charm[0] = t_dtd.Dm_charm
        m_Dm_mBC[0] = t_dtd.Dm_mBC
        m_Dm_numofchildren[0] = t_dtd.Dm_numofchildren
        m_Dm_mass[0] = t_dtd.Dm_mass
        m_Dm_BE[0] = t_dtd.Dm_BE
        m_Dm_deltaE[0] = t_dtd.Dm_deltaE
        m_chi2_4c[0] = t_dtd.chisq_4c
        m_m_D0[0] = pD0_4c.M()
        m_m_Dm[0] = pDm_4c.M()
        m_chi2_6c[0] = t_dtd.chisq_6c
        m_m_D0Dm[0] = (pD0_6c + pDm_6c).M()
        m_m_D0pip[0] = (pD0_6c + ppip_6c).M()
        m_m_Dmpip[0] = (pDm_6c + ppip_6c).M()
        for i in range(4):
            m_p4_D0[i] = t_dtd.p4_D0_6c[i]
            m_p4_Dm[i] = t_dtd.p4_Dm_6c[i]
            m_p4_pip[i] = t_dtd.p4_pip_6c[i]
        t.Fill()

def main():
    args = sys.argv[1:]
    if len(args)<2:
        usage()
        sys.exit()
    file_in = args[0]
    file_out = args[1]

    f_in = TFile(file_in)
    f_out = TFile(file_out, 'recreate')
    t_out = TTree('save', 'save')
    save(f_in, t_out)

    f_out.cd()
    t_out.Write()
    f_out.Close()

if __name__ == '__main__':
    main()

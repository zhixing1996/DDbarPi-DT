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
from tools import width
logging.basicConfig(level=logging.DEBUG, format=' %(asctime)s - %(levelname)s- %(message)s')

def usage():
    sys.stdout.write('''
NAME
    get_info.py

SYNOPSIS
    ./get_info.py [file_in] [file_out] [ecms] [mode]

AUTHOR
    Maoqiang JING <jingmq@ihep.ac.cn>

DATE
    August 2019
\n''')

def save(f_in, t, cms, mode):
    m_runNo = array('i', [0])
    m_evtNo = array('i', [0])
    m_indexmc = array('i', [0])
    m_trkidx = array('i', 100*[0])
    m_motheridx = array('i', 100*[0])
    m_motherid = array('i', 100*[0])
    m_pdgid = array('i', 100*[0])
    m_D0_mode = array('d', [0])
    m_D0_type = array('d', [0])
    m_D0_charge = array('i', [0])
    m_D0_charm = array('d', [0])
    m_D0_numofchildren = array('d', [0])
    m_D0_mass = array('d', [0])
    m_p_D0 = array('d', [0])
    m_E_D0 = array('d', [0])
    m_n_D0trks = array('i', [0])
    m_n_D0shws = array('i', [0])
    m_Dm_mode = array('d', [0])
    m_Dm_type = array('d', [0])
    m_Dm_charge = array('i', [0])
    m_Dm_charm = array('d', [0])
    m_Dm_numofchildren = array('d', [0])
    m_Dm_mass = array('d', [0])
    m_p_Dm = array('d', [0])
    m_E_Dm = array('d', [0])
    m_n_Dmtrks = array('i', [0])
    m_n_Dmshws = array('i', [0])
    m_chi2_4c = array('d', [0.])
    m_chi2_vf = array('d', [0.])
    m_m_D0 = array('d', [0])
    m_m_Dm = array('d', [0])
    m_chi2_6c = array('d', [0.])
    m_m_D0Dm = array('d', [0])
    m_m_D0pip = array('d', [0])
    m_m_Dmpip = array('d', [0])
    m_rm_D0pip = array('d', [0])
    m_rm_Dmpip = array('d', [0])
    m_p4_D0 = array('d', 4*[0])
    m_p4_Dm = array('d', 4*[0])
    m_p4_pip = array('d', 4*[0])
    m_Rxy_D0trks = array('d', 10*[999.])
    m_Rz_D0trks = array('d', 10*[999.])
    m_cos_D0trks = array('d', 10*[999.])
    m_E_D0shws = array('d', 10*[999.])
    m_dang_D0shws = array('d', 10*[999.])
    m_module_D0shws = array('d', 10*[999.])
    m_T_D0shws = array('d', 10*[999.])
    m_Rxy_Dmtrks = array('d', 10*[999.])
    m_Rz_Dmtrks = array('d', 10*[999.])
    m_cos_Dmtrks = array('d', 10*[999.])
    m_E_Dmshws = array('d', 10*[999.])
    m_dang_Dmshws = array('d', 10*[999.])
    m_module_Dmshws = array('d', 10*[999.])
    m_T_Dmshws = array('d', 10*[999.])
    m_Rxy_pip = array('d', [999.])
    m_Rz_pip = array('d', [999.])
    m_cos_pip = array('d', [999.])
    m_n_pi0 = array('i', [0])
    m_n_othershws = array('i', [0])
    m_matched_D0trks = array('i', [999])
    m_matched_D0shws = array('i', [999])
    m_matched_Dmtrks = array('i', [999])
    m_matched_Dmshws = array('i', [999])
    m_matched_pip = array('i', [999])
    m_m_delta = array('d', [0])
    m_p_pip = array('d', [0])
    m_chi2_svf_D0 = array('d', [999.])
    m_ctau_svf_D0 = array('d', [999.])
    m_L_svf_D0 = array('d', [999.])
    m_Lerr_svf_D0 = array('d', [999.])
    m_chi2_svf_Dm = array('d', [999.])
    m_ctau_svf_Dm = array('d', [999.])
    m_L_svf_Dm = array('d', [999.])
    m_Lerr_svf_Dm = array('d', [999.])
    t.Branch('runNo', m_runNo, 'm_runNo/I')
    t.Branch('evtNo', m_evtNo, 'm_evtNo/I')
    t.Branch('indexmc', m_indexmc, 'indexmc/I')
    t.Branch('trkidx', m_trkidx, 'trkidx[100]/I')
    t.Branch('motheridx', m_motheridx, 'motheridx[100]/I')
    t.Branch('motherid', m_motherid, 'motherid[100]/I')
    t.Branch('pdgid', m_pdgid, 'pdgid[100]/I')
    t.Branch('D0_mode', m_D0_mode, 'm_D0_mode/D')
    t.Branch('D0_type', m_D0_type, 'm_D0_type/D')
    t.Branch('D0_charge', m_D0_charge, 'm_D0_charge/I')
    t.Branch('D0_charm', m_D0_charm, 'm_D0_charm/D')
    t.Branch('D0_numofchildren', m_D0_numofchildren, 'm_D0_numofchildren/D')
    t.Branch('D0_mass', m_D0_mass, 'm_D0_mass/D')
    t.Branch('p_D0', m_p_D0, 'm_p_D0/D')
    t.Branch('E_D0', m_E_D0, 'm_E_D0/D')
    t.Branch('n_D0trks', m_n_D0trks, 'm_n_D0trks/D')
    t.Branch('n_D0shws', m_n_D0shws, 'm_n_D0shws/D')
    t.Branch('Dm_mode', m_Dm_mode, 'm_Dm_mode/D')
    t.Branch('Dm_type', m_Dm_type, 'm_Dm_type/D')
    t.Branch('Dm_charge', m_Dm_charge, 'm_Dm_charge/I')
    t.Branch('Dm_charm', m_Dm_charm, 'm_Dm_charm/D')
    t.Branch('Dm_numofchildren', m_Dm_numofchildren, 'm_Dm_numofchildren/D')
    t.Branch('Dm_mass', m_Dm_mass, 'm_Dm_mass/D')
    t.Branch('p_Dm', m_p_Dm, 'm_p_Dm/D')
    t.Branch('E_Dm', m_E_Dm, 'm_E_Dm/D')
    t.Branch('n_Dmtrks', m_n_Dmtrks, 'm_n_Dmtrks/D')
    t.Branch('n_Dmshws', m_n_Dmshws, 'm_n_Dmshws/D')
    t.Branch('chi2_4c', m_chi2_4c, 'm_chi2_4c/D')
    t.Branch('chi2_vf', m_chi2_vf, 'm_chi2_vf/D')
    t.Branch('m_D0', m_m_D0, 'm_m_D0/D')
    t.Branch('m_Dm', m_m_Dm, 'm_m_Dm/D')
    t.Branch('chi2_6c', m_chi2_6c, 'm_chi2_6c/D')
    t.Branch('m_D0Dm', m_m_D0Dm, 'm_m_D0Dm/D')
    t.Branch('m_D0pip', m_m_D0pip, 'm_m_D0pip/D')
    t.Branch('m_Dmpip', m_m_Dmpip, 'm_m_Dmpip/D')
    t.Branch('rm_D0pip', m_rm_D0pip, 'm_rm_D0pip/D')
    t.Branch('rm_Dmpip', m_rm_Dmpip, 'm_rm_Dmpip/D')
    t.Branch('p4_D0', m_p4_D0, 'm_p4_D0[4]/D')
    t.Branch('p4_Dm', m_p4_Dm, 'm_p4_Dm[4]/D')
    t.Branch('p4_pip', m_p4_pip, 'm_p4_pip[4]/D')
    t.Branch('Rxy_D0trks', m_Rxy_D0trks, 'm_Rxy_D0trks[10]/D')
    t.Branch('Rz_D0trks', m_Rz_D0trks, 'm_Rz_D0trks[10]/D')
    t.Branch('cos_D0trks', m_cos_D0trks, 'm_cos_D0trks[10]/D')
    t.Branch('E_D0shws', m_E_D0shws, 'm_E_D0shws[10]/D')
    t.Branch('dang_D0shws', m_dang_D0shws, 'm_dang_D0shws[10]/D')
    t.Branch('module_D0shws', m_module_D0shws, 'm_module_D0shws[10]/D')
    t.Branch('T_D0shws', m_T_D0shws, 'm_T_D0shws[10]/D')
    t.Branch('Rxy_Dmtrks', m_Rxy_Dmtrks, 'm_Rxy_Dmtrks[10]/D')
    t.Branch('Rz_Dmtrks', m_Rz_Dmtrks, 'm_Rz_Dmtrks[10]/D')
    t.Branch('cos_Dmtrks', m_cos_Dmtrks, 'm_cos_Dmtrks[10]/D')
    t.Branch('E_Dmshws', m_E_Dmshws, 'm_E_Dmshws[10]/D')
    t.Branch('dang_Dmshws', m_dang_Dmshws, 'm_dang_Dmshws[10]/D')
    t.Branch('module_Dmshws', m_module_Dmshws, 'm_module_Dmshws[10]/D')
    t.Branch('T_Dmshws', m_T_Dmshws, 'm_T_Dmshws[10]/D')
    t.Branch('Rxy_pip', m_Rxy_pip, 'm_Rxy_pip/D')
    t.Branch('Rz_pip', m_Rz_pip, 'm_Rz_pip/D')
    t.Branch('cos_pip', m_cos_pip, 'm_cos_pip/D')
    t.Branch('n_pi0', m_n_pi0, 'm_n_pi0/I')
    t.Branch('n_othershws', m_n_othershws, 'm_n_othershws/I')
    t.Branch('matched_D0trks', m_matched_D0trks, 'm_matched_D0trks/I')
    t.Branch('matched_D0shws', m_matched_D0shws, 'm_matched_D0shws/I')
    t.Branch('matched_Dmtrks', m_matched_Dmtrks, 'm_matched_Dmtrks/I')
    t.Branch('matched_Dmshws', m_matched_Dmshws, 'm_matched_Dmshws/I')
    t.Branch('matched_pip', m_matched_pip, 'm_matched_pip/I')
    t.Branch('m_delta', m_m_delta, 'm_m_delta/D')
    t.Branch('p_pip', m_p_pip, 'm_p_pip/D')
    t.Branch('chi2_svf_D0', m_chi2_svf_D0, 'm_chi2_svf_D0/D')
    t.Branch('ctau_svf_D0', m_ctau_svf_D0, 'm_ctau_svf_D0/D')
    t.Branch('L_svf_D0', m_L_svf_D0, 'm_L_svf_D0/D')
    t.Branch('Lerr_svf_D0', m_Lerr_svf_D0, 'm_Lerr_svf_D0/D')
    t.Branch('chi2_svf_Dm', m_chi2_svf_Dm, 'm_chi2_svf_Dm/D')
    t.Branch('ctau_svf_Dm', m_ctau_svf_Dm, 'm_ctau_svf_Dm/D')
    t.Branch('L_svf_Dm', m_L_svf_Dm, 'm_L_svf_Dm/D')
    t.Branch('Lerr_svf_Dm', m_Lerr_svf_Dm, 'm_Lerr_svf_Dm/D')
    M_Dm = 1.86965
    M_D0 = 1.86483
    if mode == 'signal': t_dtd = f_in.Get('DTD_signal')
    if mode == 'side1_low': t_dtd = f_in.Get('DTD_side1_low')
    if mode == 'side1_up': t_dtd = f_in.Get('DTD_side1_up')
    if mode == 'side2_low': t_dtd = f_in.Get('DTD_side2_low')
    if mode == 'side2_up': t_dtd = f_in.Get('DTD_side2_up')
    if mode == 'side3_low': t_dtd = f_in.Get('DTD_side3_low')
    if mode == 'side3_up': t_dtd = f_in.Get('DTD_side3_up')
    if mode == 'side4_low': t_dtd = f_in.Get('DTD_side4_low')
    if mode == 'side4_up': t_dtd = f_in.Get('DTD_side4_up')
    nentries = t_dtd.GetEntries()
    for ientry in range(nentries):
        t_dtd.GetEntry(ientry)
        if t_dtd.D0_mode != 0 and t_dtd.D0_mode != 1 and t_dtd.D0_mode != 3:
            continue
        if t_dtd.Dm_mode != 200 and t_dtd.Dm_mode != 201 and t_dtd.Dm_mode != 202 and t_dtd.Dm_mode != 203 and t_dtd.Dm_mode != 204:
            continue
        ppip_raw = TLorentzVector(0, 0, 0, 0)
        ppip_raw.SetPxPyPzE(t_dtd.rawp4_tagPi[0], t_dtd.rawp4_tagPi[1], t_dtd.rawp4_tagPi[2], t_dtd.rawp4_tagPi[3])
        E_Dm = 0
        pDm_raw = TLorentzVector(0, 0, 0, 0)
        m_matched_Dmtrks[0] = 1
        for iTrk in range(t_dtd.n_Dmtrks):
            ptrack_raw = TLorentzVector(0, 0, 0, 0)
            ptrack_raw.SetPxPyPzE(t_dtd.rawp4_Dmtrks[iTrk*8+0], t_dtd.rawp4_Dmtrks[iTrk*8+1], t_dtd.rawp4_Dmtrks[iTrk*8+2], t_dtd.rawp4_Dmtrks[iTrk*8+3])
            pDm_raw += ptrack_raw
            E_Dm += t_dtd.rawp4_Dmtrks[iTrk*8+3]
            if not t_dtd.rawp4_Dmtrks[iTrk*8+6] == 1:
                m_matched_Dmtrks[0] = 0
        if t_dtd.n_Dmshws == 0: m_matched_Dmshws[0] = 0
        else: m_matched_Dmshws[0] = 1
        for iShw in range(t_dtd.n_Dmshws):
            pshower_raw = TLorentzVector(0, 0, 0, 0)
            pshower_raw.SetPxPyPzE(t_dtd.rawp4_Dmshws[iShw*6+0], t_dtd.rawp4_Dmshws[iShw*6+1], t_dtd.rawp4_Dmshws[iShw*6+2], t_dtd.rawp4_Dmshws[iShw*6+3])
            pDm_raw += pshower_raw
            E_Dm += t_dtd.rawp4_Dmshws[iShw*6+3]
            if not t_dtd.rawp4_Dmshws[iShw*6+4] == 1:
                m_matched_Dmshws[0] = 0
        E_D0 = 0
        pD0_raw = TLorentzVector(0, 0, 0, 0)
        m_matched_D0trks[0] = 1
        for iTrk in range(t_dtd.n_D0trks):
            ptrack_raw = TLorentzVector(0, 0, 0, 0)
            ptrack_raw.SetPxPyPzE(t_dtd.rawp4_D0trks[iTrk*8+0], t_dtd.rawp4_D0trks[iTrk*8+1], t_dtd.rawp4_D0trks[iTrk*8+2], t_dtd.rawp4_D0trks[iTrk*8+3])
            pD0_raw += ptrack_raw
            E_D0 += t_dtd.rawp4_D0trks[iTrk*8+3]
            if not t_dtd.rawp4_D0trks[iTrk*8+6] == 1:
                m_matched_D0trks[0] = 0
        if t_dtd.n_D0shws == 0: m_matched_D0shws[0] = 0
        else: m_matched_D0shws[0] = 1
        for iShw in range(t_dtd.n_D0shws):
            pshower_raw = TLorentzVector(0, 0, 0, 0)
            pshower_raw.SetPxPyPzE(t_dtd.rawp4_D0shws[iShw*6+0], t_dtd.rawp4_D0shws[iShw*6+1], t_dtd.rawp4_D0shws[iShw*6+2], t_dtd.rawp4_D0shws[iShw*6+3])
            pD0_raw += pshower_raw
            E_D0 += t_dtd.rawp4_D0shws[iShw*6+3]
            if not t_dtd.rawp4_D0shws[iShw*6+4] == 1:
                m_matched_D0shws[0] = 0
        m_matched_pip[0] = 1
        if not t_dtd.rawp4_tagPi[4] == 1:
            m_matched_pip[0] = 0
        pD0_4c = TLorentzVector(0, 0, 0, 0)
        pDm_4c = TLorentzVector(0, 0, 0, 0)
        ppip_4c = TLorentzVector(0, 0, 0, 0)
        ppip_raw = TLorentzVector(0, 0, 0, 0)
        ppip_raw.SetPxPyPzE(t_dtd.rawp4_tagPi[0], t_dtd.rawp4_tagPi[1], t_dtd.rawp4_tagPi[2], t_dtd.rawp4_tagPi[3])
        pD0_4c.SetPxPyPzE(t_dtd.p4_D0_4c[0], t_dtd.p4_D0_4c[1], t_dtd.p4_D0_4c[2], t_dtd.p4_D0_4c[3])
        pDm_4c.SetPxPyPzE(t_dtd.p4_Dm_4c[0], t_dtd.p4_Dm_4c[1], t_dtd.p4_Dm_4c[2], t_dtd.p4_Dm_4c[3])
        ppip_4c.SetPxPyPzE(t_dtd.p4_pip_4c[0], t_dtd.p4_pip_4c[1], t_dtd.p4_pip_4c[2], t_dtd.p4_pip_4c[3])
        m_runNo[0] = t_dtd.runNo
        m_evtNo[0] = t_dtd.evtNo
        m_indexmc[0] = t_dtd.indexmc
        for i in range(t_dtd.indexmc):
            m_motheridx[i] = t_dtd.motheridx[i]
            m_motherid[i] = t_dtd.motherid[i]
            m_pdgid[i] = t_dtd.pdgid[i]
            m_trkidx[i] = t_dtd.trkidx[i]
        m_D0_mode[0] = t_dtd.D0_mode
        m_D0_type[0] = t_dtd.D0_type
        m_D0_charge[0] = t_dtd.D0_dcharge
        m_D0_charm[0] = t_dtd.D0_charm
        m_D0_numofchildren[0] = t_dtd.D0_numofchildren
        m_D0_mass[0] = t_dtd.D0_mass
        m_p_D0[0] = pD0_raw.P()
        m_E_D0[0] = E_D0
        m_n_D0trks[0] = t_dtd.n_D0trks
        m_n_D0shws[0] = t_dtd.n_D0shws
        m_Dm_mode[0] = t_dtd.Dm_mode
        m_Dm_type[0] = t_dtd.Dm_type
        m_Dm_charge[0] = t_dtd.Dm_dcharge
        m_Dm_charm[0] = t_dtd.Dm_charm
        m_Dm_numofchildren[0] = t_dtd.Dm_numofchildren
        m_Dm_mass[0] = t_dtd.Dm_mass
        m_p_Dm[0] = pDm_raw.P()
        m_E_Dm[0] = E_Dm
        m_n_Dmtrks[0] = t_dtd.n_Dmtrks
        m_n_Dmshws[0] = t_dtd.n_Dmshws
        m_chi2_4c[0] = t_dtd.chisq_4c
        m_m_D0[0] = pD0_4c.M()
        m_m_Dm[0] = pDm_4c.M()
        m_rm_D0pip[0] = (cms - pD0_4c - ppip_4c).M()
        m_rm_Dmpip[0] = (cms - pDm_4c - ppip_4c).M()
        for i in range(4):
            m_p4_D0[i] = t_dtd.p4_D0_6c[i]
            m_p4_Dm[i] = t_dtd.p4_Dm_6c[i]
            m_p4_pip[i] = t_dtd.p4_pip_6c[i]
        m_Rxy_pip[0] = t_dtd.vtx_tagPi[0]
        m_Rz_pip[0] = t_dtd.vtx_tagPi[1]
        m_cos_pip[0] = t_dtd.vtx_tagPi[2]
        for itrk in xrange(t_dtd.n_D0trks):
            m_Rxy_D0trks[itrk] = abs(t_dtd.vtx_D0trks[itrk*3+0])
            m_Rz_D0trks[itrk] = abs(t_dtd.vtx_D0trks[itrk*3+1])
            m_cos_D0trks[itrk] = abs(t_dtd.vtx_D0trks[itrk*3+2])
        for ishw in xrange(t_dtd.n_D0shws):
            m_E_D0shws[ishw] = abs(t_dtd.vtx_D0shws[ishw*6+0])
            m_dang_D0shws[ishw] = abs(t_dtd.vtx_D0shws[ishw*6+3])
            m_module_D0shws[ishw] = abs(t_dtd.vtx_D0shws[ishw*6+4])
            m_T_D0shws[ishw] = abs(t_dtd.vtx_D0shws[ishw*6+5])
        for itrk in xrange(t_dtd.n_Dmtrks):
            m_Rxy_Dmtrks[itrk] = abs(t_dtd.vtx_Dmtrks[itrk*3+0])
            m_Rz_Dmtrks[itrk] = abs(t_dtd.vtx_Dmtrks[itrk*3+1])
            m_cos_Dmtrks[itrk] = abs(t_dtd.vtx_Dmtrks[itrk*3+2])
        for ishw in xrange(t_dtd.n_Dmshws):
            m_E_Dmshws[ishw] = abs(t_dtd.vtx_Dmshws[ishw*6+0])
            m_dang_Dmshws[ishw] = abs(t_dtd.vtx_Dmshws[ishw*6+3])
            m_module_Dmshws[ishw] = abs(t_dtd.vtx_Dmshws[ishw*6+4])
            m_T_Dmshws[ishw] = abs(t_dtd.vtx_Dmshws[ishw*6+5])
        m_n_pi0[0] = t_dtd.n_pi0
        shw_status = True
        n_othershws = 0
        for iShw in range(t_dtd.n_othershws):
            if abs(t_dtd.vtx_othershws[iShw*6+3]) < 10.:
                shw_status = False
            if t_dtd.vtx_othershws[iShw*6+5] < 0. or t_dtd.vtx_othershws[iShw*6+5] > 14:
                shw_status = False
            if t_dtd.vtx_othershws[iShw*6+4] == 1 and t_dtd.vtx_othershws[iShw*6+0] < 0.025:
                shw_status = False
            if (t_dtd.vtx_othershws[iShw*6+4] == 0 or t_dtd.vtx_othershws[iShw*6+4] == 2) and t_dtd.vtx_othershws[iShw*6+0] < 0.05:
                shw_status = False
            if shw_status:
                n_othershws += 1
        m_n_othershws[0] = n_othershws
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
        if mode == 'signal':
            M_D0 = (D0_signal_low + D0_signal_up)/2.
            M_Dm = (Dm_signal_low + Dm_signal_up)/2.
        if mode == 'side1_low':
            M_D0 = (D0_signal_low + D0_signal_up)/2.
            M_Dm = (Dm_sidebandlow_low + Dm_sidebandlow_up)/2.
        if mode == 'side1_up':
            M_D0 = (D0_signal_low + D0_signal_up)/2.
            M_Dm = (Dm_sidebandup_low + Dm_sidebandup_up)/2.
        if mode == 'side2_low':
            M_D0 = (D0_sidebandlow_low + D0_sidebandlow_up)/2.
            M_Dm = (Dm_signal_low + Dm_signal_up)/2.
        if mode == 'side2_up':
            M_D0 = (D0_sidebandup_low + D0_sidebandup_up)/2.
            M_Dm = (Dm_signal_low + Dm_signal_up)/2.
        if mode == 'side3_low':
            M_D0 = (D0_sidebandlow_low + D0_sidebandlow_up)/2.
            M_Dm = (Dm_sidebandlow_low + Dm_sidebandlow_up)/2.
        if mode == 'side3_up':
            M_D0 = (D0_sidebandlow_low + D0_sidebandlow_up)/2.
            M_Dm = (Dm_sidebandup_low + Dm_sidebandup_up)/2.
        if mode == 'side4_low':
            M_D0 = (D0_sidebandup_low + D0_sidebandup_up)/2.
            M_Dm = (Dm_sidebandlow_low + Dm_sidebandlow_up)/2.
        if mode == 'side4_up':
            M_D0 = (D0_sidebandup_low + D0_sidebandup_up)/2.
            M_Dm = (Dm_sidebandup_low + Dm_sidebandup_up)/2.
        m_m_delta[0] = abs(t_dtd.D0_mass - M_D0) + abs(t_dtd.Dm_mass - M_Dm)
        pD0_6c = TLorentzVector(0, 0, 0, 0)
        pDm_6c = TLorentzVector(0, 0, 0, 0)
        ppip_6c = TLorentzVector(0, 0, 0, 0)
        pD0_6c.SetPxPyPzE(t_dtd.p4_D0_6c[0], t_dtd.p4_D0_6c[1], t_dtd.p4_D0_6c[2], t_dtd.p4_D0_6c[3])
        pDm_6c.SetPxPyPzE(t_dtd.p4_Dm_6c[0], t_dtd.p4_Dm_6c[1], t_dtd.p4_Dm_6c[2], t_dtd.p4_Dm_6c[3])
        ppip_6c.SetPxPyPzE(t_dtd.p4_pip_6c[0], t_dtd.p4_pip_6c[1], t_dtd.p4_pip_6c[2], t_dtd.p4_pip_6c[3])
        m_chi2_6c[0] = t_dtd.chisq_6c
        m_chi2_vf[0] = t_dtd.chisq_vf
        m_m_D0Dm[0] = (pD0_6c + pDm_6c).M()
        m_m_D0pip[0] = (pD0_6c + ppip_6c).M()
        m_m_Dmpip[0] = (pDm_6c + ppip_6c).M()
        m_p_pip[0] = ppip_raw.P()
        m_chi2_svf_D0[0] = t_dtd.chisq_svf_D0
        m_ctau_svf_D0[0] = t_dtd.ctau_svf_D0
        m_L_svf_D0[0] = t_dtd.L_svf_D0
        m_Lerr_svf_D0[0] = t_dtd.Lerr_svf_D0
        m_chi2_svf_Dm[0] = t_dtd.chisq_svf_Dm
        m_ctau_svf_Dm[0] = t_dtd.ctau_svf_Dm
        m_L_svf_Dm[0] = t_dtd.L_svf_Dm
        m_Lerr_svf_Dm[0] = t_dtd.Lerr_svf_Dm
        t.Fill()

def main():
    args = sys.argv[1:]
    if len(args)<4:
        usage()
        sys.exit()
    file_in = args[0]
    file_out = args[1]
    ecms = float(args[2])
    mode = args[3]

    f_in = TFile(file_in)
    f_out = TFile(file_out, 'recreate')
    t_out = TTree('save', 'save')
    cms = TLorentzVector(0.011*ecms, 0, 0, ecms)
    save(f_in, t_out, cms, mode)

    f_out.cd()
    t_out.Write()
    f_out.Close()

if __name__ == '__main__':
    main()

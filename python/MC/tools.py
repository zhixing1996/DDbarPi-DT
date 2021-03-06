#!/usr/bin/env python
"""
Common tools 
"""

__author__ = "SHI Xin <shixin@ihep.ac.cn>"
__copyright__ = "Copyright (c) SHI Xin"
__created__ = "[2016-06-28 Tue 09:17]" 

import sys 
import os
import shutil
import subprocess
import ROOT 


# ---------------------------------------------
# Function 
# ---------------------------------------------

def check_and_join(filepath, filename, mode=''):
    if not os.access(filepath, os.F_OK):
        sys.stdout.write('creating dir %s ...' % filepath)
        os.makedirs(filepath)
        sys.stdout.write(' OK.\n')
        
    file_ = os.path.join(filepath, filename)
    if os.access(file_, os.F_OK) :
        tmpfile = make_tmpfile(file_)
        shutil.copy2(file_, tmpfile)
        if mode == 'w':
            os.remove(file_)

    return file_

def check_outfile_path(outfile):
    path, tail = os.path.split(outfile)
    if path != '' and not os.access(path, os.F_OK) :
        sys.stdout.write('Creating dir %s ...\n'  % path)
        os.makedirs(path)

def convert_size_from_str(size_str):
    c_1GB = 1024*1024*1024
    factor = eval(size_str.split('G')[0])
    return factor*c_1GB

def duration(seconds):
    seconds = long(round(seconds))
    minutes, seconds = divmod(seconds, 60)
    hours, minutes = divmod(minutes, 60)
    days, hours = divmod(hours, 24)
    years, days = divmod(days, 365.242199)
 
    minutes = long(minutes)
    hours = long(hours)
    days = long(days)
    years = long(years)
 
    duration = []
    if years > 0:
        duration.append('%d year' % years + 's'*(years != 1))
    else:
        if days > 0:
            duration.append('%d day' % days + 's'*(days != 1))
        if hours > 0:
            duration.append('%d hour' % hours + 's'*(hours != 1))
        if minutes > 0:
            duration.append('%d minute' % minutes + 's'*(minutes != 1))
        if seconds > 0:
            duration.append('%d second' % seconds + 's'*(seconds != 1))
    return ' '.join(duration)


def group_files_by_size(name_list, size_max='20G'):
    size_max = convert_size_from_str(size_max)
    groups = []
    group = []    
    size_sum = 0

    for name in name_list:
        size = os.path.getsize(name)
        if size_sum < size_max:
            group.append(name)
            size_sum += float(size)
        else:
            groups.append(group)
            group = []
            size_sum = 0
            group.append(name)            
            size_sum += float(size)

        if name == name_list[-1]:
            groups.append(group)

    return groups

def proc_cmd(cmd, test=False, verbose=1, procdir=None, shell=False):
    if test:
        sys.stdout.write(cmd+'\n')
        return 

    # No need to add quote in the cmd! 
    cwd = os.getcwd()
    if procdir is not None:
        os.chdir(procdir)
    
    args = cmd.split()

    if isinstance(cmd, list) or shell:
        args = cmd 

    process = subprocess.Popen(args, stdout=subprocess.PIPE,
                               stderr=subprocess.STDOUT, shell=shell)
    stdout = process.communicate()[0]

    if 'error' in stdout:
        sys.stdout.write(stdout)
        
    if procdir is not None:
        os.chdir(cwd)

    return stdout


def save_list_into_file(file_list, dst):
    nfiles = len(file_list)
    
    path, tail = os.path.split(dst)
    if path != '' and not os.access(path, os.F_OK) :
        sys.stdout.write('Creating dir %s ...\n'  % path)
        os.makedirs(path)
                
    fo = open(dst, 'w')
    fo.write('EventCnvSvc.digiRootInputFile = {\n')

    n = 0
    for f in file_list:
        n = n+1
        if n<nfiles:
            fo.write('"%s",\n' % f)
        else:
            fo.write('"%s"\n};\n' % f)

    fo.close()
    sys.stdout.write('Saved as: %s\n' %dst)

    
def set_root_style(stat=0, grid=0, PadTopMargin=0.08,
                   PadLeftMargin=0.15):
    # must be used in the beginning
    ROOT.gROOT.SetBatch(1)
    ROOT.gROOT.Reset()

    ROOT.gStyle.SetOptTitle(0) 
    ROOT.gStyle.SetTitleFillColor(0) 
    ROOT.gStyle.SetTitleBorderSize(0)
    
    ROOT.gStyle.SetCanvasBorderMode(0)
    ROOT.gStyle.SetCanvasColor(0)
    ROOT.gStyle.SetCanvasDefX(0)
    ROOT.gStyle.SetCanvasDefY(0)
    ROOT.gStyle.SetFrameBorderMode(0)
    ROOT.gStyle.SetFrameBorderSize(1)
    ROOT.gStyle.SetFrameFillColor(0)
    ROOT.gStyle.SetFrameFillStyle(0)
    ROOT.gStyle.SetFrameLineColor(1)
    ROOT.gStyle.SetFrameLineStyle(1)
    ROOT.gStyle.SetFrameLineWidth(1)

    ROOT.gStyle.SetPadTopMargin(PadTopMargin) 
    ROOT.gStyle.SetPadLeftMargin(PadLeftMargin) 
    ROOT.gStyle.SetPadRightMargin(0.05) 

    ROOT.gStyle.SetLabelSize(0.02, "XYZ") 
    ROOT.gStyle.SetTitleSize(0.02, "XYZ") 
    ROOT.gStyle.SetTitleOffset(1.2, "Y") 

    ROOT.gStyle.SetPadBorderMode(0) 
    ROOT.gStyle.SetPadColor(0) 
    ROOT.gStyle.SetPadTickX(1)
    ROOT.gStyle.SetPadTickY(1)
    ROOT.gStyle.SetPadGridX(grid)
    ROOT.gStyle.SetPadGridY(grid)

    ROOT.gStyle.SetOptStat(stat)
    ROOT.gStyle.SetStatColor(0)
    ROOT.gStyle.SetStatBorderSize(1)

def convert_name(D_sample, D_type):
    name = ''
    if D_sample == 'Dplus' and D_type == 'D':
        name = 'D0'
    if D_sample == 'Dplus' and D_type == 'Dst':
        name = 'D0st'
    if D_sample == 'D0' and D_type == 'D':
        name = 'Dplus'
    if D_sample == 'D0' and D_type == 'Dst':
        name = 'Dplusst'
    return name

def luminosity(ecms):
    lum = 0.
    if ecms == 4600:
        lum = 586.9
    if ecms == 4620:
        lum = 511
    if ecms == 4640:
        lum = 541
    if ecms == 4660:
        lum = 529
    if ecms == 4680:
        lum = 528.46
    if ecms == 4700:
        lum = 526.20
    return lum

def width(D):
    width = 0.
    if D == 'D0':
        width = 0.024
        # width = 0.035
    if D == 'Dm':
        width = 0.020
        # width = 0.035
    return width

# def width(Dm, D0):
#     width_Dm = 0.02
#     width_D0 = 0.02
#     if Dm == 200 and D0 == 0:
#         width_Dm = 0.010
#         width_D0 = 0.010
#     if Dm == 200 and D0 == 1:
#         width_Dm = 0.011
#         width_D0 = 0.014
#     if Dm == 200 and D0 == 3:
#         width_Dm = 0.011
#         width_D0 = 0.010
#     if Dm == 200 and D0 == 4:
#         width_Dm = 0.012
#         width_D0 = 0.016
#     if Dm == 201 and D0 == 0:
#         width_Dm = 0.015
#         width_D0 = 0.012
#     if Dm == 201 and D0 == 1:
#         width_Dm = 0.016
#         width_D0 = 0.015
#     if Dm == 201 and D0 == 3:
#         width_Dm = 0.015
#         width_D0 = 0.010
#     if Dm == 201 and D0 == 4:
#         width_Dm = 0.015
#         width_D0 = 0.014
#     if Dm == 202 and D0 == 0:
#         width_Dm = 0.013
#         width_D0 = 0.012
#     if Dm == 202 and D0 == 1:
#         width_Dm = 0.015
#         width_D0 = 0.016
#     if Dm == 202 and D0 == 3:
#         width_Dm = 0.014
#         width_D0 = 0.008
#     if Dm == 202 and D0 == 4:
#         width_Dm = 0.015
#         width_D0 = 0.016
#     if Dm == 203 and D0 == 0:
#         width_Dm = 0.016
#         width_D0 = 0.012
#     if Dm == 203 and D0 == 1:
#         width_Dm = 0.017
#         width_D0 = 0.016
#     if Dm == 203 and D0 == 3:
#         width_Dm = 0.015
#         width_D0 = 0.010
#     if Dm == 203 and D0 == 4:
#         width_Dm = 0.017
#         width_D0 = 0.016
#     if Dm == 204 and D0 == 0:
#         width_Dm = 0.011
#         width_D0 = 0.012
#     if Dm == 204 and D0 == 1:
#         width_Dm = 0.011
#         width_D0 = 0.016
#     if Dm == 204 and D0 == 3:
#         width_Dm = 0.012
#         width_D0 = 0.012
#     if Dm == 204 and D0 == 4:
#         width_Dm = 0.011
#         width_D0 = 0.014
#     return width_Dm, width_D0

def cal_range(D, D_W):
    if D == 'D0':
        D0_signal_low = 1.86483 - D_W/2.
        D0_signal_up = 1.86483 + D_W/2.
        D0_sidebandlow_up = D0_signal_low - D_W
        D0_sidebandlow_low = D0_signal_low - 2. * D_W
        D0_sidebandup_low = D0_signal_up + D_W
        D0_sidebandup_up = D0_signal_up + 2. * D_W
        return D0_signal_low, D0_signal_up, D0_sidebandlow_up, D0_sidebandlow_low, D0_sidebandup_low, D0_sidebandup_up
    if D == 'Dm':
        Dm_signal_low = 1.86965 - D_W/2.
        Dm_signal_up = 1.86965 + D_W/2.
        Dm_sidebandlow_up = Dm_signal_low - D_W
        Dm_sidebandlow_low = Dm_signal_low - 2. * D_W
        Dm_sidebandup_low = Dm_signal_up + D_W
        Dm_sidebandup_up = Dm_signal_up + 2. * D_W
        return Dm_signal_low, Dm_signal_up, Dm_sidebandlow_up, Dm_sidebandlow_low, Dm_sidebandup_low, Dm_sidebandup_up


def search(allfile, root, target):
    items = os.listdir(root)
    for item in items:
        if item[0] == '.':
            continue
        path = os.path.join(root, item)
        if os.path.isdir(path):
            search(allfile, path, target)
        else:
            if target in path:
                allfile.append(path)
    return allfile
    
def group_files_by_num(name_list, num_total):
    groups = []
    group = []
    num_sum = 0

    for name in name_list:
        if int(num_sum) < int(num_total):
            group.append(name)
            num_sum = num_sum + 1
        else:
            groups.append(group)
            group = []
            num_sum = 0
            group.append(name)
            num_sum = num_sum + 1

        if name == name_list[-1]:
            groups.append(group)    
    return groups

# ---------------------------------------------
# Class 
# ---------------------------------------------

class UserFile(object):
    '''Class to handle file  '''
    def __init__(self, filename=None, data=None):
        self.data = []
        if data != None:
            self.data = data
            
        if filename:
            self.input(filename)
            self.file = filename

    def append(self, content):
        self.data.append(content)

    def input(self, filename, verbose=0):
        fi = open(filename, 'r')
        for line in fi:
            self.data.append(line)
        fi.close()

    def input_data(self, data):
        self.data = data
        
        
class BossLogFile(UserFile):
    "Handle BOSS log file"

    def __init__(self, filename=None):
        self.terminated = False
        UserFile.__init__(self, filename)
        self.parse()
        
    def parse(self):
        "parse BOSS log file" 
        line_no = -1
        for line in self.data:
            line_no += 1
            line = line.strip()
            if 'INFO Application Manager Terminated successfully' in line:
                self.terminated = True

class EventsLogFile(UserFile):
    "Handle Events log file"

    def __init__(self, filename=None):
        self.terminated = False
        UserFile.__init__(self, filename)
        self.parse()
        
    def parse(self):
        "parse Events log file" 
        line_no = -1
        for line in self.data:
            line_no += 1
            line = line.strip()
            if 'Done ' in line:
                self.terminated = True

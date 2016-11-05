# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

VERSION = "0.0.0"
APPNAME = "repo-sql"

from waflib import Logs, Utils, Context
import os

def options(opt):
    opt.load(['compiler_cxx', 'gnu_dirs'])

def configure(conf):
    conf.load(['compiler_cxx', 'gnu_dirs'])

def build(bld):
    bld(target='bin/repo-sql',
        features='cxx cxxprogram',
        source='src/main.cpp')

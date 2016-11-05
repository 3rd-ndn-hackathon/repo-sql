# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

VERSION = "0.0.0"
APPNAME = "repo-sql"

from waflib import Logs, Utils, Context
import os

def options(opt):
    opt.load(['compiler_cxx', 'gnu_dirs'])

def configure(conf):
    conf.load(['compiler_cxx', 'gnu_dirs'])

    conf.env['CXXFLAGS'] = ['-std=c++11', '-O0', '-g3', '-pedantic', '-Wall', '-Wextra']

    conf.check_cfg(package='libndn-cxx', args=['--cflags', '--libs'],
                   uselib_store='NDN_CXX', mandatory=True)

def build(bld):
    bld(target='reposql-objects',
        features='cxx',
        source=bld.path.ant_glob(['src/**/*.cpp'], excl=['src/main.cpp']),
        use='NDN_CXX')

    bld(target='bin/repo-sql',
        features='cxx cxxprogram',
        source='src/main.cpp',
        use='reposql-objects')

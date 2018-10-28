# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

VERSION = "0.0.0"
APPNAME = "repo-sql"

from waflib import Logs, Utils, Context
import os

def options(opt):
    opt.load(['compiler_cxx', 'gnu_dirs'])
    opt.load(['boost'], tooldir=['.waf-tools'])

REQUIRED_BOOST_LIBS = ['system', 'thread', 'log', 'log_setup', 'unit_test_framework']

def configure(conf):
    conf.load(['compiler_cxx', 'gnu_dirs', 'boost'])

    conf.env['CXXFLAGS'] = ['-std=c++14', '-O2', '-g', '-pedantic', '-Wall', '-Wextra']

    conf.check_cfg(package='libpq', args=['--cflags', '--libs'],
                   uselib_store='PQ', mandatory=True)

    conf.check_cfg(package='libndn-cxx', args=['--cflags', '--libs'],
                   uselib_store='NDN_CXX', mandatory=True)

    conf.check_boost(lib=REQUIRED_BOOST_LIBS, mt=False);

def build(bld):
    bld(target='reposql-objects',
        features='cxx',
        source=bld.path.ant_glob(['src/**/*.cpp', 'postgrespp/*.cpp'], excl=['src/main.cpp']),
        use='NDN_CXX PQ BOOST',
        includes='.')

    bld(target='unit-tests',
        features='cxx cxxprogram',
        source=bld.path.ant_glob(['test/**/*.cpp']),
        use='reposql-objects NDN_CXX BOOST',
        includes='. src',
        install_path=None)

    bld(target='bin/repo-sql',
        features='cxx cxxprogram',
        source='src/main.cpp',
        use='reposql-objects NDN_CXX BOOST')

    bld(target='bin/repo-sql-insert',
        features='cxx cxxprogram',
        source='tool/insert.cpp',
        use='reposql-objects NDN_CXX BOOST',
        includes='src')

    bld(target='bin/datagen',
        features='cxx cxxprogram',
        source='tool/datagen.cpp',
        use='NDN_CXX BOOST',
        includes='src',
        install_path=None)

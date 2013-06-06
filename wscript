#!/usr/bin/env python
import sys, os

def options(opt):
    opt.load('compiler_cxx')

def configure(cfg):
    cfg.load('compiler_cxx')

def build(bld):
    bld(target          = 'bitter',
        export_includes = '.')

    # install headers
    for header in bld.path.ant_glob('bitter/**/*.(h|hpp|ipp)'):
        bld.install_as('${PREFIX}/include/%s' % header.path_from(bld.path), header)

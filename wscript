#!/usr/bin/env python
import sys, os

def options(opt):
    opt.load('g++')

def configure(cfg):
    cfg.load('g++')

def build(bld):
    bld(
        target          = 'bitter',
        export_includes = '.',
    )

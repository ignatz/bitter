#!/usr/bin/env python
import sys, os

components = []

recurse = lambda ctx : map(lambda proj: ctx.recurse(proj), components)

def options(opt):
    recurse(opt)

    opt.load('g++')

def configure(cfg):
    recurse(cfg)

    cfg.load('g++')

    cfg.env.INCLUDES_BITTER = ['.',]

def build(bld):
    recurse(bld)

    bld(
        target          = 'bitter',
        export_includes = bld.env.INCLUDES_BITTER,
    )

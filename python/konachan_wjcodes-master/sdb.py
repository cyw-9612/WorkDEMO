#!/bin/env python3
# -*- coding: utf-8 -*- (for python2.x)

import sys
import os
import logging

log_path = sys.path[0] + '/log/'
log_file_default = log_path + 'debug.log'


def logger_init(output_file=None):
    if not os.path.exists(log_path):
        os.makedirs(log_path)

    logging.addLevelName(50, '#')   # CRITICAL
    logging.addLevelName(40, 'x')   # ERROR
    logging.addLevelName(30, '!')   # WARNING
    logging.addLevelName(20, '.')   # INFO
    logging.addLevelName(10, '>')   # DEBUG
    logging.addLevelName(0,  ' ')   # NOTSET

    log = logging.getLogger('root')
    log.setLevel(level=logging.DEBUG)

    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    console.setFormatter(logging.Formatter(
        fmt='%(filename)16s:%(lineno)04d %(levelname)s %(message)s',
        datefmt='%y%m%d%H%M%S'))
    log.addHandler(console)

    if output_file is not None:
        handler = logging.FileHandler(output_file)
        handler.setLevel(logging.INFO)
        handler.setFormatter(logging.Formatter(
            fmt=('%(asctime)s %(name)16s %(filename)16s:%(lineno)04d '
                 '%(levelname)s %(message)s'),
            datefmt='%y%m%d%H%M%S'))
        log.addHandler(handler)
    return log


def menu(l, p=None):
    while 1:
        i = 1
        print('\n ####')
        for item in l:
            print(' # %3d %s' % (i, item['name']))
            i += 1
        print(' #   0 return')

        if p is None:
            try:
                n = int(input('input item number:'))
            except Exception as e:
                print('get number failed: %s' % e)
                continue
        else:
            n = p

        #  print('get item number:%d' % n)
        if n == 0:
            print('quit\n')
            break
        elif n < i:
            n -= 1
            #  print('enter function: %r' % l[n]['func'])
            print(' ####')
            if l[n].get('func'):
                p = None
                if 'param' in l[n]:
                    p = l[n]['param']
                l[n]['func'](p)
            if p is not None:
                break
        else:
            print('number(%d) out of range(0~%d)' % (n, i))
            p = None

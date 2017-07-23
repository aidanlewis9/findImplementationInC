#!/usr/bin/env python2.7

import os
import sys
import commands
import argparse

funcs = ['find', 'search']
coms = ['/etc', '/etc -exec echo \{\} \;']


for com in coms:
  for func in funcs:
    start = False
    syscalls = {}
    trash, data = commands.getstatusoutput("strace -c {} {} | grep -v /etc".format(func, com))
    formatted = data.split('\n')
    for line in formatted:
      if line[0] == '-':
	start = not start
      if start and line[0] != '-':
	line = line.split() 
	if line[4].isdigit():
	  syscalls[line[5]] = int(line[3]);
	else:
	  syscalls[line[4]] = int(line[3]);
    print "#", func, com
    for i in sorted(syscalls, key=syscalls.get, reverse=True):
      print '{:>6} {:<2}'.format(syscalls[i], i)
    print
	
       

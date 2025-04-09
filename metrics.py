#!/usr/bin/python
import re
import os
import datetime
import csv
import subprocess as sp
import sys
import time

# Parameters
SIZE = 100000
#SIZE = 16777

# Executes the given string as a shell command.
# Returns the standard output and error.
def execute_command(command):
  p = sp.Popen(command, stdout = sp.PIPE, stderr = sp.PIPE)
  (stdout,stderr) = p.communicate()
  if p.returncode is not 0:
    raise RuntimeError("Command did not exit properly.")
  return (stdout,stderr)

# Returns the shell command as a string for executing perf, given the 
# metrics string and executable string.
def make_valgrind_command(executable):
  return ['valgrind', '--log-file=metrics.txt', '--tool=cachegrind', '--cache-sim=yes', '--cachegrind-out-file=/dev/null'] + executable

def read_valgrind(output, metric):
  ans = None
  lines = output.splitlines()
  for line in lines:
    idx = line.find(metric)
    if idx != -1:
      vals = line.split()
      try:
        ans = float(''.join(vals[3].split(',')))
      except ValueError:
        raise RuntimeError("Value %s found for D1." % vals[3])

  return ans

# Returns the performance metrics for one run.
def run_case(sort_option, nelements):
  try:
    os.unlink("metrics.txt")
  except:
    pass

  execute_command(['make', 'clean'])
  execute_command(['make'])

  start = time.time()
  execute_command(["./sort", sort_option, '-n', str(nelements)])
  elapsed_time = time.time() - start

  funnel_command = make_valgrind_command(["./sort", sort_option, '-n', str(nelements)])
  print ' '.join(funnel_command)

  execute_command(funnel_command)

  with open('metrics.txt','r') as fd:
    output = fd.read()

  return {'D1_misses': read_valgrind(output,"D1  misses"),
          'elapsed_time': elapsed_time}

# Main script routine.
def main():
  quick_metrics = run_case("-q", SIZE)
  print "Quick sort refs: "+str(quick_metrics['elapsed_time'])+" s"
  print "Quick sort L1 data cache misses: "+str(quick_metrics['D1_misses'])

  funnel_metrics = run_case("-f", SIZE)
  print "Funnel sort refs: "+str(funnel_metrics['elapsed_time'])+" s"
  print "Funnel sort L1 data cache misses: "+str(funnel_metrics['D1_misses'])

  print 
  print "Funnel sort / quick sort elapsed time ratio: "+ \
    str(funnel_metrics['elapsed_time']/quick_metrics['elapsed_time'])
  print "Funnel sort / quick sort L1 data cache miss ratio: "+ \
    str(float(funnel_metrics['D1_misses'])/float(quick_metrics['D1_misses']))

if __name__ == '__main__':
    if len(sys.argv) == 2:
        try:
            SIZE = int(sys.argv[-1])
        except:
            print 'Failed to use given size. Using default size: ', SIZE
    main()

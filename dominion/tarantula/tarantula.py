#!/usr/bin/env python2
import sys, os, subprocess

if __name__ == "__main__":
    print >> sys.stderr, "Running from", os.getcwd()
    num_tests = 1000
    # filenames
    fname_gcda = "../build/dominion.gcda"
    fname_gcno = "../build/dominion.gcno"
    fname_gcov = "../build/dominion.c.gcov"
    fname_csrc = "../dominion.c"
    fname_exec = "../build/ttest"
    
    if os.path.isfile(fname_gcda):
        print >> sys.stderr, "Existing coverage data found, removing", fname_gcda
        os.remove(fname_gcda)
    
    # Analyze source
    print >> sys.stderr, "Analyzing source..."
    with open(fname_csrc) as f:
        for i, l in enumerate(f):
            pass
    line_count = i + 1
    
    # Init tracking
    print >> sys.stderr, "Allocating tracker..."
    succ_lines = [0] * line_count
    fail_lines = [0] * line_count
    
    # Test and track coverage
    devnull = open(os.devnull, 'w')
    failed = 0
    passed = 0
    for i in range(0,num_tests):
        print >> sys.stderr, i,
        
        # run and choose
        print >> sys.stderr, "running,",
        res = subprocess.call([fname_exec, str(i)], stdout=devnull, stderr=devnull)
        if res == 0:
            into = succ_lines
            passed += 1
        else:
            into = fail_lines
            failed += 1
        
        
        # generage coverage report
        print >> sys.stderr, "gcov,",
        subprocess.call(['gcov', fname_csrc], stdout=devnull, stderr=devnull);
        
        # parse and analyze coverage report
        print >> sys.stderr, "analyzing,",
        with open(fname_gcov) as f:
            for i, l in enumerate(f):
                pline = l.strip().split()
                count = pline[0].split(':')[0]
                lnum = pline[1].split(':')[0]
                if count.isdigit():
                    into[int(lnum)-1] += int(count)
        
        # remove coverage data
        print >> sys.stderr, "cleaning,",
        if os.path.isfile(fname_gcda):
            os.remove(fname_gcda)
        print >> sys.stderr, "complete."
    
    # print colorized lines
    print >> sys.stderr, '---- Results ----'
    ratio = float(failed)/float(passed)
    
    GBACK = '\033[42m\033[30m'
    YBACK = '\033[43m\033[30m'
    RBACK = '\033[41m\033[30m'
    RESET = '\033[0m'
    
    with open(fname_csrc) as f:
        for i, l in enumerate(f):
            color = RESET
            if succ_lines[i]:
                lratio = float(fail_lines[i])/float(succ_lines[i])
                color = YBACK if lratio > ratio else GBACK
            elif fail_lines[i]:
                color = RBACK
            
            print color, str(succ_lines[i]).ljust(8), \
                str(fail_lines[i]).ljust(8), l, RESET,
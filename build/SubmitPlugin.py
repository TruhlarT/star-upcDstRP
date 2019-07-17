#!/usr/bin/python

#--------------------------------------------------------------
# macro to submit rpDst analysis 
#
# usage:
# ./SubmitPlugin.py outputDir inputSource
# ./SubmitPlugin.py outputDir
#
#--------------------------------------------------------------

import os
from subprocess import Popen
import re
import sys
import time


#_____________________________________________________________________________
if __name__ == "__main__":

    #name of config file from command line argument
    args = sys.argv
    args.pop(0) # cut first input from terminal = ./SubmitPlugin
    if len(args) == 2:
        outputDir = args.pop(0) # read third input from terminal = outputDir
        inputSource = args.pop(0) # read second input from terminal = inputSource
    elif len(args) == 1:
        outputDir = args.pop(0) # read third input from terminal = outputDir
        #inputSource = "/gpfs01/star/pwg/truhlar/Test/ELtrig/merge_files/StUPCRP_production.list"
        #inputSource = "/gpfs01/star/pwg/truhlar/ConnectionTest/total.list"
        inputSource = "/gpfs01/star/pwg/jaroslav/test/star-upcDst/trees/RP_central_elastic_17_v2/merge_files/StUPCRP_central_elastic_17_v2.list"
        #inputSource = "/gpfs01/star/pwg/truhlar/ConnectionTest/central.list"
        #inputSource = "/gpfs01/star/pwg/truhlar/ConnectionTest/elastic.list"
    else:
        print "Wrong input arguments"
        exit()
    top = "/gpfs01/star/pwg/truhlar/Run17Outputs"
    srcDir = os.getcwd()
    basedir = top + "/" + outputDir
    nProcesses = str(len(open(inputSource).readlines(  )))
    print("Input source: "+inputSource)
    print("Output dir: "+basedir)
    print("Source dir: "+srcDir)
    print("Number of processes: "+nProcesses)
    #create jobs output folders
    Popen("mkdir -p "+basedir, shell=True).communicate()
    #proceed with scheduler submit
    Popen("mkdir "+basedir+"/logs", shell=True).communicate()
    Popen("mkdir "+basedir+"/sched", shell=True).communicate()
    #create xml
    xmlname = "submit.xml"
    xml = open(xmlname, "w")
    for line in open("scheduler_template.xml", "r"):
        line = re.sub(r"__BASEDIR__", basedir, line)
        line = re.sub(r"__INPUT__", inputSource, line)
        line = re.sub(r"__NProcesses__", nProcesses, line)
        line = re.sub(r"__SRCDIR__", srcDir, line)
        xml.write(line)
    xml.close()
    #submit for a given catalog query
    Popen("star-submit "+xmlname, shell=True).communicate()
    print "All done."





















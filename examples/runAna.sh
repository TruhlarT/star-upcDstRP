#!/bin/bash
cd output
rm *
cd ../
cat /gpfs01/star/pwg/truhlar/star-upcDst/merge_allRP/StUPCRP_production.list | while read line
do
	root -l -q run.C\(\"$line\",\"$output\"\)
done
hadd output.root *.root

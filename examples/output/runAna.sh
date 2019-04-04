#!/bin/bash
rm output0*.root
cat /gpfs01/star/pwg/truhlar/star-upcDst/merge_allRP/StUPCRP_production.list | while read line
do
	output="output"$(echo "$line" | cut -d '_' -f 4 | cut -d '.' -f 1 )".root"
	root -l -q run.C\(\"$line\",\"$output\"\)
done
hadd output.root output0*.root

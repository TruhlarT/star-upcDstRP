#!/usr/bin/bash
rm *.session.xml 2>/dev/null
rm *.package.zip 2>/dev/null
rm -r *.package
rm *.zip

rm -r sched*
rm -r log

mkdir sched
mkdir log
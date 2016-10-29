#!/usr/bin/python

import sys
import os
import re

class RevisionInfo:
	def __init__(self, revision, name):
		self.revision = revision
		self.name = name

Author = "Lichuang"
ScriptName = "svn_check"

CheckFile = ""
Keyword = ""
RevisionList = []

def usage():
	print ScriptName + "written by" + Author
	print "usage:" + ScriptName + " filename keyword"

def check_diff():
	print "Now searching svn diff...\n"

	global CheckFile
	global Keyword
	global RevisionList

	newest = RevisionList[0]
	for revisioninfo in RevisionList[1:]:
		range = revisioninfo.revision + ":" + newest.revision  
		cmd = "svn diff -r " + range + " " + CheckFile
		pipe = os.popen(cmd)
		out = pipe.read()
		list = re.split("\n", out)

		prog = re.compile(Keyword)
		flag = 0
		for i in list:
			if prog.search(i) != None:
				if flag == 0:
					print "matched in revision " + newest.revision + ", checked in by", newest.name + ":"
					flag = 1
				print i
		if flag != 0:				
			print ""

		newest = revisioninfo
		
	print "Search svn diff done!"

def check_log():
	print "Now checking svn log...\n"
	global CheckFile

	cmd = "svn log " + CheckFile
	pipe = os.popen(cmd)
	out = pipe.read()

	list = re.split("\n", out)

	prog = re.compile("^r(\d+) \| (\w+)")
	for i in list:
		m = prog.search(i)
		if m != None and m.group(1) != None and m.group(2) != None:
			RevisionList.append(RevisionInfo(m.group(1), m.group(2)))

	print "Search check svn log done!\n"

def get_args():
	global CheckFile
	global Keyword

	arg_len = len(sys.argv)
	if (arg_len != 3):
		usage()
		sys.exit(-1)

	CheckFile = sys.argv[1]
	Keyword   = sys.argv[2]

if __name__ == "__main__":
	get_args()
	check_log()
	check_diff()


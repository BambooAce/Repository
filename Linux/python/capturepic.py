#!/usr/bin/python3
# -*- coding: utf-8 -*-
from urllib import request
from html.parser import HTMLParser
import os
import sys
class Capture(HTMLParser):
	def __init__(self):
		HTMLParser.__init__(self)
		self.links = []
	
	def handle_starttag(self, tag, attrs):
		if tag == "img":
			for name, value in attrs:
				if name == 'src':
					self.links.append(value)

if __name__ == '__main__':
	msg = None
	if len(sys.argv) != 2:
		print("capture.py http://izzs.co/dnn051.html")
		exit(1)
	else:
		response = request.urlopen(sys.argv[1])
		msg = response.read().decode('utf-8')
	hp = Capture()
	hp.feed(msg)
	hp.close()
	f = open("a.html", "w", encoding = "utf-8")
	for link in hp.links:
		hl = '<img src="' + link + '" >'
		f.write(hl)
	f.close()


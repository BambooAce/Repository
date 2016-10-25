#/usr/bin/env python
#coding:utf-8
import json
data ={
	'Name':'ACE',
	'computer':'ASIS',
	'alise':{'age':123, 'male':1},
	'listi':(23,234,556)
}
with open('data.json', 'w') as f:
	json.dump(data, f)

with open('info.json', 'r') as g:
	info=json.load(g)
	print info, info['age']

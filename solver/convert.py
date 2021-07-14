from bs4 import BeautifulSoup
import sys

filename = sys.argv[1]

with open(filename, 'r') as f:
    data = f.read()
 
Bs_data = BeautifulSoup(data, "xml")

b_unique = Bs_data.find_all('variable')

for b in b_unique:
	s = b['name']
	if s[0]=='x':
		i = s.find('_')
		u = s[1:i]
		v = s[i+1:]
		print(u, v, b['value'])

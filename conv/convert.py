from bs4 import BeautifulSoup
import sys

filename = sys.argv[1]

with open(filename, 'r') as f:
    data = f.read()
 
Bs_data = BeautifulSoup(data, "xml")
  
# Finding all instances of tag 
# `unique`
b_unique = Bs_data.find_all('variable')
  
#print(b_unique)

for b in b_unique:
	s = b['name']
	if s[0]=='x':
		i = s.find('_')
		u = s[1:i]
		v = s[i+1:]
		print(u, v, b['value'])

  
# Using find() to extract attributes 
# of the first instance of the tag
#b_name = Bs_data.find('child', {'name':'Frank'})
  
#print(b_name)
  
# Extracting the data stored in a
# specific attribute of the 
# `child` tag
#value = b_name.get('test')
  
#print(value)

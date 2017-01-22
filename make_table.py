

with open("/tmp/table") as f:
	content = f.readlines()


lineNr=0
for line in content:
	if lineNr==0:
		print("^",end="")
	else:
		print("|",end="")
	for word in line.split():
		print(word,end="")
		if lineNr == 0:
			print("^",end="")
		else:
			print("|",end="")
	lineNr+=1
	print()

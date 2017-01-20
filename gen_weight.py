import sys
from random import randint

if len(sys.argv) != 4:
	print("Wrong number of parameters. USAGE: ./gen_weight <from> <to> <seq_len>")
	exit(1)

from_num = sys.argv[1]
to_num = sys.argv[2]
seq_len = sys.argv[3]

for _ in range(0,int(seq_len)):
	print(str(randint(int(from_num),int(to_num))))

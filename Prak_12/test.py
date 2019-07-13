import subprocess
from threading import Thread
import os
from glob import iglob
import sys

def thread_run(i):
	print("Start ", i)
	subprocess.run(["./client", "test", "{}.out".format(i)], stdout=subprocess.DEVNULL)
	print("Ende  ", i)


threads = [Thread(target=thread_run, args=(i,)) for i in range(int(sys.argv[1]))]

for t in threads:
	t.start()

for t in threads:
	t.join()

for p in iglob("*.out"):
	os.remove(p)
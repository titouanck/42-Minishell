import pty
import sys
import os
import subprocess
import shutil

if len(sys.argv) != 2:
	print(f"Usage: python {sys.argv[0]} <program>")
	exit(1)

if (sys.argv[1] == "minishell"):
	prog = ['./minishell']
	stdout = "tester/minishell_stdout"
	# stderr = "tester/minishell_stderr"
	returnval_file = "tester/minishell_returnval"
else:
	prog = ['bash', '--posix']
	stdout = "tester/bash_stdout"
	# stderr = "tester/bash_stderr"
	returnval_file = "tester/bash_returnval"

stderr = "/dev/stderr"

if os.path.exists(stdout):
	os.remove(stdout)

# if os.path.exists(stderr):
#     os.remove(stderr)

if os.path.exists(returnval_file):
	os.remove(returnval_file)

def instructions():

	# Afficher un prompt en l’attente d’une nouvelle commande
	master, slave, proc = open_it()

	line(master, "\n")
	line(master, "exit\n")
	
	returnVal = close_it(master, slave, proc)
	with open(returnval_file, "a") as rf:
		rf.write(str(returnVal) + "\n")

	# Posséder un historique fonctionnel.
	master, slave, proc = open_it()
   
	line(master, "printf \"42\\n\"\n")
	line(master, '\x1b[A\n')
	line(master, "\n")
	line(master, '\x1b[A\n')
	line(master, 'exit\n')

	returnVal = close_it(master, slave, proc)
	with open(returnval_file, "a") as rf:
		rf.write(str(returnVal) + "\n")

	# Chercher et lancer le bon exécutable 
	master, slave, proc = open_it()

	line(master, "   ls\n")
	line(master, "\t" + ls_path + " -a\n")
	line(master, "cp " + ls_path + " ls2\v\n")
	line(master, "ls2\n")
	line(master, ".ls2\n")
	line(master, "./ls2\n")
	line(master, "/ls2\n")
	line(master, "/tmp\n")
	line(master, "rm ls2\n")
	line(master, "./tester/tester.py\n")

	line(master, "")
	line(master, "exit\n")

	returnVal = close_it(master, slave, proc)
	with open(returnval_file, "a") as rf:
		rf.write(str(returnVal) + "\n")

	# Gérer ’ (single quote) qui doit empêcher le shell d’interpréter les méta-caractères
	master, slave, proc = open_it()

	line(master, "echo \'$PWD << (not a here-doc) > (don't redirect) < (not an infile) >> (not an outfile)\'\n")
	line(master, "exit\n")

	returnVal = close_it(master, slave, proc)
	with open(returnval_file, "a") as rf:
		rf.write(str(returnVal) + "\n")

	# Gérer " (double quote) qui doit empêcher le shell d’interpréter les méta-caractères sauf le $
	master, slave, proc = open_it()

	line(master, "echo \"$PWD << (not a here-doc) > (don't redirect) < (not an infile) >> (not an outfile)\"\n")
	line(master, "exit\n")

	returnVal = close_it(master, slave, proc)
	with open(returnval_file, "a") as rf:
		rf.write(str(returnVal) + "\n")

def ignore_sigint(signalnum, frame):
	pass

def open_it():
	with open(stdout, "a") as output_file:
		with open(stderr, "a") as error_file:
			master, slave = pty.openpty()
			proc = subprocess.Popen(
				prog,
				stdin=slave,
				stdout=output_file,
				stderr=error_file,
				universal_newlines=True,
			)
	return (master, slave, proc)

def close_it(master, slave, proc):
	proc.wait()
	os.close(master)
	os.close(slave)
	return proc.returncode

def line(master, instruction):
	os.write(master, instruction.encode())

ls_path = shutil.which('ls')
grep_path = shutil.which('grep')
cat_path = shutil.which('cat')
if ls_path is None or grep_path is None or cat_path is None:
	print("At least one command needed by this tester cannot by find.\n")
	exit (1)

instructions()

import pty
import os
import subprocess
import shutil

if os.path.exists("tester_out"):
    os.remove("tester_out")

if os.path.exists("tester_returnval"):
    os.remove("tester_returnval")

def	instructions():
	master, slave, proc = open_minishell()

	line(master, "ls -la\n")
	line(master, ls_path + " -la\n")
	line(master, "exit\n")

	returnVal = close_minishell(master, slave, proc)
	with open("tester_returnval", "a") as returnval_file:
		returnval_file.write(str(returnVal) + "\n")


def ignore_sigint(signalnum, frame):
    pass

def open_minishell():
	# with open("tester_out", "a") as output_file:
	with open("/dev/stdout", "a") as output_file:
		master, slave = pty.openpty()
		proc = subprocess.Popen(['../minishell'], stdin=slave, stdout=output_file)
	return (master, slave, proc)

def	close_minishell(master, slave, proc):
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

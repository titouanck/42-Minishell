import pty
import os
import subprocess
import time
import shutil

minishell_stdout = "minishell_stdout.txt"
bash_stdout = "bash_stdout.txt"

ls_path = shutil.which('ls')
grep_path = shutil.which('grep')
env_path = shutil.which('env')

def delete_files():
    if os.path.exists(minishell_stdout):
        os.remove(minishell_stdout)
    if os.path.exists(bash_stdout):
        os.remove(bash_stdout)

def input(instruction):
	minishell_master_out, minishell_slave_out = pty.openpty()
	with open(minishell_stdout, "w") as fd_minishell_stdout:
		minishell_process = subprocess.Popen(['./minishell'], stdin=minishell_slave_out, stdout=fd_minishell_stdout)
	os.write(minishell_master_out, instruction.encode())
	time.sleep(0.05)	
	os.close(minishell_slave_out)

	bash_master_out, bash_slave_out = pty.openpty()
	with open(bash_stdout, "w") as fd_bash_stdout:
		bash_process = subprocess.Popen(['bash', '--posix'], stdin=bash_slave_out, stdout=fd_bash_stdout)
	os.write(bash_master_out, instruction.encode())
	time.sleep(0.1)	
	os.close(bash_master_out)
    
	result = subprocess.run(["diff", minishell_stdout, bash_stdout])
	if result.returncode != 0:
		print("La commande diff a échoué, arrêt du programme.")
		print(f"${instruction}")
		exit(1)


delete_files()

# Display a prompt when waiting for a new command.
input("\n \
      printf \"42\\n\" \
      \n")

# Have a working history
input("printf \"42\\n\"\n \
      \x1b[A\n \
      \n \
      \x1b[A\n")

# Search and launch the right executable (based on the PATH variable or using a relative or an absolute path)
input("ls -l -I minishell_stdout.txt -I bash_stdout.txt\n")
input(ls_path + " -l -I minishell_stdout.txt -I bash_stdout.txt\n")
input("   " + ls_path + " -l -I    minishell_stdout.txt     -I       bash_stdout.txt\n")
input("./ls\n")
input(".ls\n")
input("    /ls\n")
input("/tmp   \n")
input("./tmp\n")
input("     ./tester.py     \n")


# Not use more than one global variable : Can't verify that here

# Not interpret unclosed quotes or special characters which are not required by the subject : Can't verify that here

# Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence
input("echo \'$\'\'PW\'D\' << (not \'a\' here-doc) > (do not redirect) \"\"<\"\" (not an infile) >> (not an outfile)\'\n")
input("echo \"$\"\"PW\'D\' << (not a her\'e\'-doc) \'\'>\'\' (do not redirect) < (not an infile) >> (not an outfile)\"\n")

# Implement redirections
input("cat < Makefile -e > out\n" \
	+ "cat out\n" \
    + "cat < Makefile -e >> out\n"
    + "cat out\n" \
    + "rm out\n" \
    + "cat < Makefile -e >> out\n" \
	+ "cat out\n" \
    + "rm out\n" \
)

input("cat << fake < Makefile << \"just a limiter\"\n" \
    + "42\n" \
    + "fake\n" \
    + "42\n" \
    + "\"just a limiter\"\n" \
    + "\'just a limiter\'\n" \
    + "just a limiter\n" \
)

# Implement pipes (| character). The output of each command in the pipeline is connected to the input of the next command via a pipe.
input("printf \"42\\n | printf \"42\\n | printf \"42\\n")
input(env_path + " | grep \'$HOME\'\n")
input(env_path + " | grep \"$\"HOM\"E\"\n")

# echo '$''PW'D' << (not 'a' here-doc) > (do not redirect) ""<"" (not an infile) >> (not an outfile)'
# delete_files()

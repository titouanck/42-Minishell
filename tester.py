import pty
import os
import subprocess
import time
import shutil
import re

minishell_stdout = "minishell_stdout.txt"
minishell_stderr = "minishell_stderr.txt"
bash_stdout = "bash_stdout.txt"
bash_stderr = "bash_stderr.txt"

ignore_files = "-I " + minishell_stdout + " -I " + \
    minishell_stderr + " -I " + bash_stdout + " -I " + bash_stderr

ls_path = shutil.which('ls')
grep_path = shutil.which('grep')
env_path = shutil.which('env')

NC = '\033[0m'
RED = '\033[0;31m'
REDB = '\033[1;31m'
GREEN = '\033[0;32m'
GREENB = '\033[1;32m'
ORANGE = '\033[0;33m'
BLUE = '\033[1;34m'
PURPLE = '\033[0;35m'
CYAN = '\033[0;36m'
LIGHTGRAY = '\033[0;37m'

DARKGRAY = '\033[1;30m'
LIGHTRED = '\033[1;31m'
LIGHTGREEN = '\033[1;32m'
LIGHTBLUE = '\033[1;34m'
LIGHTPURPLE = '\033[1;35m'
LIGHTCYAN = '\033[1;36m'
YELLOW = '\033[1;33m'
WHITE = '\033[0;37m'


def delete_files():
    if os.path.exists(minishell_stdout):
        os.remove(minishell_stdout)
    if os.path.exists(bash_stdout):
        os.remove(bash_stdout)
    if os.path.exists(minishell_stderr):
        os.remove(minishell_stderr)
    if os.path.exists(bash_stderr):
        os.remove(bash_stderr)

g_nbr = 0
g_stdin = 0
g_stderr = 0
g_exitcode = 0

def input(instruction):
    global g_nbr
    global g_stdin
    global g_stderr
    global g_exitcode
    
    g_nbr += 1
    minishell_master_out, minishell_slave_out = pty.openpty()
    with open(minishell_stdout, "w") as fd_minishell_stdout:
        with open(minishell_stderr, "w") as fd_minishell_stderr:
            minishell_process = subprocess.Popen(
                ['./minishell'], stdin=minishell_slave_out, stdout=fd_minishell_stdout, stderr=fd_minishell_stderr)
    os.write(minishell_master_out, instruction.encode())
    os.write(minishell_master_out, "exit\n".encode())
    minishell_process.wait()
    minishell_exitcode = minishell_process.returncode
    os.close(minishell_slave_out)

    bash_master_out, bash_slave_out = pty.openpty()
    with open(bash_stdout, "w") as fd_bash_stdout:
        with open(bash_stderr, "w") as fd_bash_stderr:
            bash_process = subprocess.Popen(
                ['bash', '--posix'], stdin=bash_slave_out, stdout=fd_bash_stdout, stderr=fd_bash_stderr)
    os.write(bash_master_out, instruction.encode())
    os.write(bash_master_out, "exit\n".encode())
    bash_process.wait()
    bash_exitcode = bash_process.returncode
    os.close(bash_master_out)

    # instruction
    print(f"{PURPLE}{instruction}{NC}", end="")
    
    # Check stdin
    result = subprocess.run(["diff", minishell_stdout, bash_stdout])
    if result.returncode != 0:
        print(f"{RED}  → stdin KO!{NC}")
    else:
        print(f"{GREEN}  → stdin OK!{NC}")
        g_stdin += 1

    # Check stderr
    with open(minishell_stderr, 'r') as file:
        minishell_contenu = file.read()

    with open(bash_stderr, 'r') as file:
        bash_contenu = file.read()

    expression = r'^bash: line[^:]*:'
    if bash_contenu.startswith('bash: line'):
        bash_contenu = re.sub(expression, 'minishell:', bash_contenu, count=1)
    elif bash_contenu.startswith('bash:'):
        bash_contenu = 'minishell:' + bash_contenu[5:]
    if (minishell_contenu != bash_contenu):
        if (minishell_contenu == "" or bash_contenu == "") and (minishell_contenu != "" or bash_contenu != ""):
            print(f"{RED}  [STDERR]{NC}   ", end="")
            print(minishell_contenu, end="")
            if (minishell_contenu == ""):
                print("(null)")

            print(f"{RED}  [EXPECTED]{NC} ", end="")
            print(bash_contenu, end="")
            if (bash_contenu == ""):
                print("(null)")
        else:
            g_stderr += 1
            print(f"{ORANGE}  [STDERR]{NC}   ", end="")
            print(minishell_contenu, end="")
            if (minishell_contenu == ""):
                print("(null)")

            print(f"{ORANGE}  [OBJECTIF]{NC} ", end="")
            print(bash_contenu, end="")
            if (bash_contenu == ""):
                print("(null)")
    else:
        g_stderr += 1
        print(f"{GREEN}  → stderr OK!{NC}")
    
    # Check status
    if (minishell_exitcode == bash_exitcode):
        g_exitcode += 1;
        print(f"{GREEN}  → exit code OK!{NC}")
    else:
        print(f"{RED}  → exit code{NC} {minishell_exitcode}, {RED}expected{NC} {bash_exitcode}")
    print("")
                


delete_files()

print(f"{BLUE}Display a prompt when waiting for a new command.{NC}\n")
input("")
input("")

# # Have a working history
# input("printf \"42\\n\"\n \
#       \x1b[A\n \
#       \n \
#       \x1b[A\n \
# ")

# Search and launch the right executable (based on the PATH variable or using a relative or an absolute path)
print(f"{BLUE}Search and launch the right executable{NC}\n")
input("\"\"\n")
input("\'\'\n")
input("\n")
input("ls -l " + ignore_files + "\n")
input("ls \'-\'l " + ignore_files + "\n")
input(ls_path + " -l " + ignore_files + "\n")
input("   " + ls_path + " -l " + ignore_files + "\n")
input("./ls\n")
input(".ls\n")
input("    /ls\n")
input("/tmp   \n")
input("./tmp\n")
input("     ./tester.py     \n")


# Not use more than one global variable : Can't verify that here
# Not interpret unclosed quotes or special characters which are not required by the subject : Can't verify that here

# Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence
print(f"{BLUE}Handle single quote and double quote{NC}\n")
input("echo \'$\'\'PW\'D\' << (not \'a\' here-doc) > (do not redirect) \"\"<\"\" (not an infile) >> (not an outfile)\'\n")
input("echo \"$\"\"PW\'D\' << (not a her\'e\'-doc) \'\'>\'\' (do not redirect) < (not an infile) >> (not an outfile)\"\n")
input("echo \"$ \"\"PW\'D\' << (not a her\'e\'-doc) \'\'>\'\' (do not redirect) < (not an infile) >> (not an outfile)\"\n")
input("echo \' $\'\'PW\'D\' << (not \'a\' here-doc) > (do not redirect) \" \"<\"\" (not an infile) >> (not an outfile)\'\n")

# Implement redirections
print(f"{BLUE}Implement redirection{NC}\n")
# <, >, >>
input("cat < Makefile -e > out\n"
      "cat out\n"
      "cat < Makefile -e >> out\n"
      "cat out\n"
      "rm out\n")
input("cat < Makefile -e >> out\n"
      "cat out\n"
      "rm out\n")

# <<
input("cat << fake < Makefile << \"just a limiter\"\n"
      + "42\n"
      + "fake\n"
      + "42\n"
      + "\"just a limiter\"\n"
      + "\'just a limiter\'\n"
      + "just a limiter\n")

# Implement pipes (| character). The output of each command in the pipeline is connected to the input of the next command via a pipe.
print(f"{BLUE}Implement pipes{NC}\n")
input("printf \"42\\n\" | cat | cat | cat\n")
input("printf \"42\\n\" | cat | cat | cat\n")
input("printf \"42\\n\" | cat | printf \"4 8 15 16 23 42\\n\" | cat\n")
input("printf \"42\\n\" | cat | printf \"4 8 15 16 23 42\\n\" | cat > /dev/null\n")
input(env_path + " | grep \'$HOME\'\n")
input(env_path + " | grep \"$\"HOM\"E\"\n")

if (g_stdin == g_nbr):
    print(f"\033[1;37mSTDOUT:    {GREENB}{g_stdin}/{g_nbr}:  OK!{NC}")
else:
    print(f"\033[1;37mSTDOUT:    {REDB}{g_stdin}/{g_nbr}:  KO!{NC}")

if (g_stderr == g_nbr):
    print(f"\033[1;37mSTDERR:    {GREENB}{g_stderr}/{g_nbr}:  OK!{NC}")
else:
    print(f"\033[1;37mSTDERR:    {REDB}{g_stderr}/{g_nbr}:  KO!{NC}")

if (g_exitcode == g_nbr):
    print(f"\033[1;37mEXIT CODE: {GREENB}{g_exitcode}/{g_nbr}:  OK!{NC}")
else:
    print(f"\033[1;37mEXIT CODE: {REDB}{g_exitcode}/{g_nbr}:  KO!{NC}")

# echo '$''PW'D' << (not 'a' here-doc) > (do not redirect) ""<"" (not an infile) >> (not an outfile)'
delete_files()

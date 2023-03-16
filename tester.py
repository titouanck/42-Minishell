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
ORANGEB = '\033[1;33m'
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
WHITEB = '\033[1;37m'


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
g_stdout = 0
g_stderr = 0
g_exitcode = 0
g_stderrKO = 0

def input(instruction):
    global g_nbr
    global g_stdout
    global g_stderr
    global g_stderrKO
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
    
    # CHECK STDIN
    with open(minishell_stdout, 'r') as file:
        minishell_readed_stdout = file.read()

    with open(bash_stdout, 'r') as file:
        bash_readed_stdout = file.read()

    bash_readed_stdout = re.sub(r'bash: line \d+:', 'minishell:', bash_readed_stdout)
    bash_readed_stdout = bash_readed_stdout.replace('bash:', 'minishell:')
    if (minishell_readed_stdout != bash_readed_stdout):
        print(f"{REDB}  → stdout KO!{NC}")
        print(f"{WHITEB}MINISHELL{NC}")
        print(f"{RED}{minishell_readed_stdout}{NC}", end="")
        print(f"{WHITEB}EXPECTED{NC}")
        print(bash_readed_stdout, end="")
    else:
        g_stdout += 1
        print(f"{GREEN}  → stdout OK!{NC}")

    # CHECKSTDR
    with open(minishell_stderr, 'r') as file:
        minishell_readed_stderr = file.read()

    with open(bash_stderr, 'r') as file:
        bash_readed_stderr = file.read()

    bash_readed_stderr = re.sub(r'bash: line \d+:', 'minishell:', bash_readed_stderr)
    bash_readed_stderr = bash_readed_stderr.replace('bash:', 'minishell:')
    if (minishell_readed_stderr != bash_readed_stderr):
        if ((minishell_readed_stderr == "" or bash_readed_stderr == "") and (minishell_readed_stderr != "" or bash_readed_stderr != "") \
        or minishell_readed_stderr.count('\n') != bash_readed_stderr.count('\n')):
            g_stderrKO += 1
            print(f"{REDB}  → stderr KO!{NC}")
            print(f"{WHITEB}MINISHELL{NC}")
            if (minishell_readed_stderr == ""):
                print("(null)")
            print(f"{RED}{minishell_readed_stderr}{NC}", end="")
            print(f"{WHITEB}EXPECTED{NC}")
            if (bash_readed_stderr == ""):
                print("(null)")
            print(bash_readed_stderr, end="")
        else:
            print(f"{ORANGEB}  → stderr mismatch !{NC}")
            print(f"{WHITEB}MINISHELL{NC}")
            print(f"{RED}{minishell_readed_stderr}{NC}", end="")
            print(f"{WHITEB}EXPECTED{NC}")
            print(bash_readed_stderr, end="")
    else:
        g_stderr += 1
        print(f"{GREEN}  → stderr OK!{NC}")
    
    # CHECK EXIT CODE
    if (minishell_exitcode == bash_exitcode):
        g_exitcode += 1;
        print(f"{GREEN}  → exit code OK : {NC}{bash_exitcode}")
    else:
        print(f"{REDB}  → exit code{NC} {minishell_exitcode}, {REDB}expected{NC} {bash_exitcode}")
    print("")
                


delete_files()

print(f"{GREENB}green{GREEN} means OK{NC}")
print(f"{ORANGEB}orange{ORANGE} means manual check needed{NC}")
print(f"{REDB}red{RED} means KO{NC}\n")

print(f"{BLUE}Display a prompt when waiting for a new command.{NC}\n")
input("")
# input("echo -e \"\\n\"\n")
input("")

print(f"{BLUE}Have a working history.{NC}\n")
print(f"{ORANGEB}  → Must be check manually.{NC}\n")

print(f"{BLUE}Search and launch the right executable.{NC}\n")
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
input("./srcs\n")
input("     ./tester.py     \n")

print(f"{BLUE}Not use more than one global variable.{NC}\n")
print(f"{ORANGEB}  → Must be check manually.{NC}\n")

print(f"{BLUE}Not interpret unclosed quotes or special characters not required by the subject.{NC}\n")
print(f"{ORANGEB}  → Must be check manually.{NC}\n")

print(f"{BLUE}Handle single quote and double quote{NC}\n")
input("echo \'$\'\'PW\'D\' << (not \'a\' here-doc) > (do not redirect) \"\"<\"\" (not an infile) >> (not an outfile)\'\n")
input("echo \"$\"\"PW\'D\' << (not a her\'e\'-doc) \'\'>\'\' (do not redirect) < (not an infile) >> (not an outfile)\"\n")
input("echo \"$ \"\"PW\'D\' << (not a her\'e\'-doc) \'\'>\'\' (do not redirect) < (not an infile) >> (not an outfile)\"\n")
input("echo \' $\'\'PW\'D\' << (not \'a\' here-doc) > (do not redirect) \" \"<\"\" (not an infile) >> (not an outfile)\'\n")

print(f"{BLUE}Implement redirection{NC}\n")
input("cat < Makefile -e > out\n"
      "cat out\n"
      "cat < Makefile -e >> out\n"
      "cat out\n"
      "rm out\n"
      "cat < Makefile -e >> out\n"
      "cat out\n"
      "rm out\n")
input("cat < Makefile -e >> out\n"
      "cat out\n"
      "rm out\n")
input("cat << fake < Makefile << \"just a limiter\"\n"
      "42\n"
      "fake\n"
      "42\n"
      "\"just a limiter\"\n"
      "\'just a limiter\'\n"
      "just a limiter\n")
input("echo 42 > tester-norights.txt\n"
        "chmod 000 tester-norights.txt\n"
        "cat < Makefile > tester-norights.txt >> tester-norights.txt \n"
        "cat < tester-norights.txt << heredoc-limiter > /dev/null\n"
        "\"<inside the heredoc>\"\n"
        "heredoc-limiter\n")
input("rm -f tester-norights.txt\n")
input("echo 42 > tester-norights.txt\n"
        "chmod 000 tester-norights.txt\n"
        "cat < Makefile > tester-norights.txt >> tester-norights.txt \n"
        "rm -f tester-norights.txt\n")

print(f"{BLUE}Implement pipes{NC}\n")
input("printf \"42\\n\" | cat | cat | cat\n")
input("printf \"42\\n\" | cat | cat | cat\n")
input("printf \"42\\n\" | cat | printf \"4 8 15 16 23 42\\n\" | cat\n")
input("printf \"42\\n\" | cat | printf \"4 8 15 16 23 42\\n\" | cat > /dev/null\n")
input(env_path + " | grep \'$HOME\'\n")
input(env_path + " | grep \"$\"HOM\"E\"\n")

print(f"{BLUE}Handle environment variables and $?{NC}\n")
input("unset NONEXISTINGVARIABLE\n"
        "echo $NONEXISTINGVARIABLE\n")
input("unset NONEXISTINGVARIABLE\n"
        "echo $NONEXISTINGVARIABLE$NONEXISTINGVARIABLE\n")
input("echo $LS_COLORS$LS_COLORS\n")
input("echo $\"\"LS_COLORS\n")
input("echo $\" \"LS_COLORS\n")
input("echo $ LS_COLORS\n")
input("echo $ LS_COLORS\n")
input("echo \'$\' LS_COLORS\n")
input("echo \'$\'LS_COLORS\n")
input("$SHELL\n"
        "echo -e \"42\\n\"\n"
        "exit\n")
input("$SHELL\n"
        "echo -e \"42\\n\"\n"
        "exit\n"
        "echo $?\n")
input("echo $\"\"SHELL\n")

print(f"{BLUE}Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash{NC}\n")
print(f"{ORANGEB}  → Must be check manually.{NC}\n")

print(f"{BLUE}Your shell must implement the following builtins{NC}\n")
input("echo $HOME\n")
input("echo -n $HOME\n")
input("echo \"-\'n\" $HOME\n")
input("echo \'-\"n\' $HOME\n")
input("echo \'-nnnnn\' $HOME\n")
input("echo -nnnnn $HOME\n")
input("echo -mnnnn $HOME\n")
input("echo -nnnnm $HOME\n")
input("echo -nnnmnn $HOME\n")
input("cd to-infinity-and-beyond_donotexist\n")
input("echo $PWD, $OLDPWD\n"
        "cd to-infinity-and-beyond_donotexist\n"
        "echo $PWD, $OLDPWD\n")
input("cd ./minishell\n")
input("echo $PWD, $OLDPWD\n"
        "cd ./minishell\n"
        "echo $PWD, $OLDPWD\n")
input("cd /tmp\n")
input("echo $PWD, $OLDPWD\n"
        "cd /tmp\n"
        "echo $PWD, $OLDPWD\n")
input("cd /root\n")
input("echo $PWD, $OLDPWD\n"
        "cd /root\n"
        "echo $PWD, $OLDPWD\n")
input("cd /tmp /tmp\n")
input("cd /tmp /root\n")
input("cd /root /tmp\n")
input("pwd\n")
input("export PWD=$HOME\n"
        "pwd\n")
input("pwd 4815162342\n")
input("pwd Quarante deux\n")


if (g_stdout == g_nbr):
    print(f"\033[1;37mSTDOUT:    {GREENB}{g_stdout}/{g_nbr}:  OK!{NC}")
else:
    print(f"\033[1;37mSTDOUT:    {REDB}{g_stdout}/{g_nbr}:  KO!{NC}")

if (g_stderrKO > 0):
    print(f"\033[1;37mSTDERR:    {REDB}{g_stderr}/{g_nbr}:  KO!{NC}")
elif (g_stderr == g_nbr):
    print(f"\033[1;37mSTDERR:    {GREENB}{g_stderr}/{g_nbr}:  OK!{NC}")
else:
    print(f"\033[1;37mSTDERR:    {ORANGEB}{g_stderr}/{g_nbr}:  OK?{NC}")

if (g_exitcode == g_nbr):
    print(f"\033[1;37mEXIT CODE: {GREENB}{g_exitcode}/{g_nbr}:  OK!{NC}")
else:
    print(f"\033[1;37mEXIT CODE: {REDB}{g_exitcode}/{g_nbr}:  KO!{NC}")

# echo '$''PW'D' << (not 'a' here-doc) > (do not redirect) ""<"" (not an infile) >> (not an outfile)'
delete_files()

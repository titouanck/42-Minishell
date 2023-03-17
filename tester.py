import pty
import unicodedata
import os
import subprocess
import random
import shutil
import re
import sys

minishell_stdout = "/tmp/minishell_stdout.txt"
minishell_stderr = "/tmp/minishell_stderr.txt"
bash_stdout = "/tmp/bash_stdout.txt"
bash_stderr = "/tmp/bash_stderr.txt"
minishell_leaks = "/tmp/minishell_leaks.txt"

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

with open("/tmp/tester-ignore_readline_leaks.supp", 'w') as fd_minishell_leaks:
    fd_minishell_leaks.write(
        "{\n"
        "    tester-ignore_readline_leaks\n"
        "    Memcheck:Leak\n"
        "    ...\n"
        "    fun:readline\n"
        "}\n"
        "{\n"
        "    tester-ignore_readline_leaks\n"
        "    Memcheck:Leak\n"
        "    ...\n"
        "    fun:add_history\n"
        "}\n")

def argv_usage():
    print(f"tester.py: usage: python3 tester.py [-valgrind] [rules: 1 .. 7 .. 8]")
    exit(1)

rule = 0
check_rules = []
check_valgrind = 0

if (len(sys.argv) > 1):
   for arg in sys.argv[1:]:
        if (arg.isdigit()):
            if (int(arg) not in check_rules):
                check_rules.append(int(arg))
            else:
                argv_usage()
        elif ((arg == "-valgrind" or arg == "valgrind") and check_valgrind == 0):
            check_valgrind = 1
        else:
            argv_usage()
# else:

    
subprocess.run(["make"])
print("")

print(f"{WHITEB}Tip: {WHITE}You can choose to check a particular part :{NC}")
print(f"python3 tester.py 11\n")
print(f"{WHITEB}Tip: {WHITE}You can choose to check for memory leaks :{NC}")
print(f"python3 tester.py -valgrind\n")
print(f"{WHITEB}Tip: {WHITE}Or even do both :{NC}")
print(f"python3 tester.py -valgrind 11\n")

def delete_files():
    if os.path.exists(minishell_stdout):
        os.remove(minishell_stdout)
    if os.path.exists(bash_stdout):
        os.remove(bash_stdout)
    if os.path.exists(minishell_stderr):
        os.remove(minishell_stderr)
    if os.path.exists(bash_stderr):
        os.remove(bash_stderr)
    if os.path.exists(minishell_leaks):
        os.remove(minishell_leaks)

g_nbr = 0
g_stdout = 0
g_stderr = 0
g_exitcode = 0
g_stderrKO = 0
g_leaks = 0

minishell_readed_motd = None

def ignore_motd():
    global minishell_readed_motd

    minishell_master_out, minishell_slave_out = pty.openpty()
    with open(minishell_stdout, "w") as fd_minishell_stdout:
        with open(minishell_stderr, "w") as fd_minishell_stderr:
            minishell_process = subprocess.Popen(
                ['./minishell'], stdin=minishell_slave_out, stdout=fd_minishell_stdout, stderr=fd_minishell_stderr)
    os.write(minishell_master_out, "exit\n".encode())
    minishell_process.wait()
    os.close(minishell_slave_out)
    with open(minishell_stdout, 'r') as file:
        minishell_readed_motd = file.read()


def input(instruction):
    global g_nbr
    global g_stdout
    global g_stderr
    global g_stderrKO
    global g_exitcode
    global g_leaks
    global check_valgrind

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

    if (check_valgrind == 1):
        leaks_master_out, leaks_slave_out = pty.openpty()
        with open("/dev/null", "w") as fd_leaks_stdout:
            with open(minishell_leaks, "w") as fd_leaks_stderr:
                leaks_process = subprocess.Popen(
                    # ['valgrind', '-q', '--suppressions=/tmp/tester-ignore_readline_leaks.supp', '--leak-check=full', '--show-leak-kinds=all', './minishell'], stdin=leaks_slave_out, stdout=fd_leaks_stdout, stderr=fd_leaks_stderr)
                    ['valgrind', '--suppressions=/tmp/tester-ignore_readline_leaks.supp', './minishell'], stdin=leaks_slave_out, stdout=fd_leaks_stdout, stderr=fd_leaks_stderr)
        # --leak-check=full --show-leak-kinds=all
        os.write(leaks_master_out, instruction.encode())
        os.write(leaks_master_out, "exit\n".encode())
        leaks_process.wait()
        os.close(leaks_master_out)

    # instruction
    print(f"{PURPLE}{instruction}{NC}", end="")

    # CHECK STDOUT
    with open(minishell_stdout, 'r') as file:
        minishell_readed_stdout = file.read()

    if minishell_readed_motd is not None:
        minishell_readed_stdout = minishell_readed_stdout.replace(
            minishell_readed_motd, "")

    with open(bash_stdout, 'r') as file:
        bash_readed_stdout = file.read()

    if (minishell_readed_stdout != bash_readed_stdout):
        print(f"{REDB}  → stdout KO!{NC}")
        print(f"{WHITEB}MINISHELL{NC}")
        if (minishell_readed_stdout == ""):
            print("(null)")
        print(f"{RED}{minishell_readed_stdout}{NC}", end="")
        print(f"{WHITEB}EXPECTED{NC}")
        if (bash_readed_stdout == ""):
            print("(null)")
        print(bash_readed_stdout, end="")
        print("")
    else:
        g_stdout += 1
        print(f"{GREEN}  → stdout OK!{NC}")

    # CHECK STDERR
    with open(minishell_stderr, 'r') as file:
        minishell_readed_stderr = file.read()

    with open(bash_stderr, 'r') as file:
        bash_readed_stderr = file.read()

    if minishell_readed_stderr.endswith("exit\n"):
        minishell_readed_stderr = minishell_readed_stderr.rstrip("exit\n")
    if bash_readed_stderr.endswith("exit\n"):
        bash_readed_stderr = bash_readed_stderr.rstrip("exit\n")

    minishell_readed_stderr = re.sub(
        r'minishell: line \d+:', 'minishell:', minishell_readed_stderr)
    bash_readed_stderr = re.sub(
        r'bash: line \d+:', 'minishell:', bash_readed_stderr)
    bash_readed_stderr = bash_readed_stderr.replace('bash:', 'minishell:')
    if (minishell_readed_stderr != bash_readed_stderr):
        if (minishell_readed_stderr == "" or bash_readed_stderr == "") and (minishell_readed_stderr != "" or bash_readed_stderr != ""):
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
        print("")
    else:
        g_stderr += 1
        print(f"{GREEN}  → stderr OK!{NC}")

    # CHECK EXIT CODE
    if (minishell_exitcode == bash_exitcode):
        g_exitcode += 1
        print(f"{GREEN}  → exit code OK : {NC}{bash_exitcode}")
    else:
        print(
            f"{REDB}  → exit code{NC} {minishell_exitcode}, {REDB}expected{NC} {bash_exitcode}")

    # CHECK LEAKS
    if (check_valgrind == 1):
        with open(minishell_leaks, 'r') as file:
            minishell_readed_leaks = file.read()

        # print(minishell_readed_leaks)
        
        lines = minishell_readed_leaks.split("\n")
        filtered_lines = []
        pattern = re.compile(
            "(definitely lost|indirectly lost|possibly lost|still reachable): (?!0 bytes).*")
        for line in lines:
            if pattern.search(line):
                filtered_lines.append(line)
        modified_lines = []
        for line in filtered_lines:
            i = 0
            while i < len(line) and not line[i].isalpha():
                i += 1
            modified_lines.append(line[i:])
        modified_str = "\n".join(modified_lines)
        if (modified_str == ""):
            print(f"{GREEN}  → leaks OK!{NC}")
        else:
            g_leaks += 1
            print(f"{REDB}  → leaks KO!{NC}")
            print(modified_lines)

    print("")


delete_files()

print("")
ignore_motd()

rule = +1
if (check_rules == [] or rule in check_rules):
    print(f"{BLUE}1. Display a prompt when waiting for a new command.{NC}\n")
    input("")
    input("")

rule += 1
if (check_rules == [] or rule in check_rules):
    print(f"{BLUE}2. Have a working history.{NC}\n")
    print(f"{ORANGEB}  → Must be check manually.{NC}\n")

rule += 1
if (check_rules == [] or rule in check_rules):
    print(f"{BLUE}3. Search and launch the right executable.{NC}\n")
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

rule += 1
if (check_rules == [] or rule in check_rules):
    print(f"{BLUE}4. Not use more than one global variable.{NC}\n")
    print(f"{ORANGEB}  → Must be check manually.{NC}\n")

rule += 1
if (check_rules == [] or rule in check_rules):
    print(f"{BLUE}5. Not interpret unclosed quotes or special characters not required by the subject.{NC}\n")
    print(f"{ORANGEB}  → Must be check manually.{NC}\n")

rule += 1
if (check_rules == [] or rule in check_rules):
    print(f"{BLUE}6. Handle single quote and double quote{NC}\n")
    input("echo \'$\'\'PW\'D\' << (not \'a\' here-doc) > (do not redirect) \"\"<\"\" (not an infile) >> (not an outfile)\'\n")
    input("echo \"$\"\"PW\'D\' << (not a her\'e\'-doc) \'\'>\'\' (do not redirect) < (not an infile) >> (not an outfile)\"\n")
    input("echo \"$ \"\"PW\'D\' << (not a her\'e\'-doc) \'\'>\'\' (do not redirect) < (not an infile) >> (not an outfile)\"\n")
    input("echo \' $\'\'PW\'D\' << (not \'a\' here-doc) > (do not redirect) \" \"<\"\" (not an infile) >> (not an outfile)\'\n")

rule += 1
if (check_rules == [] or rule in check_rules):
    print(f"{BLUE}7. Implement redirection{NC}\n")
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
    input("rm -f /tmp/tester-norights.txt\n"
          "echo 42 > /tmp/tester-norights.txt\n"
          "chmod 000 /tmp/tester-norights.txt\n"
          "cat > /tmp/tester-norights.txt >> /tmp/tester-norights.txt \n"
          "rm -f /tmp/tester-norights.txt\n")
    input("rm -f /tmp/tester-norights.txt\n"
          "echo 42 > /tmp/tester-norights.txt\n"
          "chmod 000 /tmp/tester-norights.txt\n"
          "cat < Makefile > /tmp/tester-norights.txt >> /tmp/tester-norights.txt \n"
          "rm -f /tmp/tester-norights.txt\n")
    input("< \"\" echo\n")
    input("> \"\" echo\n")
    input("ls >\n")
    input("ls > >\n")
    input("rm -f tmp/a tmp/b\n")
    input("ls>/tmp/a</tmp/b\n")
    input("ls</tmp/a>/tmp/b\n")
    input("< < > >>\n")
    input(">> < < <\n")
    input("> | test\n")
    input("cat < thisfiledonotexist | echo 42\n")
    input("cat < thisfiledonotexist < thisfiledonotexist | echo 42\n")
    input("echo 42 > /dev/stdout | echo 4 8 15 16 23 42\n")
    input("touch /tmp/tester-norights.txt\n"
        "cat < wefhoe > norights.txt\n"
        "rm -f /tmp/tester-norights.txt\n")
    input("touch /tmp/tester-norights.txt\n"
        "cat > norights.txt < wefhoe\n"
        "rm -f /tmp/tester-norights.txt\n")

rule += 1
if (check_rules == [] or rule in check_rules):
    print(f"{BLUE}8. Implement heredoc{NC}\n")
    input("cat << fake < Makefile << \"just a limiter\"\n"
          "42\n"
          "fake\n"
          "42\n"
          "\"just a limiter\"\n"
          "\'just a limiter\'\n"
          "just a limiter\n")
    input("rm -f /tmp/tester-norights.txt\n"
          "echo 42 > /tmp/tester-norights.txt\n"
          "chmod 000 /tmp/tester-norights.txt\n"
          "cat < /tmp/tester-norights.txt << heredoc-limiter > /dev/null\n"
          "ls -I \"<inside the heredoc>\"\n"
          "heredoc-limiter\n")
    input("rm -f /tmp/tester-norights.txt\n"
          "export TESTER=\"ls -I <inside_the_heredoc> " + ignore_files + "\"\n"
          "<< \'$TESTER\' cat\n"
          "$HOME\n"
          "ls -I <inside_the_heredoc> -I /tmp/minishell_stdout.txt -I /tmp/minishell_stderr.txt -I /tmp/bash_stdout.txt -I /tmp/bash_stderr.txt\n"
          "$TESTER\n")
    input("export TESTER=\"ls -I <inside_the_heredoc> " + ignore_files + "\"\n"
          "<< \"$TESTER\" cat\n"
          "$HOME\n"
          "ls -I <inside_the_heredoc> -I /tmp/minishell_stdout.txt -I /tmp/minishell_stderr.txt -I /tmp/bash_stdout.txt -I /tmp/bash_stderr.txt\n"
          "$TESTER\n")
    input("export TESTER=\"ls -I <inside_the_heredoc> " + ignore_files + "\"\n"
          "<< $TESTER cat\n"
          "$HOME\n"
          "ls -I <inside_the_heredoc> -I /tmp/minishell_stdout.txt -I /tmp/minishell_stderr.txt -I /tmp/bash_stdout.txt -I /tmp/bash_stderr.txt\n"
          "$TESTER\n")
    input("<< limiter\n"
          "42\n"
          "limiter\n")
    input("cat << limiter\n"
        "$HOME\n"
        "limiter\n")
    input("cat << \'limiter\'\n"
        "$HOME\n"
        "limiter\n")
    input("cat << \"limiter\"\n"
        "$HOME\n"
        "limiter\n")

rule += 1
if (check_rules == [] or rule in check_rules):
    print(f"{BLUE}9. Implement pipes{NC}\n")
    input("printf \"42\\n\" | cat | cat | cat\n")
    input("printf \"42\\n\" | cat | cat | cat\n")
    input("printf \"42\\n\" | cat | printf \"4 8 15 16 23 42\\n\" | cat\n")
    input("printf \"42\\n\" | cat | printf \"4 8 15 16 23 42\\n\" | cat > /dev/null\n")
    input(env_path + " | grep \'$HOME\'\n")
    input(env_path + " | grep \"$\"HOM\"E\"\n")
    input("| ls\n")
    input(" ls  |||  ls \n")
    input("\'\'|\'\'|\"\"|\"\"\n")

rule += 1
if (check_rules == [] or rule in check_rules):
    print(f"{BLUE}10. Handle environment variables and $?{NC}\n")
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

rule += 1
if (check_rules == [] or rule in check_rules):
    print(f"{BLUE}11. Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash{NC}\n")
    print(f"{ORANGEB}  → Must be check manually.{NC}\n")

rule += 1
if (check_rules == [] or rule in check_rules):
    print(f"{BLUE}12. Your shell must implement the following builtins{NC}\n")
    input("echo $HOME\n")
    input("echo -n $HOME\n")
    input("echo \"-\'n\" $HOME\n")
    input("echo \'-\"n\' $HOME\n")
    input("echo \'-nnnnn\' $HOME\n")
    input("echo -nnnnn $HOME\n")
    input("echo -mn $HOME\n")
    input("echo -nm $HOME\n")
    input("echo -nnmnn $HOME\n")
    input("cd to-infinity-and-beyond_donotexist\n")
    input("echo $PWD, $OLDPWD\n"
          "cd to-infinity-and-beyond_donotexist\n"
          "echo $PWD, $OLDPWD\n")
    input("cd ./minishell\n")
    input("cd \'\"\'\n")
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
    input("cd /tmp | pwd\n"
          "pwd\n")
    input("pwd | cd /tmp\n"
          "pwd\n")
    input("pwd | cd /tmp | pwd\n"
          "pwd\n")
    input("pwd\n")
    input("export PWD=$HOME\n"
          "pwd\n")
    input("pwd 4815162342\n")
    input("pwd Quarante deux\n")
    input("export 4815162342=4815162342\n"
          "env | grep 4815162342\n")
    input("export LOST4815162342=4815162342\n"
          "env | grep 4815162342\n")
    input("export +LOST=4815162342\n"
          "env | grep 4815162342\n")
    input("export /LOST=4815162342\n")
    input("export School+=42\n"
          "env | grep School\n"
          "export School+=42\n"
          "export School-=42\n")
    input("export a + = 42\n")
    input("export a += 42\n")
    input("export | grep USER\n")
    input("export Nothing= \n"
          "export | grep Nothing\n"
          "env | grep Nothing\n")
    input("export +=\n")
    input("export \"\"\n")
    input("export /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin=42\n")
    input("unset /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin=42\n")
    input("unset $PATH\n"
          "ls\n")
    input("unset /PATH\n")
    input("unset =PATH\n")
    input("unset +=\n")
    input("unset \"<\"\n")
    input("unset 42PATH\n")
    input("unset PATH\n"
          "ls\n")
    input("unset\n")
    input("unset \"\"\n")
    input("exit 42\n"
          "test\n")
    input("exit \"  42  \"\n"
          "test\n")
    input("exit Ecole42\n"
          "test\n")
    input("exit 42 Ecole42\n"
          "test\n")
    input("exit 42Ecole\n"
          "test\n")
    input("exit Ecole42 -42\n"
          "test\n")
    input("exit -42\n"
          "test\n")
    input("exit 9223372036854775807\n"
          "test\n")
    input("exit -9223372036854775808\n"
          "test\n")
    input("exit 9223371036854775808\n"
          "test\n")
    input("exit -9223371036854775809\n"
          "test\n")
    input("exit 9223372036854775808\n"
          "test\n")
    input("exit -9223372036854775809\n"
          "test\n")

if (g_stdout == g_nbr):
    print(f"\033[1;37mSTDOUT:    {GREENB}{g_stdout:3d}/{g_nbr}:  OK!{NC}")
else:
    print(f"\033[1;37mSTDOUT:    {REDB}{g_stdout:3d}/{g_nbr}:  KO!{NC}")

if (g_stderrKO > 0):
    print(f"\033[1;37mSTDERR:    {REDB}{g_stderr:3d}/{g_nbr}:  KO!{NC}")
elif (g_stderr == g_nbr):
    print(f"\033[1;37mSTDERR:    {GREENB}{g_stderr:3d}/{g_nbr}:  OK!{NC}")
else:
    print(f"\033[1;37mSTDERR:    {ORANGEB}{g_stderr:3d}/{g_nbr}:  N/A{NC}")

if (g_exitcode == g_nbr):
    print(f"\033[1;37mEXIT CODE: {GREENB}{g_exitcode:3d}/{g_nbr}:  OK!{NC}")
else:
    print(f"\033[1;37mEXIT CODE: {REDB}{g_exitcode:3d}/{g_nbr}:  KO!{NC}")

dots = "   "
if (g_nbr > 99):
    dots = "..."
elif (g_nbr > 9):
    dots = ' ..'
else:
    dots = '  .'
if (check_valgrind == 0):
    print(
        f"\n\033[1;37mLEAKS:     {ORANGEB}{dots}/{g_nbr}:  Re-run with -valgrind")
elif (g_leaks == 0):
    print(f"\033[1;37mLEAKS:     {GREENB}{g_leaks:3d}/{g_nbr}:  OK!{NC}")
else:
    print(f"\033[1;37mLEAKS:     {REDB}{g_leaks:3d}/{g_nbr}:  KO!{NC}")

delete_files()

import pty
import unicodedata
import os
import subprocess
import random
import shutil
import re
import sys

def main():
    global  check_valgrind
    global  minishell_readed_motd

    delete_files()

    check_rules, check_valgrind = parse_argv(len(sys.argv), sys.argv)

    if (check_valgrind):
        ignore_readline_leaks_file(ignore_readline_leaks)

    compile_minishell()

    print_tips()

    ignore_motd()

    send_instructions(check_rules)

    print_results()

    delete_files()

# Define files that we are going to use
minishell_stdout = "/tmp/minishell_stdout.txt"
minishell_stderr = "/tmp/minishell_stderr.txt"
bash_stdout = "/tmp/bash_stdout.txt"
bash_stderr = "/tmp/bash_stderr.txt"
minishell_leaks = "/tmp/minishell_leaks.txt"
ignore_readline_leaks = "/tmp/tester-ignore_readline_leaks.supp"
ignore_files = "-I " + minishell_stdout + " -I " + minishell_stderr + " -I " + bash_stdout + " -I " + bash_stderr + " -I " + minishell_leaks + " -I " + ignore_readline_leaks

# Define cmd path
ls_path = shutil.which('ls')
grep_path = shutil.which('grep')
env_path = shutil.which('env')

# Initalize number of commands
cmd_nbr = 0
correct_stdout_nbr = 0
correct_stderr_nbr = 0
correct_exitcode_nbr = 0
wrong_stderr_nbr = 0
leaks_nbr = 0

minishell_readed_motd = None

# Define colors
NC = '\033[0m'
RED = '\033[0;31m'
BOLDRED = '\033[1;31m'
LIGHTRED = '\033[1;31m'

GREEN = '\033[0;32m'
BOLDGREEN = '\033[1;32m'
LIGHTGREEN = '\033[1;32m'

ORANGE = '\033[0;33m'
BOLDORANGE = '\033[1;33m'

BLUE = '\033[1;34m'
LIGHTBLUE = '\033[1;34m'
WHITE = '\033[0;37m'
BOLDWHITE = '\033[1;37m'

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
    if os.path.exists(ignore_readline_leaks):
        os.remove(ignore_readline_leaks)

def program_usage():
    print(f"tester.py: usage: python3 tester.py [-valgrind] [rules: 1 .. 7 .. 8]")
    exit(1)

def parse_argv(argc, argv):
    check_rules = []
    check_valgrind = 0

    if (argc > 1):
        for arg in argv[1:]:
            if (arg.isdigit()):
                if (int(arg) not in check_rules and int(arg) >= 1 and int(arg) <= 14):
                    check_rules.append(int(arg))
                else:
                    program_usage()
            elif ((arg == "-valgrind" or arg == "valgrind") and check_valgrind == 0):
                check_valgrind = 1
            else:
                program_usage()
    return check_rules, check_valgrind

def ignore_readline_leaks_file(ignore_readline_leaks):
    with open(ignore_readline_leaks, 'w') as fd_minishell_leaks:
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

def compile_minishell():
    subprocess.run(["make"])
    print("")


def print_tips():
    print(f"{BOLDWHITE}Tip: {WHITE}You can choose to check a particular part :{NC}")
    print(f"python3 tester.py 11\n")
    print(f"{BOLDWHITE}Tip: {WHITE}You can choose to check for memory leaks :{NC}")
    print(f"python3 tester.py -valgrind\n")
    print(f"{BOLDWHITE}Tip: {WHITE}Or even do both :{NC}")
    print(f"python3 tester.py -valgrind 11")

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

def run_process(instruction):
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
                    ['valgrind', '-q', '--track-fds=yes', '--suppressions=' + ignore_readline_leaks, '--leak-check=full', '--show-leak-kinds=all', './minishell'], stdin=leaks_slave_out, stdout=fd_leaks_stdout, stderr=fd_leaks_stderr)
        os.write(leaks_master_out, instruction.encode())
        os.write(leaks_master_out, "exit\n".encode())
        leaks_process.wait()
        os.close(leaks_master_out)

    return minishell_stdout, minishell_stderr, minishell_exitcode, bash_stdout, bash_stderr, bash_exitcode, minishell_leaks

def read_outputs(minishell_stdout, minishell_stderr, bash_stdout, bash_stderr, minishell_leaks):
    global check_valgrind

    with open(minishell_stdout, 'r') as file:
        minishell_readed_stdout = file.read()
        if minishell_readed_motd is not None:
            minishell_readed_stdout = minishell_readed_stdout.replace(minishell_readed_motd, "")

    with open(bash_stdout, 'r') as file:
        bash_readed_stdout = file.read()

    with open(minishell_stderr, 'r') as file:
        minishell_readed_stderr = file.read()
        minishell_readed_stderr = re.sub(r'minishell: line \d+:', 'minishell:', minishell_readed_stderr)
        if minishell_readed_stderr.endswith("exit\n"):
            minishell_readed_stderr = minishell_readed_stderr.rstrip("exit\n")

    with open(bash_stderr, 'r') as file:
        bash_readed_stderr = file.read()
        bash_readed_stderr = re.sub(r'bash: line \d+:', 'minishell:', bash_readed_stderr)
        bash_readed_stderr = bash_readed_stderr.replace('bash:', 'minishell:')
        if bash_readed_stderr.endswith("exit\n"):
            bash_readed_stderr = bash_readed_stderr.rstrip("exit\n")

    minishell_readed_leaks = ""
    if (check_valgrind == 1):
        with open(minishell_leaks, 'r') as file:
            minishell_readed_leaks = file.read()
            if (minishell_readed_leaks != ""):
                lines = minishell_readed_leaks.split('\n')
                new_lines = []
                for line in lines:
                    if line.startswith("=="):
                        index = line.find("==", 2)
                        new_lines.append(line[index+2:])
                minishell_readed_leaks = "\n".join(new_lines)

    return minishell_readed_stdout, minishell_readed_stderr, bash_readed_stdout, bash_readed_stderr, minishell_readed_leaks

def print_cmd(instruction, minishell_readed_stdout, minishell_readed_stderr, minishell_exitcode, bash_readed_stdout, bash_readed_stderr, bash_exitcode, minishell_readed_leaks):
    lignes = instruction.split("\n")
    instruction = "\n     ".join(lignes[:-1]) + "\n" + lignes[-1]
    status = compare_outputs(minishell_readed_stdout, minishell_readed_stderr, minishell_exitcode, bash_readed_stdout, bash_readed_stderr, bash_exitcode, minishell_readed_leaks)
    if (status == "OK"):
        print(f"{BOLDGREEN}[OK]{NC} {instruction}", end=NC)
    else:
        print(f"{BOLDRED}[KO]{BOLDWHITE} {instruction}{NC}")
    return (status)

def compare_outputs(minishell_readed_stdout, minishell_readed_stderr, minishell_exitcode, bash_readed_stdout, bash_readed_stderr, bash_exitcode, minishell_readed_leaks):
    if (minishell_readed_stdout != bash_readed_stdout):
        return ("KO")
    elif (minishell_readed_stderr != bash_readed_stderr):
        return ("KO")
    elif (minishell_readed_leaks != ""):
        return ("KO")
    elif (minishell_exitcode != bash_exitcode):
        return ("KO")
    return ("OK")

def print_stdout(minishell_readed_stdout, bash_readed_stdout):
    global correct_stdout_nbr
   
    if (minishell_readed_stdout != bash_readed_stdout):        
        if (minishell_readed_stdout == ""):
            minishell_readed_stdout = "(null)\n"
        
        print(f"{BOLDWHITE}stdout{NC}")
        if not (minishell_readed_stdout).endswith('\n'):
            print(f"{ORANGE}{minishell_readed_stdout}{NC}$")
        else:
            print(f"{ORANGE}{minishell_readed_stdout}{NC}", end="")
        
        print(f"{BOLDWHITE}expected{NC}")
        if (bash_readed_stdout == ""):
            print("(null)")
        elif not (bash_readed_stdout).endswith('\n'):
            print(bash_readed_stdout + "$")
        else:
            print(bash_readed_stdout, end="")
    
        print("")
    else:
        correct_stdout_nbr += 1

def print_stderr(minishell_readed_stderr, bash_readed_stderr):
    global correct_stderr_nbr
    global wrong_stderr_nbr

    if (minishell_readed_stderr != bash_readed_stderr):        
        if (minishell_readed_stderr == ""):
            minishell_readed_stderr = "(null)\n"
            wrong_stderr_nbr += 1
        elif (bash_readed_stderr == ""):
            wrong_stderr_nbr += 1
        
        print(f"{BOLDWHITE}stderr{NC}")

        if not (minishell_readed_stderr).endswith('\n'):
            print(f"{ORANGE}{minishell_readed_stderr}{NC}$")
        else:
            print(f"{ORANGE}{minishell_readed_stderr}{NC}", end="")
    
        print(f"{BOLDWHITE}expected{NC}")
        if (bash_readed_stderr == ""):
            print("(null)")
        print(bash_readed_stderr, end="")
        print("")
    else:
        correct_stderr_nbr += 1

def print_exitcode(minishell_exitcode, bash_exitcode):
    global correct_exitcode_nbr
   
    if (minishell_exitcode == bash_exitcode):
        correct_exitcode_nbr += 1
    else:
        print(f"{BOLDWHITE}exit code {ORANGE}{minishell_exitcode}{BOLDWHITE} : expected  {NC}{bash_exitcode} {NC}\n")

def print_leaks(minishell_readed_leaks):
    global leaks_nbr
    
    if (minishell_readed_leaks != ""):
        print(f"{BOLDWHITE}leaks{NC}")
        print(minishell_readed_leaks)
        leaks_nbr += 1

def input(instruction):
    global cmd_nbr
    global check_valgrind

    cmd_nbr += 1
    minishell_stdout, minishell_stderr, minishell_exitcode, \
    bash_stdout, bash_stderr, bash_exitcode, \
    minishell_leaks = run_process(instruction)

    minishell_readed_stdout, minishell_readed_stderr, \
    bash_readed_stdout, bash_readed_stderr, \
    minishell_readed_leaks = read_outputs(minishell_stdout, minishell_stderr, bash_stdout, bash_stderr, minishell_leaks)

    status = print_cmd(instruction, minishell_readed_stdout, minishell_readed_stderr, minishell_exitcode, bash_readed_stdout, bash_readed_stderr, bash_exitcode, minishell_readed_leaks)

    print_stdout(minishell_readed_stdout, bash_readed_stdout)

    print_stderr(minishell_readed_stderr, bash_readed_stderr)
    
    print_exitcode(minishell_exitcode, bash_exitcode)

    if (check_valgrind):
        print_leaks(minishell_readed_leaks)

    if (status == "OK"):
        print("")

def send_instructions(check_rules):
    rule = 0
    rule = +1
    if (check_rules == [] or rule in check_rules):
        print(f"\n{BLUE}{rule}. Display a prompt when waiting for a new command.{NC}\n")
        input("\n")
        input("\n")

    rule += 1
    if (check_rules == [] or rule in check_rules):
        print(f"\n{BLUE}{rule}. Have a working history.{NC}\n")
        print(f"{BOLDORANGE}  → Must be check manually.{NC}\n")

    rule += 1
    if (check_rules == [] or rule in check_rules):
        print(f"\n{BLUE}{rule}. Search and launch the right executable.{NC}\n")
        input("\"\"\n")
        input("\'\'\n")
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
        input("./\n")
        input("../\n")
        input("..\n")

    rule += 1
    if (check_rules == [] or rule in check_rules):
        print(f"\n{BLUE}{rule}. Not use more than one global variable.{NC}\n")
        print(f"{BOLDORANGE}  → Must be check manually.{NC}\n")

    rule += 1
    if (check_rules == [] or rule in check_rules):
        print(f"\n{BLUE}{rule}. Not interpret unclosed quotes or special characters not required by the subject.{NC}\n")
        print(f"{BOLDORANGE}  → Must be check manually.{NC}\n")

    rule += 1
    if (check_rules == [] or rule in check_rules):
        print(f"\n{BLUE}{rule}. Handle single quote and double quote{NC}\n")
        input("echo \'$\'\'PW\'D\' << (not \'a\' here-doc) > (do not redirect) \"\"<\"\" (not an infile) >> (not an outfile)\'\n")
        input("echo \"$\"\"PW\'D\' << (not a her\'e\'-doc) \'\'>\'\' (do not redirect) < (not an infile) >> (not an outfile)\"\n")
        input("echo \"$ \"\"PW\'D\' << (not a her\'e\'-doc) \'\'>\'\' (do not redirect) < (not an infile) >> (not an outfile)\"\n")
        input("echo \' $\'\'PW\'D\' << (not \'a\' here-doc) > (do not redirect) \" \"<\"\" (not an infile) >> (not an outfile)\'\n")

    rule += 1
    if (check_rules == [] or rule in check_rules):
        print(f"\n{BLUE}{rule}. Implement redirection{NC}\n")
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
            "cat < wefhoe > /tmp/tester-norights.txt\n"
            "rm -f /tmp/tester-norights.txt\n")
        input("touch /tmp/tester-norights.txt\n"
            "cat > /tmp/tester-norights.txt < wefhoe\n"
            "rm -f /tmp/tester-norights.txt\n")

    rule += 1
    if (check_rules == [] or rule in check_rules):
        print(f"\n{BLUE}{rule}. Implement heredoc{NC}\n")
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
        print(f"\n{BLUE}{rule}. Implement pipes{NC}\n")
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
        print(f"\n{BLUE}{rule}. Handle environment variables and $?{NC}\n")
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
        print(f"\n{BLUE}{rule}. Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash{NC}\n")
        print(f"{BOLDORANGE}  → Must be check manually.{NC}\n")

    rule += 1
    if (check_rules == [] or rule in check_rules):
        print(f"\n{BLUE}{rule}. Your shell must implement echo, cd and PWD :{NC}\n")
        input("echo $HOME\n")
        input("echo -n -n -n Hmmm..\n")
        input("echo -n $HOME\n")
        input("echo \"-\'n\" $HOME\n")
        input("echo \'-\"n\' $HOME\n")
        input("echo \'-nnnnn\' $HOME\n")
        input("echo -nnnnn $HOME\n")
        input("echo -mn $HOME\n")
        input("echo -nm $HOME\n")
        input("echo -nnmnn $HOME\n")
        input("echo \" \" \" \"\"\" \'\'\n")
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

    rule += 1
    if (check_rules == [] or rule in check_rules):
        print(f"\n{BLUE}{rule}. Your shell must implement export, unset and env :{NC}\n")
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

    rule += 1
    if (check_rules == [] or rule in check_rules):
        print(f"\n{BLUE}{rule}. Your shell must implement exit:{NC}\n")
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

def print_results():
    if (correct_stdout_nbr == cmd_nbr):
        print(f"\033[1;37mstdout:    {BOLDGREEN}{correct_stdout_nbr:3d}/{cmd_nbr}:  OK!{NC}")
    else:
        print(f"\033[1;37mstdout:    {BOLDRED}{correct_stdout_nbr:3d}/{cmd_nbr}:  KO!{NC}")

    if (wrong_stderr_nbr > 0):
        print(f"\033[1;37mstderr:    {BOLDRED}{correct_stderr_nbr:3d}/{cmd_nbr}:  KO!{NC}")
    elif (correct_stderr_nbr == cmd_nbr):
        print(f"\033[1;37mstderr:    {BOLDGREEN}{correct_stderr_nbr:3d}/{cmd_nbr}:  OK!{NC}")
    else:
        print(f"\033[1;37mstderr:    {BOLDORANGE}{correct_stderr_nbr:3d}/{cmd_nbr}:  N/A{NC}")

    if (correct_exitcode_nbr == cmd_nbr):
        print(f"\033[1;37mEXIT CODE: {BOLDGREEN}{correct_exitcode_nbr:3d}/{cmd_nbr}:  OK!{NC}")
    else:
        print(f"\033[1;37mEXIT CODE: {BOLDRED}{correct_exitcode_nbr:3d}/{cmd_nbr}:  KO!{NC}")

    dots = "   "
    if (cmd_nbr > 99):
        dots = "..."
    elif (cmd_nbr > 9):
        dots = ' ..'
    else:
        dots = '  .'
    if (check_valgrind == 0):
        print(
            f"\n\033[1;37mLEAKS:     {BOLDORANGE}{dots}/{cmd_nbr}:  Re-run with -valgrind")
    elif (leaks_nbr == 0):
        print(f"\033[1;37mLEAKS:     {BOLDGREEN}{leaks_nbr:3d}/{cmd_nbr}:  OK!{NC}")
    else:
        print(f"\033[1;37mLEAKS:     {BOLDRED}{leaks_nbr:3d}/{cmd_nbr}:  KO!{NC}")

main()
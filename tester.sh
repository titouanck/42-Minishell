NC='\033[0m'
RED='\033[0;31m'
GREEN='\033[0;32m'
ORANGE='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
LIGHTGRAY='\033[0;37m'

rm tester/minishell*
rm tester/bash*
python3 tester/tester.py "minishell"
python3 tester/tester.py "bash --posix"
make log
echo
echo ""$RED"Difference between stdout :"$NC
diff -u tester/bash_stdout tester/minishell_stdout

# echo "\n"$ORANGE"Difference between stderr :"$NC
# diff tester/bash_stderr tester/minishell_stderr

echo "\n"$RED"Difference between return values :"$NC
diff -u tester/bash_returnval tester/minishell_returnval
echo 
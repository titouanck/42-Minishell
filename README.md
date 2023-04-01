![image](https://user-images.githubusercontent.com/87268044/229261427-37e09b4a-6282-4f65-868b-3ddb00f1922c.png)

### Build and launch the project
```bash
make && ./minishell
```
### Makefile rules
![image](https://user-images.githubusercontent.com/87268044/229261904-7fdd946d-75c0-4b18-9db8-0cd6e76d0feb.png)

### Reference Shell
```js
GNU bash, version 5.1.16(1)-release (x86_64-pc-linux-gnu)
bash --posix
```
### Troubleshoot dependencies
```bash
sudo apt-get install libreadline-dev
```
### Other details

- Local variables have been implemented

- To ensure efficient memory management, dynamically allocated memory is manually freed despite the presence of a garbage collector that only frees all memory at once in case of an unexpected program exit.

- You can execute a command directly by using the "-c" option
```bash
./minishell -c [cmd]
```

- In order to closely mimic the behavior of bash, the signal handling in Minishell changes when launched with a redirect.
```C
if (!isatty(STDIN_FILENO) || !isatty(STDERR_FILENO))
  // Different signal behavior
  // See : srcs/signals
```

---

| Relevant tests |
|-------------|
| **Search and launch the right executable (based on the PATH variable or using a relative or an absolute path)** |
| `ls` | 
| `.ls` | 
| `/ls` | 
| `ls -la` | 
| `/tmp` | 
| `/usr/bin/ls` | 
| `cp /usr/bin/ls ./lsCopy` → `lsCopy` →  `./lsCopy ` | 
| **Handle single quote and double quote** |
| `echo '$PWD'` | 
| `echo "$PWD"` | 
| `echo '$''PW'D''` | 
| `echo $PW''D''` | 
| `echo ' << (not 'a' here-doc) > (nothing) ""<"" (nothing) >> (nothing)'` | 
| **Implement redirection** |
| `cat < Makefile > out.txt` → `cat out.txt` | 
| `cat < Makefile > out.txt` → `cat < Makefile >> out.txt` → `cat out.txt` | 
| `cat < thisFileDoesNotExist > out.txt` | 
| `touch noRights.txt` → `chmod 000 noRights.txt` → `cat < noRights.txt` | 
| `ls > $thisVariableDoesNotExist` | 
| `< < > >>` | 
| `>> < < <` | 
| `<<` | 
| `>>` | 
| `>` | 
| `<` | 
| **Implement heredoc** |
| `< Makefile << limiter` | 
| `cat << limiter < Makefile` | 
| `cat << limiter1 << limiter2` | 
| `echo << limiter` | 
| `cat << limiter `  → `$PATH` *(inside the heredoc)*| 
| `cat << "limiter"` → `$PATH` *(inside the heredoc)* | 
| `cat << 'limiter'` → `$PATH` *(inside the heredoc)* | 
| **Implement pipes** |
| `cat \| cat \| ls` | 
| `\| ls \| ls` | 
| `ls \| ls \|` | 
| `ls \|\|\| ls ` |
| `sleep 2 \| ls ` | 
| `cat /dev/urandom \| head -n 5 ` | 
| `printf "42\n" \| cat \| printf "4 8 15 16 23 42\n" \| cat` | 
| **Handle environment variables and $?** |
| `ls -wError` → `echo $?` | 
| `thisCommandDoesNotExist` → `echo $?` | 
| `echo $PATH` → `echo $?` | 
| `$SHELL` | 
| **Your shell must implement some builtins** |
| → https://github.com/thallard/minishell_tester | 

*On this project, my teammate and I agreed that we would each make a Minishell. Here is my version.*

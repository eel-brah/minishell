import subprocess
import os
import sys

# if len(sys.argv) != 2:
#     sys.exit(f"Usage: python3 {sys.argv[0]} minishell_prompt")

tests = {
	"normal test 1": "echo $?",
	"normal test 2": "touch f1 f2 f3",
	"normal test 3": "ls -l",
	"normal test 4": "./sfsffff",

	"expand 1": r"echo \"$HOME\"",
	"expand 2": r"echo \"$HOME\"$\"$HOME\"",
	"expand 3": r"echo \"$HOME\"\"$\"\"$HOME\"",
	"expand 4": r"echo \"$HOME\"\"$\"\"$HOME\"$",
	"expand 5": r"echo $\"$HOME\"\"$\"\"$HOME\"$",
	"expand 6": r"echo $\"$HOME\"",
	"expand 7": r"echo $\"HOME\"",
	"expand 8": r'echo $HOME$USER$HOME',
	"expand 9": r'echo \"$HOME$USER$HOME\"',
	"expand 10": r'echo $HOME$sdfsf$USER$HOME',
	"expand 11": r"echo '$HOME'$sdfsf$USER$HOME",
	"expand 12": r"echo $'HOME'\"$sdfsf\"$USER$HOME",
	"expand 13": r"echo $HOME======",
	"expand 14": r"echo \"$\"HOME======",
	"expand 15": r"echo $USER'\"\"'",
	"expand 16": r"echo $USER'\"\"',,$USER'$USER'_____$USER==____",
	"expand 17": r"echo $USER''$USER\"$USER===\"'\"'",
	"expand 18": r"echo $USER''$USER\"$USER===\"'\"'",

	"wildcard": "echo *",
	"wildcard 2": 'echo ****',
	"wildcard 3": 'echo \"h\"****',
	"wildcard 4": 'echo \"M\"**\"A\"**',
	"wildcard 5": 'echo M**A**',
	"wildcard 6": 'echo *Makefile*',
	"wildcard 7": 'echo *\"Makefile\"',
	"wildcard 8": 'echo \"Makefile\"*',
	"wildcard 9": 'echo *\"Makefile\"*',
	"wildcard 10": 'echo *****\"Makefile\"',
	"wildcard 12": 'echo * * * *    ',
	"wildcard 13": '$sdfsdfsdf/bin/ls',
	"wildcard 14": 'echo $.sdfsdfsf',
	"wildcard 15": r"echo $\"\"$",
	"wildcard 16": r'echo $\"$\"$',
	"wildcard 17": r'echo $sdfsdf=*$dl=',
	"wildcard 18": r'echo hello >> $sdfdsf',
	"wildcard 19": r'echo hello >> =$sdfdsf*',
	"wildcard 20": r"echo $USER''l''",
	"wildcard 21": r"echo $USER''$l'$'$",
	"wildcard 22": r'echo *$Makefile.c',
	"wildcard 23": r'echo Make$sdfsdf*',
	"wildcard 24": r"echo $USER''$l'$'$",
	"wildcard 25": r'echo \"$USER======\"',
	"wildcard 26": r'echo \"$======\"',
	"wildcard 29": r'echo \"$USER''$l\"',
	"wildcard 30": r'echo \"$=====\"',
	"wildcard 31": r' $USER\'\'$l',
	"wildcard 32": r'echo $USER\'\'$USER',
	"wildcard 32": r"echo *hello'*\"'",
	"wildcard 33": r"touch 'hel'*'' && echo h*'*'",
	"wildcard 34": "echo *$sdf.c",

	"env with wildcards 1": r"export a=\"helloworld\" && echo $a*",
	"env with wildcards 2": r"export a=\"helloworld\" && echo $a*****",
	"env with wildcards 3": r"export a=\"helloworld\" && echo $a==*****",
	"env with wildcards 4": r"export a='hello*world' &&echo $a***** && rm '$a'",
	"env with wildcards 5": r"touch helloworld && export a=\"hello\" && echo $a* && rm '$a'",

	"export 0": "export a=a && export | grep \"a=\"",
	"export 1": "export a+=a && export | grep \"a=\"",
	"export 2": "export a=a && export a+=a && export | grep \"a=\"",
	"export 3": "export a++=a && export | grep \"a=\"",
	"export 4": "export a++=a && ls",
	"export 5": "export aaa && export | grep \"aaa\"",
	"export 6": "export a && export a+=b && echo $a",
	"export 7": "export a++=a",
	"export 8": "export =sdfsf",
	"export 9": "export =sdfsf ++ ++++ hello",
	"export 10": "export bbbcd && export | grep bbbcd$",
	"export 10": "export bbbcd && export bbbcd= && export | grep bbbcd=",
	"export 11": "export =sdfsf ++ ++++ hello || export | grep hello",
	"export 12": "export aaax=9 aaax+=8 aaax+=0 && export | grep aaax=",
	"export 13": "export 9k=a",

	"unset": r"unset HOME && echo $HOME",
	"expand and and": r"export a=s && $''l''\"\"''\"\"''$a",
	"and 1": 'sdfsdfdsf && ls -al',
	"and 2": 'echo hello && sdfsdf && echo hello',
	"or 1": 'echo hello || sdfsdf || echo hello',
	"or 2": 'sdfsdf || sdfsdf || echo hello',
	"or 3": 'sdfsdf || sdfsdf || echo hello',
	"or 4": 'sdfsdf || echo hello || echo hello',
	"cd 1": 'cd /tmp && ls',
	"cd 2": 'mkdir testdir && cd testdir && ls -al && cd .. && rm -r testdir && ls -al',
	"cd 3": 'cd sdfsf',
	"cd 4": 'cd /exam',

	"redirection 1": 'echo hello > $sdfsdf',
	"redirection 2": 'echo hello >> $sdfsdf',
	"redirection 3": 'echo hello < $sdfsdf',
	"redirection 4": 'ls -l && echo hello > $sdfsdf',
	"redirection 5": '(echo hello && ls) > $sdfsdf',
	"redirection 6": '(echo hello && ls) < $sdfsdf',
	"redirection 7": 'ls && (echo hello && ls) > $sdfsdf',
	"redirection 8": 'echo hello > $sdfsdf',
	"redirection 9": 'export sdfsdf=\"hello world\" && echo hello >> $sdfsdf',
	"redirection 10": 'export sdfsdf=\"hello world\" && echo hello < $sdfsdf',
	"redirection 11": 'export sdfsdf=\"hello world\" && ls && echo hello > $sdfsdf',
	"redirection 12": 'export sdfsdf=\"hello world\" && (echo hello && ls) > $sdfsdf',
	"redirection 13": 'export sdfsdf=\"hello world\" && ls && (echo hello && ls) > $sdfsdf',
	"redirection 14": 'export sdfsdf=\"hello world\" && ls && (echo hello && ls) >> $sdfsdf',
	"redirection 15": 'export sdfsdf=\"hello world\" && ls && (echo hello && ls) < $sdfsdf',
	"redirection 16": 'cd /tmp && touch file1 file2 && echo hello > file* && ls -al && rm -fr file1 file2',
	"redirection 17": 'cd /tmp && touch file1 file2 && (echo hello) > file* && ls -al && rm -fr file1 file2',
	"redirection 18": "cd /tmp && mkdir testdir && cd testdir && echo hello >> \"$USER\" && ls -al && cd .. && rm -fr testdir",

	"redirections 19": 'cd /tmp && echo hello > a > b > c > d && cat d && ls && rm -fr a b c d',
	"redirections 20": 'cd /tmp && echo hello >> a >> b >> c >> d && cat d && ls && rm -fr a b c d',
	"redirections 21": 'cd /tmp && (ls -a && echo hello) >> a >> b && ls -al && cat b && cat a && rm -fr a b',
	"redirections 22": 'cd /tmp && echo hello > c &&echo >> a >> b > c hello world && ls -al && cat a b c && rm -fr a b c',
	"redirections 23": 'cd /tmp && echo hello >     a    >b> c    >     d && cat d && ls && rm -fr a b c d',
	"redirections 24": 'cd /tmp && echo hello >> a     >> b     >> c>>    d&& cat d && ls && rm -fr a b c d',
	"redirections 25": 'cd /tmp &&     (  ls -a && echo hello)>>     a>> b && ls -a && cat b && cat a && rm -fr a b',
	"redirections 26": 'cd /tmp && echo hello > c &&echo >>a    >> b  >    c hello world && ls -al && cat a b c && rm -fr a b c',
	"redirection 27": 'export a=\"hello\" && echo hello > sdfsdf $a && cat sdfsdf && rm sdfsdf',
	"redirection 28": 'export a=\"hello\" && echo hello > sdfsdf > $a && cat $a && rm $a',
	"ambiguous with builtins": 'cd /tmp && touch file1 file2 && echo hello >> file* && rm -fr file1 file2',
	"ambiguous with builtins 2": 'cd /tmp && touch file1 file2 && export >> file* && rm -fr file1 file2',
	"ambiguous with builtins 3": 'cd /tmp && touch file1 file2 && echo hello > file* && rm -fr file1 file2',
	"ambiguous with builtins 4": 'cd /tmp && touch file1 file2 && export > file* && rm -fr file1 file2',
	"ambiguous with builtins 5": 'cd /tmp && export a=\"hello world\" && echo hello >> $a',
	"ambiguous with builtins 6": 'cd /tmp && export a=\"hello world\" && export >> $a',
	"ambiguous with builtins 7": 'cd /tmp && export a=\"hello world\" && echo hello > $a',
	"ambiguous with builtins 8": 'cd /tmp && export a=\"hello world\" && export > $a',
	"ambiguous with builtins 9": 'cd /tmp && touch file1 file2 && echo hello >> file* && rm -fr file1 file2',
	"ambiguous with builtins 10": 'cd /tmp && touch file1 file2 && export >> file* && rm -fr file1 file2',
	"ambiguous with builtins 11": 'cd /tmp && touch file1 file2 && (echo hello) > file* && rm -fr file1 file2',
	"ambiguous with builtins 12": 'cd /tmp && touch file1 file2 && (export) > file* && rm -fr file1 file2',
	"ambiguous with builtins 13": 'cd /tmp && export a=\"hello world\" && (echo hello) >> $a',
	"ambiguous with builtins 14": 'cd /tmp && export a=\"hello world\" && (export) >> $a',
	"ambiguous with builtins 15": 'cd /tmp && export a=\"hello world\" && (echo hello) > $a',
	"ambiguous with builtins 16": 'cd /tmp && export a=\"hello world\" && (export) > $a',
	"ambiguous with builtins 17": 'cd /tmp && export a=\"hello world\" && (export && ls) > $a',

	"export": 'export +++ && echo $?',
	"cd status code": 'cd /exam && echo $?',
	"cd status code 2": 'cd /aaaaaaaa && echo $?',
	"syntax error 1": '()',
	"syntax error 2": ' << ()',
	"syntax error 3": 'echo hello >      ',
	"empty command": '           ',
	"just .": '.',
	"just ..": '..',
	"permisions 1": 'cd /tmp && touch hello && chmod 000 hello && echo hello > hello',
	"permisions 2": 'cd /tmp && touch hello && chmod 000 hello && ls hello >> hello',
	"files with spaces": 'cd /tmp && ls -al >     \"hello    world\"   && ls -al && cat \"hello    world\" && rm -fr \"hello    world\"',
	"quotes": "'sdfas 'sfsf\"fsdf\" yuu",
	"test 0": 'cd \"\" && ls',
	"test 1": "$=5",
	"test 2": "$+",
	"test 3": "$-",
	"test 4": "pwd dkfj kdjf kdjf",
	"test 5": "echo $USER'\"$USER'",
	"test 6": "echo ''$USER",
	"test 7": "((ls && ls | ls) > a ) > t && ls",
	"test 8": "((ls < k < g | nl < mm|| echo a < nn) > ll >> o && (cat Makefile) <k >o || echo ooooo) < h < l > q",
	"test 9": "(((ls d k f d s >> l && ls) | ls  > ls < l  && (cat | ls || l)) || (ls a idn sndu >> kkd) >> l )))) a > ls < l  >> l < d",
	"test 10": "unset HOME && cd || pwd",
	"test 11": "export ll*ll=99",
	"test 12": "export 66=99 || echo $?",
	"test 13": "export 8kkk=99 || echo $?",
	"test 14": "export XX=99 KK=00 99xx=99 LLL=8989 || export | grep LLL=",
	"test 15": "echo \"$\"ls\"\"",
	"test 16": "echo $\"ls\"",
	"test 17": "cd /tmp && echo hello > test1 test2 && cat test1",
	"test 18": "echo hello > *z",
	"test 19": "export z=*z && export | grep z=",
	"test 20": "export z=*z && echo hello > $z",
	"test 21": "cd /tmp \&\& touch hell\"**\"\* \&\& echo hell\"**\"\*",
	"test 22": "cd /tmp \&\& touch hell\"**\"\* hell\"**\"o hello \&\& echo hell\*",
	"test 23": "cd /tmp && > file && ls",
	"test 24": "cd /tmp && echo hello > test1 test2 && cat test1",
	"test 25": "echo hello > *z",
	"test 26": "export z=*z && export | grep z=",
	"test 27": "export z=*z && echo hello > $z",
	## "test 28": "export abcdef="*'*"*"'" && export | grep abcdef",
	## "test 29": "export abcdef="*'*"*'*'"'" && export | grep abcdef",
	"test 28": "mkdir /tmp/test00 && echo '#!/bin/bash' > /tmp/test00/t5555 && echo ls >> /tmp/test00/t5555 && export PATH+=\":/tmp/test00\" && t5555",
	"test 29": "mkdir /tmp/test00 && echo '#!/bin/bash' > /tmp/test00/t5555 && echo ls >> /tmp/test00/t5555 && chmod 766 /tmp/test00/t5555 && export PATH+=\":/tmp/test00\" && t5555",
	"test 30": "mkdir /tmp/test22 && touch /tmp/test22/t5555 && export PATH+=\":/tmp/test22\" && mkdir /tmp/test00 && echo '#!/bin/bash' > /tmp/test00/t5555 && echo ls >> /tmp/test00/t5555 && chmod 766 /tmp/test/t5555 && export PATH+=\":/tmp/test\" && t5555",
	"test 31": "< l > p && ls",
	"test 32": "exit",
	"test 33": "exit 88k 88",
	"test 34": "exit 88 88",
	"test 35": "exit h",
	"test 36": "exit 44",
	"test 37": "exit 98909889687665765765786",
	"test 38": "exit \"   88  \"",
	"test 39": "fskdjfjlk || exit",

}

# echo -n AAA BBB CCC DDD EEE FFFFFFFFF
# echo -nnnnnn -nnn AAA BBB CCC 

def run_tests(tests):
	skip = ["test 21", "test 22"]
	value = ["test 36", "test 38"]
	subprocess.call(['mkdir', '/tmp/tmp'])
	subprocess.call(['cp', 'minishell', '/tmp/tmp/minishell'])
	os.chdir("/tmp/tmp")
	for key in tests:
		command = tests[key] if key in skip else replace_sc(tests[key])
		expected_stdout, expected_stderr, expected_rv = get_bash_return(command)
		rm_tests(key)
		got_stdout, got_stderr, got_rv = get_minishell_return(command)
		rm_tests(key)
		if key in value and got_rv == expected_rv:
			got_rv = 0
			expected_rv = 0
		if got_stdout == expected_stdout and got_rv == expected_rv and (got_rv == 0 or (got_stderr and expected_stderr)): #and (not got_err) == (not expected_err):
			print(f"\033[0;32m{key} sucesssssss\033[0m")
		else:
			print(f"\033[0;31m{key} failed\n===>Expeced return -- value: {expected_rv}\n{expected_stdout}\n===>Got return -- value: {got_rv}\n{got_stdout}\033[0m")
	subprocess.call(['rm', '-fr', '/tmp/tmp'])

def get_bash_return(command):
	expected = subprocess.run(f"echo {command} | bash",
		shell=True, capture_output=True, text=True)
	# print(f"[{expected.stdout}]")
	return expected.stdout, expected.stderr, 127 if command == "." else expected.returncode

def get_minishell_return(command):
	got = subprocess.run(f"echo {command} | ./minishell",
		shell=True, capture_output=True, text=True)
	# print(f"[{got.stdout}]")
	return remove_fl_lines(got.stdout), got.stderr, got.returncode

def remove_fl_lines(text):
	lines = text.split('\n')
	lines = lines[1:-1]
	if not lines:
		return '\n'.join(lines)
	return '\n'.join(lines)+'\n'

def replace_sc(s):
	replacements = {
		"$": "\$",
		"&": "\&",
		"|": "\|",
		"'": "\\'",
		"*": "\*",
		">": "\>",
		"<": "\<",
		"(": "\(",
		")": "\)",
	}
	for char, replacement in replacements.items():
		s = s.replace(char, replacement)
	return s

def rm_tests(key):
	rmv = ["test 28", "test 29", "test 30"]
	if key in rmv:
		subprocess.run("rm -fr /tmp/test00 /tmp/test22", shell=True, capture_output=True)

if __name__ == '__main__':
	run_tests(tests)
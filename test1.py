


import subprocess
import os

simple_tests = {
        "normal test": "ls -l",
        # "unset": "unset HOME && echo $HOME",
        "expand 1": "echo \"$HOME\"",
        "expand 2": "echo \"$HOME\"$\"$HOME\"",
        "expand 3": "echo \"$HOME\"\"$\"\"$HOME\"",
        "expand 4": "echo \"$HOME\"\"$\"\"$HOME\"$",
        "expand 5": "echo $\"$HOME\"\"$\"\"$HOME\"$",
        "expand 6": "echo $\"$HOME\"",
        "expand 7": "echo $\"HOME\"",
        "expand 8": 'echo $HOME$USER$HOME',
        "expand 9": 'echo "$HOME$USER$HOME"',
        "expand 10": 'echo $HOME$sdfsf$USER$HOME',
        "expand 12": 'echo $HOME======',
        "wildcard": "echo *",
        "wildcard 2": "echo ****",
        "wildcard 3": "echo \"h\"****",
        "wildcard 4": "echo \"M\"**\"A\"**",
        "wildcard 5": "echo M**A**",
        "wildcard 6": "echo *Makefile*",
        "wildcard 7": 'echo *"Makefile"',
        "wildcard 8": 'echo "Makefile"*',
        "wildcard 9": 'echo *"Makefile"*',
        "wildcard 10": 'echo *****"Makefile"',
        "wildcard 12": 'echo * * * *    ',
        "env arg with wildcards": 'echo *$sdf.c',
        #"env arg with wildcards 2": 'export a="helloworld" && echo $a*',
        #"env arg with wildcards 3": 'export a="helloworld" && echo $a*****',
        #"env arg with wildcards 4": 'export a="helloworld" && echo $a==*****',
        #"env arg with wildcards 5": 'export a="helloworld" && touch '$a' &&echo $a***** && rm -fr '$a'',
        # "export": 'export a=a && export | grep "a="',
        # "export 2": 'export a+=a && export | grep "a="',
        # "export 3": 'export a++=a && export | grep "a="',
        # "export 4": 'export a++=a && ls',
        # "export 5": 'export aaa && export | grep "aaa"',
        # "export syntax error": 'export a++=a',
        # "export syntax error 2": 'export =sdfsf',
        # "export syntax error 3": 'export =sdfsf ++ ++++ hello',
        # "export syntax error 4": 'export 38sdfsf ++ hello',
        # "export syntax error 5": 'export =sdfsf ++ ++++ hello || export | grep hello',
        # "export syntax error 6": 'export 38sdfsf ++ hello || export | grep hello',
        # "export syntax error 7": 'export =sdfsf ++ ++++ hello && export | grep hello',
        # "export syntax error 8": 'export 38sdfsf ++ hello && export | grep hello',
        "and": 'sdfsdfdsf && ls -al',
        "and 2": 'echo hello && sdfsdf && echo hello',
        "or": 'echo hello || sdfsdf || echo hello',
        "or 2": 'sdfsdf || sdfsdf || echo hello',
        "or 3": 'sdfsdf || sdfsdf || echo hello',
        "or 4": 'sdfsdf || echo hello || echo hello',
        "cd": 'cd /tmp && ls',
        "cd 2": 'mkdir testdir && cd testdir && ls -al && cd .. && rm -r testdir',
        "cd 3": 'cd /sdfsf',
        "cd 4": 'cd /exam',
        "ambiguous redirection": 'echo hello > $sdfsdf',
        "ambiguous redirection 2": 'echo hello >> $sdfsdf',
        "ambiguous redirection 3": 'echo hello < $sdfsdf',
        "ambiguous redirection 4": 'ls -l && echo hello > $sdfsdf',
        "ambiguous redirection 5": 'ls -l && echo hello >> $sdfsdf',
        "ambiguous redirection 6": 'ls -l && echo hello < $sdfsdf',
        "ambiguous redirection 7": '(echo hello && ls) > $sdfsdf',
        "ambiguous redirection 8": '(echo hello && ls) >> $sdfsdf',
        "ambiguous redirection 9": '(echo hello && ls) < $sdfsdf',
        "ambiguous redirection 10": 'ls && (echo hello && ls) > $sdfsdf',
        "ambiguous redirection 11": 'ls && (echo hello && ls) >> $sdfsdf',
        "ambiguous redirection 12": 'ls && (echo hello && ls) < $sdfsdf',
        "ambiguous redirection 13": 'echo hello > $sdfsdf',
        # "ambiguous redirection 14": 'export sdfsdf="hello world" && echo hello >> $sdfsdf',
        # "ambiguous redirection 15": 'export sdfsdf="hello world" && echo hello < $sdfsdf',
        # "ambiguous redirection 16": 'export sdfsdf="hello world" && ls -l && echo hello > $sdfsdf',
        # "ambiguous redirection 17": 'export sdfsdf="hello world" && ls -l && echo hello >> $sdfsdf',
        # "ambiguous redirection 18": 'export sdfsdf="hello world" && ls -l && echo hello < $sdfsdf',
        # "ambiguous redirection 19": 'export sdfsdf="hello world" && (echo hello && ls) > $sdfsdf',
        # "ambiguous redirection 20": 'export sdfsdf="hello world" && (echo hello && ls) >> $sdfsdf',
        # "ambiguous redirection 21": 'export sdfsdf="hello world" && (echo hello && ls) < $sdfsdf',
        # "ambiguous redirection 22": 'export sdfsdf="hello world" && ls && (echo hello && ls) > $sdfsdf',
        # "ambiguous redirection 23": 'export sdfsdf="hello world" && ls && (echo hello && ls) >> $sdfsdf',
        # "ambiguous redirection 24": 'export sdfsdf="hello world" && ls && (echo hello && ls) < $sdfsdf',
        "ambiguous redirection 25": 'cd /tmp && touch file1 file2 && echo hello > file* && ls -al && rm -fr file1 file2',
        "ambiguous redirection 26": 'cd /tmp && touch file1 file2 && (echo hello) > file* && ls -al && rm -fr file1 file2',
        "ambiguous redirection 27": "cd /tmp && mkdir testdir && cd testdir && echo hello >> \"$USER\" && ls -al && cd .. && rm -fr testdir",
        "ambiguous redirection 28": "cd /tmp && mkdir testdir && cd testdir && echo hello >> \"$USER\"$\"$USER\" && ls -al && cd .. && rm -fr testdir",
        "ambiguous redirection 29": "cd /tmp && mkdir testdir && cd testdir && echo hello >> \"$USER\"\"$\"\"$USER\" && ls -al && cd .. && rm -fr testdir",
        "ambiguous redirection 30": "cd /tmp && mkdir testdir && cd testdir && echo hello >> \"$USER\"\"$\"\"$USER\"$ && ls -al && cd .. && rm -fr testdir",
        "ambiguous redirection 31": "cd /tmp && mkdir testdir && cd testdir && echo hello >> $\"$USER\"\"$\"\"$USER\"$ && ls -al && cd .. && rm -fr testdir",
        "ambiguous redirection 32": "cd /tmp && mkdir testdir && cd testdir && echo hello >> $\"$USER\" && ls -al && cd .. && rm -fr testdir",
        "ambiguous redirection 33": "cd /tmp && mkdir testdir && cd testdir && echo hello >> $\"USER\" && ls -al && cd .. && rm -fr testdir",
        "ambiguous redirection 34": 'cd /tmp && mkdir testdir && cd testdir && echo hello >> $USER$USER$USER && ls -al && cd .. && rm -fr testdir',
        "ambiguous redirection 35": 'cd /tmp && mkdir testdir && cd testdir && echo hello >> "$USER$USER$USER" && ls -al && cd .. && rm -fr testdir',
        "ambiguous redirection 36": 'cd /tmp && mkdir testdir && cd testdir && echo hello >> $USER$sdfsf$USER$USER && ls -al && cd .. && rm -fr testdir',
        "ambiguous redirection 37": 'cd /tmp && mkdir testdir && cd testdir && echo hello >> $USER====== && ls -al && cd .. && rm -fr testdir',
        # "random": 'export a="hello" && echo hello > $sdfsdf $a && cat $a && rm $a',
        "redirections": 'cd /tmp && echo hello > a > b > c > d && cat d && ls -l && rm -fr a b c d',
        "redirections 2": 'cd /tmp && echo hello >> a >> b >> c >> d && cat d && ls -l && rm -fr a b c d',
        "redirections 3": 'cd /tmp && (ls -al && echo hello) >> a >> b && ls -al && cat b && cat a && rm -fr a b',
        "redirections 4": 'cd /tmp && echo hello > c &&echo >> a >> b > c hello world && ls -al && cat a b c && rm -fr a b c',
        "redirections 5": 'cd /tmp && echo hello >     a    >b> c    >     d && cat d && ls -l && rm -fr a b c d',
        "redirections 6": 'cd /tmp && echo hello >> a     >> b     >> c>>    d&& cat d && ls -l && rm -fr a b c d',
        "redirections 7": 'cd /tmp &&     (  ls -al && echo hello)>>     a>> b && ls -al && cat b && cat a && rm -fr a b',
        "redirections 8": 'cd /tmp && echo hello > c &&echo >>a    >> b  >    c hello world && ls -al && cat a b c && rm -fr a b c',
        "ambiguous with builtins": 'cd /tmp && touch file1 file2 && echo hello >> file* && rm -fr file1 file2',
        "ambiguous with builtins 2": 'cd /tmp && touch file1 file2 && export >> file* && rm -fr file1 file2',
        "ambiguous with builtins 3": 'cd /tmp && touch file1 file2 && echo hello > file* && rm -fr file1 file2',
        "ambiguous with builtins 4": 'cd /tmp && touch file1 file2 && export > file* && rm -fr file1 file2',
        # "ambiguous with builtins 5": 'cd /tmp && export a="hello world" && echo hello >> $a',
        # "ambiguous with builtins 6": 'cd /tmp && export a="hello world" && export >> $a',
        # "ambiguous with builtins 7": 'cd /tmp && export a="hello world" && echo hello > $a',
        # "ambiguous with builtins 8": 'cd /tmp && export a="hello world" && export > $a',
        "ambiguous with builtins 9": 'cd /tmp && touch file1 file2 && echo hello >> file* && rm -fr file1 file2',
        "ambiguous with builtins 10": 'cd /tmp && touch file1 file2 && export >> file* && rm -fr file1 file2',
        "ambiguous with builtins 11": 'cd /tmp && touch file1 file2 && (echo hello) > file* && rm -fr file1 file2',
        "ambiguous with builtins 12": 'cd /tmp && touch file1 file2 && (export) > file* && rm -fr file1 file2',
        # "ambiguous with builtins 13": 'cd /tmp && export a="hello world" && (echo hello) >> $a',
        # "ambiguous with builtins 14": 'cd /tmp && export a="hello world" && (export) >> $a',
        # "ambiguous with builtins 15": 'cd /tmp && export a="hello world" && (echo hello) > $a',
        # "ambiguous with builtins 16": 'cd /tmp && export a="hello world" && (export) > $a',
        # "ambiguous with builtins 17": 'cd /tmp && export a="hello world" && (export && ls) > $a',
        # "export status code": 'export +++ && echo $?',
        "cd status code": 'cd /exam && echo $?',
        "cd status code 2": 'cd /aaaaaaaa && echo $?',
        "echo": 'echo -n AAA BBB CCC DDD EEE FFFFFFFFF',
        "cd 2": 'cd "" && ls',
        "syntax error": '()',
        "syntax error 2": ' << ()',
        "syntax error 3": 'echo hello >      ',
        "empty command": '           ',
        "permisions": 'cd /tmp && touch hello && chmod 000 hello && echo hello > hello || rm -fr hello',
        "permisions 2": 'cd /tmp && touch hello && chmod 000 hello && ls hello >> hello || rm -fr hello',
        "files with spaces": 'cd /tmp && ls -al >     "hello    world"   && ls -al && cat "hello    world" && rm -fr "hello    world"',
}

sofisticated_tests = {
        "heredoc": [[b"ls -al", 0], [b"cat Makefile", 1]],
}


"""
from pwn import *
import time
def run_sofisticated_tests(tests):
    for key in tests:
        cmds = tests[key]
        with process(b"./mini") as mini:
            print(mini.recv())
            for cmd in cmds:
                mini.send(cmd[0] + b'\n')

                got = mini.recvall()
                if cmd[1] == 1:
                    print(got.decode())
"""

def run_tests(tests):
    for key in tests:
        expected = subprocess.run(['bash', '-c', f"echo -e \'{tests[key]}\' | bash"], capture_output=True)
        #expected = subprocess.run(['bash', '-c', tests[key]], capture_output=True)
        expected, expected_err = expected.stdout, expected.stderr
        got = subprocess.run(['bash', '-c', f"echo -e \'{tests[key]}\' | ./minishell"], capture_output=True)
        got, got_err = got.stdout, got.stderr
        got = got.split(b'\n')[1:-1]
        got[-1] = got[-1][:-25] # remove b'minishell2.5>$ exit' eel-brah minishell2$ exit
        got = b'\n'.join(got)

        # adding \n to match expected
        #if len(expected) >= 1 and expected[-1] == 10:
            #got += b'\n'

        if got == expected and (not got_err) == (not expected_err):
            print("yaaaay")
        else:
            print(f"{key} failed\nexpeced ---------------\n{expected}\ngot --------------\n{got}")


if __name__ == '__main__':
    run_tests(simple_tests)
    #run_sofisticated_tests(sofisticated_tests)

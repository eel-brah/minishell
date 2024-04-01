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
        "wildcard 11": 'echo *$Makefile.c',
        "wildcard 12": 'echo Make$sdfsdf*',
        "wildcard 13": '$sdfsdfsdf/bin/ls',
        "wildcard 14": 'echo $.sdfsdfsf',
        "wildcard 15": 'echo $""$',
        "wildcard 16": 'echo $\'$\'$',
        "wildcard 17": 'echo $sdfsdf=*$dl=',
        "wildcard 18": 'echo hello >> $sdfdsf',
        "wildcard 19": 'echo hello >> =$sdfdsf*',
        "wildcard 20": "echo $USER''l''",
        "wildcard 21": 'echo $USER\'\'$l\'$\'$',
        # "wildcard 22": 'echo $USER\'\'l\'\'',
        # "wildcard 23": 'echo $USER\'\'l\'\'',
        # "wildcard 24": 'echo $USER\'\'l\'\'',
        # "wildcard 25": 'echo $USER\'\'l\'\'',
        # "wildcard 26": 'echo $USER\'\'l\'\'',
        # "wildcard 27": 'echo $USER\'\'l\'\'',
        # "wildcard 28": 'echo $USER\'\'l\'\'',
        # "wildcard 29": 'echo $USER\'\'l\'\'',
        # echo $USER''$l'$'$
        # echo "$USER======"
        # echo "$======"
        # echo "$USER''$l"
        # echo "$====="
        #  $USER''$l
        # echo $USER''$USER
}

sofisticated_tests = {
        "setup": [b"ls -al", b"cat Makefile"],
        "test": ["ls Makefile"]
}

def run_sofisticated_test(tests):
    setup = tests["setup"]
    p = subprocess.Popen("./minishell", shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    bash = subprocess.Popen("bash", shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    for cmd in setup:
        p.communicate(input=cmd)
        bash.communicate(input=cmd)
    test = tests["test"]
    for cmd in test:
        expected = bash.communicate(input=cmd)[0]
        got = p.communicate(input=cmd)[0]
        print(expected)
        print(got)

def run_tests(tests):
    for key in tests:
        expected = subprocess.check_output(['bash', '-c', tests[key]])
        got = subprocess.check_output(['bash', '-c', f"echo \'{tests[key]}\' | ./minishell"])
        got = b'\n'.join(got.split(b'\n')[1:-2]) + b'\n'
        if got == expected:
            print("yaaaay")
        else:
            print(f"{key} failed\nexpeced ---------------\n{expected}\ngot --------------\n{got}")


if __name__ == '__main__':
    run_tests(simple_tests)
    #run_sofisticated_test(sofisticated_tests)
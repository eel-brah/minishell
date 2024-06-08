import subprocess
import os
import sys
from time import sleep

# if len(sys.argv) != 2:
#     sys.exit(f"Usage: python3 {sys.argv[0]} minishell_prompt")
builts = {
	"Echo and Expand 1":r"echo hello world",
	"Echo and Expand 2":r"echo \"hello world\"",
	"Echo and Expand 3":r"echo 'hello world'",
	"Echo and Expand 4":r"echo hello'world'",
	"Echo and Expand 5":r"echo hello\"\"world",
	"Echo and Expand 6":r"echo ''",
	"Echo and Expand 7":r"echo \"$PWD\"",
	"Echo and Expand 8":r"echo '$PWD'",
	"Echo and Expand 9":r"echo \"aspas ->'\"",
	"Echo and Expand 10":r"echo \"aspas -> ' \"",
	"Echo and Expand 11":r"echo 'aspas ->\"'",
	"Echo and Expand 12":r"echo 'aspas -> \" '",
	"Echo and Expand 13":r"echo \"> >> < * ? [ ] | ; [ ] || && ( ) & # $ \\ <<\"",
	"Echo and Expand 14":r"echo '> >> < * ? [ ] | ; [ ] || && ( ) & # $ \\ <<'",
	"Echo and Expand 15":r"echo \"exit_code ->$? user ->$USER home -> $HOME\"",
	"Echo and Expand 16":r"echo 'exit_code ->$? user ->$USER home -> $HOME'",
	"Echo and Expand 17":r"echo \"$\"",
	"Echo and Expand 18":r"echo '$'",
	"Echo and Expand 19":r"echo $",
	"Echo and Expand 20":r"echo $?",
	"Echo and Expand 21":r"echo $?HELLO",
	"Echo and Expand 22":r"echo $\"USER\"",
	"Echo and Expand 23":r"echo $'USER'",
	"Echo and Expand 24":r"echo $123",
	"Echo and Expand 25":r"echo $1_abc",
	"Echo and Expand 26":r"echo $+#####",
	"Echo and Expand 27":r"echo $hello@@@@@",
	"Echo and Expand 28":r"echo \"$'USER'\"",
	"Echo and Expand 29":r"echo '\"$\"USER\"\"'",
	"Echo and Expand 30": r"echo \"$HOME\"",
	"Echo and Expand 31": r"echo \"$HOME\"$\"$HOME\"",
	"Echo and Expand 32": r"echo \"$HOME\"\"$\"\"$HOME\"",
	"Echo and Expand 33": r"echo \"$HOME\"\"$\"\"$HOME\"$",
	"Echo and Expand 34": r"echo $\"$HOME\"\"$\"\"$HOME\"$",
	"Echo and Expand 35": r"echo $\"$HOME\"",
	"Echo and Expand 36": r"echo $\"HOME\"",
	"Echo and Expand 37": r'echo $HOME$USER$HOME',
	"Echo and Expand 38": r'echo \"$HOME$USER$HOME\"',
	"Echo and Expand 39": r'echo $HOME$sdfsf$USER$HOME',
	"Echo and Expand 40": r"echo '$HOME'$sdfsf$USER$HOME",
	"Echo and Expand 41": r"echo $'HOME'\"$sdfsf\"$USER$HOME",
	"Echo and Expand 42": r"echo $HOME======",
	"Echo and Expand 43": r"echo \"$\"HOME======",
	"Echo and Expand 44": r"echo $USER'\"\"'",
	"Echo and Expand 45": r"echo $USER'\"\"',,$USER'$USER'_____$USER==____",
	"Echo and Expand 46": r"echo $USER''$USER\"$USER===\"'\"'",
	"Echo and Expand 47": r"echo $USER''$USER\"$USER===\"'\"'",
	"Echo and Expand 48": r"echo \"$\"ls\"\"",
	"Echo and Expand 49": r"echo $\"ls\"",
	"Echo and Expand 50": r"echo $?",
	"Echo and Expand 51": r"$sdfsfdsf/bin/ls",
	"Echo and Expand 51": r"$sdfsfdsf/bin/ls ",

	"Echo and Expand 54": r"echo $USER''l''",
	"Echo and Expand 55": r"echo $USER''$l'$'$",
	"Echo and Expand 56": r"echo $USER''$l'$'$",
	"Echo and Expand 57": r'echo \"$USER======\"',
	"Echo and Expand 58": r'echo \"$======\"',
	"Echo and Expand 59": r'echo \"$USER''$l\"',
	"Echo and Expand 60": r'echo \"$=====\"',
	"Echo and Expand 61": r' $USER\'\'$l',
	"Echo and Expand 62": r"echo $USER''$USER",
	"pwd 1":r"pwd",
	"pwd 2":r"pwd oi",
	"pwd 2":r"pwd 12313123 1231  23 1",
	"pwd 2":r"pwd $sdfdsf '\"' 12313123 1231  23 1",

	"Export 1":r"export hello",
	"Export 2":r"export HELLO=123",
	"Export 3":r"export A-",
	"Export 4":r"export a+=",
	"Export 5":r"export a+=+",
	"Export 6":r"export a+====",
	"Export 7":r"export a+=--",
	"Export 8":r"export a++=",
	"Export 9":r"export _a+=",
	"Export 10":r"export _a+=\"\"",
	"Export 11":r"export 5asd=",
	"Export 12":r"export 5asd_+=",
	"Export 13":r"export +HELLO=123 +A 123 55",
	"Export 14":r"export HELO=\"123 A-\"",
	"Export 15":r"export hellLo world",
	"Export 16":r"export HELLO-=123",
	"Export 17":r"export =",
	"Export 18":r"export 123",
	"Export 19":r"export holola | echo $holola",
	"Export 21":r"export a++=a",
	"Export 22":r"export 9k=a",
	"Export 23":r"export =sdfsf",

	"Unset 1":r"unset",
	"Unset 2":r"unset HELLO",
	"Unset 3":r"unset HELLO1 HELLO2",
	"Unset 4":r"unset HOME",
	"Unset 5":r"unset PATH",
	"Unset 6":r"unset ++",
	"Unset 7":r"unset +=gwllo",
	"Unset 8":r"unset 3gwllo",
	"Unset 9":r"unset _gwllo",
	"Unset 10":r"unset pwd | echo $pwd",
	"Unset 11":r"unset pwd\"'\" | echo $pwd",
	"Unset 13":r"unset hello+=",
	"Unset 14":r"unset $sdfsdf",
	"Unset 15":r"unset \"$sdfsdf\"",
	"Unset 16":r"unset SHELL",

	"cd 1":r"cd $PWD",
	"cd 2":r"cd ...",
	"cd 3":r"cd .",
	"cd 4":r"cd ..",
	"cd 5":r"cd $OLDPWD",
	"cd 6":r"cd $PWD hi ",
	"cd 7":r"cd $\"\"",
	"cd 8":r"cd $sdfdsf",
	"cd 9":r"cd /testtt",
	"cd 10":r"cd /exam",

	"Env 1":r"env | grep ^HOME",
	"Env 2":r"env | grep ^HOME | cat",
	
	"Exit 1": r"exit",
	"Exit 2":r"exit 123",
	"Exit 3":r"exit 123 | echo $?",
	"Exit 4":r"exit 298",
	"Exit 5":r"exit +100",
	"Exit 6":r"exit \"+100\"",
	"Exit 7":r"exit +\"100\"",
	"Exit 8":r"exit -100",
	"Exit 9":r"exit \"-100\"",
	"Exit 10":r"exit -\"100\"",
	"Exit 11":r"exit hello",
	"Exit 12":r"exit 42 world",
	"Exit 13":r"exit 42  | echo hello",
	"Exit 14":r"exit 42  | exit 1",
	"Exit 15":r"exit \"      42            \"",
	"Exit 16":r"exit 88k 88",
	"Exit 17":r"exit 88 88",
	"Exit 19":r"exit -9223372036854775808",
	"Exit 20":r"exit -9223372036854775809",
	"Exit 21":r"exit +9223372036854775807",
	"Exit 22":r"exit +9223372036854775808",
	"Exit 23":r"exit 989098896876657657657865",
	"Exit 24":r"exit 989098896876657657657865564564654654646546",

}

syntax = {
	"empty command": r'           ',
	"just .":r'.',
	"just ..": r'..',
	"Syntax 0": r'echo hello >      ',
    "Syntax 1": r"| echo oi",
    "Syntax 2": r"|",
    "Syntax 3": r"| |",
    "Syntax 4": r"| $",
    "Syntax 5": r"> > >> >>",
    "Syntax 6": r"< <<",
    "Syntax 7": r"cat    <| ls",
    "Syntax 8": r"echo hi <",
    "Syntax 9": r"> >>",
    "Syntax 10": r"echo hi | < |",
    "Syntax 11": r"echo hi |   |",
    "Syntax 12": r"echo hi |  |",
    "Syntax 13": r"echo hi |",
    "Syntax 14": r"> |",
    "Syntax 15": r"| >>",
    "Syntax 16": r"<<",
    "Syntax 17": r">",
}

tests_red = {
    "Redirection 1": r"grep hi <./test_files/infile",
    "Redirection 2": r'grep hi \"<infile\" <         ./test_files/infile',
    "Redirection 3": r'echo hi < ./test_files/infile bye bye',
    "Redirection 4": r'grep hi <./test_files/infile_big <./test_files/infile',
    "Redirection 5": r'echo <\"./test_files/infile\" \"bonjour       42\"',
    "Redirection 6": r'cat <\"./test_files/file name with spaces\"',
    "Redirection 7": r'cat <./test_files/infile_big ./test_files/infile',
    "Redirection 8": r'cat <\"1\"\"2\"\"3\"\"4\"\"5\"',
    "Redirection 9": r'echo <\"./test_files/infile\" <missing <\"./test_files/infile\"',
    "Redirection 10": r'echo <missing <\"./test_files/infile\" <missing',
    "Redirection 11": r'cat <\"./test_files/infile\"',
    "Redirection 12": r'echo <\"./test_files/infile_big\" | cat <\"./test_files/infile\"',
    "Redirection 13": r'echo <\"./test_files/infile_big\" | cat \"./test_files/infile\"',
    "Redirection 14": r'echo <\"./test_files/infile_big\" | echo <\"./test_files/infile\"',
    "Redirection 15": r'echo hi | cat <\"./test_files/infile\"',
    "Redirection 16": r'echo hi | cat \"./test_files/infile\"',
    "Redirection 17": r'cat <\"./test_files/infile" | echo hi',
    "Redirection 18": r'cat <\"./test_files/infile\" | grep hello',
    "Redirection 19": r'cat <\"./test_files/infile_big\" | echo hi',
    "Redirection 20": r'cat <missing',
    "Redirection 21": r'cat <missing | cat',
    "Redirection 22": r'cat <missing | echo oi',
    "Redirection 23": r'cat <missing | cat <\"./test_files/infile\"',
    "Redirection 24": r'echo <123 <456 hi | echo 42',
    "Redirection 25": r'ls >./outfiles/outfile01',
    "Redirection 26": r'ls >         ./outfiles/outfile01',
    "Redirection 27": r'echo hi >         ./outfiles/outfile01 bye',
    "Redirection 28": r'ls >./outfiles/outfile01 >./outfiles/outfile02',
    "Redirection 29": r'ls >./outfiles/outfile01 >./test_files/invalid_permission',
    "Redirection 30": r'ls >\"./outfiles/outfile with spaces\"',
    "Redirection 31": r'ls >\"./outfiles/outfile\"\"1\"\"2\"\"3\"\"4\"\"5\"',
    "Redirection 32": r'ls >\"./outfiles/outfile01\" >./test_files/invalid_permission >\"./outfiles/outfile02\"',
    "Redirection 33": r'ls >./test_files/invalid_permission >\"./outfiles/outfile01\" >./test_files/invalid_permission',
    "Redirection 34": r'cat <\"./test_files/infile\" >\"./outfiles/outfile01\"',
    "Redirection 35": r'echo hi >./outfiles/outfile01 | echo bye',
    "Redirection 36": r'echo hi >./outfiles/outfile01 >./outfiles/outfile02 | echo bye',
    "Redirection 37": r'echo hi | echo >./outfiles/outfile01 bye',
    "Redirection 38": r'echo hi | echo bye >./outfiles/outfile01 >./outfiles/outfile02',
    "Redirection 39": r'echo hi >./outfiles/outfile01 | echo bye >./outfiles/outfile02',
    "Redirection 40": r'echo hi >./outfiles/outfile01 >./test_files/invalid_permission | echo bye',
    "Redirection 41": r'echo hi >./test_files/invalid_permission | echo bye',
    "Redirection 42": r'echo hi >./test_files/invalid_permission >./outfiles/outfile01 | echo bye',
    "Redirection 43": r'echo hi | echo bye >./test_files/invalid_permission',
    "Redirection 44": r'echo hi | >./outfiles/outfile01 echo bye >./test_files/invalid_permission',
    "Redirection 45": r'echo hi | echo bye >./test_files/invalid_permission >./outfiles/outfile01',
    "Redirection 46": r'cat <\"./test_files/infile\" >./test_files/invalid_permission',
    "Redirection 47": r'cat >./test_files/invalid_permission <\"./test_files/infile\"',
    "Redirection 48": r'cat <missing >./outfiles/outfile01',
    "Redirection 49": r'cat >./outfiles/outfile01 <missing',
    "Redirection 50": r'cat <missing >./test_files/invalid_permission',
    "Redirection 51": r'cat >./test_files/invalid_permission <missing',
    "Redirection 52": r'cat >./outfiles/outfile01 <missing >./test_files/invalid_permission',
    "Redirection 53": r'ls >>./outfiles/outfile01',
    "Redirection 54": r'ls >>      ./outfiles/outfile01',
    "Redirection 55": r'ls >>./outfiles/outfile01 >./outfiles/outfile01',
    "Redirection 56": r'ls >./outfiles/outfile01 >>./outfiles/outfile01',
    "Redirection 57": r'ls >./outfiles/outfile01 >>./outfiles/outfile01 >./outfiles/outfile02',
    "Redirection 58": r'ls >>./outfiles/outfile01 >>./outfiles/outfile02',
    "Redirection 59": r'ls >>./test_files/invalid_permission',
    "Redirection 60": r'ls >>./test_files/invalid_permission >>./outfiles/outfile01',
    "Redirection 61": r'ls >>./outfiles/outfile01 >>./test_files/invalid_permission',
    "Redirection 62": r'ls >./outfiles/outfile01 >>./test_files/invalid_permission >>./outfiles/outfile02',
    "Redirection 63": r'ls <missing >>./test_files/invalid_permission >>./outfiles/outfile02',
    "Redirection 64": r'ls >>./test_files/invalid_permission >>./outfiles/outfile02 <missing',
    "Redirection 65": r'echo hi >>./outfiles/outfile01 | echo bye',
    "Redirection 66": r'echo hi >>./outfiles/outfile01 >>./outfiles/outfile02 | echo bye',
    "Redirection 67": r'echo hi | echo >>./outfiles/outfile01 bye',
    "Redirection 68": r'echo hi | echo bye >>./outfiles/outfile01 >>./outfiles/outfile02',
    "Redirection 69": r'echo hi >>./outfiles/outfile01 | echo bye >>./outfiles/outfile02',
    "Redirection 70": r'echo hi >>./test_files/invalid_permission | echo bye',
    "Redirection 71": r'echo hi >>./test_files/invalid_permission >./outfiles/outfile01 | echo bye',
    "Redirection 72": r'echo hi | echo bye >>./test_files/invalid_permission',
    "Redirection 73": r'echo hi | echo >>./outfiles/outfile01 bye >./test_files/invalid_permission',
    "Redirection 74": r'cat <minishell.h>./outfiles/outfile',
    "Redirection 75": r"cat <minishell.h|ls",
	"Redirection 76": r'echo hello > $sdfsdf',
	"Redirection 77": r'echo hello >> $sdfsdf',
	"Redirection 78": r'echo hello < $sdfsdf',
	"Redirection 83": r'echo hello > $sdfsdf',
	"Redirection 84": r'< a > b',
	"Redirection 85": r'pwd |  ls > /dev/stdin',
	"Redirection 86": r'ls > /dev/stdin | cat test_files/infile_big | head -n5',
	"Redirection 87": r'echo hlelo |  ls > /dev/stdin',
	"Redirection 88": r'echo hello >> =$sdfdsf*',
}
wildcard = {
	"wildcard 1": "echo *",
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
	"wildcard 13": '$USER*',
	"wildcard 13": '$lllll*****',
	"wildcard 14": 'echo $.sdfsdfsf',
	"wildcard 15": r"echo $\"\"$",
	"wildcard 16": r'echo $\"$\"$',
	"wildcard 17": r'echo $sdfsdf=*$dl=',
	# "wildcard 18": r'echo hello >> $sdfdsf',
	# "wildcard 19": r'echo hello >> =$sdfdsf*',
	"wildcard 22": r'echo *$Makefile.c',
	"wildcard 23": r'echo Make$sdfsdf*',
	"wildcard 32": r"echo *hello'*\"'",
	# "wildcard 33": r"touch 'hel'*'' && echo h*'*'",
	"wildcard 34": "echo *$sdf.c",
}
andorwild = {
	"and or wildcard 1":r"export a=\"helloworld\" && echo $a*",
	"and or wildcard 2":r"export a=\"helloworld\" && echo $a*****",
	"and or wildcard 3":r"export a=\"helloworld\" && echo $a==*****",
	"and or wildcard 4":r"export a='hello*world' &&echo $a***** && rm '$a'",
	"and or wildcard 5":r"touch helloworld && export a=\"hello\" && echo $a* && rm '$a'",
	"and or wildcard 6":r'cd /tmp && touch file1 file2 && echo hello >> file* && rm -fr file1 file2',
	"and or wildcard 7":r'cd /tmp && touch file1 file2 && export >> file* && rm -fr file1 file2',
	"and or wildcard 8":r'cd /tmp && touch file1 file2 && echo hello > file* && rm -fr file1 file2',
	"and or wildcard 9":r'cd /tmp && touch file1 file2 && export > file* && rm -fr file1 file2',
	"and or wildcard 10":r'cd /tmp && touch file1 file2 && echo hello >> file* && rm -fr file1 file2',
	"and or wildcard 11":r'cd /tmp && touch file1 file2 && export >> file* && rm -fr file1 file2',
	"and or wildcard 12":r'cd /tmp && touch file1 file2 && (echo hello) > file* && rm -fr file1 file2',
	"and or wildcard 13":r'cd /tmp && touch file1 file2 && (export) > file* && rm -fr file1 file2',
	"and or wildcard 14":r"cd /tmp && touch hell\"**\"* && echo hell\"**\"*",
	"and or wildcard 15":r"cd /tmp && touch hell\"**\"* hell\"**\"o hello && echo hell*",
}
andor = {
	"andor test 1":r"export a=a && export | grep \"a=\"",
	"andor test 2":r"export a+=a && export | grep \"a=\"",
	"andor test 3":r"export a=a && export a+=a && export | grep \"a=\"",
	"andor test 4":r"export a++=a && export | grep \"a=\"",
	"andor test 5":r"export a++=a && ls",
	"andor test 6":r"export aaa && export | grep \"aaa\"",
	"andor test 7":r"export a && export a+=b && echo $a",
	"andor test 8":r"export =sdfsf ++ ++++ hello",
	"andor test 9":r"export bbbcd && export | grep bbbcd$",
	"andor test 10":r"export bbbcd && export bbbcd= && export | grep bbbcd=",
	"andor test 11":r"export =sdfsf ++ ++++ hello || export | grep hello",
	"andor test 12":r"export aaax=9 aaax+=8 aaax+=0 && export | grep aaax=",
	"andor test 13":r"unset HOME && echo $HOME",
	"andor test 14":r"export a=s && $''l''\"\"''\"\"''$a",
	"andor test 15":r'sdfsdfdsf && ls -al',
	"andor test 16":r'echo hello && sdfsdf && echo hello',
	"andor test 17":r'echo hello && echo hello',
	"andor test 18":r'echo hello || sdfsdf || echo hello',
	"andor test 19":r'export sdfsdf=\"hello world\" && (echo hello && ls) > $sdfsdf',
	"andor test 20":r'sdfsdf || sdfsdf || echo hello',
	"andor test 21":r'sdfsdf || echo hello || echo hello',
	"andor test 22":r'cd /tmp && ls',
	"andor test 23":r'mkdir testdir && cd testdir && ls -al && cd .. && rm -r testdir && ls -al',
	"andor test 24":r'ls -l && echo hello > $sdfsdf',
	"andor test 25":r'(echo hello && ls) > $sdfsdf',
	"andor test 26":r'(echo hello && ls) < $sdfsdf',
	"andor test 27":r'ls && (echo hello && ls) > $sdfsdf',
	"andor test 28":r'export sdfsdf=\"hello world\" && echo hello >> $sdfsdf',
	"andor test 29":r'export sdfsdf=\"hello world\" && echo hello < $sdfsdf',
	"andor test 30":r'export sdfsdf=\"hello world\" && ls && echo hello > $sdfsdf',
	"andor test 31":r'export sdfsdf=\"hello world\" && ls && (echo hello && ls) > $sdfsdf',
	#"andor test 32":r'export sdfsdf=\"hello world\" && ls && (echo hello && ls) >> $sdfsdf',
	"andor test 33":r'export sdfsdf=\"hello world\" && ls && (echo hello && ls) < $sdfsdf',
	"andor test 34":r'cd /tmp && touch file1 file2 && echo hello > file* && ls -al && rm -fr file1 file2',
	"andor test 35":r'cd /tmp && touch file1 file2 && (echo hello) > file* && ls -al && rm -fr file1 file2',
	"andor test 36":r"cd /tmp && mkdir testdir && cd testdir && echo hello >> \"$USER\" && ls -al && cd .. && rm -fr testdir",
	"andor test 37":r'cd /tmp && echo hello > a > b > c > d && cat d && ls && rm -fr a b c d',
	"andor test 38":r'cd /tmp && echo hello >> a >> b >> c >> d && cat d && ls && rm -fr a b c d',
	"andor test 39":r'cd /tmp && (ls -a && echo hello) >> a >> b && ls -al && cat b && cat a && rm -fr a b',
	"andor test 40":r'cd /tmp && echo hello > c &&echo >> a >> b > c hello world && ls -al && cat a b c && rm -fr a b c',
	"andor test 41":r'cd /tmp && echo hello >     a    >b> c    >     d && cat d && ls && rm -fr a b c d',
	"andor test 42":r'cd /tmp && echo hello >> a     >> b     >> c>>    d&& cat d && ls && rm -fr a b c d',
	"andor test 43":r'cd /tmp &&     (  ls -a && echo hello)>>     a>> b && ls -a && cat b && cat a && rm -fr a b',
	"andor test 44":r'cd /tmp && echo hello > c &&echo >>a    >> b  >    c hello world && ls -al && cat a b c && rm -fr a b c',
	"andor test 45":r'export a=\"hello\" && echo hello > sdfsdf $a && cat sdfsdf && rm sdfsdf',
	"andor test 46":r'export a=\"hello\" && echo hello > sdfsdf > $a && cat $a && rm $a',
	"andor test 47":r'cd /tmp && touch file1 file2 && echo hello >> file* && rm -fr file1 file2',
	"andor test 48":r'cd /tmp && touch file1 file2 && export >> file* && rm -fr file1 file2',
	"andor test 49":r'cd /tmp && touch file1 file2 && echo hello > file* && rm -fr file1 file2',
	"andor test 50":r'cd /tmp && touch file1 file2 && export > file* && rm -fr file1 file2',
	"andor test 51":r'cd /tmp && export a=\"hello world\" && echo hello >> $a',
	"andor test 52":r'cd /tmp && export a=\"hello world\" && export >> $a',
	"andor test 53":r'cd /tmp && export a=\"hello world\" && echo hello > $a',
	"andor test 54":r'cd /tmp && export a=\"hello world\" && export > $a',
	"andor test 55":r'cd /tmp && touch file1 file2 && echo hello >> file* && rm -fr file1 file2',
	"andor test 56":r'cd /tmp && touch file1 file2 && export >> file* && rm -fr file1 file2',
	"andor test 57":r'cd /tmp && touch file1 file2 && (echo hello) > file* && rm -fr file1 file2',
	"andor test 57":r'cd /tmp && touch file1 file2 && (export) > file* && rm -fr file1 file2',
	"andor test 58":r'cd /tmp && export a=\"hello world\" && (echo hello) >> $a',
	"andor test 59":r'cd /tmp && export a=\"hello world\" && (export) >> $a',
	"andor test 60":r'cd /tmp && export a=\"hello world\" && (echo hello) > $a',
	"andor test 61":r'cd /tmp && export a=\"hello world\" && (export) > $a',
	"andor test 62":r'cd /tmp && export a=\"hello world\" && (export && ls) > $a',
	"andor test 63":r'export +++ && echo $?',
	"andor test 64":r'cd /exam && echo $?',
	"andor test 65":r'cd /aaaaaaaa && echo $?',
	"andor test 66":r'()',
	"andor test 67":r' << ()',
	"andor test 68":r'cd /tmp && touch hello && chmod 000 hello && echo hello > hello',
	"andor test 69":r'cd /tmp && touch hello && chmod 000 hello && ls hello >> hello',
	"andor test 70":r'cd /tmp && ls -al >     \"hello    world\"   && ls -al && cat \"hello    world\" && rm -fr \"hello    world\"',
	"andor test 71":r"'sdfas 'sfsf\"fsdf\" yuu",
	"andor test 72":r'cd \"\" && ls',
	"andor test 73":r"$=5",
	"andor test 74":r"$+",
	"andor test 75":r"$-",
	"andor test 76":r"pwd dkfj kdjf kdjf",
	"andor test 77":r"echo $USER'\"$USER'",
	"andor test 78":r"echo ''$USER",
	"andor test 79":r"((ls && ls | ls) > a ) > t && ls",
	"andor test 80":r"((ls < k < g | nl < mm|| echo a < nn) > ll >> o && (cat Makefile) <k >o || echo ooooo) < h < l > q",
	"andor test 81":r"(((ls d k f d s >> l && ls) | ls  > ls < l  && (cat | ls || l)) || (ls a idn sndu >> kkd) >> l )))) a > ls < l  >> l < d",
	"andor test 82":r"unset HOME && cd || pwd",
	"andor test 83":r"export 66=99 || echo $?",
	"andor test 84":r"export 8kkk=99 || echo $?",
	"andor test 85":r"export XX=99 KK=00 99xx=99 LLL=8989 || export | grep LLL=",
	"andor test 86":r"cd /tmp && echo hello > test1 test2 && cat test1",
	"andor test 87":r"echo hello > *z",
	"andor test 88":r"export z=*z && export | grep z=",
	"andor test 89":r"export z=*z && echo hello > $z",
	"andor test 90":r"cd /tmp && > file && ls",
	"andor test 91":r"cd /tmp && echo hello > test1 test2 && cat test1",
	"andor test 92":r"echo hello > *z",
	"andor test 93":r"export z=*z && export | grep z=",
	"andor test 94":r"export z=*z && echo hello > $z",
	## "test 28": "export abcdef="*'*"*"'" && export | grep abcdef",
	## "test 29": "export abcdef="*'*"*'*'"'" && export | grep abcdef",
	"andor test 95":r"mkdir /tmp/test00 && echo '#!/bin/bash' > /tmp/test00/t5555 && echo ls >> /tmp/test00/t5555 && export PATH+=\":/tmp/test00\" && t5555",
	"andor test 96":"mkdir /tmp/test00 && echo '#!/bin/bash' > /tmp/test00/t5555 && echo ls >> /tmp/test00/t5555 && chmod 766 /tmp/test00/t5555 && export PATH+=\":/tmp/test00\" && t5555",
	"andor test 97":r"mkdir /tmp/test22 && touch /tmp/test22/t5555 && export PATH+=\":/tmp/test22\" && mkdir /tmp/test00 && echo '#!/bin/bash' > /tmp/test00/t5555 && echo ls >> /tmp/test00/t5555 && chmod 766 /tmp/test/t5555 && export PATH+=\":/tmp/test\" && t5555",
	"andor test 98":r"< l > p && ls",
	"andor test 99":r"&&",
	"andor test 100":r"&& ||",
	"andor test 101":r"< && >",
	"andor test 102":r"()",
	"andor test 103":r"(ls) (ls)",
	"andor test 104":r"(ls) > (ls)",
	"andor test 105":r"\"(ls)\"",
	"andor test 106":r"(ls) && (ls)",
	"andor test 107":r"fskdjfjlk || exit",
	"andor test 108":r"./sfsffff",
	"andor test 109":r"export s=\"   hi      hel       l        o\" && echo $s",
}

def set_up():
	subprocess.call(['mkdir', '/tmp/tmp'])
	subprocess.call(['mkdir', 'outfiles'])
	subprocess.call(['cp', '-r', 'test_files', '/tmp/tmp'])
	subprocess.call(['cp', 'minishell', '/tmp/tmp/minishell'])
	os.chdir("/tmp/tmp")

def clean_up():
	subprocess.call(['rm', '-fr', '/tmp/tmp'])
	subprocess.call(['rm', '-fr', 'outfiles'])

def run_tests(tests):
	skip = ["test 21", "test 22"]
	value = ["test 36", "test 38","Exit 2","Exit 4","Exit 5","Exit 6","Exit 7","Exit 8","Exit 9","Exit 10","Exit 14","Exit 15","Exit 18","Exit 21"]
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
			print(f"{key}: ✅")
		else:
			test = tests[key].replace('\\', '')
			print(f"\033[1;31m{key} ❌ \033[0m{test}\033[0;31m\n===>Expeced return -- value: {expected_rv}\n{expected_stdout}\n===>Got return -- value: {got_rv}\n{got_stdout}\033[0m")
		sleep(0.02)
	# subprocess.call(['cd', '-'])



def get_bash_return(command):
	expected = subprocess.run(f"echo {command} | bash",
		shell=True, capture_output=True, text=True)
	# print(f"[{expected.stdout}] {expected.returncode} {expected.stderr}")
	return expected.stdout, expected.stderr, 127 if command == "." else expected.returncode

def get_minishell_return(command):
	got = subprocess.run(f"echo {command} | ./minishell",
		shell=True, capture_output=True, text=True)
	# print(f"[{remove_fl_lines(got.stdout)}] {got.returncode} {got.stderr}")
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
		# "?": "\?",
		";": "\;",
		"#": "\#",
	}
	for char, replacement in replacements.items():
		s = s.replace(char, replacement)
	return s


def rm_tests(key):
	rmv = ["test 28", "test 29", "test 30"]
	if key in rmv:
		subprocess.run("rm -fr /tmp/test00 /tmp/test22", shell=True, capture_output=True)

if __name__ == '__main__':
	set_up()
	# print(
	# 	"""
	# 	\033[1;35m.______    __    __   __   __      .___________. __  .__   __.      _______.   .___________. _______      _______..___________.  _______.
	# 	|   _  \  |  |  |  | |  | |  |     |           ||  | |  \ |  |     /       |   |           ||   ____|    /       ||           | /       |
	# 	|  |_)  | |  |  |  | |  | |  |     `---|  |----`|  | |   \|  |    |   (----`   `---|  |----`|  |__      |   (----``---|  |----`|   (----`
	# 	|   _  <  |  |  |  | |  | |  |         |  |     |  | |  . `  |     \   \           |  |     |   __|      \   \        |  |      \   \ 
	# 	|  |_)  | |  `--'  | |  | |  `----.    |  |     |  | |  |\   | .----)   |          |  |     |  |____ .----)   |       |  |  .----)   |
	# 	|______/   \______/  |__| |_______|    |__|     |__| |__| \__| |_______/           |__|     |_______||_______/        |__|  |_______/\033[0m 
	# """,
	# )
	print(
		"""
		\033[1;35m
		███╗░░░███╗░█████╗░███╗░░██╗██████╗░░█████╗░████████╗░█████╗░██████╗░██╗░░░██╗
		████╗░████║██╔══██╗████╗░██║██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗██╔══██╗╚██╗░██╔╝
		██╔████╔██║███████║██╔██╗██║██║░░██║███████║░░░██║░░░██║░░██║██████╔╝░╚████╔╝░
		██║╚██╔╝██║██╔══██║██║╚████║██║░░██║██╔══██║░░░██║░░░██║░░██║██╔══██╗░░╚██╔╝░░
		██║░╚═╝░██║██║░░██║██║░╚███║██████╔╝██║░░██║░░░██║░░░╚█████╔╝██║░░██║░░░██║░░░
		╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝░░╚══╝╚═════╝░╚═╝░░╚═╝░░░╚═╝░░░░╚════╝░╚═╝░░╚═╝░░░╚═╝░░░\033[0m 
	""",
	)
	print(
		"""
		\033[0;36m
█▄▄ █░█ █ █░░ ▀█▀ █▀   █ █▄░█
█▄█ █▄█ █ █▄▄ ░█░ ▄█   █ █░▀█\033[0m 
	""",
	)
	run_tests(builts)
	print(
		"""
		\033[0;36m
█▀█ █▀▀ █▀▄ █ █▀█ █▀▀ █▀▀ ▀█▀ █ █▀█ █▄░█
█▀▄ ██▄ █▄▀ █ █▀▄ ██▄ █▄▄ ░█░ █ █▄█ █░▀█\033[0m 
	""",
	)
	run_tests(tests_red)
	print(
		"""
		\033[0;36m
█▀ █▄█ █▄░█ ▄▀█ ▀█▀ ▄▀█ ▀▄▀
▄█ ░█░ █░▀█ █▀█ ░█░ █▀█ █░█\033[0m 
	""",
	)

	run_tests(syntax)
	# run_tests(rederction)
	# run_tests(tests_red)
	if (len(sys.argv) >= 2 and sys.argv[1] == "bonus"):
		print(
			"""
			\033[1;35m
			
			██████╗░░█████╗░███╗░░██╗██╗░░░██╗░██████╗
			██╔══██╗██╔══██╗████╗░██║██║░░░██║██╔════╝
			██████╦╝██║░░██║██╔██╗██║██║░░░██║╚█████╗░
			██╔══██╗██║░░██║██║╚████║██║░░░██║░╚═══██╗
			██████╦╝╚█████╔╝██║░╚███║╚██████╔╝██████╔╝
			╚═════╝░░╚════╝░╚═╝░░╚══╝░╚═════╝░╚═════╝░\033[0m 
		""",
		)
		print(
			"""
			\033[0;36m
█░█░█ █ █░░ █▀▄ █▀▀ ▄▀█ █▀█ █▀▄
▀▄▀▄▀ █ █▄▄ █▄▀ █▄▄ █▀█ █▀▄ █▄▀\033[0m 
		""",
		)
		run_tests(wildcard)
		print(
			"""
			\033[0;36m
▄▀█ █▄░█ █▀▄     █▀█ █▀█
█▀█ █░▀█ █▄▀     █▄█ █▀▄\033[0m 
		""",
		)
		run_tests(andor)
		print(
			"""
			\033[0;36m

▄▀█ █▄░█ █▀▄   █▀█ █▀█   █░█░█ █ █░░ █▀▄ █▀▀ ▄▀█ █▀█ █▀▄
█▀█ █░▀█ █▄▀   █▄█ █▀▄   ▀▄▀▄▀ █ █▄▄ █▄▀ █▄▄ █▀█ █▀▄ █▄▀\033[0m 
		""",
		)
		run_tests(andorwild)
	clean_up()
RESET="\033[0m"
BLACK="\033[30m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
WHITE="\033[37m"
BOLDBLACK="\033[1m\033[30m"
BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
BOLDYELLOW="\033[1m\033[33m"
BOLDBLUE="\033[1m\033[34m"
BOLDMAGENTA="\033[1m\033[35m"
BOLDCYAN="\033[1m\033[36m"
BOLDWHITE="\033[1m\033[37m"

make -C ../ > /dev/null
cp ../minishell .
chmod 755 minishell

function exec_test_with_files()
{
    echo test1 > test1.txt
    echo test22 > test2.txt
    echo test333 > test3.txt
    echo test4444 > test4.txt
    echo test55555 > test5.txt
    echo test666666 > test6.txt
	TEST1=$(echo $@ "; exit" | ./minishell 2>&-)
	ES_1=$?
    rm test[1-6].txt
    echo test1 > test1.txt
    echo test22 > test2.txt
    echo test333 > test3.txt
    echo test4444 > test4.txt
    echo test55555 > test5.txt
    echo test666666 > test6.txt
	TEST2=$(echo $@ "; exit" | bash 2>&-)
	ES_2=$?
    rm test[1-6].txt
	if [ "$TEST1" == "$TEST2" ] && [ "$ES_1" == "$ES_2" ]; then
        echo "[OK] " $@ >> results.txt
	else
        echo "[KO] " $@ >> results.txt
	fi
    echo $TEST1 >> results.txt
    echo $TEST2 >> results.txt
    echo >> results.txt
	if [ "$TEST1" == "$TEST2" ] && [ "$ES_1" == "$ES_2" ]; then
		printf " $GREEN%s$RESET" "[OK] "
	else
		printf " $RED%s$RESET" "[KO] "
	fi
	printf "$CYAN \"$@\" $RESET"
	if [ "$TEST1" != "$TEST2" ]; then
		echo
		echo
		printf $RED"Your output : \n%.20s\n$RED$TEST1\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
		printf $WHITE"Expected output : \n%.20s\n$WHITE$TEST2\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
	fi
	if [ "$ES_1" != "$ES_2" ]; then
		echo
		echo
		printf $RED"Your exit status : $RED$ES_1$RESET\n"
		printf $WHITE"Expected exit status : $WHITE$ES_2$RESET\n"
	fi
	echo
	sleep 0.1
}

function exec_test()
{
	TEST1=$(echo $@ "; exit" | ./minishell 2>&-)
	ES_1=$?
	TEST2=$(echo $@ "; exit" | bash 2>&-)
	ES_2=$?
	if [ "$TEST1" == "$TEST2" ] && [ "$ES_1" == "$ES_2" ]; then
        echo "[OK] " $@ >> results.txt
	else
        echo "[KO] " $@ >> results.txt
	fi
    echo $TEST1 >> results.txt
    echo $TEST2 >> results.txt
    echo >> results.txt
	if [ "$TEST1" == "$TEST2" ] && [ "$ES_1" == "$ES_2" ]; then
		printf " $GREEN%s$RESET" "[OK] "
	else
		printf " $RED%s$RESET" "[KO] "
	fi
	printf "$CYAN \"$@\" $RESET"
	if [ "$TEST1" != "$TEST2" ]; then
		echo
		echo
		printf $RED"Your output : \n%.20s\n$RED$TEST1\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
		printf $WHITE"Expected output : \n%.20s\n$WHITE$TEST2\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
	fi
	if [ "$ES_1" != "$ES_2" ]; then
		echo
		echo
		printf $RED"Your exit status : $RED$ES_1$RESET\n"
		printf $WHITE"Expected exit status : $WHITE$ES_2$RESET\n"
	fi
	echo
	sleep 0.1
}

# Mandatory Part
echo > results.txt ;

# Syntax error
# exec_test ';'
# exec_test '|'
# exec_test '<'
# exec_test '; ; '
# exec_test '| |'
# exec_test '< >'
# exec_test ';; echo'
# exec_test '| echo'
# exec_test 'echo > <'
# exec_test 'echo | |'
# exec_test 'echo hello > |'
# exec_test 'echo hello > ;'
# exec_test 'echo hello >> |'
# exec_test 'echo hello >> ;'
# exec_test 'cat < |'
# exec_test 'cat < ;'

# ; Separator
exec_test 'echo hello'
exec_test 'echo hello world'
exec_test 'echo hello ; echo world'
exec_test 'echo hello ; echo world'
exec_test 'echo 01234567890123456789 ; echo ; echo ; echo'

# | Pipe
exec_test 'ls | wc | wc'
exec_test 'ls | wc | wc ; echo test'
exec_test 'ls | wc | wc ; echo test | wc'
# exec_test 'sleep 1 | echo 1 ; sleep 2 | echo 2'

# unknown|

# > < >> Redirection

# redirection op without file should be syntax error
# exec_test 'echo hello > '
# exec_test 'echo hello >'
# exec_test 'echo hello >>'
# exec_test 'cat <'
# exec_test 'cat < '

# exec_test_with_files 'echo hello > > test1.txt'
# exec_test_with_files 'echo hello >> >> test1.txt'
# exec_test_with_files 'echo hello > >> test1.txt'
# exec_test_with_files 'echo hello > < test1.txt'
# exec_test_with_files 'cat < < test1.txt'

# redirection should work without command
# exec_test '> echo ; file echo ; rm echo'
# exec_test '>> echo ; file echo ; rm echo'
# exec_test '< echo'
# exec_test_with_files '<test1.txt'
# exec_test_with_files '<test1.txt<test2.txt<test3.txt<test4.txt'

# < redirection shoud fail if file does not exist
exec_test 'cat < nonexistingfile'
exec_test 'echo hello > test.txt ; cat < nonexistingfile < test.txt'
rm test.txt

# < redirection should work with multiple files
exec_test_with_files 'wc < test1.txt < test2.txt < test3.txt < test4.txt < test5.txt ; cat test[1-6].txt'

# > redirection should redirect output to file
exec_test 'echo > test.txt ; cat test.txt ; rm test.txt'
exec_test 'echo hello > test.txt world ; cat test.txt'

# > redirection should create non-existing file
exec_test 'echo non-existing file > nonexist ; cat nonexist ; rm nonexist'

# > redirection should work with multiple files
exec_test_with_files 'echo aaa > test1.txt > test2.txt > test3.txt > test4.txt > test5.txt > test6.txt ; cat test[1-6].txt'

# >> redirection should create file
exec_test 'rm test1.txt ; echo aaa >> test1.txt ; echo bbb >> test1.txt ; cat test1.txt'

# >> redirection should work with multiple files
exec_test_with_files 'echo aaa >> test1.txt >> test2.txt >> test3.txt ; echo bbb >> test1.txt >> test2.txt >> test3.txt ; cat test[1-3].txt'

# redirections whould work in mix
exec_test_with_files 'wc < test1.txt < test2.txt > test3.txt > test4.txt < test5.txt > test6.txt'

exec_test_with_files 'cat > test2.txt > test3.txt > test4.txt < test1.txt'

exec_test_with_files 'echo | cat | wc > test.txt < test6.txt ; cat test.txt'

exec_test_with_files 'echo test666666 >> test6.txt | head -n 1 < test6.txt > test1.txt ; wc > test2.txt < test1.txt ; cat test2.txt'

# redirections and pipes
exec_test_with_files 'cat test1.txt | cat < test2.txt'
exec_test_with_files 'cat < test1.txt | cat > test2.txt ; cat test2.txt'

# Quotes

# quotes should work in simple use cases
exec_test "echo hello'world'foo"
exec_test "echo \"oh'hello\"world\"yeah'wow\""
exec_test "echo \"oh'hello\"wor\"\"''ld\"yeah'wow\""
exec_test "echo \"oh'h\"\"ello\"wor\"\"''''''ld\"yeah'w''ow\""
exec_test "echo hel'lo;''wor|$'''ld yeah"

# quotes should work empty
exec_test "echo ''"
exec_test "echo ''''''"
exec_test "echo \"\""
exec_test "echo \"\"\"\"\"\""

# quotes should work with whitespaces
exec_test "echo '   ' | cat -e"
exec_test "echo '   ''      ''  ' | cat -e"
exec_test "echo \"             \" | cat -e"
exec_test "echo \"  \"\"    \"\"         \""

# quotes should not add operators
exec_test "echo hello ';' cat ; echo hello \";\" cat"
exec_test "echo hello '|' cat ; echo hello \"|\" cat"
exec_test " echo hello '>' '>' ; echo hello \">\" \">\""
exec_test " echo hello '>>' '>>' ; echo hello \">>\" \">>\""
exec_test "echo > \">\" hello ; cat \">\" ; rm \">\""
exec_test "echo hello \' >  > \' > '>' > \" > >\" world ; cat \" > >\" ; rm \" > >\""

# quotes do not split fields
exec_test "\"echo hello\""
exec_test "'echo hello'"
exec_test "\"  echo hello there my friend | cat    \""
exec_test "'  echo hello there my friend | cat    '"

# Single quotes

# single quote should preserve literal values
exec_test "echo 'hello \$USER %* # @ ^ &* ( @ ! ) there | cat ; wc ; {}|\\\\\\ '"

# Double quotes

# double quotes should preserve literal values
exec_test "echo \"hello USER %* #' @ ^ &* ( @ ! ) there | cat ; wc ; {}|\""

# double quotes backslash should escape only if followed by $ " \
exec_test "echo \" hello \;there\2my\0friend\""
exec_test "echo \" hello \there\my\nfriend\""

exec_test "echo \" \\\" lit\\\"erally \""
exec_test "echo \" \\\$USER lit\\\$USERerally \""
exec_test "echo \" \\\\ lit\\\\erally \""

# double quotes should expand $
exec_test "echo \"$\""
exec_test "echo \"  $  $  $ $ $ $\""
exec_test "echo \"   \$USER \$CWD \""
exec_test "echo \"   aaa\$USER bbb\$CWDccc \""

# double quotes should add empty to unknown variable
exec_test "echo \"  \$UNKNOWNVARIABLE  \$_WHATIS_this999 \$_32175891  \$________\""


# $?

# $? should print exit status
exec_test 'true ; echo $? ; false ; echo $? ; cat nonexist ; echo $?'
exec_test 'true ;echo $?$?$?'
exec_test "false ; echo \$USER\$?\"\$?\"'\$?' $  \$?"
exec_test "/bin/ls | cat | wc ; echo \$?"

# $? should work in sequence
exec_test 'echo $? $? $? $? $?'
exec_test 'true ; echo $? ; false ; echo $? ; true | echo $? ; false | echo $?'

# $? should be undefined in pipe
exec_test 'true ; echo $? | cat | false | echo $?'

# $? should work in pipes and redirections
exec_test_with_files "false ; echo \$? > test1.txt > test2.txt ; echo \$? | cat >>test2.txt ; cat test2.txt | echo \$?"

# Expansion
# dollar sign should print
exec_test 'echo $'
exec_test 'echo $ $ $    $ $ $ $'

# dollar sign should expand
exec_test 'echo $USER $ZXY $PWD'
exec_test 'echo $UNKNWONVARIABLE $_WHATis_This999 $_1324810'
exec_test "echo \$USER aaa\$USER\"bbb 'ccc' \"\"\"\"\$USER \"\$USERddd"
exec_test "echo aaa\$USER\"\$ZXY\"\$ZXY"

# variable expansion should work with ;
# exec_test 'export A=aaa ; echo $A ; unset A'

# variable expansion order is undefined in pipe
# exec_test 'export B=bbb | echo $B | cat'
# exec_test 'echo $B | export B=bbb | cat'

# variable expansion is before redirection
# exec_test 'export C=ccc ; echo $C > $C ; cat $C'

# variable expansion is before word
# exec_test 'export ECHO=echo ; export CAT=cat ; \$ECHO \$CAT | \$CAT'


# variable expansion as special characters

# PIPE='|' ; echo $PIPE ; echo PIPE $PIPE cat

# PIPE='|' ; GREATER='>' ; CAT=cat ; echo $PIPE | $CAT ; echo $PIPE | $CAT $GREATER wow ; cat wow ; rm wow

# ECHO='echo hello world' ; $ECHO ; echo $ECHO
# ECHO="echo hello | cat | wc" ; $ECHO ; echo $ECHO

# echo hello | cat
# CAT=cat ; echo hello | $CAT
# CAT='cat -e' ; echo hello | $CAT
# CAT='cat -e | wc' ; echo $CAT
# CAT='cat -e | wc' ; $CAT
# CAT='cat -e | wc' ; echo hello | $CAT
# CAT='cat -e ; echo what ; $CAT'
# echo what > what.txt ; CAT='cat what.txt' ; $CAT ; rm what.txt
# CAT='cat $CAT' ; $CAT

# ECHO='echo hello > what.txt ; cat what.txt' ; $ECHO

# variables expansion should be after redirection
# ECHO='echo what > what.txt' ; $ECHO
# ECHO='echo what > what.txt' ; $ECHO > what.txt ; cat what.txt

# WOW='hello world' ; cat aaa$WOW ; cat "aaa$WOW" ; cat 'aaa$WOW'


# Builtins
# exec_test 'pwd ; cd .. | pwd'

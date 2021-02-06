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

### Mandatory Part
echo > results.txt ;

### HOT FIX
exec_test 'cat /dev/random | head -c 100 | wc -c'
exec_test "export a=\"aaa\"; echo \"\$a\", '\$a', \$a"
exec_test "echo \"\\\$\$a\""
exec_test "export a=\"echo abc > test1.txt\"; \$a"

exec_test "export A ; echo hello > \$A"
exec_test "export A= ; echo hello > \$A"
exec_test "export A='a b c' ; echo hello > \$A"

exec_test "export a=\"   a   \"; export b=\"  bbb  \"; echo \$a\$b"
exec_test "export a='a' b=' b ' c='c ' d=' d' e='e e'; echo \$a\$a \$a\$b \$a\$c \$a\$d \$a\$e | cat -e; echo \$b\$a \$b\$b \$b\$c \$b\$d \$b\$e | cat -e; echo \$c\$a \$c\$b \$c\$c \$c\$d \$c\$e | cat -e; echo \$d\$a \$d\$b \$d\$c \$d\$d \$d\$e | cat -e; echo \$e\$a \$e\$b \$e\$c \$e\$d \$e\$e | cat -e"
exec_test "echo ^*@^@#890#aAS"
exec_test "export TEST=echo ; ''\$TEST 1"
exec_test "export TEST=echo ; \"\"''\$T'ES'T 1"

exec_test "\$A"
exec_test "\$OLDPWD"

exec_test "export ECHO=echo'\"' ; \$ECHO 1"
exec_test "export ECHO=echo\"'\" ; \$ECHO 1"
exec_test "export 'ECHO=echo\"\"' ; \$ECHO 1"
exec_test "export \"ECHO=echo''\" ; \$ECHO 1"

exec_test "export PATH=' ';echo \$PATH"
exec_test "export A=\"           \" ;echo \$A"

exec_test "export A='' B=\" \" C=\"    \" D=\"  d \" ; echo \$A \$B \$C \$D | cat -e ; echo \$A\$B''\"\$C\"\$D | cat -e"
exec_test "export A='a' B=' ' C=' c ' ; echo \$A\$B\$A | cat -e ; echo \$A\$A\$A \$A \$A | cat -e ; echo \$C\$B \$B\$C | cat -e ; echo \$A\$C \$A\$B\$C"
exec_test "export A='a' B=' ' ; echo \$A\$B\$A ; echo \$A \$B \$A; echo \$A\$B\$B\$B\$A"

exec_test "exit -"
exec_test 'exit +1'
exec_test 'exit +0'
exec_test "exit ' 3'"
exec_test "exit '\t\f\r 3'"
exec_test "exit 4294967296"
exec_test "exit -4294967297"
exec_test "exit 4294967295"
exec_test "exit -4294967295"
exec_test "exit Mollitia asperiores"
exec_test "exit 123 456 asperiores"
exec_test "exit 18446744073709551615"
exec_test "exit -922337285"
exec_test "exit +922337285"
exec_test "exit -922337203685"
exec_test "exit +922337203685"
exec_test "exit -9223372036854775"
exec_test "exit +9223372036854775"
exec_test "exit -9223372036854775807"
exec_test "exit -9223372036854775808"
exec_test "exit -9223372036854775809"
exec_test "exit +9223372036854775807"
exec_test "exit +9223372036854775806"
exec_test "exit 9223372036854775808"
exec_test "exit 92233720368547758099999999"
exec_test "exit -92233720368547758099999999"
exec_test "exit +00092233720368547758099999999"
exec_test "exit -0000092233720368547758099999999"


exec_test "mkdir ccc ; echo '#!/bin/sh' 'echo hello' > ccc/ddd ; chmod +x ccc/ddd ; ccc/ddd ; rm -rf ccc"


### 未対応
# exec_test "export A=aaa ; export A+=bbb ; echo \$A"

### Syntax error
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
# exec_test 'echo hello > '
# exec_test 'echo hello >>'
# exec_test 'cat < '
# exec_test_with_files 'echo hello > > test1.txt'
# exec_test_with_files 'echo hello >> >> test1.txt'
# exec_test_with_files 'echo hello > >> test1.txt'
# exec_test_with_files 'echo hello > < test1.txt'
# exec_test_with_files 'cat < < test1.txt'


### Commnad not found (127)
exec_test 'unknowncommand'
exec_test 'unknowncommand ; commandunknown'
exec_test 'unknowncommand | commandunknown'
exec_test 'unknowncommand > nani ; file nani ; rm nani'
exec_test 'asdfas | echo hello | echo world > WORLD | asdfas ; file WORLD; rm WORLD'


### Command not executable (126)
exec_test "echo '#!/bin/bash' > a ; echo 'echo hello'>>a ; ./a; echo \$?;chmod +x a; ./a; echo \$? ; rm a"

### Absolute path

exec_test '/bin/ls ; echo $?'
exec_test '/home ; echo $?'
exec_test '$PWD/minishell ; echo $SHLVL'


### Relative path
exec_test '$OLDPWD ; echo $?'
exec_test './. ; echo $?'
exec_test './.. ; echo $?'
exec_test './... ; echo $?'
exec_test 'touch aaa ; ./aaa ; rm aaa'
exec_test 'touch bbb ; chmod +x bbb ; ./bbb ; rm bbb'
exec_test 'mkdir ccc ; ./ccc ; rmdir ccc'
exec_test 'mkdir ccc ; touch ccc/ddd ; chmod +x ccc/ddd ; ccc/ddd ; rm -rf ccc'
exec_test 'touch eee ; echo "echo \$SHLVL" > eee ; echo exit >> eee ; ./minishell < eee ; rm eee'
exec_test 'touch fff ; echo "echo \$SHLVL" > fff ; echo exit >> fff ; ././././././././minishell < fff ; rm fff'
exec_test 'touch ggg ; echo "echo \$SHLVL" > ggg ; echo exit >> ggg ; .///././/./././minishell < ggg ; rm ggg'
exec_test 'touch hhh ; echo "echo \$SHLVL" > hhh ; echo \$OLDPWD >> hhh ; echo exit >> hhh ; .///././/./././minishell < hhh; rm hhh'


### SHLVL
# SHLVLV に文字列やINT_MAX, INT_MIN 以外の数字を与える
# SHLVL の最大値は 1000
# 1000 は表示されない


### Builtins

# echo
# exec_test 'echo $_ | echo $_ ; echo $_'
# exec_test 'echo $_ | echo $_ ; echo $_ ; echo $_'
# exec_test 'echo 1 2 3 ; echo $_ ; echo $_'
exec_test "echo | cat -e"
exec_test "echo 1 | cat -e"
exec_test "echo 1 2 3 | cat -e"
exec_test "echo 1 2 3      | cat -e"
exec_test "echo 1 2                      3 | cat -e"
exec_test 'echo -n | cat -e'
exec_test 'echo -""n | cat -e'
exec_test 'echo ""-""n"" | cat -e'
exec_test 'echo -n hello | cat -e'
exec_test 'echo -n ""h""el""""lo | cat -e'
exec_test 'echo -nnnnnnnn hello | cat -e'
exec_test 'echo ""-nn""n""""nnn""nn hello | cat -e'
exec_test 'echo -n1 | cat -e'
exec_test 'echo -n -n | cat -e'
exec_test 'echo -n -n hello | cat -e'
exec_test 'echo -n -nnnnn hello | cat -e'
exec_test 'echo -nnnnn -nnnnn hello | cat -e'
exec_test 'echo -n -n -n | cat -e'
exec_test 'echo -n -n -n hello | cat -e'
exec_test 'echo -n -nnnn -n hello -n -nnnnn -n | cat -e'
exec_test 'echo -nnnn -nnnn -n hello -n -nnnnn -n | cat -e'
exec_test 'echo -n -nnnn -n hello -n -nnnnn -n world | cat -e'
exec_test 'echo -""n -nnnn -n hello -n -nnnnn -n world | cat -e'
exec_test "echo - n1 | cat -e"
exec_test "echo - n 1 | cat -e"
exec_test "echo -n 1 | cat -e"
exec_test "echo -n 1 2 3 | cat -e"
exec_test "echo -n 1 2                      3 | cat -e"
exec_test "echo 1 2 3 -n | cat -e"
exec_test "echo -n 1 2 3 -n | cat -e"
exec_test "echo -n 1 2 -n 3 -n | cat -e"
exec_test "       echo 1 2 3 -n | cat -e"
exec_test "       echo -n 1 2 3 -n | cat -e"
exec_test "echo a '' b '' c '' d | cat -e"
exec_test 'echo a "" b "" c "" d | cat -e'
exec_test "echo -n a '' b '' c '' d | cat -e"
exec_test 'echo -n a "" b "" c "" d | cat -e'
exec_test "echo '' '' ''          Echo 1 | cat -e"
exec_test "eCho 1 | cat -e"
exec_test "ecHo 1 | cat -e"
exec_test "echO 1 | cat -e"
exec_test "EchO 1 | cat -e"
exec_test "eCHo 1 | cat -e"
exec_test "EcHo 1 | cat -e"
exec_test "eChO 1 | cat -e"
exec_test "ECHO 1 | cat -e"
exec_test "unset PATH ; echo 1 | cat -e"
exec_test "unset PATH ; ECHO 1 | cat -e"
exec_test "''e''c''h''o'' 1 | cat -e"
exec_test "''e''c''h''o'''''' 1 | cat -e"
exec_test '""e""c""h""o"" 1 | cat -e'
exec_test '""e""c""h""o"""""" 1 | cat -e'
# exec_test 'echo 1 ; echo $_'
# exec_test 'ls ; echo $_'
# exec_test '1 ; echo $_'
# exec_test '1 ; echo $_ ; echo $_'
# exec_test '1 | echo $_'
# exec_test '1 | echo $_ | echo $_'
# exec_test '1 | echo $_ ; echo $_'
# exec_test 'echo $_ | echo $_'
# exec_test 'echo $_ | echo $_ ; echo $_'
# exec_test 'echo $_ | echo $_ ; echo $_ ; echo $_'
# exec_test 'echo 1 2 3 ; echo $_ ; echo $_'
# exec_test 'echo "1 2 3" ; echo $_ ; echo $_'

# cd
exec_test 'mkdir a ; cd a ; rm -fr ../a ; cd ; pwd'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; cd "" ; pwd'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; cd . ; pwd'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; cd ./ ; pwd'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; cd ./././ ; pwd'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; cd ./././///././///// ; pwd'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; cd .. ; pwd'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; cd ../ ; pwd'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; export HOME= PWD= OLDPWD= ; cd ; pwd ; echo $HOME $PWD $OLDPWD'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; export HOME= PWD= OLDPWD= ; cd "" ; pwd ; echo $HOME $PWD $OLDPWD'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; export HOME= PWD= OLDPWD= ; cd $HOME ; pwd ; echo $HOME $PWD $OLDPWD'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; export HOME= PWD= OLDPWD= ; cd . ; pwd ; echo $HOME $PWD $OLDPWD'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; export HOME= PWD= OLDPWD= ; cd . ; pwd ; echo $HOME $PWD $OLDPWD ; cd . ; pwd ; echo $HOME $PWD $OLDPWD'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; export HOME= PWD= OLDPWD= ; cd . ; pwd ; echo $HOME $PWD $OLDPWD ; cd . ; pwd ; echo $HOME $PWD $OLDPWD ; cd // ; pwd ; cd home ; pwd ; echo $HOME $PWD $OLDPWD'
exec_test 'mkdir a ; cd a ; rm -fr ../a ; export HOME= PWD= OLDPWD= ; cd .. ; pwd ; echo $HOME $PWD $OLDPWD'
exec_test 'cd /.// ; pwd'
exec_test 'cd /./////././//// ; pwd'
exec_test 'cd // ; pwd'
exec_test 'cd //...........////// ; pwd'
exec_test 'cd // ; pwd ; cd / ; pwd'
exec_test 'cd // ; pwd ; cd / ; pwd ; cd /// ; pwd'
exec_test 'cd // ; pwd ; cd bin ; pwd'
exec_test 'cd //. ; pwd'
exec_test 'cd //./ ; pwd'
exec_test 'cd //.///./././//// ; pwd'
exec_test 'cd /// ; pwd'
exec_test 'cd ///./// ; pwd'
exec_test 'cd //// ; pwd'
exec_test 'cd ////./././////// ; pwd'
exec_test 'cd ///// ; pwd'
exec_test 'cd /bin ; pwd'
exec_test 'cd /usr/bin ; pwd'
exec_test 'cd /home ; pwd'
exec_test 'cd /home ; pwd ; cd .. ; ls -l'
exec_test 'cd //home ; pwd'
exec_test 'cd //home ; pwd ; cd .. ; ls -l'
exec_test 'cd //./home ; pwd'
exec_test 'cd //./home ; pwd ; cd .. ; ls -l'
exec_test 'cd "//home" ; pwd'
exec_test 'cd "//home" ; pwd ; cd .. ; ls -l'
exec_test 'mkdir test_dir ; cd test_dir ; rm -rf ../test_dir ; pwd ; echo $PWD $OLDPWD ; unset PWD OLDPWD ; pwd ; echo $PWD $OLDPWD ; cd . ; pwd ; echo $PWD $OLDPWD ; cd . ; pwd ; echo $PWD $OLDPWD ; cd .. ; pwd ; echo $PWD $OLDPWD'
exec_test 'mkdir test_dir ; cd test_dir ; rm -rf ../test_dir ; cd . ; pwd ; cd . ; pwd ; cd .. ; pwd'
exec_test 'mkdir test_dir ; cd test_dir ; rm -rf ../test_dir ; cd . ; cd . ; echo $PWD ; echo $OLDPWD ; cd .. ; pwd'
exec_test 'mkdir test_dir ; cd test_dir ; rm -rf ../test_dir ; cd . ; pwd ; cd . ; pwd ; cd .. ; pwd'
exec_test 'unset PWD ; cd ; echo $? ; pwd ; echo $PWD ; echo $OLDPWD'
exec_test 'unset PWD ; cd "" ; echo $? ; pwd ; echo $PWD ; echo $OLDPWD'
exec_test 'unset OLDPWD ; cd ; echo $? ; pwd ; echo $PWD ; echo $OLDPWD'
exec_test 'unset OLDPWD ; cd "" ; echo $? ; pwd ; echo $PWD ; echo $OLDPWD'
exec_test 'unset PWD OLDPWD ; cd ; echo $? ; pwd ; unset PWD ; pwd ; echo $PWD ; echo $OLDPWD'
exec_test 'unset PWD OLDPWD ; cd "" ; echo $? ; pwd ;unset PWD ; pwd ; echo $PWD ; echo $OLDPWD'
exec_test 'cd ; echo $HOME ; echo $PWD ; echo $OLDPWD ; echo $?'
exec_test 'cd "" ; echo $HOME ; echo $PWD ; echo $OLDPWD ; echo $?'
exec_test 'export HOME=aaaa ; cd ; echo $HOME ; echo $PWD ; echo $OLDPWD ; echo $?'
exec_test 'export HOME=aaaa ; cd "" ; echo $HOME ; echo $PWD ; echo $OLDPWD ; echo $?'
exec_test 'export HOME= ; export PWD= ; export OLDPWD= ; cd ; echo $HOME ; echo $PWD ; echo $OLDPWD ; echo $?'
exec_test 'export HOME= ; cd ; echo $HOME ; echo $PWD ; echo $OLDPWD ; echo $?'
exec_test 'export HOME= ; export PWD= ; export OLDPWD= ; cd "" ; echo $HOME ; echo $PWD ; echo $OLDPWD ; echo $?'
exec_test 'cd aaaaaa'
exec_test 'cd aaaaaa ; pwd'
exec_test 'echo $PWD ; echo $OLDPWD ; cd aaaaaa ; echo $PWD ; echo $OLDPWD'
exec_test 'cd ""'
exec_test 'cd "" ; pwd'
exec_test 'echo $PWD ; echo $OLDPWD ; cd "" ; echo $PWD ; echo $OLDPWD'
exec_test 'cd " "'
exec_test 'cd " " ; pwd'
exec_test 'echo $PWD ; echo $OLDPWD ; cd " " ; echo $PWD ; echo $OLDPWD'
exec_test 'cd ./'
exec_test 'cd ./ ; pwd'
exec_test 'echo $PWD ; echo $OLDPWD ; cd ./ ; echo $PWD ; echo $OLDPWD'
exec_test 'echo $OLDPWD'
exec_test 'cd $OLDPWD ; pwd'
exec_test 'cd $OLDPWD ; cd $OLDPWD ; pwd'
exec_test 'unset OLDPWD ; cd $OLDPWD ; pwd ; echo $PWD ; echo $OLDPWD'
exec_test 'unset OLDPWD ; cd $OLDPWD ; pwd ; echo $OLDPWD ; echo $PWD'
exec_test 'cd $WWW ; pwd'
exec_test 'cd $WWW$WWW../../ ; pwd'
exec_test 'cd $WWW./$WWW../$WWW ; pwd'
exec_test 'cd ../../../../../../../ ; pwd'
exec_test 'cd ../.././.././.././.././.././../ ; pwd'
exec_test '""c""d"" . ; pwd'
exec_test 'cd . ; pwd'
exec_test 'cd .. ; pwd'
exec_test 'cd .."""" ; pwd'
exec_test 'cd "".""."""" ; pwd'
exec_test 'cd ... ; pwd'
exec_test 'cd ../.. ; pwd'
exec_test 'cd ../. ; pwd'
exec_test 'cd ./. ; pwd'
exec_test 'cd / ; pwd'
exec_test 'cd // ; pwd'
exec_test 'cd //home ; pwd'
exec_test 'cd "//home" ; pwd'
exec_test 'echo $PWD; echo $OLDPWD; cd "/"; pwd; echo $PWD; echo $OLDPWD'
exec_test 'echo $PWD; echo $OLDPWD; cd "//"; pwd; echo $PWD; echo $OLDPWD'
exec_test 'cd /// ; pwd'
exec_test 'cd // ; cd / ; env | grep | sort | PWD'
exec_test 'cd / ; cd // ; env | grep | sort | PWD'
exec_test 'cd /. ; cd //. ; env | grep | sort | PWD'
exec_test 'cd /.. ; cd //.. ; env | grep | sort | PWD'
exec_test 'cd ; pwd'
exec_test 'cd                    ; pwd'
exec_test 'cd        ""    ""        ; pwd'
exec_test 'cd        " "        ; echo $?'
# exec_test 'cd ~ ; pwd'
# exec_test 'cd ~/ ; pwd'
# exec_test 'cd ~/. ; pwd'
exec_test 'unset HOME ; cd ; echo $? ; pwd'
exec_test 'unset HOME ; cd "" ; echo $? ; pwd'
# exec_test 'unset HOME ; cd ~ ; pwd'
exec_test 'mkdir d ; ln -s d dd ; cd dd ; pwd ; rm -r ../d ../dd'
exec_test 'mkdir -m 000 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 001 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 002 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 003 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 004 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 005 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 006 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 007 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 010 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 020 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 030 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 040 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 050 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 060 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 070 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 100 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 200 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 300 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 400 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 500 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 600 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 700 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 755 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 644 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 311 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 111 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 222 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 333 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 0777 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 1000 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 2000 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 3000 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 4000 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 5000 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 6000 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 7000 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 1777 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 2777 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 3777 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 4777 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 5777 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 6777 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 7777 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'
exec_test 'mkdir -m 0000 d ; echo $PWD ; echo $OLDPWD ; cd d ; echo $OLDPWD'

# pwd
exec_test 'pwd'
exec_test 'pwd | cat -e'
exec_test 'unset PWD ; pwd'
exec_test 'unset PWD ; pwd ; echo $PWD'
exec_test 'export PWD=test ; pwd ; echo $PWD'
exec_test 'cd / ; cd home ; pwd'
exec_test 'cd / ; cd home ; pwd ; cd .. ; pwd'
exec_test 'cd // ; cd home ; pwd'
exec_test 'cd // ; cd home ; pwd ; cd .. ; pwd'
exec_test 'echo $PWD ; echo $OLDPWD ; cd / ; pwd ; echo $PWD ; echo $OLDPWD'
exec_test 'echo $PWD ; echo $OLDPWD ; cd /. ; pwd ; echo $PWD ; echo $OLDPW'
exec_test 'echo $PWD ; echo $OLDPWD ; cd /./ ; pwd ; echo $PWD ; echo $OLDPW'
exec_test 'echo $PWD ; echo $OLDPWD ; cd /././././ ; pwd; echo $PWD ; echo $OLDPW'
exec_test 'echo $PWD ; echo $OLDPWD ; cd // ; pwd ; echo $PWD ; echo $OLDPW'
exec_test 'echo $PWD ; echo $OLDPWD ; cd /// ; pwd ; echo $PWD ; echo $OLDPW'
exec_test 'echo $PWD ; echo $OLDPWD ; cd //// ; pwd ; echo $PWD ; echo $OLDPWD'
exec_test 'echo $PWD ; echo $OLDPWD ; cd ///// ; pwd ; echo $PWD ; echo $OLDPWD'
exec_test 'echo $PWD ; echo $OLDPWD ; cd ; echo $PWD ; echo $OLDPWD'
exec_test 'echo $PWD ; echo $OLDPWD ; cd ; echo $OLDPWD'

# export
exec_test 'export ECHO="echo   " ; $ECHO 1'
exec_test 'export ECHO="echo""   " ; $ECHO 1'
exec_test 'export ECHO=" echo   " ; $ECHO 1'
exec_test 'export ECHO=" echo   " ; $ECHO 1'
exec_test 'export A=aaa ; export B=bbb ; echo $A$B | cat -e'
exec_test 'export A=" aaa " ; export B=" bbb " ; echo $A$B | cat -e'

exec_test 'export TEST=echo ; $TEST 1'
exec_test "export TEST=echo ; ''$TEST 1"
exec_test 'export TEST=echo ; ""$TEST 1'
exec_test 'export TEST=echo ; "$TEST" 1'
exec_test "export TEST=echo ; '$TEST' 1"

# export unset (export | sort | grep -v SHLVL | grep -v _=)
# export unset
# exec_test 'export | grep -v _='

# exec_test 'export -f > /dev/null 2>&1 ; echo $? ; export -n > /dev/null 2>&1 ; echo $?'
# exec_test "env | wc > /dev/null 2>&1"
# exec_test "env > /dev/null 2>&1; echo \$? ; env > env.txt ; echo \$?; cat env.txt | wc > /dev/null 2>&1; echo \$? ; rm env.txt"
# exec_test "env -i > /dev/null 2>&1; echo \$?"
# exec_test "env name=value > /dev/null 2>&1; echo \$?"

exec_test "export _A=aaa ; echo \$_A \"\$_A\" '\$_A' ; unset _A ; echo \$_A"
exec_test 'export AB912__2asdvb_ ; export | grep AB912 | cat | wc ; unset AB91 ; export | grep AB912 ; unset AB912__2asdvb_ ; export | grep AB912'
exec_test "export ___A=aaa ___b=bbb ___C=ccc ___ddd='ddd ddd ddd' ___f=  ___g ___A=a ; export | grep ___"
exec_test "export A ; export A= ; export A='hello' ; export A='world' ; export A=\"hello world\" ; echo \$A"
exec_test "export A ; echo \$? ; export A= ; echo \$? ; export A=aaa ; echo \$?"
exec_test "export HELLO ; unset HELLO ; echo \$? ; unset HELLO ; echo \$? ; export HELLO= ; unset HELLO ; echo \$?"
exec_test 'unset ; echo $?'
exec_test 'export _A=aaa | echo $_A "$_A" ; echo $_B | export _B=bbb '

exec_test "echo 'export _A=aaa'>export.txt;export < export.txt|grep _A ; echo \$? ; rm export.txt"

# env
exec_test 'env | sort | grep -v _='
exec_test "export _ABC ; env | grep _ABC ; export _DEF= ; env | grep _DEF ; export _GHI='hello there' ; env | grep _GHI"

# exit
exec_test 'exit 0'
exec_test 'exit -'
exec_test 'exit -1'
exec_test 'exit -0'
exec_test 'exit ----21'
exec_test 'exit --++-+-21'
exec_test 'exit 255'
exec_test 'exit exit'
exec_test 'exit what ; echo $?'
exec_test 'exit 00000000'
exec_test 'echo | exit > exit.txt ; file exit.txt ; rm exit.txt'
exec_test 'echo 9 > exit.txt ; exit < exit.txt | cat ; rm exit.txt '

exec_test 'echo | exit 99 ; echo $?'
exec_test 'echo | exit 42 | exit 21 ; echo $?'
exec_test 'echo | exit 999999999999999999 ; echo $?'
exec_test 'echo | exit -12345 ; echo $?'
exec_test 'echo | exit 0 ; echo $?'
exec_test 'echo 123 | exit ; echo $?'
exec_test 'echo -123 | exit ; echo $?'

# builtins should work in pipe
exec_test 'pwd ; cd .. | pwd'

# builtins should work with redirections


### ; Separator
exec_test 'echo hello'
exec_test 'echo hello world'
exec_test 'echo hello ; echo world'
exec_test 'echo hello ; echo world'
exec_test 'echo 01234567890123456789 ; echo;echo;echo'


### | Pipe
exec_test 'ls | wc | wc'
exec_test 'ls | wc | wc ; echo test'
exec_test 'ls | wc | wc ; echo test | wc'
# exec_test 'sleep 1 | echo 1 ; sleep 2 | echo 2'


### Redirections

# redirection should work without command
exec_test '>what ; echo $? ; file what; rm what'
exec_test '> echo ; echo $?; file echo ; rm echo'
exec_test '>> echo ; echo $? ; file echo ; rm echo'
exec_test '< echo ; echo $?'
exec_test_with_files '<test1.txt'
exec_test_with_files '<test1.txt<test2.txt<test3.txt<test4.txt'

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

# redirections should work in mix
exec_test_with_files 'wc < test1.txt < test2.txt > test3.txt > test4.txt < test5.txt > test6.txt'
exec_test_with_files 'cat > test2.txt > test3.txt > test4.txt < test1.txt'
exec_test_with_files 'echo | cat | wc > test.txt < test6.txt ; cat test.txt'
exec_test_with_files 'echo test666666 >> test6.txt | head -n 1 < test6.txt > test1.txt ; wc > test2.txt < test1.txt ; cat test2.txt'

# redirections and pipes
exec_test_with_files 'cat test1.txt | cat < test2.txt'
exec_test_with_files 'cat < test1.txt | cat > test2.txt ; cat test2.txt'

# redirections with quotes
exec_test "echo hello > 'world' ; cat world ; rm world"
exec_test "echo hello > aaa\"world\" > \" a b c \"; file aaaworld ; cat \" a b c \" ; rm aaaworld \" a b c \""

# redirections with variables
exec_test "echo hello > \$USER ; cat \$USER ; rm \$USER"
exec_test "echo hello > aaa\$USER\"\$PWD\" ; cat aaa\$USER\"\$PWD\" ; rm aaa\$USER\"\$PWD\""

# redirections should fail when permission denied
exec_test "echo hello > a; cat < a;chmod -r a; cat < a; rm a"
exec_test "echo hello > a; cat a;chmod -w a; echo >> a; cat a; rm a"

### Quotes

# quotes should work in simple use cases
exec_test "echo hello'world'foo"
exec_test "echo \"oh'hello\"world\"yeah'wow\""
exec_test "echo \"oh'hello\"wor\"\"''ld\"yeah'wow\""
exec_test "echo \"oh'h\"\"ello\"wor\"\"''''''ld\"yeah'w''ow\""
exec_test "echo hel'lo;''wor|$'''ld yeah"

# quotes should work empty
exec_test "echo '' | cat -e"
exec_test "echo '''''' | cat -e"
exec_test "echo '' '''' | cat -e"
exec_test "echo \"\" | cat -e"
exec_test "echo \"\" \"\"\"\" | cat -e"
exec_test "echo '' '' ''   ''"
exec_test "echo \" \" \"\" \"\"  \"\""
exec_test "''echo '' | cat -e"
exec_test "''\"\"e'c'\"\"ho'' | cat -e"
exec_test "''\"\"l's''' | cat -e"
exec_test "ls'''''' ''\"\"-la'' | cat -e"
exec_test "echo'''' '' | ''c'at' -\"\"e"
exec_test "echo''\"\" '' '' | \"\"''cat -e"

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
exec_test "echo hello ' >  > ' > '>' > \" > >\" world ; cat \" > >\" ; rm \" > >\""

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
exec_test "echo \"\\\$\$USER\""

# double quotes should expand $
exec_test "echo \"$\""
exec_test "echo \"  $  $  $ $ $ $\""
exec_test "echo \"   \$USER \$CWD \""
exec_test "echo \"   aaa\$USER bbb\$CWDccc \""

# double quotes should add empty to unknown variable
exec_test "echo \"  \$UNKNOWNVARIABLE  \$_WHATIS_this999 \$_32175891  \$________\""


### $?

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

### Expansion

# dollar sign preceding double quote
exec_test "echo \$\"\""
exec_test "echo \$\"hello\""
exec_test "echo \$\"USER\$USER what...\""

# dollar sign should print
exec_test 'echo $'
exec_test 'echo $ $ $    $ $ $ $'

# dollar sign should expand
exec_test 'echo $USER $ZXY $PWD'
exec_test 'echo $UNKNWONVARIABLE $_WHATis_This999 $_1324810'
exec_test "echo \$USER aaa\$USER\"bbb 'ccc' \"\"\"\"\$USER \"\$USERddd"
exec_test "echo aaa\$USER\"\$ZXY\"\$ZXY"
exec_test "export WOW='hello world' ; cat aaa\$WOW ; cat \"aaa\$WOW\" ; cat 'aaa\$WOW'"

# variable expansion should work with ;
exec_test 'export A=aaa ; echo $A ; unset A ; echo $A | cat -e'

# variable expansion order is undefined in pipe
exec_test 'export B=bbb | echo $B | cat -e'
exec_test 'echo $B | export B=bbb | cat -e'

# variable expansion is before redirection
exec_test 'export C=ccc ; echo $C > $C ; cat $C ; rm $C'
exec_test "export ECHO='echo what > what.txt' ; \$ECHO"
exec_test "export ECHO='echo what > what.txt' ; \$ECHO > what.txt ; cat what.txt"

# variable expansion is before word
exec_test 'export ECHO=echo ; export CAT=cat ; $ECHO $CAT | $CAT'

# variable expansion as special characters
exec_test "export PIPE='|' ; echo \$PIPE ; echo PIPE \$PIPE cat"
exec_test "export PIPE='|' ; export GREATER='>' ; export CAT=cat ; echo \$PIPE | \$CAT ; echo \$PIPE \$GREATER wow | \$CAT"
exec_test "export ECHO='echo hello world' ; \$ECHO ; echo \$ECHO"
exec_test "export ECHO=\"echo hello | cat | wc\" ; \$ECHO ; echo \$ECHO"
exec_test "export CAT=cat ; echo hello | \$CAT"
exec_test "export CAT='cat -e' ; echo hello | \$CAT"
exec_test "export CAT='cat -e | wc' ; echo \$CAT"
exec_test "export CAT='cat -e | wc' ; \$CAT"
exec_test "echo what > what.txt ; export CAT='cat what.txt' ; \$CAT ; rm what.txt"
exec_test "export ECHO='echo hello > what.txt ; cat what.txt' ; \$ECHO"


### Optional

# export _="1 2 3" ; echo $_

# Tilda Expansion (optional?)
#exec_test 'echo ~'
#exec_test 'echo ~~'
#exec_test 'echo ~ ~'
#exec_test 'echo ~/tildaaaa'
#exec_test 'echo ~//'
#exec_test 'echo ~/ /'
#exec_test 'ls ~'
#exec_test 'ls ~'


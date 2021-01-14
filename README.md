# minishell

#### Makefileに *.c *.o *.h を追加する

`make source`

#### テストする

`make test`


## 全体像

![Parsing Commandline](https://www.dropbox.com/s/4gsycvbf4yi3f31/parsing_commandline.png?raw=1)


### Grammer

```
<commandline> ::= <pipeline>
        |  <pipeline> ";"

<pipeline> ::=  <command>
        |   <pipeline> "|" <command>

<command>  ::=  <word>
        |   <word> ">" <filename>
        |   <word> "<" <filename>
        |   <word> ">>" <filename>

<word> ::= <pathname>
        |   <word> <token>
```

## メモ

### pipe

```c
void simple_pipe(char *cmd1, char *argv1, char *cmd2, char * argv2) {
    int pipefd[2], r, status;

	r = pipe(pipefd);

    pid_t child1 = fork();
    if (child1 == 0) {
        close(pipefd[0]);
		dup2(pipefd[1], 1);
		close(pipefd[1]);
        execvp(cmd1, argv1);
    }
    assert(child1 > 0);

    pid_t child2 = fork();
    if (child2 == 0) {
        close(pipefd[1]);
		dup2(pipefd[0], 0);
		close(pipefd[0]); // closeしないとread endがブロッキングし、ずっと待った状態になる
        execvp(cmd2, argv2);
    }
    assert(child2 > 0);

	close(pipefd[0]);  // closeしないとread endがブロッキングし、ずっと待った状態になる
	close(pipefd[1]);
	r = waitpid(child1, &status, 0);
	r = waitpid(child2, &status, 0);
}
```


### redirect




## 参考


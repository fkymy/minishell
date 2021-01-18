### コマンド

`make source` Makefileに *.c *.o *.h を追加する

`make test` テストを実行する

`lsof -c minishell` File Descriptor leak確認

`strace -o trace.txt -f bash -c "<commandline>"` 本家のsyscallやsignalを確認

### 文法

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

### パース

![Parsing Commandline](https://www.dropbox.com/s/4gsycvbf4yi3f31/parsing_commandline.png?raw=1)

### プロセス

forkをすると親と子プロセスはtext segmentを共有し、register, data space, heap, stackなどのメモリはコピーされる（実際はcopy-on-write。これらの領域はread-onlyで共有されてて、writeされるときkernelはそのあたりの1ページをコピーする）。file descriptor tableなどといった情報もコピーされる。

![Parsing Commandline](https://www.dropbox.com/s/qiyjj23zhnlg2tu/Screen_Shot_2021-01-18_at_16.53.34.png?raw=1)


### プロセス間通信

プロセスは隔離されたアドレス空間で動く。そんなプロセス同士でコミュニケーションするためにUnixにはいくつかのIPC(Interprocess Communication)メカニズムがある。IPCの中で最も古くて身近なのがhalf-duplex pipe：

![Parsing Commandline](https://www.dropbox.com/s/4qjs6g1h6888g6x/half_duplexpipe.png?raw=1)

古典的なhalf-duplex pipeはデータが一方向にのみ流れる。

pipeは共通の親を持つプロセス間のみでしか使えない。pipe自体はkernel側に存在するので、作成元のプロセスか、そこから継承したプロセスでないと、そのpipeのありかを指すすべがない。（名前ありのFIFOSなら可能）

そのため通常親プロセスがpipeを作ってからforkし、親と子の間でIPCチャネルを作る：

![Parsing Commandline](https://www.dropbox.com/s/3d1s800487i7jbe/pipe_fork.png?raw=1)

forkしてからどうするかは、どっち方向にデータが流れてほしいかによる。親がwriteしたものを子にreadさせたい場合は、親がpipeのread end (fd[0])をcloseし、子がpipeのwrite end (fd[1])をcloseする。

pipeのルール：

1. 全てのwrite endがcloseされたらreadはEOFを返す。pipe fdはストリーム側のfdなので、空のpipeをreadしようとするとブロックする。バッファが満タンのpipeにwriteしようとするとブロックする。
2. read endが全てcloseされたpipeにwriteしようとすると SIGPIPE signalが発生する。それを無視したりsignal handlerからすぐreturnするとwriteは-1を返し、errnoの値がEPIPEになる


#### パイプラインの実装：

![Parsing Commandline](https://www.dropbox.com/s/h9jvf02g804tob1/pipeline.png?raw=1)


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
        close(pipefd[0]);
        execvp(cmd2, argv2);
    }
    assert(child2 > 0);

    close(pipefd[0]);
    close(pipefd[1]);
    r = waitpid(child1, &status, 0);
    r = waitpid(child2, &status, 0);
}
```


### リダイレクション




## 参考

https://brennan.io/2015/01/16/write-a-shell-in-c/

https://indradhanush.github.io/blog/writing-a-unix-shell-part-1/

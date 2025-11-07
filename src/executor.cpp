#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "executor.hpp"

SimpleCommand::~SimpleCommand()
{
    delete name;
    while (args) {
        Argument *tmp = args;
        args = args->next;
        delete tmp;
    }
}

void SimpleCommand::Execute() const
{
    char **argv = new char*[len + 2];
    argv[0] = const_cast<char *>(name->GetLexeme());
    argv[len + 1] = 0;

    unsigned int i = 1;
    for (Argument *p = args; p; p = p->next) {
        argv[i] = const_cast<char *>(p->arg->GetLexeme());
        i++;
    }
    execvp(name->GetLexeme(), argv);
    perror(name->GetLexeme());
    exit(1);
}

Pipeline::~Pipeline()
{
    while (commands) {
        Item *tmp = commands;
        commands = commands->next;
        delete tmp;
    }
}

int Pipeline::ExecuteSimplestCase() const
{
    int pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    if (pid == 0) {
        commands->cmd->Execute();
    }
    return 0;
}

int Pipeline::ExecuteFirstProcess(SimpleCommand *cmd,
        int fdin, int fdout) const
{
    int pid = fork();
    if (pid == -1) {
        close(fdin);
        close(fdout);
        perror("fork");
        return -1;
    }
    if (pid == 0) {
        close(fdin);
        dup2(fdout, 1);
        close(fdout);
        cmd->Execute();
    }
    close(fdout);
    return 0;
}

int Pipeline::ExecuteMiddleProcess(SimpleCommand *cmd, int fd,
        int fdin, int fdout) const
{
    int pid = fork();
    if (pid == -1) {
        close(fdin);
        close(fdout);
        close(fd);
        perror("fork");
        return -1;
    }
    if (pid == 0) {
        close(fdin);
        dup2(fd, 0);
        close(fd);
        dup2(fdout, 1);
        close(fdout);
        cmd->Execute();
    }
    close(fd);
    close(fdout);
    return 0;
}

int Pipeline::ExecuteLastProcess(SimpleCommand *cmd, int fdin) const
{
    int pid = fork();
    if (pid == -1) {
        close(fdin);
        perror("fork");
        return -1;
    }
    if (pid == 0) {
        dup2(fdin, 0);
        close(fdin);
        cmd->Execute();
    }
    close(fdin);
    return 0;
}

int Pipeline::ExecuteCompositeCase() const
{
    Item *cur = commands;

    int fds[2];
    pipe(fds);

    int res = ExecuteFirstProcess(cur->cmd, fds[0], fds[1]);
    if (res == -1) {
        return -1;
    }

    int fd = fds[0];
    cur = cur->next;
    for (unsigned int i = 1; i < len - 1; i++) {
        pipe(fds);
        int res = ExecuteMiddleProcess(cur->cmd, fd, fds[0], fds[1]);
        if (res == -1) {
            return -1;
        }
        fd = fds[0];
        cur = cur->next;
    }
    return ExecuteLastProcess(cur->cmd, fd);
}

void Pipeline::Wait() const
{
    int pid;
    do {
        pid = wait(0);
    } while (pid != -1);
}

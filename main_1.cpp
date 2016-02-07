
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <string>
#include <sstream>
#include <vector>

#include <string.h>

#ifdef BATLE
#define LOG_PATH  "/home/box/log.txt"
#define OUT_PATH  "/home/box/result.out"
#else
#define LOG_PATH  "log.txt"
#define OUT_PATH  "result.out"
#endif

std::vector<std::string> split(const std::string &s,  char delim) {
    std::stringstream ss(s);
    std::vector<std::string> elems;
    std::string e;
    while(std::getline(ss, e, delim)) {
        if (not e.empty()){
            elems.push_back(e);
        }
    }
    return elems;
}

long replace(std::string& str, const std::string& from, const std::string& to, size_t start = 0, long count = -1) {
    if (from.empty()) return 0;

    size_t startpos = str.find(from, start);
    long replaceCount = 0;

    while (startpos != std::string::npos){
        str.replace(startpos, from.length(), to);
        startpos += to.length();
        replaceCount++;

        if (count > 0 && replaceCount >= count) break;
        startpos = str.find(from, startpos);
    }

    return replaceCount;
}

void exec_command (const std::string &command){

    auto c = split(command, ' ');
    std::vector< char* > argv;
    for(auto& a : c)
        argv.push_back((char*)a.c_str());
    argv.push_back(0);

    execvp (c[0].c_str(),argv.data());

}

void exec_last_command (const std::string &command){
    close (STDOUT_FILENO);
    auto f = open (OUT_PATH, O_CREAT|O_WRONLY, 0666);
    dup2(f,STDOUT_FILENO);
    exec_command(command);
    close(STDOUT_FILENO);
}

void go2 (const std::vector<std::string> &comands, const int deep){

    int pfd[2];
    pipe(pfd);

     if(!fork()){
        close(STDOUT_FILENO);
        dup2(pfd[1],STDOUT_FILENO);
        close(pfd[1]);
        close(pfd[0]);

        exec_command(comands[deep]);
    }
    else{
        close(STDIN_FILENO);
        dup2(pfd[0],STDIN_FILENO);
        close(pfd[1]);
        close(pfd[0]);

        if (deep+2 < comands.size())
            go2(comands, deep+1);
        else
        exec_last_command(comands[deep+1]);
    }

    if (deep != 0)
        exit (0);
}


int main (int, char**){

    char in[1024];
    fgets(in, 1024, stdin);

    auto f = open (LOG_PATH, O_CREAT|O_WRONLY|O_APPEND);
    write (f, in, strnlen(in, 1024));
    close(f);

    auto commands = split(std::string(in), '|');
    for  (auto& c : commands)
        replace (c,"\n","");

    if (commands.size() > 1)
        go2 (commands, 0);
    else if (commands.size())
        exec_last_command(commands[0]);

    sleep(1);
    return 0;
}

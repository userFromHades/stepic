

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>


#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> split(const std::string &s,  char delim) {
    std::stringstream ss(s);
    std::vector<std::string> elems;
    std::string e;
    while(std::getline(ss, e, delim)) {
        if (not e.empty())
            elems.push_back(e);
    }
    return elems;
}

#include <unistd.h>
void go (){
    int pfd[2];
    pipe(pfd);

    if(!fork()){
        close(STDOUT_FILENO);
        dup2(pfd[1],STDOUT_FILENO);
        close(pfd[1]);
        close(pfd[0]);




        char* pp[] = {"who", 0};

        execvp ("who",pp);

    }
    else{
        close(STDIN_FILENO);
        dup2(pfd[0],STDIN_FILENO);
        close(pfd[1]);
        close(pfd[0]);
        execlp ("sort","sort",NULL);
    }
}

void exec_command (const std::string &command){

    auto c = split(command, ' ');
    std::vector< char* > argv;
    for(auto& a : c)
        argv.push_back((char*)a.c_str());

    execvp (c[0].c_str(),argv.data());

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
        else {
            close (STDOUT_FILENO);
            auto f = open ("/home/box/result.out", O_CREAT|O_WRONLY);
            dup2(f,STDOUT_FILENO);
            close(f);
            exec_command(comands[deep+1]);
        }
    }

    if (deep != 0)
        exit (0);

}


int main (int, char**){

    //while (1){
        char in[1024];
        fgets(in, 1024, stdin);

        auto commands = split(std::string(in), '|');
         go2 (commands, 0);
    //}


    //auto commands = split(std::string("who | sort | uniq -c | sort -nk1"), '|');
    //go2 (commands, 0);

    return 0;
}

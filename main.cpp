#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main (int, char**){
    //mkfifo ("home/box/out.fifo",0666);
    //mkfifo ("home/box/in.fifo",0666);

    auto in = open("home/box/in.fifo",O_RDONLY|O_NONBLOCK);
    auto out= open("home/box/out.fifo",O_WRONLY);

    printf ("start %i %i\n", in, out);

    char buff[10*1024];
    while(1){

        auto total_read = read(in,buff,10*1024);
        if(total_read > 0){
            auto wr = write(out,buff,total_read);
            fwrite(buff,total_read,1,stdout);
            printf("|w %i |\n",wr);
            fflush(stdout);
        }
    }
    return 0;
}

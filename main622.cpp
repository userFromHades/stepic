
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
 #include <mqueue.h>

struct message {
    long mtype;
    char mtext[80];
};

int main (int, char**){



    mqd_t q =  mq_open ("/test.mq", O_CREAT|O_RDWR |0666);


    char buff[1024];
    auto r = mq_receive (q,buff,1024,0);


    auto f =open ("/home/box/message.txt",O_CREAT|O_WRONLY, 0666);
    write (f, buff, r);
    close (f);


    mq_close (q);
    return 0;
}

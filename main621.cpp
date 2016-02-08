
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

struct message {
    long mtype;
    char mtext[80];
};

int main (int, char**){

    auto key = ftok("/tmp/msg.temp", 1);
    auto id = msgget(key, IPC_CREAT | 0777);

    printf("key %i id %i \n",key,id);
    //msqid_ds data;
    //msgctl (id,IPC_STAT,&data);
    message msg;
    auto res =  msgrcv (id,&msg,80,0,0);
    printf("%i\n",res);

    auto f =open ("/home/box/message.txt",O_CREAT|O_WRONLY, 0666);
    write (f, msg.mtext, 80);
    close (f);


    msgctl (id,IPC_RMID,0);
    return 0;
}

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
 
void *thread_isr(void *p)
{
    char buf[100];
    int fd;
    fd=open("/dev/int1",O_RDONLY);
 
    while(1)
    {
        read(fd,buf,1);
 
        printf("Interrupt handler\n");
    }
 
}
 
int main()
{
    pthread_t t1;
    puts("start");
    pthread_create(&t1,NULL,thread_isr,NULL);
    sleep(100);
    return 1;
}

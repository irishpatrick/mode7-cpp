#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
//#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include "monitor.h"

void update()
{

}

void draw()
{

}

void deallocate(void** ptr, unsigned long mmap_size)
{
    int ret = munmap(*ptr, mmap_size);
    if (ret == -1)
    {
        fprintf(stderr, "dealloc error\n");
        exit(0);
    }
    *ptr = NULL;
}

int main(int argc, char** argv)
{
    int map_size = 
            4 * sizeof(int);
    
    void* map = mmap(
        NULL,
        map_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        0, 0
    );
    memset(map, 0, map_size);

    int* command = (int*)map;
    command[2] = 1;
    command[3] = 1;

    int pid = fork();
    if (pid == 0)
    {
        char buffer[100];
        while (command[3] > 0)
        {
            printf("> ");
            gets(buffer);            
            if (buffer[0] == 0)
            {
                continue;
            }

            printf("echo %s\n", buffer);

            if (!strcmp(buffer, "exit"))
            {
                break;
            }
        
            memset(buffer, 0, 100);
        }
        
        wait(NULL);
    }
    else
    {
        monitor_init(768, 768);
        while (!monitor_should_quit())
        {
            monitor_clear();
            monitor_update();
            update();
            draw();
            monitor_draw();
        }
        monitor_destroy();

        wait(NULL);
    }
    
    deallocate((void**)&command, map_size);
    
    return 0;
}
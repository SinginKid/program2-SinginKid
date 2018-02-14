#include "pcb.h"
#include "QueueArray.h"
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define READ_END 0
#define WRITE_END 1
#define PRIORITY_LEVELS 4

void newProcess(int pid, int val, int run_time);
void block(int rid);
void unblock(int rid);
void timeIncr();
void change(char cmd, int num);
void print();
void turnaround();

main(int argc, char *argv[]){
    cout << "PMAN: I am in the main and running!" << endl;
    //variable to represent time in the code, always starts at 0
    //when time in incremented, we update pcb.
    int time = 0;
    //table of pcb's to be managed by the process scheduler
    vector<pcb> pcb_table[100];
    //queueArray that acts as the ReadyState, list of queues
    //that hold processes ready to execute.
    QueueArray <int> ready(PRIORITY_LEVELS);
    //queueArray that acts as the BlockedState, holds processes
    //that are using resources, when finished, it places the processes
    //back into ReadyState.
    QueueArray <int> blocked(PRIORITY_LEVELS);
    //a pointer to an index in the pcb_table, holds the process being
    //ran.
    int * running;
    int mcpipe[2];

    mcpipe[READ_END] = atoi(argv[1]);
    mcpipe[WRITE_END] = atoi(argv[2]);

    char letter, cmd;
    string input;
    int check, pid, value, run_time, rid;

    close(mcpipe[WRITE_END]);

    do{
      check = read(mcpipe[READ_END], &letter, sizeof(char));
      cout << "PMAN: I just read " << letter << endl;
      if(check == 0)
      {
        cout << "PMAN: I finished reading! Now exiting..." << endl;
        return 0;
      }
      else if(check == -1)
      {
        cout << "PMAN: Encountered Problem reading!" << endl;
      }
      switch(letter)
      {
        case 'S':
        read(mcpipe[READ_END], &pid, sizeof(int));
        read(mcpipe[READ_END], &value, sizeof(int));
        read(mcpipe[READ_END], &run_time, sizeof(int));
        newProcess(pid, value, run_time);
        break;
        case 'B':
        read(mcpipe[READ_END], &rid, sizeof(int));
        block(rid);
        break;
        case 'U':
        read(mcpipe[READ_END], &rid, sizeof(int));
        unblock(rid);
        break;
        case 'Q':
        timeIncr();
        break;
        case 'C':
        read(mcpipe[READ_END], &cmd, sizeof(char));
        read(mcpipe[READ_END], &value, sizeof(int));
        change(cmd, value);
        break;
        case 'P':
        print();
        break;
        case 'T':
        turnaround();
        break;
      }
    }while(letter != 'T');

    close(mcpipe[READ_END]);
}
void newProcess(int pid, int val, int run_time)
{
    return;
}
void block(int rid)
{
    return;
}
void unblock(int rid)
{
    return;
}
void timeIncr()
{
    return;
}
void change(char cmd, int num)
{
    switch(cmd)
    {
      case'A':
      break;
      case'S':
      break;
      case'M':
      break;
      case'D':
      break;
    }
    return;
}
void print()
{
    return;
}
void turnaround()
{
    return;
}

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
#include <queue>
#include <iomanip>

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

int time1 = 0;
int processesFinished = 0;
//table of pcb's to be managed by the process scheduler
pcb pcb_table[100];
//queueArray that acts as the ReadyState, list of queues
//that hold processes ready to execute.
QueueArray <int> ready(PRIORITY_LEVELS);
//queueArray that acts as the BlockedState, holds processes
//that are using resources, when finished, it places the processes
//back into ReadyState.int * running;
QueueArray <int> r0(PRIORITY_LEVELS);
QueueArray <int> r1(PRIORITY_LEVELS);
QueueArray <int> r2(PRIORITY_LEVELS);

//a pointer to an index in the pcb_table, holds the process being
//ran.
pcb * running = NULL;

main(int argc, char *argv[]){
  cout << "PMAN: I am in the main and running!" << endl;
  int mcpipe[2];

  mcpipe[READ_END] = atoi(argv[1]);
  mcpipe[WRITE_END] = atoi(argv[2]);

  char letter, cmd;
  string input;
  int check, pid, value, run_time, rid;

  close(mcpipe[WRITE_END]);
  cout <<"PMAN: running\n";
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
      timeIncr();
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
  pcb_table[pid].create(pid, val, run_time);
  if(running == NULL)
  {
    running = &pcb_table[pid];
  }
  ready.Enqueue(pid, 0);
  return;
}
void block(int rid)
{
  if(rid == 0)
  {
    r0.Enqueue(running->get_pID(), running->get_priority());
  }
  if(rid == 1)
  {
    r1.Enqueue(running->get_pID(), running->get_priority());
  }
  if(rid == 2)
  {
    r2.Enqueue(running->get_pID(), running->get_priority());
  }
  if(running -> get_priority() != 0)
  {
    running -> set_priority((running -> get_priority()) - 1);
  }
  int check = ready.Dequeue();
  if(check == 0)
  {
    running = NULL;
  }
  else
  {
    running = &pcb_table[check];
  }
}
void unblock(int rid)
{
  int check;
  if(rid == 0)
  {
    check = r0.Dequeue();
    ready.Enqueue(check, pcb_table[check].get_priority());
  }
  else if(rid == 1)
  {
    check = r1.Dequeue();
    ready.Enqueue(check, pcb_table[check].get_priority());
  }
  else if(rid == 2)
  {
    check = r2.Dequeue();
    ready.Enqueue(check, pcb_table[check].get_priority());
  }
  if(running == NULL)
  {
    running = &pcb_table[ready.Dequeue()];
  }
}
void timeIncr()
{
  time1++;
  running->incr_ct();
  if(running->get_priority() == 0 && running -> get_ct() == 1)
  {
    running -> set_priority(running -> get_priority() + 1);
    ready.Enqueue(running -> get_pID(), running -> get_priority());
    running = &pcb_table[ready.Dequeue()];
  }
  else if(running -> get_priority() == 1 && running -> get_ct() == 2)
  {
    running -> set_priority(running -> get_priority() + 1);
    ready.Enqueue(running -> get_pID(), running -> get_priority());
    running = &pcb_table[ready.Dequeue()];
  }
  else if(running -> get_priority() == 2 && running -> get_ct() == 4)
  {
    running -> set_priority(running -> get_priority() + 1);
    ready.Enqueue(running -> get_pID(), running -> get_priority());
    running = &pcb_table[ready.Dequeue()];
  }
  else if(running -> get_priority() == 3 && running -> get_ct() == 8)
  {
    ready.Enqueue(running -> get_pID(), running -> get_priority());
    running = &pcb_table[ready.Dequeue()];
  }
}
void change(char cmd, int num)
{
  if(running != NULL)
  {
    switch(cmd)
    {
      case'A':
      running -> set_value((running -> get_value()) + num);
      break;
      case'S':
      running -> set_value((running -> get_value()) - num);
      break;
      case'M':
      running -> set_value((running -> get_value()) * num);
      break;
      case'D':
      running -> set_value((running -> get_value()) / num);
      break;
    }
    int check = ready.Dequeue();
    if(check == 0)
    {
      running = NULL;
    }
    else
    {
      running = &pcb_table[check];
    }
    processesFinished++;
  }
  return;
}
void print()
{
  int* help;
  cout << "****************************************************************" << endl;
  cout << " The current system state is as follows:" << endl;
  cout << "****************************************************************" << endl << endl;
  cout << "CURRENT TIME: " << time1 << endl << endl;
  cout << "RUNNING PROCESSES:" << endl;
  if(running != NULL)
  {
    cout << "PID  Priority Value  Start Time  Total CPU time" << endl;
    cout << setw(2) << running -> get_pID() << setw(5) << running -> get_priority() << setw(12)
    << running -> get_value() << setw(9) << running -> get_st() << setw(14) << running -> get_ct() << endl << endl;
  }
  else
  {
    cout << "There is no running process" << endl << endl;
  }
  cout << "BLOCKED PROCESS:" << endl;
  if(r0.Qsize(0) == 0 && r0.Qsize(1) == 0 && r0.Qsize(2) && r0.Qsize(3) == 0)
  {
    cout << "Queue of processes Blocked for resource 0 is empty" << endl;
  }
  else
  {
    cout << "Queue of processes Blocked for resource 0:" << endl;
    cout << "PID  Priority Value  Start Time  Total CPU time" << endl;
    for(int h = 0; h < PRIORITY_LEVELS; h++){
      if(r0.Qsize(h)!= 0)
      {
        help = r0.Qstate(h);
        for(int i = 0; i < r0.Qsize(h); i++)
        {
          cout << setw(2) << pcb_table[help[i]].get_pID() << setw(5) << pcb_table[help[i]].get_value()
          << setw(12) << pcb_table[help[i]].get_priority() << setw(9) << pcb_table[help[i]].get_st()
          << setw(14) << pcb_table[help[i]].get_ct() << endl << endl;
        }
      }
    }
  }
  if(r1.Qsize(0) == 0 && r1.Qsize(1) == 0 && r1.Qsize(2) && r1.Qsize(3) == 0)
  {
    cout << "Queue of processes Blocked for resource 1 is empty" << endl;
  }
  else
  {
    cout << "Queue of processes Blocked for resource 1:" << endl;
    cout << "PID  Priority Value  Start Time  Total CPU time" << endl;
    for(int h = 0; h < PRIORITY_LEVELS; h++){
      if(r1.Qsize(h)!= 0)
      {
        help = r1.Qstate(h);
        for(int i = 0; i < r0.Qsize(h); i++)
        {
          cout << setw(2) << pcb_table[help[i]].get_pID() << setw(5) << pcb_table[help[i]].get_value()
          << setw(12) << pcb_table[help[i]].get_priority() << setw(9) << pcb_table[help[i]].get_st()
          << setw(14) << pcb_table[help[i]].get_ct() << endl << endl;
        }
      }
    }
  }
  if(r2.Qsize(0) == 0 && r2.Qsize(1) == 0 && r2.Qsize(2) && r2.Qsize(3) == 0)
  {
    cout << "Queue of processes Blocked for resource 2 is empty" << endl;
  }
  else
  {
    cout << "Queue of processes Blocked for resource 2:" << endl;
    cout << "PID  Priority Value  Start Time  Total CPU time" << endl;
    for(int h = 0; h < PRIORITY_LEVELS; h++){
      if(r2.Qsize(h)!= 0)
      {
        help = r2.Qstate(h);
        for(int i = 0; i < r2.Qsize(h); i++)
        {
          cout << setw(2) << pcb_table[help[i]].get_pID() << setw(5) << pcb_table[help[i]].get_value()
          << setw(12) << pcb_table[help[i]].get_priority() << setw(9) << pcb_table[help[i]].get_st()
          << setw(14) << pcb_table[help[i]].get_ct() << endl << endl;
        }
      }
    }
  }
  cout << "PROCESSES READY TO RUN:" << endl;
  for(int h = 0; h < PRIORITY_LEVELS; h++){
    if(ready.Qsize(h)!= 0)
    {
      help = ready.Qstate(h);
      cout << "Queue of processes with priority " << h << " :" << endl;
      for(int i = 0; i < ready.Qsize(h); i++)
      {
        cout << "PID  Priority Value  Start Time  Total CPU time" << endl;
        cout << setw(2) << pcb_table[help[i]].get_pID() << setw(5) << pcb_table[help[i]].get_value()
        << setw(12) << pcb_table[help[i]].get_priority() << setw(9) << pcb_table[help[i]].get_st()
        << setw(14) << pcb_table[help[i]].get_ct() << endl << endl;
      }
    }
    else
    {
      cout << "Queue of processes with priority " << h << " is empty" << endl;
    }
  }
    cout << "****************************************************************" << endl << endl << endl;
}
void turnaround()
{
  return;
}

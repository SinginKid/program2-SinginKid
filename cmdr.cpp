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
#define BUFFER_SIZE 25

main()
{
  int max = 100, i, pid;
  //Will be the pipe for communication to scheduler.cpp
  int mcpipe[2], status, num;
  char mc0[10], mc1[10];
  char read_buffer[BUFFER_SIZE];
  char test;

  if (pid = pipe(mcpipe)) {
    perror("CMD: Unable to make pipe");
    return 1;
  }

  sprintf(mc0, "%d", mcpipe[0]);
  sprintf(mc1, "%d", mcpipe[1]);

  if ((pid = fork()) == -1)
  {
    perror("CMD: I cant fork!");
    return 1;
  }
  else if(pid > 0)
  {
    //close the read end of the pipe
    close(mcpipe[READ_END]);
    do{
      sleep(1);
      cin >> test;
      //cout << "CMDR READ: " << test << endl;
      switch(test)
      {
        case 'S':
        write(mcpipe[WRITE_END], &test, sizeof(char));
        for(int i = 0; i < 3; i++)
        {
          cin >> num;
          write(mcpipe[WRITE_END], &num, sizeof(int));
        }
        break;
        case 'B':
        write(mcpipe[WRITE_END], &test, sizeof(char));
        cin >> num;
        write(mcpipe[WRITE_END], &num, sizeof(int));
        break;
        case 'U':
        write(mcpipe[WRITE_END], &test, sizeof(char));
        cin >> num;
        write(mcpipe[WRITE_END], &num, sizeof(int));
        break;
        case 'Q':
        write(mcpipe[WRITE_END], &test, sizeof(char));
        break;
        case 'C':
        write(mcpipe[WRITE_END], &test, sizeof(char));
        cin >> test;
        write(mcpipe[WRITE_END], &test, sizeof(char));
        cin >> num;
        write(mcpipe[WRITE_END], &num, sizeof(int));
        break;
        case 'P':
        write(mcpipe[WRITE_END], &test, sizeof(char));
        break;
        case 'T':
        write(mcpipe[WRITE_END], &test, sizeof(char));
        break;
        default:
        cout << "CMD: Switch: You hit default, a no go, I got: " << test << endl;
      }
    }while(test != 'T');
    close(mcpipe[WRITE_END]);
    wait(&status);
  }
  else if(pid == 0)
  {
    execl("scheduler", "scheduler", mc0, mc1, NULL);
  }
}

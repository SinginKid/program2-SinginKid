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
  cout << "I am currently running" << endl;
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
    cout << "This is the Parent" << endl;
    cout << "Please enter commands: " << endl;
    //close the read end of the pipe
    close(mcpipe[READ_END]);
    do{
      sleep(2);
      cin >> test;
      switch(test)
      {
        case 'S':
        cout << "CMDR: Writing case S" << endl;
        write(mcpipe[WRITE_END], &test, sizeof(char));
        cout <<"CMDR: sent\n";
        for(int i = 0; i < 3; i++)
        {
          cout <<"CMDR: i = " <<i<<endl;
          cin >> num;
          cout << "Im piping: " << num << endl;
          write(mcpipe[WRITE_END], &num, sizeof(int));
        }
        break;
        case 'B':
        cout << "Writing case B" << endl;
        write(mcpipe[WRITE_END], &test, sizeof(char));
        cin >> num;
        cout << "Im piping: " << num << endl;
        write(mcpipe[WRITE_END], &num, sizeof(int));
        break;
        case 'U':
        cout << "Writing case U" << endl;
        write(mcpipe[WRITE_END], &test, sizeof(char));
        cin >> num;
        cout << "Im piping: " << num << endl;
        write(mcpipe[WRITE_END], &num, sizeof(int));
        break;
        case 'Q':
        cout << "Writing case Q" << endl;
        write(mcpipe[WRITE_END], &test, sizeof(char));
        break;
        case 'C':
        cout << "Writing case C" << endl;
        write(mcpipe[WRITE_END], &test, sizeof(char));
          cin >> test;
          cout << "Im piping: " << test << endl;
          write(mcpipe[WRITE_END], &test, sizeof(char));
        break;
        case 'P':
        cout << "Writing case P" << endl;
        write(mcpipe[WRITE_END], &test, sizeof(char));
        break;
        case 'T':
        cout << "Writing case T" << endl;
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
    cout << "I am the child!" << endl;
    execl("scheduler", "scheduler", mc0, mc1, NULL);
  }
}

#ifndef PCB_H
#define PCB_H
class pcb
{
private:
  int pID;
  int priority;
  int value;
  int start_time; //Time when process was created
  int run_time;
  int cpu_time; //Time on processer
public:
  pcb();
  ~pcb();
  int get_pID();
  int get_priority();
  int get_value();
  int get_st();
  int get_rt();
  int get_ct();
  void set_priority(int num);
  void set_value(int num);
  void incr_rt();
  void incr_ct();
  void create(int pid, int val, int rt, int time1);
};
pcb::pcb()
{
  pID=-1;
  priority=0;
}
pcb::~pcb(){

}
int pcb::get_pID()
{
  return pID;
}
int pcb::get_priority()
{
  return priority;
}
int pcb::get_value()
{
  return value;
}
int pcb::get_st()
{
  return start_time;
}
int pcb::get_rt()
{
  return run_time;
}
int pcb::get_ct()
{
  return cpu_time;
}
void pcb::set_value(int num)
{
  value = num;
}
void pcb::set_priority(int num)
{
  priority = num;
}
void pcb::incr_rt()
{
  run_time++;
}
void pcb::incr_ct()
{
  cpu_time++;
}
void pcb::create(int pid, int val, int rt, int time1)
{
  pID = pid;
  priority = 0;
  value = val;
  cpu_time = 0;
  run_time = rt;
  start_time = time1;
}
#endif

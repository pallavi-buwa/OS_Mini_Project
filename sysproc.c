#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return myproc()->pid;
}

// TO DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
struct shmid_ds
{
  struct ipc_perm shm_perm; /* Ownership and permissions */
  uint shm_size;         /* Size of segment (bytes) */
  void* addr;            //start address
  int shmid;
  int key;          /* Key supplied to shmget(2) */
} shms[SHMT];

struct ipc_perm
{
  unsigned short mode;  /* Permissions + SHM_DEST and SHM_LOCKED flags */
};

void 
init_shm(){
  for(int i = 0; i < SHMT; i++){
    shms->addr = 0;
    shms->shmid = i;
  }
}

int sys_shmget(int key, int size, int shmflg){
  //check if mapping with key already exists
  PGROUNDUP(size);
  int newsz = 0;
  pde_t * pgdir;
  newsz = allocshmuvm(pgdir, 0, size);
  return newsz - size;
}

void* sys_shmat(int shmid, const void *shmaddr, int shmflg){

}

int sys_shmdt(const void *shmaddr){

}

int sys_shmctl(int shmid, int cmd, struct shmid_ds *buf){

}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

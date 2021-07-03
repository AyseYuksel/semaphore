#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> //posix semaphore library
#include <sys/wait.h>

const char *semName="any_name"; //semaphore name

void parent(void){
//sem_open using for opening a semaphore 
//sem_open parameters =>
// semaphore name,flags,permission,semaphore initial value 	
sem_t *sem_id=sem_open(semName,0_CREAT,0600,0);

if(sem_id==SEM_FAILED){
perror("Parent : [sem_open] failed \n");
return;
}

printf("Parent : Wait for child process to print\n");
//sem_wait using for the process to wait
if(sem_wait(sem_id)<0){
printf("Parent : [sem_wait] failed \n");
return;
}

printf("Parent : Child printed \n");

if(sem_close(sem_id)!=0){
perror("Parent : [sem_close] failed \n");
return;
}

//sem_unlink using for close the link and destroy the semaphore 
if(sem_unlink(semName)<0){
printf("Parent: [sem_unlink] failed\n ");
return;
}

}
void child(void){
//child process create semaphore with the same semaphore name 	
sem_t *sem_id=sem_open(semName,O_CREATE,0600,0);

if(sem_id==SEM_FAILED){
perror("Child : [sem_open] failed \n");
return;
}

printf("Child : I am done ! Release semaphore \n");

//child process use the field of semaphore and increment semaphore value than
//main(parent) process will be use the field of semaphore again.

//in the beginning child use sem_open function and this function has the 
// 0 value for semaphore initial value. if this value=0 it means semaphore can 
// use current process. with sem_post() method , this semaphore value increment //and current value will be 1. if current value is 1 it means semaphore field is// locked. if current process use sem_wait() method than
// semaphore value will be 0 again and will be using again.

if(sem_post(sem_id)<0){ 
printf("Child : [sem_post] failed.\n");
}

}

int main(int argc,char *argv[]){
pid_t pid;
pid=fork();

if(pid<0){
perror("fork");
exit(EXIT_FAILURE);
}

if(!pid){ //pid==0 -> child process
child();
printf("Child : Done with sem_open \n");
}else{ //pid==1 -> main(parent) process
int status;
parent();
wait(&status);
printf("Parent : Done with sem_open \n");
}
return 0;

//some posix semaphone methods 
//sem_open, sem_unlink , sem_close , sem_getvalue
//sem_wait => if semaphore using with a process than it blocked 
//sem_post => semaphore value is increment and it will be unblocked

}

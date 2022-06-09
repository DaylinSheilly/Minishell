#include "leercadena.h"
#include <assert.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  char command[256];
  char **comando;
  
  char *tasks[100];
  int whichTask = 0;
  int inBackGround = 0;
  int pids[100];
  
  while(1)
  {
    int i;
    printf("> ");
    leer_de_teclado(256, command);
    
    if(strcmp(command, "salir") == 0)
    {
      kill(0, SIGKILL);
    }
    
    comando = de_cadena_a_vector(command);
    i = 0;
    
    while(comando[i])
    {
      i++;
    }
    pid_t wpid;
    
    inBackGround = 0;
    
    if(strcmp(comando[i-1], "&") == 0)
    {
      inBackGround = 1;
      comando[i - 1] = NULL;
      
      tasks[whichTask] = (char*)malloc(sizeof(char)*257);
      strcpy(tasks[whichTask],command);
    }
    
    if(strcmp(command, "tareas") == 0)
    {
      if(tasks[0])
      {
        for(int y=0;y<whichTask;y++)
        {
          printf("[%i]", pids[y]);
          printf(" %s\n", tasks[y]);
        }
      }
    }
    
    i = 0;

    int x;
    wpid = fork();
    
    if(strcmp(comando[0], "detener") == 0)
    {
      int pidToDetener = atoi(comando[1]);
      kill(pidToDetener,SIGKILL);
      for(int s=0;s<whichTask;s++)
      {
        if(pids[s]==pidToDetener)
        {
          printf("<< deteniendo proceso >>");
		      for(int contador = s; contador < whichTask; contador++)
          { 
			      strcpy(tasks[contador], tasks[contador + 1]);
			      pids[contador] = pids[contador + 1];
          }
          break;
        }
      }
      printf("\n<< proceso detenido >>");
    }
    
    assert(wpid >= 0);
    
    if(wpid==0) //proceso Hijo
    {
      execvp(comando[0], comando);
      printf("\n:D ");
    }
    else
    {
      if(inBackGround==0)
      {
        waitpid(wpid, &x, 0);
        printf(":D ");
      }
      else
      {
        pids[whichTask] = wpid;
        whichTask++;
        printf("Ejecución en segundo plano\n");
        printf(":D ");
      }
    }
  }
  return 0;
}
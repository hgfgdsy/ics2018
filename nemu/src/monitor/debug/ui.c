#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Operating N lines", cmd_si },
  { "info", "Printing the situation of the program", cmd_info },
  { "x", "Printing the date in the memory", cmd_x },
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_x(char *args) {
	int k=0;
	char *arg=args;
	while(*arg!=' ')
	{k++;
		arg++;
	}
	int n=0;
        while(k--) {
	n=n*10+(*args-'0');
	args++;
	};
	int temp=n;
        args=args+3;
        int t=strlen(args);
        int p=0;
	while(t--) {
	if('0'<=*args&&*args<='9')
	  p=p*16+(*args-'0');
	else if('a'<=*args&&*args<='f')
	  p=p*16+(*args-'a');
        else if('A'<=*args&&*args<='F')
	  p=p*16+(*args-'A');
        args++;}
        for(int i=0;i<temp;i++) {
	  printf("0x");
	  for(int j=p+3;j>=p;j--)
		  printf("%x",pmem[j]);
	  printf("\n");
	  p+=4;
	}
	return 0;
}
static int cmd_si(char *args) {
	int n=0;
       if(args==NULL) {
	       n=1;}
       else {
	       int t=strlen(args);
	while(t--) {
          n=n*10+(*args-'0');
	  args++;
	 }
       }	
            cpu_exec(n);
	return 0;
}

static int cmd_info(char *args) {
	if(*args=='r') {
		printf("eax:0x%x\t%d\n",cpu.eax,cpu.eax);
		printf("ecx:0x%x\t%d\n",cpu.ecx,cpu.ecx);
		printf("edx:0x%x\t%d\n",cpu.edx,cpu.edx);
		printf("ebx:0x%x\t%d\n",cpu.ebx,cpu.ebx);
		printf("esp:0x%x\t%d\n",cpu.esp,cpu.esp);
		printf("ebp:0x%x\t%d\n",cpu.ebp,cpu.ebp);
		printf("esi:0x%x\t%d\n",cpu.esi,cpu.esi);
		printf("edi:0x%x\t%d\n",cpu.edi,cpu.edi);
		printf("eip:0x%x\t%d\n",cpu.eip,cpu.eip);
	}
	return 0;
}
static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;
 
  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

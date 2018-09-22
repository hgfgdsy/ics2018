#include<stdio.h>
#include<monitor/expr.h>
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);
  char buff[65535];
  FILE *fp=NULL;
  fp=fopen( " ~/ics2018/nemu/tools/gen-expr/input", "r");
  int t=5;
  while(t--){
	  fgets(buff,65535,(FILE*)fp);
	  bool k=1;
	  int z=expr(buff,&k);
	  if(!k){
		  printf("URE_E!\n");
	  }
	  else
		  printf("%d\n",z);
  }
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static unsigned gen_num(){
       return rand()%4294967296 +0;}
static int choose(){
       return rand()%3 +0;}
static int gen_rand_op(){
       int temp=rand()4 +1;
       switch(temp){
	       case 1: return '+';
	       case 2: return '-';
	       case 3: return '*';
	       case 4: return '/';
	       default: return ' ';}
}	
static char buf[65536];
static inline void gen_rand_expr() {
  buf[0] = '\0';
  int label;
  switch(choose()){
	  case 0:buf[
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen(".code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc .code.c -o .expr");
    if (ret != 0) continue;

    fp = popen("./.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}

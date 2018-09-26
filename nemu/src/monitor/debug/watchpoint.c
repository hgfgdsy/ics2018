#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
extern  WP* new_wp(){
	if(free_==NULL){
		assert(0);}
	else{
		WP* p=free_;
		free_=free_->next;
		p->next=head;
		head=p;
		return p;}
}

extern void free_wp(WP* wp){
	WP* p=head;
	WP* q=NULL;
	while(p!=NULL&&p!=wp){
		q=p;
		p=p->next;
	}
	if(p==NULL){
		printf("No such WP!");}
	else
	{
		if(q!=NULL){
		q->next=p->next;
	        p->next=free_;
	        free_=p;}
		else{
			head=NULL;
			p->next=free_;
			free_=p;}
		}
}


extern void iw(){
	WP* p=head;
	if(p==NULL){
		printf("No watchpoint!!!\n");
	}
	else{
	while(p!=NULL){
		printf("NO:%d\texpr=%s\nOld value=%d\nNew value=%d\n",p->NO,p->STR,p->LAST,p->NEWV);p=p->next;}}}




extern void ide(int n){
       WP* p=head;
       while(p!=NULL&&p->NO!=n){
       p=p->next;}
       if(p!=NULL)
	 free_wp(p);}





extern int pan(){
	WP* p=head;
	int t=1;
        while(p!=NULL){
		bool k=1;
		int z=p->NEWV;
		p->NEWV=expr(p->STR,&k);
		if(p->NEWV!=z){
			t=0;}
		p->LAST=z;
		p=p->next;}
	return t;}



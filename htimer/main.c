
#include<sys/epoll.h>
#include<stdio.h>
#include"rbt_timer.h"

void print_hello(timer_node_t* t){
  printf("hello timer handled %u\n", t->node.key);
}
int main(){
  init_timer();
  add_timer(1000, print_hello);
  add_timer(3000, print_hello);
  add_timer(5000, print_hello);
  add_timer(6000, print_hello);
  int epfd = epoll_create(1);
  struct epoll_event evs[512] = {0};
  for (;;){
    int nearst = find_nearst_time();
    printf("nearst : %d\n", nearst);
    int n = epoll_wait(epfd, evs, 512, nearst);
    for (int i = 0; i < n;i++){
      
    }
    handle_timer();
  }
  return 0;
}
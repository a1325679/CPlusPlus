#ifndef RBT_TIMER_H
#define RBT_TIMER_H

#include"ngx_rbtree.h"
#include<stdlib.h>
#include<time.h>
#include<assert.h>
struct timer_node_s;
typedef void (*timer_hander_pt)(struct timer_node_s *);
typedef struct timer_node_s
{
  ngx_rbtree_node_t node;
  timer_hander_pt hander;
} timer_node_t;

ngx_rbtree_t tree;
static ngx_rbtree_node_t sentinel;

unsigned int current_time(){
  unsigned int now;
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC,&tp);
  now = tp.tv_sec * 1000;
  now += tp.tv_nsec / 1000000;
  return now; 
}

void init_timer()
{
  ngx_rbtree_init(&tree, &sentinel, ngx_rbtree_insert_timer_value);
}

timer_node_t *add_timer(int expire,timer_hander_pt func){
  timer_node_t *te = (timer_node_t *)malloc(sizeof(timer_node_t));
  assert(te != NULL);
  te->node.key = expire+current_time();
  te->hander = func;
  ngx_rbtree_insert(&tree, &te->node);

  return te;
}

void del_timer(timer_node_t *tn){
  ngx_rbtree_delete(&tree, &tn->node);
}
void handle_timer(){
  ngx_rbtree_node_t *root, *node;
  for (;;){
    root = tree.root;
    if(root==tree.sentinel){
      break;
    }
    node = ngx_rbtree_min(root, tree.sentinel);
    if(node->key>current_time()){
      break;
    }
    timer_node_t *tn = (timer_node_t *)(char *)node;
    tn->hander(tn);
    ngx_rbtree_delete(&tree, &tn->node);
    free(tn);
  }
}
int find_nearst_time(){
  if(tree.root == tree.sentinel){
    return -1;
  }
  ngx_rbtree_node_t *node;
  node = ngx_rbtree_min(tree.root, tree.sentinel);
  int diff = (int)node->key - (int)current_time();
  return diff > 0 ? diff : 0;
}
#endif
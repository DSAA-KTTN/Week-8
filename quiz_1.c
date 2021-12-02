#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include "jrb.h"
#include "jval.h"
#include "dllist.h"

typedef struct{
  JRB edges;
  JRB vertices;
}Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char* name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int* output);
int outdegree(Graph graph, int v, int* output);
int DAG(Graph graph);
void dropGraph(Graph graph);
void DFS(Graph graph, int start, int stop, void (*func)(int));

/* createGraph */
Graph createGraph(){
  Graph a;
  a.edges=make_jrb();
  a.vertices=make_jrb();
  return a;
}

/* addVertex */
void addVertex(Graph graph, int id, char* name){
  JRB node=jrb_find_int(graph.vertices,id);
  if(node==NULL)
      jrb_insert_int(graph.vertices,id, new_jval_s(strdup(name)));
  else strcpy(jval_s(node->val),name);
}

/* getVertex */
char *getVertex(Graph graph, int id){
  JRB node=jrb_find_int(graph.vertices,id);
    if(node==NULL)return NULL;
    else return jval_s(node->val);
}

/* hasEdge */
int hasEdge(Graph graph, int v1, int v2){
  JRB node, tree;
  node = jrb_find_int(graph.edges, v1);
  if(node == NULL) return 0;
  tree = (JRB)jval_v(node->val);
  if(jrb_find_int(tree,v2) == NULL) return 0;
  else return 1;
}

 /* addEdge */
void addEdge(Graph graph, int v1, int v2){
  JRB node, tree;
  if(hasEdge(graph,v1,v2)==0){
    node = jrb_find_int(graph.edges,v1);
    if(node == NULL){
      tree = make_jrb();
      jrb_insert_int(graph.edges,v1, new_jval_v(tree));
    }else{
        tree = (JRB)jval_v(node -> val);
    }
    jrb_insert_int(tree, v2,new_jval_i(1));
  }
}

/* indegree */
int indegree(Graph graph, int v, int* output){
  JRB node,tree;
  int total=0;
  jrb_traverse(node, graph.edges){ 
    tree=(JRB)jval_v(node -> val);
    if(jrb_find_int(tree,v)!=NULL)
      output[total++] = jval_i(node->key);
  }
  return total;
}

/* outdegree */
int outdegree(Graph graph, int v, int* output){
  JRB node,tree;
  node = jrb_find_int(graph.edges, v);
  if(node==NULL)return 0;
    else tree = (JRB)jval_v(node->val);
  int total=0;
  jrb_traverse(node, tree) output[total++] = jval_i(node->key);
  return total;
}

/* DAG */
int DAG(Graph graph){
  JRB ptr;  
  Dllist stack = new_dllist(),node;
  int x,n,i,a[100];
  int visit[100];
  jrb_traverse(ptr,graph.edges){
    stack = new_dllist();
    dll_prepend(stack,ptr->key);

    memset(visit,0,100*sizeof(int));

    while(dll_empty(stack)==0){
      node=dll_first(stack);
      x=jval_i(node->val);
      dll_delete_node(node);  

      if(visit[x]==0){

        memset(a,0,100);
        n=outdegree(graph,x,a);

        for(i=0;i<n;i++) 
        if(visit[a[i]]==0) dll_prepend(stack,new_jval_i(a[i]));
          else if(a[i]==jval_i(ptr->key)){return 0;} 
        visit[x]=1;
      }          
    }
  }
  return 1;
}
/* dropGraph */
void dropGraph(Graph graph){
  JRB node;
  jrb_traverse(node, graph.edges) jrb_free_tree(jval_v(node->val));
  jrb_free_tree(graph.edges);
  jrb_free_tree(graph.vertices);
}

void printVertex(int v) { 
  printf("%4d", v);
}

/* DFS */
void DFS(Graph graph, int start, int stop, void (*func)(int)){
  Dllist stack = new_dllist(),node;
  int x,n,i,a[100];
  int visit[100];
  dll_prepend(stack,new_jval_i(start));

  memset(visit,0,100*sizeof(int));

  while(dll_empty(stack)==0){
    node=dll_first(stack);
    x=jval_i(node->val);
    dll_delete_node(node);  

    if(x==stop){
      func(x);
      break;
    }

    if(visit[x]==0){
      func(x);

      memset(a,0,100);
      n=outdegree(graph,x,a);

      for(i=0;i<n;i++) 
      if(visit[a[i]]==0) dll_prepend(stack,new_jval_i(a[i]));
        visit[x]=1;
    }
  }
}

/* main */
int main(){
  int size=5,n,i;
  int output[50];
  int a[100];

  Graph g=createGraph();
  addVertex(g, 0, "V0");
  addVertex(g, 1, "V1");
  addVertex(g, 2, "V2");
  addVertex(g, 3, "V3");

    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    //addEdge(g, 2, 0);
    addEdge(g, 1, 3);
    //addEdge(g,3,0);

    if (DAG(g)) printf("The graph is acycle\n");
    else printf("Have cycles in the graph\n");


  dropGraph(g);
  
}

/******************************************************/
/********  Minimum Independent Dominating Set  ********/
/********                 by                   ********/
/********   	    Faisal Abu-Khzam           ********/
/********            December 2023             ********/
/******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "assert.h"
#include <unistd.h>
#include <string.h>
#include <math.h>

/***** Global Variables  for MDS *****/
int **AL; //adjacency list 
int *deg; //initial degrees
double *weights;
int *rank;

int n; // number of vertices in graph
int e; // number of edges in graph

void copyIt(int *A, int *B)
{
    int i;
    for(i=0;i<n;i++)
        B[i]=A[i];
}

void readGraph(char *file)
{
    FILE *fp;
    int i, j, u, v, x, y;
    
    if ((fp = fopen(file, "r")) == NULL) 
    {
        fprintf(stderr, "Can't open file %s\n", file);
        exit(1);
    } 
    
    fscanf(fp, "%d %d", &n,&e); 
    
    if((AL = (int **) malloc(n * sizeof(int *))) == NULL)
    {
        fprintf(stderr, "Cannot malloc for AL\n"); 
        exit(1); 
    }
    
    if((deg = (int *) malloc(n * sizeof(int))) == NULL)
    {
 	fprintf(stderr, "Cannot malloc for deg\n");
        exit(1);
    }

    for(i = 0; i < n; i++)
    {
        if((AL[i] = (int *) malloc(n * sizeof(int))) == NULL)
        {
            fprintf(stderr, "Cannot malloc for ALi\n");
            exit(1);
        }
        deg[i] = 0;
    }
    
    for(i = 0; i < e; i++) 
    { 
        fscanf(fp, "%d %d", &u, &v);
        x = deg[(u)]++; 
	y = deg[(v)]++; 
	AL[(u)][x] = v;
	AL[(v)][y] = u;
    } 
    
    fclose(fp); 
}

void delVertex(int *d, int *sts, double *util, int x)
{
   int i,y;
   sts[x] = 0;
   for(i=0;i<d[x];i++){
      y = AL[x][i];
      if(sts[y] == 1)
         util[y] -= weights[x];
      }
}

int maxUtil(int *d, int *sts, double *util)
{
   int i,maxi = -1;
   double maxu = 0.0;

   for(i=0;i<n;i++)
      if(sts[i]==1 && util[i] > maxu){
         maxu = util[i];
         maxi = i;
         }
   return maxi;
}


int idsh(int *d, int *sts, double *util)
{
   int v,w,i,count=0, r=1;

   v = maxUtil(d,sts,util);
   while(v != -1){
      sts[v] = 2;
      rank[v] = r++;
      count++;
      for(i=0;i<d[v];i++){
         w = AL[v][i];
         if(sts[w] == 1)//still active
            delVertex(d,sts,util,w);
         }
      v = maxUtil(d,sts,util);
      }
   for(i=0;i<n;i++)
      if(sts[i] == 1){ // isolated
         sts[i]=2;
         rank[i] = r++;
         count++;
         }
   return count;
}

int main(int argc, char **argv)
{
   int i,j,count, *sts;
   double *util;
   clock_t start, finish;
   double  duration;
   FILE *fp;

   if(argc != 3)
   {
      printf("USAGE: <%s> <graph-file> <weights-file>\n",argv[0]);
      exit(1);
   }

   readGraph(argv[1]);

//start = clock();

   sts = (int *) malloc(n * sizeof(int));
   util = (double *) malloc(n * sizeof(double));
   rank = (int *) malloc(n * sizeof(int));
   weights = (double *) malloc(n * sizeof(double));

   fp = fopen(argv[2],"r");
   
   for(i=0;i<n;i++){
      rank[i]=0;
      sts[i] = 1;
      fscanf(fp,"%lg",&weights[i]);
      util[i] = weights[i];
      for(j=0;j<deg[i];j++)
         util[i] += weights[j];
      }
   fclose(fp);

   count = idsh(deg,sts,util);

   printf("An ids of size %d is (with ranks):\n",count);
   printf("Feature\t Rank\t Weight\t Utility\n");
   for(i=0;i<n;i++)
      if(sts[i] == 2)
         printf("%d\t%d\t%0.1f\t%0.1f\n",i,rank[i],weights[i],util[i]);
   printf("\n");

//finish = clock();
//duration = (double)(finish - start) / CLOCKS_PER_SEC;
//printf("Total time taken is: %2.1f sec\n", duration);
}

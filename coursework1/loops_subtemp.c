#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define N 729
#define reps 1000 
#include <omp.h> 

double a[N][N], b[N][N], c[N];
int jmax[N];  


void init1(void);
void init2(void);
void loop1(void);
void loop2(void);
void valid1(void);
void valid2(void);

int dist_loop1[N];
int size_loop1;
int dist_loop2[N];
int size_loop2;

char stype[] = "SCHEDULE_NAME\0";


int main(int argc, char *argv[]) { 

  double start1,start2,end1,end2;
  int r;

  size_loop1 = atoi(getenv("OMP_NUM_THREADS"));
  size_loop2 = size_loop1;

  init1(); 

  start1 = omp_get_wtime(); 

  for (r=0; r<reps; r++){ 
    loop1();
  } 

  end1  = omp_get_wtime();  

  printf("loop1,%d,%s,%f",size_loop1,stype,(float)(end1-start1)); 

  valid1(); 

  init2(); 

  start2 = omp_get_wtime(); 

  for (r=0; r<reps; r++){ 
    loop2();
  } 

  end2  = omp_get_wtime(); 

  printf("loop2,%d,%s,%f",size_loop2,stype,(float)(end2-start2)); 

  valid2(); 

#ifdef TRACEPARA
  char fname[100];
  char sline[100];
  for (int m = 0;m < N;m++) {
    printf("DISTLP1,%s,%d,%d,%d\n",stype,size_loop1,dist_loop1[m],m);
  }
  for (int m = 0;m < N;m++) {
    printf("DISTLP2,%s,%d,%d,%d\n",stype,size_loop2,dist_loop2[m],m);
  }
#endif
} 

void init1(void){
  int i,j; 

  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      a[i][j] = 0.0; 
      b[i][j] = 3.142*(i+j); 
    }
  }

}

void init2(void){ 
  int i,j, expr; 

  for (i=0; i<N; i++){ 
    expr =  i%( 3*(i/30) + 1); 
    if ( expr == 0) { 
      jmax[i] = N;
    }
    else {
      jmax[i] = 1; 
    }
    c[i] = 0.0;
  }

  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      b[i][j] = (double) (i*j+1) / (double) (N*N); 
    }
  }
 
} 

void loop1(void) { 
  int i,j;
 
#pragma omp parallel for schedule(SCHEDULE_DIREC) shared(a,b,dist_loop1,size_loop1) private(i,j) default(none) 
  for (i=0; i<N; i++){
#ifdef TRACEPARA
    dist_loop1[i] = omp_get_thread_num();
    size_loop1 =  omp_get_num_threads();
#endif
    for (j=N-1; j>i; j--){
      a[i][j] += cos(b[i][j]);
    } 
  }

} 



void loop2(void) {
  double rN2; 
  int i,j,k; 

  rN2 = 1.0 / (double) (N*N);  

#pragma omp parallel for schedule(SCHEDULE_DIREC) shared(jmax,c,b,rN2,dist_loop2,size_loop2) private(i,j,k) default(none) 
  for (i=0; i<N; i++){ 
#ifdef TRACEPARA
    dist_loop2[i] = omp_get_thread_num();
    size_loop2 =  omp_get_num_threads();
#endif
    for (j=0; j < jmax[i]; j++){
      for (k=0; k<j; k++){ 
	c[i] += (k+1) * log (b[i][j]) * rN2;
      } 
    }
  }

}

void valid1(void) { 
  int i,j; 
  double suma; 
  
  suma= 0.0; 
  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      suma += a[i][j];
    }
  }
  printf(",%lf\n", suma);

} 


void valid2(void) { 
  int i; 
  double sumc; 
  
  sumc= 0.0; 
  for (i=0; i<N; i++){ 
    sumc += c[i];
  }
  printf(",%f\n", sumc);
} 
 


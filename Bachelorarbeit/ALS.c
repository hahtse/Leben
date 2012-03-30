/*
** ALS algorithm for Non-negative matrix factorization (NMF)
** Lit: Berry, Browne, Langville (2006)
**
** G.Lohmann, July 2011
*/
#include <viaio/Vlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "f2c.h"

#include <time.h>

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_fit.h>
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_statistics.h>

#define ABS(x) ((x) > 0 ? (x) : -(x))

extern int dgetrf_ (int *m, int *n, double *a, int *lda, int *ipiv, int *info);
extern int dgetrs_(char *trans, int *n, int *nrhs, double *a, int *lda, int *ipiv, 
		    double *b, int *ldb, int *info);

/* R^2 */
double
ACheckResults(gsl_matrix *V,gsl_matrix *VV,
	      gsl_matrix *W,gsl_matrix *H,double *symm,double *mse)
{
  int i,j;
  double u,v,sum,mean,nx,sst,sse,r2;
  double *ptr1,*ptr2;

  gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,W,H,0.0,VV);

  /* check if solution is symmetric */
  sum = nx = 0;
  for (i=0; i<V->size1; i++) {
    for (j=0; j<i; j++) {
      u = gsl_matrix_get(VV,j,i);
      v = gsl_matrix_get(VV,i,j);
      sum += ABS(u-v);
      nx++;
    }
  }
  *symm = sum/nx;


  /* get R^2 */
  mean = nx = 0;
  for (i=0; i<V->size1; i++) {
    for (j=0; j<V->size2; j++) {
      u = gsl_matrix_get(V,i,j);
      if (i==j) u = 1;
      mean += u;
      nx++;
    }
  }
  mean /= nx;

  sst = sse = 0;
  for (i=0; i<V->size1; i++) {
    for (j=0; j<V->size2; j++) {
      u = gsl_matrix_get(V,i,j);
      v = gsl_matrix_get(VV,i,j);
      sst += (u-mean)*(u-mean);
      sse += (u-v)*(u-v);
    }
  }
  r2 = 0;
  if (sst > 1.0e-6) r2 = 1.0 - sse/sst;

  /* MSE */
  ptr1 = V->data;
  ptr2 = VV->data;
  sum = nx = 0;
  for (i=0; i<V->size1; i++) {
    for (j=0; j<i; j++) {
      u = (*ptr2++) - (*ptr1++);
      sum += u*u;
      nx++;
    }
  }
  *mse = 100.0*sum/nx;

  return r2;
}


double
ANMF(gsl_matrix *V,gsl_matrix *W,gsl_matrix *H,int maxiter,double *xsymm)
{
  gsl_matrix *HH=NULL,*HV=NULL,*WW=NULL,*VV=NULL,*WV=NULL;
  double u,r2,r2_old=0,symm=0,symm_old=0,mse=0;
  int i,j,n,m,r,iter,info;
  gsl_permutation *p;
  int s=0;
  double *pp1,*pp2;
  gsl_vector *b,*x;

  /*
  clock_t start, end;
  double cpu_time_used=0;
  */

  /*
  ** initialization
  */
  n = W->size1;
  r = W->size2;
  m = H->size2;

  VV = gsl_matrix_calloc (n,m);
  if (VV == NULL) VError(" err allocating matrix");
  WW = gsl_matrix_calloc (r,r);
  if (VV == NULL) VError(" err allocating matrix");
  HH = gsl_matrix_calloc (r,r);
  if (HH == NULL) VError(" err allocating matrix");
  WV = gsl_matrix_calloc (r,m);
  if (WV == NULL) VError(" err allocating matrix");
  HV = gsl_matrix_calloc (r,n);
  if (HV == NULL) VError(" err allocating matrix");
  p = gsl_permutation_alloc (r);
  b = gsl_vector_calloc(r);
  x = gsl_vector_calloc(r);


  /*
  ** main iteration
  */
  fprintf(stderr,"ALS-NMF...\n");
  r2_old = r2 = 0;
  symm = symm_old = 9999;
  for (iter=0; iter < maxiter; iter++) {

    if (iter%5 == 0) {
      r2 = ACheckResults(V,VV,W,H,&symm,&mse);
      fprintf(stderr," %5d   %10.7f   %f   %f\n",iter,r2,mse,symm);
      
      if (ABS(r2-r2_old) < 1.0e-8 && ABS(symm-symm_old) < 1.0e-8 && iter > 100) goto ende;
      if (r2 < r2_old && iter > 20) goto ende;
      
      r2_old = r2;
      symm_old = symm;
    }

    /* update H */
    gsl_blas_dgemm(CblasTrans,CblasNoTrans,1.0,W,W,0.0,WW);
    gsl_blas_dgemm(CblasTrans,CblasNoTrans,1.0,W,V,0.0,WV);
    info = gsl_linalg_LU_decomp (WW, p, &s);

    for (i=0; i<m; i++) {
      for (j=0; j<r; j++) {
	u = gsl_matrix_get(WV,j,i);
	gsl_vector_set(b,j,u);
      }
      gsl_linalg_LU_solve (WW, p, b, x);

      pp2 = x->data;
      for (j=0; j<r; j++) {
	u = *pp2++;
	if (u < 0) u = 0;
	gsl_matrix_set(H,j,i,u);
      }
    }

 
    /* update W */
    gsl_blas_dgemm(CblasNoTrans,CblasTrans,1.0,H,H,0.0,HH);
    /* start = clock(); */
    gsl_blas_dgemm(CblasNoTrans,CblasTrans,1.0,H,V,0.0,HV);
    /*
    end = clock();
    cpu_time_used += ((double) (end - start)) / CLOCKS_PER_SEC;
    */

    info = gsl_linalg_LU_decomp (HH, p, &s);

    for (i=0; i<n; i++) {

      for (j=0; j<r; j++) {
	u = gsl_matrix_get(HV,j,i);
	gsl_vector_set(b,j,u);
      }      
      gsl_linalg_LU_solve (HH, p, b, x);

      pp1 = gsl_matrix_ptr(W,i,0);
      pp2 = x->data;
      for (j=0; j<r; j++) {
	u = *pp2++;
	if (u < 0) u = 0;
	*pp1++ = u;
      }
    }
  }
  /* fprintf(stderr,"cpu_time_used: %f\n", cpu_time_used);*/


  /*
  **  normalize W,H
  */
 ende:
  gsl_matrix_free(HV);
  gsl_matrix_free(HH);
  gsl_matrix_free(WW);
  gsl_matrix_free(WV);

  int n0,p0;
  double chisq;
  p0 = W->size2;
  n0 = W->size1;
  gsl_multifit_linear_workspace *work = gsl_multifit_linear_alloc (n0,p0);
  gsl_vector *y0 = gsl_vector_calloc(n0);
  gsl_vector *x0 = gsl_vector_calloc(p0);
  gsl_matrix *cov = gsl_matrix_calloc(p0,p0);
  gsl_vector_set_all(y0,(double)1.0);
  gsl_multifit_linear (W,y0,x0,cov,&chisq,work);
  /* for (i=0; i<p0; i++) fprintf(stderr," x[%d]= %f\n",i,gsl_vector_get(x0,i)); */

  for (i=0; i<W->size1; i++) {
    for (j=0; j<W->size2; j++) {
      u = gsl_matrix_get(W,i,j);
      gsl_matrix_set(W,i,j,u*gsl_vector_get(x0,j));
    }
  }
  for (i=0; i<H->size1; i++) {
    for (j=0; j<H->size2; j++) {
      u = gsl_matrix_get(H,i,j);
      gsl_matrix_set(H,i,j,u/gsl_vector_get(x0,i));
    }
  }

  /*
  gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,W,H,0.0,VV);
  FILE *fp = fopen("test.txt","w");
  size_t k=0;
  for (i=0; i<V->size1; i+=5) {
    for (j=0; j<i; j+=5) {
      u = gsl_matrix_get(V,i,j);
      v = gsl_matrix_get(VV,i,j);
      fprintf(fp," %f  %f\n",u,v);
      k++;
      if (k > 9000) break;
    }
  }
  fclose(fp);  
  */

  r2 = ACheckResults(V,VV,W,H,&symm,&mse);
  fprintf(stderr," %5d   %10.7f   %f   %f\n",iter,r2,mse,symm);
  gsl_matrix_free(VV);

  *xsymm = symm;
  return r2;
}

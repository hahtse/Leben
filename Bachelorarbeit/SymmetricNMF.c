/*
** Algorithm for symmetric problem: V = H H^t
**
** Reference: Ding, He, Simon (2005) 
** On the Equivalence of NMF and spectral clustering,
** SIAM conf.
**
** G.Lohmann, Oct 2011
*/
#include <viaio/Vlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "f2c.h"

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

/* R^2 */
double
SCheckResults(gsl_matrix *V,gsl_matrix *VV,gsl_matrix *H,double *mse)
{
  int i,j;
  double u,v,sum,mean,nx,sst,sse,r2;
  double *ptr1,*ptr2;

  gsl_blas_dgemm(CblasNoTrans,CblasTrans,1.0,H,H,0.0,VV);


  /* get R^2 */
  mean = nx = 0;
  for (i=0; i<V->size1; i++) {
    for (j=0; j<V->size2; j++) {
      mean += gsl_matrix_get(V,i,j);
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
SymmNMF(gsl_matrix *V,gsl_matrix *H,double beta,int maxiter)
{
  gsl_matrix *VV=NULL,*HV=NULL,*HH=NULL,*HHH=NULL;
  double u,v,w,r2,r2_old=0,mse=0;
  size_t i,j,n,r,iter;

  /*
  ** initialization, matrix H is n x r (with r small, and V = n x n)
  */
  n = V->size1;
  r = H->size2;

  VV = gsl_matrix_calloc (n,n);
  if (VV == NULL) VError(" err allocating matrix VV");
  HH = gsl_matrix_calloc (r,r);
  if (HH == NULL) VError(" err allocating matrix HH");
  HHH = gsl_matrix_calloc (n,r);
  if (HHH == NULL) VError(" err allocating matrix HHH");
  HV = gsl_matrix_calloc (r,n);
  if (HV == NULL) VError(" err allocating matrix HV");

 
  /*
  ** main iteration
  */
  fprintf(stderr,"symmetric NMF...\n");
  r2_old = r2 = 0;
  for (iter=0; iter < maxiter; iter++) {

    if (iter%5 == 0) {
      r2 = SCheckResults(V,VV,H,&mse);
      fprintf(stderr," %5d   %10.7f   %f\n",iter,r2,mse);
      
      if (ABS(r2-r2_old) < 1.0e-6 && iter > 20) goto ende;
      if (r2_old > r2 && iter > 5) goto ende;
      r2_old = r2;
    }

    /* WH */
    gsl_blas_dgemm(CblasTrans,CblasTrans,1.0,H,V,0.0,HV);

    /* H H^t H */
    gsl_blas_dgemm(CblasTrans,CblasNoTrans,1.0,H,H,0.0,HH);
    gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,H,HH,0.0,HHH);

    /* update H */
    for (i=0; i<H->size1; i++) {
      for (j=0; j<H->size2; j++) {
	u = gsl_matrix_get(H,i,j);
	v = gsl_matrix_get(HV,j,i);  /* fast to use transpose */
	w = gsl_matrix_get(HHH,i,j);
	if (w != 0) u *= (1.0 - beta + beta*v/w);
	gsl_matrix_set(H,i,j,u);
      }
    }
  }

 ende:
  gsl_matrix_free(HV);
  gsl_matrix_free(HH);
  gsl_matrix_free(HHH);

  fprintf(stderr," symm ok, r2=%f\n",r2);
  return r2;
}

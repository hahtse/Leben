/*
** Non-negative matrix factorization (NMF)
**
** G.Lohmann, Dec 2005
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_fit.h>
#include <gsl/gsl_multifit.h>

#define ABS(x) ((x) > 0 ? (x) : -(x))

extern double ACheckResults(gsl_matrix *V,gsl_matrix *VV,
			    gsl_matrix *W,gsl_matrix *H,double *symm,double *mse);


double
XNMF(gsl_matrix *V,gsl_matrix *W,gsl_matrix *H,int maxiter,double *symm)
{
  static gsl_matrix *HH=NULL,*WW=NULL,*VV=NULL;
  static gsl_matrix *WV=NULL,*WH=NULL,*WWH=NULL,*VH=NULL,*WHH=NULL;
  double u,h,wv,wh,w,vh;
  double r2,r2_old=0,mse=0,mse_old=0,xsymm=0;
  double *ptr1,*ptr2,*ptr3,*ptr4;
  int i,j,n,m,r,iter;
  
  /*
  ** initialization
  */
  n = W->size1;
  r = W->size2;
  m = H->size2;
  fprintf(stderr," NMF...\n");

  if (WW == NULL) {
    VV = gsl_matrix_calloc (n,m);
    WW = gsl_matrix_calloc (n,r);
    HH = gsl_matrix_calloc (r,m);
    WV = gsl_matrix_calloc (W->size2,V->size2);
    WH = gsl_matrix_calloc (V->size1,H->size2);
    WWH = gsl_matrix_calloc (W->size2,H->size2);
    VH  = gsl_matrix_calloc (W->size1,W->size2);
    WHH = gsl_matrix_calloc (W->size1,W->size2);
  }


  /*
  ** iteration
  */
  mse = 0;
  mse_old = 9999;
  r2_old = r2 = 0;
  for (iter=0; iter < maxiter; iter++) {

    if (iter%5 == 0) {
      r2 = ACheckResults(V,VV,W,H,&xsymm,&mse);
      fprintf(stderr," %5d   %10.7f   %f   %f\n",iter,r2,xsymm,mse);
      if (ABS(r2-r2_old) < 0.0001 && iter > 100) goto ende;
      if (r2 < r2_old && iter > 5) goto ende;
      if (mse > mse_old && iter > 5) goto ende;
      r2_old = r2;
      mse_old = mse;
    }

    /* update H */
    gsl_blas_dgemm(CblasTrans,CblasNoTrans,1.0,W,V,0.0,WV);
    gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,W,H,0.0,WH);
    gsl_blas_dgemm(CblasTrans,CblasNoTrans,1.0,W,WH,0.0,WWH);

    ptr1 = H->data;
    ptr2 = WV->data;
    ptr3 = WWH->data;
    ptr4 = HH->data;

    for (i=0; i<m*r; i++) {
      h  = *ptr1++;
      wv = *ptr2++;
      wh = *ptr3++;

      u = 0;
      if (wh != 0) u = h * wv/wh;
      
      *ptr4++ = u;
    }


    /* update W */
    gsl_blas_dgemm(CblasNoTrans,CblasTrans,1.0,V,H,0.0,VH);
    gsl_blas_dgemm(CblasNoTrans,CblasTrans,1.0,WH,H,0.0,WHH);

    ptr1 = W->data;
    ptr2 = VH->data;
    ptr3 = WHH->data;
    ptr4 = WW->data;

    for (i=0; i<n*r; i++) {
      w  = *ptr1++;
      vh = *ptr2++;
      wh = *ptr3++;
      
      u = 0;
      if (wh != 0) u = w * vh/wh;
      
      *ptr4++ = u;
    }
    gsl_matrix_memcpy (H,HH);
    gsl_matrix_memcpy (W,WW);
  }


  /*
  **  normalize H,W
  */
 ende: ;
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
  r2 = ACheckResults(V,VV,W,H,&xsymm,&mse);
  fprintf(stderr," %5d   %10.7f   %f   %f\n",iter,r2,xsymm,mse);
  gsl_matrix_free(VV);

  *symm = xsymm;
  return r2;
}

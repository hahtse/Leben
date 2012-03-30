/*
** group-level NMF
**
** G.Lohmann, July 2011
*/
#include <viaio/Vlib.h>
#include <viaio/VImage.h>
#include <viaio/mu.h>
#include <viaio/option.h>

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_cblas.h>
#include <gsl/gsl_sort.h>
#include <gsl/gsl_sort_double.h>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define NBASIS 100

#define SQR(x) ((x) * (x))
#define ABS(x) ((x) > 0 ? (x) : -(x))

extern int isnan(double);
extern int isinf(double);

extern double SymmNMF(gsl_matrix *,gsl_matrix *,double,int);
extern double XNMF(gsl_matrix *,gsl_matrix *,gsl_matrix *,int,double *);
extern double ANMF(gsl_matrix *,gsl_matrix *,gsl_matrix *,int,double *);
extern float *ECM(gsl_matrix *V);
extern void VProb(double *data,size_t n,double *x,double *hist,size_t len);


VDictEntry TypeDict[] = {
  { "gauss", 0 },
  { "flat", 1 },
  { NULL }
};


int 
main (int argc,char *argv[])
{  
  static VString  wfilename = "";
  static VShort   nbasis   = 5;
  static VLong    seed     = 99402622;
  static VLong    maxiter1  = 200;
  static VLong    maxiter2  = 50;
  static VShort   type      = 1;
  static VBoolean symmetric = TRUE;
  static VOptionDescRec  options[] = {   
    {"wfile",VStringRepn,1,(VPointer) &wfilename,VOptionalOpt,NULL,"wimage"},
    {"nbasis", VShortRepn,1,(VPointer) &nbasis,VOptionalOpt,NULL,"Number of basis functions"},
    {"seed", VLongRepn,1,(VPointer) &seed,VOptionalOpt,NULL,"Seed for random numbers"},
    {"aiter", VLongRepn,1,(VPointer) &maxiter1,VOptionalOpt,NULL,"max number of iterations for ALS"},
    {"biter", VLongRepn,1,(VPointer) &maxiter2,VOptionalOpt,NULL,"max number of iterations for 2nd stage"},
    {"type", VShortRepn,1,(VPointer) &type,VOptionalOpt,TypeDict,"type of random ini"},
    {"symmetric", VBooleanRepn,1,(VPointer) &symmetric,VOptionalOpt,NULL,"symmetric CP-factorization"}
  };
  VAttrListPosn posn;
  FILE *in_file,*out_file,*fp;
  VAttrList list=NULL,out_list=NULL,list2=NULL;
  VImage map=NULL,wmap=NULL,wimage=NULL,himage=NULL;
  VImage Himage[NBASIS],Wimage[NBASIS];
  VImage image=NULL;
  VBundle src=NULL;
  size_t i,j;
  VLong nvox,nslices,nrows,ncols;
   
  /* parse command line */
  VParseFilterCmd(VNumber(options),options,argc,argv,&in_file,&out_file);

  gsl_rng_env_setup();
  const gsl_rng_type *T  = gsl_rng_default;
  gsl_rng *rx = gsl_rng_alloc(T);
  gsl_rng_set(rx,(unsigned long)seed);


  /*
  ** read basis file W if present
  */
  if (strlen(wfilename) > 1) {
    fp = VOpenInputFile (wfilename, TRUE);
    list2 = VReadFile (fp, NULL);
    if (! list2) VError("Error reading W-file");
    fclose(fp);
    int flag = 0;
    int mflag = 0;
    for (VFirstAttr (list2, & posn); VAttrExists (& posn); VNextAttr (& posn)) {
      if (VGetAttrRepn (& posn) != VImageRepn) continue;
      VGetAttrValue (& posn, NULL,VImageRepn, & image);

      if (mflag == 0) {
        if (VGetAttr(VImageAttrList(image),"map",NULL,VImageRepn,&wmap) == VAttrFound) mflag = 1;
      }
      if (strcmp(VGetAttrName (&posn),"W-matrix") == 0) {
	wimage = image;
        flag = 1;
      }
      if (strcmp(VGetAttrName (&posn),"H-matrix") == 0) {
	himage = image;
      }
    }
    if (mflag == 0) VError(" no map found, voxel addresses missing");
    if (flag == 0) VError(" no w-file found");
    if (wmap == NULL) VError(" no map found, voxel addresses missing");

    nbasis = VImageNColumns(wimage);
    fprintf(stderr," nbasis = %d\n",nbasis);
  }



  if (! (list = VReadFile (in_file, NULL))) exit (1);
  fclose(in_file);

  i = 0;
  for (VFirstAttr (list, & posn); VAttrExists (& posn); VNextAttr (& posn)) {
    if (VGetAttrRepn (& posn) != VBundleRepn) continue;
    i++;
 
    VGetAttrValue (& posn, NULL,VBundleRepn, & src);
    if (VGetAttr (src->list,"map", NULL,VImageRepn, (VPointer) & map) != VAttrFound)
      VError(" attr 'map' not found");

    if (VGetAttr (src->list,"nvoxels", NULL,VLongRepn, (VPointer) & nvox) != VAttrFound)
      VError(" attr 'nvoxels' not found");
    if (VGetAttr (src->list,"nslices", NULL,VLongRepn, (VPointer) & nslices) != VAttrFound)
      VError(" attr 'nslices' not found");
    if (VGetAttr (src->list,"nrows", NULL,VLongRepn, (VPointer) & nrows) != VAttrFound)
      VError(" attr 'nrows' not found");
    if (VGetAttr (src->list,"ncols", NULL,VLongRepn, (VPointer) & ncols) != VAttrFound)
      VError(" attr 'ncols' not found");
    fprintf(stderr," image dims: %ld %ld %ld, %ld\n",nslices,nrows,ncols,nvox);
    break;
  }
  if (!src) VError(" input matrix not found");

  if (wmap != NULL) {
    if (VImageNRows(wmap) != VImageNRows(map)) 
      VError(" inconsistent dims in map, nrows: %d %d",VImageNRows(wmap),VImageNRows(map));
    if (VImageNColumns(wmap) != VImageNColumns(map)) 
      VError(" inconsistent dims in map, ncols: %d %d",VImageNColumns(wmap),VImageNColumns(map));
  }


  /*
  ** NMF of group correlation matrix
  */
  fprintf(stderr," alloc V...\n"); 
  float *A = src->data;
  gsl_matrix *V = gsl_matrix_calloc(nvox,nvox);
  if (!V) VError(" err allocating correlation matrix V");

  fprintf(stderr," ini V...\n"); 
  double u = 0;
  size_t k = 0;
  for (i=0; i<nvox; i++) {
    for (j=0; j<=i; j++) {
      u = A[k++];
      if (u < 0) u = 0;
      gsl_matrix_set(V,i,j,u);
      gsl_matrix_set(V,j,i,u);
    }
  }
  VFree(A);

  
  if (wimage != NULL) {
    nbasis = VImageNColumns(wimage);
    fprintf(stderr," nbasis= %d\n",nbasis);
  }

  fprintf(stderr," ini W,H...\n"); 
  int n = (int)nvox;
  int m = (int)nvox;
  int r = (int)nbasis;


  gsl_matrix *W0 = gsl_matrix_calloc (n,r);
  if (!W0) VError(" err alloc W");
  gsl_matrix *H0 = gsl_matrix_calloc (r,m);
  if (!H0) VError(" err alloc H");

  
  /* ini H,W using prior info */
  if (wimage != NULL) {
    fprintf(stderr," ini H,W, prior info \n");
    r = VImageNColumns(wimage);

    if (VImageNRows(wimage) != W0->size1) 
      VError(" inconsistent dims in W: %d %d",VImageNRows(wimage),W0->size1);
    if (VImageNColumns(wimage) != W0->size2) 
      VError(" inconsistent dims in W: %d %d",VImageNColumns(wimage),W0->size2);
    for (i=0; i<W0->size1; i++) {
      for (j=0; j<W0->size2; j++) {
	u = VGetPixel(wimage,0,i,j);
	if (isnan(u) || isinf(u)) u = 0;
	if (u < 0) u = 0;
	gsl_matrix_set(W0,i,j,u);
      }
    }

    if (VImageNRows(himage) != H0->size1) 
      VError(" inconsistent dims in H: %d %d",VImageNRows(himage),H0->size1);
    if (VImageNColumns(himage) != H0->size2) 
      VError(" inconsistent dims in H: %d %d",VImageNColumns(himage),H0->size2);
    for (i=0; i<H0->size1; i++) {
      for (j=0; j<H0->size2; j++) {
	u = VGetPixel(himage,0,i,j);
	if (isnan(u) || isinf(u)) u = 0;
	if (u < 0) u = 0;
	gsl_matrix_set(H0,i,j,u);
      }
    }
  }


  /* ini H,W randomly  */
  else {
    fprintf(stderr," ini W, random \n");
    double sum1=0,sum2=0,nx=0;
    double smin = VRepnMaxValue(VDoubleRepn);
    double smax = VRepnMinValue(VDoubleRepn);
    double mean=0,sigma=0;

    sum1 = sum2 = nx = 0;
    for (i=0; i<V->size1; i++) {
      for (j=0; j<=i; j++) {
	u = gsl_matrix_get(V,i,j);
	if (u < smin) smin = u;
	if (u > smax) smax = u;
	sum1 += u;
	sum2 += u*u;
	nx++;
      }
    }
    mean = sum1/nx;
    sigma = sqrt((sum2 - nx * mean*mean) / (nx - 1.0));
    if (smin < 0) smin = 0;
    while (mean-sigma < 0) sigma *= 0.9;
    fprintf(stderr," %.6f  %.6f\n",mean,sigma);
    fprintf(stderr," %.6f  %.6f\n",smin,smax);

    for (i=0; i<W0->size1; i++) {
      for (j=0; j<W0->size2; j++) {
	if (type == 0)
	  u = mean + gsl_ran_gaussian(rx,0.5*sigma);
	else
	  u = gsl_ran_flat(rx,smin,smax);
	if (u < smin) u = smin;
	if (u > smax) u = smax;
	if (u < 0) u = 0;
	gsl_matrix_set(W0,i,j,u);
      }
    }


    /* ini H */
    for (i=0; i<H0->size1; i++) {
      for (j=0; j<H0->size2; j++) {
	u = 1.0 / (double) r;
	gsl_matrix_set(H0,i,j,u);
      }
    }
  }

  /* degree distribution of input matrix V */
  /*
  size_t len = 100;
  size_t klen = V->size1*V->size2;
  double *xaxis = VCalloc(len,sizeof(double));
  double *hist1 = VCalloc(len,sizeof(double));
  VProb(V->data,klen,xaxis,hist1,len);
  gsl_ran_discrete_t *gx = gsl_ran_discrete_preproc (len,hist1);

  for (i=0; i<W0->size2; i++) {
    for (j=0; j<W0->size1; j++) {
      k = gsl_ran_discrete (rx,gx);
      u = xaxis[k];
      if (u < 0) u = 0;
      gsl_matrix_set(W0,j,i,u);
    }
  }
  */

  /* NMF */
  double r2=0;
  double symm=0;
  r2 = ANMF(V,W0,H0,maxiter1,&symm);
 
  if (symmetric) {
    for (i=0; i<n; i++)
      for (j=0; j<r; j++)
	gsl_matrix_set(W0,i,j,0.5*(gsl_matrix_get(W0,i,j)+gsl_matrix_get(H0,j,i)));

    double beta = 0.5;
    r2 = SymmNMF(V,W0,(double)beta,(int)maxiter2);
    for (i=0; i<n; i++)
      for (j=0; j<r; j++)
	gsl_matrix_set(H0,j,i,gsl_matrix_get(W0,i,j));
  }
  else {
    r2 = XNMF(V,W0,H0,maxiter2,&symm);
  }
  


  /* degree distribution of residuals */
  /*
  double *hist2 = VCalloc(len,sizeof(double));
  VProb(V->data,klen,xaxis,hist2,len);
  FILE *f=fopen("degree_distribtion.txt","w");
  if (!f) VError("err opening txt-file");
  for (i=0; i<len; i++) fprintf(f,"%d %f %f\n",(int)i,hist1[i],hist2[i]);
  fclose(f);
  */


  /* output W */
  VString str;
  VExtractAttr (src->list,"lattice",NULL,VStringRepn,&str,FALSE);
  VExtractAttr (src->list,"birth",NULL,VStringRepn,&str,FALSE);

  int bb,rr,cc;
  for (j=0; j<r; j++) {
    Wimage[j] = VCreateImage((int)nslices,(int)nrows,(int)ncols,VFloatRepn);
    if (!Wimage[j]) VError(" error allocating W-image %d",j);
    VFillImage(Wimage[j],VAllBands,0);
    VImageAttrList (Wimage[j]) = VCopyAttrList(src->list);

    for (i=0; i<nvox; i++) {
      bb = VPixel(map,0,0,i,VShort);
      rr = VPixel(map,0,1,i,VShort);
      cc = VPixel(map,0,2,i,VShort);
      VPixel(Wimage[j],bb,rr,cc,VFloat) = gsl_matrix_get(W0,i,j);
    }
  }

  out_list = VCreateAttrList();
  for (j=0; j<r; j++) {
    VSetAttr(VImageAttrList(Wimage[j]),"modality",NULL,VStringRepn,"conimg");
    VSetAttr(VImageAttrList(Wimage[j]),"name",NULL,VStringRepn,"W");
    VSetAttr(VImageAttrList(Wimage[j]),"R^2",NULL,VFloatRepn,(VFloat)r2);
    VSetAttr(VImageAttrList(Wimage[j]),"symm",NULL,VFloatRepn,(VFloat)symm);
    VSetAttr(VImageAttrList(Wimage[j]),"seed",NULL,VLongRepn,(VLong)seed);
    VAppendAttr(out_list,"W",NULL,VImageRepn,Wimage[j]);
  }

  /* output H */
  for (j=0; j<r; j++) {
    Himage[j] = VCreateImage((int)nslices,(int)nrows,(int)ncols,VFloatRepn);
    if (!Himage[j]) VError(" error allocating H-image %d",j);
    VFillImage(Himage[j],VAllBands,0);
    VImageAttrList (Himage[j]) = VCopyAttrList(src->list);

    for (i=0; i<nvox; i++) {
      bb = VPixel(map,0,0,i,VShort);
      rr = VPixel(map,0,1,i,VShort);
      cc = VPixel(map,0,2,i,VShort);
      VPixel(Himage[j],bb,rr,cc,VFloat) = gsl_matrix_get(H0,j,i);
    }
  }

  for (j=0; j<r; j++) {
    VSetAttr(VImageAttrList(Himage[j]),"modality",NULL,VStringRepn,"conimg");
    VSetAttr(VImageAttrList(Himage[j]),"name",NULL,VStringRepn,"H");
    VSetAttr(VImageAttrList(Himage[j]),"R^2",NULL,VFloatRepn,(VFloat)r2);
    VSetAttr(VImageAttrList(Himage[j]),"symm",NULL,VFloatRepn,(VFloat)symm);
    VSetAttr(VImageAttrList(Himage[j]),"seed",NULL,VLongRepn,(VLong)seed);
    VAppendAttr(out_list,"H",NULL,VImageRepn,Himage[j]);
  }


  /* W,H as matrices */
  VImage Wmatrix = VCreateImage(1,W0->size1,W0->size2,VFloatRepn);
  /* VSetAttr(VImageAttrList(Wmatrix),"map",NULL,VImageRepn,"map"); */
  VSetAttr(VImageAttrList(Wmatrix),"modality",NULL,VStringRepn,"conimg");
  VSetAttr(VImageAttrList(Wmatrix),"name",NULL,VStringRepn,"W-matrix");
  double *wptr = W0->data;
  VFloat *wpp = VImageData(Wmatrix);
  for (i=0; i<VImageNPixels(Wmatrix); i++) *wpp++ = (float)(*wptr++);
  VAppendAttr(out_list,"W-matrix",NULL,VImageRepn,Wmatrix);


  VImage Hmatrix = VCreateImage(1,H0->size1,H0->size2,VFloatRepn);
  VSetAttr(VImageAttrList(Hmatrix),"modality",NULL,VStringRepn,"conimg");
  VSetAttr(VImageAttrList(Hmatrix),"name",NULL,VStringRepn,"H-matrix");
  VAppendAttr(out_list,"H-matrix",NULL,VImageRepn,Hmatrix);
  double *hptr = H0->data;
  VFloat *hpp = VImageData(Hmatrix);
  for (i=0; i<VImageNPixels(Hmatrix); i++) *hpp++ = (float)(*hptr++);

  /* get principal eigenvector of residuals */
  /*
  float *ev=NULL;
  ev = ECM(V);
  VImage Rimage = VCreateImage((int)nslices,(int)nrows,(int)ncols,VFloatRepn);
  VFillImage(Rimage,VAllBands,0);
  VFillImage(Rimage,VAllBands,0);
  VImageAttrList (Rimage) = VCopyAttrList(src->list);
  VSetAttr(VImageAttrList(Rimage),"modality",NULL,VStringRepn,"conimg");
  VSetAttr(VImageAttrList(Rimage),"name",NULL,VStringRepn,"ECM_of_residuals");

  for (i=0; i<nvox; i++) {
    bb = VPixel(map,0,0,i,VShort);
    rr = VPixel(map,0,1,i,VShort);
    cc = VPixel(map,0,2,i,VShort);
    VPixel(Rimage,bb,rr,cc,VFloat) = ev[i];
  }
  VAppendAttr(out_list,"residuals",NULL,VImageRepn,Rimage);
  */

  /* write to disk */
 ende:
  if (! VWriteFile (out_file, out_list)) exit (1);
  fprintf (stderr, "%s: done.\n", argv[0]);
  exit(0);
}


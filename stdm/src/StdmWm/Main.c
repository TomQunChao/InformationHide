#include "stdmwm.h"
#include <stdio.h>

void testAll(const Image *im, const Image *wm) {
	WmImage wi, nwi;
	wi.w = NULL;
	nwi.w = NULL;

	Image nwm;
	double res;
	FILE *f;
	unsigned epN = 50;
	double a=8;
	char buf[256];

	copyImage(&nwm, wm);
	copyImage(&nwi.im,im);
	copyImage(&wi.im,im);
	addWm(im, wm, &wi, 8, 8);
	addWm(im, wm, &nwi, 8, 8);

	// 50 Gauss/bright/contrast, delta=8
	f = fopen("atkgbc_d8_k8.hex", "w");
	addWm(im, wm, &wi, 8, 8);
	for (unsigned i = 0; i < 50; i++) {
		a = i / 10.0;
		// no attack
		getWm(&wi, &nwm,8);
		res = wmDiff(wm->realDat, nwm.realDat, wm->size);
		fprintf(f, "%f %f ",a, res);
		// add gauss noise
		attackGuss(&nwi.im, &wi.im, a);
		getWm(&nwi, &nwm,8);
		res = wmDiff(wm->realDat, nwm.realDat, wm->size);
		fprintf(f, "%f ", res);
		// change bright
		changeBright(&nwi.im, &wi.im, a);
		getWm(&nwi, &nwm,8);
		res = wmDiff(wm->realDat, nwm.realDat, wm->size);
		fprintf(f, "%f ", res);
		// change contrast
		changeContrast(&nwi.im, &wi.im, a);
		getWm(&nwi, &nwm,8);
		res = wmDiff(wm->realDat, nwm.realDat, wm->size);
		fprintf(f, "%f ", res);
		fprintf(f, "\n");

	}
	fflush(f);
	fclose(f);

	// 50 jpeg delta=8
	f=fopen("atkj_d8_k8.hex","w");
	for(unsigned i=0;i<epN;i++){
		a=i/0.5;
		getWm(&wi,&nwm,8);
		res=wmDiff(wm->realDat,nwm.realDat,wm->size);
		fprintf(f,"%f %f ",a,res);
		// jpeg compress
		jpegCompress(&nwi.im,&wi.im,a);
		getWm(&nwi,&nwm,8);
		res=wmDiff(wm->realDat,nwm.realDat,wm->size);
		fprintf(f,"%f ",res);
		fprintf(f,"\n");
	}
	fflush(f);
	fclose(f);

	// 50 delta noise STD=2.5/5
	f=fopen("atk_n25_5_dd.hex","w");
	for(unsigned i=0;i<epN;i++){
		a=(i+1)*2/5.0;
		addWm(im,wm,&wi,8,a);
		sprintf(buf,"im%d.bmp%c",i+1,'\0');
//		writeImage(buf,&wi.im);
		// no attack
		getWm(&wi,&nwm,a);
		res=wmDiff(wm->realDat,nwm.realDat,wm->size);
		fprintf(f,"%f %f ",a, res);
		// add guass noise
		sprintf(buf,"imw%d.bmp%c",i+1,'\0');
//		writeImage(buf,&nwm);

		attackGuss(&nwi.im,&wi.im,2.5);
		getWm(&nwi,&nwm,a);
		res=wmDiff(wm->realDat,nwm.realDat,wm->size);
		fprintf(f,"%f ",res);
		//
		attackGuss(&nwi.im,&wi.im,5);
		getWm(&nwi,&nwm,a);
		res=wmDiff(wm->realDat,nwm.realDat,wm->size);
		fprintf(f,"%f ",res);
		res=mse(im->realDat,wi.im.realDat,im->size);
		fprintf(f,"%f",res);
		fprintf(f,"\n");
	}
	fflush(f);
	fclose(f);

	freeImage(&nwi.im);
	freeImage(&wi.im);
	freeImage(&nwm);
	free(wi.w);
	free(nwi.w);
}

int main() {
	printf("initialize...\n");
	initImageFac(8);
	initWm(8);
	initBoxMuller(1000,2000);

	Image im, wm, nwm;
	printf("read image...\n");
	readImage("LENA.BMP", &im);
	readImage("LOGO.BMP", &wm);
	WmImage wi;
	wi.w = NULL;
	copyImage(&wi.im, &im);
	printf("add water mark...\n");
	addWm(&im, &wm, &wi, 8, 8);
	writeImage("newLena.bmp", &wi.im);
	copyImage(&nwm, &wm);
	printf("water mark decode...\n");
	getWm(&wi, &nwm,8);
	printf("save image...\n");
	writeImage("newLogo.bmp", &nwm);
	writeImage("newLena.bmp", &wi.im);

	printf("test all...\n");
	testAll(&im,&wm);

	printf("clean memory...\n");
	freeImage(&im);
	freeImage(&wm);
	freeImage(&nwm);
	freeImage(&wi.im);
	freeBoxMuller();

	freeImageFac();
	freeWm();
	printf("complete!\n");
	return 0;
}

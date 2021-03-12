#define _CRT_SECURE_NO_WARNINGS
#include "SsWm.h"
#include "roc.h"
#include <stdio.h>
#include <conio.h>

void testSsWm(ImgFac &imf,const Image &im,const Image &wm) {
	SsWm sw(8);
	double res,a;
	WmImage wi = sw.addWm(im, wm, 1.8);
	WmImage	nwi = sw.addWm(im, wm, 1.8);
	Image nwm;
	memcpy(&nwm, &wm, sizeof(Image));
	nwm.data = new char[nwm.bfSize];
	memcpy(nwm.data, wm.data, sizeof(char) * wm.bfSize);
	nwm.realDat = &nwm.data[nwm.bfOffBits];
	imf.setJpegN(8);
	imf.jpegCompress(nwi, wi, 100);
	fstream f;
	f.open("achange_c02_b02_g05.hex", ios::out);
	f << 50 << endl;
	char buf[256];
	for (unsigned i = 0; i < 50; i++) {
		a = i/10.0;
		sw.addWm(wi, im, wm, a);
		sw.getWm(nwm, wi);
		sprintf(buf, "img%d.bmp", i);
		/*imf.saveImage(nwm, buf);*/
		f << a ;
		mse(im.realDat, wi.realDat, im.size, res);
		f << " " << res;
		wmDiff(wm.realDat, nwm.realDat, wm.size, res);
		f << ' ' << res;
		imf.changeBright(nwi,wi, 0.2);
		sw.getWm(nwm,nwi);
		wmDiff(wm.realDat, nwm.realDat, wm.size, res);
		f << ' ' << res;
		imf.changeContrast(nwi,wi, 0.2);
		sw.getWm(nwm,nwi);
		wmDiff(wm.realDat, nwm.realDat, wm.size, res);
		f << ' ' << res;
		imf.attackGuss(nwi,wi, 5);
		sw.getWm(nwm,nwi);
		wmDiff(wm.realDat, nwm.realDat, wm.size, res);
		f << ' ' << res;
		f << "\n";
	}
	f.flush();
	f.close();
	f.open("achange_c05_b05_g10.hex", ios::out);
	f << 50 << endl;
	for (unsigned i = 0; i < 50; i++) {
		a = i / 10.0;
		sw.addWm(wi, im, wm, a);
		sw.getWm(nwm, wi);
		sprintf(buf, "img%d.bmp", i);
		/*imf.saveImage(nwm, buf);*/
		f << a;
		mse(im.realDat, wi.realDat, im.size, res);
		f << " " << res;
		wmDiff(wm.realDat, nwm.realDat, wm.size, res);
		f << ' ' << res;
		imf.changeBright(nwi, wi, 0.5);
		sw.getWm(nwm, nwi);
		wmDiff(wm.realDat, nwm.realDat, wm.size, res);
		f << ' ' << res;
		imf.changeContrast(nwi, wi, 0.5);
		sw.getWm(nwm, nwi);
		wmDiff(wm.realDat, nwm.realDat, wm.size, res);
		f << ' ' << res;
		imf.attackGuss(nwi, wi, 10);
		sw.getWm(nwm, nwi);
		wmDiff(wm.realDat, nwm.realDat, wm.size, res);
		f << ' ' << res;
		f << "\n";
	}
	f.flush();
	f.close();

	sw.addWm(wi,im, wm, 1.8);

	// 改变对比度50
	f.open("a18_cchange.hex",ios::out);
	f << 50 << endl;
	for (unsigned i = 0; i < 50; i++) {
		a = i / 50.0;
		f << a;
		imf.changeContrast(nwi,wi, a);
		sw.getWm(nwm,nwi);
		wmDiff(wm.realDat, nwm.realDat, wm.size, res);
		f << " " << res;
		f << "\n";
	}
	nouse(f, 50, 2);
	f.flush();
	f.close();
	// 改变亮度50
	f.open("a18_bchange.hex",ios::out);
	f << 50 << endl;
	for (unsigned i = 0; i < 50; i++) {
		a = i / 50.0;
		f << a;
		imf.changeBright(nwi,wi, a);
		sw.getWm(nwm,nwi);
		wmDiff(nwm.realDat, wm.realDat, wm.size, res);
		sprintf(buf, "b%d.bmp", i);
		imf.saveImage(nwi, buf);
		sprintf(buf, "bw%d.bmp", i);
		imf.saveImage(nwm, buf);
		
		f << " " << res;
		f << "\n";
	}
	nouse(f, 50, 2);
	f.flush();
	f.close();
	// 改变高斯攻击50
	f.open("a18_gchange.hex",ios::out);
	f << 50 << endl;
	for (unsigned i = 0; i < 50; i++) {
		a = i / 5.0;
		f << a;
		imf.attackGuss(nwi,wi, a);
		sw.getWm(nwm,nwi);
		wmDiff(wm.realDat, nwm.realDat, wm.size, res);
		f << " " << res;
		f << "\n";
		if (i >= 23 && i <= 49) {
			sprintf(buf, "wm%db.bmp", i);
			imf.saveImage(nwm, buf);
		}
	}
	nouse(f, 50, 2);
	f.flush();
	f.close();
	// jpeg压缩50
	f.open("a18_jchange.hex", ios::out);
	f << 100 << endl;
	imf.setJpegN(8);
	for (unsigned i = 0; i < 100; i++) {
		f << i;
		imf.jpegCompress(nwi, wi, i+1);
		sw.getWm(nwm, nwi);
		wmDiff(wm.realDat, nwm.realDat, wm.size, res);
		f << " " << res;
		f << "\n";
		if (i >= 23 && i <= 100) {
			sprintf(buf, "wm%db.bmp", i);
			imf.saveImage(nwm, buf);
			sprintf(buf, "im%db.bmp", i);
			imf.saveImage(nwi, buf);
		}
	}
	nouse(f, 100, 2);
	f.flush();
	f.close();
}
int main() {
	double res;

	cout << "数据隐藏..." << endl;
	SsWm sw(8);
	ImgFac &imf=ImgFac::instance();
	Image origin=imf.readImage("lena.bmp");
	Image wm = imf.readImage("tj-logo.bmp");
	imf.setImgModel(origin);
	imf.setWmModel(wm);
	WmImage wi = sw.addWm(origin, wm,8,8);
	imf.saveImage(wi, "new.bmp");
	Image newWm = sw.getWm(wi);
	imf.saveImage(newWm, "wme.bmp");

	Image reim = imf.readImage("result(1).bmp");
	WmImage rewi;
	memcpy(&rewi, &reim, sizeof(Image));
	rewi.blkS = wi.blkS;
	rewi.dctMat = wi.dctMat;
	rewi.K = 32;
	rewi.N = 128;
	rewi.wMat = wi.wMat;
	rewi.start = 16;
	Image rewm = sw.getWm(rewi);
	imf.saveImage(rewm, "result_wm.bmp");
	cout << "ok" << endl;


	testSsWm(imf, origin, wm);
	cout << "测试完成，按任意键继续" << endl;
	_getch();
	

	cout << "\n蒙特卡罗仿真..." << endl;
	Roc r;
	r.setX(4190832, 512345351);
	r.setX(10, 3279841, 95335098432, 34881932, 4719325, 2839719324);
	r.setV(8590283, 5839123);
	r.setWRand(2525529);
	r.setRange(1e-6, 1e-1,50);
	r.set(1000);
	// x~GGD(0,10),a=1.8,c=2
	r.test("noatk_a18_c20.hex", 1.8, 2, 1, 1000000, Roc::ggd2);
	// c=1
	r.test("noatk_a18_c10.hex", 1.8, 1, 1, 1000000, Roc::ggd1);
	// c=0.5
	r.test("noatk_a18_c05.hex", 1.8, 0.5, 1, 1000000, Roc::ggd05);
	// a=1.5
	r.test("noatk_a15_c20.hex", 1.5, 2, 1, 1000000, Roc::ggd2);
	
	// x~N(0,10),a=1.8,no noise
	r.test("noatk_a18_n25.hex", 1.8, 0, 1, 1000000, 0);
	r.test("noatk_a18_n50.hex", 1.8, 0, 1, 1000000, 0);
	// x~N(0,10),a=1.8,noise~N(0,2.5)
	r.test("atk_a18_n25.hex", 1.8, 2.5, 1, 1000000, 1);
	// x~N(0,10,a=1.8,noise~N(0,5)
	r.test("atk_a18_n50.hex", 1.8, 5, 1, 1000000, 1);
	cout << "仿真完成，按任意键继续" << endl;
	_getch();

	return 0;
}

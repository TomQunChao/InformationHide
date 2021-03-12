
#include "image.h"
//ImgFac ImgFac::imf = ImgFac();
ImgFac *ImgFac::imf = NULL;
Image::~Image() {
	//delete[] data;
}
ImgFac::ImgFac() {
}
ImgFac& ImgFac::instance() {
	if (!imf)imf = new ImgFac;
	return *imf;
}
Image ImgFac::readImage(string filename) {
	fstream f;
	f.open(filename,ios::in|ifstream::binary);
	Image im = {0};
	if (!f)return im;
	char buf[128];
	f.read(buf, sizeof(im.bfType));
	memcpy(&im.bfType, buf, sizeof(im.bfType));
	f.read(buf, sizeof(im.bfSize));
	memcpy(&im.bfSize, buf, sizeof(im.bfSize));
	f.seekg(10, ios::beg);
	f.read(buf, sizeof(im.bfOffBits));
	memcpy(&im.bfOffBits, buf, sizeof(im.bfOffBits));
	f.seekg(14, ios::beg);
	f.read(buf, sizeof(im.biSize));
	memcpy(&im.biSize, buf, sizeof(im.biSize));
	f.seekg(18, ios::beg);
	f.read(buf, sizeof(im.w));
	memcpy(&im.w, buf, sizeof(im.w));
	f.seekg(22, ios::beg);
	f.read(buf, sizeof(im.h));
	memcpy(&im.h, buf, sizeof(im.h));
	im.data = new char[im.bfSize];	//整个文件，包括文件头
	f.seekg(ios::beg);	// 重新定位到文件起始
	f.read(im.data, sizeof(char) * im.bfSize);	// 读出所有数据
	f.close();
	im.realDat = &im.data[im.bfOffBits];
	im.size = im.bfSize - im.bfOffBits;

	return im;
}
Image ImgFac::genImage(int size) {
	if (!size)size = imgModel.size;
	Image im;
	memcpy(&im, &imgModel, sizeof(Image));
	im.bfSize = im.bfOffBits + size;
	im.size = size;
	im.data = new char[im.bfSize];
	im.realDat = &im.data[im.bfOffBits];
	memset(im.realDat, 0, sizeof(char) * im.size);
	memcpy(im.data, imgModel.data, sizeof(char) * im.bfSize);
	for (int i = 0; i < size; i++) {
		im.realDat[i] |= (unsigned char)(r.extract(0, 256));
	}
	return im;
}
Image ImgFac::genWm(int size) {
	if (!size)size = wmModel.size;
	Image im;
	memcpy(&im, &wmModel, sizeof(Image));
	im.bfSize = im.bfOffBits + size;
	im.size = size;
	im.data = new char[im.bfSize];
	im.realDat = &im.data[im.bfOffBits];
	memcpy(im.data, wmModel.data, sizeof(char) * im.bfSize);
	memset(im.realDat, 0, sizeof(char) * im.size);
	for (int i = 0; i < size; i++) {
		if(r.extract(0,1)>0.5)im.realDat[i >> 3] |= (1 << (7 - (i & 0x7)));
	}
	return im;
}
void ImgFac::changeBright(Image &des,const Image& src, double bright){
	for (unsigned i = 0; i < src.size; i++) {
		double x = (unsigned char)(src.realDat[i]) + bright * 255;
		if (x < 0)x = 0;
		if (x > 255)x = 255;
		des.realDat[i] = char(x);
	}
}
void ImgFac::changeContrast(Image &des,const Image& src, double contrast){
	double k = tan((45 + 44 * contrast) / 180 * 3.1415926535);
	for (unsigned i = 0; i < src.size; i++) {
		double x = ((unsigned char)(src.realDat[i])-127.5)*k + 127.5;
		if (x < 0)x = 0;
		if (x > 255)x = 255;
		des.realDat[i] = char(x+0.5);
	}
}
void ImgFac::attackGuss(Image& dst,const Image &src,const double std){
	BoxMuller b(1000, 2000);
	for (unsigned i = 0; i < src.size; i++) {
		double x = (unsigned char)(src.realDat[i]) +b.extract()*std;
		if (x < 0)x = 0;
		if (x > 255)x = 255;
		dst.realDat[i] = char(x);
	}
}
void ImgFac::jpegCompress(Image& des, const Image& src, const unsigned level){
	if (!level)return;
	double qua = level > 50 ? (100.0 - level) / 50 : 50.0 / level;
	for (int i = 0; i < jpegN*jpegN; i++) {
		Q[i] = int(jpegMat[i] * qua);
		if (Q[i] > 255)Q[i] = 255;
		if (Q[i] < 0)Q[i] = 0;
	}
	char* srcD = src.realDat;
	char* desD = des.realDat;
	double* dctTemp = (double*)alloca(sizeof(double) * jpegN * jpegN);
	char* t1 = (char*)alloca(sizeof(char) * jpegN * jpegN);
	for (unsigned i = 0; i < src.w; i += 8) {
		for (unsigned j = 0; j < src.h; j += 8) {
			for (unsigned k = 0; k < jpegN * jpegN; k++) {
				t1[k] = srcD[(i + k / jpegN) * src.w + k % jpegN + j]-128;
			}
			dct(t1, dctTemp, dctMat, jpegN);
			// quantization
			for (unsigned k = 0; k < jpegN*jpegN&&qua!=0; k++) {
				double t = dctTemp[k];
				if(Q[k])
					dctTemp[k] = int(dctTemp[k] / Q[k] + 0.5)*Q[k];
				//printf("%f\n", dctTemp[k]-t);
			}
			//printf("\n");
			dctR(dctTemp, t1, dctMat, jpegN);
			for (unsigned k = 0; k < jpegN*jpegN; k++) {
				desD[(i + k / jpegN) * src.w + k % jpegN + j] = t1[k] + 128;
			}
			
		}
	}
}
bool ImgFac::saveImage(const Image &im,string filename) {
	if (filename.length() == 0)filename = time(NULL) + ".bmp";
	fstream f;
	f.open(filename, ios::out | ostream::binary);
	if (!f)return 0;
	f.write(im.data, sizeof(char) * im.bfSize);
	f.flush();
	f.close();
	return 1;
}
void ImgFac::setRandom(const Random &rm) {
	memcpy(&r, &rm, sizeof(Random));
}
void ImgFac::setImgModel(const Image& im) {
	memcpy(&this->imgModel, &im, sizeof(Image));
	imgModel.data = new char[imgModel.bfSize];
	memcpy(this->imgModel.data, im.data, sizeof(char) * im.bfSize);
	imgModel.realDat = &imgModel.data[imgModel.bfOffBits];
}
void ImgFac::setWmModel(const Image& im) {
	memcpy(&this->wmModel, &im, sizeof(Image));
	wmModel.data = new char[wmModel.bfSize];
	memcpy(this->wmModel.data, im.data, sizeof(char) * im.bfSize);
	wmModel.realDat = &wmModel.data[wmModel.bfOffBits];
}
void ImgFac::setJpegN(const unsigned N){
	this->jpegN = N;//N 只能是8
	delete[] zzMat;
	delete[] dctMat;
	zzMat = new int[N * N];
	dctMat = new double[N * N];
	// init dctMat
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			dctMat[ind(i, j, N)] = i ? sqrt(double(2) / N)
				* cos((2 * j + 1) * (i)*3.1415926535 / 2 / N)
				: 1 / sqrt(N);
		}
	}
	// init zzMat
	// i 行坐标 j 列坐标 cnt ZigZag数组下标
	int i = 0, j = 0, cnt = 0;
	// 是否计算完毕
	while (cnt < N * N) {
		// 判断边界
		while (i >= 0 && j < N) {
			// 一维数组表示二维数组，把二维坐标(i,j)映射到一维为i*4+j
			zzMat[cnt++] = i * N + j;
			// 向右上角移动
			i--;
			j++;
		}
		// 如果是上三角，则上面的函数结束后i<0,j<4
		// 是下三角,j=4
		i = j == N ? i + 2 : 0;
		j = j == N ? N - 1 : j;
		while (j >= 0 && i < N) {
			zzMat[cnt++] = i * N + j;
			// 向左下角移动
			j--;
			i++;
		}
		// 如果是上三角，结束后j<0,i<4
		// 下三角，i=4
		j = i == N ? j + 2 : 0;
		i = i == N ? N - 1 : i;
	}
	// init jpegMat,unknown
	
}
WmImage ImgFac::genWmImage(uint32_t size) {
	if (!size)size = imgModel.size;
	WmImage res;
	res.bfOffBits = imgModel.bfOffBits;
	res.bfSize = imgModel.bfOffBits + size;
	res.size = size;
	res.bfType = imgModel.bfType;
	res.biSize = size;
	res.w = imgModel.w;
	res.h = imgModel.h;
	res.data = new char[res.bfOffBits + size];
	memset(res.data, 0, sizeof(char) * res.bfSize);
	memcpy(res.data, imgModel.data, sizeof(char) * res.bfOffBits);
	res.realDat = &res.data[res.bfOffBits];
	return res;
}
//ImgFac& ImgFac::instance() {
//	return imf;
//}
ImgFac::~ImgFac() {
	/*delete imgModel.data;
	delete wmModel.data;
	delete imf;*/
}

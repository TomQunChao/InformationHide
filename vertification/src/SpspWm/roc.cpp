
#include "roc.h"

//Roc Roc::r = Roc();
Roc::Roc() {

}
//Roc& Roc::instance() {
//	static Roc r;
//	return r;
//}
void Roc::gen(Flag flag,double std,double nStd) {
	sumx = 0;
	sumwmx = 0;
	sumvx = 0;
	sumwmvx = 0;
	switch (flag) {
	case ggd05:
		for (unsigned i = 0; i < N; i++) {
			x[i] = ggX->extract05();
			xv[i] = x[i];
			sumx +=w[i]==1? x[i]:-x[i];
			sumwmx += (w[i]==1?x[i]:-x[i])+a;
			if (nStd) {
				double n = bmN->extract() * nStd;
				sumvx += (w[i] == 1 ? x[i] + n : -x[i] - n);
				sumwmvx += (w[i] == 1 ? x[i] + n : -x[i] - n) + a;
			}
		}
		break;
	case ggd1:
		for (unsigned i = 0; i < N; i++) {
			x[i] = ggX->extract1();
			xv[i] = x[i];
			sumx += w[i] == 1 ? x[i] : -x[i];
			sumwmx += (w[i] == 1 ? x[i] : -x[i]) + a;
			if (nStd) {
				double n = bmN->extract() * nStd;
				sumvx += (w[i] == 1 ? x[i] + n : -x[i] - n);
				sumwmvx += (w[i] == 1 ? x[i] + n : -x[i] - n) + a;
			}
		}
		break;
	case ggd2:
		for (unsigned i = 0; i < N; i++) {
			x[i] = bmX->extract() * std;
			xv[i] = x[i];
			sumx += w[i] == 1 ? x[i] : -x[i];
			sumwmx += (w[i] == 1 ? x[i] : -x[i]) + a;
			if (nStd) {
				double n = bmN->extract() * nStd;
				sumvx += (w[i] == 1 ? x[i] + n : -x[i] - n);
				sumwmvx += (w[i] == 1 ? x[i] + n : -x[i] - n) + a;
			}
		}
		break;
	case normal:
		for (unsigned i = 0; i < N; i++) {
			x[i] = bmX->extract() * std;
			xv[i] = x[i];
			sumx += (w[i] == 1 ? x[i] : -x[i]);
			sumwmx += (w[i] == 1 ? x[i] : -x[i]) + a;
			if (nStd) {
				double n = bmN->extract() * nStd;
				
				sumvx += (w[i] == 1 ? x[i] + n : -x[i] - n);
				sumwmvx += (w[i] == 1 ? x[i] + n : -x[i] - n) + a;
			}
		}
		break;
	}
	//for (unsigned i = 0; i < N; i++) {
	//	x[i] = m->extractDouble()*10-5;
	//	xv[i] = x[i];
	//}
}
double Roc::merfc(double x) {
	//double res=double(1) / 6 * exp(-x * x) + double(1) / 2 * exp(-4.0 / 3 * x * x);
	//cout << res - erfc(x) << endl;
	return erfc(x);
}
double Roc::ierfc(double y) {
	// inverse of the error function erfc
	// Copyright(C) 1996 Takuya OOURA (email: ooura@mmm.t.u-tokyo.ac.jp)

	const double IERFC_LIM = 27;
	double z = (y > 1) ? 2 - y : y;
	if (z < 1e-300) return (y > 1) ? -IERFC_LIM : IERFC_LIM;
	double w = 0.916461398268964 - log(z);
	double u = sqrt(w);
	double s = (log(u) + 0.488826640273108) / w;
	double t = 1 / (u + 0.231729200323405);
	double x = u * (1 - s * (s * 0.124610454613712 + 0.5)) -
		((((-0.0728846765585675 * t + 0.269999308670029) * t +
			0.150689047360223) * t + 0.116065025341614) * t +
			0.499999303439796) * t;
	t = 3.97886080735226 / (x + 3.97886080735226);
	u = t - 0.5;
	s = (((((((((0.00112648096188977922 * u +
		1.05739299623423047e-4) * u - 0.00351287146129100025) * u -
		7.71708358954120939e-4) * u + 0.00685649426074558612) * u +
		0.00339721910367775861) * u - 0.011274916933250487) * u -
		0.0118598117047771104) * u + 0.0142961988697898018) * u +
		0.0346494207789099922) * u + 0.00220995927012179067;
	s = ((((((((((((s * u - 0.0743424357241784861) * u -
		0.105872177941595488) * u + 0.0147297938331485121) * u +
		0.316847638520135944) * u + 0.713657635868730364) * u +
		1.05375024970847138) * u + 1.21448730779995237) * u +
		1.16374581931560831) * u + 0.956464974744799006) * u +
		0.686265948274097816) * u + 0.434397492331430115) * u +
		0.244044510593190935) * t -
		z * exp(x * x - 0.120782237635245222);
	x += s * (x * s + 1);
	return (y > 1) ? -x : x;
}
double Roc::Q(double x) {
	return 0.5 * merfc(x / sqrt(2));
}
double Roc::iQ(double x) {
	return sqrt(2) * ierfc(2 * x);
}
void Roc::attack(double nStd) {
	for (unsigned i = 0; i < N; i++) {
		x[i] += bmN->extract() * nStd;
	}
}
double Roc::sum(double *x) {
	double s = 0;
	double s1 = 0;
	for (int i = 0; i < N; i++) {
		s += x[i] * w[i];
		//s1 += x[i];
	}
	//cout << s << " "<<s1<<endl;
	return s;
}

void Roc::addWm(double a,double *x) {
	for (unsigned i = 0; i < N; i++) {
		x[i] += a * w[i];
	}
}
double Roc::theValue(double pfa, double a, double sigmax) {
	return 1 - Q(iQ(pfa) - sqrt(N) * a / sqrt(sigmax * sigmax));
}
double Roc::theValue(double pfa, double a, double sigmax, double sigmav) {
	return 1 - Q(iQ(pfa) - sqrt(N) * a / sqrt(sigmax * sigmax + sigmav * sigmav));
}
double Roc::getPsi(double pfa, double sigmax) {
	return sigmax * iQ(pfa) / sqrt(N);
}
// 无攻击
void Roc::test(string filename, double a, double c, int epN, int epS, Flag flag) {
	double pm = 0, pfa = 0;
	double psi = 0;
	fstream f;
	this->a = a;
	f.open(filename, ios::out);
	f << bins << endl;
	// 每一轮计算实验性能
	for (unsigned i = 0; i < epN; i++) {
		// 计算实验性能
		cout << "\n第" << i << "轮" << endl;
		memset(pfaMat, 0, sizeof(uint32_t) * bins);
		memset(pmMat, 0, sizeof(uint32_t) * bins);
		for (unsigned j = 0; j < epS; j++) {
			gen(flag,10);
			// [9e-6,9e-1)
			for (unsigned k = 0; k < bins; k++) {
				psi = getPsi(range[k],10)*N;
				if (sumx > psi)pfaMat[k]++;
				if (sumwmx < psi)pmMat[k]++;
			}
		}
		for (unsigned k = 0; k < bins; k++) {
			cout << pfaMat[k] / double(epS) << " " << pmMat[k] / double(epS) << endl;
			f << pfaMat[k] / double(epS) << " " << pmMat[k] / double(epS) << "\n";
		}
		

	}
	// 输出理论值
	cout << "理论性能" << endl;
	for (unsigned i = 0; i < bins; i++) {
		pfa = range[i];
		cout << pfa << " " << theValue(pfa, a, 10)<<endl;
		f << pfa << " " << theValue(pfa, a, 10)<<endl;
	}
	f.flush();
	f.close();
}
// 有攻击
void Roc::test(string filename, double a, double nStd, int epN, int epS, bool isNoise) {
	double res, pm, pfa, pmAtk, pfaAtk;
	double psi = 0;
	fstream f;
	this->a = a;
	f.open(filename, ios::out);
	f << bins*epN << endl;
	if (isNoise) {	// 受到攻击时的实验性能
		for (int i = 0; i < epN; i++) {
			cout << "\n受到攻击时的实验性能，第" << i << "轮" << endl;
			memset(pfaMat, 0, sizeof(int) * bins);
			memset(pmMat, 0, sizeof(int) * bins);
			for (int j = 0; j < epS; j++) {
				gen(normal, 10, nStd);
				for (int k = 0; k < bins; k++) {
					psi = getPsi(range[k], 10) * N;
					//cout << sumvx << " " << sumwmvx << " " << sumwmvx - sumvx << endl;
					if (sumvx > psi)pfaMat[k]++;
					if (sumwmvx < psi)pmMat[k]++;
				}
			}
			for (int k = 0; k < bins; k++) {
				cout << pfaMat[k] / double(epS) << " " << pmMat[k] / double(epS) << endl;
				f << pfaMat[k] / double(epS) << " " << pmMat[k] / double(epS) << "\n";
			}
		}
	}
	else {
		// statics of no use
		for (unsigned i = 0; i < bins; i++) {
			f << "0" << " " << "0\n";
		}
	}
	// 输出理论性能
	cout << "理论性能" << endl;
	for (int i = 0; i < 50; i++) {
		pfa = range[i];
		cout << pfa  << " " << theValue(pfa, a, 10, nStd) << endl;
		f << pfa <<" "<<theValue(pfa,a,10,nStd) << "\n";
	}
	f.flush();
	f.close();
}
void Roc::set(unsigned N) {
	this->N = N;
	delete[] x;
	x = new double[N];
	delete[] w;
	w = new char[N];
	delete[] xv;
	xv = new double[N];
	memset(w, 1, sizeof(char) * N / 2);
	memset(&w[N / 2], -1, sizeof(char) * N / 2);
	for (unsigned i = 0; i < N; i++) {
		unsigned p = unsigned(m->extractDouble() * (N-i)+i);
		swap(w[i], w[p]);
	}

}
void Roc::setV(int s1, int s2) {
	delete bmN;
	bmN = new BoxMuller(s1, s2);
}
void Roc::setX(double beta, int s1, int s2, int s3, int s4, int s5) {
	delete ggX;
	ggX = new GgdGen(beta, s1, s2, s3, s4, s5);
}
void Roc::setX(int s1, int s2) {
	delete bmX;
	bmX = new BoxMuller(s1, s2);
}
void Roc::setWRand(int s) {
	delete m;
	m = new Mt19937(s);
}

void Roc::setRange(double d1, double d2,unsigned bins) {
	delete[] range;
	delete[] pmMat;
	delete[] pfaMat;
	this->bins = bins;
	range = new double[bins];
	pmMat = new uint32_t[bins];
	pfaMat = new uint32_t[bins];
	double low = log10(d1);
	double high = log10(d2);
	for (unsigned i = 0; i < bins; i++) {
		range[i] = pow(10, low + i * (high - low) / bins);
	}
}

void Roc::testAll(unsigned epN, unsigned epS, unsigned bin) {
	fstream f;
	unsigned pfa[50], pm[50], vpfa[50], vpm[50];
	double psi;
	for (unsigned i = 0; i < epN; i++) {
		for (unsigned j = 0; j < epS; j++) {
			
		}
	}
	for (int i = 0; i < 50; i++) {
		cout <<merfc(-i/50.0)<<" "<<erfc(-i/50.0)<<" "<< merfc(-i/50.0) - erfc(-i/50.0) << endl;
	}
	
}

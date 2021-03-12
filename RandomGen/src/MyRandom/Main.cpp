#include "Random.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <conio.h>

using namespace std;

int main() {
	ExpGen em(1000, 5);
	BoxMuller bm(1000, 2000);
	GgdGen gm(4, 1000, 2000, 3000, 4000, 5000);
	const int RAND_NUM = 100000;
	double* normalArr = new double[RAND_NUM],
		* expArr = new double[RAND_NUM],
		* ggd05Arr = new double[RAND_NUM],
		* ggd1Arr = new double[RAND_NUM];

	////将数据打印到文件
	fstream norPw("normal.hex",ios::out),
		expPw("exp.hex",ios::out),
		ggd05Pw("ggd05.hex",ios::out),
		ggd1Pw("ggd1.hex",ios::out);
	norPw << RAND_NUM << endl;
	expPw << RAND_NUM << endl;
	ggd05Pw << RAND_NUM << endl;
	ggd1Pw << RAND_NUM << endl;

	for (int i = 0; i < RAND_NUM; i++) {
		normalArr[i] = bm.extract();
		expArr[i] = em.extract();
		ggd05Arr[i] = gm.extract05();
		ggd1Arr[i] = gm.extract1();

		norPw<<normalArr[i]<<endl;
		expPw<<expArr[i]<<endl;
		ggd05Pw<<ggd05Arr[i]<<endl;
		ggd1Pw<<ggd1Arr[i]<<endl;
	}
	////拟合数据
	est::NorEstRes res1= est::NorMaxLike(normalArr,RAND_NUM);
	norPw<<res1.mu<<" "<<res1.delta2<<endl;
	est::NorEstRes res2 = est::NormalMoment(normalArr,RAND_NUM);
	norPw << res2.mu << " " << res2.delta2 << endl;
	expPw << est::exp(expArr, RAND_NUM);

	////输出拟合结果
	printf("Normal Distribution -- Maximum Likelihood Estimation: mu=%f, delta=%f\n"
		, res1.mu, res1.delta2);
	printf("Normal Distribution -- Moment Estimation: mu=%f, delta=%f\n",
		res2.mu, res2.delta2);
	printf("Exponential Distribution -- delta=%f\n",
		est::exp(expArr,RAND_NUM));
	printf("GGD c=0.5, c=%f\n", est::ggd(ggd05Arr,RAND_NUM));
	printf("GGD c=1, c=%f\n", est::ggd(ggd1Arr,RAND_NUM));

	printf("GGD c=2, c=%f\n", est::ggd(normalArr,RAND_NUM));
	printf("GGD Exp c=1, c=%f\n", est::ggd(expArr,RAND_NUM));


	delete[]normalArr;
	delete[]expArr;
	delete[]ggd05Arr;
	delete[]ggd1Arr;

	////关闭打印流
	expPw.flush();
	ggd05Pw.flush();
	ggd1Pw.flush();
	norPw.flush();
	expPw.close();
	ggd05Pw.close();
	norPw.close();
	ggd1Pw.close();

	cout << "按任意键继续" << endl;
	_getch();
	return 0;
}

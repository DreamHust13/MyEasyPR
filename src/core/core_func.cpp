// ����ļ�������EasyPR������plate�жϵ�ͨ�ú���
// ���������ռ�Ϊeasypr
// ��������еĺ������ײ�Ҫ�Ķ�

#include "../include/prep.h"
#include "../include/core_func.h"

/*! \namespace easypr
Namespace where all the C++ EasyPR functionality resides
*/
namespace easypr {

	//! ����һ��ͼ������ɫģ���ȡ��Ӧ�Ķ�ֵͼ
	//! ����RGBͼ��, ��ɫģ�壨��ɫ����ɫ��
	//! ����Ҷ�ͼ��ֻ��0��255����ֵ��255����ƥ�䣬0����ƥ�䣩
	Mat colorMatch(const Mat& src, Mat& match, const Color r, const bool adaptive_minsv)
	{
		// S��V����Сֵ��adaptive_minsv���boolֵ�ж�
		// ���Ϊtrue������Сֵȡ����Hֵ��������˥��
		// ���Ϊfalse����������Ӧ��ʹ�ù̶�����Сֵminabs_sv
		// Ĭ��Ϊfalse
		const float max_sv = 255;
		const float minref_sv = 64;

		const float minabs_sv = 95;

		//blue��H��Χ
		const int min_blue = 100;  //100
		const int max_blue = 140;  //140

		//yellow��H��Χ
		const int min_yellow = 15; //15
		const int max_yellow = 40; //40

		Mat src_hsv;
		// ת��HSV�ռ���д�����ɫ������Ҫʹ�õ���H����������ɫ���ɫ��ƥ�乤��
		cvtColor(src, src_hsv, CV_BGR2HSV);

		vector<Mat> hsvSplit;
		split(src_hsv, hsvSplit);
		//Split():�ָ��ͨ������ɼ�����ͨ��������ߴ���������ȡһ��ͨ��
		equalizeHist(hsvSplit[2], hsvSplit[2]);
		//EqualizeHist():�Ҷ�ͼ��ֱ��ͼ���⻯  �÷�����һ��ͼ�����Ⱥ���ǿ�Աȶȡ�
		merge(hsvSplit, src_hsv);
		//Merge():split()�ķ���������Ӽ�����ͨ��������ϳɶ�ͨ����������һ����ͨ������

		//ƥ��ģ���ɫ,�л��Բ�����Ҫ�Ļ�ɫ
		int min_h = 0;
		int max_h = 0;
		switch (r) {
		case BLUE:
			min_h = min_blue;
			max_h = max_blue;
			break;
		case YELLOW:
			min_h = min_yellow;
			max_h = max_yellow;
			break;
		}

		float diff_h = float((max_h - min_h) / 2);
		int avg_h = min_h + diff_h;

		int channels = src_hsv.channels();
		int nRows = src_hsv.rows;
		//ͼ����������Ҫ����ͨ������Ӱ�죻
		int nCols = src_hsv.cols * channels;

		if (src_hsv.isContinuous())//�����洢�����ݣ���һ�д���
		{
			nCols *= nRows;
			nRows = 1;
		}

		int i, j;
		uchar* p;
		float s_all = 0;
		float v_all = 0;
		float count = 0;
		for (i = 0; i < nRows; ++i)
		{
			p = src_hsv.ptr<uchar>(i);
			for (j = 0; j < nCols; j += 3)
			{
				int H = int(p[j]); //0-180
				int S = int(p[j + 1]);  //0-255
				int V = int(p[j + 2]);  //0-255

				s_all += S;
				v_all += V;
				count++;

				bool colorMatched = false;

				if (H > min_h && H < max_h)
				{
					int Hdiff = 0;
					if (H > avg_h)
						Hdiff = H - avg_h;
					else
						Hdiff = avg_h - H;

					float Hdiff_p = float(Hdiff) / diff_h;

					// S��V����Сֵ��adaptive_minsv���boolֵ�ж�
					// ���Ϊtrue������Сֵȡ����Hֵ��������˥��
					// ���Ϊfalse����������Ӧ��ʹ�ù̶�����Сֵminabs_sv
					float min_sv = 0;
					if (true == adaptive_minsv)
						min_sv = minref_sv - minref_sv / 2 * (1 - Hdiff_p); // inref_sv - minref_sv / 2 * (1 - Hdiff_p)
					else
						min_sv = minabs_sv; // add

					if ((S > min_sv && S < max_sv) && (V > min_sv && V < max_sv))
						colorMatched = true;
				}

				if (colorMatched == true) {
					p[j] = 0; p[j + 1] = 0; p[j + 2] = 255;
				}
				else {
					p[j] = 0; p[j + 1] = 0; p[j + 2] = 0;
				}
			}
		}

		//cout << "avg_s:" << s_all / count << endl;
		//cout << "avg_v:" << v_all / count << endl;

		// ��ȡ��ɫƥ���Ķ�ֵ�Ҷ�ͼ
		Mat src_grey;
		vector<Mat> hsvSplit_done;
		split(src_hsv, hsvSplit_done);
		src_grey = hsvSplit_done[2];

		match = src_grey;

		return src_grey;
	}


	//! �ж�һ�����Ƶ���ɫ
	//! ���복��mat����ɫģ��
	//! ����true��fasle
	bool plateColorJudge(const Mat& src, const Color r, const bool adaptive_minsv)
	{
		// �ж���ֵ
		const float thresh = 0.5;

		Mat src_gray;
		colorMatch(src, src_gray, r, adaptive_minsv);

		float percent = float(countNonZero(src_gray)) / float(src_gray.rows * src_gray.cols);
		// cout << "percent:" << percent << endl;

		if (percent > thresh)
			return true;
		else
			return false;
	}

	//getPlateType
	//�жϳ��Ƶ�����
	Color getPlateType(const Mat&  src, const bool adaptive_minsv)
	{
		if (plateColorJudge(src, BLUE, adaptive_minsv) == true) {
			//cout << "BLUE" << endl;
			return BLUE;
		}
		else if (plateColorJudge(src, YELLOW, adaptive_minsv) == true) {
			//cout << "YELLOW" << endl;
			return YELLOW;
		}
		else {
			//cout << "OTHER" << endl;
			return BLUE;
		}
	}

	//clearLiuDing
	//ȥ�������Ϸ���ť��
	//����ÿ��Ԫ�صĽ�Ծ�������С��X��Ϊ��������������ȫ����0��Ϳ�ڣ�
	//X���Ƽ�ֵΪ���ɸ���ʵ�ʵ���
	Mat clearLiuDing(Mat img)
	{
		const int x = 7;
		Mat jump = Mat::zeros(1, img.rows, CV_32F);
		for (int i = 0; i < img.rows; i++)
		{
			int jumpCount = 0;
			for (int j = 0; j < img.cols - 1; j++)
			{
				if (img.at<char>(i, j) != img.at<char>(i, j + 1))
					jumpCount++;
			}
			jump.at<float>(i) = jumpCount;
		}
		for (int i = 0; i < img.rows; i++)
		{
			if (jump.at<float>(i) <= x)
			{
				for (int j = 0; j < img.cols; j++)
				{
					img.at<char>(i, j) = 0;
				}
			}
		}
		return img;
	}


	//! ֱ��ͼ����
	Mat histeq(Mat in)
	{
		Mat out(in.size(), in.type());
		if (in.channels() == 3)
		{
			Mat hsv;
			vector<Mat> hsvSplit;
			cvtColor(in, hsv, CV_BGR2HSV);
			split(hsv, hsvSplit);
			equalizeHist(hsvSplit[2], hsvSplit[2]);
			merge(hsvSplit, hsv);
			cvtColor(hsv, out, CV_HSV2BGR);
		}
		else if (in.channels() == 1)
		{
			equalizeHist(in, out);
		}
		return out;
	}


	// ����ȡ��ֱ��ˮƽ����ֱ��ͼ
	Mat ProjectedHistogram(Mat img, int t)
	{
		int sz = (t) ? img.rows : img.cols;
		Mat mhist = Mat::zeros(1, sz, CV_32F);

		for (int j = 0; j<sz; j++){
			Mat data = (t) ? img.row(j) : img.col(j);
			mhist.at<float>(j) = countNonZero(data);	//ͳ����һ�л�һ���У�����Ԫ�صĸ����������浽mhist��
		}

		//Normalize histogram
		double min, max;
		minMaxLoc(mhist, &min, &max);

		if (max>0)
			mhist.convertTo(mhist, -1, 1.0f / max, 0);//��mhistֱ��ͼ�е����ֵ����һ��ֱ��ͼ

		return mhist;
	}

}	/* \namespace easypr  */
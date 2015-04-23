#ifndef __CORE_FUNC_H__
#define __CORE_FUNC_H__

/*! \namespace easypr
Namespace where all the C++ EasyPR functionality resides
*/
namespace easypr {

	enum Color{ BLUE, YELLOW };

	//! ����һ��ͼ������ɫģ���ȡ��Ӧ�Ķ�ֵͼ
	//! ����RGBͼ��, ��ɫģ�壨��ɫ����ɫ��
	//! ����Ҷ�ͼ��ֻ��0��255����ֵ��255����ƥ�䣬0����ƥ�䣩
	Mat colorMatch(const Mat& src, Mat& match, const Color r, const bool adaptive_minsv);

	//! �ж�һ�����Ƶ���ɫ
	//! ���복��mat����ɫģ��
	//! ����true��fasle
	bool plateColorJudge(const Mat&  src, const Color r, const bool adaptive_minsv);

	//clearLiuDing
	//ȥ�������Ϸ���ť��
	//����ÿ��Ԫ�صĽ�Ծ�������С��X��Ϊ��������������ȫ����0��Ϳ�ڣ�
	//X���Ƽ�ֵΪ���ɸ���ʵ�ʵ���
	Mat clearLiuDing(Mat img);

	//! ��ó�����ɫ
	Color getPlateType(const Mat&  src, const bool adaptive_minsv);

	//! ֱ��ͼ����
	Mat histeq(Mat in);

	// ����ȡ��ֱ��ˮƽ����ֱ��ͼ
	Mat ProjectedHistogram(Mat img, int t);

}	/*! \namespace easypr*/

#endif
/* endif __CORE_FUNC_H__ */
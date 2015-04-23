//////////////////////////////////////////////////////////////////////////
// Name:	    plate_detect Header
// Version:		1.2
// Date:	    2014-09-28
// MDate:	    2015-03-13
// Author:	    liuruoze
// Copyright:   liuruoze
// Reference:	Mastering OpenCV with Practical Computer Vision Projects
// Reference:	CSDN Bloger taotao1233
// Desciption:  
// Defines CPlateDetect
//////////////////////////////////////////////////////////////////////////
#ifndef __PLATE_DETECT_H__
#define __PLATE_DETECT_H__

#include "prep.h"

#include "plate_locate.h"
#include "plate_judge.h"


/*! \namespace easypr
    Namespace where all the C++ EasyPR functionality resides
*/
namespace easypr {

class CPlateDetect
{
public:
	CPlateDetect();

	//! ���Ƽ�⣺���ƶ�λ���ж�
	int plateDetect(Mat, vector<Mat>&, int index = 0);

	//! ��ȳ��Ƽ�⣬ʹ����ɫ�����Sobel���ۺ�
	int plateDetectDeep(Mat src, vector<Mat>& resultVec, bool showDetectArea = true, int index = 0);

	int showResult(const Mat& result);

	//! װ��SVMģ��
	void LoadSVM(string s);

	//! ����ģʽ�빤ҵģʽ�л�
	inline void setPDLifemode(bool param){m_plateLocate->setLifemode(param);}

	//! �Ƿ�������ģʽ
	inline void setPDDebug(int param){ m_plateLocate->setDebug(param);}

	//! ��ȡ����ģʽ״̬
	inline int getPDDebug(){ return m_plateLocate->getDebug();}

	//! �������ȡ����
	inline void setGaussianBlurSize(int param){	m_plateLocate->setGaussianBlurSize(param);}
	inline int getGaussianBlurSize() const{	return m_plateLocate->getGaussianBlurSize();}

	inline void setMorphSizeWidth(int param){m_plateLocate->setMorphSizeWidth(param);}
	inline int getMorphSizeWidth() const{return m_plateLocate->getMorphSizeWidth();}

	inline void setMorphSizeHeight(int param){m_plateLocate->setMorphSizeHeight(param);}
	inline int getMorphSizeHeight() const{return m_plateLocate->getMorphSizeHeight();}

	inline void setVerifyError(float param){m_plateLocate->setVerifyError(param);}
	inline float getVerifyError() const { return m_plateLocate->getVerifyError();}
	inline void setVerifyAspect(float param){m_plateLocate->setVerifyAspect(param);}
	inline float getVerifyAspect() const { return m_plateLocate->getVerifyAspect();}

	inline void setVerifyMin(int param){m_plateLocate->setVerifyMin(param);}
	inline void setVerifyMax(int param){m_plateLocate->setVerifyMax(param);}

	inline void setJudgeAngle(int param){m_plateLocate->setJudgeAngle(param);}

	inline void setMaxPlates(float param){ m_maxPlates = param; }
	inline float getMaxPlates() const { return m_maxPlates; }

private:
	//! ����һ��ͼ������ж��ٳ���
	int m_maxPlates;

	//! ���ƶ�λ
	CPlateLocate* m_plateLocate;

	//! �����ж�
	CPlateJudge* m_plateJudge;
};

}	/*! \namespace easypr*/

#endif /* endif __PLATE_DETECT_H__ */
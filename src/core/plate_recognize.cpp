#include "../include/plate_recognize.h"

/*! \namespace easypr
    Namespace where all the C++ EasyPR functionality resides
*/
namespace easypr{

CPlateRecognize::CPlateRecognize()
{
	//cout << "CPlateRecognize" << endl;
	//m_plateDetect= new CPlateDetect();
	//m_charsRecognise = new CCharsRecognise();
}

////! װ��SVMģ��
//void CPlateRecognize::LoadSVM(string strSVM)
//{
//	m_plateDetect->LoadModel(strSVM.c_str());
//}
//
////! װ��ANNģ��
//void CPlateRecognize::LoadANN(string strANN)
//{
//	m_charsRecognise->LoadModel(strANN.c_str());
//}
//
//int CPlateRecognize::plateDetect(Mat src, vector<Mat>& resultVec)
//{
//	int result = m_plateDetect->plateDetect(src, resultVec);
//	return result;
//}
//
//int CPlateRecognize::charsRecognise(Mat plate, string& plateLicense)
//{
//	int result = m_charsRecognise->charsRecognise(plate, plateLicense);
//	return result;
//}

int CPlateRecognize::plateRecognize(Mat src, vector<string>& licenseVec)
{
	// ���Ʒ��鼯��
	vector<Mat> plateVec;
	
	// ���������Debugģʽ����������ʾ���е�ͼƬ
	bool showDetectArea = getPDDebug();

	// ������ȶ�λ��ʹ����ɫ��Ϣ�����Sobel
	int resultPD = plateDetectDeep(src, plateVec, showDetectArea, 0);

	if (resultPD == 0)
	{
		int num = plateVec.size();

		int resultCR = 0;
		for (int j = 0; j < num; j++)
		{
			Mat plate = plateVec[j];
			
			//��ȡ������ɫ
			string plateType = getPlateColor(plate);

			//��ȡ���ƺ�
			string plateIdentify = "";
			int resultCR = charsRecognise(plate, plateIdentify);
			if (resultCR == 0)
			{
				string license = plateType + ":" + plateIdentify;
				licenseVec.push_back(license);
			}
		}
	}

	return resultPD;
}

}	/*! \namespace easypr*/


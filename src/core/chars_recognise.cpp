#include "../include/chars_recognise.h"

/*! \namespace easypr
Namespace where all the C++ EasyPR functionality resides
*/
namespace easypr {

CCharsRecognise::CCharsRecognise()
    : m_charsSegment(nullptr), m_charsIdentify(nullptr) {
  m_charsSegment = new CCharsSegment();
  m_charsIdentify = new CCharsIdentify();
}

CCharsRecognise::~CCharsRecognise() {
  if (m_charsSegment) {
    delete m_charsSegment;
    m_charsSegment = nullptr;
  }
  if (m_charsIdentify) {
    delete m_charsIdentify;
    m_charsIdentify = nullptr;
  }
}

void CCharsRecognise::LoadANN(string s) {
  m_charsIdentify->LoadModel(s.c_str());
}

int CCharsRecognise::charsRecognise(Mat plate, string& plateLicense) {
  //�����ַ����鼯��
  vector<Mat> matVec;

  string plateIdentify = "";

  int result = m_charsSegment->charsSegment(plate, matVec);
  if (result == 0) {
    int num = matVec.size();
    for (int j = 0; j < num; j++) {
      Mat charMat = matVec[j];
      bool isChinses = false;

      //Ĭ���׸��ַ����������ַ�
      if (j == 0) isChinses = true;

      string charcater = m_charsIdentify->charsIdentify(charMat, isChinses);
      plateIdentify = plateIdentify + charcater;
    }
  }

  plateLicense = plateIdentify;

  return 0;
}

} /*! \namespace easypr*/
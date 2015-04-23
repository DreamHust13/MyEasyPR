#ifndef __UTIL_H__
#define __UTIL_H__

#include <vector>
#include <string>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace easypr {

class Utils {
 public:
  static long getTimestamp();

  /*
   * Get file name from a given path
   * bool postfix: including the postfix
   */
  static std::string getFileName(const std::string& path,
                                 const bool postfix = false);

  /*
   * Split the given string into segements by a delimiter
   */
  static std::vector<std::string> splitString(const std::string& str,
                                              const char delimiter);

  /*
   * returns the smaller of the two numbers
   */
  template <typename T>
  static T min(const T& v1, const T& v2) {
    return (v1 < v2) ? v1 : v2;
  }

  /*
 * Get files from a given folder
 * all: including all sub-folders
 */
  static std::vector<std::string> getFiles(const std::string& folder,
                                           const bool all = true);
};

}

//! levenshtein���룬���ڼ����������Ƶľ���
//��EasyPR����levenshtein�����������ʶ������ʵ���Ƶ����
template<class T>
unsigned int levenshtein_distance(const T &s1, const T & s2) {
	const size_t len1 = s1.size(), len2 = s2.size();
	std::vector<unsigned int> col(len2+1), prevCol(len2+1);
 
	for (unsigned int i = 0; i < prevCol.size(); i++)
		prevCol[i] = i;
	for (unsigned int i = 0; i < len1; i++) {
		col[0] = i+1;
		for (unsigned int j = 0; j < len2; j++)
            col[j+1] = easypr::Utils::min(easypr::Utils::min(prevCol[1 + j] + 1, col[j] + 1), \
								prevCol[j] + (s1[i]==s2[j] ? 0 : 1) );
		col.swap(prevCol);
	}
	return prevCol[len2];
}


/// test.cpp�з���
int testMain();

/// accuracy_test.cpp�з���
int acurayTest(const std::string&);

/// mc_data_prepare.cpp�з���
void getLearnData();
void Code2Province(const std::string& code, std::string& province);
void changeFileName();
void getPlateLicense(const std::string& filepath, std::string& plateLicense);

/// learn_prepare.cpp�з���
void label_data();

/// general_test_prepare.cpp�з���
int general_test();


//	deface.cpp�з���
//	������ʶ����
//	�ڷ������ݵ�general_test���ݼ������������ķ�����ͼ����д���
//	EasyPR��Դ��Ŀ�ǳ�ע�ر���ͼƬ�м�ʻ�˵���˽
int deface();

/// generate_gdts.cpp�з���
int generate_gdts();

#endif
/* endif __UTIL_H__ */

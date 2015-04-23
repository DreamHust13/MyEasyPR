#include "../include/chars_identify.h"

/*! \namespace easypr
    Namespace where all the C++ EasyPR functionality resides
*/
namespace easypr{

#define HORIZONTAL    1
#define VERTICAL    0
#define NDEBUG

//�й�����
const char strCharacters[] = {'0','1','2','3','4','5',\
	'6','7','8','9','A','B', 'C', 'D', 'E','F', 'G', 'H', /* û��I */\
	'J', 'K', 'L', 'M', 'N', /* û��O */ 'P', 'Q', 'R', 'S', 'T', \
	'U','V', 'W', 'X', 'Y', 'Z'}; 
const int numCharacter = 34; /* û��I��0,10��������24��Ӣ���ַ�֮�� */

//���¶�����ѵ��ʱ�õ��������ַ����ݣ�����ȫ�棬��Щʡ��û��ѵ����������û���ַ�
const string strChinese[] = {"zh_cuan" /* �� */, "zh_e" /* �� */,  "zh_gan" /* ��*/, \
	"zh_hei" /* �� */, "zh_hu" /* �� */,  "zh_ji" /* �� */, \
	"zh_jl" /* �� */, "zh_jin" /* �� */, "zh_jing" /* �� */, "zh_shan" /* �� */, \
	"zh_liao" /* �� */, "zh_lu" /* ³ */, "zh_min" /* �� */, "zh_ning" /* �� */, \
	"zh_su" /* �� */,  "zh_sx" /* �� */, "zh_wan" /* �� */,\
	 "zh_yu" /* ԥ */, "zh_yue" /* �� */, "zh_zhe" /* �� */};

const int numChinese = 20;
const int numAll = 54; /* 34+20=54 */

CCharsIdentify::CCharsIdentify()
{
	//cout << "CCharsIdentify" << endl;
	m_predictSize = 10;
	m_path = "model/ann.xml";
	LoadModel();

	if (m_map.empty())
	{
		m_map.insert(pair<string, string>("zh_cuan","��"));
		m_map.insert(pair<string, string>("zh_e","��"));
		m_map.insert(pair<string, string>("zh_gan","��"));
		m_map.insert(pair<string, string>("zh_hei","��"));
		m_map.insert(pair<string, string>("zh_hu","��"));
		m_map.insert(pair<string, string>("zh_ji","��"));
		m_map.insert(pair<string, string>("zh_jl","��"));
		m_map.insert(pair<string, string>("zh_jin","��"));
		m_map.insert(pair<string, string>("zh_jing","��"));
		m_map.insert(pair<string, string>("zh_shan","��"));
		m_map.insert(pair<string, string>("zh_liao","��"));
		m_map.insert(pair<string, string>("zh_lu","³"));
		m_map.insert(pair<string, string>("zh_min","��"));
		m_map.insert(pair<string, string>("zh_ning","��"));
		m_map.insert(pair<string, string>("zh_su","��"));
		m_map.insert(pair<string, string>("zh_sx","��"));
		m_map.insert(pair<string, string>("zh_wan","��"));
		m_map.insert(pair<string, string>("zh_yu","ԥ"));
		m_map.insert(pair<string, string>("zh_yue","��"));
		m_map.insert(pair<string, string>("zh_zhe","��"));
	}
}


void CCharsIdentify::LoadModel()
{
	ann.clear();
	ann.load(m_path.c_str(), "ann");
}

void CCharsIdentify::LoadModel(string s)
{
	ann.clear();
	ann.load(s.c_str(), "ann");
}

//create the accumulation histograms,img is a binary image, t is ˮƽ��ֱ
Mat CCharsIdentify::ProjectedHistogram(Mat img, int t)
{
	int sz=(t)?img.rows:img.cols;
	Mat mhist=Mat::zeros(1,sz,CV_32F);

	for(int j=0; j<sz; j++){
		Mat data=(t)?img.row(j):img.col(j);
		mhist.at<float>(j)=countNonZero(data);	//ͳ����һ�л�һ���У�����Ԫ�صĸ����������浽mhist��
	}

	//Normalize histogram
	double min, max;
	minMaxLoc(mhist, &min, &max);

	if(max>0)
		mhist.convertTo(mhist,-1 , 1.0f/max, 0);//��mhistֱ��ͼ�е����ֵ����һ��ֱ��ͼ

	return mhist;
}

//! ����ַ�������ͼ
Mat CCharsIdentify::features(Mat in, int sizeData)
{
	//Histogram features
	Mat vhist=ProjectedHistogram(in, VERTICAL);
	Mat hhist=ProjectedHistogram(in, HORIZONTAL);

	//Low data feature
	Mat lowData;
	resize(in, lowData, Size(sizeData, sizeData) );

	//Last 10 is the number of moments components
	int numCols = vhist.cols + hhist.cols + lowData.cols*lowData.cols;

	Mat out = Mat::zeros(1, numCols, CV_32F);

	//Asign values to feature,ANN����������Ϊˮƽ����ֱֱ��ͼ�͵ͷֱ���ͼ������ɵ�ʸ��
	int j=0;
	for(int i=0; i<vhist.cols; i++)
	{
		out.at<float>(j)=vhist.at<float>(i);
		j++;
	}
	for(int i=0; i<hhist.cols; i++)
	{
		out.at<float>(j)=hhist.at<float>(i);
		j++;
	}
	for(int x=0; x<lowData.cols; x++)
	{
		for(int y=0; y<lowData.rows; y++){
			out.at<float>(j)=(float)lowData.at<unsigned char>(x,y);
			j++;
		}
	}

	return out;
}


//! ������������ʶ��
int CCharsIdentify::classify(Mat f, bool isChinses){
	int result = -1;
	Mat output(1, numAll, CV_32FC1);
	ann.predict(f, output);

	if (!isChinses)
	{
		result = 0;
		float maxVal = -2;
		for(int j = 0; j < numCharacter; j++)
		{
			float val = output.at<float>(j);
			//cout << "j:" << j << "val:"<< val << endl;
			if (val > maxVal)
			{
				maxVal = val;
				result = j;
			}
		}
	}
	else
	{
		result = numCharacter;
		float maxVal = -2;
		for(int j = numCharacter; j < numAll; j++)
		{
			float val = output.at<float>(j);
			//cout << "j:" << j << "val:"<< val << endl;
			if (val > maxVal)
			{
				maxVal = val;
				result = j;
			}
		}
	}
	return result;
}


//���뵱���ַ�Mat,�����ַ���string
string CCharsIdentify::charsIdentify(Mat input, bool isChinese)
{
	Mat f = features(input, m_predictSize);
	string result = "";
	int index = classify(f, isChinese);

	if (!isChinese)
	{
		result = result + strCharacters[index];
	}
	else
	{
		string s = strChinese[index - numCharacter];
		string province = m_map[s];
		result = province + result;
	}

	return result;
}



}	/*! \namespace easypr*/

#pragma once
#include "cwBasicCTAStrategy.h"

//����һ��������Ե�demo�����ڲ��Խ��ܣ����԰ٶ��£��к��꾡˵�����˴�����׸��
// �ò���ʵ����CTAPlatform��һ���Ӳ���ʵ�֣��������Ԥ�ڳֲ֣�
// ��CTAPlatform�ۺϸ�Ʒ�ֺ�Լ����Ԥ�ڳֲֲֳֺ��µ���������������ʵ�ʳֲֺͶ�������
//
class cwDualTrust :
    public cwBasicCTAStrategy
{
public:
	cwDualTrust(const char * szStrategyName);
	//������һ����K�ߵ�ʱ�򣬻���øûص�
	void			OnBar(bool bFinished, int iTimeScale, cwBasicKindleStrategy::cwKindleSeriesPtr pKindleSeries) override;
};


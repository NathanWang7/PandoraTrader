//////////////////////////////////////////////////////////////////////////////////
//*******************************************************************************
//---
//---	author: Wu Chang Sheng
//---
//--	Copyright (c) by Wu Chang Sheng. All rights reserved.
//--    Consult your license regarding permissions and restrictions.
//--
//*******************************************************************************
//////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
namespace cwCommonUtility
{

	//Math ��ֵ���ܳ���int�������������
	//����ȡֵ����������С��λ
	double cwCeil(double dValue, double dTickSize = 1);
	//����ȡֵ����������С��λ
	double cwFloor(double dValue, double dTickSize = 1);
	//�������룬��������С��λ
	inline double cwRound(double dValue, double dTickSize = 1) 
	{
		return cwFloor(dValue + 0.5 * dTickSize, dTickSize);
	};

	int    cwDouble2Int(double dValue, double dTickSize = 1);

	// TOOLs

	#define CW_DISALLOW_COPYCTOR_AND_ASSIGNMENT(TypeName) \
	private:\
		TypeName(const TypeName&); \
		TypeName& operator=(const TypeName&);
}

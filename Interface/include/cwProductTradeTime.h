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
#include <memory>
#include <map>
#include <vector>
#include "cwMarketTime.h"

//Ʒ�ֽ���ʱ��
class cwProductTradeTime
{
	
public:
	cwProductTradeTime();
	cwProductTradeTime(const char* filename);
	~cwProductTradeTime();

	enum cwTradeTimeSpace
	{
		NoTrading = 0										//�ǽ���ʱ��
		, NightPartOne = 1									//ҹ��
		, AMPartOne = 2									    //�����һ�׶�
		, AMPartTwo = 3									    //����ڶ��׶�
		, PMPartOne = 4									    //�����һ�׶�
		, AMCallAuctionOrderingOpen = 5					    //���Ͼ��۱��������翪�̣�
		, AMCallAuctionMatchOpen = 6						//���Ͼ��۴�ϣ����翪�̣�
		, NightCallAuctionOrderingOpen = 7					//���Ͼ��۱�����ҹ�̿��̣�
		, NightCallAuctionMatchOpen = 8					    //���Ͼ��۴�ϣ�ҹ�̿��̣�
		, CallAuctionOrderingClose = 9					    //���Ͼ��۱��������̣�
		, CallAuctionMatchClose = 10					    //���Ͼ��۴�ϣ����̣�
		, TradeTimeSpaceCnt
	};

	enum cwRangeOpenClose
	{
		cwLeftOpenRightOpen = 0,							//(a,b)�� ���ҿ�����
		cwLeftOpenRightClose = 1,							//(a,b]�� ���ұ�����
		cwLeftCloseRightOpen = 2,							//[a,b)�� ����ҿ�����
		cwLeftCloseRightClose = 3							//[a,b]�� ����ұ�����
	};

	typedef struct tagProductTradeTime
	{
		cwTradeTimeSpace TradeTimeSpace;
		cwRangeOpenClose RangeOpenClose;

		cwMarketTimePtr BeginTime;
		cwMarketTimePtr EndTime;

	}ProductTradeTime;
	typedef std::shared_ptr<ProductTradeTime> TradeTimePtr;

	//����Ʒ��ID,��ʱ���ȡ����ʱ��
	bool GetTradeTimeSpace(std::string ProductId, std::string updatetime,
		cwTradeTimeSpace& iTradeIndex, int& iOpen, int& iClose);
	bool GetTradeTimeSpace(std::string ProductId, uint32_t hour, uint32_t minute, uint32_t second,
		cwTradeTimeSpace& iTradeIndex, int& iOpen, int& iClose);
	inline bool GetTradeTimeSpace(std::vector<TradeTimePtr>& TradeTimeVec, cwPandoraTrader::cwMarketTime& time,
		cwTradeTimeSpace& iTradeIndex, int& iOpen, int& iClose);

	TradeTimePtr GetTradeTime(std::string ProductId, cwTradeTimeSpace iTradeIndex);

	//��ȡ��ǰһ������ʱ�ε�ʱ���
	int	 GetPreTimeSpaceInterval(std::string ProductId, cwTradeTimeSpace iTradeIndex);
	int	 GetTimeSpaceInterval(std::string productId, std::string starttime, std::string endTime);

	void InitialTradeTimeMap();
	void InitialTradeTimeMapFromFile(const char* filename);
	bool GenerateTradeTimeToFile(const char* filename);
private:
	std::map<std::string, std::vector<TradeTimePtr>> m_ProductTradeTimeMap;
};


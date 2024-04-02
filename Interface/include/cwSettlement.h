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

#include "cwTradeCommonDefine.h"
#include "cwMutex.h"

#include <unordered_map>
#include <map>
#include <string>
#include <deque>

class cwSettlement
{
public:
	cwSettlement();
	~cwSettlement();

#ifdef _MSC_VER
#pragma region CommenDefine
#endif // _MSC_VER



#ifdef _MSC_VER
#pragma endregion
#endif // _MSC_VER

	//���ĸ��º���
	void SettlementPrice(std::string sInstrumentid, double dPrice, double dVolumeMultiple);
	void UpdateTrade(std::string instrumentid, double price, int volume, double dVolumeMultiple);

	//��չ���º���
	void UpdatePrice(cwMarketDataPtr pMdData, double dPriceTick, double dVolumeMultiple);

	void UpdateOrder(cwOrderPtr pOrder);
	void UpdateTrade(cwTradePtr pTrade, double dVolumeMultiple);

	//���
	void Deposit(double ddeposit)
	{
		m_dPreBalance += ddeposit;
		m_dBalance += ddeposit;
	}

	cwMUTEX													m_ProcessMutex;

	std::unordered_map<std::string, double>					m_LastPriceMap;					//Key Insrumentid,	value: LastPrice

	std::unordered_map<std::string, int>					m_Position;						//key Instrumentid	value: NetPosition

	std::map<std::string, cwOrderPtr>						m_OrderList;					//key OrderRef		value: orders
	std::map<std::string, cwTradePtr>						m_TradeList;					//key TradeId		value: trades

	std::unordered_map<std::string, int>					m_CancelCount;					//key Instrumentid	value: Cancel Count

	//Profit and loss Statistics
	std::unordered_map<std::string, double>					m_CloseProfitMap;				//key:InstrumentID, value: This Instrument ClosePrifit
	std::unordered_map<std::string, double>					m_PositionProfitMap;			//key:InstrumentID, value: This Instrument PositionProfit
	std::unordered_map<std::string, double>					m_FeeMap;						//key:InstrumentID, value: This Instrument Fee

	std::unordered_map<std::string, std::deque<double>>		m_UnClosePositionCostDetail;	//key:InstrumentID, value: Position Cost deque
	std::unordered_map<std::string, double>					m_UnClosePositionCost;			//key:InstrumentID, value: Position Cost
	std::unordered_map<std::string, int>					m_TradeCountMap;				//key:InstrumentID, value: TradeCount

	std::unordered_map<std::string, double>					m_dInsMaxBalanceMap;			//key:InstrumentID, value: Instrument Max Balance For Max Drop Down
	std::unordered_map<std::string, double>					m_dInsMaxDropDownMap;			//Key:InstrumentID, value: Max Drop Down

	double													m_dPreBalance;					//��ʼȨ��
	double													m_dBalance;						//��ǰȨ��
	double													m_dMaxFundOccupied;				//����ʽ�ռ��
	double													m_dCurFundOccupied;				//��ǰ�ʽ�ռ��

	double													m_dPositionProfit;				//�ֲ�ӯ��
	double													m_dCloseProfit;					//ƽ��ӯ��
	double													m_dFee;							//������

	double													m_dCurMaxBalance;				//��ֹĿǰ���Ȩ�棬�������س���
	double													m_dMaxDropDown;					//���س�

	double													m_dDefaultFeeRate = 0.0001;		//0.0001;

	void													Analysis();
	///AnalysisResult
};


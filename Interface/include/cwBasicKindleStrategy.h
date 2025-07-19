//////////////////////////////////////////////////////////////////////////////////
//*******************************************************************************
//---
//---	Create by Wu Chang Sheng on May. 10th 2020
//---
//--	Copyright (c) by Wu Chang Sheng. All rights reserved.
//--    Consult your license regarding permissions and restrictions.
//--
//*******************************************************************************
//////////////////////////////////////////////////////////////////////////////////

//��ʾ���»��߿�ͷ�ĺ�����_PriceUpdateΪϵͳ���ã�������á�

#pragma once
#include "cwCommonUtility.h"
#include "cwBasicStrategy.h"
#include "cwKindleStickSeries.h"
#include <condition_variable>
#include <atomic>

#define		CW_CORRECT_TRADINGDAY
//#define		CW_USING_MYSQL_LIB


class cwBasicKindleStrategy :
	public cwBasicStrategy
{
public:
	typedef std::shared_ptr<cwKindleStickSeries>				cwKindleSeriesPtr;

public:
	cwBasicKindleStrategy();
	virtual ~cwBasicKindleStrategy();

	///MarketData SPI
	//������£�OnBar������PriceUpdate�ص��� ��PriceUpdate�Ѿ����Ի�ȡ���ºõ�K�ߣ�
	void					PriceUpdate(cwMarketDataPtr pPriceData) override {};
	//������һ����K�ߵ�ʱ�򣬻���øûص�
	virtual void			OnBar(cwMarketDataPtr pPriceData, int iTimeScale, cwBasicKindleStrategy::cwKindleSeriesPtr pKindleSeries) {};

	///Trade SPI
	//�ɽ��ر�
	void					OnRtnTrade(cwTradePtr pTrade) override {};
	//�����ر�, pOrderΪ���±�����pOriginOrderΪ��һ�θ��±����ṹ�壬�п���ΪNULL
	void					OnRtnOrder(cwOrderPtr pOrder, cwOrderPtr pOriginOrder = cwOrderPtr()) override {};
	//�����ɹ�
	void					OnOrderCanceled(cwOrderPtr pOrder) override {};
	//����¼��������Ӧ
	void					OnRspOrderInsert(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo) override {};
	//��������������Ӧ
	void					OnRspOrderCancel(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo) override {};

	///System Call Back
	//��ʱ����Ӧ
	//��ʱ��ID, ��SetTimer��ʱ�򴫸�ϵͳ�����InstrumentID��NULL,�ڻص���ʱ��szInstrumentIDΪ���ַ�������������
	//����ʲô��Լ��TimerId��OnStrategyTimer��szInstrumentID�����Ǹ���Լ��Ϣ
	void					OnStrategyTimer(int iTimerId, const char * szInstrumentID) override {};
	//�����Խ��׳�ʼ�����ʱ�����OnReady, �����ڴ˺��������Եĳ�ʼ������
	void					OnReady() override {};


	//����k�ߣ� iTimeScale��k�����ڣ���������5����Ϊ300��
	cwKindleSeriesPtr		SubcribeKindle(const char * szInstrumentID, int iTimeScale, int HisKindleCount = 0);
	//pParserHisKindle �Ǹ�����ָ�����ڶ�ȡ��ʷ����
	//szFilePath�ᴫ����ʷ�����ļ���·������ֵInitialHisKindleFromHisKindleFolder���룬�ú�����K�����ݰ�ʱ��˳���0-n�����KindleList��
	//��ʷk�ߴ��������򷵻�true���������⣬�򷵻�false.
	cwKindleSeriesPtr		SubcribeKindle(const char * szInstrumentID, int iTimeScale,
		bool(*pParserHisKindle)(const char* szFilePath,
			const char* szInstrumentID, 
			const char* szProductID,
			const char* szExchangeID,
			std::deque<cwKindleStickPtr>& KindleList));
	//��������K��
	cwKindleSeriesPtr		SubcribeDailyKindle(const char * szInstrumentID);
	cwKindleSeriesPtr		SubcribeDailyKindle(const char* szInstrumentID,
		bool(*pParserHisKindle)(const char* szFilePath,
			const char* szInstrumentID,
			const char* szProductID,
			const char* szExchangeID,
			std::deque<cwKindleStickPtr>& KindleList));
	//����ָ��K��
	cwKindleSeriesPtr		SubcribeIndexKindle(const char* szProductId, int iTimeScale, int HisKindleCount = 0);
	cwKindleSeriesPtr		SubcribeIndexKindle(const char* szProductId, int iTimeScale, 
		bool(*pParserHisKindle)(const char* szFilePath,
			const char* szInstrumentID,
			const char* szProductID,
			const char* szExchangeID,
			std::deque<cwKindleStickPtr>& KindleList));
	std::string				GetIndexName(const char* szProductId);


	bool					InitialHisKindleFromHisKindleFolder(const char* szHisFolder);
	bool					LoadHisKindleFromHisKindleFile(const char* KindleFilePath, std::deque<cwKindleStickPtr>& KindleList, int iTimeScale = 60);
	//
	void					GetKindleFromPublicBus();

	bool					InitialHisKindleFromDB();

	//��ȡ�Ѿ����ĵ�k��
	cwKindleSeriesPtr		GetKindleSeries(const char * szInstrumentID, int iTimeScale);

	//��������--�޼۵�
	cwOrderPtr				InputLimitOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);
	//��������--FAK����Filled And Kill �����ɽ�ʣ���Զ�����ָ�
	cwOrderPtr				InputFAKOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);
	//��������--FOK��(FOK Filled Or Kill ����ȫ���ɽ������Զ�����ָ��)
	cwOrderPtr				InputFOKOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);

	//�򻯱��������� volume����ʾ�򣬸���ʾ�����Զ���ƽ���гֲ־�ƽ�֣�û�оͿ���
	cwOrderPtr				EasyInputOrder(const char * szInstrumentID, int volume, double price,
		cwOpenCloseMode openclosemode = cwOpenCloseMode::CloseTodayThenYd,
		cwInsertOrderType insertordertype = cwInsertOrderType::cwInsertLimitOrder);

	//�򻯱��������� volume����ʾ�򣬸���ʾ�����Զ���ƽ���гֲ־�ƽ�֣�û�оͿ���
	//�ú�����Զ����������µ�ģʽ�ͽ�������Լ��Ϣ���ã����в𵥲�����
	std::deque<cwOrderPtr>	EasyInputMultiOrder(const char * szInstrumentID, int volume, double price,
		cwOpenCloseMode openclosemode = cwOpenCloseMode::CloseTodayThenYd,
		cwInsertOrderType insertordertype = cwInsertOrderType::cwInsertLimitOrder);

	//����
	bool					CancelOrder(cwOrderPtr pOrder);
	//ȫ������
	int						CancelAll();
	//��ָ����Լȫ������
	int						CancelAll(const char * szInstrumentID);
	//��ָ����Լ�ͷ���ȫ������
	int						CancelAll(const char * szInstrumentID, cwFtdcDirectionType direction);

	//���ö�ʱ�� iTimerId��ʱ��id����OnStrategyTimer�ص����ݴ�id�ж����ĸ���ʱ������, iElapse ������������룩
	//Ŀǰ���֧��100����ʱ������ʱ���ڻص������������ʱ�߼���
	//ͬ��id�£�����������ᱻ����
	// 
	//�ر�ע�⣺
	//szInstrumentID �Ƕ�ʱ�������ĺ�Լ��Ϣ���ö�ʱ���ص�ʱ�����ڸú�Լ��Ӧ���ʲ����ID�£�
	//���ûص��������portfolio�µĻص���Ϣ��ͬ���̴߳���OnStrategyTimer�ص�
	//���Բ�ָ�������ĺ�Լ��Ϣ��������nullptr,����Ĭ�ϵĹ����̴߳���OnStrategyTimer�ص�
	bool					SetTimer(int iTimerId, int iElapse, const char* szInstrumentID = nullptr);

	//ί�н��ף�PositionAgency����������ᰴ�������óֲ�
	//ע�⣬������PositionAgency����֮���������µ����߳������������������ͻ��
	virtual void			SetAgentManager(void * pAgentMgr);

	//���ú�Լ�����ʲ����ID, ����û�����õĺ�Լ��Ĭ�����ʲ���ϣ�portfolio)IDΪ0���ʲ�����С�
	//����ͬ��portfolio�µĺ�Լ������ͬ���߳�����������ÿ���ʲ���϶����Լ��Ĵ����߳�
	void					SetPortfolioId(const char * szInstrumentID, unsigned int iPortfolioId);

	//����ͬ��ģʽ
	//true:ͬ��, false:�첽
	//�����λ�͹ҵ���ص���Ϣ����Ҫ���ݻص��ӿ�������ͳ�ƵĻ�����ʹ��ͬ��ģʽ
	//�����λ�͹ҵ���ص���Ϣ��ֻ��ƽ̨�ص��ӿ�����ȡ���ɣ������ʲ���ϵĻ����������첽ģʽ����
	//�����ڻز��ʱ��ʹ��ͬ��ģʽ
	void					SetSynchronizeMode(bool bSynchronous);

	//�����Ƿ�����ָ���������������д�뻺���ļ���
	//����е���ָ��������̣�������Ϊ����Ҫ��false��,���ֻ��������̣�������Ϊ��Ҫ��true��
	void					SetWriteIndexInfoCacheToFile(bool bNeedWriteToFile) { m_bNeedWriteCacheToFile = bNeedWriteToFile; };

	//�о�ģʽ
	void					SetResearchMode(bool bResearch, int iReserveTime = 5);


	///ϵͳ���ýӿ���Ϣ����
	void					_SetReady() override;
	void					_OnDisConnect() override;
	void					_OnSimulationBegin(int64_t timeStamp) override;
	void					_OnSimulationPartEnd(int iSimPartID = 0) override;
	void					_OnSimulationFinished() override;
	void					_PriceUpdate(cwMarketDataPtr& pPriceData) override;
	void					_OnRtnTrade(cwTradePtr& pTrade) override;
	void					_OnRtnOrder(cwOrderPtr& pOrder, cwOrderPtr& pOriginOrder) override;
	void					_OnOrderCanceled(cwOrderPtr& pOrder) override;
	void					_OnRspOrderInsert(cwOrderPtr& pOrder, cwRspInfoPtr& pRspInfo) override;
	void					_OnRspOrderCancel(cwOrderPtr& pOrder, cwRspInfoPtr& pRspInfo) override;
	void					_OnTimer(int iTimerId, const char * szInstrumentID) override;

	enum cwKINDLE_TIMESCALE:int
	{
		cwKINDLE_TIMESCALE_1MIN = 60,
		cwKINDLE_TIMESCALE_3MIN = cwKINDLE_TIMESCALE_1MIN * 3,
		cwKINDLE_TIMESCALE_5MIN = cwKINDLE_TIMESCALE_1MIN * 5,
		cwKINDLE_TIMESCALE_15MIN = cwKINDLE_TIMESCALE_1MIN * 15,
		cwKINDLE_TIMESCALE_30MIN = cwKINDLE_TIMESCALE_1MIN * 30,
		cwKINDLE_TIMESCALE_1HOUR = cwKINDLE_TIMESCALE_1MIN * 60,
		cwKINDLE_TIMESCALE_DAILY = 86400
	};
private:
	///ϵͳ���ýӿ���Ϣ����
	//����K��
	void					_UpdateKindleSeries(cwMarketDataPtr pPriceData, std::map<int, cwKindleSeriesPtr> & OnBarMap);
	bool					_GetAgentWorking(std::string instrumentid);

protected:
	const int c_NightModeStartHour = 19;						//Ĭ��ҹ����ʼСʱΪ19����19�㣨��00�֣����賿3�㣨��59�֣�
	const int	c_NightModeEndHour = 3;							//Ĭ��ҹ�̽���СʱΪ03����19�㣨��00�֣����賿3�㣨��59�֣�
	bool					m_bNightMode;						//����ʱ���Ƿ�Ϊҹ��
	bool					m_bNightNextDay;					//����ʱ���Ƿ�Ϊҹ�̹�12ʱ

	std::string				m_strAppStartDay;					//APP��������
	std::string				m_strAppStartNextDay;				//APP�����ڶ�������(��Ȼ�գ�
	std::string				m_strAppStartNextTradingDay;				//��һ�������գ���APP�������ڼ��㣬��һ��������,��2023.11.8��������ҹ����������ֵΪ20231109��
	std::string				m_strAppStartTime;					//������ʱ��

	const unsigned int		m_iDefaultWorkBenchId;				//Ĭ�Ϲ�����ID, Ϊ0���Զ��幤����ID,�����0.

	bool					m_bResearchMode = false;			//�о�ģʽ

	std::string				m_strHisDataPath;

private:
	bool					m_bSynchronizeMode;					//�Ƿ�ͬ��	true:ͬ���� false:�첽

	cwMUTEX																			m_cwDealKindleMutex;			//K�ߴ���ͬ��
	///K������ key:instrument key: TimeScale value :Kindle Series
	std::unordered_map<std::string, std::unordered_map<int, cwKindleSeriesPtr>>		m_KindleSeriesMap;
	///��ʷk������ Key:Instrument key: TimeScale value:HisKindle Count
	std::unordered_map<std::string, std::unordered_map<int, int>>					m_HisKindleCountMap;

	///Updating Thread 
	///�����¼�����
	enum StrategyEventType
	{
		EventType_OnReady = 0							//ϵͳReady�ص�
		, EventType_SimulationBegin						//�ز⿪ʼ
		, EventType_SimulationPartEnd					//�ز�һ�����ֽ�����һ�����������ļ���
		, EventType_SimulationFinish					//�ز����
		, EventType_OnTimer								//��ʱ���ص�
		, EventType_PriceUpdate							//Tick�������
		, EventType_OnBar								//K�߸���
		, EventType_RtnTrade							//�ɽ��ر�
		, EventType_RtnOrder							//�����ر�
		, EventType_OnCanceled							//�����ر�
		, EventType_OnRspInsert							//����¼��ر���Ӧ
		, EventType_OnRspCancel							//��������������Ӧ
		, AgentType_PriceUpdate							//�������������
		, AgentType_RtnTrade							//������ �ɽ��ر�
		, AgentType_RtnOrder							//������ �����ر�
		, AgentType_OnCanceled							//������ �����ر�
		, AgentType_OnRspInsert							//������ ����¼��ر���Ӧ
		, AgentType_OnRspCancel							//������ ��������������Ӧ
	};

	///�����¼���Ϣ���ݣ� ��ͬ�¼������²�ͬ�������ֶ�������
	struct EventTypeStruct
	{
		StrategyEventType		EventType;				//�¼���Ϣ����
		cwMarketDataPtr			pPriceData;				//��������
		cwTradePtr				pTrade;					//�ɽ���Ϣ
		cwOrderPtr				pOrder;					//��ǰ������Ϣ
		cwOrderPtr				pOriginOrder;			//����ǰ������Ϣ����
		cwRspInfoPtr			pRspInfo;				//�ر���Ϣ

		std::string				strInstrumentID;		//��Լ
		int64_t					iBarId;					//k�ߺ�
		cwKindleSeriesPtr		pKindle;				//K������
	};
	typedef std::shared_ptr<EventTypeStruct>					EventTypeStructPtr;

	//�ʲ���Ϲ�����
	struct PortfolioWorkBench
	{
		unsigned int											iWorkBenchId;					//������ID��������
		std::string												strWorkBenchName;				//���������ƣ��ɲ���ֵ

		std::atomic<int>										iTradeInfoCnt;					//��ǰ��Ҫ����Ľ�����Ϣ����
		std::condition_variable									TradeInfoDoneCv;				//

		std::deque<EventTypeStructPtr>							EventTypeStructDeque;			//�������¼���Ϣ����
		cwMUTEX													EventTypeDequeMutex;			//�¼���Ϣ����ͬ��
		std::condition_variable									EventWorkingMutexCv;			//�����������֪ͨ�����������߳�
		std::atomic<bool>										bEventFinished;

		std::thread												EventTypeWorkingThread;			//�����������߳�
		volatile std::atomic<bool>								bEventTypeWorkingThreadRun;		//�������߳�����״̬
	};
	typedef std::shared_ptr<PortfolioWorkBench>					PortfolioWorkBenchPtr;

	//֧�ָ����ʲ���ϣ�portfolio)���������趨�����߳�������
	std::unordered_map<std::string, unsigned int>				m_InstrumentToPortfolioMap;		//Key:InstrumentID�� value:WorkBenchID
	std::unordered_map<unsigned int, PortfolioWorkBenchPtr>		m_PortfolioMgrIntMap;			//key:WorkBenchID, value:WorkBench
	std::unordered_map<std::string, PortfolioWorkBenchPtr>		m_PortfolioMgrStrMap;			//Key:InstrumentID, value:WorkBench

	PortfolioWorkBenchPtr										m_pDefaultWorkBench;			//Ĭ�Ϲ�����

	//����������
	PortfolioWorkBenchPtr						CreateWorkBench(unsigned int iBenchId, const char * pBenchName = "");
	//��ȡ������
	PortfolioWorkBenchPtr						GetWorkBench(std::string instrumentid);

	//�����������߳�
	void										_EventTypeWorkingThread(PortfolioWorkBenchPtr pWorkBench);
	void										_AddEventType(PortfolioWorkBenchPtr& pWorkBench, EventTypeStructPtr& EventPtr);


	//std::deque<EventTypeStructPtr>				m_EventTypeStructDeque;
	//cwMUTEX										m_EventTypeDequeMutex;
	//std::condition_variable						m_EventWorkingMutexCv;

	//std::thread									m_EventTypeWorkingThread;
	//volatile bool								m_bEventTypeWorkingThreadRun;

	//void										_EventTypeWorkingThread();
	//void										_AddEventType(EventTypeStructPtr EventPtr);

	CW_DISALLOW_COPYCTOR_AND_ASSIGNMENT(cwBasicKindleStrategy);

	void *										m_pAgentManager;


	///Index Price and Kindle Update;
	bool										m_bNeedIndexKindle = false;
	bool										m_bNeedKindle = false;

	std::unordered_map<std::string, cwMarketDataPtr>									m_FileLastMDCacheMap;
	//key Product, key InstrumentID
	std::unordered_map <std::string, std::unordered_map<std::string, cwMarketDataPtr>>	m_IndexCalcuteDataCache;

	//ָ�����㹤���߳�
	cwMUTEX										m_UpdateIndexPriceDequeMutex;
	bool										m_bUpdateIndexPriceThreadRun = false;
	bool										m_bNeedWriteCacheToFile = false;		//Ĭ�ϲ���Ҫ������д��Cache�ļ���ֻ������洢�������Ҫ��
	void										_UpdateIndexPriceWorkingThread();
	std::thread									m_UpdateIndexPriceWorkingThread;
};


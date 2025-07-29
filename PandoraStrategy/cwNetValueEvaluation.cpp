#include <algorithm>
#include <cmath>

#include "cwNetValueEvaluation.h"
#include "cwTimeStamp.h"

cwNetValueEvaluation::cwNetValueEvaluation()
    : m_iTimeStamp(0)
    , m_iStartTimeStamp(0)
    , m_dPreTotalPNL(0.0)
    , m_dLastEquity(0.0)
    , m_dUnits(0.0)
    , m_dPreNetAsset(1.0)
    , m_dCurNetAsset(1.0)
    , m_dTradingYears(0.0)
    , m_dIRR(0.0)
    , m_dAR(0.0)
    , m_dVolatility(0.0)
    , m_dVolatilityDownward(0.0)
    , m_HighestNetAsset(1.0)
    , m_dDrawDownRatio(0.0)
    , m_dMaxDrawDownRatio(0.0)
    , m_dAverageDDR(0.0)
    , m_dSharpeRatio(0.0)
    , m_dSortinoRatio(0.0)
    , m_dCalmarRatio(0.0)
    , m_dSterlingRatio(0.0)
    , m_iKindleCount(0)
    , m_dAvgDailyRet(0.0)
    , m_dExpectedRet(0.05)
    , m_dDoubleEQ(0.0001)
{
}

cwNetValueEvaluation::~cwNetValueEvaluation()
{
}

void cwNetValueEvaluation::Deposit(double deposit)
{
    double dFund = deposit;

    if (deposit < 0
        && deposit + m_dLastEquity < 0) {
        if (m_dLastEquity <= m_dDoubleEQ) {
            dFund = 0;
        } else {
            dFund = (std::max)(deposit, (-1 * m_dLastEquity));
        }
    }

    // �ݶ����
    m_dUnits = m_dUnits + dFund / m_dCurNetAsset;
    m_dLastEquity += dFund;
}

void cwNetValueEvaluation::UpdateNetValueByTotalPNL(std::uint64_t timestamp, double totalPNL, double fundUsed)
{
    // ����ʱ��
    m_iTimeStamp = timestamp;

    // ��ʼ����ʱ��
    if (m_iStartTimeStamp == 0 && fundUsed > 0) {
        m_iStartTimeStamp = timestamp;

        cwTimeStamp tEndDay(timestamp);
        tEndDay.SetHour(16);
        tEndDay.SetMinute(0);
        tEndDay.SetSecond(0);

        if (tEndDay.GetTotalMicrosecond() < timestamp) {
            tEndDay.GoForward(1);
        }
        while (tEndDay.GetDayofWeek() == cwTimeStamp::Sunday || tEndDay.GetDayofWeek() == cwTimeStamp::Saturday) {
            tEndDay.GoForward(1);
        }
        m_iThisDayEndTimeStamp = tEndDay.GetTotalMicrosecond();
    }

    if (m_iStartTimeStamp == 0) {
        return;
    }

    // ���մ���
    if (timestamp > m_iThisDayEndTimeStamp) {
        double ret = m_dCurNetAsset / m_dPreNetAsset - 1;
        m_dailyRetDQ.push_back(ret);
        m_dPreNetAsset = m_dCurNetAsset;

        int size = m_dailyRetDQ.size();

        if (size > 0) {
            m_dAvgDailyRet = (m_dAvgDailyRet * (size - 1) + ret) / size;
        }
    }

    // K������ͳ��
    m_iKindleCount++;

    if (fundUsed > m_dLastEquity) {
        // ���
        Deposit(fundUsed - m_dLastEquity);
    }

    m_dLastEquity += (totalPNL - m_dPreTotalPNL);
    m_dPreTotalPNL = totalPNL;

    // ��ֵ
    m_dCurNetAsset = m_dLastEquity / m_dUnits;

    // ����س��ʺ����س���
    if (m_dCurNetAsset > m_HighestNetAsset) {
        m_HighestNetAsset = m_dCurNetAsset;
    }

    m_dDrawDownRatio = (m_HighestNetAsset - m_dCurNetAsset) / m_HighestNetAsset;
    if (m_dDrawDownRatio > m_dMaxDrawDownRatio) {
        m_dMaxDrawDownRatio = m_dDrawDownRatio;
    }
}

void cwNetValueEvaluation::Calculate()
{
    m_dAverageDDR = (m_dAverageDDR * (m_iKindleCount - 1) + m_dDrawDownRatio) / m_iKindleCount;

    m_dTradingYears = (m_iTimeStamp - m_iStartTimeStamp) / (1000.0 * 1000 * 24 * 60 * 60 * 365); // 24Сʱ*60����*60��*365��

    // ����������
    m_dAR = (m_dCurNetAsset - 1) / m_dTradingYears;
    m_dIRR = pow(m_dCurNetAsset, m_dTradingYears);

    // m_dVolatility�����ʼ��� �� m_dVolatilityDownward���в����ʼ���
    int size = m_dailyRetDQ.size();
    double sum1 = 0;
    double sum2 = 0;
    for (int i = 0; i < size; i++) {
        sum1 = sum1 + (m_dailyRetDQ[i] - m_dAvgDailyRet) * (m_dailyRetDQ[i] - m_dAvgDailyRet);
        if (m_dailyRetDQ[i] < m_dExpectedRet)
            sum2 = sum2 + (m_dailyRetDQ[i] - m_dExpectedRet) * (m_dailyRetDQ[i] - m_dExpectedRet);
    }
    if (size > 1) {
        m_dVolatility = sqrt(sum1 / (size - 1) * 252);
        m_dVolatilityDownward = sqrt(sum2 / (size - 1) * 252);
    }

    // �������ձ��ʡ�������ʡ�˹���ֱ���
    m_dSharpeRatio = m_dIRR / m_dVolatility;
    m_dSortinoRatio = m_dIRR / m_dVolatilityDownward;

    m_dCalmarRatio = m_dIRR / m_dMaxDrawDownRatio;
    m_dSterlingRatio = m_dIRR / m_dAverageDDR;
}

void cwNetValueEvaluation::SetExpectedReturn(double expectedret)
{
    m_dExpectedRet = expectedret;
}

#ifndef COMMON_H
#define COMMON_H

#define LOG_DEBUG_FULL      0x01
#define LOG_DEBUG           0x02
#define LOG_NORMAL          0x04
#define LOG_WARNING         0x08
#define LOG_ERROR           0x10
#define LOG_SUCCESS         0x20


#include "QDateTime"
struct TransactionStruct
{
    QString date;
    QString dateUTC;
    QString instrumentName;
    QString period;
    QString profitAndLoss;
    QString transactionType;
    QString reference;
    double openLevel;
    double closeLevel;
    double deltaPoint;
    QString size;
    QString currency;
    bool cashTransaction;
};

struct LiveTransactionStruct
{
    double contractSize;
    QDateTime createdDate;
    QString createdDateUTC;
    QString dealId;
    QString dealReference;
    double size;
    QString direction;
    double limitLevel;
    double level;
    QString currency;
    bool controlledRisk;
    double stopLevel;
    double trailingStep;
    double trailingStopDistance;
};

struct StructProxy
{
    QString Type;
    QString HostName;
    quint16 Port ;
};

struct StructAccountInfo
{
    QString Type;
    QString Id;
    QString password ;
    QString api ;
};

struct AccountInfo
{
    QString CashAvailable;
    QString Balance;
    QString ProfitLoss;
    QString ID;
};


enum RequestType{
    NoRequest,
    RequestConnectionIG,
    RequestGetTransactionList,
    RequestGetOpenPosition,
    RequestConnectionLightStreamer,
    RequestRebindLightStreamer,
    RequestOpenPosition,
    RequestClosePosition

};
#endif // COMMON_H

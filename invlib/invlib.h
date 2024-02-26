//     Stock market website
// 
//     Copyright (C)  2019-2024 Tom Stevelt
// 
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU Affero General Public License as
//     published by the Free Software Foundation, either version 3 of the
//     License, or (at your option) any later version.
// 
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU Affero General Public License for more details.
// 
//     You should have received a copy of the GNU Affero General Public License
//     along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef HAVE_INVLIB
#define	HAVE_INVLIB

#define		TEMPDIR			"/var/local/tmp"
#define		WORKDIR			"/var/local/invest"
#define		WEBSTATEDIR		"/var/local/webstate/invest"

#define		ROLE_FREE		'M'
#define		ROLE_PAID		'P'
#define		ROLE_PREMIUM	'R'
#define		ROLE_EXPIRED	'X'
#define		ROLE_BETA		'B'
#define		ROLE_ADMIN		'A'

#define		STATUS_PENDING	'P'
#define		STATUS_VERIFIED	'V'

/*----------------------------------------------------------
	MungeData used with DEMO_MEMBER, applies to more than just HistoryArray.
----------------------------------------------------------*/
#define		DEMO_MEMBER		502
TYPE	int			MungeData;

#define		VOLUME_METHOD_OPEN_CLOSE		79
#define		VOLUME_METHOD_PREVIOUS_CLOSE	80

#define		DATE_RANGE_ONE_MONTH			'1'
#define		DATE_RANGE_SIX_MONTHS			'6'
#define		DATE_RANGE_ONE_YEAR				'Y'
#define		DATE_RANGE_TWO_YEARS 			'T'

#define		SORTBY_RETURN		'R'
#define		SORTBY_CORREL		'C'
#define		SORTBY_VARIANCE		'V'
#define		SORTBY_COMPARED		'B'
#define		SORTBY_GAIN			'G'
#define		SORTBY_TICKER		'T'
#define		SORTBY_NONE			'N'
#define		SORTBY_SECTOR		'S'
#define		SORTBY_INDUSTRY		'I'

#define		STYLE_BASE			'B'
#define		STYLE_MSTAR			'M'
#define		STYLE_TODAY			'T'
#define		STYLE_ALERTS		'A'
#define		STYLE_FULL			'F'
#define		STYLE_OVERNIGHT		'O'
#define		STYLE_BUCKETS		'P'
#define		STYLE_FUNDAMENTAL	'X'
#define		STYLE_YOY			'Y'
#define		STYLE_CUSTOM		'C'

#define		STYPE_BOND			'B'
#define		STYPE_ETF			'E'
#define		STYPE_PREFER		'P'
#define		STYPE_STOCK			'S'
#define		STYPE_ADR			'A'
#define		STYPE_REIT			'R'
#define		STYPE_CRYPTO		'C'
#define		STYPE_INDEX			'X'
#define		STYPE_OTHER			'?'

#define		LARGE_CAP		'L'
#define		MID_CAP			'M'
#define		SMALL_CAP		'S'
#define		HEDGE			'G'
#define		DEVELOPED		'D'
#define		EMERGING		'E'

#define		ALERT_NONE			'N'
#define		ALERT_ABOVE_AMT		'A'
#define		ALERT_BELOW_AMT		'B'
#define		ALERT_TRAIL_STOP	'C'
#define		ALERT_BENCHMARK		'D'
#define		ALERT_ABOVE_MA5		'H'
#define		ALERT_ABOVE_MA10	'I'
#define		ALERT_ABOVE_MA20	'J'
#define		ALERT_ABOVE_MA50	'K'
#define		ALERT_ABOVE_MA100	'L'
#define		ALERT_ABOVE_MA200	'M'
#define		ALERT_BELOW_MA5		'P'
#define		ALERT_BELOW_MA10	'Q'
#define		ALERT_BELOW_MA20	'R'
#define		ALERT_BELOW_MA50	'S'
#define		ALERT_BELOW_MA100	'T'
#define		ALERT_BELOW_MA200	'U'
#define		ALERT_MA5_DECREASE	'1'
#define		ALERT_MA10_DECREASE	'2'
#define		ALERT_MA20_DECREASE	'3'
#define		ALERT_MA50_DECREASE	'4'
#define		ALERT_MA5_INCREASE	'6'
#define		ALERT_MA10_INCREASE	'7'
#define		ALERT_MA20_INCREASE	'8'
#define		ALERT_MA50_INCREASE	'9'

#define		CATYPE_BUY_STOCK	"BS"
#define		CATYPE_SELL_STOCK	"SS"
#define		CATYPE_DRIP_STOCK	"DS"
#define		CATYPE_DEPOSIT		"DP"
#define		CATYPE_WITHDRAW		"WD"
#define		CATYPE_DIVIDEND		"DV"
#define		CATYPE_INTEREST		"IN"
#define		CATYPE_SELL_OPTION		"SO"
#define		CATYPE_BUY_OPTION		"BO"
#define		CATYPE_EXPIRE_OPTION	"XO"

#define		AVERAGE_TRADING_DAYS_PER_MONTH		21.67

#define		FREE_MEMBER_PORTFOLIO_LIMIT		5
#define		FREE_MEMBER_WATCHLIST_LIMIT		5
#define		FREE_MEMBER_REPORTS_LIMIT		50

#define		PAID_MEMBER_PORTFOLIO_LIMIT		500
#define		PAID_MEMBER_WATCHLIST_LIMIT		500
#define		PAID_UNLIMITED					-1

TYPE		int		PortfolioLimit;
TYPE		int		WatchlistLimit;
TYPE		int		ReportLimit;

#define		YAHOO_ASSETPROFILE					'A'
#define		YAHOO_DEFAULTKEYSTATISTICS			'B'
#define		YAHOO_EARNINGSHISTORY				'C'
#define		YAHOO_EARNINGS						'D'
#define		YAHOO_EARNINGSTREND					'E'
#define		YAHOO_FINANCIALDATA					'F'
#define		YAHOO_INDEXTREND					'G'
#define		YAHOO_INDUSTRYTREND					'H'
#define		YAHOO_INSIDERHOLDERS				'I'
#define		YAHOO_MAJORHOLDERSBREAKDOWN			'J'
#define		YAHOO_NETSHAREPURCHASEACTIVITY		'K'
#define		YAHOO_RECOMMENDATIONTREND			'L'
#define		YAHOO_SECTORTREND					'M'
#define		YAHOO_UPGRADEDOWNGRADEHISTORY		'N'
#define		YAHOO_SUMMARYDETAIL					'O'
#define		YAHOO_QUOTE							'P'


/* Generated by Color Designer at www.stevelt.com. */
#define	PAGE_BACKGROUND	"#fefefe"
#define	STORY_BACKGROUND	"#ebebeb"
#define	NORMAL_BACKGROUND	"#3399cc"
#define	CURRENT_BACKGROUND	"#006699"
#define	HOVER_BACKGROUND	"#ffffff"
#define	DISABLE_BACKGROUND	"#999999"
#define	PAGE_COLOR	"#000000"
#define	STORY_COLOR	"#000000"
#define	NORMAL_COLOR	"#000000"
#define	CURRENT_COLOR	"#ffffff"
#define	HOVER_COLOR	"#000000"
#define	DISABLE_COLOR	"#000000"

/*----------------------------------------------------------
	Email aliases on google gmail
----------------------------------------------------------*/
#define		SUPPORT_ADDRESS			"support@silverhammersoftware.com"
#define		ADMIN_ADDRESS			"admin@silverhammersoftware.com"
#define		SIGNUP_ADDRESS			"signup@silverhammersoftware.com"
#define		WEBMASTER_ADDRESS		"webmaster@silverhammersoftware.com"
#define		REPORT_ADDRESS			"reports@silverhammersoftware.com"

TYPE	char		env_ApiKey[65];
TYPE	char		env_ApiURL[65];
TYPE	char		env_PaypalKey[65];
TYPE	char		env_PaypalURL[65];
TYPE	char		Support_Address[128];
TYPE	char		Admin_Address[128];
TYPE	char		Signup_Address[128];
TYPE	char		Webmaster_Address[128];
TYPE	char		Report_Address[128];

#define		SORT_ASCEND		'A'
#define		SORT_DESCEND	'D'

#define		AVG_5		0
#define		AVG_10		1
#define		AVG_20		2
#define		AVG_50		3
#define		AVG_100		4
#define		AVG_200		5

#ifdef HISTORY
typedef struct
{
	char	Date[12];
	double	Open;
	double	High;
	double	Low;
	double	Close;
	long	Volume;
	double	Average[6];
	long	AvgVol;
	double	RSI;
	double	STDDEV;
	double	CTB;
	short	GotRecord;
	short	Flag;
	long	HistoryID;
	long	AverageID;
} HISTORY_RECORD;

#define		MAXHISTRECS		(30*52*5)
TYPE	HISTORY_RECORD		HistoryArray[MAXHISTRECS];
TYPE	int					HistoryCount;

TYPE	double		HistoryOpenArray[MAXHISTRECS];
TYPE	double		HistoryHighArray[MAXHISTRECS];
TYPE	double		HistoryLowArray[MAXHISTRECS];
TYPE	double		HistoryCloseArray[MAXHISTRECS];
TYPE	long		HistoryVolumeArray[MAXHISTRECS];
#endif

#ifdef CRYPTO
typedef struct
{
	long	CryptoTime;
	double	Open;
	double	High;
	double	Low;
	double	Close;
	long	Volume;
	double	Average[6];
	long	AvgVol;
	double	RSI;
	short	GotRecord;
	short	Flag;
	long	CryptoID;
} CRYPTO_RECORD;

#define		MAXCRYPTORECS		(366*24*60)
TYPE	CRYPTO_RECORD		CryptoArray[MAXCRYPTORECS];
TYPE	int					CryptoCount;

TYPE	double		CryptoOpenArray[MAXCRYPTORECS];
TYPE	double		CryptoHighArray[MAXCRYPTORECS];
TYPE	double		CryptoLowArray[MAXCRYPTORECS];
TYPE	double		CryptoCloseArray[MAXCRYPTORECS];
TYPE	long		CryptoVolumeArray[MAXCRYPTORECS];
#endif

/*------------------------------
:.,$d
:r ! mkproto -p *.c
#ifdef AVERAGE
#endif
#ifdef HISTORY
#endif
#ifdef PORTFOLIO
#endif
#ifdef STOCK
#endif
#ifdef MEMBER
#endif
#ifdef WATCHLIST
#endif
#ifdef SYSTEM
#endif
#ifdef DIVIDEND
#endif
#ifdef FUNDAMENTAL
#endif
------------------------------*/


/* IEX_RateLimit.c */
void IEX_RateLimit ( int Debug );

/* AlertDescription.c */
char *AlertDescription ( char AlertType );

/* CreateStock.c */
void CreateStock ( MYSQL *MySql, char *Ticker , char *Name , char StockType , char DomFor );

/* UsageFuncs.c */
void SindexUsage ( int IncludeMemberFeatures );
void ValidStockIndex ( char StockIndex ,int IncludeMemberFeatures, int (*Usage )());
void FormatUsage ( int IncludeWatchlist );
char ValidReportFormatUsage ( char *FormatArg , int (*Usage )());

/* CalcAccumDistrVolume.c */
int CalcAccumDistrVolume ( MYSQL *MySql, char Ticker [], char *LastDate , int Days , double *Ratio , double *AvgVolumeUp , double *AvgVolumeDown );

/* AccumDistrFormula.c */
double AccumDistrFormula ( int UpCount , int DownCount );

/* IndexName.c */
char *IndexName ( char IndexCode );

#ifdef DIVIDEND
/* LoadDividendCB.c */
int LoadDividendCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XDIVIDEND *ptrDividend , int (*CallBack )(), int PrintErrors );

/* LoadDividendSum.c */
double LoadDividendSum ( MYSQL *MySql, char *Ticker , char *CutoffDate );
#endif

#ifdef MARKET
/* LoadMarket.c */
int LoadMarket ( MYSQL *MySql, char *WhereClause , XMARKET *ptrMarket , int PrintErrors );

/* LoadMarketCB.c */
int LoadMarketCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XMARKET *ptrMarket , int (*CallBack )(), int PrintErrors );
#endif

#ifdef INDUSTRY
/* LoadIndustry.c */
int LoadIndustry ( MYSQL *MySql, char *WhereClause , XINDUSTRY *ptrIndustry , int PrintErrors );

/* LoadIndustryCB.c */
int LoadIndustryCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XINDUSTRY *ptrIndustry , int (*CallBack )(), int PrintErrors );
#endif

#ifdef SECTOR
/* LoadSector.c */
int LoadSector ( MYSQL *MySql, char *WhereClause , XSECTOR *ptrSector , int PrintErrors );

/* LoadSectorCB.c */
int LoadSectorCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XSECTOR *ptrSector , int (*CallBack )(), int PrintErrors );
#endif

#ifdef AVERAGE
/* LoadAverage.c */
int LoadAverage ( MYSQL *MySql, char *WhereClause , XAVERAGE *ptrAverage , int PrintErrors );

/* LoadAverageCB.c */
int LoadAverageCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XAVERAGE *ptrAverage , int (*CallBack )(), int PrintErrors );
#endif

#ifdef HISTORY
/* LoadHistory.c */
int LoadHistory ( MYSQL *MySql, char *WhereClause , XHISTORY *ptrHistory, int PrintErrors );

/* LoadHistoryCB.c */
int LoadHistoryCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XHISTORY *ptrHistory , int (*CallBack )(), int PrintErrors);
#endif

/* LoadHistoryArray.c */
int LoadHistoryArray ( MYSQL *MySql, char *Ticker , char *LastDate , int OrderBy , int HowMany );

/* LoadHistoryArrays.c */
void CopyHistoryArrays ( void );
int LoadHistoryArrays ( MYSQL *MySql, char *Ticker , char *LastDate , int OrderBy , int HowMany );

#ifdef PORTFOLIO
/* LoadPortfolioCB.c */
int LoadPortfolioCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XPORTFOLIO *ptrPortfolio , int (*CallBack )(), int PrintErrors);

/* LoadPortfolio.c */
int LoadPortfolio ( MYSQL *MySql, char *WhereClause , XPORTFOLIO *ptrPortfolio , int PrintErrors );
#endif

#ifdef PORTHIST
/* LoadPorthistCB.c */
int LoadPorthistCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XPORTHIST *ptrPorthist , int (*CallBack )(), int PrintErrors);

/* LoadPorthist.c */
int LoadPorthist ( MYSQL *MySql, char *WhereClause , XPORTHIST *ptrPorthist , int PrintErrors );
#endif

#ifdef STOCK
/* LoadStock.c */
int LoadStock ( MYSQL *MySql, char *WhereClause , XSTOCK *ptrStock, int PrintErrors, int CalcTTM );

/* LoadStockCB.c */
int LoadStockCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XSTOCK *ptrStock , int (*CallBack )(), int PrintErrors);

#endif

/* FundamentalHuman.c */
char *FundamentalHuman ( char *Field );

#ifdef FUNDAMENTAL
/* LoadFundamental.c */
int LoadFundamental ( MYSQL *MySql, char *WhereClause , XFUNDAMENTAL *ptrFundamental, int PrintErrors );

/* LoadFundamentalCB.c */
int LoadFundamentalCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XFUNDAMENTAL *ptrFundamental , int (*CallBack )(), int PrintErrors);

#endif

#ifdef SYSTEM
/* LoadSystem.c */
int LoadSystem ( MYSQL *MySql, XSYSTEM *ptrSystem , int PrintErrors );
#endif

#ifdef MEMBER
/* LoadMemberCB.c */
int LoadMemberCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XMEMBER *ptrMember , int (*CallBack )(), int Verbose );

/* LoadMember.c */
int LoadMember ( MYSQL *MySql, char *WhereClause , XMEMBER *ptrMember , int PrintErrors );
#endif

#ifdef WATCHLIST
/* LoadWatchlistCB.c */
int LoadWatchlistCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XWATCHLIST *ptrWatchlist , int (*CallBack )(), int PrintErrors );

/* LoadWatchlist.c */
int LoadWatchlist ( MYSQL *MySql, char *WhereClause , XWATCHLIST *ptrWatchlist , int PrintErrors );
#endif

#ifdef TEMP
/* LoadTemp.c */
int LoadTemp ( MYSQL *MySql, char *WhereClause , XTEMP *ptrTemp , int PrintErrors );

/* LoadTempCB.c */
int LoadTempCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XTEMP *ptrTemp , int (*CallBack )(), int PrintErrors );
#endif

/* PrintErrors.c */
void PrintErrors ( void );

/* StartMySQL.c */
void StartMySQL ( MYSQL *MySql , char *DatabaseName );

/* SetWhereClause.c */
void SetWhereClause ( char WhereClause [], char StockIndex );

/* ToWatchlist.c */
int ToWatchlist ( MYSQL *MySql, char *Filename , char *Delimiters , int TickerColumn , int ReportFormat , long MemberID , int PrintErrors );

/* ToTemp.c */
int ToTemp ( MYSQL *MySql, char *Filename , char *Delimiters , int TickerColumn , int ReportFormat , long MemberID , int PrintErrors );

/* SetMemberLimits.c */
void SetMemberLimits ( char MemberRole );

/* InPortfolio.c */
int InPortfolio ( MYSQL *MySql, char *Ticker );

/* IsBenchmark.c */
int IsBenchmark ( char *Ticker );

/* OrderEntryBuy.c */
int OrderEntryBuy ( MYSQL *MySql, long MemberID , char Ticker [], char PurchDate [], double Shares , double Price );

/* OrderEntrySell.c */
int OrderEntrySell ( MYSQL *MySql, long MemberID , char Ticker [], char PurchDate [], double Shares , double Price );

/* LoadCryptoArray.c */
int LoadCryptoArray ( MYSQL *MySql, char *Ticker , int OrderBy , int HowMany );

/* LoadCryptoArrays.c */
int LoadCryptoArrays ( MYSQL *MySql, char *Ticker , int OrderBy , int HowMany );

/* ManyDecimalDouble.c */
int ManyDecimalDouble ( char *strValue );

/* GetFundamentalsTiingo.c */
int GetFundamentalsTiingo ( const char *Ticker , const char Type,  const int Days, char *Buffer, size_t BufferSize );

/* GetInvestCfg.c */
void GetInvestCfg ( int UseTiingo );

/* ReadStockType.c */
char ReadStockType ( char *token );

#ifdef CRYPTO
/* LoadCrypto.c */
int LoadCrypto ( MYSQL *MySql, char *WhereClause , XCRYPTO *ptrCrypto , int PrintErrors );

/* LoadCryptoCB.c */
int LoadCryptoCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XCRYPTO *ptrCrypto , int (*CallBack )(), int PrintErrors );
#endif

#ifdef CASHBAL
/* LoadCashbalCB.c */
int LoadCashbalCB ( MYSQL *MySql, char *WhereClause , char *OrderByClause , XCASHBAL *ptrCashacct , int (*CallBack )(), int PrintErrors );
#endif

#endif

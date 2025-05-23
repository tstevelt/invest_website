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


#undef PAYPAL_ON_HOME_PAGE
#define PAYPAL_ON_HOME_PAGE

#define PAYPAL_ON_PROFILE_PAGE
#undef PAYPAL_ON_PROFILE_PAGE

#undef STAY_IN_ADD_MODE_UNTIL_LIMIT
#define STAY_IN_ADD_MODE_UNTIL_LIMIT

#define DENY_MULTIPLE_POSITIONS
#undef DENY_MULTIPLE_POSITIONS

#define CAN_GET_DIVIDENDS
#undef CAN_GET_DIVIDENDS

#define		CSS_VERSION		"20240624_03"

#define		TABLE_CLASS		"table-condensed table-borderless"

#define		SET_SELF		"onclick='javascript:SetSelf();'"

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<ctype.h>
#include	<math.h>

#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <libgen.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <sys/types.h>
#include <curl/curl.h>

#include	"ta-lib/ta_func.h"
#include	"shslib.h"

#include	"weblib.h"
#include	"webCookie.h"
#include	"dbylib.h"
#include	"rptlib.h"

#define		STOCK
#define		FUNDAMENTAL
#define		SECTOR
#define		INDUSTRY
#define		HISTORY
#define		CRYPTO
#define		AVERAGE
#define		MARKET
#define		PORTFOLIO
#define		PORTHIST
#define		CASHBAL
#define		WATCHLIST
#define		MEMBER
#define		SYSTEM
#include	"fileinvest.h"
#include	"invlib.h"

TYPE	XSTOCK		MaintStock;
TYPE	XSTOCK		OrderEntryStock;

#define		MEMBER_COOKIE_NAME		"invest"
#define		MEMBER_COOKIE_LIFE_TIME	(3600*12)
TYPE	char	SessionUnique[20];
TYPE	char	SessionFileName[256];
TYPE	XMEMBER		CookieMember;

#define		INTRODUCTORY_PRICE		99
#define		ANNUAL_PRICE			99
#define		PREMIUM_PRICE			500
#define		REFERRAL_FEE			5
#define		PROMO_CODE_LENGTH		9

#define		STRONG_MESSAGE		"h2"
#define		STORY_TITLE			"h3"

#define		URL_NAME		"stocks.silverhammersoftware.com"
#define		MAX_SHORT_STRING	256
#define		MAX_LONG_STRING		2048
#define		MAXPAGES			20
#define		MAXSTOCKPORT		10

TYPE	DATEVAL	dvToday;
TYPE	char	Today[12];
TYPE	char	Title[MAX_LONG_STRING];
TYPE	char	Footer[MAX_LONG_STRING];
TYPE	char	*Remote_Address;

typedef struct
{
	int		PageNumber;
	char	String[MAX_SHORT_STRING];
} CONTENT_RECORD;

TYPE	CONTENT_RECORD	MenuArray[MAXPAGES];
TYPE	int				MenuCount;
TYPE	int				MenuIndex;

TYPE	CONTENT_RECORD	PageArray[MAXPAGES];
TYPE	int				PageCount;
TYPE	int				PageIndex;

#define		MODE_UNKNOWN				101
#define		MODE_LOGIN					102
#define		MODE_SIGNUP					103
#define		MODE_VERIFY					104
#define		MODE_CONTACT				105
#define		MODE_LOGOUT					106
#define		MODE_SAVE_PROFILE			107
#define		MODE_SEND_MESSAGE_TO_MEMBER	108
#define		MODE_FORGOT					109
#define		MODE_PAINT_PASSWORD			110
#define		MODE_SAVE_PASSWORD			111
#define		MODE_CHANGE_PASSWORD		112
#define		MODE_CHANGE_EMAIL			113
#define		MODE_DELETE_STOCKS			114
#define		MODE_FIND_STOCK				115
#define		MODE_INSERT_STOCK			116
#define		MODE_CLEAR_STOCK			117
#define		MODE_REMOVE_ALL_WATCHLIST	118
#define		MODE_REMOVE_NO_ALERT_WATCHLIST	119
#define		MODE_PROCESS_PAYMENT		120
#define		MODE_PRINT_SHEET			121
#define		MODE_TV_CHART				122
#define		MODE_ORDER_ENTRY_BUY		123
#define		MODE_ORDER_ENTRY_SELL		124
#define		MODE_ORDER_ENTRY_CLEAR		125
#define		MODE_CONFIRM_BUY			126
#define		MODE_CONFIRM_SELL			127
#define		MODE_ORDER_ENTRY_PAINT		128
#define		MODE_ORDER_ENTRY_DRIP		129
#define		MODE_CONFIRM_DRIP			130
#define		MODE_CASH_ACCOUNT			131
#define		MODE_CASH_ACCOUNT_SAVE		132
#define		MODE_CASH_ACCOUNT_CONFIRM	133
#define		MODE_CASH_ACCOUNT_CLEAR		134
#define		MODE_WATCHLIST_CHART		135
#define		MODE_PORTFOLIO_CHART		136
#define		MODE_COMBINED_CHART			137
#define		MODE_STOCK_CHART			138
#define		MODE_CHART_PREVIOUS			139
#define		MODE_CHART_NEXT				140
#define		MODE_SAVE_STOCK				141
#define		MODE_DELETE_STOCK			142
#define		MODE_STOCK_FUNDAMENTALS_RAW		143
#define		MODE_STOCK_FUNDAMENTALS_FMT		144
#define		MODE_STOCK_DIVIDENDS			145
#define		MODE_PORTFOLIO_DIVIDENDS		146
#define		MODE_STOCK_STATS_RAW			147

#define		SCREENER_AND_REPORT_OFFSET		201
#define		MODE_REPORT_REGRESS				201
#define		MODE_REPORT_VOLUME_ACCUM		202
#define		MODE_REPORT_VOLUME_INCREASE		203
#define		MODE_REPORT_PORTFOLIO			204
#define		MODE_REPORT_WATCHLIST			205
#define		MODE_REPORT_AD_HISTORY			206
#define		MODE_REPORT_52_WEEK				207
#define		MODE_REPORT_MA_CROSSOVER		208
#define		MODE_REPORT_BREAKOUT			209
#define		MODE_REPORT_SEC_IND_ALLOCATION	210
#define		MODE_REPORT_RELATIVE_STRENGTH	211
#define		MODE_REPORT_DI_CROSSOVER		213
#define		MODE_REPORT_COOPER				214
#define		MODE_REPORT_ADX_TREND			215
#define		MODE_REPORT_MA_RATIO			216
#define		MODE_REPORT_EQUITY_FUNDAMENTALS	217
#define		MODE_REPORT_ETF_FUNDAMENTALS	218
#define		MODE_REPORT_VALUE				219
#define		MODE_REPORT_CTB_STRENGTH		220
#define		MODE_REPORT_ASSET_ALLOCATION	221
#define		MAX_SCREENER_AND_REPORT			21

#define		MODE_PAINT_PORTFOLIO_AND_WATCHLIST		310
#define		MODE_PAINT_EDIT_WATCHLIST		311
#define		MODE_UPDATE_WATCHLIST			312
#define		MODE_DELETE_WATCHLIST			313
#define		MODE_INSERT_WATCHLIST			314
#define		MODE_PAINT_IMPORT_WATCHLIST		315
#define		MODE_IMPORT_WATCHLIST			316
#define		MODE_PAINT_EDIT_PORTFOLIO		321
#define		MODE_UPDATE_PORTFOLIO			322
#define		MODE_DELETE_PORTFOLIO			323
#define		MODE_INSERT_PORTFOLIO			324
#define		MODE_PAINT_EDIT_CASHBAL			325
#define		MODE_UPDATE_CASHBAL				326
#define		MODE_DELETE_CASHBAL				327
#define		MODE_INSERT_CASHBAL				328
#define		MODE_PAINT_IMPORT_PORTFOLIO		329
#define		MODE_IMPORT_PORTFOLIO			330
TYPE	int		RunMode;

#define	ACCORDIAN_WATCHLIST			1
#define	ACCORDIAN_WATCHLIST_REPORT	2
#define	ACCORDIAN_PORTFOLIO			3
#define	ACCORDIAN_PORTFOLIO_REPORT	4
#define	ACCORDIAN_SEC_IND_ALLOCATION	5
#define	ACCORDIAN_ASSET_ALLOCATION		6
#define	ACCORDIAN_STOCK				7
#define	ACCORDIAN_ORDERS			8
#define	ACCORDIAN_CASHBAL			9
#define	ACCORDIAN_STATEMENT			10
#define	ACCORDIAN_EQUITY_FUNDAMENTAL_REPORT	11
#define	ACCORDIAN_ETF_FUNDAMENTAL_REPORT	12
TYPE	int		AccordianID;

TYPE	char	*SignupName;
TYPE	char	*SignupPager;
TYPE	char	*SignupEmail;
TYPE	char	*SignupPassword;
TYPE	int		SignupCookie;
TYPE	char	*ConfirmPassword;
TYPE	char	*RefPromoCode;
TYPE	char	*ContactSubject;
TYPE	char	*ContactEmail;
TYPE	int		AcceptAllRisks;
TYPE	int		NotAFinancialAdvisor;
TYPE	int		DoFurtherResearch;
TYPE	int		TermsAndConditions;
#ifdef ELECTRONIC_SIGNATURE
TYPE	char	*MySignature;
#endif
// TYPE	char	IncludeDividend;
TYPE	int		ChartNumber;
TYPE	int		ChartFast;
TYPE	int		ChartSlow;
TYPE	double	CashT;
TYPE	double	BondT;
TYPE	double	DomT;
TYPE	double	ForT;
TYPE	char	*ContactMessage;
TYPE	long	VerifyID;
TYPE	char	*VerifyIPaddr;
TYPE	time_t	VerifyTime;
TYPE	char	*LoginEmail;
TYPE	char	*LoginPassword;
TYPE	char	LogoutSelect;
TYPE	char	*ForgotEmail;
TYPE	char	*ForgotString;
TYPE	long	FromMemberID;
TYPE	long	ToMemberID;
TYPE	long	PortfolioID;
TYPE	long	WatchlistID;
TYPE	long	CashbalID;
TYPE	long	PortfolioRecordCount, WatchlistRecordCount, CashbalRecordCount, RptlogRecordCount, TempRecordCount;
TYPE	char	*OrderEntryTicker;
TYPE	double	OrderEntryShares;
TYPE	double	OrderEntryPrice;
TYPE	char	*OrderEntryDate;
TYPE	char	*CashAccountType;
TYPE	char	*CashAccountComment;
TYPE	double	CashAccountShares;
TYPE	double	CashAccountPrice;
TYPE	char	*CashAccountDate;

#define		MAXDELETE		50
TYPE	char	*DeleteTickersArray[MAXDELETE];
TYPE	int		 DeleteTickersCount;

TYPE int ChartCurrentRow;
TYPE int ChartRowCount;

/*----------------------------------------------------------
	Report stuff
----------------------------------------------------------*/
typedef struct
{
	char	Field[20];
	int		DataType;
	SHS_DATA_UNION	Minimum;
	SHS_DATA_UNION	Maximum;
} SCREENER_MIN_MAX;

#define		MAXSCREENERFIELDS		25
TYPE	SCREENER_MIN_MAX		ScreenerMinMaxArray[MAXSCREENERFIELDS];
TYPE	int		ScreenerMinMaxCount;
TYPE	char	StockIndexList[10];
TYPE	char	StockTypeList[10];

TYPE	char	StockIndex[MAX_SCREENER_AND_REPORT];
TYPE	char	StockType[MAX_SCREENER_AND_REPORT];
TYPE	char	Broker[MAX_SCREENER_AND_REPORT][12];
TYPE	char	BuySellAll[MAX_SCREENER_AND_REPORT];
TYPE	char	DateRange[MAX_SCREENER_AND_REPORT];
TYPE	int		TopCount[MAX_SCREENER_AND_REPORT];
TYPE	double	TopPercent[MAX_SCREENER_AND_REPORT];
TYPE	int		MinimumVolume[MAX_SCREENER_AND_REPORT];
TYPE	int		NumberOfDays[MAX_SCREENER_AND_REPORT];
TYPE	int		NumberOfDays1[MAX_SCREENER_AND_REPORT];
TYPE	int		NumberOfDays2[MAX_SCREENER_AND_REPORT];
TYPE	char	SortBy[MAX_SCREENER_AND_REPORT];
TYPE	char	ReportStyle[MAX_SCREENER_AND_REPORT];
TYPE	char	ReportFormat[MAX_SCREENER_AND_REPORT];
TYPE	char	TopCountOrMinimumRS;
TYPE	char	*StatementYear[MAX_SCREENER_AND_REPORT];
TYPE	char	*StatementMonth[MAX_SCREENER_AND_REPORT];
TYPE	int		Value;
TYPE	double	CorpAAA;
TYPE	double	RangeBHS;

/*----------------------------------------------------------
	Portfolio and Watchlist stuff
----------------------------------------------------------*/
#define		ACTION_DELETE		'D'
#define		ACTION_INSERT		'I'
#define		ACTION_UPDATE		'U'
#ifdef ABANDONED
typedef struct
{
	long	xid;
	int		RunMode;
	int		ActionCode;
	char	*xticker;
	char	*xdate;
	double	xshares;
	double	xcost;
	char	*xbenchmark;
	char	*xbroker;
	char	xtaxable;
	double	xalert;
	char	*xoptexp;
} PORTFOLIO_WATCHLIST_RECORD;

#define			MALLOC_START		25
#define			MALLOC_CHUNK		10
TYPE	PORTFOLIO_WATCHLIST_RECORD	*PortfolioWatchlistArray;
TYPE	int							AllocatedSpace;
TYPE	int							PortfolioWatchlistCount;
#endif

TYPE	XPORTFOLIO		PortfolioArray[MAXSTOCKPORT];
TYPE	int				PortfolioCount;

/*----------------------------------------------------------
	chart stuff
----------------------------------------------------------*/
#define		STOCK_CHART_WIDTH		1200
#define		STOCK_CHART_HEIGHT		600
#define		VOLUME_CHART_WIDTH		1200
#define		VOLUME_CHART_HEIGHT		150

/*----------------------------------------------------------
	process payments
----------------------------------------------------------*/
typedef struct
{
	char	*Transaction;
	char	*Status;
	double	Amount;
	char	*Currency;
	char	*Category;
	char	*ItemNumber;
	char	*Signature;
} PAYMENT_RECORD;

TYPE	PAYMENT_RECORD	Payment;

/*----------------------------------------------------------
	mysql and dbylib stuff
----------------------------------------------------------*/
TYPE  MYSQL   MySql;
TYPE	DBY_OPTIONS	DbyOptions;
TYPE	char		Statement[MAXSTATEMENT];
TYPE	char		WhereClause[1024];
TYPE	char		OrderByClause[1024];
#ifdef MAIN
TYPE	char	*LogFileName = "/var/local/invest.log";
#else
TYPE	char	*LogFileName;
#endif

/*----------------------------------------------------------
    libcurl stuff
----------------------------------------------------------*/
TYPE    char    TempFileName[128];
TYPE    CURL        *curl;
TYPE    CURLcode    curlRV;


/*------------------------------
:.,$d
:r ! mkproto -p *.c
? PaintMisc
-----------------------------*/

/* ChkInput.c */
void ChkInput ( void );

/* DeleteStocks.c */
void DeleteStocks ( void );

/* Forgot.c */
int Forgot ( void );

/* GetContents.c */
void GetContents ( int Index );

/* GetInput.c */
void GetInput ( void );

/* GetReportCount.c */
long GetReportCount ( void );

/* InsertRptlog.c */
void InsertRptlog ( char *CommandLine );

/* invest.c */
int main ( int argc , char *argv []);

/* Login.c */
int Login ( void );

/* Logout.c */
int Logout ( void );

/* NotifyAdmin.c */
void NotifyAdmin ( char *Message );

/* PaintAccumDistro.c */
void PaintAccumDistro ( void );

/* PaintAdmin.c */
void PaintAdmin ( void );

/* PaintBottom.c */
void PaintBottom ( void );

/* PaintChart.c */
void PaintChart ( int RunMode , char *Ticker );

/* PaintStockDividends.c */
void PaintStockDividends ( int RunMode , char *Ticker );

/* PaintFundamentals.c */
void PaintFundamentals ( char *Ticker, char *Name, char *Type, char *CikCode );

/* PaintStatsRaw.c */
void PaintStatsRaw ( char *xsticker , char *xsname );

/* PaintETF_FundamentalScreener.c */
void PaintETF_FundamentalScreener ( void );

/* PaintEquityFundamentalScreener.c */
void PaintEquityFundamentalScreener ( void );

/* PaintCombinedChart.c */
void PaintCombinedChart ();

/* PaintContact.c */
void PaintContact ( void );

/* PaintEditPortfolio.c */
void PaintEditPortfolio ( void );

/* PaintEditWatchlist.c */
void PaintEditWatchlist ( void );

/* PaintEditCashbal.c */
void PaintEditCashbal ( void );

/* PaintHome.c */
void PaintHome ( void );

/* PaintImportWatchlist.c */
void PaintImportWatchlist ( void );

/* PaintImportPortfolio.c */
void PaintImportPortfolio ( void );

/* PaintMiscFuncs.c */
void PushSignup ( void );
void PrintUpgradeMessage ( int WhichMessage );
void PaintStartReportStory ( char *HeadingCode , char *Title , int IsIn );
void PaintEndReportStory ( void );
void PaintSelectStype ( int RunMode );
void PaintStockUniverse ( int RunMode );
void PaintVolumeCalcMethod ( int RunMode );
void PaintDateRange ( int RunMode );
void PaintTopCount ( int RunMode , int Default );
void PaintTopPercent ( int RunMode , char *Label , double Default );
void PaintMinimumVolume ( int RunMode , int Default );
void PaintNumberOfDays ( int RunMode , char *Label , int Default );
void PaintTwoNumberOfDays ( int RunMode , char *Label1 , int Default1 , char *Label2 , int Default2 );
void PaintTwoSelectorOfDays ( int RunMode , char *Label1 , int Array1 [], int Default1 , char *Label2 , int Array2 [], int Default2 );
void PaintSortBy ( int RunMode , int OptionArray [], int OptionCount );
void PaintFormat ( int RunMode , int IncludeWatchlist );
void PaintSubmit ( int RunMode , char *Label , int Columns );
void PaintPaypal ( void );

/* PaintMovingAverages.c */
void PaintMovingAverages ( int InAccordian );

/* PaintNews.c */
void PaintNews ( void );

/* PaintPassword.c */
void PaintPassword ( void );

/* PaintPortfolioAndWatchlist.c */
void PaintPortfolioAndWatchlist ( void );

/* PaintProfile.c */
void PaintProfile ( void );

/* PaintReportBottom.c */
void PaintReportBottom ( void );

/* PaintReports.c */
void PaintReports ( void );

/* PaintReportTop.c */
void PaintReportTop ( void );

/* PaintScreen.c */
void PaintScreen ( void );

/* PaintSignup.c */
void PaintSignup ( void );

/* PaintSpecial.c */
void PaintSpecial ( int FunctionCode );

/* PaintStats.c */
void PaintStats ( void );

/* PaintStockMaint.c */
void PaintStockMaint ( void );

/* PaintTableDescription.c */
void PaintTableDescription ( char *WhichDescription );

/* PaintTop.c */
void PaintTop ( void );

/* ProcessPayment.c */
void ProcessPayment ( void );

/* SaveCashBalance.c */
void SaveCashBalance ( void );

/* SaveEmail.c */
int SaveEmail ( void );

/* SavePassword.c */
void SavePassword ( void );

/* SavePortfolio.c */
void SavePortfolio ( int RunMode );

/* SaveProfile.c */
void SaveProfile ( void );

/* SaveStock.c */
void SaveStock ( void );

/* SaveWatchlist.c */
void SaveWatchlist ( int RunMode );

/* SessionFuncs.c */
void WriteSession ( void );
void ReadSession ( void );
void RemoveSession ( void );

/* Signup.c */
int Signup ( void );

/* TakeAction.c */
void TakeAction ( char Format );

/* Verify.c */
int Verify ( void );

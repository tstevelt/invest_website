/*----------------------------------------------------------------------------
	Program : invest/GetInput.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Get user input from the screen
	Return  : 
----------------------------------------------------------------------------*/
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

#include	"invest.h"

#define	DEBUG
#undef	DEBUG

static int AddField ( char *Field )
{
	if ( ScreenerMinMaxCount >= MAXSCREENERFIELDS )
	{
		fprintf ( stderr, "Exceens MAXSCREENERFIELDS\n" );
		exit ( 1 );
	}
	nsStrcpy ( ScreenerMinMaxArray[ScreenerMinMaxCount].Field, Field );
	ScreenerMinMaxCount++;
	return ( ScreenerMinMaxCount - 1 );
}

static void SaveMinimum ( char *Field, int DataType, SHS_DATA_UNION Union, int TestMode )
{
	int		ndx;

	for ( ndx = 0; ndx < ScreenerMinMaxCount; ndx++ )
	{
		if ( nsStrcmp ( ScreenerMinMaxArray[ndx].Field, Field ) == 0 )
		{
			break;
		}
	}

	ScreenerMinMaxArray[ndx].DataType = DataType;
	switch ( DataType )
	{
		case DATATYPE_STRING:
			if ( nsStrlen(Union.String) == 0 || TestMode != RunMode )
			{
				return;
			}
			if ( ndx == ScreenerMinMaxCount )
			{
				ndx = AddField ( Field );
			}
			ScreenerMinMaxArray[ndx].Minimum.String = Union.String;
			break;
		case DATATYPE_DOUBLE:
			if ( Union.Double == 0.0 || TestMode != RunMode )
			{
				return;
			}
			if ( ndx == ScreenerMinMaxCount )
			{
				ndx = AddField ( Field );
			}
			ScreenerMinMaxArray[ndx].Minimum.Double = Union.Double;
			break;
		case DATATYPE_INT:
			if ( Union.Integer == 0 || TestMode != RunMode )
			{
				return;
			}
			if ( ndx == ScreenerMinMaxCount )
			{
				ndx = AddField ( Field );
			}
			ScreenerMinMaxArray[ndx].Minimum.Integer = Union.Integer;
			break;
		case DATATYPE_CHAR:
		case DATATYPE_SHORT:
		case DATATYPE_LONG:
		case DATATYPE_FLOAT:
		case DATATYPE_MYSQL_DATE:
		case DATATYPE_SYSTEM_DATE:
		default:
			printf ( "SaveMinimum: DataType not handled\n" );
			break;
	}
}

static void SaveMaximum ( char *Field, int DataType, SHS_DATA_UNION Union, int TestMode )
{
	int		ndx;

	for ( ndx = 0; ndx < ScreenerMinMaxCount; ndx++ )
	{
		if ( nsStrcmp ( ScreenerMinMaxArray[ndx].Field, Field ) == 0 )
		{
			break;
		}
	}
	ScreenerMinMaxArray[ndx].Maximum = Union;
	switch ( DataType )
	{
		case DATATYPE_STRING:
			if ( nsStrlen(Union.String) == 0 || TestMode != RunMode )
			{
				return;
			}
			if ( ndx == ScreenerMinMaxCount )
			{
				ndx = AddField ( Field );
			}
			ScreenerMinMaxArray[ndx].Maximum.String = Union.String;
			break;
		case DATATYPE_DOUBLE:
			if ( Union.Double == 0.0 || TestMode != RunMode )
			{
				return;
			}
			if ( ndx == ScreenerMinMaxCount )
			{
				ndx = AddField ( Field );
			}
			ScreenerMinMaxArray[ndx].Maximum.Double = Union.Double;
			break;
		case DATATYPE_INT:
			if ( Union.Integer == 0 || TestMode != RunMode )
			{
				return;
			}
			if ( ndx == ScreenerMinMaxCount )
			{
				ndx = AddField ( Field );
			}
			ScreenerMinMaxArray[ndx].Maximum.Integer = Union.Integer;
			break;
		case DATATYPE_CHAR:
		case DATATYPE_SHORT:
		case DATATYPE_LONG:
		case DATATYPE_FLOAT:
		case DATATYPE_MYSQL_DATE:
		case DATATYPE_SYSTEM_DATE:
		default:
			printf ( "SaveMaximum: DataType not handled\n" );
			break;
	}
}


/*----------------------------------------------------------
GetInput: submit_217 = Run Screener  equity screener
GetInput: submit_218 = Run Screener  ETF    screener
----------------------------------------------------------*/
void GetInput ()
{
	int		xa, ndx, len;
	SHS_DATA_UNION Union;

	if (( Remote_Address = getenv ( "REMOTE_ADDR" )) == (char *)0 )
	{
		printf ( "Cannot determine your IP address." );
		return;
	}

	if ( webGetInput () != 0 )
	{
		printf ( "webGetInput: Input error!<br>\n" );
	}

	MenuIndex = 1;
	RunMode = MODE_UNKNOWN;
	SignupCookie = 0;
	AcceptAllRisks = 0;
	NotAFinancialAdvisor = 0;
	DoFurtherResearch = 0;
	TermsAndConditions = 0;
	ContactEmail = "";
	memset ( StockIndexList, '\0', sizeof(StockIndexList) );
	memset ( StockTypeList, '\0', sizeof(StockTypeList) );

	for ( xa = 0; xa < webCount; xa++ )
	{
		if ( nsStrncmp ( webNames[xa], "submit_", 7 ) == 0 )
		{
			RunMode = nsAtoi ( &webNames[xa][7] );

// printf ( "Got RunMode first %d<br>\n", RunMode );
		}
	}

	for ( xa = 0; xa < webCount; xa++ )
	{
		webFixHex ( webValues[xa] );

//		SafeError ( COLOR_NORMAL, "GetInput: %s = %s", webNames[xa] , webValues[xa] );
//		printf (  "GetInput: RunMode %d %s = %s<br>\n", RunMode, webNames[xa] , webValues[xa] );

		if ( nsStrcmp (webNames[xa], "MenuIndex" ) == 0 )
		{
			MenuIndex =  nsAtoi ( webValues[xa] );
		}

		/*----------------------------------------------------------
			login page
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "LoginEmail" ) == 0 )
		{
			LoginEmail = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "LoginPassword" ) == 0 )
		{
			LoginPassword = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitLogin" ) == 0 )
		{
			RunMode = MODE_LOGIN;
		}

		/*----------------------------------------------------------
			forgot page
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "ForgotEmail" ) == 0 )
		{
			ForgotEmail = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitForgot" ) == 0 )
		{
			RunMode = MODE_FORGOT;
		}

		/*----------------------------------------------------------
			responing to forgot email
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "forgot" ) == 0 )
		{
			ForgotString = webValues[xa];
			RunMode = MODE_PAINT_PASSWORD;
		}

		else if ( nsStrcmp ( webNames[xa], "SubmitSavePassword" ) == 0 )
		{
			RunMode = MODE_SAVE_PASSWORD;
		}

		/*----------------------------------------------------------
			logout page
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "LogoutSelect" ) == 0 )
		{
			LogoutSelect = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitLogout" ) == 0 )
		{
			RunMode = MODE_LOGOUT;
		}

		/*----------------------------------------------------------
			signup page
			profile page
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "SignupName" ) == 0 )
		{
			SignupName = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "SignupPager" ) == 0 )
		{
			SignupPager = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "SignupEmail" ) == 0 )
		{
			SignupEmail = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "SignupPassword" ) == 0 )
		{
			SignupPassword = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "ConfirmPassword" ) == 0 )
		{
			ConfirmPassword = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "RefPromoCode" ) == 0 )
		{
			RefPromoCode = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "SignupCookie" ) == 0 )
		{
			SignupCookie = 1;
		}
		else if ( nsStrcmp ( webNames[xa], "AcceptAllRisks" ) == 0 )
		{
			AcceptAllRisks = 1;
		}
		else if ( nsStrcmp ( webNames[xa], "NotAFinancialAdvisor" ) == 0 )
		{
			NotAFinancialAdvisor = 1;
		}
		else if ( nsStrcmp ( webNames[xa], "DoFurtherResearch" ) == 0 )
		{
			DoFurtherResearch = 1;
		}
		else if ( nsStrcmp ( webNames[xa], "TermsAndConditions" ) == 0 )
		{
			TermsAndConditions = 1;
		}
#ifdef ELECTRONIC_SIGNATURE
		else if ( nsStrcmp ( webNames[xa], "MySignature" ) == 0 )
		{
			MySignature =  webValues[xa];
		}
#endif
//		else if ( nsStrcmp ( webNames[xa], "IncludeDividend" ) == 0 )
//		{
//			IncludeDividend =  webValues[xa][0];
//		}
		else if ( nsStrcmp ( webNames[xa], "ChartNumber" ) == 0 )
		{
			ChartNumber =  nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "ChartFast" ) == 0 )
		{
			ChartFast =  nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "ChartSlow" ) == 0 )
		{
			ChartSlow =  nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "CashT" ) == 0 )
		{
			CashT =  nsAtof ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "BondT" ) == 0 )
		{
			BondT =  nsAtof ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "DomT" ) == 0 )
		{
			DomT =  nsAtof ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "ForT" ) == 0 )
		{
			ForT =  nsAtof ( webValues[xa] );
		}

		else if ( nsStrcmp ( webNames[xa], "SubmitSignup" ) == 0 )
		{
			RunMode = MODE_SIGNUP;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitProfile" ) == 0 )
		{
			RunMode = MODE_SAVE_PROFILE;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitChangePassword" ) == 0 )
		{
			RunMode = MODE_CHANGE_PASSWORD;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitChangeEmail" ) == 0 )
		{
			RunMode = MODE_CHANGE_EMAIL;
		}

		else if ( nsStrcmp ( webNames[xa], "Button" ) == 0 )
		{
			if ( nsStrcmp ( webValues[xa], "SubmitProfile" ) == 0 )
			{
				RunMode = MODE_SAVE_PROFILE;
			}
		}

		/*----------------------------------------------------------
			verify link from verifaction email
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "verify" ) == 0 )
		{
			VerifyID = nsAtol ( webValues[xa] );
			RunMode = MODE_VERIFY;
		}
		else if ( nsStrcmp ( webNames[xa], "ipaddr" ) == 0 )
		{
			VerifyIPaddr = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "time" ) == 0 )
		{
			VerifyTime = nsAtol ( webValues[xa] );
		}

		/*----------------------------------------------------------
			contact page
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "ContactSubject" ) == 0 )
		{
			ContactSubject = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "ContactMessage" ) == 0 )
		{
			ContactMessage = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "ContactEmail" ) == 0 )
		{
			ContactEmail = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "submitContact" ) == 0 )
		{
			RunMode = MODE_CONTACT;
		}

		/*----------------------------------------------------------
			sf=5&from=%ld&to=%ld
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "sf" ) == 0 )
		{
			RunMode = MODE_SEND_MESSAGE_TO_MEMBER;
		}
		else if ( nsStrcmp ( webNames[xa], "from" ) == 0 )
		{
			CookieMember.xid = FromMemberID = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "to" ) == 0 )
		{
			ToMemberID = nsAtol ( webValues[xa] );
		}

		/*----------------------------------------------------------
			some RSS readers click on the <guid> address
		----------------------------------------------------------*/
		else if (( nsStrcmp ( webNames[xa], "EventID"   ) == 0 ) ||
				 ( nsStrcmp ( webNames[xa], "RSS_Event" ) == 0 ))
		{
		}

		/*----------------------------------------------------------
			reports
		----------------------------------------------------------*/
		else if ( nsStrncmp ( webNames[xa], "StockIndex_", 11 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][11] ) - SCREENER_AND_REPORT_OFFSET;
			StockIndex[ndx] = webValues[xa][0];
		}
		else if ( nsStrncmp ( webNames[xa], "StockType_", 10 ) == 0 )
		{
			StockType[ndx] = toupper(webValues[xa][0]);
		}
		else if ( nsStrncmp ( webNames[xa], "Broker_", 7 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][7] ) - SCREENER_AND_REPORT_OFFSET;
			sprintf ( Broker[ndx], "%s", webValues[xa] );
		}
		else if ( nsStrncmp ( webNames[xa], "BuySellAll_", 11 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][11] ) - SCREENER_AND_REPORT_OFFSET;
			BuySellAll[ndx] = webValues[xa][0];
		}
		else if ( nsStrncmp ( webNames[xa], "DateRange_", 10 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][10] ) - SCREENER_AND_REPORT_OFFSET;
			DateRange[ndx] = webValues[xa][0];
		}
		else if ( nsStrncmp ( webNames[xa], "TopCount_", 9 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][9] ) - SCREENER_AND_REPORT_OFFSET;
			TopCount[ndx] = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrncmp ( webNames[xa], "TopPercent_", 11 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][11] ) - SCREENER_AND_REPORT_OFFSET;
			TopPercent[ndx] = nsAtof ( webValues[xa] );
		}
		else if ( nsStrncmp ( webNames[xa], "NumberOfDays_", 13 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][13] ) - SCREENER_AND_REPORT_OFFSET;
			NumberOfDays[ndx] = nsAtof ( webValues[xa] );
		}
		else if ( nsStrncmp ( webNames[xa], "NumberOfDays1_", 14 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][14] ) - SCREENER_AND_REPORT_OFFSET;
			NumberOfDays1[ndx] = nsAtof ( webValues[xa] );
		}
		else if ( nsStrncmp ( webNames[xa], "NumberOfDays2_", 14 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][14] ) - SCREENER_AND_REPORT_OFFSET;
			NumberOfDays2[ndx] = nsAtof ( webValues[xa] );
		}
		else if ( nsStrncmp ( webNames[xa], "MinimumVolume_", 14 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][14] ) - SCREENER_AND_REPORT_OFFSET;
			MinimumVolume[ndx] = nsAtof ( webValues[xa] );
		}
#ifdef CRAP
		else if ( nsStrncmp ( webNames[xa], "CalcVolumeMethod_", 17 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][17] ) - SCREENER_AND_REPORT_OFFSET;
			CalcVolumeMethod[ndx] = nsAtoi ( webValues[xa] );
		}
#endif
		else if ( nsStrcmp ( webNames[xa], "CorpAAA" ) == 0 )
		{
			CorpAAA = nsAtof ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "RangeBHS" ) == 0 )
		{
			RangeBHS = nsAtof ( webValues[xa] );
		}
		else if ( nsStrncmp ( webNames[xa], "SortBy_", 7 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][7] ) - SCREENER_AND_REPORT_OFFSET;
			SortBy[ndx] = webValues[xa][0];
		}
		else if ( nsStrncmp ( webNames[xa], "ReportStyle_", 12 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][12] ) - SCREENER_AND_REPORT_OFFSET;
			ReportStyle[ndx] = webValues[xa][0];
		}
		else if ( nsStrncmp ( webNames[xa], "ReportFormat_", 13 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][13] ) - SCREENER_AND_REPORT_OFFSET;
			ReportFormat[ndx] = webValues[xa][0];
		}
		else if ( nsStrncmp ( webNames[xa], "StatementMonth_", 15 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][15] ) - SCREENER_AND_REPORT_OFFSET;
			StatementMonth[ndx] = webValues[xa];
		}
		else if ( nsStrncmp ( webNames[xa], "StatementYear_", 14 ) == 0 )
		{
			ndx = nsAtoi ( &webNames[xa][14] ) - SCREENER_AND_REPORT_OFFSET;
			StatementYear[ndx] = webValues[xa];
		}
		/*---------------------------------------------------------------------------
			moved up ... but left here to avoid unknown name/Union
		---------------------------------------------------------------------------*/
		else if ( nsStrncmp ( webNames[xa], "submit_", 7 ) == 0 )
		{
			// RunMode = nsAtoi ( &webNames[xa][7] );
		}
		else if ( nsStrcmp ( webNames[xa], "RunMode" ) == 0 )
		{
			RunMode = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "TopCountOrMinimumRS" ) == 0 )
		{
			TopCountOrMinimumRS = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "Value" ) == 0 )
		{
			Value = nsAtoi ( webValues[xa] );
		}

		/*----------------------------------------------------------
			Accordian 
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "AccordianID" ) == 0 )
		{
			AccordianID = nsAtoi ( webValues[xa] );
		}

		/*----------------------------------------------------------
			Portfolio 
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "PortfolioID" ) == 0 )
		{
			PortfolioID = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "Pticker" ) == 0 )
		{
			snprintf ( xportfolio.xpticker, sizeof(xportfolio.xpticker), "%s", nsStrToUpcase(webValues[xa]) );
		}
		else if ( nsStrcmp ( webNames[xa], "Pdate" ) == 0 )
		{
			snprintf ( xportfolio.xpdate, sizeof(xportfolio.xpdate), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "Pshares" ) == 0 )
		{
			xportfolio.xpshares = nsAtof ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "Pprice" ) == 0 )
		{
			xportfolio.xpprice = nsAtof ( webValues[xa] );
		}

		else if ( nsStrcmp ( webNames[xa], "Palerttype" ) == 0 )
		{
			xportfolio.xpalerttype[0] =  webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "Palertprice" ) == 0 )
		{
			xportfolio.xpalertprice =  nsAtof(webValues[xa]);
		}

		else if ( nsStrcmp ( webNames[xa], "Ptrailpct" ) == 0 )
		{
			xportfolio.xptrailpct = nsAtof ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "Pbenchmark" ) == 0 )
		{
			snprintf ( xportfolio.xpbenchmark, sizeof(xportfolio.xpbenchmark), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "Palert" ) == 0 )
		{
			xportfolio.xpalert = nsAtof ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "Poptexp" ) == 0 )
		{
			snprintf ( xportfolio.xpoptexp, sizeof(xportfolio.xpoptexp), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "Pbroker" ) == 0 )
		{
			snprintf ( xportfolio.xpbroker, sizeof(xportfolio.xpbroker), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "Pbucket" ) == 0 )
		{
			xportfolio.xpbucket = nsAtoi(webValues[xa]);
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitUpdatePortfolio" ) == 0 )
		{
			RunMode = MODE_UPDATE_PORTFOLIO;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitDeletePortfolio" ) == 0 )
		{
			RunMode = MODE_DELETE_PORTFOLIO;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitInsertPortfolio" ) == 0 )
		{
			RunMode = MODE_INSERT_PORTFOLIO;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitPrintSheet" ) == 0 )
		{
			RunMode = MODE_PRINT_SHEET;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitWatchlistChart" ) == 0 )
		{
			RunMode = MODE_WATCHLIST_CHART;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitPortfolioChart" ) == 0 )
		{
			RunMode = MODE_PORTFOLIO_CHART;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitCombinedPortfolioChart" ) == 0 )
		{
			RunMode = MODE_COMBINED_CHART;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitStockChart" ) == 0 )
		{
			RunMode = MODE_STOCK_CHART;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitStockDividends" ) == 0 )
		{
			RunMode = MODE_STOCK_DIVIDENDS;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitPortfolioDividends" ) == 0 )
		{
			RunMode = MODE_PORTFOLIO_DIVIDENDS;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitStockFundRaw" ) == 0 )
		{
			RunMode = MODE_STOCK_FUNDAMENTALS_RAW;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitStockStatsRaw" ) == 0 )
		{
			RunMode = MODE_STOCK_STATS_RAW;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitStockFundFmt" ) == 0 )
		{
			RunMode = MODE_STOCK_FUNDAMENTALS_FMT;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitChart" ) == 0 )
		{
			RunMode = MODE_TV_CHART;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitImportPortfolio" ) == 0 )
		{
			RunMode = MODE_IMPORT_PORTFOLIO;
		}

		/*----------------------------------------------------------
			watchlist
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "WatchlistID" ) == 0 )
		{
			WatchlistID = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "Wticker" ) == 0 )
		{
			snprintf ( xwatchlist.xwticker, sizeof(xwatchlist.xwticker), "%s", nsStrToUpcase(webValues[xa]) );
		}
		else if ( nsStrcmp ( webNames[xa], "Walerttype" ) == 0 )
		{
			xwatchlist.xwalerttype[0] =  webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "Walertprice" ) == 0 )
		{
			xwatchlist.xwalertprice =  nsAtof(webValues[xa]);
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitUpdateWatchlist" ) == 0 )
		{
			RunMode = MODE_UPDATE_WATCHLIST;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitDeleteWatchlist" ) == 0 )
		{
			RunMode = MODE_DELETE_WATCHLIST;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitInsertWatchlist" ) == 0 )
		{
			RunMode = MODE_INSERT_WATCHLIST;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitImportWatchlist" ) == 0 )
		{
			RunMode = MODE_IMPORT_WATCHLIST;
		}

		/*----------------------------------------------------------
			cash balances
			fixit
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "CashbalID" ) == 0 )
		{
			CashbalID = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "CBacct" ) == 0 )
		{
			snprintf ( xcashbal.xcbacct, sizeof(xcashbal.xcbacct), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "CBdate" ) == 0 )
		{
			snprintf ( xcashbal.xcbdate, sizeof(xcashbal.xcbdate), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "CBamount" ) == 0 )
		{
			xcashbal.xcbamount =  nsAtof(webValues[xa]);
		}
		else if ( nsStrcmp ( webNames[xa], "CBcomment" ) == 0 )
		{
			snprintf ( xcashbal.xcbcomment, sizeof(xcashbal.xcbcomment), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitUpdateCashbal" ) == 0 )
		{
			RunMode = MODE_UPDATE_CASHBAL;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitDeleteCashbal" ) == 0 )
		{
			RunMode = MODE_DELETE_CASHBAL;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitInsertCashbal" ) == 0 )
		{
			RunMode = MODE_INSERT_CASHBAL;
		}

		/*----------------------------------------------------------
			delete stocks from admin tab, Slast < Latest
		----------------------------------------------------------*/
		else if ( nsStrncmp ( webNames[xa], "delete_", 7 ) == 0 )
		{
			if ( DeleteTickersCount >= MAXDELETE )
			{
				SafeError ( COLOR_ERROR, "Exceeds MAXDELETE" );	
			}
			else
			{
				DeleteTickersArray[DeleteTickersCount++] = &webNames[xa][7];
			}
		}

		/*----------------------------------------------------------
			stock maintenance -- find and insert only
			tms	2022-08-08 added Delete
			tms 2023-09-20 added Save
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "field_Sticker" ) == 0 )
		{
			snprintf ( MaintStock.xsticker, sizeof(MaintStock.xsticker), "%s", nsStrToUpcase(webValues[xa]) );
		}
		else if ( nsStrcmp ( webNames[xa], "field_Sdj" ) == 0 )
		{
			MaintStock.xsdj[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "field_Sname" ) == 0 )
		{
			snprintf ( MaintStock.xsname, sizeof(MaintStock.xsname), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_Ssp500" ) == 0 )
		{
			MaintStock.xssp500[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "field_Slast" ) == 0 )
		{
		}
		else if ( nsStrcmp ( webNames[xa], "field_Snasdaq" ) == 0 )
		{
			MaintStock.xsnasdaq[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "field_Stype" ) == 0 )
		{
			MaintStock.xstype[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "field_Stype2" ) == 0 )
		{
			MaintStock.xstype2[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "field_Sdomfor" ) == 0 )
		{
			MaintStock.xsdomfor[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "field_Srussell" ) == 0 )
		{
			MaintStock.xsrussell[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "submitClear" ) == 0 )
		{
			RunMode = MODE_CLEAR_STOCK;
		}
		else if ( nsStrcmp ( webNames[xa], "submitFind" ) == 0 )
		{
			RunMode = MODE_FIND_STOCK;
		}
		else if ( nsStrcmp ( webNames[xa], "submitInsert" ) == 0 )
		{
			RunMode = MODE_INSERT_STOCK;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitSaveStock" ) == 0 )
		{
			RunMode = MODE_SAVE_STOCK;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitDeleteStock" ) == 0 )
		{
			RunMode = MODE_DELETE_STOCK;
		}

		/*----------------------------------------------------------
			Order Entry
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "OrderEntryTicker" ) == 0 )
		{
			OrderEntryTicker = nsStrToUpcase(webValues[xa]);
		}
		else if ( nsStrcmp ( webNames[xa], "OrderEntryDate" ) == 0 )
		{
			OrderEntryDate = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "OrderEntryShares" ) == 0 )
		{
			OrderEntryShares = nsAtof ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "OrderEntryPrice" ) == 0 )
		{
			OrderEntryPrice = nsAtof ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "submitOrderEntryClear" ) == 0 )
		{
			RunMode = MODE_ORDER_ENTRY_CLEAR;
		}
		else if ( nsStrcmp ( webNames[xa], "submitOrderEntryBuy" ) == 0 )
		{
			RunMode = MODE_ORDER_ENTRY_BUY;
		}
		else if ( nsStrcmp ( webNames[xa], "submitOrderEntryDrip" ) == 0 )
		{
			RunMode = MODE_ORDER_ENTRY_DRIP;
		}
		else if ( nsStrcmp ( webNames[xa], "submitOrderEntrySell" ) == 0 )
		{
			RunMode = MODE_ORDER_ENTRY_SELL;
		}
		else if ( nsStrcmp ( webNames[xa], "submitConfirmBuy" ) == 0 )
		{
			RunMode = MODE_CONFIRM_BUY;
		}
		else if ( nsStrcmp ( webNames[xa], "submitConfirmDrip" ) == 0 )
		{
			RunMode = MODE_CONFIRM_DRIP;
		}
		else if ( nsStrcmp ( webNames[xa], "submitConfirmSell" ) == 0 )
		{
			RunMode = MODE_CONFIRM_SELL;
		}

		/*----------------------------------------------------------
			user made payment on paypal
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "tx" ) == 0 )
		{
			Payment.Transaction = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "st" ) == 0 )
		{
			Payment.Status = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "amt" ) == 0 )
		{
			Payment.Amount = nsAtof ( webValues[xa] );
			RunMode = MODE_PROCESS_PAYMENT;
		}
		else if ( nsStrcmp ( webNames[xa], "cc" ) == 0 )
		{
			Payment.Currency = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "cm" ) == 0 )
		{
			Payment.Category = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "item_number" ) == 0 )
		{
			Payment.ItemNumber = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "sig" ) == 0 )
		{
			Payment.Signature = webValues[xa];
		}

		else if ( nsStrcmp ( webNames[xa], "token" ) == 0 )
		{
			/* ignore - user canceled payment */
		}

		/*----------------------------------------------------------
			Chart navigation
		----------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "ChartRowCount" ) == 0 )
		{
			ChartRowCount = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "ChartCurrentRow" ) == 0 )
		{
			ChartCurrentRow = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitChartPrevious" ) == 0 )
		{
			RunMode = MODE_CHART_PREVIOUS;
		}
		else if ( nsStrcmp ( webNames[xa], "SubmitChartNext" ) == 0 )
		{
			RunMode = MODE_CHART_NEXT;
		}

		/*----------------------------------------------------------
			Fundamental screener
		----------------------------------------------------------*/
		else if ( nsStrncmp ( webNames[xa], "cb_fund_", 8 ) == 0 )
		{
			if ( RunMode == MODE_REPORT_ETF_FUNDAMENTALS && webNames[xa][8] == 'E' )
			{
				StockIndexList[0] = 'E';
				StockIndexList[1] = '\0';
				len = 1;
			}
			else if ( RunMode == MODE_REPORT_EQUITY_FUNDAMENTALS && webNames[xa][8] == 'O' )
			{
				StockIndexList[0] = 'O';
				StockIndexList[1] = '\0';
				len = 1;
			}
			else if ( RunMode == MODE_REPORT_EQUITY_FUNDAMENTALS && webNames[xa][8] != 'E' )
			{
				len = nsStrlen ( StockIndexList );
				StockIndexList[len] = webNames[xa][8];
			}
// printf ( "StockIndexList %s<br>\n", StockIndexList );
		}
		else if ( nsStrncmp ( webNames[xa], "Fentval_min_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fentval", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fentval_max_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fentval", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fvalue_min_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fvalue", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fvalue_max_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fvalue", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fepsttm_min_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fepsttm", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fepsttm_max_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fepsttm", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fpettm_min_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fpettm", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fpettm_max_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fpettm", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fepsfwd_min_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fepsfwd", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fepsfwd_max_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fepsfwd", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fpefwd_min_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fpefwd", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fpefwd_max_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fpefwd", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fpb_min_", 8 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fpb", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][8]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fpb_max_", 8 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fpb", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][8]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fmargin_min_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fmargin", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fmargin_max_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fmargin", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fyield_min_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fyield", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fyield_max_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fyield", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fbeta_min_", 10 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fbeta", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][10]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fbeta_max_", 10 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fbeta", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][10]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fgrow5_min_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fgrow5", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fgrow5_max_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fgrow5", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fgrow1_min_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fgrow1", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fgrow1_max_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fgrow1", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Ftarget_min_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Ftarget", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Ftarget_max_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Ftarget", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Freco_min_", 10 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Freco", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][10]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Freco_max_", 10 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Freco", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][10]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fquick_min_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fquick", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fquick_max_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fquick", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fdebteq_min_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fdebteq", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fdebteq_max_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fdebteq", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "FreturnA_min_", 13 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "FreturnA", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][13]) );
		}
		else if ( nsStrncmp ( webNames[xa], "FreturnA_max_", 13 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "FreturnA", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][13]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Frevenuegrow_min_", 17 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Frevenuegrow", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][17]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Frevenuegrow_max_", 17 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Frevenuegrow", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][17]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fearngrow_min_", 14 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fearngrow", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][14]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fearngrow_max_", 14 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fearngrow", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][14]) );
		}

		else if ( nsStrncmp ( webNames[xa], "Ffcfgrow_min_", 13 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Ffcfgrow", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][13]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Ffcfgrow_max_", 13 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Ffcfgrow", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][13]) );
		}

		else if ( nsStrncmp ( webNames[xa], "Ffcfcv_min_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Ffcfcv", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Ffcfcv_max_", 11 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Ffcfcv", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][11]) );
		}

		/*----------------------------
			etf
		----------------------------*/
		else if ( nsStrncmp ( webNames[xa], "Favgret3_min_", 13 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Favgret3", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][13]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Favgret3_max_", 13 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Favgret3", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][13]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Favgret5_min_", 13 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Favgret5", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][13]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Favgret5_max_", 13 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Favgret5", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][13]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Ftotasst_min_", 13 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Ftotasst", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][13]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Ftotasst_max_", 13 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Ftotasst", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][13]) );
		}

		else if ( nsStrncmp ( webNames[xa], "Fexpratio_min_", 14 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fexpratio", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][14]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fexpratio_max_", 14 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fexpratio", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][14]) );
		}
#ifdef WAS_SHORT
		else if ( nsStrncmp ( webNames[xa], "Fmstar_min_", 11 ) == 0 )
		{
			Union.Integer = nsAtoi(webValues[xa]);
			SaveMinimum ( "Fmstar", DATATYPE_INT, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fmstar_max_", 11 ) == 0 )
		{
			Union.Integer = nsAtoi(webValues[xa]);
			SaveMaximum ( "Fmstar", DATATYPE_INT, Union, nsAtoi(&webNames[xa][11]) );
		}
#endif
		else if ( nsStrncmp ( webNames[xa], "Fmstar_min_", 11 ) == 0 )
		{
			Union.String = webValues[xa];
			SaveMinimum ( "Fmstar", DATATYPE_STRING, Union, nsAtoi(&webNames[xa][11]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fmstar_max_", 11 ) == 0 )
		{
			Union.String = webValues[xa];
			SaveMaximum ( "Fmstar", DATATYPE_STRING, Union, nsAtoi(&webNames[xa][11]) );
		}



		else if ( nsStrncmp ( webNames[xa], "Ffactset_min_", 13 ) == 0 )
		{
			Union.String = webValues[xa];
			SaveMinimum ( "Ffactset", DATATYPE_STRING, Union, nsAtoi(&webNames[xa][13]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Ffactset_max_", 13 ) == 0 )
		{
			Union.String = webValues[xa];
			SaveMaximum ( "Ffactset", DATATYPE_STRING, Union, nsAtoi(&webNames[xa][13]) );
		}

		else if ( nsStrncmp ( webNames[xa], "Fmktcap_min_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fmktcap", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fmktcap_max_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fmktcap", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fvolume_min_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMinimum ( "Fvolume", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}
		else if ( nsStrncmp ( webNames[xa], "Fvolume_max_", 12 ) == 0 )
		{
			Union.Double = nsAtof(webValues[xa]);
			SaveMaximum ( "Fvolume", DATATYPE_DOUBLE, Union, nsAtoi(&webNames[xa][12]) );
		}

		else
		{
			SafeError ( COLOR_ERROR, "Unknown name= value %s=%s", webNames[xa] , webValues[xa] );
			RunMode = MODE_UNKNOWN;
		}
	}
}

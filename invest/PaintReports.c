/*----------------------------------------------------------------------------
	Program : invest/PaintReports.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint the screeners page. 
				Don't remember why this isn't called PaintScreeners.c
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

#define		NEED_ACCOUNT_TOP	1
#define		NEED_ACCOUNT_BUTTON	2
static	int		NeedAccount = NEED_ACCOUNT_BUTTON;

static void PaintNeedAccount ( int Columns )
{
	printf ("<tr>\n" );
	printf ("<td class='report-table' align='left' colspan='%d'>\n", Columns );
	printf ( "You must be signed in to run screeners!<br>\n" );
	PushSignup ();
	printf ("</td>\n" );
	printf ("</tr>\n" );
}

void PaintReports ()
{
	int		IsDisabled = 0;
#ifdef UNIX
	int		OptionArray[4];
	int		ndx;
	/*----------------------------------------------------------
		cross over arrays
	----------------------------------------------------------*/
	int		Array1 [] = { 5, 10, 20, 50, 100, 200, 0 };
	int		Array2 [] = {    10, 20, 50, 100, 200, 0 };
	/*----------------------------------------------------------
		52 week arrays
	----------------------------------------------------------*/
	int		Within [] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20, 0 };
	int		Dips   [] = { 3, 5, 10, 15, 20, 25, 30, 40, 50, 0 };
#endif

	if ( CookieMember.xid == 0 && NeedAccount == NEED_ACCOUNT_TOP )
	{
		printf ( "<div class='error border-warn'>\n" );
		printf ( "You must be signed in to run screeners!<br>\n" );
		PushSignup ();
		printf ( "</div>\n" );
	}

	if ( CookieMember.xid == 0 )
	{
		PortfolioRecordCount = WatchlistRecordCount = TempRecordCount = 0;
		IsDisabled = 1;
	}
	else
	{
		sprintf ( WhereClause, "Pmember = %ld", CookieMember.xid );
		PortfolioRecordCount = dbySelectCount ( &MySql, "portfolio", WhereClause, LogFileName );
		sprintf ( WhereClause, "Wmember = %ld", CookieMember.xid );
		WatchlistRecordCount = dbySelectCount ( &MySql, "watchlist", WhereClause, LogFileName );
		sprintf ( WhereClause, "Tmember = %ld", CookieMember.xid );
		TempRecordCount = dbySelectCount ( &MySql, "temp", WhereClause, LogFileName );

		if ( xmember.xmrole[0] == ROLE_FREE )
		{
			RptlogRecordCount = GetReportCount ();
			if ( RptlogRecordCount >= FREE_MEMBER_REPORTS_LIMIT )
			{
				printf ( "<div class='error border-warn'>\n" );
				PrintUpgradeMessage ( 2 );
				printf ( "</form>\n" );
				PaintPaypal ();
				printf ( "</div>\n" );
				IsDisabled = 1;
			}
		}
	}

printf ( "<div class='container'>\n" );
printf ( "  <div class='panel-group' id='accordion'>\n" );

#ifdef UNIX
	/*----------------------------------------------------------
		market moving averages
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "MARKET MOVING AVERAGES", 0 );

	PaintMovingAverages ( 1 );

	PaintEndReportStory ();

	/*----------------------------------------------------------
		accum distro history
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "ACCUMULATE/DISTRIBUTE HISTORY", 0 );

	PaintDateRange ( MODE_REPORT_AD_HISTORY );

	PaintFormat ( MODE_REPORT_AD_HISTORY, 0 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_AD_HISTORY, "Run Report", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}

	PaintEndReportStory ();
#endif

	/*----------------------------------------------------------
		equity fundamentals
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "EQUITY FUNDAMENTALS SCREENER", 0 );

	PaintEquityFundamentalScreener ();

	PaintFormat ( MODE_REPORT_EQUITY_FUNDAMENTALS, 0 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_EQUITY_FUNDAMENTALS, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}

	PaintEndReportStory ();

#ifdef UNIX
	/*----------------------------------------------------------
		ETF and MUTUALFUND fundamentals
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "ETF FUNDAMENTALS SCREENER", 0 );

	PaintETF_FundamentalScreener ();

	PaintFormat ( MODE_REPORT_ETF_FUNDAMENTALS, 0 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_ETF_FUNDAMENTALS, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}

	PaintEndReportStory ();

	/*----------------------------------------------------------
		near 52 
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "NEAR OLD 52 WEEK HIGH (CUP)", 0 );

	PaintStockUniverse ( MODE_REPORT_52_WEEK );

	PaintNumberOfDays ( MODE_REPORT_52_WEEK, "52 Week High Age (Months)", 2 );

	PaintTwoSelectorOfDays ( MODE_REPORT_52_WEEK, "Current Within Percent", Within, 5, "Minimum Dip Perecnt", Dips, 25 );

	PaintFormat ( MODE_REPORT_52_WEEK, 1 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_52_WEEK, "Run Report", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}


	PaintEndReportStory ();

	/*----------------------------------------------------------
		regression channel
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "REGRESSION CHANNEL SCREENER", 0 );

	PaintStockUniverse ( MODE_REPORT_REGRESS );

	PaintDateRange ( MODE_REPORT_REGRESS );

	PaintTopCount ( MODE_REPORT_REGRESS, 10 );

	OptionArray[0] = SORTBY_RETURN;
	OptionArray[1] = SORTBY_CORREL;
	OptionArray[2] = SORTBY_VARIANCE;
	PaintSortBy ( MODE_REPORT_REGRESS, OptionArray, 3 );

	PaintFormat ( MODE_REPORT_REGRESS, 1 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_REGRESS, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}

	PaintEndReportStory ();

	/*----------------------------------------------------------
		accum volume
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "ACCUMULATE/DISTRIBUTE VOLUME SCREENER", 0 );

	PaintStockUniverse ( MODE_REPORT_VOLUME_ACCUM );
	
	PaintTopPercent ( MODE_REPORT_VOLUME_ACCUM, "Volume increase percentage", 125.0 );

	PaintNumberOfDays ( MODE_REPORT_VOLUME_ACCUM, "Current trading days", 30 );

	PaintMinimumVolume ( MODE_REPORT_VOLUME_ACCUM, 250000 );
	
#ifdef CRAP
	PaintVolumeCalcMethod ( MODE_REPORT_VOLUME_ACCUM );
#endif

	PaintFormat ( MODE_REPORT_VOLUME_ACCUM, 1 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_VOLUME_ACCUM, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}

	PaintEndReportStory ();

	/*----------------------------------------------------------
		increasing volume
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "INCREASING VOLUME SCREENER", 0 );

	PaintStockUniverse ( MODE_REPORT_VOLUME_INCREASE );

	PaintTopPercent ( MODE_REPORT_VOLUME_INCREASE, "Volume increased percentage", 120.0 );

	PaintTwoNumberOfDays ( MODE_REPORT_VOLUME_INCREASE, "Current trading days", 5, "Previous trading days", 45 );

	PaintMinimumVolume ( MODE_REPORT_VOLUME_INCREASE, 250000 );

	PaintFormat ( MODE_REPORT_VOLUME_INCREASE, 1 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_VOLUME_INCREASE, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}


	PaintEndReportStory ();

	/*----------------------------------------------------------
		moving average crossover
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "MOVING AVERAGES CROSSOVER SCREENER", 0 );

	PaintStockUniverse ( MODE_REPORT_MA_CROSSOVER );

	PaintTwoSelectorOfDays ( MODE_REPORT_MA_CROSSOVER, "Fast AVG days", Array1, 10, "Slow AVG days", Array2, 50 );

	PaintNumberOfDays ( MODE_REPORT_MA_CROSSOVER, "Days back", 5 );

// fixit - wtf?
	ndx = MODE_REPORT_MA_CROSSOVER -  - SCREENER_AND_REPORT_OFFSET;

	printf ("<tr>\n" );
	printf ("<td class='report-table'>\n" );
	printf ("Buy or Sell Signal\n" );
	printf ("</td>\n" );
	printf ("<td class='report-table' colspan='3'>\n" );
	printf ("<select name='BuySellAll_%d'>\n", MODE_REPORT_MA_CROSSOVER );
	printf ("<option value='B'%s>Buy</option>\n",  BuySellAll[ndx] == 'B' ? " selected" : "" );
	printf ("<option value='S'%s>Sell</option>\n", BuySellAll[ndx] == 'S' ? " selected" : "" );
	printf ("<option value='A'%s>All</option>\n", BuySellAll[ndx] == 'A' ? " selected" : "" );
	printf ("</select>\n" );
	printf ("</td>\n" );
	printf ("</tr>\n" );

	PaintFormat ( MODE_REPORT_MA_CROSSOVER, 1 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_MA_CROSSOVER, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}


	PaintEndReportStory ();

	/*----------------------------------------------------------
		moving average ratio
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "MOVING AVERAGES RATIO SCREENER", 0 );

	PaintStockUniverse ( MODE_REPORT_MA_RATIO );

	PaintTwoSelectorOfDays ( MODE_REPORT_MA_RATIO, "Fast AVG days", Array1, 10, "Slow AVG days", Array2, 50 );

	PaintNumberOfDays ( MODE_REPORT_MA_RATIO, "Days back", 5 );

// fixit -- minimum
	PaintTopPercent ( MODE_REPORT_MA_RATIO, "Minimum Percent", 90.0 );

	PaintFormat ( MODE_REPORT_MA_RATIO, 1 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_MA_RATIO, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}


	PaintEndReportStory ();

	/*----------------------------------------------------------
		directional index crossover
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "-DI/+DI CROSSOVER SCREENER", 0 );

	PaintStockUniverse ( MODE_REPORT_DI_CROSSOVER );

	PaintNumberOfDays ( MODE_REPORT_DI_CROSSOVER, "Days back", 1 );

// -buyadx #     = minimum ADX for buy, default 30.0
	PaintTopPercent ( MODE_REPORT_DI_CROSSOVER, "Minimum ADX", 30.0 );

// -buyamt #     = Dollar to buy, default 1000.0
// -minshares #  = Minimum shares to buy. Default 10
	PaintTwoNumberOfDays ( MODE_REPORT_DI_CROSSOVER, "Each position", 1000, "Minimum shares", 10 );

	PaintFormat ( MODE_REPORT_DI_CROSSOVER, 1 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_DI_CROSSOVER, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}

	PaintEndReportStory ();

	/*----------------------------------------------------------
		ADX Trending
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "ADX TRENDING SCREENER", 0 );

	PaintStockUniverse ( MODE_REPORT_ADX_TREND );

//	PaintTopPercent ( MODE_REPORT_ADX_TREND, "Minimum ADX", 30.0 );

// -minadx # = minimum ADX 
// -min_di #     = Dollar to buy, default 1000.0
	PaintTwoNumberOfDays ( MODE_REPORT_ADX_TREND, "Minimum ADX", 30, "-DI/+DI Range", 10 );

// -type B S A
	printf ("<tr>\n" );
	printf ("<td class='report-table'>\n" );
	printf ("Buy or Sell Signal\n" );
	printf ("</td>\n" );
	printf ("<td class='report-table' colspan='3'>\n" );
	printf ("<select name='BuySellAll_%d'>\n", MODE_REPORT_ADX_TREND );
	printf ("<option value='B'%s>Buy</option>\n",  BuySellAll[ndx] == 'B' ? " selected" : "" );
	printf ("<option value='S'%s>Sell</option>\n", BuySellAll[ndx] == 'S' ? " selected" : "" );
	printf ("<option value='A'%s>All</option>\n", BuySellAll[ndx] == 'A' ? " selected" : "" );
	printf ("</select>\n" );
	printf ("</td>\n" );
	printf ("</tr>\n" );

	PaintFormat ( MODE_REPORT_ADX_TREND, 1 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_ADX_TREND, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}

	PaintEndReportStory ();

	/*----------------------------------------------------------
		breakout 
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "BREAKOUT SCREENER", 0 );

	PaintStockUniverse ( MODE_REPORT_BREAKOUT );

	PaintNumberOfDays ( MODE_REPORT_BREAKOUT, "Price increase percentage", 3 );

	PaintTopPercent ( MODE_REPORT_BREAKOUT, "Volume increase percentage", 25.0 );

	PaintMinimumVolume ( MODE_REPORT_BREAKOUT, 300000 );

	PaintFormat ( MODE_REPORT_BREAKOUT, 1 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_BREAKOUT, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}

	PaintEndReportStory ();

	/*----------------------------------------------------------
		rsi screener 
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "RSI SCREENER", 0 );

	PaintStockUniverse ( MODE_REPORT_RELATIVE_STRENGTH );

	OptionArray[0] = SORTBY_SECTOR;
	OptionArray[1] = SORTBY_INDUSTRY;
	OptionArray[2] = SORTBY_NONE;
	PaintSortBy ( MODE_REPORT_RELATIVE_STRENGTH, OptionArray, 3 );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "How many?\n" );
	printf ( "</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='TopCountOrMinimumRS'>\n" );
	printf ( "<option value='T'>Top Count</option>\n" );
	printf ( "<option value='B'>Bottom Count</option>\n" );
	printf ( "<option value='N'>Minimum RSI</option>\n" );
	printf ( "<option value='X'>Maximum RSI</option>\n" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "Value\n" );
	printf ( "</td>\n" );
	printf ( "<td>\n" );
	if ( Value == 0 )
	{
		Value = 5;
	}
	printf ( "<input type='search' name='Value' value='%d'>\n", Value );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	PaintFormat ( MODE_REPORT_RELATIVE_STRENGTH, 1 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_RELATIVE_STRENGTH, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}

	PaintEndReportStory ();

	/*----------------------------------------------------------
		ctb screener 
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "COMPARE TO S&P 500", 0 );

	PaintStockUniverse ( MODE_REPORT_CTB_STRENGTH );

//	OptionArray[0] = SORTBY_SECTOR;
//	OptionArray[1] = SORTBY_INDUSTRY;
//	OptionArray[2] = SORTBY_NONE;
//	PaintSortBy ( MODE_REPORT_CTB_STRENGTH, OptionArray, 3 );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "How many?\n" );
	printf ( "</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='TopCountOrMinimumRS'>\n" );
	printf ( "<option value='T'>Top Count</option>\n" );
	printf ( "<option value='B'>Bottom Count</option>\n" );
	printf ( "<option value='N'>Minimum CTB</option>\n" );
	printf ( "<option value='X'>Maximum CTB</option>\n" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "Value\n" );
	printf ( "</td>\n" );
	printf ( "<td>\n" );
	if ( Value == 0 )
	{
		Value = 5;
	}
	printf ( "<input type='search' name='Value' value='%d'>\n", Value );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	PaintFormat ( MODE_REPORT_CTB_STRENGTH, 1 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_CTB_STRENGTH, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}


	PaintEndReportStory ();

	/*----------------------------------------------------------
		cooper hit and run 
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "JEFF COOPER HIT AND RUN SCREENER", 0 );

	PaintStockUniverse ( MODE_REPORT_COOPER );

	PaintFormat ( MODE_REPORT_COOPER, 1 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_COOPER, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}

	PaintEndReportStory ();

	/*----------------------------------------------------------
		intrinsic value report
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "INTRINSIC VALUE REPORT (BUY/HOLD/SELL)", 0 );

	PaintStockUniverse ( MODE_REPORT_VALUE );

	/*----------------------------
		-currentAAA #.# current yield of AAA corp bonds, default 3.0 percent
	----------------------------*/
	if ( CorpAAA < 0.01 )
	{
		CorpAAA = 3.0;
	}

	printf ("<tr>\n" );
	printf ("<td class='report-table'>%s</td>\n", "Corporate AAA Bond Rate" );
	printf ("<td class='report-table'>" );
	printf ( "<input type='search' name='CorpAAA'" );
	printf ( " value='%.2f'", CorpAAA );
	printf ( "></td>\n" );
	printf ("<td class='report-table' colspan='2'>%s</td>\n", "<a target='popup' href='https://fred.stlouisfed.org/series/AAA'>reference</a>" );
	printf ("</tr>\n" );

	/*----------------------------
		-range #.#, percent for Buy/Hold/Sell, default is 15.0 percent
	----------------------------*/
	if ( RangeBHS < 0.01 )
	{
		RangeBHS = 15.0;
	}

	printf ("<tr>\n" );
	printf ("<td class='report-table'>%s</td>\n", "Percent Range for Buy/Hold/Sell" );
	printf ("<td class='report-table' colspan='3'>" );
	printf ( "<input type='search' name='RangeBHS'" );
	printf ( " value='%.2f'", RangeBHS );
	printf ( "></td>\n" );
	printf ("</tr>\n" );


	PaintFormat ( MODE_REPORT_VALUE, 1 );

	if ( IsDisabled == 0 )
	{
		PaintSubmit ( MODE_REPORT_VALUE, "Run Screener", 2 );
	}
	else if ( NeedAccount == NEED_ACCOUNT_BUTTON )
	{
		PaintNeedAccount ( 2 );
	}

	PaintEndReportStory ();
#endif

#ifdef UNIX
	/*----------------------------------------------------------
		glossary
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "GLOSSARY", 0 );

	printf ( "<tr><td><b>Stock universe</b> - select stocks from index, portfolio or watchlist</td></tr>\n" );
	printf ( "<tr><td><b>Date range</b> - past number of months or years to examine</td></tr>\n" );
	printf ( "<tr><td><b>Top count</b> - limit report to the top number of results</td></tr>\n" );
	printf ( "<tr><td><b>Sort by</b> - sort report by selected column</td></tr>\n" );
	printf ( "<tr><td><b>Volume increase percentage</b> - report only greater than this percentage</td></tr>\n" );
	printf ( "<tr><td><b>Minimum volume</b> - minimum current volume to report</td></tr>\n" );
	printf ( "<tr><td><b>Volume calculation method</b> - attribute days volume to buyers or sellers based on open or previous close</td></tr>\n" );
	printf ( "<tr><td><b>Current trading days</b> - number of trading days ending with latest history</td></tr>\n" );
	printf ( "<tr><td><b>Previous trading days</b> - number of trading days prior to the current period above.</td></tr>\n" );
	printf ( "<tr><td><b>Format</b> - report format, if not screen, then sent to your email</td></tr>\n" );
#else
	PaintStartReportStory ( STORY_TITLE, "IEX DEAD", 0 );

	printf ( "SYSTEM HAS LIMITED HISTORICAL DATA (ONLY FOR OWNED SECURITIES).<br>" );
	printf ( "THESE SCREENERS ARE THEREFORE NOT USEFUL:<br>" );
	printf ( "&emsp;Market moving averages<br>" );
	printf ( "&emsp;Accumulate/distribute history<br>" );
	printf ( "&emsp;ETF fundamentals screener<br>" );
	printf ( "&emsp;Near old 52 week high (cup)<br>" );
	printf ( "&emsp;Regression channel screener<br>" );
	printf ( "&emsp;Accumulate/distribute volume screener<br>" );
	printf ( "&emsp;Increasing volume screener<br>" );
	printf ( "&emsp;Moving averages crossover screener<br>" );
	printf ( "&emsp;Moving averages ratio screener<br>" );
	printf ( "&emsp;-di/+di Crossover screener<br>" );
	printf ( "&emsp;ADX trending screener<br>" );
	printf ( "&emsp;Breakout screener<br>" );
	printf ( "&emsp;RSI screener<br>" );
	printf ( "&emsp;Compare to s&p 500<br>" );
	printf ( "&emsp;Jeff Cooper hit and run screener<br>" );
	printf ( "&emsp;Intrinsic value report (buy/hold/sell)<br>" );
#endif
	
	PaintEndReportStory ();

printf ( "  </div>\n" );
printf ( "</div>\n" );

}

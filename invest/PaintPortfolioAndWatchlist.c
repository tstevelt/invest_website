/*----------------------------------------------------------------------------
	Program : invest/PaintPortfolioAndWatchlist.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint the portfolio page, using collapsing stories.
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

static void PaintPortfolioHeader ( int Count )
{
	printf ( "<table class='table-condensed table-borderless'>\n" );
	printf ( "<tr>\n" );
	printf ( "<td align='left'>TICKER</td>\n" );
	printf ( "<td align='left' class='hidden-xs'>COMPANY</td>\n" );
	printf ( "<td align='right'>SHARES</td>\n" );
	printf ( "<td align='right'>COST</td>\n" );
	printf ( "<td align='right' class='hidden-xs hidden-sm'>TTM&nbsp;DIV</td>\n" );
	printf ( "<td align='center' class='hidden-xs hidden-sm'>YIELD</td>\n" );
	printf ( "<td align='center' class='hidden-xs hidden-sm'>ALERT</td>\n" );
	printf ( "<td align='left' class='hidden-xs hidden-sm'>OPTION</td>\n" );
	printf ( "</tr>\n" );
}

extern	double	xstock_ttm_dividends;

static int PaintStockP ()
{
	static int lineno = 0;
	static char PrevBroker[12];

	if ( lineno == 0 )
	{
		PaintPortfolioHeader ( lineno );
		sprintf ( PrevBroker, "zzzzzzzzzz" );
	}

	sprintf ( WhereClause, "Sticker = '%s'", xportfolio.xpticker );
	LoadStock ( &MySql, WhereClause, &xstock, 0, 1 );

	/*----------------------------------------------------------
		load fundamentals
	----------------------------------------------------------*/
	sprintf ( WhereClause, "Fticker = '%s'", xportfolio.xpticker );
	if ( LoadFundamental ( &MySql, WhereClause, &xfundamental, 0 ) != 1 )
	{
		memset ( &xfundamental, '\0', sizeof(xfundamental) );
	}

	if ( nsStrcmp ( PrevBroker, xportfolio.xpbroker ) != 0 )
	{
		sprintf ( PrevBroker, "%s", xportfolio.xpbroker );

		printf ( "<tr>\n" );
		printf ( "<td colspan='8'>\n" );
		printf ( "%s\n", xportfolio.xpbroker );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );

	}

	printf ( "<tr>\n" );

	printf ( "<td><a href='invest.cgi?MenuIndex=%d&PortfolioID=%ld&RunMode=%d'>%s</a></td>\n", 
			MenuIndex, xportfolio.xid, MODE_PAINT_EDIT_PORTFOLIO, xportfolio.xpticker );

	printf ( "<td align='left' class='hidden-xs'>%-30.30s</td>\n", xstock.xsname ); 

	printf ( "<td align='right'>" );
	printf ( "%.2f", /* xportfolio.xid, */ xportfolio.xpshares );
	printf ( "</td>\n" );

	printf ( "<td align='right'>" );
	printf ( "%.2f", /* xportfolio.xid, */ xportfolio.xpprice );
	printf ( "</td>\n" );

	printf ( "<td align='right' class='hidden-xs hidden-sm'>%.2f</td>\n", xstock_ttm_dividends ); 

	printf ( "<td align='right' class='hidden-xs hidden-sm'>%.2f</td>\n", xfundamental.xfyield ); 

	printf ( "<td align='center' class='hidden-xs hidden-sm'>%s</td>\n", AlertDescription(xportfolio.xpalerttype[0]) ); 

	printf ( "<td align='left' class='hidden-xs hidden-sm'>%s</td>\n", 
		xportfolio.xpoptexp[0] != '0' ? xportfolio.xpoptexp : "&nbsp;" ); 

	printf ( "</tr>\n" );

	lineno++;
	
	return ( 0 );
}

static void PaintWatchlistHeader ( int Count )
{
	printf ( "<table class='table-condensed table-borderless'>\n" );
	printf ( "<tr>\n" );
	printf ( "<th>TICKER</th>\n" );
	printf ( "<th>COMPANY</th>\n" );
	printf ( "<th>ALERT TYPE</th>\n" );
	printf ( "<th>ALERT PRICE</th>\n" );
	printf ( "</tr>\n" );
}

static int PaintStockW ()
{
	static int lineno = 0;

	if ( lineno == 0 )
	{
		PaintWatchlistHeader ( lineno );
	}

	sprintf ( WhereClause, "Sticker = '%s'", xwatchlist.xwticker );
	LoadStock ( &MySql, WhereClause, &xstock, 0, 0 );

	printf ( "<tr>\n" );

	printf ( "<td><a href='invest.cgi?MenuIndex=%d&WatchlistID=%ld&RunMode=%d'>%s</a></td>\n", 
			MenuIndex, xwatchlist.xid, MODE_PAINT_EDIT_WATCHLIST, xwatchlist.xwticker );

	printf ( "<td>%s</td>\n", xstock.xsname );

	switch ( xwatchlist.xwalerttype[0] )
	{
		case ALERT_NONE:
			printf ( "<td>&nbsp;</td><td>&nbsp;</td>\n" );
			break;
		case ALERT_ABOVE_AMT:
		case ALERT_BELOW_AMT:
			printf ( "<td>%s</td><td>%.2f</td>\n", AlertDescription(xwatchlist.xwalerttype[0]), xwatchlist.xwalertprice );
			break;
		default:
			printf ( "<td colspan='2'>%s</td>\n", AlertDescription(xwatchlist.xwalerttype[0]) );
			break;
	}

	printf ( "</tr>\n" );

	lineno++;

	return ( 0 );
}

static void PaintCashHeader ( int Count )
{
	printf ( "<table class='table-condensed table-borderless'>\n" );
	printf ( "<tr>\n" );
	printf ( "<th>ACCOUNT</th>\n" );
	printf ( "<th>AS-OF DATE</th>\n" );
	printf ( "<th>AMOUNT</th>\n" );
	printf ( "<th>COMMENT</th>\n" );
	printf ( "</tr>\n" );
}

static	double	TotalCashBalance = 0.0;

static int PaintCashbal ()
{
	static int lineno = 0;

	if ( lineno == 0 )
	{
		PaintCashHeader ( lineno );
	}

	printf ( "<tr>\n" );

	printf ( "<td><a href='invest.cgi?MenuIndex=%d&CashbalID=%ld&RunMode=%d'>%s</a></td>\n", 
			MenuIndex, xcashbal.xid, MODE_PAINT_EDIT_CASHBAL, xcashbal.xcbacct );

	printf ( "<td>%s</td>\n", xcashbal.xcbdate );

	printf ( "<td>%.2f</td>\n", xcashbal.xcbamount );

	printf ( "<td>%s</td>\n", xcashbal.xcbcomment );

	printf ( "</tr>\n" );

	TotalCashBalance += xcashbal.xcbamount;
	lineno++;

	return ( 0 );
}


void PaintPortfolioAndWatchlist ()
{
	int		CanAddToPortfolio = 1;
	int		CanAddToWatchlist = 1;
	int		IsDisabled = 0;
	int		OptionArray[4];
	int		ndx = RunMode - SCREENER_AND_REPORT_OFFSET;

	if ( CookieMember.xid == 0 )
	{
		printf ( "<div class='error border-warn'>\n" );
		printf ( "You must be signed in to setup portfolio and watchlist!<br>\n" );
		PushSignup ();
		printf ( "</div>\n" );
		PaintTableDescription ( "watchlist" );
		PaintTableDescription ( "watchlist_report" );
		PaintTableDescription ( "portfolio" );
		PaintTableDescription ( "portfolio_report" );
		PaintTableDescription ( "alert" );
		return;
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
				IsDisabled = 1;
			}
		}
	}

	if ( AccordianID == 0 )
	{
		switch ( RunMode )
		{
			case MODE_PAINT_EDIT_WATCHLIST:
			case MODE_PAINT_IMPORT_WATCHLIST:
			case MODE_UPDATE_WATCHLIST:
			case MODE_DELETE_WATCHLIST:
			case MODE_INSERT_WATCHLIST:
				AccordianID = ACCORDIAN_WATCHLIST;
				break;

			case MODE_PAINT_EDIT_PORTFOLIO:
			case MODE_PAINT_IMPORT_PORTFOLIO:
			case MODE_UPDATE_PORTFOLIO:
			case MODE_DELETE_PORTFOLIO:
			case MODE_INSERT_PORTFOLIO:
				AccordianID = ACCORDIAN_PORTFOLIO;
				break;

			case MODE_REPORT_EQUITY_FUNDAMENTALS:
				AccordianID = ACCORDIAN_EQUITY_FUNDAMENTAL_REPORT;
				break;

			case MODE_REPORT_ETF_FUNDAMENTALS:
				AccordianID = ACCORDIAN_ETF_FUNDAMENTAL_REPORT;
				break;

			case MODE_REPORT_PORTFOLIO:
				AccordianID = ACCORDIAN_PORTFOLIO_REPORT;
				break;

			case MODE_REPORT_WATCHLIST:
				AccordianID = ACCORDIAN_WATCHLIST_REPORT;
				break;

			case MODE_REPORT_SEC_IND_ALLOCATION:
				AccordianID = ACCORDIAN_SEC_IND_ALLOCATION;
				break;

			case MODE_REPORT_ASSET_ALLOCATION:
				AccordianID = ACCORDIAN_ASSET_ALLOCATION;
				break;

			case MODE_CLEAR_STOCK:
			case MODE_FIND_STOCK:
			case MODE_INSERT_STOCK:
				AccordianID = ACCORDIAN_STOCK;
				break;

			case MODE_ORDER_ENTRY_BUY:
			case MODE_ORDER_ENTRY_SELL:
			case MODE_ORDER_ENTRY_CLEAR:
			case MODE_ORDER_ENTRY_PAINT:
			case MODE_CONFIRM_BUY:
			case MODE_CONFIRM_SELL:
			case MODE_ORDER_ENTRY_DRIP:
			case MODE_CONFIRM_DRIP:
				AccordianID = ACCORDIAN_ORDERS;
				break;

			case MODE_CASH_ACCOUNT:
			case MODE_CASH_ACCOUNT_SAVE:
			case MODE_CASH_ACCOUNT_CONFIRM:
			case MODE_CASH_ACCOUNT_CLEAR:
				AccordianID = ACCORDIAN_CASHBAL;
				break;

		}
	}

printf ( "<div class='container'>\n" );
printf ( "  <div class='panel-group' id='accordion'>\n" );

	/*----------------------------------------------------------
		Stock maint screen
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "SECURITIES", AccordianID == ACCORDIAN_STOCK ? 1 : 0 );

	PaintStockMaint ();

	PaintEndReportStory ();

	/*----------------------------------------------------------
		watchlist
		ticker company
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "WATCHLIST", AccordianID == ACCORDIAN_WATCHLIST ? 1 : 0 );

	sprintf ( WhereClause, "Wmember = %ld", CookieMember.xid );
	WatchlistRecordCount = LoadWatchlistCB ( &MySql, WhereClause, "Wticker", &xwatchlist, (int(*)()) PaintStockW, 0 );

	/*----------------------------------------------------------
		add stock to watchlist
	----------------------------------------------------------*/
	if  ( WatchlistRecordCount < 1 )
	{
		PaintWatchlistHeader ( (int) WatchlistRecordCount );
	}
	else if ( xmember.xmrole[0] == ROLE_FREE && WatchlistRecordCount >= FREE_MEMBER_WATCHLIST_LIMIT )
	{
		CanAddToWatchlist = 0;
	}
	else if ( WatchlistRecordCount >= PAID_MEMBER_WATCHLIST_LIMIT )
	{
		CanAddToWatchlist = 0;
	}

	printf ( "<tr>\n" );
	if ( CanAddToWatchlist )
	{
		printf ( "<tr>\n" );
		printf ( "<td>&nbsp;</td>\n" );
		printf ( "<td>" );

		/*----------------------------
			add stock
		----------------------------*/
		printf ( "<input type='button' value='Add stock to watchlist'" );
		printf ( " onclick='javascript:window.open(\"invest.cgi?MenuIndex=%d&WatchlistID=0&RunMode=%d\",\"_self\");'>",
						MenuIndex,  MODE_PAINT_EDIT_WATCHLIST );

		/*----------------------------
			import watchlist
		----------------------------*/
		printf ( "&emsp;<input type='button' value='Import watchlist'" );
		printf ( " onclick='javascript:window.open(\"invest.cgi?MenuIndex=%d&WatchlistID=0&RunMode=%d\",\"popup\",\"height=600,width=750,top=50,left=50,scrollbars=yes\");'>",
						MenuIndex,  MODE_PAINT_IMPORT_WATCHLIST );

		if ( WatchlistRecordCount > 0 /* && CookieMember.xid != DEMO_MEMBER */ )
		{
			printf ( "&emsp;<input type='submit' name='submit_%d' value='Remove all stocks' %s>\n", MODE_REMOVE_ALL_WATCHLIST, SET_SELF );
			printf ( "&emsp;<input type='submit' name='submit_%d' value='Remove stocks without alerts' %s>\n", MODE_REMOVE_NO_ALERT_WATCHLIST, SET_SELF );
		}

		printf ( "</td>" );
		printf ( "</tr>\n" );
	}
	else
	{
		printf ( "<td>&nbsp;</td>\n" );
		printf ( "<td>" );
		if ( WatchlistRecordCount > 0 )
		{
			printf ( "<input type='submit' name='submit_%d' value='Remove all stocks' %s>&emsp;\n", MODE_REMOVE_ALL_WATCHLIST, SET_SELF );
			printf ( "&emsp;<input type='submit' name='submit_%d' value='Remove stocks without alerts' %s>\n", MODE_REMOVE_NO_ALERT_WATCHLIST, SET_SELF );
		}
		printf ( "%s to %d securities in watchlist", 
			xmember.xmrole[0] == ROLE_FREE ? "Free members limited" : "Limited",
			xmember.xmrole[0] == ROLE_FREE ? FREE_MEMBER_WATCHLIST_LIMIT : PAID_MEMBER_WATCHLIST_LIMIT );
		printf ( "</td>" );
	}
	printf ( "</tr>\n" );
	PaintEndReportStory ();

	/*----------------------------------------------------------
		watchlist report
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "WATCHLIST REPORT", AccordianID == ACCORDIAN_WATCHLIST_REPORT ? 1 : 0 );

	if ( CookieMember.xid > 0 && WatchlistRecordCount == 0 )
	{
		printf ( "There are no records in the watchlist<br>\n" );
	}
	else if ( IsDisabled == 1 )
	{
		PrintUpgradeMessage ( 2 );
		printf ( "</form>\n" );
	}
	else
	{
		printf ("<tr>\n" );
		printf ("<td class='report-table'>\n" );
		printf ("Report Style\n" );
		printf ("</td>\n" );
		printf ("<td class='report-table' colspan='3'>\n" );
		printf ("<select name='ReportStyle_%d'>\n", MODE_REPORT_WATCHLIST );
		printf ("<option value='%c'%s>Basic</option>\n",  STYLE_BASE, ReportStyle[ndx] == STYLE_BASE ? " selected" : "" );
		printf ("<option value='%c'%s>Fundamental</option>\n",  STYLE_FUNDAMENTAL, ReportStyle[ndx] == STYLE_FUNDAMENTAL ? " selected" : "" );
		printf ("</select>\n" );
		printf ("</td>\n" );
		printf ("</tr>\n" );

		PaintFormat ( MODE_REPORT_WATCHLIST, 0 );

		PaintSubmit ( MODE_REPORT_WATCHLIST, "Run Report", 2 );
	}

	PaintEndReportStory ();

	/*----------------------------------------------------------
		portfolio
		ticker buydate shares cost broker taxable benchmark alert optexp 
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "PORTFOLIO", AccordianID == ACCORDIAN_PORTFOLIO ? 1 : 0 );

	sprintf ( WhereClause, "Pmember = %ld", CookieMember.xid );
	// PortfolioRecordCount = LoadPortfolioCB ( &MySql, WhereClause, "Pticker", &xportfolio, (int(*)()) PaintStockP, 0 );
	PortfolioRecordCount = LoadPortfolioCB ( &MySql, WhereClause, "Pbroker,Pticker", &xportfolio, (int(*)()) PaintStockP, 0 );

	/*----------------------------------------------------------
		add stock to portfolio
	----------------------------------------------------------*/
	if ( PortfolioRecordCount < 1 )
	{
		PaintPortfolioHeader ( (int) PortfolioRecordCount );
	}
	else if ( xmember.xmrole[0] == ROLE_FREE && PortfolioRecordCount >= FREE_MEMBER_PORTFOLIO_LIMIT )
	{
		CanAddToPortfolio = 0;
	}
	else if ( PortfolioRecordCount >= PAID_MEMBER_PORTFOLIO_LIMIT )
	{
		CanAddToPortfolio = 0;
	}

	printf ( "<tr>\n" );

	if ( CanAddToPortfolio )
	{
		printf ( "<td>&nbsp;</td>\n" );
		printf ( "<td colspan='4'>" );
		printf ( "<input type='button' value='Add stock to portfolio'" );
		printf ( " onclick='javascript:window.open(\"invest.cgi?MenuIndex=%d&PortfolioID=0&RunMode=%d\",\"_self\");'>",
						MenuIndex,  MODE_PAINT_EDIT_PORTFOLIO );

		/*----------------------------
			import portfolio
		----------------------------*/
		printf ( "&emsp;<input type='button' value='Import portfolio'" );
		printf ( " onclick='javascript:window.open(\"invest.cgi?MenuIndex=%d&PortfolioID=0&RunMode=%d\",\"popup\",\"height=600,width=750,top=50,left=50,scrollbars=yes\");'>",
						MenuIndex,  MODE_PAINT_IMPORT_PORTFOLIO );
	}
	else
	{
		printf ( "<td colspan='5'>%s to %d securities in portfolio", 
			xmember.xmrole[0] == ROLE_FREE ? "Free members limited" : "Limited",
			xmember.xmrole[0] == ROLE_FREE ? FREE_MEMBER_PORTFOLIO_LIMIT : PAID_MEMBER_PORTFOLIO_LIMIT );
	}

	/*----------------------------------------------------------
		Chart button - to chart the entire portfolio
	----------------------------------------------------------*/
	printf ( "&emsp;" );
	printf ( "<input type='submit' name='SubmitCombinedPortfolioChart' value='Chart' onclick='javascript:SetNamed(\"sheet\");'>\n" );

	printf ( "</td></tr>\n" );

	PaintEndReportStory ();

	/*----------------------------------------------------------
		portfolio report
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "PORTFOLIO REPORT", AccordianID == ACCORDIAN_PORTFOLIO_REPORT ? 1 : 0 );

	if ( CookieMember.xid > 0 && PortfolioRecordCount == 0 )
	{
		printf ( "There are no records in the portfolio<br>\n" );
	}
	else if ( IsDisabled == 1 )
	{
		printf ( "You have run %ld reports.<br>\n", RptlogRecordCount );
		printf ( "<b>Upgrade to full access for introductory price of $%d/year.</b><br>\n", INTRODUCTORY_PRICE );
		printf ( "</form>\n" );
	}
	else
	{
		printf ("<tr>\n" );
		printf ("<td class='report-table'>\n" );
		printf ("Report Style\n" );
		printf ("</td>\n" );
		printf ("<td class='report-table' colspan='3'>\n" );
		printf ("<select name='ReportStyle_%d'>\n", MODE_REPORT_PORTFOLIO );
		printf ("<option value='%c'%s>Basic</option>\n",  STYLE_BASE, ReportStyle[ndx] == STYLE_BASE ? " selected" : "" );
		printf ("<option value='%c'%s>Full</option>\n",   STYLE_FULL,  ReportStyle[ndx] == STYLE_FULL  ? " selected" : "" );
		printf ("<option value='%c'%s>Alerts</option>\n", STYLE_ALERTS, ReportStyle[ndx] == STYLE_ALERTS ? " selected" : "" );
		printf ("<option value='%c'%s>Today</option>\n",  STYLE_TODAY, ReportStyle[ndx] == STYLE_TODAY ? " selected" : "" );
		printf ("<option value='%c'%s>Overnight</option>\n", STYLE_OVERNIGHT,  ReportStyle[ndx] == STYLE_OVERNIGHT  ? " selected" : "" );
		// printf ("<option value='%c'%s>Year Over Year</option>\n", STYLE_YOY,  ReportStyle[ndx] == STYLE_YOY  ? " selected" : "" );
		printf ("<option value='%c'%s>Performance</option>\n", STYLE_PERFORMANCE,  ReportStyle[ndx] == STYLE_PERFORMANCE  ? " selected" : "" );
		printf ("<option value='%c'%s>Fundamental</option>\n",  STYLE_FUNDAMENTAL, ReportStyle[ndx] == STYLE_FUNDAMENTAL ? " selected" : "" );
		printf ("<option value='%c'%s>Buckets</option>\n", STYLE_BUCKETS,  ReportStyle[ndx] == STYLE_BUCKETS  ? " selected" : "" );
		printf ("<option value='%c'%s>MorningStar</option>\n", STYLE_MSTAR,  ReportStyle[ndx] == STYLE_MSTAR  ? " selected" : "" );
		printf ("<option value='%c'%s>YOY</option>\n", STYLE_YOY,  ReportStyle[ndx] == STYLE_YOY  ? " selected" : "" );
//		printf ("<option value='%c'%s>Custom</option>\n", STYLE_CUSTOM,  ReportStyle[ndx] == STYLE_CUSTOM  ? " selected" : "" );
		printf ("</select>\n" );
		printf ("</td>\n" );
		printf ("</tr>\n" );
		
		PaintSelectStype ( MODE_REPORT_PORTFOLIO );

		/*----------------------------------------------------------
			tms	07/17/2023	added broker/account
		----------------------------------------------------------*/
		printf ("<tr>\n" );
		printf ("<td class='report-table'>\n" );
		printf ("Broker/Account\n" );
		printf ("</td>\n" );
		printf ("<td class='report-table' colspan='3'>\n" );
		printf ( "<input type='search' name='Broker_%d'>\n", MODE_REPORT_PORTFOLIO );
		printf ("</td>\n" );
		printf ("</tr>\n" );

		OptionArray[0] = SORTBY_TICKER;
		OptionArray[1] = SORTBY_GAIN;
		OptionArray[2] = SORTBY_RETURN;
		OptionArray[3] = SORTBY_COMPARED;
		PaintSortBy ( MODE_REPORT_PORTFOLIO, OptionArray, 4 );

		PaintFormat ( MODE_REPORT_PORTFOLIO, 1 );
		PaintSubmit ( MODE_REPORT_PORTFOLIO, "Run Report", 2 );
	}

	PaintEndReportStory ();

	/*----------------------------------------------------------
		sector/industry allocation report
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "SECTOR/INDUSTRY ALLOCATION REPORT", AccordianID == ACCORDIAN_SEC_IND_ALLOCATION ? 1 : 0 );

	if ( CookieMember.xid > 0 && PortfolioRecordCount == 0 )
	{
		printf ( "There are no records in the portfolio<br>\n" );
	}
	else if ( IsDisabled == 1 )
	{
		printf ( "You have run %ld reports.<br>\n", RptlogRecordCount );
		printf ( "<b>Upgrade to full access for introductory price of $%d/year.</b><br>\n", INTRODUCTORY_PRICE );
		printf ( "</form>\n" );
	}
	else
	{
		printf ("<tr>\n" );
		printf ("<td class='report-table'>\n" );
		printf ("Report Style\n" );
		printf ("</td>\n" );

		/*---------------------------------------------------------------------------
			cheat: steal STYLE_ALERTS and STYLE_FULL for sector and industry
		---------------------------------------------------------------------------*/
		printf ("<td class='report-table' colspan='3'>\n" );
		printf ("<select name='ReportStyle_%d'>\n", MODE_REPORT_SEC_IND_ALLOCATION );
		printf ("<option value='%c'%s>Sector</option>\n",              STYLE_BASE,   ReportStyle[ndx] == STYLE_BASE ? " selected" : "" );
		printf ("<option value='%c'%s>Industry</option>\n",            STYLE_ALERTS, ReportStyle[ndx] == STYLE_ALERTS  ? " selected" : "" );
		printf ("<option value='%c'%s>Sector and Industry</option>\n", STYLE_FULL,   ReportStyle[ndx] == STYLE_FULL ? " selected" : "" );
		printf ("</select>\n" );
		printf ("</td>\n" );
		printf ("</tr>\n" );

		PaintFormat ( MODE_REPORT_SEC_IND_ALLOCATION, 0 );

		PaintSubmit ( MODE_REPORT_SEC_IND_ALLOCATION, "Run Report", 2 );
	}

	PaintEndReportStory ();

	/*----------------------------------------------------------
		asset type allocation report
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "ASSET TYPE ALLOCATION REPORT", AccordianID == ACCORDIAN_ASSET_ALLOCATION ? 1 : 0 );

	if ( CookieMember.xid > 0 && PortfolioRecordCount == 0 )
	{
		printf ( "There are no records in the portfolio<br>\n" );
	}
	else if ( IsDisabled == 1 )
	{
		printf ( "You have run %ld reports.<br>\n", RptlogRecordCount );
		printf ( "<b>Upgrade to full access for introductory price of $%d/year.</b><br>\n", INTRODUCTORY_PRICE );
		printf ( "</form>\n" );
	}
	else
	{
		PaintFormat ( MODE_REPORT_ASSET_ALLOCATION, 0 );

		PaintSubmit ( MODE_REPORT_ASSET_ALLOCATION, "Run Report", 2 );
	}

	PaintEndReportStory ();

	/*----------------------------------------------------------
		Cash Balances screen
	----------------------------------------------------------*/
	PaintStartReportStory ( STORY_TITLE, "CASH BALANCES", AccordianID == ACCORDIAN_CASHBAL ? 1 : 0 );

	TotalCashBalance = 0.0;
	sprintf ( WhereClause, "CBmember = %ld", CookieMember.xid );
	CashbalRecordCount = LoadCashbalCB ( &MySql, WhereClause, "CBacct", &xcashbal, (int(*)()) PaintCashbal, 0 );

	printf ( "<tr>\n" );

	printf ( "<td colspan='2'>Total</td>\n" );

	printf ( "<td>%.2f</td>\n", TotalCashBalance );

	printf ( "<td>&nbsp;</td>\n" );

	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		add account to cashbal
	----------------------------------------------------------*/

	printf ( "<tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>&nbsp;</td>\n" );
	printf ( "<td colspan='4'>" );
	printf ( "<input type='button' value='Add account'" );
	printf ( " onclick='javascript:window.open(\"invest.cgi?MenuIndex=%d&CashbalID=0&RunMode=%d\",\"_self\");'>",
					MenuIndex,  MODE_PAINT_EDIT_CASHBAL );
	printf ( "</td>" );
	printf ( "</tr>\n" );

	printf ( "</tr>\n" );
	PaintEndReportStory ();

	printf ( "  </div>\n" );
	printf ( "</div>\n" );
}

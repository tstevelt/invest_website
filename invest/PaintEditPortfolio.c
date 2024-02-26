/*----------------------------------------------------------------------------
	Program : invest/PaintEditPortfolio.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Edit a portfolio entry.
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

static char AlertTypeArray [] =
{
	ALERT_NONE, ALERT_ABOVE_AMT, ALERT_BELOW_AMT, ALERT_TRAIL_STOP, ALERT_BENCHMARK,
	ALERT_ABOVE_MA5, ALERT_ABOVE_MA10, ALERT_ABOVE_MA20, ALERT_ABOVE_MA50, ALERT_ABOVE_MA100, ALERT_ABOVE_MA200,
	ALERT_BELOW_MA5, ALERT_BELOW_MA10, ALERT_BELOW_MA20, ALERT_BELOW_MA50, ALERT_BELOW_MA100, ALERT_BELOW_MA200,
	ALERT_MA5_DECREASE, ALERT_MA10_DECREASE, ALERT_MA20_DECREASE, ALERT_MA50_DECREASE, 
	ALERT_MA5_INCREASE, ALERT_MA10_INCREASE, ALERT_MA20_INCREASE, ALERT_MA50_INCREASE,
	'\0'
};

static void HorizontalRule ( int State )
{
	return;
	switch ( State )
	{
		case 1:
			printf ( "<table>\n" );
			printf ( "<tr><td>\n" );
			break;
		case 2:
			printf ( "</td></tr>\n" );
			printf ( "</table>\n" );
			printf ( "<table>\n" );
			printf ( "<tr><td>\n" );
			break;
		case 3:
			printf ( "</td></tr>\n" );
			printf ( "</table>\n" );
			break;
	}
}

void PaintEditPortfolio ()
{
	double	Percent;
	int		xa;

	printf ( "<div class='story'>\n" );
	printf ( "<input type='hidden' name='PortfolioID' value='%ld'>\n", PortfolioID );
	printf ( "<table class='table-condensed table-borderless'>\n" );

	if ( PortfolioID > 0 )
	{
		sprintf ( WhereClause, "portfolio.id = %ld", PortfolioID );
		LoadPortfolio ( &MySql, WhereClause, &xportfolio, 0 );

		sprintf ( WhereClause, "Sticker = '%s'", xportfolio.xpticker );
		LoadStock ( &MySql, WhereClause, &xstock, 0, 0 );
		
		sprintf ( WhereClause, "Hticker = '%s' and Hdate = '%s'", xstock.xsticker, xstock.xslast );
		LoadHistory ( &MySql, WhereClause, &xhistory, 0 );
		
		printf ( "<%s>Edit Portfolio</%s>\n", STORY_TITLE, STORY_TITLE );
	}
	else
	{
		printf ( "<%s>Add Stock to Portfolio</%s>\n", STORY_TITLE, STORY_TITLE );
	}

	/*----------------------------
		Pticker    | varchar(20)
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Ticker</td>" );
	printf ( "<td>" );
	printf ( "<input type='search' id='Pticker' name='Pticker'" );
	if ( PortfolioID > 0 )
	{
		printf ( " value='%s'", xportfolio.xpticker );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	if ( PortfolioID > 0 )
	{
		/*----------------------------
			company name
		----------------------------*/
		printf ( "<tr>\n" );
		printf ( "<td>Company</td>\n" );
		printf ( "<td>%s</td>\n", xstock.xsname );
		printf ( "</tr>\n" );
	}

	/*----------------------------
		Pdate      | date
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Purchase Date</td>" );
	printf ( "<td>" );
	printf ( "<input type='search' name='Pdate'" );
	if (( PortfolioID > 0                                 ) &&
		( nsStrncmp ( xportfolio.xpdate, "(null)", 6 ) != 0 ) &&
		( nsStrncmp ( xportfolio.xpdate, "0000-0", 6 ) != 0 ))
	{
		printf ( " value='%s'", xportfolio.xpdate );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		Pshares    | double
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Shares</td>" );
	printf ( "<td>" );
	printf ( "<input type='search' name='Pshares'" );
	if ( PortfolioID > 0 )
	{
		printf ( " value='%.2f'", xportfolio.xpshares );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		Pprice     | double
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Cost</td>" );
	printf ( "<td>" );
	printf ( "<input type='search' name='Pprice'" );
	if ( PortfolioID > 0 )
	{
		printf ( " value='%.2f'", xportfolio.xpprice );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		row - yesterday close
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Yesterday Close</td>\n" );
	if ( PortfolioID > 0 )
	{
		Percent = 100.0 * ( xhistory.xhclose - xportfolio.xpprice ) / xportfolio.xpprice;
		printf ( "<td>%.2f&emsp;(%.2f)</td>\n", xhistory.xhclose, Percent );
	}
	printf ( "</tr>\n" );

	/*----------------------------
		row - total value
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Current Value</td>\n" );
	if ( PortfolioID > 0 )
	{
		printf ( "<td>%.2f</td>\n", xhistory.xhclose * xportfolio.xpshares );
	}
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		alert type and alert price - tms 05/24/2019
		disable unused fields based on alert type
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Alert Type</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='Palerttype' id='Palerttype' onChange='javascript:AlertTypeToggle(this);'>" );
	for ( xa = 0; AlertTypeArray[xa]; xa++ )
	{
		printf ( "<option value='%c'%s>%s</option>\n",
			AlertTypeArray[xa], 
			xportfolio.xpalerttype[0] == AlertTypeArray[xa] ? " selected" : "", 
			AlertDescription ( AlertTypeArray[xa] ) );
	}
	printf ( "</select>" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );
	
	/*---------------------------------------------------------------------------
		section - above or below a specific price
	---------------------------------------------------------------------------*/
	HorizontalRule ( 1 );

	printf ( "<tr class='price_alert'>\n" );
	printf ( "<td>Alert Price</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='Palertprice' id='Palertprice'" );
	if ( PortfolioID > 0 )
	{
		printf ( " value='%.2f'", xportfolio.xpalertprice );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	/*---------------------------------------------------------------------------
		section - below trailing stop
	---------------------------------------------------------------------------*/
	HorizontalRule ( 2 );

	/*----------------------------
		row - trailing percent
	----------------------------*/
	printf ( "<tr class='trailing_alert'>\n" );
	printf ( "<td>Trailing Stop Percent</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='Ptrailpct' id='Ptrailpct'" );
	if ( PortfolioID > 0 )
	{
		printf ( " value='%.2f'", xportfolio.xptrailpct );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		row - trailing price
		s = c * (1 - p)
		s = c - cp
		cp = c - s
		p = (c - s) / c

	----------------------------*/
	printf ( "<tr class='trailing_alert'>\n" );
	printf ( "<td>Trailing Stop Price</td>\n" );
	printf ( "<td>\n" );
	if ( PortfolioID > 0 )
	{
		printf ( "%.2f", xportfolio.xptrailvalue );
		if ( xportfolio.xptrailpct > 0.01 )
		{
			Percent = 100.0 * ( xhistory.xhclose - xportfolio.xptrailvalue ) / xhistory.xhclose;
			printf ( "&emsp;(%.2f)", Percent );
		}
	}
	else
	{
		printf ( "&nbsp;" );
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*---------------------------------------------------------------------------
		section - below benchmark performance
	---------------------------------------------------------------------------*/
	HorizontalRule ( 2 );

	/*----------------------------
		Pbenchmark | varchar(20)
	----------------------------*/
	printf ( "<tr class='benchmark_alert'>\n" );
	printf ( "<td>Benchmark</td>" );
	printf ( "<td>\n" );
	printf ( "<select name='Pbenchmark' id='Pbenchmark'>\n" );
	printf ( "<option value='SPY'%s>S&P 500</option>\n",      PortfolioID > 0 && nsStrncmp(xportfolio.xpbenchmark,"SPY",3) == 0 ? " selected" : "" );
	printf ( "<option value='DIA'%s>Dow Jones</option>\n",    PortfolioID > 0 && nsStrncmp(xportfolio.xpbenchmark,"DIA",3) == 0 ? " selected" : "" );
	printf ( "<option value='QQQ'%s>Nasdaq 100</option>\n",   PortfolioID > 0 && nsStrncmp(xportfolio.xpbenchmark,"QQQ",3) == 0 ? " selected" : "" );
	printf ( "<option value='IWM'%s>Russell 2000</option>\n", PortfolioID > 0 && nsStrncmp(xportfolio.xpbenchmark,"IWM",3) == 0 ? " selected" : "" );
	printf ( "<option value='EFA'%s>MSCI EAFE</option>\n",    PortfolioID > 0 && nsStrncmp(xportfolio.xpbenchmark,"EFA",3) == 0 ? " selected" : "" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		Palert     | double 
	----------------------------*/
	printf ( "<tr class='benchmark_alert'>\n" );
	printf ( "<td>Benchmark Percent</td>" );
	printf ( "<td>" );
	printf ( "<input type='search' name='Palert' id='Palert'" );
	if ( PortfolioID > 0 )
	{
		printf ( " value='%.2f'", xportfolio.xpalert );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	/*---------------------------------------------------------------------------
		end of sections
	---------------------------------------------------------------------------*/
	HorizontalRule ( 3 );


	/*----------------------------
		Poptexp    | date 
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Option Expiration</td>" );
	printf ( "<td>" );
	printf ( "<input type='search' name='Poptexp'" );
	if (( PortfolioID > 0                                 ) &&
		( nsStrncmp ( xportfolio.xpoptexp, "(null)", 6 ) != 0 ) &&
		( nsStrncmp ( xportfolio.xpoptexp, "0000-0", 6 ) != 0 ))
	{
		printf ( " value='%s'", xportfolio.xpoptexp );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		Pbroker    | char(10)
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Broker</td>" );
	printf ( "<td>" );
	printf ( "<input type='search' name='Pbroker'" );
	if ( PortfolioID > 0 )
	{
		printf ( " value='%s'", xportfolio.xpbroker );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		Ptaxshel   | tinyint(1)
	printf ( "<tr>\n" );
	printf ( "<td>Tax Sheltered</td>" );
	printf ( "<td>\n" );
	printf ( "<select name='Ptaxshel'>\n" );
	printf ( "<option value='0'%s>No</option>\n",  PortfolioID > 0 && xportfolio.xptaxshel == '0' ? " selected" : "" );
	printf ( "<option value='1'%s>Yes</option>\n", PortfolioID > 0 && xportfolio.xptaxshel == '1' ? " selected" : "" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );
	----------------------------*/

	/*----------------------------
		Pbucket   | int(4)
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Bucket</td>" );
	printf ( "<td>\n" );
	printf ( "<select name='Pbucket'>\n" );
	printf ( "<option value='0'%s>--select--</option>\n",  xportfolio.xpbucket < 1 || xportfolio.xpbucket > 3 ? " selected" : "" );
	printf ( "<option value='1'%s>Cash Bucket</option>\n", xportfolio.xpbucket == 1 ? " selected" : "" );
	printf ( "<option value='2'%s>Fixed Bucket</option>\n", xportfolio.xpbucket == 2 ? " selected" : "" );
	printf ( "<option value='3'%s>Equity Bucket</option>\n", xportfolio.xpbucket == 3 ? " selected" : "" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		row submit
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>&nbsp;</td>\n" );
	if ( PortfolioID > 0 )
	{
		printf ( "<td>\n" );
		printf ( "<input type='submit' name='SubmitUpdatePortfolio' value='Save' %s>\n", SET_SELF );

		printf ( "&emsp;" );
		printf ( "<input type='submit' name='SubmitDeletePortfolio' value='Remove' %s>\n", SET_SELF );

		printf ( "&emsp;" );
		printf ( "<input type='submit' name='SubmitPrintSheet' value='Print' onclick='javascript:SetNamed(\"sheet\");'>\n" );

		printf ( "&emsp;" );
		printf ( "<input type='submit' name='SubmitPortfolioChart' value='Chart' onclick='javascript:SetNamed(\"sheet\");'>\n" );

		printf ( "&emsp;" );
		printf ( "<input type='submit' name='SubmitPortfolioDividends' value='Dividends' onclick='javascript:SetNamed(\"sheet\");'>\n" );

		printf ( "&emsp;<input type='button' id='SubmitStockFundFmt' value='Fundamentals'" );
		printf ( " onclick='javascript:ShowFundamentals(\"Pticker\",\"\");'>\n" );

#ifdef OTHER_CHARTS
		printf ( "&emsp;" );
		printf ( "<input type='submit' name='SubmitChart' value='TV Chart' onclick='javascript:SetNamed(\"sheet\");'>\n" );
#endif

		printf ( "&emsp;" );
		printf ( "<input type='button' value='BarChart' " );
		printf ( "onclick='javascript:window.open(\"https://www.barchart.com/stocks/quotes/%s/interactive-chart\",\"screen\");'",
					xportfolio.xpticker );
		printf ( ">\n" );

		printf ( "</td>\n" );
	}
	else
	{
		printf ( "<td>\n" );
		if ( CookieMember.xid == DEMO_MEMBER )
		{
			printf ( "<input type='button' value='Add Disabled'>\n" );
		}
		else
		{
			printf ( "<input type='submit' name='SubmitInsertPortfolio' value='Add'>\n" );
		}
		printf ( "</td>\n" );
	}
	printf ( "</tr>\n" );

	printf ( "</table>\n" );
	printf ( "</div>\n" );

//	printf ( "<script type='test/javascript'>\n" );
//	printf ( "AlertTypeToggle(Palerttype);\n" );
//	printf ( "window.onload = function () { alert('It is loaded!') }\n" );
//	printf ( "</script>\n" );


printf ( "<script>\n" );
printf ( "$( document ).ready(function() {\n" );
printf ( "/* console.log( 'document loaded' ); */\n" );
printf ( "AlertTypeToggle(Palerttype);\n" );
printf ( "});\n" );
printf ( "</script>\n" );

}

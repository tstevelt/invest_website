/*----------------------------------------------------------------------------
	Program : invest/PaintCombinedChart.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint portfolio performance bar chart.
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
/*----------------------------------------------------------
	https://www.w3schools.com/tags/ref_canvas.asp
----------------------------------------------------------*/
/*----------------------------------------------------------
	found this on the XLNX investor relations page:
	https://api.nasdaqomx.wallst.com/advancedchart?display=line&symbol=XLNX.O&scale=linear&duration=1yr&frequency=1dy&gridLine=b&bgColor=ffffff&lineColor=ee3324&fillcolor=ee3c22|f05122&width=750&height=635
----------------------------------------------------------*/

#include	"invest.h"

#define	ONE_YEAR		252
#define SIX_MONTH		126
#define THREE_MONTH		63

#define	ROUND_UP		'U'
#define	ROUND_DOWN		'D'

static double AutoScale ( double InputValue, int Direction )
{
	long long		WorkValue;
	double	rv;

	/*----------------------------------------------------------
		what errors?
	----------------------------------------------------------*/
	if ( InputValue < 1.0 )
	{
		return ( InputValue );
	}

	/*----------------------------------------------------------
		9.51   ==>  951
		16.19  ==>  1619
		123.45 ==>  12345
	----------------------------------------------------------*/
	WorkValue = InputValue * 100.0;

	/*----------------------------------------------------------
		951   ==>   9
		1619  ==>  16
		12345 ==> 123
	----------------------------------------------------------*/
	WorkValue = WorkValue / 100;

	if ( WorkValue < 10 )
	{
// printf ( "/* Input %.2f  Work %d  Direction %s */\n", InputValue, (int) WorkValue, Direction == ROUND_DOWN ? "down" : "up" );
		/*----------------------------------------------------------
			9  ==> 8 or 10
		----------------------------------------------------------*/
		if ( Direction == ROUND_DOWN )
		{
			WorkValue -= 1;
		}
		else if ( Direction == ROUND_UP )
		{
			WorkValue += 1;
		}
	}
	else if ( WorkValue < 30 )
	{
// printf ( "/* Input %.2f  Work %d  Direction %s */\n", InputValue, (int) WorkValue, Direction == ROUND_DOWN ? "down" : "up" );
		/*----------------------------------------------------------
			16 ==>  14  or 19
		----------------------------------------------------------*/
		if ( Direction == ROUND_DOWN )
		{
			WorkValue -= 2;
		}
		else if ( Direction == ROUND_UP )
		{
			WorkValue += 2;
		}
	}
	else if ( WorkValue < 100 )
	{
		/*----------------------------------------------------------
			16 ==> 3
			3  ==> 15
		----------------------------------------------------------*/
		WorkValue = WorkValue / 5;
		WorkValue = WorkValue * 5;

		if ( Direction == ROUND_UP )
		{
			WorkValue =  WorkValue + 5;
		}
	}
	else
	{
		/*----------------------------------------------------------
			123 ==> 12
			12  ==> 120
		----------------------------------------------------------*/
		WorkValue = WorkValue / 10;
		WorkValue = WorkValue * 10;

		if ( Direction == ROUND_UP )
		{
			WorkValue =  WorkValue + 10;
		}
	}

	rv = (double) WorkValue;

	return ( rv );
}

void PaintCombinedChart ()
{
	int		ndx;
	double	StockMinLow, StockMaxHigh;
	double	Y_Array[10];
	DBY_QUERY	*Query;
	char	StartDate[12];
	DATEVAL	dvStart;

	sprintf ( WhereClause, "member.id = %ld", CookieMember.xid );
	LoadMember ( &MySql, WhereClause, &CookieMember, 0 );

	switch ( CookieMember.xmchrtnum )
	{
		case 6:
			DateAdd ( &dvToday, -182, &dvStart );
			break;
		case 12:
			DateAdd ( &dvToday, -365, &dvStart );
			break;
		case 18:
			DateAdd ( &dvToday, -547, &dvStart );
			break;
		case 24:
		default:
			DateAdd ( &dvToday, -730, &dvStart );
			break;
	}
	
	sprintf ( StartDate, "%04d-%02d-%02d", dvStart.year4, dvStart.month, dvStart.day );
	
//	printf ( "%ld %s<br>\n", CookieMember.xmchrtnum, StartDate );

	sprintf ( Statement, 
		"select PHdate, PHinvest, PHequity, PHbonds, PHcash from porthist where PHmember = %ld and PHdate >= '%s' order by PHdate",
		CookieMember.xid, StartDate );

	// printf ( "%s<br>\n", Statement );

	HistoryCount = 0;
	Query = dbySelect ( "invest", &MySql, Statement, LogFileName );
	while (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
	{

		sprintf ( HistoryArray[HistoryCount].Date, "%s", Query->EachRow[0] );
		HistoryArray[HistoryCount].Open  = nsAtof ( Query->EachRow[1] );
		HistoryArray[HistoryCount].High  = nsAtof ( Query->EachRow[2] );
		HistoryArray[HistoryCount].Low   = nsAtof ( Query->EachRow[3] );
		HistoryArray[HistoryCount].Close = nsAtof ( Query->EachRow[4] );
		HistoryCount++;
	}

	printf ( "<div class='chart'>\n" );
	printf ( "<h3>Portfolio Chart</h3>\n" );

	printf ( "<canvas id='stockCanvas' width='%d' height='%d' style='border:1px solid #000000;'>\n", STOCK_CHART_WIDTH, STOCK_CHART_HEIGHT );
	printf ( "Your browser does not support the HTML5 canvas tag.\n" );
	printf ( "</canvas>\n" );
	printf ( "</div>\n" );

	StockMinLow  = HistoryArray[0].High + HistoryArray[0].Low + HistoryArray[0].Close;
	StockMaxHigh = HistoryArray[0].High + HistoryArray[0].Low + HistoryArray[0].Close;

	double	Value;

	printf ( "<script>\n" );
	printf ( "var History = [\n" );
	for ( ndx = 0; ndx < HistoryCount; ndx++ )
	{
		printf ( "{Date:'%2.2s/%2.2s/%2.2s',Equity:%.2f,Bonds:%.2f,Cash:%.2f}%s\n",
			&HistoryArray[ndx].Date[5], &HistoryArray[ndx].Date[8], &HistoryArray[ndx].Date[2],
			HistoryArray[ndx].High,
			HistoryArray[ndx].Low,
			HistoryArray[ndx].Close,
			ndx + 1 < HistoryCount ? "," : "" );

		Value = HistoryArray[ndx].High + HistoryArray[ndx].Low + HistoryArray[ndx].Close;
		if ( StockMinLow  >  Value )
		{
			StockMinLow  = Value;
		}

		if ( StockMaxHigh <  Value )
		{
			StockMaxHigh = Value;
		}
	}

	printf ( "];\n" );
	// StockMinLow = AutoScale(StockMinLow,ROUND_DOWN);
	StockMinLow = 0.0;
	StockMaxHigh = AutoScale(StockMaxHigh,ROUND_UP);
	printf ( "var StockMinLow = %.2f;\n", StockMinLow );
	printf ( "var StockMaxHigh = %.2f;\n", StockMaxHigh );
	printf ( "var StockChartWidth = %d;\n", STOCK_CHART_WIDTH );
	printf ( "var StockChartHeight = %d;\n", STOCK_CHART_HEIGHT );

	for ( ndx = 0; ndx < 10; ndx++ )
	{
		Y_Array[ndx] = StockMinLow + ndx * (StockMaxHigh - StockMinLow) / 10.0;
	}

	printf ( "var Yaxis = [ " );
	for ( ndx = 0; ndx < 10; ndx++ )
	{
		printf ( "'%.2f',", Y_Array[ndx] );
	}
	printf ( "'%.2f'", StockMaxHigh );
	printf ( " ];\n" );

	printf ( "$( document ).ready(function() {\n" );
	printf ( "/* console.log( 'document loaded' ); */\n" );
	printf ( "DrawPortfolioChart();\n" );
	printf ( "});\n" );
	printf ( "</script>\n" );

	printf ( "</div>\n" );

	printf ( "</form>\n" );
}

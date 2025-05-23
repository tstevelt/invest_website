/*----------------------------------------------------------------------------
	Program : invest/PaintChart.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Chart of a single stock.
	Return  : 
	
	Who		Date		Modification
	---------------------------------------------------------------------
	tms		03/20/2025	Skip cash and bonds.
	
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

/*----------------------------------------------------------
	these defines will be used if member.Mchrtnum == 0 
----------------------------------------------------------*/
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

static char *CurrentTicker;
static char ChartFilename[128];
static FILE *fpChart;

static int EachTicker ( DBY_QUERY *Query )
{
	if ( ChartRowCount == 0 )
	{
		sprintf ( ChartFilename, "%s/%s.chart", TEMPDIR, SessionUnique );
		if (( fpChart = fopen ( ChartFilename, "w" )) == (FILE *)0 )
		{
			printf ( "Cannot open %s for writing\n", ChartFilename );
			fprintf ( stderr, "Cannot open %s for writing\n", ChartFilename );
			return ( -1 );
		}
	}

	fprintf ( fpChart, "%s\n", Query->EachRow[0] );
	ChartRowCount++;

	if ( nsStrcmp ( CurrentTicker, Query->EachRow[0] ) == 0 )
	{
		ChartCurrentRow = ChartRowCount;
	}

	return ( 0 );
}

void PaintChart ( int RunMode, char *Ticker )
{
	int		ndx, lineno;
	double	StockMinLow, StockMaxHigh;
	double	VolumeMaxHigh;
	double	Y_Array[10];
	char	xbuffer[sizeof(xstock.xsticker)+3];
	int		SlowIndex, FastIndex;
	double	*InputArray;
	double	Slope, StartIntercept, EndIntercept, StdDev;
	int		outBegIndex, outNBElement;

	CurrentTicker = Ticker;
	switch ( RunMode )
	{
		case MODE_WATCHLIST_CHART:
// sprintf ( Statement, "select Wticker from watchlist where Wmember = '%ld' order by Wticker", xmember.xid );
sprintf ( Statement, 
	"select Wticker from watchlist, stock where Wticker = Sticker and Stype != '%c' and Stype != '%c' and Wmember = '%ld' order by Wticker", 
				STYPE_CRYPTO, STYPE_BOND, xmember.xid );
			dbySelectCB ( "invest", &MySql, Statement, (int(*)()) EachTicker, LogFileName );
			nsFclose ( fpChart );
			break;
		case MODE_PORTFOLIO_CHART:
// sprintf ( Statement, "select Pticker from portfolio where Pmember = '%ld' order by Pticker", xmember.xid );
sprintf ( Statement, 
	"select Pticker from portfolio, stock where Pticker = Sticker and Stype != '%c' and Stype != '%c' and Pmember = '%ld' order by Pticker", 
				STYPE_CRYPTO, STYPE_BOND, xmember.xid );
			dbySelectCB ( "invest", &MySql, Statement, (int(*)()) EachTicker, LogFileName );
			nsFclose ( fpChart );
			break;
		case MODE_CHART_PREVIOUS:
		case MODE_CHART_NEXT:
			switch ( RunMode )
			{
				case MODE_CHART_PREVIOUS:
					ChartCurrentRow--;
					break;
				case MODE_CHART_NEXT:
					ChartCurrentRow++;
					break;
			}
			sprintf ( ChartFilename, "%s/%s.chart", TEMPDIR, SessionUnique );
			if (( fpChart = fopen ( ChartFilename, "r" )) == (FILE *)0 )
			{
				printf ( "Cannot open %s for writing\n", ChartFilename );
				fprintf ( stderr, "Cannot open %s for writing\n", ChartFilename );
				break;
			}
			lineno = 0;
			while ( fgets ( xbuffer, sizeof(xbuffer), fpChart ) != (char *)0 )
			{
				lineno++;
				if ( lineno == ChartCurrentRow )
				{
					TrimRight ( xbuffer );
					Ticker = xbuffer;
					break;
				}
			}
			nsFclose ( fpChart );
			break;
		case MODE_STOCK_CHART:
		default:
			break;
	}

	sprintf ( WhereClause, "Sticker = '%s'", Ticker );
	LoadStock ( &MySql, WhereClause, &xstock, 0, 0 );
	if ( xstock.xstype[0] == STYPE_CRYPTO ) // if ( xstock.xstype[0] == 'C' )
	{
		struct tm	*tm;

		/*----------------------------------------------------------
			CRYPTO then load from crypto instead of history
		----------------------------------------------------------*/
		if ( xmember.xmchrtnum == 0 )
		{
			// int LoadCryptoArray ( &MySql, char *Ticker, int OrderBy, int HowMany )
			LoadCryptoArray ( &MySql, xstock.xsticker, SORT_ASCEND, SIX_MONTH );
		}
		else
		{
			LoadCryptoArray ( &MySql, xstock.xsticker, SORT_ASCEND, xmember.xmchrtnum * 23 );
		}

		for ( ndx = 0, HistoryCount = 0; ndx < CryptoCount; ndx++ )
		{
			tm = localtime ( &CryptoArray[ndx].CryptoTime );

			sprintf ( HistoryArray[HistoryCount].Date, "%4d-%02d-%02d", 
							tm->tm_year + 1900,
							tm->tm_mon + 1,
							tm->tm_mday );

			HistoryArray[HistoryCount].Open = CryptoArray[ndx].Open;
			HistoryArray[HistoryCount].High = CryptoArray[ndx].High;
			HistoryArray[HistoryCount].Low = CryptoArray[ndx].Low;
			HistoryArray[HistoryCount].Close = CryptoArray[ndx].Close;
			HistoryArray[HistoryCount].Volume = CryptoArray[ndx].Volume;
			// HistoryArray[ndx].AvgVol,
			// HistoryArray[ndx].Average[SlowIndex],
			// HistoryArray[ndx].Average[FastIndex],
			HistoryCount++;
		}
	}
	else
	{
		if ( xmember.xmchrtnum == 0 )
		{
			LoadHistoryArray ( &MySql, xstock.xsticker, xstock.xslast, SORT_ASCEND, SIX_MONTH );
		}
		else
		{
			LoadHistoryArray ( &MySql, xstock.xsticker, xstock.xslast, SORT_ASCEND, xmember.xmchrtnum * 21 );
		}
	}


	printf ( "<input type='hidden' name='ChartRowCount' value='%d'>\n", ChartRowCount );
	printf ( "<input type='hidden' name='ChartCurrentRow' value='%d'>\n", ChartCurrentRow );
	printf ( "<input type='hidden' id='ChartTicker' value='%s'>\n", Ticker );

	printf ( "<div class='chart'>\n" );
	if ( xstock.xstype[0] == STYPE_FX && xstock.xstype2[0] == STYPE2_HUNDREDTH )
	{
		printf ( "%s %s Currency in hundredths<br>\n", xstock.xsticker, xstock.xsname );
	}
	else if ( xstock.xstype[0] == STYPE_FX && xstock.xstype2[0] == STYPE2_BASIS )
	{
		printf ( "%s %s Currency in ten-thousandths<br>\n", xstock.xsticker, xstock.xsname );
	}
	else
	{
		printf ( "%s %s<br>\n", xstock.xsticker, xstock.xsname );
	}

	VolumeMaxHigh = HistoryArray[0].Volume;

	for ( ndx = 1; ndx < HistoryCount; ndx++ )
	{
		if ( VolumeMaxHigh < HistoryArray[ndx].Volume )
		{
			VolumeMaxHigh = HistoryArray[ndx].Volume;
		}
	}

	printf ( "<canvas id='stockCanvas' width='%d' height='%d' style='border:1px solid #000000;'>\n", STOCK_CHART_WIDTH, STOCK_CHART_HEIGHT );
	printf ( "Your browser does not support the HTML5 canvas tag.\n" );
	printf ( "</canvas>\n" );
	if ( VolumeMaxHigh > 0 )
	{
		printf ( "<canvas id='volumeCanvas' width='%d' height='%d' style='border:1px solid #000000;'>\n", VOLUME_CHART_WIDTH, VOLUME_CHART_HEIGHT );
		printf ( "Your browser does not support the HTML5 canvas tag.\n" );
		printf ( "</canvas>\n" );
	}
	printf ( "</div>\n" );

	StockMinLow  = HistoryArray[0].Low;
	StockMaxHigh = HistoryArray[0].High;
	switch ( xmember.xmchrtslow )
	{
		case 10: SlowIndex = AVG_10; break;
		case 20: SlowIndex = AVG_20; break;
		case 50: SlowIndex = AVG_50; break;
		case 100: SlowIndex = AVG_100; break;
		case 200: 
		default : SlowIndex = AVG_200; break;
	}
	switch ( xmember.xmchrtfast )
	{
		case  5: FastIndex = AVG_5; break;
		case 10: FastIndex = AVG_10; break;
		case 20: FastIndex = AVG_20; break;
		case 50: FastIndex = AVG_50; break;
		case 100: 
		default : FastIndex = AVG_100; break;
	}
	printf ( "<script>\n" );
	printf ( "var SlowDays = %ld;\n", xmember.xmchrtslow );
	printf ( "var FastDays = %ld;\n", xmember.xmchrtfast );
	printf ( "var History = [\n" );
	for ( ndx = 0; ndx < HistoryCount; ndx++ )
	{
		printf ( 
"{Date:'%2.2s/%2.2s',Open:%.2f,High:%.2f,Low:%.2f,Close:%.2f,Volume:%ld,Avg:%ld,Slow:%.2f,Fast:%.2f,RSI:%.2f,SD:%.2f}%s\n",
			&HistoryArray[ndx].Date[5], &HistoryArray[ndx].Date[8],
			HistoryArray[ndx].Open,
			HistoryArray[ndx].High,
			HistoryArray[ndx].Low,
			HistoryArray[ndx].Close,
			HistoryArray[ndx].Volume,
			HistoryArray[ndx].AvgVol,
			HistoryArray[ndx].Average[SlowIndex],
			HistoryArray[ndx].Average[FastIndex],
			HistoryArray[ndx].RSI,
			HistoryArray[ndx].STDDEV,
			ndx + 1 < HistoryCount ? "," : "" );

		if ( StockMinLow  > HistoryArray[ndx].Low )
		{
			StockMinLow  = HistoryArray[ndx].Low;
		}
		if ( HistoryArray[ndx].Average[AVG_50] > 0.0 && StockMinLow  > HistoryArray[ndx].Average[AVG_50] )
		{
			StockMinLow  = HistoryArray[ndx].Average[AVG_50];
		}
		if ( HistoryArray[ndx].Average[AVG_200] > 0.0 && StockMinLow  > HistoryArray[ndx].Average[AVG_200] )
		{
			StockMinLow  = HistoryArray[ndx].Average[AVG_200];
		}

		if ( StockMaxHigh < HistoryArray[ndx].High )
		{
			StockMaxHigh = HistoryArray[ndx].High;
		}
		if ( HistoryArray[ndx].Average[AVG_50] > 0.0 && StockMaxHigh < HistoryArray[ndx].Average[AVG_50] )
		{
			StockMaxHigh = HistoryArray[ndx].Average[AVG_50];
		}
		if ( HistoryArray[ndx].Average[AVG_200] > 0.0 && StockMaxHigh < HistoryArray[ndx].Average[AVG_200] )
		{
			StockMaxHigh = HistoryArray[ndx].Average[AVG_200];
		}
	}
	printf ( "];\n" );

	/*----------------------------------------------------------
		calculate linearr regression and standard deviation
		samples from ../rpt_cooper/EachStock.c (other functions)
TA_MINUS_DI ( 0, Expected-1, HistoryHighArray, HistoryLowArray, HistoryCloseArray, 14, &outMinusBegIndex, &outMinusNBElement, outMinusDI );
TA_PLUS_DI ( 0, Expected-1, HistoryHighArray, HistoryLowArray, HistoryCloseArray, 14, &outPlusBegIndex, &outPlusNBElement, outPlusDI );
TA_ADX ( 0, Expected-1, HistoryHighArray, HistoryLowArray, HistoryCloseArray, 14, &outAdxBegIndex, &outAdxNBElement, outADX );



TA_RetCode TA_LINEARREG_INTERCEPT( int    startIdx,
                                   int    endIdx,
                                   const double inReal[],
                                   int           optInTimePeriod,
                                   int          *outBegIdx,
                                   int          *outNBElement,
                                   double        outReal[] );

TA_RetCode TA_LINEARREG_SLOPE( int    startIdx,
                               int    endIdx,
                               const double inReal[],
                               int           optInTimePeriod,
                               int          *outBegIdx,
                               int          *outNBElement,
                               double        outReal[] );


 
TA_RetCode TA_STDDEV( int    startIdx,
                      int    endIdx,
                      const double inReal[],
                      int           optInTimePeriod,  From 2 to 100000 
                      double        optInNbDev,  From TA_REAL_MIN to TA_REAL_MAX 
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outReal[] );
fixit here
	----------------------------------------------------------*/

	InputArray = calloc ( HistoryCount, sizeof(double) );
	// int		outBegIndex, outNBElement;
	double        optInNbDev = 1.0;
	for ( ndx = 0; ndx < HistoryCount; ndx++ )
	{
		InputArray[ndx] = HistoryArray[ndx].Close;
	}
	TA_LINEARREG_INTERCEPT ( 0, HistoryCount - 1, InputArray, HistoryCount, &outBegIndex, &outNBElement, &StartIntercept );
	TA_LINEARREG_SLOPE     ( 0, HistoryCount - 1, InputArray, HistoryCount, &outBegIndex, &outNBElement, &Slope );
	TA_STDDEV ( 0, HistoryCount - 1, InputArray, HistoryCount, optInNbDev, &outBegIndex, &outNBElement, &StdDev );
	
	EndIntercept = HistoryCount * Slope + StartIntercept;

	/*----------------------------------------------------------
		adjust min and max if std dev exceeds them.
	----------------------------------------------------------*/
	double	xmin, xmax;
	xmin = dmin ( StartIntercept - StdDev, EndIntercept - StdDev );
	xmax = dmax ( StartIntercept + StdDev, EndIntercept + StdDev );
	if ( StockMinLow > xmin )
	{
		StockMinLow = xmin;
	}
	if ( StockMaxHigh < xmax )
	{
		StockMaxHigh = xmax;
	}

	StockMinLow = AutoScale(StockMinLow,ROUND_DOWN);
	StockMaxHigh = AutoScale(StockMaxHigh,ROUND_UP);
	printf ( "var StockMinLow = %.2f;\n", StockMinLow );
	printf ( "var StockMaxHigh = %.2f;\n", StockMaxHigh );
	printf ( "var StockChartWidth = %d;\n", STOCK_CHART_WIDTH );
	printf ( "var StockChartHeight = %d;\n", STOCK_CHART_HEIGHT );

	printf ( "var VolumeMaxHigh = %.0f;\n", AutoScale(VolumeMaxHigh,ROUND_UP) );
	printf ( "var VolumeChartWidth = %d;\n", VOLUME_CHART_WIDTH );
	printf ( "var VolumeChartHeight = %d;\n", VOLUME_CHART_HEIGHT );

	printf ( "var StartIntercept = %.2f;\n", StartIntercept );
	printf ( "var EndIntercept = %.2f;\n", EndIntercept );
	printf ( "var Slope = %.2f;\n",  Slope );
	printf ( "var StdDev = %.2f;\n", StdDev );

	if ( StockMaxHigh - StockMinLow <= 5.0 )
	{
	}
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
	printf ( "DrawStockChart();\n" );
	printf ( "});\n" );
	printf ( "</script>\n" );

	if ( ChartRowCount > 0 )
	{
		// printf ( "&emsp;INTERCEPT %.2f to %.2f SLOPE %.2f STD DEV %.2f", StartIntercept, EndIntercept, Slope, StdDev );

		printf ( "&emsp;%d / %d", ChartCurrentRow, ChartRowCount );
		if ( ChartCurrentRow > 1 )
		{
			printf ( "&emsp;<input type='submit' name='SubmitChartPrevious' value='Previous'>\n" );
		}
		else
		{
			printf ( "&emsp;<input type='button' value='Previous' disabled>\n" );
		}
		if ( ChartCurrentRow < ChartRowCount )
		{
			printf ( "&emsp;<input type='submit' id='SubmitChartNext' name='SubmitChartNext' value='Next'>\n" );
		}
		else
		{
			printf ( "&emsp;<input type='button' value='Next' disabled>\n" );
		}

		printf ( "&emsp;<input type='button' id='SubmitStockFundFmt' value='Fundamentals'" );
		printf ( " onclick='javascript:ShowFundamentals(\"ChartTicker\",\"SubmitChartNext\");'>\n" );

	}

	printf ( "</form>\n" );
}

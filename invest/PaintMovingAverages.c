/*----------------------------------------------------------------------------
	Program : invest/PaintMovingAverages.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint the moving averages on the screeners page.
	Return  : 

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		2023		Added RSI and STDDEV

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

typedef struct
{
	char	*Ticker;
	char	*Tracking;
} RECORD;

static RECORD Array[] =
{
	{ "DIA", "Dow Jones Ind Avg" },
	{ "SPY", "S&P 500" },
	{ "IWM", "Russell 2000" },
	{ "QQQ", "NASDAQ 100" },
	{ "EFA", "Foreign Developed" },
#ifdef HAVE_ALL_DATA
	{ "XLC", "Comm Srvcs Sector" },
	{ "XLP", "Consumer Staples Sector" },
	{ "XLY", "Consumer Dscrtnry Sector" },
	{ "XLE", "Energy Sector" },
	{ "XLF", "Financial Sector" },
	{ "XLV", "Health Care Sector" },
	{ "XLI", "Industrial Sector" },
	{ "XLB", "Materials Sector" },
	{ "XLRE", "Real Estate Sector" },
	{ "XLK", "Technology Sector" },
	{ "XLU", "Utilities Sector" },
#endif
};

static int Count = sizeof(Array) / sizeof(RECORD);
static int Index;
static int Days;

void PaintMovingAverages ( int InAccordian )
{
	char	*Class;
	double	Average[6];

	if ( InAccordian == 0 )
	{
		printf ( "<div class='story'>\n" );
		printf ( "<%s>Market Moving Averages (SMA)</%s>\n", STORY_TITLE, STORY_TITLE );
	}
	printf ( "<table class='table-condensed table-borderless'>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>TICKER</td>\n" );
	printf ( "<td>TRACKING</td>\n" );
	printf ( "<td align='right'>200 DAY</td>\n" );
	printf ( "<td align='right'>100 DAY</td>\n" );
	printf ( "<td align='right'>50 DAY</td>\n" );
	printf ( "<td align='right'>20 DAY</td>\n" );
	printf ( "<td align='right'>10 DAY</td>\n" );
	printf ( "<td align='right'>5 DAY</td>\n" );
	printf ( "<td align='center'>RSI</td>\n" );
	printf ( "<td align='center'>STDDEV</td>\n" );
	printf ( "</tr>\n" );

	for ( Index = 0; Index < Count; Index++ )
	{
		sprintf ( WhereClause, "Sticker = '%s'", Array[Index].Ticker );
		LoadStock ( &MySql, WhereClause, &xstock, 0, 0 );

		sprintf ( WhereClause, "Aticker = '%s' and Adate = '%s'", xstock.xsticker, xstock.xslast );
		LoadAverage ( &MySql, WhereClause, &xaverage, 0 );
		Average[0] = xaverage.xama5;
		Average[1] = xaverage.xama10;
		Average[2] = xaverage.xama20;
		Average[3] = xaverage.xama50;
		Average[4] = xaverage.xama100;
		Average[5] = xaverage.xama200;

		printf ( "<tr>\n" );
		printf ( "<td>%s</td>\n", Array[Index].Ticker );
		printf ( "<td>%s</td>\n", Array[Index].Tracking );

		for ( Days = 5; Days >= 0; Days-- )
		{
			if ( Days == 5 )
			{
				Class = "";
			}
			else if ( Average[Days] < Average[Days+1] )
			{
				Class = " class='bg-danger'";
			}
			else if ( Average[Days] > Average[Days+1] )
			{
				Class = " class='bg-success'";
			}
			else
			{
				Class = "";
			}

			printf ( "<td align='right'%s>%.2f</td>\n", Class, Average[Days] );
		}

		printf ( "<td align='right'>%.2f</td>\n", xaverage.xarsi );

		double Percent = 100.0 * xaverage.xastddev /  Average[5];
		printf ( "<td align='right'>%.2f (%.2f%%)</td>\n", xaverage.xastddev, Percent );

		printf ( "</tr>\n" );
	}

	printf ( "</table>\n" );
	printf ( "<br>\n" );

#ifdef HAVE_ALL_DATA
	char	cmdline [1024];
	sprintf ( cmdline, "/usr/local/bin/getstats -report -neg" );
	ForkAndExec ( cmdline, PRINT_DESTINATION_STDOUT );
#endif

	if ( InAccordian == 0 )
	{
		printf ( "</div>\n" );
	}
}

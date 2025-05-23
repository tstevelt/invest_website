/*----------------------------------------------------------------------------
	Program : invest/PaintAdmin.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint the adminstrator page.
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

static int DebugPaintAdmin = 0;

#define		MAXOUTOFDATE		100
typedef struct
{
	char	*Table;
	char	*WhereClause;
	char	*Subtitle;
} DBSTATS_RECORD;

static DBSTATS_RECORD TableArray [] =
{
	{ "member", "Mstatus = 'P'",                 "Pending" },
	{ "member", "Mstatus = 'V' and Mrole = 'M'", "Free" },
	{ "member", "Mstatus = 'V' and Mrole = 'B'", "Beta" },
	{ "member", "Mstatus = 'V' and Mrole = 'P'", "Paid" },
	{ "member", "Mstatus = 'V' and Mrole = 'X'", "Expired" },
	{ "member", "Mstatus = 'V' and Mrole = 'A'", "Admin" },
#ifdef DBSTATS_IS_TOO_SLOW_AND_WHO_CARES
	{ "stock", "", "&nbsp;" },
	{ "history", "", "&nbsp;" },
	{ "portfolio", "", "&nbsp;" },
	{ "watchlist", "", "&nbsp;" },
	{ "rptlog", "", "&nbsp;" },
#endif
};

static int TableCount = sizeof(TableArray) / sizeof(DBSTATS_RECORD);
static int NullLastDate = 0;

static int EachStock ( DBY_QUERY *Query )
{
	static int Counter = 0;
	char	IndexString[256];
	long	AvgVol = 123;
	int		HistCount = -2;
	DBY_QUERY	*TempQuery;

	if ( Query->EachRow[0][0] == '_' )
	{
		return ( 0 );
	}

	/*----------------------------------------------------------
		get average volume
	----------------------------------------------------------*/
	if ( NullLastDate == 0 )
	{
		snprintf ( Statement, sizeof(Statement),
			"select Avol50 from average, stock where Aticker = Sticker and Aticker = '%s' and Adate = Slast",
					Query->EachRow[0] );

		if ( DebugPaintAdmin )
		{
			printf ( "%s<br>\n", Statement );
			fflush ( stdout );
		}

		TempQuery = dbySelect ( "PaintAdmin", &MySql, Statement, LogFileName );

		if ( TempQuery == (DBY_QUERY *)0 )
		{
			AvgVol = -1;
		}
		else if ( TempQuery->NumRows == 0 )
		{
			AvgVol = 0;
		}	
		else
		{
			if (( TempQuery->EachRow = mysql_fetch_row ( TempQuery->Result )) != NULL )
			{
				AvgVol = nsAtol ( TempQuery->EachRow[0] );
			}
		}

		/*----------------------------------------------------------
			get count of history records in past month
		----------------------------------------------------------*/
		snprintf ( Statement, sizeof(Statement),
			"select count(*) from history, stock where Hticker = Sticker and Hdate > DATE_SUB(Slast,interval 30 day) and Sticker = '%s'",
					Query->EachRow[0] );

		if ( DebugPaintAdmin )
		{
			printf ( "%s<br>\n", Statement );
			fflush ( stdout );
		}

		TempQuery = dbySelect ( "PaintAdmin", &MySql, Statement, LogFileName );

		if ( TempQuery == (DBY_QUERY *)0 )
		{
			HistCount = -1;
		}
		else if ( TempQuery->NumRows == 0 )
		{
			HistCount = 0;
		}	
		else
		{
			if (( TempQuery->EachRow = mysql_fetch_row ( TempQuery->Result )) != NULL )
			{
				HistCount = nsAtoi ( TempQuery->EachRow[0] );
			}
		}
	}
	else
	{
		AvgVol = 0;
		HistCount = 0;
	}

	Counter++;

// 0        1      2      3    4        5       6         7
// Sticker, Sname, Slast, Sdj, Snasdaq, Ssp500, Srussell, Hclosde

	memset ( IndexString, '\0', sizeof(IndexString) );

	if ( Query->EachRow[3][0] == 'Y' )
	{
		nsStrcpy ( IndexString, "Dow Jones" );
	}
	else if ( Query->EachRow[5][0] == 'Y' )
	{
		nsStrcpy ( IndexString, "S&P 500" );
	}
	else if ( Query->EachRow[6][0] == '1' )
	{
		nsStrcpy ( IndexString, "Russell 1000" );
	}
	else if ( Query->EachRow[6][0] == '2' )
	{
		nsStrcpy ( IndexString, "Russell 2000" );
	}
	else if ( Query->EachRow[4][0] == 'Y' )
	{
		nsStrcpy ( IndexString, "NASDAQ" );
	}

	if ( Counter == 1 )
	{
		printf ( "<table class='table-condensed table-borderless'>\n" );

		printf ( "<tr>\n" );
		printf ( "<td>Ticker</td>\n" );
		printf ( "<td>Type</td>\n" );
		printf ( "<td>Company</td>\n" );
		printf ( "<td>Index</td>\n" );
		printf ( "<td>Last Date</td>\n" );
		printf ( "<td align='right'>Last Close</td>\n" );
		printf ( "<td align='right'>Avg Vol</td>\n" );
		printf ( "<td align='right'>Count</td>\n" );
		printf ( "<td align='center'>Delete</td>\n" );
		printf ( "</tr>\n" );
	}

	if ( DebugPaintAdmin )
	{
		for ( int ndx = 0; ndx < 9; ndx++ )
		{
			if ( Query->EachRow[ndx] == NULL )
			{
				printf ( "%d is NULL<br>\n", ndx );
			}
			else
			{
				printf ( "%d is NOT NULL<br>\n", ndx );
			}
		}
		fflush ( stdout );
	}

	printf ( "<tr>\n" );

//	printf ( "<td><a target='sa' href='https://seekingalpha.com/symbol/%s'>%s</a></td>\n", Query->EachRow[0], Query->EachRow[0] );

//	this still works if you want it.  tms 11/11/2023
//	printf ( "<td><a target='sa' href='https://finance.yahoo.com/quote/%s?p=%s'>%s</a></td>\n", 
//				Query->EachRow[0], Query->EachRow[0], Query->EachRow[0] );

	printf ( "<td><a target='sa' href='https://www.barchart.com/stocks/quotes/%s/news'>%s</a></td>\n",
				Query->EachRow[0], Query->EachRow[0] );

	printf ( "<td>%s</td>\n", Query->EachRow[1] );
	printf ( "<td>%s</td>\n", Query->EachRow[2] );
	printf ( "<td>%s</td>\n", IndexString );
	printf ( "<td>%s</td>\n", Query->EachRow[3] );
	printf ( "<td align='right'>%s</td>\n", Query->EachRow[8] );
	printf ( "<td align='right'>%ld</td>\n", AvgVol );
	printf ( "<td align='right'>%d</td>\n", HistCount );
	printf ( "<td align='center'><input type='checkbox' name='delete_%s'></td>\n", Query->EachRow[0] );

	printf ( "</tr>\n" );

	return ( 0 );
}


void PaintAdmin ()
{
	int			ndx;
	long		RecordCount, TotalCount;
	DBY_QUERY	*TempQuery;

#define PAINT_STATS
#define PAINT_MISSING_PREVIOUS

#ifdef PAINT_STATS
	/*----------------------------------------------------------
		database statistics
	----------------------------------------------------------*/
	printf ( "<div class='story'>\n" );

	printf ( "<table class='table-condensed table-borderless'>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>Database Statistics</td>\n" );
	printf ( "<td>Type</td>\n" );
	printf ( "<td align='left'>Record Count</td>\n" );
	printf ( "</tr>\n" );

	for ( ndx = 0; ndx < TableCount; ndx++ )
	{
		if ( nsStrlen ( TableArray[ndx].WhereClause) > 0 )
		{
			RecordCount = dbySelectCount ( &MySql, TableArray[ndx].Table, TableArray[ndx].WhereClause, LogFileName );
		}
		else
		{
			RecordCount = dbySelectCount ( &MySql, TableArray[ndx].Table, NULL, LogFileName );
		}
		if ( RecordCount > 0 )
		{
			printf ( "<tr>\n" );
			printf ( "<td>%s</td>\n", TableArray[ndx].Table );
			printf ( "<td>%s</td>\n", TableArray[ndx].Subtitle );
			printf ( "<td align='left'>%ld</td>\n", RecordCount );
			printf ( "</tr>\n" );
		}
	}

	printf ( "</table>\n" );
	printf ( "</div>\n" );
#endif

	/*----------------------------------------------------------
		index members whose last date is behind
	----------------------------------------------------------*/
#ifdef PAINT_MISSING_PREVIOUS
	printf ( "<div class='story'>\n" );

	TotalCount = 0;

#ifdef ONLY_INDEX_MEMBERS
#define ONLY_INDEX_MEMBERS
	sprintf ( Statement, 
	"select Sticker, Stype, Sname, Slast, Sdj, Snasdaq, Ssp500, Srussell from stock \
	  where (Sdj = 'Y' or Snasdaq = 'Y' or Ssp500 = 'Y' or Srussell = '1' or Srussell = '2') \
		and Slast < (select max(Slast) from stock where Stype != '%c) order by Sname ", STYPE_FX );

	RecordCount = dbySelectCB ( "invest", &MySql, Statement, (int(*)()) EachStock, LogFileName );

	TotalCount += RecordCount;

#else
	sprintf ( Statement, 
		"select count(*) from stock, history \
		  where Slast is not NULL and Slast < (select max(Slast) from stock where Stype != '%c') \
			and Hticker = Sticker and Hdate = Slast", STYPE_FX );

	if ( DebugPaintAdmin )
	{
		printf ( "%s<br>\n", Statement );
	}

	TempQuery = dbySelect ( "invest", &MySql, Statement, LogFileName );

	RecordCount = 0;
	if ( TempQuery == (DBY_QUERY *)0 )
	{
		RecordCount = 0;
	}
	else if ( TempQuery->NumRows == 0 )
	{
		RecordCount = 0;
	}	
	else
	{
		if (( TempQuery->EachRow = mysql_fetch_row ( TempQuery->Result )) != NULL )
		{
			RecordCount = nsAtol ( TempQuery->EachRow[0] );
		}
	}

	if ( DebugPaintAdmin )
	{
		printf ( "RecordCount: %ld<br>\n", RecordCount );
	}

	if ( RecordCount == 0 )
	{
		TotalCount = 0;
	}
	else if ( RecordCount > MAXOUTOFDATE )
	{
		TotalCount = RecordCount;
	}
	else
	{
		/*----------------------------------------------------------
			stocks with past last date
		----------------------------------------------------------*/
		sprintf ( Statement, 
			"select Sticker, Stype, Sname, Slast, Sdj, Snasdaq, Ssp500, Srussell, Hclose from stock, history \
			  where Slast is not NULL and Slast < (select max(Slast) from stock where Stype != '%c') \
				and Hticker = Sticker and Hdate = Slast order by Slast ", STYPE_FX );

		if ( DebugPaintAdmin )
		{
			printf ( "%s<br>\n", Statement );
			fflush ( stdout );
		}

		NullLastDate = 0;
		RecordCount = dbySelectCB ( "invest", &MySql, Statement, (int(*)()) EachStock, LogFileName );

		TotalCount += RecordCount;

#ifdef INCLUDE_NULL_DATE
#define INCLUDE_NULL_DATE
		/*----------------------------------------------------------
			stocks with NULL last date
		----------------------------------------------------------*/
		sprintf ( Statement, 
			"select Sticker, Stype, Sname, 'NULL', Sdj, Snasdaq, Ssp500, Srussell, 0.0 from stock \
			  where Slast is NULL and Stype != '%c' and Stype != '%c' order by Sname ", STYPE_BOND, STYPE_CRYPTO );

		if ( DebugPaintAdmin )
		{
			printf ( "%s<br>\n", Statement );
		}

		NullLastDate = 1;
		RecordCount = dbySelectCB ( "invest", &MySql, Statement, (int(*)()) EachStock, LogFileName );

		TotalCount += RecordCount;
#endif
	}

#endif

	if ( TotalCount == 0 )
	{
#ifdef ONLY_INDEX_MEMBERS
		printf ( "No out-of-date index members" );
#else
		printf ( "No out-of-date stocks" );
#endif
	}
	else if ( TotalCount > MAXOUTOFDATE )
	{
		printf ( "%ld out-of-date stocks", TotalCount );
	}
	else
	{
		if ( TotalCount > 10 )
		{
			printf ( "<tr>\n" );
			printf ( "<td>%ld</td>\n", TotalCount );
			printf ( "<td colspan='2'>Securities</td>\n" );
			printf ( "</tr>\n" );
		}

		printf ( "<tr>\n" );
		printf ( "<td colspan='4'>&nbsp;</td>\n" );
		printf ( "<td align='center'><input type='submit' name='submit_%d' value='Delete'></td>\n", MODE_DELETE_STOCKS );
		printf ( "</tr>\n" );

		printf ( "</table>\n" );
	}

	if ( DebugPaintAdmin )
	{
		printf ( "Happy days<br>\n" );
	}

	printf ( "</div>\n" );
#endif

}

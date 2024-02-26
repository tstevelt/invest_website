/*----------------------------------------------------------------------------
	Program : invlib/LoadHistoryArray.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Load this table
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

#include	"invprivate.h"
#include	"invlib.h"

static int LoadHowMany;
static int MyPrintErrors = 0;
static char Sortby = SORT_ASCEND;
static char *RequestedLastDate;
static	MYSQL	*MyMySql;

static int IntoArray ()
{
	char	MyWhereClause[128];

	if ( HistoryCount >= MAXHISTRECS )
	{
		if ( MyPrintErrors )
		{
			fprintf ( stderr, "LoadHistoryArray: Exceeds MAXHISTRECS\n" );
		}
		return ( -1 );
	}
	
	if ( HistoryCount == 0 && nsStrncmp ( xhistory.xhdate, RequestedLastDate, 10 ) != 0 )
	{
		if ( MyPrintErrors )
		{
			fprintf ( stderr, "LoadHistoryArray: History not up-to-date\n" );
		}
		return ( -1 );
	}

	nsStrcpy ( HistoryArray[HistoryCount].Date, xhistory.xhdate );
	HistoryArray[HistoryCount].Volume = xhistory.xhvolume;
	if ( MungeData == 0 )
	{
		HistoryArray[HistoryCount].Open = xhistory.xhopen;
		HistoryArray[HistoryCount].Close = xhistory.xhclose;
		HistoryArray[HistoryCount].High = xhistory.xhhigh;
		HistoryArray[HistoryCount].Low = xhistory.xhlow;
	}
	else
	{
		HistoryArray[HistoryCount].Open = dblMunge(xhistory.xhopen,-0.25,0.25);
		HistoryArray[HistoryCount].Close = dblMunge(xhistory.xhclose,-0.25,0.25);
		HistoryArray[HistoryCount].High = dblMunge(xhistory.xhhigh,-0.25,0.25);
		HistoryArray[HistoryCount].Low = dblMunge(xhistory.xhlow,-0.25,0.25);
	}

	sprintf ( MyWhereClause, "Aticker = '%s' and Adate = '%s'", xhistory.xhticker, xhistory.xhdate );
	if ( LoadAverage ( MyMySql, MyWhereClause, &xaverage, 0 ) == 1 )
	{
		HistoryArray[HistoryCount].GotRecord = 1;
		HistoryArray[HistoryCount].AverageID = xaverage.xaid;
		if ( MungeData == 0 )
		{
			HistoryArray[HistoryCount].Average[AVG_5]   = xaverage.xama5;
			HistoryArray[HistoryCount].Average[AVG_10]  = xaverage.xama10;
			HistoryArray[HistoryCount].Average[AVG_20]  = xaverage.xama20;
			HistoryArray[HistoryCount].Average[AVG_50]  = xaverage.xama50;
			HistoryArray[HistoryCount].Average[AVG_100] = xaverage.xama100;
			HistoryArray[HistoryCount].Average[AVG_200] = xaverage.xama200;
			HistoryArray[HistoryCount].RSI              = xaverage.xarsi;
			HistoryArray[HistoryCount].STDDEV           = xaverage.xastddev;
			HistoryArray[HistoryCount].CTB              = xaverage.xactb;
		}
		else
		{
			HistoryArray[HistoryCount].Average[AVG_5]   = dblMunge(xaverage.xama5,-0.10,0.10);
			HistoryArray[HistoryCount].Average[AVG_10]  = dblMunge(xaverage.xama10,-0.10,0.10);
			HistoryArray[HistoryCount].Average[AVG_20]  = dblMunge(xaverage.xama20,-0.10,0.10);
			HistoryArray[HistoryCount].Average[AVG_50]  = dblMunge(xaverage.xama50,-0.10,0.10);
			HistoryArray[HistoryCount].Average[AVG_100] = dblMunge(xaverage.xama100,-0.10,0.10);
			HistoryArray[HistoryCount].Average[AVG_200] = dblMunge(xaverage.xama200,-0.10,0.10);
			HistoryArray[HistoryCount].RSI              = dblMunge(xaverage.xarsi,-0.2,0.2);
			HistoryArray[HistoryCount].STDDEV           = dblMunge(xaverage.xastddev,-0.1,0.1);
			HistoryArray[HistoryCount].CTB              = dblMunge(xaverage.xactb,-0.1,0.1);
		}
		HistoryArray[HistoryCount].AvgVol           = xaverage.xavol50;
	}
	else
	{
		HistoryArray[HistoryCount].GotRecord = 0;
		HistoryArray[HistoryCount].AverageID = -1;
		HistoryArray[HistoryCount].Average[AVG_5]   = 0.0;
		HistoryArray[HistoryCount].Average[AVG_10]  = 0.0;
		HistoryArray[HistoryCount].Average[AVG_20]  = 0.0;
		HistoryArray[HistoryCount].Average[AVG_50]  = 0.0;
		HistoryArray[HistoryCount].Average[AVG_100] = 0.0;
		HistoryArray[HistoryCount].Average[AVG_200] = 0.0;
		HistoryArray[HistoryCount].AvgVol           = 0;
		HistoryArray[HistoryCount].RSI              = 0.0;
		HistoryArray[HistoryCount].STDDEV           = 0.0;
		HistoryArray[HistoryCount].CTB              = 0.0;
	}

	HistoryArray[HistoryCount].Flag = 0;
	HistoryArray[HistoryCount].HistoryID = xhistory.xhid;
	HistoryCount++;

	if (LoadHowMany > 0 && HistoryCount >=LoadHowMany )
	{
		return ( -1 );
	}

	return ( 0 );
}

static int cmprec ( HISTORY_RECORD *a, HISTORY_RECORD *b )
{
	if ( Sortby == SORT_ASCEND )
	{
		return ( nsStrcmp ( a->Date, b->Date ) );
	}
	else
	{
		return ( nsStrcmp ( b->Date, a->Date ) );
	}
	return ( 0 );
}

int LoadHistoryArray ( MYSQL *MySql, char *Ticker, char *LastDate, int OrderBy, int HowMany )
{
	char	MyWhereClause[128];

	MyMySql = MySql;

	LoadHowMany = HowMany;
	HistoryCount = 0;
	RequestedLastDate = LastDate;

	switch ( OrderBy )
	{
		case SORT_ASCEND:
		case SORT_DESCEND:
			Sortby = OrderBy;
			break;
		default:
			if ( MyPrintErrors )
			{
				fprintf ( stderr, "Invalid OrderBy\n" );
				return ( 0 );
			}
			break;
	}

	sprintf ( MyWhereClause, "Hticker = '%s' and Hdate <= '%s'", Ticker, LastDate );
	LoadHistoryCB ( MySql, MyWhereClause, "Hdate desc", &xhistory, (int(*)()) IntoArray, MyPrintErrors );

	if ( HistoryCount > 1 )
	{
		qsort ( HistoryArray, HistoryCount, sizeof(HISTORY_RECORD), (int(*)()) cmprec );
	}

	return ( HistoryCount );
}

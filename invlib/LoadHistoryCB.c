/*----------------------------------------------------------------------------
	Program : invlib/LoadHistoryCB.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Load this table, with CallBack function
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

static  char    Statement[MAXSTATEMENT];

int LoadHistoryCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XHISTORY *ptrHistory, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qryHistory;
	int		NumberOfRows = 0;

	// ErrorCount = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from history", FIELDS_HISTORY );

	if ( WhereClause != NULL && nsStrlen ( WhereClause ) > 1 && nsStrcmp ( WhereClause, "all" ) != 0 )
	{
		strcat ( Statement, " where " );
		strcat ( Statement, WhereClause );
	}

	if ( OrderByClause != NULL && nsStrlen ( OrderByClause ) > 1 )
	{
		strcat ( Statement, " order by " );
		strcat ( Statement, OrderByClause );
	}

#ifdef DEBUG
#define DEBUG
	printf ( "LoadHistoryCB: %s\n", Statement );
	fflush ( stdout );
#endif

	qryHistory = dbySelect ( "LoadHistoryCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadHistoryCB: %s<br>\n", PtrToStr ( (char *) qryHistory ) );
	fflush ( stdout );
#endif 

	if ( qryHistory == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadHistoryCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qryHistory->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadHistoryCB select returned zero rows. %s\n", WhereClause );
		}
		dbyFreeQuery ( qryHistory );
		qryHistory = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qryHistory->EachRow = mysql_fetch_row ( qryHistory->Result )) != NULL )
		{

// #define FIELDS_HISTORY  "Hid,Hticker,Hdate,Hopen,Hhigh,Hlow,Hclose,Hvolume"

			ptrHistory->xhid = nsAtof( qryHistory->EachRow[0] );
			snprintf ( ptrHistory->xhticker, sizeof(ptrHistory->xhticker), "%s", qryHistory->EachRow[1] );
			sprintf ( ptrHistory->xhdate, "%-10.10s", qryHistory->EachRow[2] );
			if ( MungeData == 0 )
			{
				ptrHistory->xhopen   = nsAtof( qryHistory->EachRow[3] );
				ptrHistory->xhhigh   = nsAtof( qryHistory->EachRow[4] );
				ptrHistory->xhlow    = nsAtof( qryHistory->EachRow[5] );
				ptrHistory->xhclose  = nsAtof( qryHistory->EachRow[6] );
			}
			else
			{
				ptrHistory->xhopen   = dblMunge(nsAtof( qryHistory->EachRow[3] ),-0.25,0.25);
				ptrHistory->xhhigh   = dblMunge(nsAtof( qryHistory->EachRow[4] ),-0.25,0.25);
				ptrHistory->xhlow    = dblMunge(nsAtof( qryHistory->EachRow[5] ),-0.25,0.25);
				ptrHistory->xhclose  = dblMunge(nsAtof( qryHistory->EachRow[6] ),-0.25,0.25);
			}
			ptrHistory->xhvolume = nsAtol( qryHistory->EachRow[7] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrHistory ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery ( qryHistory );
	qryHistory = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

/*----------------------------------------------------------------------------
	Program : invlib/LoadWatchlistCB.c
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

int LoadWatchlistCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XWATCHLIST *ptrWatchlist, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qryWatchlist;
	int		NumberOfRows = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from watchlist", FIELDS_WATCHLIST );

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
	printf ( "LoadWatchlistCB: %s\n", Statement );
	fflush ( stdout );
#endif

	qryWatchlist = dbySelect ( "LoadWatchlistCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadWatchlistCB: %s<br>\n", PtrToStr ( (char *) qryWatchlist ) );
	fflush ( stdout );
#endif 

	if ( qryWatchlist == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadWatchlistCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qryWatchlist->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadWatchlistCB select returned zero rows. %s\n", WhereClause );
		}
		dbyFreeQuery ( qryWatchlist );
		qryWatchlist = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qryWatchlist->EachRow = mysql_fetch_row ( qryWatchlist->Result )) != NULL )
		{
			ptrWatchlist->xid            = nsAtol( qryWatchlist->EachRow[0] );
			snprintf ( ptrWatchlist->xwticker, sizeof(ptrWatchlist->xwticker), "%s", qryWatchlist->EachRow[1] );
			ptrWatchlist->xwmember       = nsAtol( qryWatchlist->EachRow[2] );
			ptrWatchlist->xwalerttype[0] = qryWatchlist->EachRow[3][0];
			ptrWatchlist->xwalertprice   = nsAtof( qryWatchlist->EachRow[4] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrWatchlist ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery ( qryWatchlist );
	qryWatchlist = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

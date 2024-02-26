/*----------------------------------------------------------------------------
	Program : invlib/LoadMarketCB.c
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

int LoadMarketCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XMARKET *ptrMarket, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qryMarket;
	int		NumberOfRows = 0;

	// ErrorCount = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from market", FIELDS_MARKET );

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
	printf ( "LoadMarketCB: %s\n", Statement );
	fflush ( stdout );
#endif

	qryMarket = dbySelect ( "LoadMarketCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadMarketCB: %s<br>\n", PtrToStr ( (char *) qryMarket ) );
	fflush ( stdout );
#endif 

	if ( qryMarket == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadMarketCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qryMarket->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadMarketCB select returned zero rows. %s\n", WhereClause );
		}
		dbyFreeQuery ( qryMarket );
		qryMarket = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qryMarket->EachRow = mysql_fetch_row ( qryMarket->Result )) != NULL )
		{
			sprintf ( ptrMarket->xmktdate, "%-10.10s", qryMarket->EachRow[0] );
			ptrMarket->xmktindex[0] = qryMarket->EachRow[1][0];
			ptrMarket->xmktaccum    = nsAtoi( qryMarket->EachRow[2] );
			ptrMarket->xmktdist     = nsAtoi( qryMarket->EachRow[3] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrMarket ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery ( qryMarket );
	qryMarket = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

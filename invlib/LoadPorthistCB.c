/*----------------------------------------------------------------------------
	Program : invlib/LoadPorthistCB.c
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

int LoadPorthistCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XPORTHIST *ptrPorthist, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qryPorthist;
	int		NumberOfRows = 0;

	// ErrorCount = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from porthist", FIELDS_PORTHIST );

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
	printf ( "LoadPorthistCB: %s\n", Statement );
	fflush ( stdout );
#endif

	qryPorthist = dbySelect ( "LoadPorthistCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadPorthistCB: %s<br>\n", PtrToStr ( (char *) qryPorthist ) );
	fflush ( stdout );
#endif 

	if ( qryPorthist == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadPorthistCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qryPorthist->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadPorthistCB select returned zero rows. %s\n", WhereClause );
		}
		dbyFreeQuery ( qryPorthist );
		qryPorthist = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qryPorthist->EachRow = mysql_fetch_row ( qryPorthist->Result )) != NULL )
		{

// #define FIELDS_PORTHIST  "PHmember,PHdate,PHinvest,PHequity,PHbonds,PHcash"

			ptrPorthist->xphmember = nsAtol( qryPorthist->EachRow[0] );
			sprintf ( ptrPorthist->xphdate, "%-10.10s", qryPorthist->EachRow[1] );
			ptrPorthist->xphinvest = nsAtof( qryPorthist->EachRow[2] );
			ptrPorthist->xphequity = nsAtof( qryPorthist->EachRow[3] );
			ptrPorthist->xphbonds  = nsAtof( qryPorthist->EachRow[4] );
			ptrPorthist->xphcash   = nsAtof( qryPorthist->EachRow[5] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrPorthist ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery ( qryPorthist );
	qryPorthist = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

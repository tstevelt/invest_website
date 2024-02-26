/*----------------------------------------------------------------------------
	Program : invlib/LoadTempCB.c
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

int LoadTempCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XTEMP *ptrTemp, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qryTemp;
	int		NumberOfRows = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from temp", FIELDS_TEMP );

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
	printf ( "LoadTempCB: %s\n", Statement );
	fflush ( stdout );
#endif

	qryTemp = dbySelect ( "LoadTempCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadTempCB: %s<br>\n", PtrToStr ( (char *) qryTemp ) );
	fflush ( stdout );
#endif 

	if ( qryTemp == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadTempCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qryTemp->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadTempCB select returned zero rows. %s\n", WhereClause );
		}
		dbyFreeQuery ( qryTemp );
		qryTemp = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qryTemp->EachRow = mysql_fetch_row ( qryTemp->Result )) != NULL )
		{
// #define FIELDS_TEMP  "id, Tmember, Tticker"
			ptrTemp->xid            = nsAtol( qryTemp->EachRow[0] );
			ptrTemp->xtmember       = nsAtol( qryTemp->EachRow[1] );
			snprintf ( ptrTemp->xtticker, sizeof(ptrTemp->xtticker), "%s", qryTemp->EachRow[2] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrTemp ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery ( qryTemp );
	qryTemp = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

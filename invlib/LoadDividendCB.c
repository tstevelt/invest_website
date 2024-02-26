/*----------------------------------------------------------------------------
	Program : invlib/LoadDividendCB.c
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

int LoadDividendCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XDIVIDEND *ptrDividend, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qryDividend;
	int		NumberOfRows = 0;

	// ErrorCount = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from dividend", FIELDS_DIVIDEND );

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
	printf ( "LoadDividendCB: %s\n", Statement );
	fflush ( stdout );
#endif

	qryDividend = dbySelect ( "LoadDividendCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadDividendCB: %s<br>\n", PtrToStr ( (char *) qryDividend ) );
	fflush ( stdout );
#endif 

	if ( qryDividend == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadDividendCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qryDividend->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadDividendCB select returned zero rows. %s\n", WhereClause );
		}
		dbyFreeQuery ( qryDividend );
		qryDividend = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qryDividend->EachRow = mysql_fetch_row ( qryDividend->Result )) != NULL )
		{
// id, Dticker, Dexdate, Damount, Dcurr

// yyyy-mm-dd
			ptrDividend->xid =  nsAtol( qryDividend->EachRow[0] );
			snprintf ( ptrDividend->xdticker, sizeof(ptrDividend->xdticker), "%s", qryDividend->EachRow[1] );
			sprintf ( ptrDividend->xdexdate, "%-10.10s", qryDividend->EachRow[2] );
			ptrDividend->xdamount   = nsAtof( qryDividend->EachRow[3] );
			snprintf ( ptrDividend->xdcurr, sizeof(ptrDividend->xdcurr), "%s", qryDividend->EachRow[4] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrDividend ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery ( qryDividend );
	qryDividend = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

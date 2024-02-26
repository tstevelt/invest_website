/*----------------------------------------------------------------------------
	Program : invlib/LoadCashbalCB.c
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

int LoadCashbalCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XCASHBAL *ptrCashacct, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qryCashacct;
	int		NumberOfRows = 0;

	// ErrorCount = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from cashbal", FIELDS_CASHBAL );

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
	printf ( "LoadCashbalCB: %s\n", Statement );
	fflush ( stdout );
#endif

	qryCashacct = dbySelect ( "LoadCashbalCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadCashbalCB: %s<br>\n", PtrToStr ( (char *) qryCashacct ) );
	fflush ( stdout );
#endif 

	if ( qryCashacct == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadCashbalCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qryCashacct->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadCashbalCB select returned zero rows. %s\n", WhereClause );
		}
		dbyFreeQuery ( qryCashacct );
		qryCashacct = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qryCashacct->EachRow = mysql_fetch_row ( qryCashacct->Result )) != NULL )
		{

// #define FIELDS_CASHBAL  "id,CBmember,CBacct,CBdate,CBamount,CBcomment"

			ptrCashacct->xid         = nsAtol( qryCashacct->EachRow[0] );
			ptrCashacct->xcbmember   = nsAtol( qryCashacct->EachRow[1] );
			snprintf ( ptrCashacct->xcbacct, sizeof(ptrCashacct->xcbacct), "%s", qryCashacct->EachRow[2] );
			sprintf ( ptrCashacct->xcbdate, "%-10.10s", qryCashacct->EachRow[3] );
			ptrCashacct->xcbamount    = nsAtof( qryCashacct->EachRow[4] );
			snprintf ( ptrCashacct->xcbcomment, sizeof(ptrCashacct->xcbcomment), "%s", qryCashacct->EachRow[5] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrCashacct ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery ( qryCashacct );
	qryCashacct = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

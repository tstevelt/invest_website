/*----------------------------------------------------------------------------
	Program : invlib/LoadCryptoCB.c
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

int LoadCryptoCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XCRYPTO *ptrCrypto, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qryCrypto;
	int		NumberOfRows = 0;

	// ErrorCount = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from crypto", FIELDS_CRYPTO );

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
	printf ( "LoadCryptoCB: %s\n", Statement );
	fflush ( stdout );
#endif

	qryCrypto = dbySelect ( "LoadCryptoCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadCryptoCB: %s<br>\n", PtrToStr ( (char *) qryCrypto ) );
	fflush ( stdout );
#endif 

	if ( qryCrypto == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadCryptoCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qryCrypto->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadCryptoCB select returned zero rows. %s\n", WhereClause );
		}
		dbyFreeQuery ( qryCrypto );
		qryCrypto = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qryCrypto->EachRow = mysql_fetch_row ( qryCrypto->Result )) != NULL )
		{
			snprintf ( ptrCrypto->xcticker, sizeof(ptrCrypto->xcticker), "%s", qryCrypto->EachRow[0] );
			ptrCrypto->xctime = nsAtol (  qryCrypto->EachRow[1] );
			ptrCrypto->xcvolume = nsAtol( qryCrypto->EachRow[6] );
			if ( MungeData == 0 )
			{
				ptrCrypto->xcopen   = nsAtof( qryCrypto->EachRow[2] );
				ptrCrypto->xchigh   = nsAtof( qryCrypto->EachRow[3] );
				ptrCrypto->xclow    = nsAtof( qryCrypto->EachRow[4] );
				ptrCrypto->xcclose  = nsAtof( qryCrypto->EachRow[5] );
			}
			else
			{
				ptrCrypto->xcopen   = dblMunge(nsAtof( qryCrypto->EachRow[2] ),-0.25,0.25);
				ptrCrypto->xchigh   = dblMunge(nsAtof( qryCrypto->EachRow[3] ),-0.25,0.25);
				ptrCrypto->xclow    = dblMunge(nsAtof( qryCrypto->EachRow[4] ),-0.25,0.25);
				ptrCrypto->xcclose  = dblMunge(nsAtof( qryCrypto->EachRow[5] ),-0.25,0.25);
			}

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrCrypto ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery ( qryCrypto );
	qryCrypto = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

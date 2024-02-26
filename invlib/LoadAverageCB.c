/*----------------------------------------------------------------------------
	Program : invlib/LoadAverageCB.c
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

int LoadAverageCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XAVERAGE *ptrAverage, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qryAverage;
	int		NumberOfRows = 0;

	// ErrorCount = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from average", FIELDS_AVERAGE );

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
	printf ( "LoadAverageCB: %s\n", Statement );
	fflush ( stdout );
#endif

	qryAverage = dbySelect ( "LoadAverageCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadAverageCB: %s<br>\n", PtrToStr ( (char *) qryAverage ) );
	fflush ( stdout );
#endif 

	if ( qryAverage == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadAverageCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qryAverage->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadAverageCB select returned zero rows. %s\n", WhereClause );
		}
		dbyFreeQuery ( qryAverage );
		qryAverage = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qryAverage->EachRow = mysql_fetch_row ( qryAverage->Result )) != NULL )
		{

//                          0   1       2     3     4    5     6     7      8      9     10    11      12
// #define FIELDS_AVERAGE  "Aid,Aticker,Adate,Ama5,Ama10,Ama20,Ama50,Ama100,Ama200,Avol50,Arsi,Astddev,Actb"

			ptrAverage->xaid    = nsAtol( qryAverage->EachRow[0] );
			snprintf ( ptrAverage->xaticker, sizeof(ptrAverage->xaticker), "%s", qryAverage->EachRow[1] );
			sprintf ( ptrAverage->xadate, "%-10.10s", qryAverage->EachRow[2] );
			ptrAverage->xama5   = nsAtof( qryAverage->EachRow[3] );
			ptrAverage->xama10  = nsAtof( qryAverage->EachRow[4] );
			ptrAverage->xama20  = nsAtof( qryAverage->EachRow[5] );
			ptrAverage->xama50  = nsAtof( qryAverage->EachRow[6] );
			ptrAverage->xama100 = nsAtof( qryAverage->EachRow[7] );
			ptrAverage->xama200 = nsAtof( qryAverage->EachRow[8] );
			ptrAverage->xavol50 = nsAtol( qryAverage->EachRow[9] );
			ptrAverage->xarsi   = nsAtof( qryAverage->EachRow[10] );
			ptrAverage->xastddev = nsAtof( qryAverage->EachRow[11] );
			ptrAverage->xactb   = nsAtof( qryAverage->EachRow[12] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrAverage ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery ( qryAverage );
	qryAverage = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

/*----------------------------------------------------------------------------
	Program : invlib/LoadPortfolioCB.c
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

int LoadPortfolioCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XPORTFOLIO *ptrPortfolio, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qryPortfolio;
	int		NumberOfRows = 0;

	// ErrorCount = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from portfolio", FIELDS_PORTFOLIO );

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
	printf ( "LoadPortfolioCB: %s\n", Statement );
	fflush ( stdout );
#endif

	qryPortfolio = dbySelect ( "LoadPortfolioCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadPortfolioCB: %s<br>\n", PtrToStr ( (char *) qryPortfolio ) );
	fflush ( stdout );
#endif 

	if ( qryPortfolio == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadPortfolioCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qryPortfolio->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadPortfolioCB select returned zero rows. %s\n", WhereClause );
		}
		dbyFreeQuery ( qryPortfolio );
		qryPortfolio = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qryPortfolio->EachRow = mysql_fetch_row ( qryPortfolio->Result )) != NULL )
		{

// #define FIELDS_PORTFOLIO  "id,Pticker,Pdate,Pshares,Pprice,Pbenchmark,Palert,Poptexp,Pbroker,Pmember,Ptrailpct,Ptrailvalue,Palerttype,Palertprice,Pbucket"
			ptrPortfolio->xid         = nsAtol( qryPortfolio->EachRow[0] );
			snprintf ( ptrPortfolio->xpticker, sizeof(ptrPortfolio->xpticker), "%s", qryPortfolio->EachRow[1] );
			sprintf ( ptrPortfolio->xpdate, "%-10.10s", qryPortfolio->EachRow[2] );
			ptrPortfolio->xpshares    = nsAtof( qryPortfolio->EachRow[3] );
			ptrPortfolio->xpprice     = nsAtof( qryPortfolio->EachRow[4] );
			snprintf ( ptrPortfolio->xpbenchmark, sizeof(ptrPortfolio->xpbenchmark), "%s", qryPortfolio->EachRow[5] );
			ptrPortfolio->xpalert     = nsAtof( qryPortfolio->EachRow[6] );
			if ( qryPortfolio->EachRow[7] )
			{
				sprintf ( ptrPortfolio->xpoptexp, "%-10.10s", qryPortfolio->EachRow[7] );
			}
			else
			{
				memset ( ptrPortfolio->xpoptexp, '\0', sizeof(ptrPortfolio->xpoptexp) );
			}
			if ( qryPortfolio->EachRow[8] )
			{
				sprintf ( ptrPortfolio->xpbroker, "%-10.10s", qryPortfolio->EachRow[8] );
			}
			else
			{
				memset ( ptrPortfolio->xpbroker, '\0', sizeof(ptrPortfolio->xpbroker) );
			}
			ptrPortfolio->xpmember     = nsAtol( qryPortfolio->EachRow[9] );
			ptrPortfolio->xptrailpct   = nsAtof( qryPortfolio->EachRow[10] );
			ptrPortfolio->xptrailvalue = nsAtof( qryPortfolio->EachRow[11] );
			ptrPortfolio->xpalerttype[0] = qryPortfolio->EachRow[12][0];
			ptrPortfolio->xpalertprice = nsAtof( qryPortfolio->EachRow[13] );
			ptrPortfolio->xpbucket = nsAtoi( qryPortfolio->EachRow[14] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrPortfolio ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery ( qryPortfolio );
	qryPortfolio = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

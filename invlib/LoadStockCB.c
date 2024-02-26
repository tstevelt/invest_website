/*----------------------------------------------------------------------------
	Program : invlib/LoadStockCB.c
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

int LoadStockCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XSTOCK *ptrStock, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qryStock;
	int		NumberOfRows = 0;

	// ErrorCount = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from stock", FIELDS_STOCK );

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
	printf ( "LoadStockCB: %s<br>\n", Statement );
	fflush ( stdout );
#endif 

	qryStock = dbySelect ( "LoadStockCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadStockCB: %s<br>\n", PtrToStr ( (char *) qryStock ) );
	fflush ( stdout );
#endif 

	if ( qryStock == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadStockCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qryStock->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadStockCB select returned zero rows. %s\n", WhereClause );
		}
		dbyFreeQuery (  qryStock );
		qryStock = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qryStock->EachRow = mysql_fetch_row ( qryStock->Result )) != NULL )
		{

// FIELDS_STOCK  "Sticker,Sname,Slast,Sclose,Stype,Ssp500,Srussell,Sdj,Snasdaq,Scheck,Shigh52,Sdate52,Sindustry,Sexchange,Ssector,Snews,Sdomfor,Stype2,Scik"

			snprintf ( ptrStock->xsticker, sizeof(ptrStock->xsticker), "%s", qryStock->EachRow[0] );
			if ( MungeData == 0 )
			{
				snprintf ( ptrStock->xsname, sizeof(ptrStock->xsname), "%s", qryStock->EachRow[1] );
			}
			else
			{
				// fixit -- tms 01/23/2023 -- turned off MungeData for company name
				// snprintf ( ptrStock->xsname, sizeof(ptrStock->xsname), "%s", strMunge(qryStock->EachRow[1]) );
				snprintf ( ptrStock->xsname, sizeof(ptrStock->xsname), "%s", qryStock->EachRow[1] );
			}

			sprintf ( ptrStock->xslast,    "%-10.10s", qryStock->EachRow[2] );
			ptrStock->xsclose =                nsAtof( qryStock->EachRow[3] );
			sprintf ( ptrStock->xstype,    "%-1.1s",   qryStock->EachRow[4] );
			sprintf ( ptrStock->xssp500,   "%-1.1s",   qryStock->EachRow[5] );
			sprintf ( ptrStock->xsrussell, "%-1.1s",   qryStock->EachRow[6] );
			sprintf ( ptrStock->xsdj,      "%-1.1s",   qryStock->EachRow[7] );
			sprintf ( ptrStock->xsnasdaq,  "%-1.1s",   qryStock->EachRow[8] );
			sprintf ( ptrStock->xscheck,   "%-1.1s",   qryStock->EachRow[9] );
			ptrStock->xshigh52 =               nsAtof( qryStock->EachRow[10] );
			sprintf ( ptrStock->xsdate52,  "%-10.10s", qryStock->EachRow[11] );
			ptrStock->xsindustry =             nsAtol( qryStock->EachRow[12] );
			if ( qryStock->EachRow[14] )
			{
				sprintf ( ptrStock->xsexchange, "%s",  qryStock->EachRow[13] );
			}
			else
			{
				ptrStock->xsexchange[0] = '\0';
			}
			ptrStock->xssector = nsAtol( qryStock->EachRow[14] );
			ptrStock->xsnews   = nsAtol( qryStock->EachRow[15] );
			sprintf ( ptrStock->xsdomfor,  "%-1.1s", qryStock->EachRow[16] );
			sprintf ( ptrStock->xstype2,   "%-1.1s", qryStock->EachRow[17] );
			sprintf ( ptrStock->xscik,     "%-10.10s", qryStock->EachRow[18] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrStock ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery (  qryStock );
	qryStock = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

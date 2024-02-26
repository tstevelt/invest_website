/*----------------------------------------------------------------------------
	Program : invlib/LoadIndustryCB.c
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

int LoadIndustryCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XINDUSTRY *ptrIndustry, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qryIndustry;
	int		NumberOfRows = 0;

	// ErrorCount = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from industry", FIELDS_INDUSTRY );

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
	printf ( "LoadIndustryCB: %s\n", Statement );
	fflush ( stdout );
#endif

	qryIndustry = dbySelect ( "LoadIndustryCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadIndustryCB: %s<br>\n", PtrToStr ( (char *) qryIndustry ) );
	fflush ( stdout );
#endif 

	if ( qryIndustry == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadIndustryCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qryIndustry->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadIndustryCB select returned zero rows.\n" );
			fprintf ( stderr, "WhereClause: [%s]\n", WhereClause );
			fprintf ( stderr, "Statement: [%s]\n", Statement );
		}
		dbyFreeQuery ( qryIndustry );
		qryIndustry = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qryIndustry->EachRow = mysql_fetch_row ( qryIndustry->Result )) != NULL )
		{
			ptrIndustry->xid = nsAtol( qryIndustry->EachRow[0] );
			snprintf ( ptrIndustry->xindname, sizeof(ptrIndustry->xindname), "%s", qryIndustry->EachRow[1] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrIndustry ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery ( qryIndustry );
	qryIndustry = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

/*----------------------------------------------------------------------------
	Program : invlib/LoadSectorCB.c
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

int LoadSectorCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XSECTOR *ptrSector, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qrySector;
	int		NumberOfRows = 0;

	// ErrorCount = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from sector", FIELDS_SECTOR );

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
	printf ( "LoadSectorCB: %s\n", Statement );
	fflush ( stdout );
#endif

	qrySector = dbySelect ( "LoadSectorCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadSectorCB: %s<br>\n", PtrToStr ( (char *) qrySector ) );
	fflush ( stdout );
#endif 

	if ( qrySector == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadSectorCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qrySector->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadSectorCB select returned zero rows.\n" );
			fprintf ( stderr, "WhereClause: [%s]\n", WhereClause );
			fprintf ( stderr, "Statement: [%s]\n", Statement );
		}
		dbyFreeQuery ( qrySector );
		qrySector = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qrySector->EachRow = mysql_fetch_row ( qrySector->Result )) != NULL )
		{
			ptrSector->xid = nsAtol( qrySector->EachRow[0] );
			snprintf ( ptrSector->xsecname, sizeof(ptrSector->xsecname), "%s", qrySector->EachRow[1] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrSector ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery ( qrySector );
	qrySector = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

/*----------------------------------------------------------------------------
	Program : invlib/LoadSystem.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Load this table
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

int LoadSystem ( MYSQL *MySql, XSYSTEM *ptrSystem, int PrintErrors )
{
	DBY_QUERY     *qrySystem;
	int		NumberOfRows = 0;

	// ErrorCount = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement), "select %s from system where system.id = 1", FIELDS_SYSTEM );

	qrySystem = dbySelect ( "LoadSystem", MySql, Statement, LogFileName );

	if ( qrySystem == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadSystem select returned NULL.\n" );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qrySystem->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadSystem select returned zero rows.\n" );
		}
		dbyFreeQuery ( qrySystem );
		qrySystem = (DBY_QUERY *) 0;
		return ( -2 );
	}
	else 
	{
		if (( qrySystem->EachRow = mysql_fetch_row ( qrySystem->Result )) != NULL )
		{
			ptrSystem->xid      = nsAtol( qrySystem->EachRow[0] );
			ptrSystem->xydivcnt = nsAtol( qrySystem->EachRow[1] );
			sprintf ( ptrSystem->xydivtime, "%-20.20s", qrySystem->EachRow[2] );
			ptrSystem->xystkcnt = nsAtol( qrySystem->EachRow[3] );
			sprintf ( ptrSystem->xystktime, "%-20.20s", qrySystem->EachRow[4] );
		}
		else
		{
			memset ( ptrSystem, '\0', sizeof(XSYSTEM) );
			ptrSystem->xid = -1;
		}
	}

	dbyFreeQuery ( qrySystem );
	qrySystem = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

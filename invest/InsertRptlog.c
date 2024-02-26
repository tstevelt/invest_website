/*----------------------------------------------------------------------------
	Program : invest/InsertRptlog.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Log report usage.
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

#include	"invest.h"

static int DebugInsertRptlog = 0;

void InsertRptlog ( char *CommandLine )
{
	int			Affected;
	char		xbuffer[256];
	int			xl, xs, xe;

	if ( DebugInsertRptlog )
	{
		printf ( "InsertRptlog: %ld %c<br>\n", CookieMember.xid, CookieMember.xmrole[0] );
	}

	if ( CookieMember.xmrole[0] != ROLE_FREE )
	{
		return;
	}

	/*---------------------------------------------------------------------------
		Command line should look like:
			rpt_xxxx arg1 arg2 arg3 ...
		But could look like these:
			/usr/local/bin/rpt_xxxx arg1 arg2 arg3 ...
			rpt_noargs
		And they don't necessarily start with rpt_
	---------------------------------------------------------------------------*/

	xl = snprintf ( xbuffer, sizeof(xbuffer), "%s", CommandLine );

	for ( xe = 0; xe < xl; xe++ )
	{
		if ( xbuffer[xe] == ' ' )
		{
			xbuffer[xe] = '\0';
			break;
		}
	}

	for ( xs = xe - 1; xs > 0; xs-- )
	{
		if ( xbuffer[xs] == '/' )
		{
			break;
		}
	}
	
	snprintf ( Statement, sizeof(Statement),
		"insert into rptlog ( Rmember, Rcommand, Rdatetime ) values ( %ld, '%s', '%s' )",
			CookieMember.xid, &xbuffer[xs], fmtGetTodayDate(DATEFMT_TIMESTAMP) );

	/*----------------------------------------------------------
		insert rptlog
	----------------------------------------------------------*/

	Affected = dbyInsert ( "InsertRptlog", &MySql, Statement, 0, LogFileName );
	if ( Affected == 0 )
	{
		SafeError ( COLOR_ERROR, "InsertRptlog failed to add new record." );
	}
}

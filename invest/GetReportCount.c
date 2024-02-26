/*----------------------------------------------------------------------------
	Program : invest/GetReportCount.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: How many reports did user run in past 30 days?
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

long GetReportCount ()
{
	long	rv;
	DATEVAL	dvStart;

#ifdef LIMIT_BY_CALENDAR_MONTH
	char	ThisMonth[8];
	sprintf ( ThisMonth, "%7.7s", Today );
	sprintf ( WhereClause, "Rmember = %ld and Rdatetime like '%s%%'", CookieMember.xid, ThisMonth );
#endif

	DateAdd ( &dvToday, -30, &dvStart );
	sprintf ( WhereClause, "Rmember = %ld and Rdatetime > '%4d-%02d-%02d'", 
				CookieMember.xid, dvStart.year4, dvStart.month, dvStart.day );

	rv = dbySelectCount ( &MySql, "rptlog", WhereClause, LogFileName );

	return ( rv );
}

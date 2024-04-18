/*----------------------------------------------------------------------------
	Program : invest/SaveProfile.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Save changes to user profile.
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

#define	DEBUG
#undef	DEBUG

void SaveProfile ()
{
	char	cmdline[1024];

	LoadMember ( &MySql, WhereClause, &xmember, 0 );

	TrimRightAndLeft ( SignupName );
	TrimRightAndLeft ( SignupPager );
	if (( nsStrcmp ( xmember.xmname, SignupName   ) == 0 ) &&
		( nsStrcmp ( xmember.xmpager, SignupPager ) == 0 ) &&
		( xmember.xmchrtnum == ChartNumber        )        &&
		( xmember.xmchrtfast == ChartFast         )        &&
		( xmember.xmchrtslow == ChartSlow         )        &&
		( xmember.xmcasht == CashT                )        &&
		( xmember.xmbondt == BondT                )        &&
		( xmember.xmdomt == DomT                  )        &&
		( xmember.xmfort == ForT                  ))
	{
		SafeError ( COLOR_WARN, "Nothing to do" );
		return;
	}

	/*----------------------------------------------------------
		save name
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
"update member set Mname = '%s', Mpager = '%s', Mchrtnum = %d, Mchrtfast = %d, Mchrtslow = %d, \
 McashT = %.2f, MbondT = %.2f, MdomT = %.2f, MforT = %.2f \
 where member.id = %ld", 
			SignupName, SignupPager, /* IncludeDividend, */ ChartNumber, ChartFast, ChartSlow, CashT, BondT, DomT, ForT, CookieMember.xid );

	dbyUpdate ( "SaveProfile", &MySql, Statement, 0, LogFileName );

	snprintf ( WhereClause, sizeof(WhereClause), "member.id = %ld", CookieMember.xid );
	LoadMember ( &MySql, WhereClause, &xmember, 0 );

	sprintf ( cmdline, "/usr/local/bin/getdiv -member %ld", CookieMember.xid );
	ForkAndExec ( cmdline, PRINT_DESTINATION_ERROR_ARRAY );
}

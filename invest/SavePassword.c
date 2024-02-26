/*----------------------------------------------------------------------------
	Program : invest/SavePassword.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Save new password.
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

void SavePassword ()
{
	long	Affected;
	int		WasLoggedIn = 0;

	if ( CookieMember.xid > 0 )
	{
		xmember.xid = CookieMember.xid;
		WasLoggedIn = 1;
	}

	/*----------------------------------------------------------
		set new password
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"update member set Mpassword = '%s' where member.id = %ld", 
			pw_sha_make_pw ( (unsigned char *) SignupPassword ), xmember.xid );

#ifdef DEBUG
#define DEBUG
	SafeError ( COLOR_ERROR, Statement );
#endif

	Affected = dbyUpdate ( "SavePassword", &MySql, Statement, 0, LogFileName );
	if ( Affected == 0 )
	{
		SafeError ( COLOR_ERROR, "Change password failed." );
		return;
	}
	else if ( WasLoggedIn == 0 )
	{
		SafeError ( COLOR_SUCCESS, "Password saved for %s.", xmember.xmname );
	}
	else
	{
		SafeError ( COLOR_SUCCESS, "Password changed." );
	}


	if ( CookieMember.xid == 0 )
	{
		MenuIndex = 4;
	}
	else
	{
		MenuIndex = 1;
	}

	return;
}

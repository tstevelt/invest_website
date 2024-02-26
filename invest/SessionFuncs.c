/*----------------------------------------------------------------------------
	Program : invest/SessionFuncs.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Session functions. 
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
static	FILE	*fpSession;

static void MakeStringSpace ()
{
	snprintf ( CookieMember.xmname, sizeof(CookieMember.xmname), "Not logged in" );
}

static void MakeSessionFileName ()
{
	sprintf ( SessionFileName, "%s/%s.session", WEBSTATEDIR, SessionUnique );
}

void WriteSession ()
{
	MakeSessionFileName ();

	if (( fpSession = fopen ( SessionFileName, "w" )) == (FILE *)0 )
	{
		SafeError ( COLOR_ERROR, "Cannot create session file %s", SessionFileName );
		return;
	}

	sprintf ( WhereClause, "member.id = %ld", CookieMember.xid );
	if ( LoadMember ( &MySql, WhereClause, &CookieMember, 0 ) < 1 )
	{
		SafeError ( COLOR_ERROR, "CannotLoadMember %ld", CookieMember.xid );
		nsFclose ( fpSession );
		return;
	}

	fprintf ( fpSession, "%ld|%c|%s\n", CookieMember.xid, CookieMember.xmrole[0], CookieMember.xmname );

	nsFclose ( fpSession );
}

void ReadSession ()
{
	MakeStringSpace ();

	MakeSessionFileName ();

	if (( fpSession = fopen ( SessionFileName, "r" )) == (FILE *)0 )
	{
		CookieMember.xid = 0;
		return;
	}

	fscanf ( fpSession, "%ld|%1c|%30c", &CookieMember.xid, CookieMember.xmrole, CookieMember.xmname );

	nsFclose ( fpSession  );
}

void RemoveSession ()
{
	MakeStringSpace ();

	MakeSessionFileName ();

	unlink ( SessionFileName );

}

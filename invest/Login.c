/*----------------------------------------------------------------------------
	Program : invest/Login.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Log in the user and write seesion.
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

int Login ()
{
	int			rv = 0;
	int			mysql_rv;
	long		Affected;
	time_t		tvec;
	struct tm	*tm;
	char		Login_Date[12];

	CookieMember.xid = 0L;

	/*----------------------------------------------------------
		find member by (email or name) and password.
	----------------------------------------------------------*/
	snprintf ( WhereClause, sizeof(WhereClause), "(Memail = '%s' or Mname = '%s') and Mpassword = '%s'", 
				LoginEmail, LoginEmail, pw_sha_make_pw ( (unsigned char *) LoginPassword ) );

	if (( mysql_rv = LoadMember ( &MySql, WhereClause, &xmember, 0 )) < 1 )
	{
		SafeError ( COLOR_ERROR, "Unknown member or password" );
		rv = -1;
	}
	else 
	{
		/*---------------------------------------------------------
			check status
		----------------------------------------------------------*/
		switch ( xmember.xmstatus[0] )
		{
			case STATUS_PENDING:
				SafeError ( COLOR_ERROR, "%s, please verify your email before logging in!", xmember.xmname );
				break;

			case STATUS_VERIFIED:
				CookieMember.xid = xmember.xid;

				/*----------------------------------------------------------
					set login date
				----------------------------------------------------------*/
				tvec = time ( NULL );
				tm = localtime ( &tvec );

				sprintf ( Login_Date, "%4d-%02d-%02d", 
							tm->tm_year + 1900,
							tm->tm_mon + 1,
							tm->tm_mday );

				TrimRightAndLeft(xmember.xmlogdt);
				TrimRightAndLeft(xmember.xmipaddr);

// SafeError ( COLOR_ERROR, "[%s] [%s]", Login_Date,     xmember.xmlogdt  );
// SafeError ( COLOR_ERROR, "[%s] [%s]", Remote_Address, xmember.xmipaddr );


				if (( nsStrcmp ( Login_Date,     xmember.xmlogdt  ) != 0 ) ||
					( nsStrcmp ( Remote_Address, xmember.xmipaddr ) != 0 ))
				{
					snprintf ( Statement, sizeof(Statement),
						"update member set Mlogdt = '%s', Mipaddr = '%s' where member.id = %ld", 
							Login_Date,
							Remote_Address,
							CookieMember.xid );

					Affected = dbyUpdate ( "Login", &MySql, Statement, 0, LogFileName );
				}
				else
				{
					Affected = 1;
				}
				

				if ( Affected == 0 )
				{
					SafeError ( COLOR_ERROR, "Login failed." );
					rv = -3;
				}
				else
				{
					if ( xmember.xmrole[0] == ROLE_FREE )
					{
						RptlogRecordCount = GetReportCount ();
						if ( RptlogRecordCount >= FREE_MEMBER_REPORTS_LIMIT )
						{
							SafeError ( COLOR_WARN, "Hi %s. You have run %d reports.", 
								xmember.xmname, RptlogRecordCount );
							SafeError ( COLOR_WARN, "Free users are limited to %d reports per month.", FREE_MEMBER_REPORTS_LIMIT );
							SafeError ( COLOR_WARN, "Please upgrade in order to run unlimited reports." );
						}
						else
						{
							SafeError ( COLOR_NORMAL, "Hi %s. You have run %d reports.", 
								xmember.xmname, RptlogRecordCount );
						}
					}
					WriteSession ();
				}
				break;

			default:	
				SafeError ( COLOR_ERROR, "Unknown member status" );
				rv = -4;
				break;
		}
	}

	if ( rv == 0 )
	{
		MenuIndex = 1;
	}

	return ( rv );
}

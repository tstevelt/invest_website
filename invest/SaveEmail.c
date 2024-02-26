/*----------------------------------------------------------------------------
	Program : invest/SaveEmail.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Save new email address.
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

int SaveEmail ()
{
	int			Affected;
	int			rv = 0;
	char		fn[128];
	FILE		*fp;
	char		Link[256];
	time_t		tvec;

	/*----------------------------------------------------------
		check if email already on file
	----------------------------------------------------------*/
	sprintf ( WhereClause, "Memail = '%s'", SignupEmail );
	if ( LoadMember ( &MySql, WhereClause, &xmember, 0 ) >= 1 )
	{
		if ( CookieMember.xid == xmember.xid )
		{
			SafeError ( COLOR_ERROR, "Same email address -- nothing to do." );
		}
		else
		{
			SafeError ( COLOR_ERROR, "Email address is already on file." );
		}
		rv = -1;
		return ( rv );
	}

// SafeError ( COLOR_ERROR, "Not finished." );
// return  ( 0 );

	/*----------------------------------------------------------
		set new email and status back to pending
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"update member set Memail = '%s', Mstatus = '%c' where member.id = %ld", 
			SignupEmail, STATUS_PENDING, CookieMember.xid );

#ifdef DEBUG
#define DEBUG
	SafeError ( COLOR_ERROR, Statement );
#endif

	Affected = dbyUpdate ( "SaveEmail", &MySql, Statement, 0, LogFileName );
	if ( Affected == 0 )
	{
		SafeError ( COLOR_ERROR, "Change email failed." );
		rv = -2;
		return ( rv );
	}
	else
	{
		SafeError ( COLOR_SUCCESS, "Email changed." );
	}

	/*------------------------------------------------------------------
		do not need to reload member, because they will be logged out!
	------------------------------------------------------------------*/

	/*----------------------------------------------------------
		send verification email
	----------------------------------------------------------*/
	sprintf ( fn, "/var/local/tmp/member_%d.txt", getpid() );
	if (( fp = fopen ( fn, "w" )) == (FILE *)0 )
	{
		SafeError ( COLOR_ERROR, "Cannot create file to send email." );
	}
	else
	{
		tvec = time ( NULL );

		snprintf ( Link, sizeof(Link), "https://stocks.silverhammersoftware.com?verify=%ld&ipaddr=%s&time=%ld", CookieMember.xid, Remote_Address, tvec );

		fprintf ( fp, "<html>\n" );

		fprintf ( fp, "<head>\n" );
		fprintf ( fp, "<title>message</title>\n" );
		fprintf ( fp, "</head>\n" );

		fprintf ( fp, "<body color='#FAEBD7'>\n" );

		fprintf ( fp, "<p>\n" );

		fprintf ( fp, "Hello %s,\n", SignupName );
		fprintf ( fp, "<p>\n" );

		fprintf ( fp, "This email address was changed on %s.<br>\n", URL_NAME );
		fprintf ( fp, "If you did not change your email address, then please ignore this messeage.\n" );
		fprintf ( fp, "<p>\n" );

		fprintf ( fp, "Otherwise, we are asking that you verify your email addrsss<br>\n" );
		fprintf ( fp, "by clicking the link below (expires within one hour):\n" );
		fprintf ( fp, "<p>\n" );

		fprintf ( fp, "<a href='%s'>Verify %s</a>\n", Link, SignupEmail );
		fprintf ( fp, "<p>\n" );

		fprintf ( fp, "Thank you, Silver Hammer Software\n" );
		fprintf ( fp, "<p>\n" );

		fprintf ( fp, "Sign up from IP ADDRESS %s\n", Remote_Address );
		fprintf ( fp, "<p>\n" );

		fprintf ( fp, "</body>\n" );
		fprintf ( fp, "</html>\n" );

		nsFclose ( fp );

		SendAttached ( SUPPORT_ADDRESS, SignupEmail,
					"", "", 	"Email Verification from Silver Hammer Software",
						1, fn,
						0, NULL );
		
		SafeError ( COLOR_SUCCESS, "Verification email sent to %s. Please respond within one hour from the same device (IP address).", SignupEmail );
	}

	Logout ();

	return ( rv );
}

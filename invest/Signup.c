/*----------------------------------------------------------------------------
	Program : invest/Signup.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Save signup information. Notify admin.
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

int Signup ()
{
	int			Affected;
	int			rv = 0;
	char		fn[128];
	FILE		*fp;
	char		Link[256];
	time_t		tvec;

	/*----------------------------------------------------------
		insert member
	----------------------------------------------------------*/
	if ( nsStrlen ( RefPromoCode ) == PROMO_CODE_LENGTH )
	{
		snprintf ( Statement, sizeof(Statement),
			"insert into member ( Mname, Memail, Mpassword, Mipaddr, Mrefpromo ) values ( '%s', '%s', '%s', '%s', '%s' )",
				SignupName, SignupEmail, pw_sha_make_pw ( (unsigned char *)SignupPassword ), Remote_Address, RefPromoCode );
	}
	else
	{
		snprintf ( Statement, sizeof(Statement),
			"insert into member ( Mname, Memail, Mpassword, Mipaddr ) values ( '%s', '%s', '%s', '%s' )",
				SignupName, SignupEmail, pw_sha_make_pw ( (unsigned char *)SignupPassword ), Remote_Address );
	}

	Affected = dbyInsert ( "Signup", &MySql, Statement, 0, LogFileName );
	if ( Affected == 0 )
	{
		SafeError ( COLOR_ERROR, "1. Signup failed to insert new member record." );
		rv = -2;
	}
	else if (( xmember.xid = mysql_insert_id ( &MySql )) == 0L )
	{
		SafeError ( COLOR_ERROR, "2. Signup failed to insert new member record." );
		rv = -3;
	}
	else
	{
		/*----------------------------------------------------------
			send verification email
		----------------------------------------------------------*/
		sprintf ( fn, "/var/local/tmp/member_%d.txt", getpid() );
		if (( fp = fopen ( fn, "w" )) == (FILE *)0 )
		{
			SafeError ( COLOR_ERROR, "Cannot create file to send email." );
			rv = -4;
		}
		else
		{
			tvec = time ( NULL );

			snprintf ( Link, sizeof(Link), "https://stocks.silverhammersoftware.com?verify=%ld&ipaddr=%s&time=%ld", xmember.xid, Remote_Address, tvec );

			fprintf ( fp, "<html>\n" );

			fprintf ( fp, "<head>\n" );
			fprintf ( fp, "<title>Verify your email address</title>\n" );
			fprintf ( fp, "</head>\n" );

			fprintf ( fp, "<body color='#FAEBD7'>\n" );

			fprintf ( fp, "<p>\n" );

			fprintf ( fp, "Hello %s,\n", SignupName );
			fprintf ( fp, "<p>\n" );

			fprintf ( fp, "This email address was used to sign up on %s.<br>\n", URL_NAME );
			fprintf ( fp, "If you did not sign up, then please ignore this messeage.\n" );
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
			
			SafeError ( COLOR_SUCCESS, "Verification email sent to %s.", SignupEmail );
			SafeError ( COLOR_SUCCESS, "Please respond within one hour from the same device (IP address)." );

			sprintf ( Link, "Signup %ld %s", xmember.xid, SignupName );
			NotifyAdmin ( Link );
		}
	}

	return ( rv );
}

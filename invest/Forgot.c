/*----------------------------------------------------------------------------
	Program : invest/Forgot.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Forgot my password routine, send email.
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

int Forgot ()
{
	int			rv = 0;
	char		fn[128];
	FILE		*fp;
	char		Link[256];
	char		KeepThisSecret[128];
	time_t		tvec;
	int			Salt;

	/*----------------------------------------------------------
		get member
	----------------------------------------------------------*/
	sprintf ( WhereClause, "Memail = '%s'", ForgotEmail );
	if ( LoadMember ( &MySql, WhereClause, &xmember, 0 ) < 1 )
	{
		SafeError ( COLOR_ERROR, "Unknown email address." );
		rv = -1;
		return ( rv );
	}

	/*---------------------------------------------------------
		check status
	----------------------------------------------------------*/
	switch ( xmember.xmstatus[0] )
	{
		case STATUS_VERIFIED:
			sprintf ( fn, "/var/local/tmp/member_%d.txt", getpid() );
			if (( fp = fopen ( fn, "w" )) == (FILE *)0 )
			{
				SafeError ( COLOR_ERROR, "Cannot create file to send email." );
				break;
			}

			tvec = time ( NULL );
			srand ( time ( NULL ) );

#ifdef SEND_URL_IN_PLAINTEXT
			snprintf ( Link, sizeof(Link), "https://stocks.silverhammersoftware.com?forgot=%ld&ipaddr=%s&time=%ld", xmember.xid, Remote_Address, tvec );
#else
			do
			{
				Salt = rand ();
				Salt = Salt % 26;
			} while ( Salt < 0 || Salt > 25 );
			Salt = 'a' + Salt;

			snprintf ( KeepThisSecret, sizeof(KeepThisSecret), "%ld:%s:%ld", xmember.xid, Remote_Address, tvec );
			snprintf ( Link, sizeof(Link), "https://stocks.silverhammersoftware.com?forgot=%s", tmsEncode(Salt,KeepThisSecret) );
#endif

			fprintf ( fp, "<html>\n" );

			fprintf ( fp, "<head>\n" );
			fprintf ( fp, "<title>instructions</title>\n" );
			fprintf ( fp, "</head>\n" );

			fprintf ( fp, "<body color='#FAEBD7'>\n" );

			fprintf ( fp, "<p>\n" );

			fprintf ( fp, "Click the following URL to change your password on %s<br>\n", URL_NAME );
			fprintf ( fp, "This URL is valid for 1 hour.<br>\n" );
			fprintf ( fp, "<p>\n" );
			fprintf ( fp, "<a href='%s'>%s</a><br>\n", Link, Link );
			fprintf ( fp, "<p>\n" );
			fprintf ( fp, "If you did not initiate this request by clicking the 'Forgot my password'<br>\n" );
			fprintf ( fp, "button on %s, or you have remembered your password since then, you ", URL_NAME );
			fprintf ( fp, "should ignore this message.<br>\n" );
			fprintf ( fp, "<p>\n" );
			fprintf ( fp, "This request was initiated from IP Address %s", Remote_Address );


			fprintf ( fp, "</body>\n" );
			fprintf ( fp, "</html>\n" );

			nsFclose ( fp );

			SendAttached ( SUPPORT_ADDRESS, ForgotEmail,
						"", "", 	"Set password on Silver Hammer Software",
							1, fn,
							0, NULL );
			
			SafeError ( COLOR_ERROR, "New password instructions sent to %s", ForgotEmail );
			break;

		case STATUS_PENDING:
		default:	
			SafeError ( COLOR_ERROR, "Account has not been verified!" );
			rv = -4;
			break;
	}

	if ( rv == 0 )
	{
		MenuIndex = 1;
	}

	return ( rv );
}

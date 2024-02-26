/*----------------------------------------------------------------------------
	Program : invest/NotifyAdmin.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Send a message to administrators.
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

static	char		fn[128];

static	XMEMBER		xadmin;

static int EachAdmin ()
{

	SendAttached ( WEBMASTER_ADDRESS, xadmin.xmemail,
					"", "", 	"Notify Admin from Silver Hammer Software",
						1, fn,
						0, NULL );


	/*----------------------------------------------------------
		return -1 to break out and only notify the primary admin
		return 0 to notify all admins
	----------------------------------------------------------*/
	return ( -1 );
}

void NotifyAdmin ( char *Message )
{
	FILE		*fp;

	sprintf ( fn, "/var/local/tmp/notify_%d.txt", getpid() );

	if (( fp = fopen ( fn, "w" )) == (FILE *)0 )
	{
		fprintf ( stderr, "Cannot create file to send email.\n" );
	}
	else
	{
		fprintf ( fp, "<html>\n" );
		fprintf ( fp, "<head>\n" );
		fprintf ( fp, "<title>Notify Admin</title>\n" );
		fprintf ( fp, "</head>\n" );
		fprintf ( fp, "<body color='#FAEBD7'>\n" );
		fprintf ( fp, "<p>\n" );
		fprintf ( fp, "%s\n", Message );
		fprintf ( fp, "<p>\n" );
		fprintf ( fp, "</body>\n" );
		fprintf ( fp, "</html>\n" );
		nsFclose ( fp );

		sprintf ( WhereClause, "Mrole = '%c'", ROLE_ADMIN );
		LoadMemberCB ( &MySql, WhereClause, "id", &xadmin, (int(*)()) EachAdmin, 0 );
	}
}

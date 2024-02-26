/*----------------------------------------------------------------------------
	Program : invest/GetContents.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Content Management System
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

void GetContents ( int Index )
{
	char	fn[256];
	FILE	*fp;
	char	xbuffer[2048];
	char	*tokens[3];
	int		tokcnt;

	sprintf ( fn, "%s/contents.db", WEBSTATEDIR );

	if (( fp = fopen ( fn, "r" )) == (FILE *)0 )
	{
		printf ( "Cannot open %s<br>\n", fn );
		return;
	}

	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != (char *)0 )
	{
		TrimRightAndLeft ( xbuffer );

		if ( nsStrncmp ( xbuffer, "title", 5 ) == 0 )
		{
			if (( tokcnt = GetTokensW ( xbuffer, tokens, 2 )) < 2 )
			{
				printf ( "Syntax error in contents.db/title<br>\n" );
			}
			else
			{
				sprintf ( Title, "%s", tokens[1] );
			}
		}
		else if ( nsStrncmp ( xbuffer, "footer", 6 ) == 0 )
		{
			if (( tokcnt = GetTokensW ( xbuffer, tokens, 2 )) < 2 )
			{
				printf ( "Syntax error in contents.db/footer<br>\n" );
			}
			else
			{
				sprintf ( Footer, "%s", tokens[1] );
			}
		}
		else if ( nsStrncmp ( xbuffer, "menu", 4 ) == 0 )
		{
			if (( tokcnt = GetTokensW ( xbuffer, tokens, 3 )) < 3 )
			{
				printf ( "Syntax error in contents.db/menu<br>\n" );
			}
			else if ( MenuCount >= MAXPAGES )
			{
				printf ( "Exceeds MAXPAGES contents.db/menu<br>\n" );
			}
			else
			{
				MenuArray[++MenuCount].PageNumber = nsAtoi ( tokens[1] );
				snprintf ( MenuArray[MenuCount].String, MAX_SHORT_STRING, "%s", tokens[2] );
				// MenuCount++;
			}
		}
		else if ( nsStrncmp ( xbuffer, "page", 4 ) == 0 )
		{
			if (( tokcnt = GetTokensW ( xbuffer, tokens, 3 )) < 3 )
			{
				printf ( "Syntax error in contents.db/page<br>\n" );
			}
			else if ( PageCount >= MAXPAGES )
			{
				printf ( "Exceeds MAXPAGES contents.db/page<br>\n" );
			}
			else if ( MenuIndex == nsAtoi ( tokens[1] ) )
			{
				PageArray[PageCount].PageNumber = nsAtoi ( tokens[1] );
				TrimRight ( tokens[2] );
				snprintf ( PageArray[PageCount].String, MAX_SHORT_STRING, "%s", tokens[2] );
				PageCount++;
			}
		}
	}

	nsFclose ( fp );
}

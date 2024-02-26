/*----------------------------------------------------------------------------
	Program : invest/PaintScreen.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Content Management System.
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

#include	<errno.h>
#include	"invest.h"

void PaintScreen ()
{
	int		xi;
	char	fn[256];
	FILE	*fp;
	char	xbuffer[2048];
	char	*Glyph;
	char	*http_host;

// Includes over 250 glyphs in font format from the Glyphicon Halflings set. 
// Glyphicons Halflings are normally not available for free, but their creator has made them available for Bootstrap free of cost. 
// As a thank you, we only ask that you include a link back to Glyphicons whenever possible.
// li class='active' for the active option
	
	/*----------------------------------------------------------
		paint menu
	----------------------------------------------------------*/
	printf ( "<nav class='navbar navbar-inverse navbar-fixed-top'>\n" );
	printf ( "<div class='container-fluid'>\n" );

	printf ( "<div class='navbar-header'>\n" );
	printf ( "<button type='button' class='navbar-toggle' data-toggle='collapse' data-target='#myNavbar'>\n" );
	/*----------------------------
		hamburger!
	----------------------------*/
	printf ( "<span class='icon-bar'></span>\n" );
	printf ( "<span class='icon-bar'></span>\n" );
	printf ( "<span class='icon-bar'></span>\n" );
	printf ( "</button>\n" );

	http_host = getenv ( "HTTP_HOST" );

	if ( http_host != (char *)0 && nsStrncmp ( http_host, "dev.", 4 ) == 0 )
	{
		printf ( "<a class='navbar-brand' href='#'>Development</a>\n" );
	}
	else
	{
		printf ( "<a class='navbar-brand' href='#'>%s</a>\n", Title );
		// printf ( "<a class='navbar-brand' href='#'>%s %s</a>\n", Title, CookieMember.xid == 0 ? " not logged in" : xmember.xmname );
	}
	printf ( "</div>\n" );	// end navbar-header

	printf ( "<div class='collapse navbar-collapse' id='myNavbar'>\n" );

	printf ( "<ul class='nav navbar-nav'>\n" );

	for ( xi = 1; xi <= MenuCount; xi++ )
	{
		/*----------------------------------------------------------
			skip right side items for now
		----------------------------------------------------------*/
		if (( nsStrcmp ( MenuArray[xi].String, "Logout" ) == 0 ) ||
			( nsStrcmp ( MenuArray[xi].String, "Login"  ) == 0 ) ||
			( nsStrcmp ( MenuArray[xi].String, "Signup" ) == 0 ))
		{
			continue;
		}

		if ( CookieMember.xid == 0 )
		{
			if (( nsStrcmp ( MenuArray[xi].String, "Admin" ) == 0 ) ||
				( nsStrcmp ( MenuArray[xi].String, "Credits" ) == 0 ) ||
				( nsStrcmp ( MenuArray[xi].String, "Profile" ) == 0 ))
			{
				continue;
			}
		}
		else if ( xmember.xmrole[0] != ROLE_ADMIN && nsStrcmp ( MenuArray[xi].String, "Admin" ) == 0 )
		{
			continue;
		}
		
		switch ( RunMode )
		{
			case MODE_PAINT_EDIT_WATCHLIST:
//			case MODE_PAINT_IMPORT_WATCHLIST:
				printf ( "<li%s><a href='invest.cgi?MenuIndex=%d&AccordianID=%d'>%s</a></li>\n",
					MenuArray[xi].PageNumber  == MenuIndex ? " class='active'" : "",
					MenuArray[xi].PageNumber, 
					ACCORDIAN_WATCHLIST,
					MenuArray[xi].String );
				break;

			case MODE_PAINT_EDIT_PORTFOLIO:
				printf ( "<li%s><a href='invest.cgi?MenuIndex=%d&AccordianID=%d'>%s</a></li>\n",
					MenuArray[xi].PageNumber  == MenuIndex ? " class='active'" : "",
					MenuArray[xi].PageNumber, 
					ACCORDIAN_PORTFOLIO,
					MenuArray[xi].String );
				break;

			default:
				printf ( "<li%s><a href='invest.cgi?MenuIndex=%d'>%s</a></li>\n",
					MenuArray[xi].PageNumber  == MenuIndex ? " class='active'" : "",
					MenuArray[xi].PageNumber, 
					MenuArray[xi].String );
				break;
		}

	}

	printf ( "</ul>\n" );

	printf ( "<ul class='nav navbar-nav navbar-right'>\n" );

	if ( CookieMember.xid == 0 )
	{
		printf ( "<li class='visible-lg visible-md'><a href='#'>%s</a></li>", " not logged in" );
	}
	else
	{
		printf ( "<li class='visible-lg visible-md'><a href='#'>%s</a></li>", xmember.xmname );
	}

	for ( xi = 1; xi <= MenuCount; xi++ )
	{
		if ( CookieMember.xid == 0 )
		{
			if ( nsStrcmp ( MenuArray[xi].String, "Login"  ) == 0 )
			{
				Glyph = "glyphicon-log-in";
			}
			else if ( nsStrcmp ( MenuArray[xi].String, "Signup" ) == 0 )
			{
				Glyph = "glyphicon-user";
			}
			else
			{
				continue;
			}
		}
		else
		{
			if ( nsStrcmp ( MenuArray[xi].String, "Logout" ) != 0 )
			{
				continue;
			}
			else
			{
				Glyph = "glyphicon-log-out";
			}
		}

		printf ( "<li%s><a href='invest.cgi?MenuIndex=%d'><span class='glyphicon %s'></span>&nbsp;%s</a></li>\n",
			MenuArray[xi].PageNumber  == MenuIndex ? " class='active'" : "",
			MenuArray[xi].PageNumber, 
			Glyph,
			MenuArray[xi].String );
	}
	printf ( "</ul>\n" );
	printf ( "</div>\n" );	// end collapse ...

	printf ( "</div>\n" );	// end container-fluid for menu

	printf ( "</nav>\n" );

	/*----------------------------------------------------------
		paint stories
	----------------------------------------------------------*/
//	printf ( "<div class='container'>\n" );
	
	printf ( "<div class='topspacer'>&nbsp;</div>\n" );

	/*----------------------------------------------------------
		paint error / messages if any
	----------------------------------------------------------*/
	if ( ErrorCount > 0 )
	{
		int		DivColor;

		DivColor = ErrorColor ();

		switch ( DivColor )
		{
			case COLOR_ERROR:
				printf ( "<div class='error border-error'>\n" );
				break;
			case COLOR_WARN:
				printf ( "<div class='error border-warn'>\n" );
				break;
			case COLOR_SUCCESS:
				printf ( "<div class='error border-success'>\n" );
				break;
			case COLOR_NORMAL:
				printf ( "<div class='error border-black'>\n" );
				break;
		}
		for ( xi = 0; xi < ErrorCount; xi++ )
		{
			printf ( "%s<br>\n", ErrorArray[xi].Message );
		}
		printf ( "</div>\n" );
	}


	switch ( RunMode )
	{
		case MODE_PAINT_PASSWORD:
		case MODE_PAINT_EDIT_PORTFOLIO:
		case MODE_PAINT_EDIT_WATCHLIST:
		case MODE_PAINT_IMPORT_WATCHLIST:
		case MODE_PAINT_IMPORT_PORTFOLIO:
		case MODE_PAINT_EDIT_CASHBAL:
			PaintSpecial ( RunMode );
			break;

		default:
			for ( xi = 0; xi < PageCount; xi++ )
			{

				if ( nsStrncmp ( PageArray[xi].String, "sf=", 3 ) == 0 )
				{
					int		FunctionCode;
					FunctionCode = nsAtoi ( &PageArray[xi].String[3] );

					printf ( "<!-- start of special function %d -->\n", FunctionCode );

					PaintSpecial ( FunctionCode );

					printf ( "<!-- end of special function %d -->\n", FunctionCode );
				}
				else
				{
					printf ( "<div class='story'>\n" );

					printf ( "<!-- start contents of %s -->\n", PageArray[xi].String );
					snprintf ( fn, sizeof(fn), "%s/%s", WEBSTATEDIR, PageArray[xi].String );
					if (( fp = fopen ( fn, "r" )) == (FILE *)0 )
					{
#ifdef DEBUG
#define DEBUG
						printf ( "Cannot open %s, errno %d\n", fn, errno );
#else
						printf ( "Cannot open %s\n", PageArray[xi].String );
#endif
					}
					else
					{
						while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != (char *)0 )
						{
							printf ( "%s", xbuffer );
						}

						nsFclose ( fp );
					}
					printf ( "<!-- end of %s -->\n", PageArray[xi].String );

					printf ( "</div>\n" );
				}
			}

			/*----------------------------------------------------------
				stories cell end here 
			----------------------------------------------------------*/
			break;
	}

//	printf ( "</div>\n" );	// end container for stories



	printf ( "<div class='footer'>\n" );
	// printf ( "<a target='stevelt' href='https://www.stevelt.com'>%s</a>\n", Footer );
	printf ( "%s\n", Footer );
	printf ( "</div>\n" );
	printf ( "<br>\n" );
	printf ( "<br>\n" );
}

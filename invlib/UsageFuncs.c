/*----------------------------------------------------------------------------
	Program : invlib/UsageFuncs.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Print and Validation functions for stock index and report format
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

#include	"invprivate.h"
#include	"invlib.h"

void SindexUsage ( int IncludeMemberFeatures )
{
	printf ( "index:\n" );	
	printf ( " S = S&P 500\n" );	
	printf ( " D = Dow Jones Industrials\n" );	
	printf ( " N = Nasdaq 100\n" );	
	printf ( " M = Midcap (Russell 1000 ex S&P 500)\n" );	
	printf ( " 1 = Russell 1000\n" );	
	printf ( " 2 = Russell 2000\n" );	
	printf ( " 3 = Russell 3000\n" );	
	printf ( " A = All stocks\n" );	
	printf ( " O = Not indexed\n" );	
	if ( IncludeMemberFeatures )
	{
		printf ( " P = Portfolio\n" );		
		printf ( " W = Watchlist\n" );
		printf ( " Z = Temp file\n" );
	}
}

void ValidStockIndex ( char StockIndex, int IncludeMemberFeatures, int (*Usage)() )
{
	switch ( StockIndex )	
	{	
		case 'S':	
		case 'D':	
		case 'N':	
		case 'M':	
		case '1':	
		case '2':	
		case '3':	
		case 'A':	
		case 'O':	
			break;	
		case 'W':	
		case 'P':	
		case 'Z':	
			if ( IncludeMemberFeatures == 0 )
			{
				Usage ();	
			}
			break;
		default:	
			Usage ();	
			break;	
	}
}

void FormatUsage ( int IncludeWatchlist )
{
	if ( IncludeWatchlist == 0 )
	{
		printf ( " -fmt = csv (default), txt, html, xlsx or pdf (n/a)\n" );
	}
	else
	{
		printf ( " -fmt = csv (default), txt, html, xlsx, pdf (n/a), aw, rw or tmp\n" );
	}
}

char ValidReportFormatUsage ( char *FormatArg, int (*Usage)() )
{
	char	rv;

	if ( nsStrcmp ( FormatArg, "csv" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_CSV; 
	} 
	else if ( nsStrcmp ( FormatArg, "txt" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_TEXT; 
	} 
	else if ( nsStrcmp ( FormatArg, "html" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_HTML; 
	} 
	else if ( nsStrcmp ( FormatArg, "email" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_EMAIL; 
	} 
	else if ( nsStrcmp ( FormatArg, "aw" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_APPEND_W; 
	} 
	else if ( nsStrcmp ( FormatArg, "rw" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_REPLACE_W; 
	} 
	else if ( nsStrcmp ( FormatArg, "tmp" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_TEMP; 
	} 
	else if ( nsStrcmp ( FormatArg, "pdf" ) == 0 ) 
	{ 
		printf ( "Not available format\n" ); 
		Usage ();
		exit ( 1 );
	} 
	else if ( nsStrcmp ( FormatArg, "xlsx" ) == 0 ) 
	{ 
		rv = RPT_FORMAT_EXCEL; 
	} 
	else 
	{ 
		printf ( "Unknown format\n" ); 
		Usage ();
		exit ( 1 );
	} 

	return ( rv );
}

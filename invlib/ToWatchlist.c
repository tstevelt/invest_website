/*----------------------------------------------------------------------------
	Program : invlib/ToWatchlist.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Load a .csv file into the member's watchlist table.
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

static	XMEMBER		MyMember;

int ToWatchlist ( MYSQL *MySql, char *Filename, char *Delimiters, int TickerColumn, int ReportFormat, long MemberID, int PrintErrors )
{
	#define		MAXTOKS		100
	char	xbuffer[1024];
	char	Statement[1024];
	char	*tokens[MAXTOKS];
	int		tokcnt, rv, InsertCount;
	FILE	*fp;
	long	WatchlistRecordCount;
	
	sprintf ( Statement, "member.id = %ld", MemberID );
	if ( LoadMember ( MySql, Statement, &MyMember, 0 ) != 1 )
	{
		if ( PrintErrors )
		{
			printf ( "ToWatchlist: Unknown member id %ld\n", MemberID );
		}
		return ( -1 );
	}

	SetMemberLimits ( MyMember.xmrole[0] );

	InsertCount = 0;

	switch ( ReportFormat )
	{
		case RPT_FORMAT_REPLACE_W:
		case RPT_FORMAT_APPEND_W:
			if ( TickerColumn > MAXTOKS )
			{
				if ( PrintErrors )
				{
					printf ( "ToWatchlist: ticker column exceeds MAXTOKS\n" );
				}
				return ( -1 );
			}
			break;
		default:
			if ( PrintErrors )
			{
				printf ( "ToWatchlist: unknown report format\n" );
			}
			return ( -1 );
			break;
	}

	if ( ReportFormat == RPT_FORMAT_REPLACE_W )
	{
		sprintf ( xbuffer, "delete from watchlist where Wmember = %ld", MemberID );
		dbyDelete ( "invest", MySql, xbuffer, 0, LogFileName );
	}

	sprintf ( Statement, "Wmember = %ld", MemberID );
	WatchlistRecordCount = dbySelectCount ( MySql, "watchlist", Statement, LogFileName );

	if ( WatchlistRecordCount >= WatchlistLimit )
	{
		if ( PrintErrors )
		{
			printf ( "ToWatchlist: Watchlist is full\n" );
		}
		return ( 0 );
	}

	if (( fp = fopen ( Filename, "r" )) == (FILE *)0 )
	{
		if ( PrintErrors )
		{
			printf ( "ToWatchlist: Cannot open report file %s\n", Filename );
		}
		return ( -1 );
	}

	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != (char *)0 )
	{
		if (( tokcnt = GetTokensStd ( xbuffer, Delimiters, tokens, MAXTOKS, 0 )) < TickerColumn )
		{
			continue;
		}

		if ( ReportFormat == RPT_FORMAT_APPEND_W )
		{
			sprintf ( Statement, "Wmember = %ld and  Wticker = '%s'", MemberID, tokens[TickerColumn-1] );
			rv = dbySelectCount ( MySql, "watchlist", Statement, LogFileName );
			if ( rv > 0 )
			{
				if ( PrintErrors )
				{
					printf ( "%s is already in watchlist\n", tokens[TickerColumn-1] );
				}
				continue;
			}
		}

		if ( WatchlistRecordCount >= WatchlistLimit )
		{
			if ( PrintErrors )
			{
				printf ( "Watchlist is full, can not insert %s\n", tokens[TickerColumn-1] );
			}
			break;
		}
		else
		{
			sprintf ( Statement, "insert into watchlist (Wmember,Wticker) values ( %ld, '%s' )", MemberID, tokens[TickerColumn-1] );
			rv = dbyInsert ( "invest", MySql, Statement, 0, LogFileName );
			if ( rv != 1 )
			{
				if ( PrintErrors )
				{
					printf ( "Cannot insert %s into watchlist\n", tokens[TickerColumn-1] );
				}
				break;
			}
			else
			{
				InsertCount++;
				WatchlistRecordCount++;
			}
		}
	}

	nsFclose ( fp );

	return ( InsertCount );
}

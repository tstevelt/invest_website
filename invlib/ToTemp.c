/*----------------------------------------------------------------------------
	Program : invlib/ToTemp.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Load a .csv file into the member's temp table.
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

int ToTemp ( MYSQL *MySql, char *Filename, char *Delimiters, int TickerColumn, int ReportFormat, long MemberID, int PrintErrors )
{
	#define		MAXTOKS		100
	char	xbuffer[1024];
	char	Statement[1024];
	char	*tokens[MAXTOKS];
	int		tokcnt, rv, InsertCount;
	FILE	*fp;
	
	sprintf ( Statement, "member.id = %ld", MemberID );
	if ( LoadMember ( MySql, Statement, &MyMember, 0 ) != 1 )
	{
		if ( PrintErrors )
		{
			printf ( "ToTemp: Unknown member id %ld\n", MemberID );
		}
		return ( -1 );
	}

	InsertCount = 0;

	switch ( ReportFormat )
	{
		case RPT_FORMAT_TEMP:
			if ( TickerColumn > MAXTOKS )
			{
				if ( PrintErrors )
				{
					printf ( "ToTemp: ticker column exceeds MAXTOKS\n" );
				}
				return ( -1 );
			}
			break;
		default:
			if ( PrintErrors )
			{
				printf ( "ToTemp: unknown report format %d\n", ReportFormat );
			}
			return ( -1 );
			break;
	}

	sprintf ( xbuffer, "delete from temp where Tmember = %ld", MemberID );
	dbyDelete ( "invest", MySql, xbuffer, 0, LogFileName );

	if (( fp = fopen ( Filename, "r" )) == (FILE *)0 )
	{
		if ( PrintErrors )
		{
			printf ( "ToTemp: Cannot open report file %s\n", Filename );
		}
		return ( -1 );
	}

	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != (char *)0 )
	{
		if (( tokcnt = GetTokensStd ( xbuffer, Delimiters, tokens, MAXTOKS, 0 )) < TickerColumn )
		{
			continue;
		}

		sprintf ( Statement, "Tmember = %ld and  Tticker = '%s'", MemberID, tokens[TickerColumn-1] );
		rv = dbySelectCount ( MySql, "temp", Statement, LogFileName );
		if ( rv > 0 )
		{
			if ( PrintErrors )
			{
				printf ( "%s is already in temp\n", tokens[TickerColumn-1] );
			}
			continue;
		}

		sprintf ( Statement, "insert into temp (Tmember,Tticker) values ( %ld, '%s' )", MemberID, tokens[TickerColumn-1] );
		rv = dbyInsert ( "invest", MySql, Statement, 0, LogFileName );
		if ( rv != 1 )
		{
			if ( PrintErrors )
			{
				printf ( "Cannot insert %s into temp\n", tokens[TickerColumn-1] );
			}
			break;
		}
		else
		{
			InsertCount++;
		}
	}

	nsFclose ( fp );

	return ( InsertCount );
}

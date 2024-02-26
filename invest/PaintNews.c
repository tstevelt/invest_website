/*----------------------------------------------------------------------------
	Program : invest/PaintNews.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Load and paint newsfor stocks held in portfolio and watchlist.
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

static int EachStock ( DBY_QUERY *Query )
{
	char	FileName[256];
	FILE	*fp;
	char	xbuffer[4096];
	time_t	StoryTime;
	struct tm	*tm;
#ifdef USE_SUMMARY
	int		HasSummary;
#endif
	char	SaveSource[48];

	TrimRightAndLeft ( Query->EachRow[0] );
	TrimRightAndLeft ( Query->EachRow[1] );

	sprintf ( FileName, "%s/%s_news.txt", WEBSTATEDIR, Query->EachRow[0] );

	if (( fp = fopen ( FileName, "r" )) == NULL )
	{
		printf ( "<tr>\n" );
		printf ( "<td>\n" );
		printf ( "Cannot open %s", FileName );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );
	}
	else
	{
		SaveSource[0] = '\0';
		while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != NULL )
		{
			TrimRight ( xbuffer );

			if ( nsStrncmp ( xbuffer, "ticker ", 7 ) == 0 )
			{
				printf ( "<tr>\n" );
				printf ( "<td colspan='2'><b>%s", Query->EachRow[1] );
				printf ( " (%s)</b></td>\n", &xbuffer[7] );
			}
			else if ( nsStrncmp ( xbuffer, "datetime ", 9 ) == 0 )
			{
				// printf ( "<td>%s</td>\n", &xbuffer[9] );

				StoryTime = nsAtol ( &xbuffer[9] );
				tm = localtime ( &StoryTime );
				printf ( "<td>%s %d, 20%d %d:%02d</td>\n", 
					MonthOfYear ( tm->tm_mon ),
					tm->tm_mday,
					tm->tm_year % 100,
					tm->tm_hour,
					tm->tm_min );

			}
			else if ( nsStrncmp ( xbuffer, "source ", 7 ) == 0 )
			{
				printf ( "<td>Source: <i>%s</i></td>\n", &xbuffer[7] );
				printf ( "</tr>\n" );
				snprintf ( SaveSource, sizeof(SaveSource), "%s", &xbuffer[7] );
			}
		}

#ifdef USE_SUMMARY
		HasSummary = 0;
#endif

		rewind ( fp );
		while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != NULL )
		{
			TrimRight ( xbuffer );

			if ( nsStrncmp ( xbuffer, "summary ", 8 ) == 0 )
			{
				if ( nsStrncmp ( &xbuffer[8], "No summary available", 20 ) != 0 )
				{
					printf ( "<tr>\n" );
					// printf ( "<td>&nbsp;</td>\n" );
					printf ( "<td colspan='4'>%s</td>\n", &xbuffer[8] );
					printf ( "</tr>\n" );
#ifdef USE_SUMMARY
					HasSummary = 1;
#endif
				}
			}
		}

#ifdef USE_SUMMARY
		if ( HasSummary == 0 )
#endif
		{
			rewind ( fp );
			while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != NULL )
			{
				TrimRight ( xbuffer );

				if ( nsStrncmp ( xbuffer, "url ", 4 ) == 0 )
				{
					printf ( "<tr>\n" );

					// printf ( "<td colspan='4'>%s</td>\n", &xbuffer[4] );

					printf ( "<td>" );
					printf ( "<a target='NewsStory' href='%s'>Story on %s</a>", &xbuffer[4], SaveSource );
					printf ( "<td>\n" );

					printf ( "</tr>\n" );
				}
			}
		}

		nsFclose ( fp );
	}

	return ( 0 );
}

void PaintNews ()
{
	long	CutoffTime;

	time ( &CutoffTime );
	CutoffTime -= ( 7 * 24 * 60 * 60 );

	printf ( "<div class='story'>\n" );
	printf ( "<%s>Portfolio News</%s>\n", STORY_TITLE, STORY_TITLE );

	printf ( "<table class='table-condensed table-borderless'>\n" );

	sprintf ( Statement,
		"select Pticker, Sname from portfolio, stock where Sticker = Pticker and Pmember = %ld and Snews > %ld order by Snews desc",
				CookieMember.xid, CutoffTime );

	//printf ( "%s<br>\n", Statement );

	dbySelectCB ( "invest", &MySql, Statement, EachStock, LogFileName );

	printf ( "</table>\n" );
	printf ( "</div>\n" );

	printf ( "<div class='story'>\n" );
	printf ( "<%s>Watchlist News</%s>\n", STORY_TITLE, STORY_TITLE );

	printf ( "<table class='table-condensed table-borderless'>\n" );

	sprintf ( Statement,
		"select Wticker, Sname from watchlist, stock where Sticker = Wticker and Wmember = %ld and Snews > %ld order by Snews desc",
				CookieMember.xid, CutoffTime );

	//printf ( "%s<br>\n", Statement );

	dbySelectCB ( "invest", &MySql, Statement, EachStock, LogFileName );

	printf ( "</table>\n" );
	printf ( "</div>\n" );

}

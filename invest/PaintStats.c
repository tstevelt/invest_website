/*----------------------------------------------------------------------------
	Program : invest/PaintStats.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Get and display some system statistic on the About page.
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

static int EachStat ( DBY_QUERY *Query )
{
	static	int		Counter = 0;

	if ( Counter == 0 )
	{
		printf ( "<table class='table-condensed table-borderless'>\n" );
		printf ( "<tr>\n" );
		printf ( "<td colspan='5'>Benchmark statistics</tr>\n" );
		printf ( "</tr>\n" );
		printf ( "<tr>\n" );
		printf ( "<td align='center'>TICKER</td>\n" );
		printf ( "<td>NAME</td>\n" );
		printf ( "<td>START</td>\n" );
		printf ( "<td>LATEST</td>\n" );
		printf ( "<td align='right'>COUNT</td>\n" );
		printf ( "</tr>\n" );
	}

	printf ( "<tr>\n" );
	printf ( "<td align='center'>%s</td>\n", Query->EachRow[0] );
	printf ( "<td>%s</td>\n", Query->EachRow[1] );
	printf ( "<td>%s</td>\n", Query->EachRow[2] );
	printf ( "<td>%s</td>\n", Query->EachRow[3] );
	printf ( "<td align='right'>%s</td>\n", Query->EachRow[4] );
	printf ( "</tr>\n" );

	Counter++;

	return ( 0 );
}

void PaintStats ()
{
	long		rv;

	printf ( "<div class='story'>\n" );

	printf ( "<%s>System Statistics</%s>\n", STORY_TITLE, STORY_TITLE );

	if ( LoadSystem ( &MySql, &xsystem, 0 ) != 1 )
	{
		printf ( "Cannot load statistics" );
		return;
	}

	printf ( "Loaded %ld history records at %s<br>\n", xsystem.xystkcnt, xsystem.xystktime );
//	printf ( "Updated TTM dividends on %ld portfolio records at %s<br>\n", xsystem.xydivcnt, xsystem.xydivtime );

	snprintf ( Statement, sizeof(Statement),
"select Sticker, Sname, min(Hdate), max(Hdate), count(*) \
from stock, history \
where Sticker = Hticker \
and Sticker in ( 'SPY', 'DIA', 'QQQ', 'IWM', 'EFA' ) \
group by Sticker" );

	rv = dbySelectCB ( "invest", &MySql, Statement, EachStat, LogFileName );

	if ( rv )
	{
		printf ( "</table>\n" );
	}
	printf ( "</div>\n" );
}

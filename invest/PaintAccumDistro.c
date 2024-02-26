/*----------------------------------------------------------------------------
	Program : invest/PaintAccumDistro.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Display 10 days of A / D information on the home page.
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

typedef struct
{
	char	Date[12];
	int		Accumulate[2];
	int		Distribute[2];
} RECORD;

#define		MAXDAYS		10
static RECORD	Array[MAXDAYS];
static int Count = 0;
static int Days;

static char IndexArray [] = { 'S', 'N' };
static int	IndexCount = sizeof(IndexArray) / sizeof(char);
static int	IndexColumn;

static int EachMarket ()
{
	int		xd;

	for ( xd = 0; xd < Count; xd++ )
	{
		if ( nsStrcmp ( Array[xd].Date, xmarket.xmktdate ) == 0 )
		{
			break;
		}
	}
	if ( xd >= Count )
	{
		if ( Count >= MAXDAYS )
		{	
			printf ( "Something wrong<br>\n" );
			exit ( 0 );
		}
		nsStrcpy ( Array[Count].Date, xmarket.xmktdate );
		xd = Count;
		Count++;
	}
	Array[xd].Accumulate[IndexColumn] += xmarket.xmktaccum;
	Array[xd].Distribute[IndexColumn] += xmarket.xmktdist;

	Days++;
	if ( Days >= MAXDAYS )
	{
		return ( -1 );
	}

	return ( 0 );
}

void PaintAccumDistro ()
{
//	char	*Class;
	int		xd, xi;

	printf ( "<div class='story'>\n" );
	printf ( "<%s>Accumulate/Distribution Ratios</%s>\n", STORY_TITLE, STORY_TITLE );

	printf ( "Displays percentage from 0.0 to 100.0 of buying activity.<br>\n" );
	printf ( "This shows the strength of the market during the past 6 weeks.\n" );
	printf ( "<ul>\n" );
	printf ( "<li>A value of 100.0 means only strong buying days.</li>\n" );
	printf ( "<li>A value of   0.0 means nothing but selling days.</li>\n" );
	printf ( "<li>A value of  50.0 means equal number of buying and selling days.</li>\n" );
	printf ( "</ul>\n" );

	printf ( "<p>See the <b>Accumulate/Distribute History</b> on the <b>Sceeners</b> page for more detailed information.<br>\n" );

	PushSignup ();

	printf ( "<table width='350px' class='table-condensed table-borderless'>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>DATE</td>\n" );
	printf ( "<td>SP500</td>\n" );
	printf ( "<td>NASDAQ</td>\n" );
	printf ( "</tr>\n" );

	for ( IndexColumn = 0; IndexColumn < IndexCount; IndexColumn++ )
	{
		sprintf ( WhereClause, "MKTindex = '%c'", IndexArray[IndexColumn] );

		Days = 0;
		LoadMarketCB ( &MySql, WhereClause, "MKTdate desc", &xmarket, (int(*)()) EachMarket, 0 );
	}

	for ( xd = 0; xd < Count; xd++ )
	{
		printf ( "<tr>\n" );
		printf ( "<td>%s</td>\n", Array[xd].Date );

		for ( xi = 0; xi < IndexCount; xi++ )
		{
			printf ( "<td>%.2f</td>\n", 100.0 * AccumDistrFormula ( Array[xd].Accumulate[xi], Array[xd].Distribute[xi]) );
		}

		printf ( "</tr>\n" );
	}

	printf ( "</table>\n" );


	printf ( "</div>\n" );
}

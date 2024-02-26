/*----------------------------------------------------------------------------
	Program : invest/PaintStockDividends.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Display dividends for a security.
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

static int DividendCount;
static	DATEVAL		dvYearAgo;
static	char		strYearAgo[11];
static	double		DividendsTTM;

static int GotDividend ( DBY_QUERY *Query )
{
	static	double	ThisDividend, PreviousDividend, Difference, Percent;

	if ( DividendCount == 0 )
	{
		PreviousDividend = 0;
		printf ( "<table  class='table-condensed'>\n" );
		printf ( "<tr><td>Ex-Date</td><td align='right'>Dividend</td><td align='right'>Change</td><td align='right'>Percent</td></tr>\n" );
	}

	ThisDividend = nsAtof(Query->EachRow[1]);

	printf ( "<tr class='%s'>\n", DividendCount % 2 ? "report-even" : "report-odd" );

	printf ( "<td>%s</td><td align='right'>%.4f</td>", Query->EachRow[0], ThisDividend );
	
	Difference = ThisDividend - PreviousDividend;
	Percent = 100.0 * Difference / PreviousDividend;

/*---------------------------------------------------------------------------
.price_alert
{
	background-color: Coral;
}

.trailing_alert
{
	background-color: LightSalmon;
}

---------------------------------------------------------------------------*/
	if ( DividendCount == 0 || Difference == 0.0 )
	{
		printf ( "<td>&nbsp;</td>\n" );
	}
	else
	{
		// printf ( "<td %s align='right'>%.4f</td>\n", Difference < 0.0 ? "class='price_alert'" : "", Difference );
		printf ( "<td %s align='right'>%.4f</td>\n", Difference < 0.0 ? "class='trailing_alert'" : "", Difference );
		printf ( "<td %s align='right'>%.2f</td>\n", Difference < 0.0 ? "class='trailing_alert'" : "", Percent );
	}

	printf ( "</tr>\n" );

	if ( nsStrcmp ( Query->EachRow[0], strYearAgo ) > 0 )
	{
		DividendsTTM += ThisDividend;
	}

	DividendCount++;

	PreviousDividend = ThisDividend;
	return ( 0 );
}

void PaintStockDividends ( int RunMode, char *Ticker )
{
	double	Yield = 0.0;
	DividendCount = 0;
	DividendsTTM = 0.0;

	CurrentDateval ( &dvToday );
	DateAdd ( &dvToday, -365, &dvYearAgo );
	sprintf ( strYearAgo, "%04d-%02d-%02d", 
		dvYearAgo.year4, dvYearAgo.month, dvYearAgo.day );

	sprintf ( WhereClause, "Sticker = '%s'", Ticker );
	LoadStock ( &MySql, WhereClause, &xstock, 0, 0 );

	printf ( "<h3>Dividends for %s</h3><hr>\n" , xstock.xsname );

	/*----------------------------------------------------------
		+---------+------------------+------+-----+---------+----------------+
		| Field   | Type             | Null | Key | Default | Extra          |
		+---------+------------------+------+-----+---------+----------------+
		| id      | int(11) unsigned | NO   | PRI | NULL    | auto_increment |
		| Dticker | varchar(20)      | NO   | MUL |         |                |
		| Dexdate | date             | NO   |     | NULL    |                |
		| Damount | double           | NO   |     | 0       |                |
		| Dcurr   | char(4)          | NO   |     | USD     |                |
		+---------+------------------+------+-----+---------+----------------+
	----------------------------------------------------------*/
	sprintf ( Statement, 
		"select Dexdate, Damount from dividend where Dticker = '%s' order by Dexdate", Ticker );

	if ( dbySelectCB ( "invest", &MySql, Statement, (int(*)()) GotDividend, LogFileName ) < 1 )
	{
		printf ( "No dividends on file for %s<br>\n", Ticker );
	}
	else
	{
		Yield = 100.0 * DividendsTTM / xstock.xsclose;
		printf ( "<td>Close</td><td>%.4f</td></tr>\n", xstock.xsclose );
		printf ( "<td>Dividends TTM</td><td>%.4f</td></tr>\n", DividendsTTM );
		printf ( "<td>Current Yield</td><td>%.4f</td></tr>\n", Yield );
		
		printf ( "</table>\n" );
	}
}

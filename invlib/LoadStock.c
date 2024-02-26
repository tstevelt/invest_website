/*----------------------------------------------------------------------------
	Program : invlib/LoadStock.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Load this table
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

double	xstock_ttm_dividends;

static int GotDividend ( DBY_QUERY *Query )
{
	xstock_ttm_dividends = nsAtof(Query->EachRow[0]);

	return ( 0 );
}

static int Breakout ()
{
	return ( -1 );
}

int LoadStock ( MYSQL *MySql, char * WhereClause, XSTOCK *ptrStock, int PrintErrors, int CalcDividend )
{
	int		NumberOfRows = 0;
	char	Statement[1024];

	NumberOfRows = LoadStockCB ( MySql, WhereClause, NULL, ptrStock, Breakout, PrintErrors );

	xstock_ttm_dividends = 0.0;
	if ( CalcDividend )
	{
		sprintf ( Statement, 
				"select sum(Damount) from dividend where Dticker = '%s' and Dexdate >= date_sub( now(),interval 1 year)",
					ptrStock->xsticker );

		if ( dbySelectCB ( "invest", MySql, Statement, (int(*)()) GotDividend, LogFileName ) != 1 )
		{
			xstock_ttm_dividends = 0.0;
		}
	}

	return ( NumberOfRows );
}

/*----------------------------------------------------------------------------
	Program : invest/DeleteStocks.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Given global array (DeleteTickersArray) delete certain stock
				records.  Not if in portfolio or watchlist.  Delete child
				records.
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


void DeleteStocks ()
{
	int		xd, rv, ChildrenCount, StockCount;

	for ( xd = 0; xd < DeleteTickersCount; xd++ )
	{
		ChildrenCount = StockCount = 0;

		/*----------------------------------------------------------
			check portfolio
		----------------------------------------------------------*/
		sprintf ( WhereClause, "Pticker = '%s'", DeleteTickersArray[xd] );
		if (( rv = dbySelectCount ( &MySql, "portfolio", WhereClause, LogFileName )) > 0 )
		{
			SafeError ( COLOR_ERROR, "%s is in %d portfolio%s", 
					DeleteTickersArray[xd], rv, rv > 1 ? "s" : "" );
			continue;
		}

		/*----------------------------------------------------------
			check watchlist
		----------------------------------------------------------*/
		sprintf ( WhereClause, "Wticker = '%s'", DeleteTickersArray[xd] );
		if (( rv = dbySelectCount ( &MySql, "watchlist", WhereClause, LogFileName )) > 0 )
		{
			SafeError ( COLOR_ERROR, "%s is in %d watchlist%s", 
					DeleteTickersArray[xd], rv, rv > 1 ? "s" : "" );
			continue;
		}

		/*----------------------------------------------------------
			delete history
		----------------------------------------------------------*/
		sprintf ( Statement, "delete from history where Hticker = '%s'", DeleteTickersArray[xd] );
		ChildrenCount += dbyDelete ( "invest", &MySql, Statement, 0, LogFileName );

		/*----------------------------------------------------------
			delete average
		----------------------------------------------------------*/
		sprintf ( Statement, "delete from average where Aticker = '%s'", DeleteTickersArray[xd] );
		ChildrenCount += dbyDelete ( "invest", &MySql, Statement, 0, LogFileName );

		/*----------------------------------------------------------
			delete dividend
		----------------------------------------------------------*/
		sprintf ( Statement, "delete from dividend where Dticker = '%s'", DeleteTickersArray[xd] );
		ChildrenCount += dbyDelete ( "invest", &MySql, Statement, 0, LogFileName );

		/*----------------------------------------------------------
			delete fundamental
		----------------------------------------------------------*/
		sprintf ( Statement, "delete from fundamental where Fticker = '%s'", DeleteTickersArray[xd] );
		ChildrenCount += dbyDelete ( "invest", &MySql, Statement, 0, LogFileName );

		/*----------------------------------------------------------
			delete crypto
		----------------------------------------------------------*/
		sprintf ( Statement, "delete from crypto where Cticker = '%s'", DeleteTickersArray[xd] );
		ChildrenCount += dbyDelete ( "invest", &MySql, Statement, 0, LogFileName );

		/*----------------------------------------------------------
			delete temp
		----------------------------------------------------------*/
		sprintf ( Statement, "delete from temp where Tticker = '%s'", DeleteTickersArray[xd] );
		ChildrenCount += dbyDelete ( "invest", &MySql, Statement, 0, LogFileName );

		/*----------------------------------------------------------
			delete stock
		----------------------------------------------------------*/
		sprintf ( Statement, "delete from stock where Sticker = '%s'", DeleteTickersArray[xd] );
		StockCount = dbyDelete ( "invest", &MySql, Statement, 0, LogFileName );

		if ( StockCount != 1 )
		{
			SafeError ( COLOR_ERROR, "Could not delete %s, deleted %d child records", DeleteTickersArray[xd], ChildrenCount );
		}
	}
}

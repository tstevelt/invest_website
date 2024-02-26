/*----------------------------------------------------------------------------
	Program : invlib/LoadDividendSum.c
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

static	XDIVIDEND	MyDividend;
static	double	SumDividends;

static int EachDividend ()
{
	SumDividends += MyDividend.xdamount;
	
	return ( 0 );
}

double LoadDividendSum ( MYSQL *MySql, char *Ticker, char *CutoffDate )
{
	SumDividends = 0.0;

	sprintf ( WhereClause, "Dticker = '%s' and Dexdate >= '%s'", Ticker, CutoffDate );
	LoadDividendCB ( MySql, WhereClause, NULL, &MyDividend, (int(*)()) EachDividend, 0 );

	return ( SumDividends );
}

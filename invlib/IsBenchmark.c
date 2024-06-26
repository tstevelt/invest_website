/*----------------------------------------------------------------------------
	Program : invlib/IsBenchmark.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Identify Benchmark Index
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

/*---------------------------------------------------------------------------
	DJ Indus		tracked by DIA
	S&P 500			tracked by SPY (SPX is better, but data not available)
	Russell 2000	tracked by IWM
	Nasdaq 100   	tracked by QQQ
	MSCI EAFE		tracked by EFA
---------------------------------------------------------------------------*/

int IsBenchmark ( char *Ticker )
{
	if ( nsStrlen ( Ticker ) < 3 )
	{
		return ( 0 );
	}

	if ( nsStrstr  ( ":SPY:DIA:QQQ:IWM:EFA:", Ticker ) != (char *)0 )
	{
		return ( 1 );
	}

	return ( 0 );
}

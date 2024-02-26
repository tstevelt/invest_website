/*----------------------------------------------------------------------------
	Program : invlib/LoadHistoryArrays.c
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

void CopyHistoryArrays ()
{
	for ( int ndx = 0; ndx < HistoryCount; ndx++ )
	{
		HistoryOpenArray[ndx] = HistoryArray[ndx].Open;
		HistoryCloseArray[ndx] = HistoryArray[ndx].Close;
		HistoryHighArray[ndx] = HistoryArray[ndx].High;
		HistoryLowArray[ndx] = HistoryArray[ndx].Low;
		HistoryVolumeArray[ndx] = HistoryArray[ndx].Volume;
	}
}

int LoadHistoryArrays ( MYSQL *MySql, char *Ticker, char *LastDate, int OrderBy, int HowMany )
{
	LoadHistoryArray ( MySql, Ticker, LastDate, OrderBy, HowMany );

	CopyHistoryArrays ();

	return ( HistoryCount );
}

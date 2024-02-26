/*----------------------------------------------------------------------------
	Program : invlib/IndexName.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Given index code, return index name
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

char *IndexName ( char IndexCode )
{
	switch ( IndexCode )
	{
		case 'S':	return ( "S&P 500" );
		case 'D':	return ( "Dow Jones Industrials" );
		case 'N':	return ( "Nasdaq 100" );
		case 'M':	return ( "Midcap (Russell 1000 ex S&P 500)" );
		case '1':	return ( "Russell 1000" );
		case '2':	return ( "Russell 2000" );
		case '3':	return ( "Russell 3000" );
		case 'F':	return ( "Selected Stocks" );
		case 'W':	return ( "Watch List" );
		case 'P':	return ( "Portfolio" );
		case 'Z':	return ( "Temp File" );
		case 'A':	return ( "All stocks\n" );	
		case 'O':	return ( "Stocks not indexed\n" );	
	}
	return ( "Unknown" );
}

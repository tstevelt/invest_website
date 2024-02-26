/*----------------------------------------------------------------------------
	Program : invlib/ManyDecimalDouble.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Does a number in a string contains more than 2 decimal places?
	Return  : 1 = yes, 0 = no
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

int ManyDecimalDouble ( char *strValue )
{
	int		rv, xl, xi, xd, dc;

	rv = 1;

	for ( xi = 0, xd = 0, dc = 0, xl = nsStrlen(strValue); xi < xl && rv == 1; xi++ )
	{
		switch ( strValue[xi] )
		{
			case '-':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if ( xd )
				{
					dc++;
				}
				break;
			case '.':
				xd++;
				break;
			default:
				rv = 0;
				break;
		}
	}

	if ( xd == 1 && dc > 2 )
	{
		return ( 1 );
	}

	return ( 0 );
}

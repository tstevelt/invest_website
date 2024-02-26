/*----------------------------------------------------------------------------
	Program : invlib/PrintErrors.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: print ErrorArray to stderr.
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

void PrintErrors ()
{
	int		xe;

	if ( ErrorCount == 0 )
	{
		return;
	}

	for ( xe = 0; xe < ErrorCount; xe++ )
	{
		fprintf ( stderr, "%d: %s\n", xe + 1, ErrorArray[xe].Message );
	}

}

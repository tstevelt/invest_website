/*----------------------------------------------------------------------------
	Program : invlib/SetWhereClause.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Given Stock Index, start a where clause.  Calling program may
				add more restrictions.
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

void SetWhereClause ( char WhereClause[], char StockIndex )
{
	switch ( StockIndex )
	{
		case 'S':
			sprintf ( WhereClause, "Ssp500 = 'Y'" );
			break;
		case 'D':
			sprintf ( WhereClause, "Sdj = 'Y'" );
			break;
		case 'N':
			sprintf ( WhereClause, "Snasdaq = 'Y'" );
			break;
		case 'M':
			sprintf ( WhereClause, "Srussell = '1' and Ssp500 != 'Y'" );
			break;
		case '1':
		case '2':
			sprintf ( WhereClause, "Srussell = '%c'", StockIndex );
			break;
		case '3':
			sprintf ( WhereClause, "(Srussell = '1' or Srussell = '2')" );
			break;
		case 'A':
			sprintf ( WhereClause, "Slast is not NULL" );
			break;
		case 'O':
			sprintf ( WhereClause, "Slast is not NULL and Sdj != 'Y' and Ssp500 != 'Y' and Snasdaq != 'Y' and Srussell != '1' and Srussell != '2'" );
			break;
		case 'P':
		case 'W':
		case 'Z':
			sprintf  ( WhereClause, "not supported" );
			break;
	}
}

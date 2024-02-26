/*----------------------------------------------------------------------------
	Program : invlib/LoadIndustry.c
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

static int Breakout ()
{
	return ( -1 );
}

int LoadIndustry ( MYSQL *MySql, char * WhereClause, XINDUSTRY *ptrIndustry, int PrintErrors )
{
	int		NumberOfRows = 0;

	NumberOfRows = LoadIndustryCB ( MySql, WhereClause, NULL, ptrIndustry, Breakout, PrintErrors );

	return ( NumberOfRows );
}

/*----------------------------------------------------------------------------
	Program : invlib/CreateStock.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Create a default record in the stock table.
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

void CreateStock ( MYSQL *MySql, char *Ticker, char *Name, char StockType, char DomFor )
{
	char	Statement[128];

	sprintf ( Statement, 
			"insert into stock (Sticker,Sname,Stype,Sdomfor) values ( '%s', '%s', '%c', '%c' )", 
					Ticker, Name, StockType, DomFor );

	int rv = dbyInsert ( "invest", MySql, Statement, 0, LogFileName );
	if ( rv != 1 )
	{
		printf ( "Cannot insert %s into stock\n", Ticker );
		exit ( 1 );
	}

	return;
}

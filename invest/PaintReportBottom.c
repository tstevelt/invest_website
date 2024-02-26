/*----------------------------------------------------------------------------
	Program : invest/PaintReportBottom.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint the bottom of a report sent to the screen.
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

void PaintReportBottom ()
{
	printf ( "</div>\n" );

	printf ( "<div class='center'>\n" );

	/*--------------------------------------------------------------------------
		this is table instead of table-condensed to keep the button centered.
	--------------------------------------------------------------------------*/
	printf ( "<table class='table'>\n" );
	printf ( "<tr>\n" );
	printf ( "<td align='center'>\n" );
	printf ( "<input type='button' value='Close' onClick='javascript:window.close();'>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );
	printf ( "</table>\n" );
	printf ( "</div>\n" );

	webEndPage ();
}

/*----------------------------------------------------------------------------
	Program : invest/PaintContact.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint contact story on About page.
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

void PaintContact ()
{
	printf ( "<div class='story'>\n" );

	printf ( "<%s>Contact Us</%s>\n", STORY_TITLE, STORY_TITLE );
	printf ( "<p>\n" );
	printf ( "<table class='table-condensed table-borderless'>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>Subject</td>\n" );
	printf ( "<td colspan='2'>\n" );
	printf ( "<input class='resize' type='search' name='ContactSubject' size='60'>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>Message</td>\n" );
	printf ( "<td rowspan='5' colspan='2'><textarea name='ContactMessage' rows='7'></textarea></td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>&nbsp;</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>&nbsp;</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>&nbsp;</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>&nbsp;</td>\n" );
	printf ( "</tr>\n" );

	if ( CookieMember.xid == 0 )
	{
		printf ( "<tr>\n" );
		printf ( "<td colspan='2'><b>Please include your contact information if you want us to respond.</b></td>\n" );
		printf ( "</tr>\n" );
		printf ( "<tr>\n" );

		printf ( "<td>Your email </td>\n" );
		printf ( "<td>\n" );
		printf ( "<input class='resize' type='search' name='ContactEmail'>\n" );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );
	}

	printf ( "<tr>\n" );
	printf ( "<td>&nbsp;</td>\n" );
	printf ( "<td colspan='2'><input type='submit' name='submitContact' value='Send'></td>\n" );
	printf ( "</tr>\n" );
	printf ( "</table>\n" );

	printf ( "</div>\n" );
	printf ( "</div>\n" );
}

/*----------------------------------------------------------------------------
	Program : invest/PaintReportTop.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Initial HTML code for report sent to the screen.
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

void PaintReportTop ()
{
	WEBPARMS	*ptrWebParms = webInitParms();

	ptrWebParms->WP_Title = URL_NAME;
	webStartHead ( ptrWebParms );

	printf ( "<link rel='stylesheet' media='not screen' href='invest.css?version=%s'>\n", CSS_VERSION );
	printf ( "<link rel='stylesheet' media='screen and (min-width: 600px)' href='invest.css?version=%s'>\n", CSS_VERSION );
	printf ( "<link rel='stylesheet' media='screen and (max-width: 600px)' href='invest.css?version=%s'>\n", CSS_VERSION );

	webHeadToBody ( ptrWebParms );

	webFreeParms ( ptrWebParms );

	printf ( "<div class='story'>\n" );
}

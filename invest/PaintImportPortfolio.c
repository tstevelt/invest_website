/*----------------------------------------------------------------------------
	Program : invest/PaintImportPortfolio.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Import a .csv file into the portfolio.
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

void PaintImportPortfolio ()
{
	webStartJava ();
	printf ( "function RunImport ()\n" );
	printf ( "{\n" );
	printf ( "\tvar objForm;\n" );
	printf ( "\tobjForm = document.getElementById ( 'invest' );\n" );
	printf ( "\tobjForm.enctype='multipart/form-data';\n" );
	printf ( "\tobjForm.action='pupld.cgi';\n" );
	printf ( "\tobjForm.submit();\n" );
	printf ( "}\n" );
	webEndJava ();

	printf ( "<div class='story'>\n" );
	printf ( "<input type='hidden' name='MemberID' value='%ld'>\n", CookieMember.xid );
	printf ( "<table class='table-condensed table-borderless'>\n" );

	printf ( "<%s>Import Stocks to Portfolio</%s>\n", STORY_TITLE, STORY_TITLE );

	/*----------------------------
		row 1
	----------------------------*/
	printf ( "<tr>\n" );

	printf ( "<td>Replace or Append to Portfolio</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='ImportReplace'>\n" );
	printf ( "<option value='R'>Replace</option>\n" );
	printf ( "<option value='A'>Append</option>\n" );
	printf ( "</select>\n" );
	
	printf ( "</td>\n" );

	printf ( "</tr>\n" );

	/*----------------------------
		row 2
	----------------------------*/
	printf ( "<tr>\n" );

	printf ( "<td>Verbose</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='ImportVerbose'>\n" );
	printf ( "<option value='N'>No</option>\n" );
	printf ( "<option value='Y'>Yes</option>\n" );
	printf ( "</select>\n" );
	
	printf ( "</td>\n" );

	printf ( "</tr>\n" );

	/*----------------------------
		row 3
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Format</td>\n" );
	printf ( "<td>TICKER, DATE (yyyy-mm-dd), SHARES, PRICE</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		row 4
	----------------------------*/
	printf ( "<tr>\n" );

	printf ( "<td>Filename</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='file' name='ImportFileName'></td>\n" );

	printf ( "</tr>\n" );

	/*----------------------------
		row submit
	---------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>&nbsp;</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='button' id='SubmitImportPortfolio' name='SubmitImportPortfolio' value='Import' onclick='javascript:RunImport();'>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>\n" );
	printf ( "</div>\n" );
}

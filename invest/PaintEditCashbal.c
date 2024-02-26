/*----------------------------------------------------------------------------
	Program : invest/PaintEditCashbal.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Edit cash balance story at bottom of portfolio page.
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

void PaintEditCashbal ()
{
	printf ( "<div class='story'>\n" );
	printf ( "<input type='hidden' name='CashbalID' value='%ld'>\n", CashbalID );
	printf ( "<table class='table-condensed table-borderless'>\n" );

/*----------------------------------------------------------

		LoadCashbalCB ( &MySql, WhereClause, &xcashbal, 0 );
int LoadCashbalCB ( &MySql, char * WhereClause, char * OrderByClause, XCASHBAL *ptrCashacct, int (*CallBack)(), int PrintErrors )

----------------------------------------------------------*/
	if ( CashbalID > 0 )
	{
		sprintf ( WhereClause, "cashbal.id = %ld", CashbalID );
		LoadCashbalCB ( &MySql, WhereClause, "", &xcashbal, NULL, 0 );

		printf ( "<%s>Edit Cash Balance</%s>\n", STORY_TITLE, STORY_TITLE );
	}
	else
	{
		printf ( "<%s>Add Account </%s>\n", STORY_TITLE, STORY_TITLE );
	}

	/*----------------------------
		row 1
	----------------------------*/
	printf ( "<tr>\n" );

	printf ( "<td>Account</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='CBacct'" );
	if ( CashbalID > 0 )
	{
		printf ( " value='%s'", xcashbal.xcbacct );
	}
	printf ( "></td>\n" );

	printf ( "</tr>\n" );

	/*----------------------------
		row 2
	----------------------------*/
	printf ( "<tr>\n" );

	printf ( "<td>As-of Date</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='CBdate'" );
	if ( CashbalID > 0 )
	{
		printf ( " value='%s'", xcashbal.xcbdate );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		row 3
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Amount</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='CBamount'" );
	if ( CashbalID > 0 )
	{
		printf ( " value='%.2f'", xcashbal.xcbamount );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		row 4 
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Comment</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='CBcomment'" );
	if ( CashbalID > 0 )
	{
		printf ( " value='%s'", xcashbal.xcbcomment );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		row submit
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>&nbsp;</td>\n" );
	if ( CashbalID > 0 )
	{
		printf ( "<td>\n" );
		printf ( "<input type='submit' name='SubmitUpdateCashbal' value='Save'>\n" );
		printf ( "&emsp;" );
		printf ( "<input type='submit' name='SubmitDeleteCashbal' value='Remove'>\n" );
		printf ( "</td>\n" );
	}
	else
	{
		printf ( "<td>\n" );
		if ( CookieMember.xid == DEMO_MEMBER )
		{
			printf ( "<input type='button' value='Add Disabled'>\n" );
		}
		else
		{
			printf ( "<input type='submit' name='SubmitInsertCashbal' value='Add'>\n" );
		}
		printf ( "</td>\n" );
	}
	printf ( "</tr>\n" );

	printf ( "</table>\n" );
	printf ( "</div>\n" );
}

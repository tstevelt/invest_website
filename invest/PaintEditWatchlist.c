/*----------------------------------------------------------------------------
	Program : invest/PaintEditWatchlist.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Edit a watchlist entry.
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

static char AlertTypeArray [] =
{
	ALERT_NONE, ALERT_ABOVE_AMT, ALERT_BELOW_AMT,
	ALERT_ABOVE_MA5, ALERT_ABOVE_MA10, ALERT_ABOVE_MA20, ALERT_ABOVE_MA50, ALERT_ABOVE_MA100, ALERT_ABOVE_MA200,
	ALERT_BELOW_MA5, ALERT_BELOW_MA10, ALERT_BELOW_MA20, ALERT_BELOW_MA50, ALERT_BELOW_MA100, ALERT_BELOW_MA200,
	ALERT_MA5_DECREASE, ALERT_MA10_DECREASE, ALERT_MA20_DECREASE, ALERT_MA50_DECREASE, 
	ALERT_MA5_INCREASE, ALERT_MA10_INCREASE, ALERT_MA20_INCREASE, ALERT_MA50_INCREASE,
	'\0'
};


void PaintEditWatchlist ()
{
	int		xa;

	printf ( "<div class='story'>\n" );
	printf ( "<input type='hidden' name='WatchlistID' value='%ld'>\n", WatchlistID );
	printf ( "<table class='table-condensed table-borderless'>\n" );

	if ( WatchlistID > 0 )
	{
		sprintf ( WhereClause, "watchlist.id = %ld", WatchlistID );
		LoadWatchlist ( &MySql, WhereClause, &xwatchlist, 0 );

		sprintf ( WhereClause, "Sticker = '%s'", xwatchlist.xwticker );
		LoadStock ( &MySql, WhereClause, &xstock, 0, 0 );
		
		sprintf ( WhereClause, "Hticker = '%s' and Hdate = '%s'", xstock.xsticker, xstock.xslast );
		LoadHistory ( &MySql, WhereClause, &xhistory, 0 );
		
		printf ( "<%s>Edit Watchlist</%s>\n", STORY_TITLE, STORY_TITLE );
	}
	else
	{
		printf ( "<%s>Add Stock to Watchlist</%s>\n", STORY_TITLE, STORY_TITLE );
	}

	/*----------------------------
		row 1
	----------------------------*/
	printf ( "<tr>\n" );

	printf ( "<td>Ticker</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' id='Wticker' name='Wticker'" );
	if ( WatchlistID > 0 )
	{
		printf ( " value='%s'", xwatchlist.xwticker );
	}
	printf ( "></td>\n" );

	printf ( "</tr>\n" );

	/*----------------------------
		row 2
	----------------------------*/
	printf ( "<tr>\n" );

	printf ( "<td>Company</td>\n" );
	if ( WatchlistID > 0 )
	{
		printf ( "<td>%s</td>\n", xstock.xsname );
	}
	printf ( "</tr>\n" );

	/*----------------------------
		row 3 - yesterday close
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Yesterday Close</td>\n" );
	if ( WatchlistID > 0 )
	{
		printf ( "<td>%.2f</td>\n", xhistory.xhclose );
	}
	printf ( "</tr>\n" );

	/*----------------------------
		row 4 - alert type
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Alert Type</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='Walerttype'>" );
	for ( xa = 0; AlertTypeArray[xa]; xa++ )
	{
		printf ( "<option value='%c'%s>%s</option>\n",
			AlertTypeArray[xa], 
			xwatchlist.xwalerttype[0] == AlertTypeArray[xa] ? " selected" : "", 
			AlertDescription ( AlertTypeArray[xa] ) );
	}
	printf ( "</select>" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		row 5 - alert price
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>Alert Price</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='Walertprice'" );
	if ( WatchlistID > 0 )
	{
		printf ( " value='%.2f'", xwatchlist.xwalertprice );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------
		row submit
	----------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>&nbsp;</td>\n" );
	if ( WatchlistID > 0 )
	{
		printf ( "<td>\n" );
		printf ( "<input type='submit' name='SubmitUpdateWatchlist' value='Save'>\n" );
		printf ( "&emsp;" );
		printf ( "<input type='submit' name='SubmitDeleteWatchlist' value='Remove'>\n" );
		printf ( "&emsp;" );
		printf ( "<input type='submit' name='SubmitWatchlistChart' value='Chart' onclick='javascript:SetNamed(\"sheet\");'>\n" );

		printf ( "&emsp;<input type='button' id='SubmitStockFundFmt' value='Fundamentals'" );
		printf ( " onclick='javascript:ShowFundamentals(\"Wticker\",\"\");'>\n" );

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
			printf ( "<input type='submit' name='SubmitInsertWatchlist' value='Add'>\n" );
		}
		printf ( "</td>\n" );
	}
	printf ( "</tr>\n" );

	printf ( "</table>\n" );
	printf ( "</div>\n" );
}

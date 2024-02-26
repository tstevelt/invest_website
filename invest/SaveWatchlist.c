/*----------------------------------------------------------------------------
	Program : invest/SaveWatchlist.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Save new/changed watchlist record.
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

// static int DebugSaveWatchlist = 0;

static	XWATCHLIST	xtest;

void SaveWatchlist ( int RunMode )
{

	switch ( RunMode )
	{
		case MODE_UPDATE_WATCHLIST:
			sprintf ( WhereClause, "watchlist.id = %ld",  WatchlistID );
			LoadWatchlist ( &MySql, WhereClause, &xtest, 0 );
				
			if (( nsStrcmp ( xwatchlist.xwticker, xtest.xwticker ) != 0 ) ||
				( xwatchlist.xwalerttype[0]  != xtest.xwalerttype[0]  ) ||
				( xwatchlist.xwalertprice    != xtest.xwalertprice    ))
			{
				snprintf ( Statement, sizeof(Statement),
					"update watchlist set Wticker = '%s', Walerttype = '%c', Walertprice = %.2f where watchlist.id = %ld",
						 xwatchlist.xwticker, xwatchlist.xwalerttype[0], xwatchlist.xwalertprice, WatchlistID );

				if ( dbyUpdate ( "SaveWatchlist", &MySql, Statement, 0, LogFileName ) < 1 )
				{
					SafeError ( COLOR_ERROR, "SaveWatchlist update failed." );
				}
			}
			break;

		case MODE_DELETE_WATCHLIST:
			snprintf ( Statement, sizeof(Statement),
				"delete from watchlist where watchlist.id = %ld", WatchlistID );

			if ( dbyDelete ( "SaveWatchlist", &MySql, Statement, 0, LogFileName ) < 1 )
			{
				SafeError ( COLOR_ERROR, "SaveWatchlist delete failed." );
			}
			else
			{
				WatchlistID = 0;
			}
			break;

		case MODE_INSERT_WATCHLIST:
			snprintf ( Statement, sizeof(Statement),
				"insert into watchlist ( Wmember, Wticker, Walerttype, Walertprice ) values ( %ld, '%s', '%c', %.2f )",
					CookieMember.xid, xwatchlist.xwticker, xwatchlist.xwalerttype[0], xwatchlist.xwalertprice );

			if ( dbyInsert ( "SaveWatchlist", &MySql, Statement, 0, LogFileName ) < 1 )
			{
				SafeError ( COLOR_ERROR, "SaveWatchlist insert failed." );
				break;
			}

			SafeError ( COLOR_SUCCESS, "Added %s %s.", xwatchlist.xwticker, xstock.xsname );

			if ( WatchlistRecordCount + 1 < WatchlistLimit )
			{
				/*----------------------------
					add mode
				----------------------------*/
				WatchlistID = 0;
			}
			else
			{
				/*----------------------------
					edit mode
				----------------------------*/
				SafeError ( COLOR_ERROR, "Watchlist is full (%d)", WatchlistLimit );

				snprintf ( WhereClause, sizeof(WhereClause),
						"Wmember = %ld and Wticker = '%s'", CookieMember.xid, xwatchlist.xwticker );
				LoadWatchlist ( &MySql, WhereClause, &xtest, 0 );
				WatchlistID = xtest.xid;
			}

			break;
	}
}

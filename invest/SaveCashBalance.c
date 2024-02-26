/*----------------------------------------------------------------------------
	Program : invest/SaveCashBalance.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Save changes from cash balances screen
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

void SaveCashBalance ()
{
	switch ( RunMode )
	{
		case MODE_UPDATE_CASHBAL:
			sprintf ( Statement, 
				"update cashbal set CBacct = '%s', CBdate = '%s', CBamount = %.2f, CBcomment = '%s' where cashbal.id = %ld",
					xcashbal.xcbacct,
					xcashbal.xcbdate,
					xcashbal.xcbamount,
					xcashbal.xcbcomment,
					CashbalID );

			if ( dbyUpdate ( "invest", &MySql, Statement, 0, LogFileName ) < 1 )
			{
				SafeError ( COLOR_ERROR, "Save Account failed." );
				return;
			}

			SafeError ( COLOR_SUCCESS, "Saved cash balance account." );
			break;

		case MODE_INSERT_CASHBAL:
			sprintf ( Statement, "insert into cashbal (CBacct, CBdate, CBamount, CBmember, CBcomment ) \
				 values ( '%s', '%s', %.4f, %ld, '%s' )",
					xcashbal.xcbacct,
					xcashbal.xcbdate,
					xcashbal.xcbamount,
					CookieMember.xid,
					xcashbal.xcbcomment );

			if ( dbyInsert ( "invest", &MySql, Statement, 0, LogFileName ) < 1 )
			{
				SafeError ( COLOR_ERROR, "Insert account failed." );
				return;
			}

			SafeError ( COLOR_SUCCESS, "Added cash balance account." );
			break;

		case MODE_DELETE_CASHBAL:
			sprintf ( Statement, "delete from cashbal where cashbal.id = %ld", CashbalID );
			if ( dbyDelete ( "invest", &MySql, Statement, 0, LogFileName ) < 1 )
			{
				SafeError ( COLOR_ERROR, "Delete failed." );
				return;
			}
			SafeError ( COLOR_SUCCESS, "Deleted cash balance account." );
			break;

	}



}

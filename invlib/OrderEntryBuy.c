/*----------------------------------------------------------------------------
	Program : invlib/OrderEntryBuy.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Record purchase in portfolio and deduct from cash balance.
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

int OrderEntryBuy ( MYSQL *MySql, long MemberID, char Ticker[], char PurchDate[], double Shares, double Price )
{
	char	Statement[1024];

	sprintf ( Statement, "member.id = %ld", MemberID );
	if ( dbySelectCount ( MySql, "member", Statement, LogFileName ) != 1 )
	{
		return ( -1 );
	}

	sprintf ( Statement, "Sticker = '%s'", Ticker );
	if ( dbySelectCount ( MySql, "stock", Statement, LogFileName ) != 1 )
	{
		return ( -2 );
	}

	sprintf ( Statement, "Sticker = 'SPY'" );
	if ( dbySelectCount ( MySql, "stock", Statement, LogFileName ) != 1 )
	{
		return ( -3 );
	}

	sprintf ( Statement, "insert into portfolio (Pmember,Pticker,Pdate,Pshares,Pprice,Pbenchmark,Palerttype) \
			values ( %ld, '%s', '%s', %f, %f, 'SPY', '%c' )", MemberID, Ticker, PurchDate, Shares, Price , ALERT_NONE);

	if ( dbyInsert ( "invest", MySql, Statement, 0, LogFileName ) < 1 )
	{
		return ( -4 );
	}

	sprintf ( Statement, "insert into cashbal (CBacct, CBdate, CAshares, CBamount, CBmember, CAtype ) \
 values ( '%s', '%s', %.4f, %.4f, %ld, '%s' )",
			Ticker,
			fmtGetTodayDate ( DATEFMT_YYYY_MM_DD ),
			0.0-Shares,
			Price,
			MemberID,
			CATYPE_BUY_STOCK );
	
	if ( dbyInsert ( "invest", MySql, Statement, 0, LogFileName ) < 1 )
	{
		return ( -5 );
	}

	return ( 0 );
}

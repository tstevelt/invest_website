/*----------------------------------------------------------------------------
	Program : invlib/OrderEntrySell.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Record sale (deduct or delete) portfolio and increase cash.
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

#define		MAXSTOCKPORT		10
static double TotalShares;
static    XPORTFOLIO      PortfolioArray[MAXSTOCKPORT];
static    int             PortfolioCount;
static		char		Statement[1024];

static int ToPortfolioArray ()
{
	if ( PortfolioCount >= MAXSTOCKPORT )
	{
		fprintf ( stderr,  "Your portfolio contains more than %d positions of %s.", MAXSTOCKPORT, xportfolio.xpticker  );
		return ( -1 );
	}
	memcpy ( &PortfolioArray[PortfolioCount++], &xportfolio, sizeof(XPORTFOLIO) );
	TotalShares += xportfolio.xpshares;
	return ( 0 );
}

int OrderEntrySell ( MYSQL *MySql, long MemberID, char Ticker[], char PurchDate[], double Shares, double Price )
{
	int		xp;
	double	ProcessShares;

	sprintf ( Statement, "Sticker = '%s'", Ticker );
	if ( dbySelectCount ( MySql, "stock", Statement, LogFileName ) != 1 )
	{
		return ( -1 );
	}

	TotalShares = 0.0;
	PortfolioCount = 0;
	sprintf ( WhereClause, "Pticker = '%s' and Pmember = %ld", Ticker, MemberID);
	if ( LoadPortfolioCB ( MySql, WhereClause, "Pdate, id", &xportfolio, (int(*)()) ToPortfolioArray, 0 ) < 1 )
	{
		return ( -2 );
	}

	if ( TotalShares < Shares )
	{
		return ( -3 );;
	}

	ProcessShares = Shares;

	for ( xp = 0; xp < PortfolioCount && ProcessShares > 0.0; xp++ )
	{
		if ( PortfolioArray[xp].xpshares <= ProcessShares )
		{
			/*----------------------------------------------------------
				delete this portfolio record
			----------------------------------------------------------*/
			sprintf ( Statement, "delete from portfolio where portfolio.id = %ld", PortfolioArray[xp].xid );

			if ( dbyDelete ( "invest", MySql, Statement, 0, LogFileName ) != 1 )
			{
				return ( -4 );
			}

			ProcessShares -= PortfolioArray[xp].xpshares;
		}
		else
		{
			/*----------------------------------------------------------
				update Pshare in this portfolio record
			----------------------------------------------------------*/
			sprintf ( Statement, "update portfolio set Pshares = %.4f where portfolio.id = %ld", 
				PortfolioArray[xp].xpshares - ProcessShares, PortfolioArray[xp].xid );

			if ( dbyUpdate ( "invest", MySql, Statement, 0, LogFileName ) != 1 )
			{
				return ( -5 );
			}

			ProcessShares = 0.0;
		}
	}

	sprintf ( Statement, "insert into cashbal (CBacct, CBdate, CAshares, CBamount, CBmember, CAtype ) \
values ( '%s', '%s', %.4f, %.4f, %ld, '%s' )",
			Ticker,
			fmtGetTodayDate ( DATEFMT_YYYY_MM_DD ),
			Shares,
			Price,
			MemberID,
			CATYPE_SELL_STOCK );
	
	if ( dbyInsert ( "invest", MySql, Statement, 0, LogFileName ) < 1 )
	{
		return ( -6 );
	}

	return ( 0 );
}

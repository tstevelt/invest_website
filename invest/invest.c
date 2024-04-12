/*----------------------------------------------------------------------------
	Program : invest.c
	Author  : Tom Stevelt
	Date    : March 2019
	Synopsis: invest website app
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

#define		MAIN
#include	"invest.h"

int main ( int argc, char *argv[] )
{
	COOKIE_RECORD	MyCookie;
	int		xi, xt, ndx;
	long	xl;

	/*----------------------------------------------------------
		cookie stuff has to be written before html starts.
	----------------------------------------------------------*/
	/*----------------------------------------------------------
		get cookie
	----------------------------------------------------------*/
	if ( webParseCookie ( MEMBER_COOKIE_NAME, &MyCookie ) != 0 )
	{
		MyCookie.LoggedIn = 1;
		MyCookie.Counter  = 1;

		shs_seed_random ();

		for ( xt = 1; xt <= 100; xt++ )
		{
			for ( xi = 0; xi < 15; xi++ )
			{
				xl = random_range ( 65, 90 );
				SessionUnique[xi] = (char) xl;
			}

			snprintf ( SessionFileName, sizeof ( SessionFileName ), "%s/%15.15s.session", WEBSTATEDIR, SessionUnique );

			if ( access ( SessionFileName, F_OK ) == 0 )
			{
				SafeError ( COLOR_ERROR, "Session %s already on file.", SessionFileName );
			}
			else
			{
				break;
			}
		}

		sprintf ( MyCookie.UserName, "%-15.15s", SessionUnique );

		/*----------------------------------------------------------
			write cookie
		----------------------------------------------------------*/
		nsStrcpy  ( MyCookie.CookieName, MEMBER_COOKIE_NAME );
		MyCookie.LifeSeconds = MEMBER_COOKIE_LIFE_TIME;
		webPrintCookieRecord ( &MyCookie );
	}
	else
	{
		sprintf ( SessionUnique, "%s", MyCookie.UserName );
	}

	CurrentDateval ( &dvToday );

	sprintf ( Today, "%s", fmtGetTodayDate ( DATEFMT_YYYY_MM_DD ) );

	PaintTop ();

	GetInvestCfg ( 0 );

	StartMySQL ( &MySql, "invest" );

	GetInput ();

	ReadSession ();

	if ( CookieMember.xid > 0L )
	{
		sprintf ( WhereClause, "member.id = %ld", CookieMember.xid );
		LoadMember ( &MySql, WhereClause, &xmember, 1 );
	}

	ChkInput ();

	ndx = RunMode - SCREENER_AND_REPORT_OFFSET;
	TakeAction ( ReportFormat[ndx] );

	switch ( RunMode )
	{
		case MODE_REPORT_REGRESS:
		case MODE_REPORT_EQUITY_FUNDAMENTALS:
		case MODE_REPORT_ETF_FUNDAMENTALS:
		case MODE_REPORT_VOLUME_ACCUM:
		case MODE_REPORT_VOLUME_INCREASE:
		case MODE_REPORT_MA_CROSSOVER:
		case MODE_REPORT_MA_RATIO:
		case MODE_REPORT_DI_CROSSOVER:
		case MODE_REPORT_ADX_TREND:
		case MODE_REPORT_PORTFOLIO:
		case MODE_REPORT_WATCHLIST:
		case MODE_REPORT_AD_HISTORY:
		case MODE_REPORT_52_WEEK:
		case MODE_REPORT_BREAKOUT:
		case MODE_REPORT_SEC_IND_ALLOCATION:
		case MODE_REPORT_ASSET_ALLOCATION:
		case MODE_REPORT_RELATIVE_STRENGTH:
		case MODE_REPORT_CTB_STRENGTH:
		case MODE_REPORT_COOPER:
		case MODE_REPORT_VALUE:
			if ( ReportFormat[ndx] == RPT_FORMAT_HTML || ReportFormat[ndx] == RPT_FORMAT_PDF_VIEW )
			{
				exit ( 0 );
			}
			break;
		case MODE_PRINT_SHEET:
		case MODE_TV_CHART:
		case MODE_WATCHLIST_CHART:
		case MODE_PORTFOLIO_CHART:
		case MODE_COMBINED_CHART:
		case MODE_STOCK_CHART:
		case MODE_STOCK_DIVIDENDS:
		case MODE_PORTFOLIO_DIVIDENDS:
		case MODE_CHART_PREVIOUS:
		case MODE_CHART_NEXT:
		case MODE_STOCK_FUNDAMENTALS_RAW:
		case MODE_STOCK_FUNDAMENTALS_FMT:
		case MODE_STOCK_STATS_RAW:
			exit ( 0 );
	}

	printf ( "<input type='hidden' id='MenuIndex' name='MenuIndex' value='%d'>\n", MenuIndex );

	GetContents ( MenuIndex );

	PaintScreen ();

	PaintBottom ();
	
	/*----------------------------------------------------------
		close database
	----------------------------------------------------------*/
	dbyClose ( &MySql );

	return ( 0 );
}

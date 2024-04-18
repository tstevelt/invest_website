/*----------------------------------------------------------------------------
	Program : invest/ChkInput.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Validate user input.
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

static double TotalShares;
static int DebugChkInput = 0;

static int ToPortfolioArray ()
{
	if ( PortfolioCount >= MAXSTOCKPORT )
	{
		SafeError ( COLOR_ERROR, "Your portfolio contains more than %d positions of %s.", MAXSTOCKPORT, xportfolio.xpticker  );
		return ( -1 );
	}
	memcpy ( &PortfolioArray[PortfolioCount++], &xportfolio, sizeof(XPORTFOLIO) );
	TotalShares += xportfolio.xpshares;
	return ( 0 );
}

void ChkInput ()
{
	int		rv, ndx;
#ifdef ELECTRONIC_SIGNATURE
	int		xl;
#endif
	time_t	tvec;
	char	*DecodedString;
#define		MAXTOKS		10
	char	*tokens[MAXTOKS];
	int		tokcnt;
	char	*String_Address;
	long	StringTime;
	DATEVAL	dvTest;

	tvec = time ( NULL );

	SetMemberLimits ( xmember.xmrole[0] );

	if ( DebugChkInput )
	{
		char	message[128];
		sprintf ( message, "RunMode %d, CashT %.2f", RunMode, CashT );
		SafeError ( COLOR_ERROR, message );
	}

	switch ( RunMode )
	{
		case MODE_CHANGE_EMAIL:
			if (( nsStrlen ( SignupEmail ) < 5 ) || 
				( nsStrchr ( SignupEmail, '@' ) == (char *)0 ) || 
				( nsStrchr ( SignupEmail, '.' ) == (char *)0 ))
			{
				SafeError ( COLOR_ERROR, "Please enter valid email address." );
				RunMode = MODE_UNKNOWN;
			}
			break;

		case MODE_SAVE_PROFILE:
		case MODE_SIGNUP:
			if (( nsStrlen ( SignupName ) < 3 ) || 
				( nsStrchr ( SignupName, ' ' ) == (char *)0 ))
			{
				SafeError ( COLOR_ERROR, "Please enter both first and last name." );
				RunMode = MODE_UNKNOWN;
			}

			if ( RunMode == MODE_SIGNUP )
			{
				if (( nsStrlen ( SignupEmail ) < 5 ) || 
					( nsStrchr ( SignupEmail, '@' ) == (char *)0 ) || 
					( nsStrchr ( SignupEmail, '.' ) == (char *)0 ))
				{
					SafeError ( COLOR_ERROR, "Please enter valid email address." );
					RunMode = MODE_UNKNOWN;
				}
				else
				{
					/*----------------------------------------------------------
						check if name or email already on file
					----------------------------------------------------------*/
					sprintf ( WhereClause, "Memail = '%s'", SignupEmail );
					if ( LoadMember ( &MySql, WhereClause, &xmember, 0 ) >= 1 )
					{
						SafeError ( COLOR_ERROR, "Email address is already on file." );
						SafeError ( COLOR_ERROR, "You can login with your email address or your name." );
						SafeError ( COLOR_ERROR, "Use the 'Forgot' screen if you forgot your password." );
						RunMode = MODE_UNKNOWN;
					}
				}

				if ( nsStrcmp ( SignupPassword, ConfirmPassword ) != 0 )
				{
					SafeError ( COLOR_ERROR, "Passwords do not match." );
					RunMode = MODE_UNKNOWN;
				}

				if (( rv = pw_policy ( SignupPassword, 3 )) != 0 )
				{
					SafeError ( COLOR_ERROR, "%s", pw_policy_string ( rv ) );
					RunMode = MODE_UNKNOWN;
				}
			
				/*----------------------------------------------------------
					if entered promo code, check if valid
				----------------------------------------------------------*/
				if ( nsStrlen ( RefPromoCode ) == PROMO_CODE_LENGTH )
				{
					sprintf ( WhereClause, "Mmypromo = '%s'", RefPromoCode );
					if ( LoadMember ( &MySql, WhereClause, &xmember, 0 ) < 1 )
					{
						SafeError ( COLOR_ERROR, "Unknown Promotion Code." );
						RunMode = MODE_UNKNOWN;
					}
				}
			
				if ( SignupCookie == 0 )
				{
					SafeError ( COLOR_ERROR, "Sorry, this website won't work for you unless you allow cookies." );
					RunMode = MODE_UNKNOWN;
				}
if (( AcceptAllRisks == 0 ) || ( NotAFinancialAdvisor == 0 ) || ( DoFurtherResearch == 0 ) || ( TermsAndConditions == 0 ) )
				{
					SafeError ( COLOR_ERROR, "We are sorry you don't agree to the terms.  Please contact us if we can assist you." );
					RunMode = MODE_UNKNOWN;
				}
#ifdef ELECTRONIC_SIGNATURE
				xl = nsStrlen ( MySignature );
				if (( nsStrncmp ( MySignature, "/s/", 3 ) != 0 ) || ( nsStrncmp ( &MySignature[xl-1], "/", 1 ) != 0 ))
				{
					SafeError ( COLOR_ERROR, "Invalid electronic signature." );
					RunMode = MODE_UNKNOWN;
				}
				else if ( nsStrncmp ( &MySignature[3], SignupName, xl - 4 ) != 0 )
				{
					SafeError ( COLOR_ERROR, "Electronic signature does not match name." );
					RunMode = MODE_UNKNOWN;
				}
#endif
			}
			else if ( RunMode == MODE_SAVE_PROFILE )
			{
				double	Total = CashT + BondT + DomT + ForT;
				if ( Total != 100.0 )
				{
					SafeError ( COLOR_ERROR, "Allocation targets must total 100 percent" );
					RunMode = MODE_UNKNOWN;
				}
			}
			break;

		case MODE_CHANGE_PASSWORD:
			if ( nsStrcmp ( SignupPassword, ConfirmPassword ) != 0 )
			{
				SafeError ( COLOR_ERROR, "Passwords do not match." );
				RunMode = MODE_UNKNOWN;
			}

			if (( rv = pw_policy ( SignupPassword, 3 )) != 0 )
			{
				SafeError ( COLOR_ERROR, "%s", pw_policy_string ( rv ) );
				RunMode = MODE_UNKNOWN;
			}
			break;

		case MODE_VERIFY:
			// VerifyID
			// VerifyIPaddr
			if ( tvec - VerifyTime > 60 * 60 )
			{
				SafeError ( COLOR_ERROR, "Verification link is expired." );
				RunMode = MODE_UNKNOWN;
			}
			if ( nsStrcmp ( Remote_Address, VerifyIPaddr ) != 0 )
			{
				SafeError ( COLOR_ERROR, "Invalid IP address." );
				RunMode = MODE_UNKNOWN;
			}
			break;

		case MODE_LOGIN:
			break; 

		case MODE_FORGOT:
			if (( nsStrlen ( ForgotEmail ) < 5 ) || 
				( nsStrchr ( ForgotEmail, '@' ) == (char *)0 ) || 
				( nsStrchr ( ForgotEmail, '.' ) == (char *)0 ))
			{
				SafeError ( COLOR_ERROR, "Please enter valid email address." );
				RunMode = MODE_UNKNOWN;
			}
			break; 

		case MODE_LOGOUT:
//			if ( LogoutSelect != 'Y' )
//			{
//				SafeError ( COLOR_ERROR, "Pick Yes if you want to log out" );
//				RunMode = MODE_UNKNOWN;
//			}
			break; 

		case MODE_CONTACT:
			if ( nsStrlen ( ContactSubject ) < 4 )
			{
				SafeError ( COLOR_ERROR, "Please use a longer subject." );
				RunMode = MODE_UNKNOWN;
			}

			if ( nsStrlen ( ContactMessage ) < 5 )
			{
				SafeError ( COLOR_ERROR, "Please enter a longer message." );
				RunMode = MODE_UNKNOWN;
			}

			if ( nsStrlen ( ContactEmail ) > 1 )
			{
				if (( nsStrchr ( ContactEmail, '@' ) == (char *)0 ) || 
					( nsStrchr ( ContactEmail, '.' ) == (char *)0 ))
				{
					SafeError ( COLOR_ERROR, "Please enter valid email address." );
				}
			}

			break;

		case MODE_SEND_MESSAGE_TO_MEMBER:
			if ( FromMemberID != CookieMember.xid )
			{
				SafeError ( COLOR_ERROR, "Something fishy going on here. %ld != %ld", FromMemberID, CookieMember.xid );
				RunMode = MODE_UNKNOWN;
			}
			break;

		case MODE_PAINT_PASSWORD:
		case MODE_SAVE_PASSWORD:
			DecodedString = tmsDecode ( ForgotString );
			if ( nsStrcmp ( DecodedString, "hacker" ) == 0 )
			{
				printf ( "<%s>Hacking is a crime!</%s>\n", STRONG_MESSAGE, STRONG_MESSAGE );
				exit ( 0 );
			}

			if (( tokcnt = GetTokensD ( DecodedString, ":", tokens, MAXTOKS )) < 3 )
			{
				SafeError ( COLOR_ERROR, "Can not get tokens from %s", ForgotString );
				RunMode = MODE_UNKNOWN;
				return;
			}
			// xmember.xid, Remote_Address, tvec

			xmember.xid    = nsAtol ( tokens[0] );
			String_Address =        tokens[1];
			StringTime     = nsAtol ( tokens[2] );
#ifdef DEBUG
#define DEBUG
			SafeError ( COLOR_ERROR, "member %ld, ip addr %s time %ld", xmember.xid, String_Address, StringTime );
#endif
			if ( nsStrcmp ( String_Address, Remote_Address ) != 0 )
			{
				SafeError ( COLOR_ERROR, "Invalid IP address" );
				RunMode = MODE_UNKNOWN;
				return;
			}
			
			if ( tvec - StringTime > 60 * 60 )
			{
				SafeError ( COLOR_ERROR, "New password link is expired." );
				RunMode = MODE_UNKNOWN;
				return;
			}

			sprintf ( WhereClause, "member.id = %ld", xmember.xid );
			if ( LoadMember ( &MySql, WhereClause, &xmember, 0 ) < 1 )
			{
				SafeError ( COLOR_ERROR, "Unknown member." );
				RunMode = MODE_UNKNOWN;
				return;
			}

			break;

		case MODE_INSERT_WATCHLIST:
		case MODE_UPDATE_WATCHLIST:
			switch ( xwatchlist.xwalerttype[0] )
			{
				case ALERT_ABOVE_AMT:
				case ALERT_BELOW_AMT:
					if ( xwatchlist.xwalertprice < 0.01 )
					{
						SafeError ( COLOR_ERROR, "Missing alert amount" );
						RunMode = MODE_PAINT_PORTFOLIO_AND_WATCHLIST;
						return;
					}
					break;
			}
			sprintf ( WhereClause, "Wmember = %ld", CookieMember.xid );
			WatchlistRecordCount = dbySelectCount ( &MySql, "watchlist", WhereClause, LogFileName );
			if ( RunMode == MODE_INSERT_WATCHLIST && WatchlistRecordCount >= WatchlistLimit )
			{
				SafeError ( COLOR_ERROR, "Limited to %d securities in watchlist.", WatchlistLimit );
				RunMode = MODE_PAINT_PORTFOLIO_AND_WATCHLIST;
				break;
			}

			sprintf ( WhereClause, "Sticker = '%s'", xwatchlist.xwticker );
			if ( LoadStock ( &MySql, WhereClause, &xstock, 0, 0 ) < 1 )
			{
				SafeError ( COLOR_ERROR, "Unknown TICKER %s.", xwatchlist.xwticker );
				RunMode = MODE_PAINT_PORTFOLIO_AND_WATCHLIST;
				break;
			}

			sprintf ( WhereClause, "Wticker = '%s' and Wmember = %ld and watchlist.id != %ld", 
						xwatchlist.xwticker, CookieMember.xid, WatchlistID );
			WatchlistRecordCount = dbySelectCount ( &MySql, "watchlist", WhereClause, LogFileName );
#ifdef DENY_MULTIPLE_POSITIONS
			if ( WatchlistRecordCount >= 1 )
			{
				SafeError ( COLOR_ERROR, "TICKER %s is already in your watchlist.", xwatchlist.xwticker );
				RunMode = MODE_PAINT_PORTFOLIO_AND_WATCHLIST;
			}
#endif
			break;

		case MODE_INSERT_PORTFOLIO:
		case MODE_UPDATE_PORTFOLIO:
			sprintf ( WhereClause, "Pmember = %ld", CookieMember.xid );
			PortfolioRecordCount = dbySelectCount ( &MySql, "portfolio", WhereClause, LogFileName );
			if ( RunMode == MODE_INSERT_PORTFOLIO && PortfolioRecordCount >= PortfolioLimit )
			{
				SafeError ( COLOR_ERROR, 
					"Limited to %d securities in portfolio.", PortfolioLimit );
				RunMode = MODE_PAINT_PORTFOLIO_AND_WATCHLIST;
				return;
			}

#ifdef DENY_MULTIPLE_POSITIONS
	// no test here!
#endif
			sprintf ( WhereClause, "Sticker = '%s'", xportfolio.xpticker );
			if ( LoadStock ( &MySql, WhereClause, &xstock, 0, 0 ) < 1 )
			{
				SafeError ( COLOR_ERROR, "Unknown TICKER %s.", xportfolio.xpticker );
			}

			if ( StrToDatevalFmt ( xportfolio.xpdate, DATEFMT_YYYY_MM_DD, &dvTest ) != 0 )
			{
				SafeError ( COLOR_ERROR, "Invalid purchase date, use YYYY-MM-DD format." );
			}

			if ( xportfolio.xpshares < 0.01 )
			{
				SafeError ( COLOR_ERROR, "Missing number of shares." );
			}

			if ( xportfolio.xpprice < 0.01 )
			{
				SafeError ( COLOR_ERROR, "Missing cost." );
			}

			if ( nsStrlen ( xportfolio.xpoptexp) > 0 && StrToDatevalFmt ( xportfolio.xpoptexp, DATEFMT_YYYY_MM_DD, &dvTest ) != 0 )
			{
				SafeError ( COLOR_ERROR, "Invalid option expiration, use YYYY-MM-DD format." );
			}

			if ( ErrorCount )
			{
				RunMode = MODE_PAINT_EDIT_PORTFOLIO;
				break;
			}

			/*---------------------------------------------------------------
				sanity check on prices.  not an error unless invalid
				date, ie. no history on that day.
				price may be outside high and low, due to option execution.
			---------------------------------------------------------------*/
			sprintf ( WhereClause, "Hticker = '%s' and Hdate = '%s'", xportfolio.xpticker, xportfolio.xpdate );
			if ( LoadHistory ( &MySql, WhereClause, &xhistory, 0 ) < 1 )
			{
				SafeError ( COLOR_WARN, "No history found for %s on %s.", xportfolio.xpticker, xportfolio.xpdate );
			}
			else if ( xportfolio.xpprice < xhistory.xhlow || xportfolio.xpprice > xhistory.xhhigh )
			{
				SafeError ( COLOR_WARN,
					"%s traded between %.2f and %.2f on %s.", xportfolio.xpticker, xhistory.xhlow, xhistory.xhhigh, xportfolio.xpdate );
			}

			switch ( xportfolio.xpalerttype[0] )
			{
				case ALERT_ABOVE_AMT:
				case ALERT_BELOW_AMT:
					if ( xportfolio.xpalertprice <= 0.0 )
					{
						xportfolio.xpalertprice = 0.0 - xportfolio.xpalertprice;
						SafeError ( COLOR_ERROR, "Stop Percent should be positive." );
					}
					break;

				case ALERT_BENCHMARK:
					sprintf ( WhereClause, "Hticker = '%s' and Hdate = '%s'", xportfolio.xpbenchmark, xportfolio.xpdate );
					if ( LoadHistory ( &MySql, WhereClause, &xhistory, 0 ) < 1 )
					{
						SafeError ( COLOR_ERROR, "No history found for BENCHMARK on %s.", xportfolio.xpdate );
					}
					break;

				case ALERT_TRAIL_STOP:
					if ( xportfolio.xptrailpct <= 0.0 )
					{
						xportfolio.xptrailpct = 0.0 - xportfolio.xptrailpct;
						SafeError ( COLOR_ERROR, "Trailing Stop Percent should be positive." );
					}
					break;
			}

			break;

		case MODE_REPORT_EQUITY_FUNDAMENTALS:
			if ( nsStrlen ( StockIndexList ) == 0 )
			{
				SafeError ( COLOR_ERROR, "Please pick at least one index" );
				break;
			}
			if ( ScreenerMinMaxCount == 0 )
			{
				SafeError ( COLOR_ERROR, "No criteria entered" );
				break;
			}
			/* FALL-THROUGH */
		case MODE_REPORT_ETF_FUNDAMENTALS:
		case MODE_REPORT_PORTFOLIO:
		case MODE_REPORT_WATCHLIST:
		case MODE_REPORT_REGRESS:
		case MODE_REPORT_VOLUME_ACCUM:
		case MODE_REPORT_VOLUME_INCREASE:
		case MODE_REPORT_MA_CROSSOVER:
		case MODE_REPORT_MA_RATIO:
		case MODE_REPORT_DI_CROSSOVER:
		case MODE_REPORT_ADX_TREND:
		case MODE_REPORT_AD_HISTORY:
		case MODE_REPORT_52_WEEK:
		case MODE_REPORT_BREAKOUT:
		case MODE_REPORT_SEC_IND_ALLOCATION:
		case MODE_REPORT_ASSET_ALLOCATION:
		case MODE_REPORT_RELATIVE_STRENGTH:
		case MODE_REPORT_COOPER:
		case MODE_REPORT_VALUE:
			ndx = RunMode - SCREENER_AND_REPORT_OFFSET;
			if ( RunMode == MODE_REPORT_PORTFOLIO )
			{
				switch ( SortBy[ndx] )
				{
					case SORTBY_TICKER:
						break;
					case SORTBY_RETURN:
						switch ( ReportStyle[ndx] )
						{
							case STYLE_FUNDAMENTAL:
								SafeError ( COLOR_ERROR, "These reports do not contain GAIN column." );
								RunMode = MODE_UNKNOWN;
								break;
						}
						break;
					case SORTBY_GAIN:
						switch ( ReportStyle[ndx] )
						{
							case STYLE_ALERTS:
							case STYLE_FUNDAMENTAL:
								SafeError ( COLOR_ERROR, "These reports do not contain GAIN column." );
								RunMode = MODE_UNKNOWN;
								break;
						}
						break;
					case SORTBY_COMPARED:
						switch ( ReportStyle[ndx] )
						{
							case STYLE_BASE:
							case STYLE_TODAY:
							case STYLE_FULL:
							case STYLE_FUNDAMENTAL:
								SafeError ( COLOR_ERROR, "These reports do not contain COMPARED column." );
								RunMode = MODE_UNKNOWN;
								break;
						}
						break;
				}


				if ( RunMode == MODE_UNKNOWN )
				{
					break;
				}
			}
			if ( ReportFormat[ndx] == '?' )
			{
				SafeError ( COLOR_ERROR, "Please select a report format/destination." );
				RunMode = MODE_UNKNOWN;
				break;
			}
			if ( ReportLimit != PAID_UNLIMITED )
			{
				RptlogRecordCount = GetReportCount ();
				if ( RptlogRecordCount >= ReportLimit )
				{
					if ( ReportFormat[ndx] == 'H' )
					{
						printf ( "Limited to %d reports per month.<br>\n", ReportLimit );
					}
					else
					{
						SafeError ( COLOR_ERROR, "Limited to %d reports per month.", ReportLimit );
					}
					RunMode = MODE_UNKNOWN;
					break;
				}
			}
			break;
		
		case MODE_FIND_STOCK:
			if ( lastchr ( MaintStock.xsticker, sizeof(MaintStock.xsticker) ) < 1 )
			{
				SafeError ( COLOR_ERROR, "Missing TICKER" );
				RunMode = MODE_UNKNOWN;
			}
			break;

		case MODE_INSERT_STOCK:
			if ( lastchr ( MaintStock.xsticker, sizeof(MaintStock.xsticker) ) < 1 )
			{
				SafeError ( COLOR_ERROR, "Missing TICKER" );
				RunMode = MODE_UNKNOWN;
			}
			if ( lastchr ( MaintStock.xsname, sizeof(MaintStock.xsname) ) < 3 )
			{
				SafeError ( COLOR_ERROR, "Missing COMPANY NAME" );
				RunMode = MODE_UNKNOWN;
			}
			sprintf ( WhereClause, "Sticker = '%s'", MaintStock.xsticker );
			if ( LoadStock ( &MySql, WhereClause, &MaintStock, 0, 0 ) == 1 )
			{
				SafeError ( COLOR_ERROR, "%s is already on file.", MaintStock.xsticker );
				RunMode = MODE_FIND_STOCK;
			}
			break;

		case MODE_ORDER_ENTRY_BUY:
		case MODE_CONFIRM_BUY:
			if ( StrToDatevalFmt ( OrderEntryDate, DATEFMT_YYYY_MM_DD, &dvTest ) != 0 )
			{
				SafeError ( COLOR_ERROR, "Invalid date, use YYYY-MM-DD format." );
				RunMode = MODE_ORDER_ENTRY_PAINT;
			}
			sprintf ( WhereClause, "Sticker = '%s'", OrderEntryTicker );
			if ( LoadStock ( &MySql, WhereClause, &OrderEntryStock, 0, 0 ) < 1 )
			{
				SafeError ( COLOR_ERROR, "Unknown TICKER %s.", OrderEntryTicker );
				RunMode = MODE_ORDER_ENTRY_PAINT;
				break;
			}
			break;

		case MODE_ORDER_ENTRY_SELL:
		case MODE_CONFIRM_SELL:
		case MODE_ORDER_ENTRY_DRIP:
		case MODE_CONFIRM_DRIP:
			if ( StrToDatevalFmt ( OrderEntryDate, DATEFMT_YYYY_MM_DD, &dvTest ) != 0 )
			{
				SafeError ( COLOR_ERROR, "Invalid date, use YYYY-MM-DD format." );
				RunMode = MODE_ORDER_ENTRY_PAINT;
			}
			TotalShares = 0.0;
			PortfolioCount = 0;
			sprintf ( WhereClause, "Pticker = '%s' and Pmember = %ld", OrderEntryTicker, CookieMember.xid );
			if ( LoadPortfolioCB ( &MySql, WhereClause, "Pdate, id", &xportfolio, (int(*)()) ToPortfolioArray, 0 ) < 1 )
			{
				SafeError ( COLOR_ERROR, "You do not own %s", OrderEntryTicker );
				RunMode = MODE_ORDER_ENTRY_PAINT;
				break;
			}
			if ( RunMode == MODE_ORDER_ENTRY_DRIP || RunMode == MODE_CONFIRM_DRIP )
			{
				if ( PortfolioCount > 1 )
				{
					SafeError ( COLOR_ERROR, "You have more than one %s in your portfolio", OrderEntryTicker );
					RunMode = MODE_ORDER_ENTRY_PAINT;
					break;
				}
				break;
			}
			sprintf ( WhereClause, "Sticker = '%s'", OrderEntryTicker );
			LoadStock ( &MySql, WhereClause, &OrderEntryStock, 0, 0 );
			if ( TotalShares < OrderEntryShares )
			{
				SafeError ( COLOR_ERROR, "You only own %.4f shares of this security", TotalShares );
				RunMode = MODE_ORDER_ENTRY_PAINT;
				break;
			}
			break;

		case MODE_ORDER_ENTRY_CLEAR:
			break;

	}
}

/*----------------------------------------------------------------------------
	Program : invest/PaintHome.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint the Home page.
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

#define DEBUG
#undef DEBUG

void PaintHome ()
{
	printf ( "<div class='story'>\n" );

	printf ( "<%s>Features</%s>\n", STORY_TITLE, STORY_TITLE );

	printf ( "<p>\n" );

	if ( CookieMember.xid == 0 )
	{
		printf ( "<div class='error border-error'>\n" );
		printf ( "DEMONSTRATION account available!<br>\n" );
		printf ( "&emsp;Login: demo<br>\n" ); 
		printf ( "&emsp;Password: demo<br>\n" ); 
		printf ( "</div>\n" );
	}
	else if ( CookieMember.xid == DEMO_MEMBER )
	{
		printf ( "<div class='error border-error'>\n" );
		printf ( "This is a DEMONSTRATION account.\n" );
		printf ( "<ul>\n" );
		printf ( "<li>Some features are disabled.</li>\n" );
		printf ( "<li>Some numbers are 'munged', do not use any number you see here for making investment decisions!</li>\n" );
		// printf ( "<li>Company names are also 'munged', to remind you that the numbers on the report are not true.</li>\n" );
		printf ( "<li>Since 'demo' does not have an email address, CSV, TEXT, Excel reports are not available..</li>\n" );
		printf ( "</ul>\n" );
		printf ( "</div>\n" );
	}

	printf ( "<p>\n" );

	/*----------------------------------------------------------
		anonymous user
	----------------------------------------------------------*/
	printf ( "<p>\n" );
	if ( CookieMember.xid == 0 )
	{
		printf ( "<span style='background: yellow;'>Anonymous user</span>\n" );
	}
	else
	{
		printf ( "Anonymous user\n" );
	}
	printf ( "<ul>\n" );
	printf ( "<li>Accumulate/Distribution Ratios - See below - Past 10 days. FREE, no sign-in needed.</li>\n" );
	printf ( "<li>Market Moving Averages - Market Segments Temperature Chart, on the Screeners page. Shows 6 moving averages for 16 benchmarks.</li>\n" );
	printf ( "</ul>\n" );


	/*----------------------------------------------------------
		registered member
	----------------------------------------------------------*/
	printf ( "<p>\n" );
	if ( CookieMember.xid == 0 )
	{
		printf ( "<span style='background: yellow;'>Registered member - FREE</span>\n" );
	}
	else
	{
		printf ( "Registered member - FREE\n" );
	}
	printf ( "<ul>\n" );
	printf ( "<li>No credit card required! </li>\n" );
	printf ( "<li>Privacy guaranteed.  Email address used only to deliver reports to your inbox.</li>\n" );
	printf ( "<li>We will never share you email address with anyone.</li>\n" );
	printf ( "<li>No ads. Other sites bombard free users with ads.  We don't.</li>\n" );
	printf ( "<li>All the same features as a paid user, so you can evaluate our service.</li>\n" );
	printf ( "<li>Limits: %d securities in portfolio and watchlist.</li>\n", FREE_MEMBER_PORTFOLIO_LIMIT );
	printf ( "<li>Reports: %d reports past 30 days.</li>\n", FREE_MEMBER_REPORTS_LIMIT );
	printf ( "</ul>\n" );

	/*----------------------------------------------------------
		paid subscribder
	----------------------------------------------------------*/
	printf ( "<p>\n" );
	if ( CookieMember.xid == 0 )
	{
		printf ( "<span style='background: yellow;'>Paid subscriber</span>\n" );
	}
	else
	{
		printf ( "Paid subscriber\n" );
	}
	printf ( "<ul>\n" );
	printf ( "<li>Maximum of %d securities in portfolio</li>\n", PAID_MEMBER_PORTFOLIO_LIMIT );
	printf ( "<li>Maximum of %d securities in watchlist</li>\n", PAID_MEMBER_WATCHLIST_LIMIT );
	printf ( "<li>Portfolio and WatchList Alerts sent daily based on your setup for each stock</li>\n" );
	if ( CookieMember.xid == 0 )
	{
		printf ( "<li>Introductory price only $%d/year.</li>\n", INTRODUCTORY_PRICE );
	}
	printf ( "<li>Unlimited referral fees. You can earn more than the cost of a subscription!</li>\n" );
	printf ( "</ul>\n" );

	if ( CookieMember.xid == 0 )
	{
		PushSignup ();
	}

	printf ( "</div>\n" );

#ifdef PAYPAL_ON_HOME_PAGE
	if ( CookieMember.xid > 0 )
	{
		/*----------------------------------------------------------
			subscribe via paypal
		----------------------------------------------------------*/
		printf ( "</form>\n" );
		printf ( "<div class='story'>\n" );
		printf ( "<%s>Subscription information</%s>\n", STORY_TITLE, STORY_TITLE );
		printf ( "Name&emsp;%s<br>\n", xmember.xmname );
		switch ( xmember.xmrole[0] )
		{
			case ROLE_ADMIN:
				printf ( "Adminstrator does not need a subscription.<br>\n" );
				break;
			case ROLE_BETA:
				printf ( "Beta testing membership, expected to complete %s.<br>\n", xmember.xmsubexpire );
				break;
			case ROLE_FREE:
				RptlogRecordCount = GetReportCount ();
				PrintUpgradeMessage ( RptlogRecordCount > 0 ? 2 : 1 );
				break;
			case ROLE_PAID:
				printf ( "Fully paid membership, expires on %s.<br>\n", xmember.xmsubexpire );
				break;
			case ROLE_EXPIRED:
				printf ( "Expired membership, expiration date %s.<br>\n", xmember.xmsubexpire );
				printf ( "<b>Renew now for full access for $%d/year.</b><br>\n", ANNUAL_PRICE );
				break;
		}

		PaintPaypal ();

		printf ( "</div>\n" );
		printf ( "</form>\n" );
	}
#endif
}

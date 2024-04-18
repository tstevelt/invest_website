/*----------------------------------------------------------------------------
	Program : invest/PaintProfile.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint the user profile page.
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

void PaintProfile ()
{
#ifdef PAYPAL_ON_PROFILE_PAGE
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
			PrintUpgradeMessage ( 1 );
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

	printf ( "<form name='invest' id='invest' action='invest.cgi' method='POST' enctype='application/x-www-form-urlencoded'>\n" );
	printf ( "<input type='hidden' id='MenuIndex' name='MenuIndex' value='%d'>\n", MenuIndex );
#endif

	printf ( "<input type='hidden' id='Button' name='Button' value='unknown'>\n" );

	/*----------------------------------------------------------
		change email address
	----------------------------------------------------------*/
	printf ( "<div class='story'>\n" );
	printf ( "<table class='table-condensed table-borderless'>\n" );
	printf ( "<tr>\n" );
	printf ( "<td colspan='2'><%s>Change your email address</%s></td>\n", STORY_TITLE, STORY_TITLE );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td colspan='2'>\n" );
	printf ( "If you change your email address, you will be logged out and have to verify you new address before you can log in again.\n" );
	printf ( "<ul>\n" );
	printf ( "<li>A verification email will be sent.</li>\n" );
	printf ( "<li>The email will expire in one hour.</li>\n" );
	printf ( "<li>You must verify from the same device (IP address).</li>\n" );
	printf ( "</ul>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td width='40%%'>New Email Address</td>\n" );
	printf ( "<td width='60%%'><input type='search' name='SignupEmail' size='85' maxlength='80'\n" );
	printf ( " value='%s'>\n", xmember.xmemail );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td colspan='2'>\n" );
	printf ( "<input type='reset' name='reset' value='Reset'>\n" );
	if ( CookieMember.xid == DEMO_MEMBER )
	{
		printf ( "<input type='button' value='Save Disabled'>\n" );
	}
	else
	{
		printf ( "<input type='submit' name='SubmitChangeEmail' value='Save New Email'>\n" );
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>\n" );

	printf ( "</div>\n" );

	/*----------------------------------------------------------
		change name and subscription info
	----------------------------------------------------------*/
	if ( xmember.xmstatus[0] == STATUS_VERIFIED )
	{
		printf ( "<div class='story'>\n" );
		printf ( "<table class='table-condensed table-borderless'>\n" );
		printf ( "<tr>\n" );
		printf ( "<td colspan='2'><%s>Edit Profile</%s></td>\n", STORY_TITLE, STORY_TITLE );
		printf ( "</tr>\n" );

		printf ( "<tr>\n" );
		printf ( "<td width='40%%'>Your ID</td>\n" );
		printf ( "<td width='60%%'>%ld</td>\n", xmember.xid );
		printf ( "</tr>\n" );

		printf ( "<tr>\n" );
		printf ( "<td>Your name</td>\n" );
		printf ( "<td><input type='search' name='SignupName' size='35' maxlength='30'\n" );
		printf ( " value='%s'>\n", xmember.xmname );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );

		printf ( "<tr>\n" );
		printf ( "<td>Pager</td>\n" );
		printf ( "<td><input type='search' name='SignupPager' size='65' maxlength='60'\n" );
		printf ( " value='%s'>\n", xmember.xmpager );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );

		printf ( "<tr>\n" );
		printf ( "<td colspan='2'>\n" );
		printf ( "Give this code to people when you refer them to this site.  When they upgrade, you will earn $%d!\n", REFERRAL_FEE );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );

		printf ( "<tr>\n" );
		printf ( "<td>Promotion Code</td>\n" );
		if ( nsStrlen ( xmember.xmmypromo ) > 0 )
		{
			printf ( "<td>%s</td>\n", xmember.xmmypromo );
		}
		else 
		{
			switch ( xmember.xmrole[0] )
			{
				case ROLE_BETA:
				case ROLE_FREE:
					printf ( "<td>Assigned on upgrade</td>\n" );
					break;
				case ROLE_ADMIN:
				case ROLE_PAID:
				case ROLE_EXPIRED:
					printf ( "<td>&nbsp;</td>\n" );
					break;
			}
		}
		printf ( "</tr>\n" );

		/*----------------------------------------------------------
			chart options
		----------------------------------------------------------*/
		printf ( "<tr>\n" );
		printf ( "<td>Chart Days</td>\n" );
		printf ( "<td>\n" );
		printf ( "<select name='ChartNumber'>\n" );
		printf ( "<option value='6'%s>6 Months</option>\n",   xmember.xmchrtnum == 6 ? " selected" : "" );
		printf ( "<option value='12'%s>12 Months</option>\n", xmember.xmchrtnum == 12 ? " selected" : "" );
		printf ( "<option value='18'%s>18 Months</option>\n", xmember.xmchrtnum == 18 ? " selected" : "" );
		printf ( "<option value='24'%s>24 Months</option>\n", xmember.xmchrtnum == 24 ? " selected" : "" );
		printf ( "</select>\n" );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );


		printf ( "<tr>\n" );
		printf ( "<td>Chart Fast Average</td>\n" );
		printf ( "<td>\n" );
		printf ( "<select name='ChartFast'>\n" );
		printf ( "<option value='5'%s>5 Days</option>\n", xmember.xmchrtfast == 5 ? " selected" : "" );
		printf ( "<option value='10'%s>10 Days</option>\n", xmember.xmchrtfast == 10 ? " selected" : "" );
		printf ( "<option value='20'%s>20 Days</option>\n", xmember.xmchrtfast == 20 ? " selected" : "" );
		printf ( "<option value='50'%s>50 Days</option>\n", xmember.xmchrtfast == 50 ? " selected" : "" );
		printf ( "<option value='100'%s>100 Days</option>\n", xmember.xmchrtfast == 100 ? " selected" : "" );
		printf ( "</select>\n" );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );


		printf ( "<tr>\n" );
		printf ( "<td>Chart Slow Average</td>\n" );
		printf ( "<td>\n" );
		printf ( "<select name='ChartSlow'>\n" );
		printf ( "<option value='10'%s>10 Days</option>\n", xmember.xmchrtslow == 10 ? " selected" : "" );
		printf ( "<option value='20'%s>20 Days</option>\n", xmember.xmchrtslow == 20 ? " selected" : "" );
		printf ( "<option value='50'%s>50 Days</option>\n", xmember.xmchrtslow == 50 ? " selected" : "" );
		printf ( "<option value='100'%s>100 Days</option>\n", xmember.xmchrtslow == 100 ? " selected" : "" );
		printf ( "<option value='200'%s>200 Days</option>\n", xmember.xmchrtslow == 200 ? " selected" : "" );
		printf ( "</select>\n" );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );

		/*----------------------------------------------------------
			added target allocation fields. 04/18/2024
		----------------------------------------------------------*/
		printf ( "<tr>\n" );
		printf ( "<td>Cash Target</td>\n" );
		printf ( "<td>\n" );
		printf ( "<input type='search' name='CashT' id='CashT' value='%.2f'>\n", xmember.xmcasht );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );

		printf ( "<tr>\n" );
		printf ( "<td>Fixed Income  Target</td>\n" );
		printf ( "<td>\n" );
		printf ( "<input type='search' name='BondT' id='BondT' value='%.2f'>\n", xmember.xmbondt );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );

		printf ( "<tr>\n" );
		printf ( "<td>Domestic Equity Target</td>\n" );
		printf ( "<td>\n" );
		printf ( "<input type='search' name='DomT' id='DomT' value='%.2f'>\n", xmember.xmdomt );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );

		printf ( "<tr>\n" );
		printf ( "<td>Foreign Equity Target</td>\n" );
		printf ( "<td>\n" );
		printf ( "<input type='search' name='ForT' id='ForT' value='%.2f'>\n", xmember.xmfort );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );


		printf ( "<tr>\n" );
		printf ( "<td colspan='2'>\n" );
		printf ( "<input type='reset' name='reset' value='Reset'>\n" );
		if ( CookieMember.xid == DEMO_MEMBER )
		{
			printf ( "<input type='button' value='Save Disabled'>\n" );
		}
		else
		{
			// printf ( "<input type='submit' name='SubmitProfile' value='Save Changes'>\n" );
			printf ( "<input type='button' name='SubmitProfile' value='Save Changes' onclick='javascript:SubmitAllocate_100(this);'>\n" );
		}
		printf ( "</td>\n" );
		printf ( "</tr>\n" );

		printf ( "</table>\n" );

		printf ( "</div>\n" );
	}

	/*----------------------------------------------------------
		change password
	----------------------------------------------------------*/
	printf ( "<div class='story'>\n" );
	
	printf ( "<table class='table-condensed table-borderless'>\n" );

	printf ( "<tr>\n" );
	printf ( "<td colspan='2'><%s>Change Password</%s></td>\n", STORY_TITLE, STORY_TITLE );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td colspan='2'>Password 6 to 30 characters, contain three of upper, lower, numbers and punctuation.</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td width='40%%'>Password</td>\n" );
	printf ( "<td width='60%%'><input type='password' name='SignupPassword' size='30' maxlength='24'>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>Confirm Password</td>\n" );
	printf ( "<td><input type='password' name='ConfirmPassword' size='30' maxlength='24'>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td colspan='2'>\n" );
	if ( CookieMember.xid == DEMO_MEMBER )
	{
		printf ( "<input type='button' value='Change Disabled'>\n" );
	}
	else
	{
		printf ( "<input type='submit' name='SubmitChangePassword' value='Change Password'>\n" );
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>\n" );
	printf ( "</div>\n" );


}

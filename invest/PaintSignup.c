/*----------------------------------------------------------------------------
	Program : invest/PaintSignup.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint the signup page.
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

void PaintSignup ()
{
	printf ( "<div class='story'>\n" );
	printf ( "<table class='table-condensed table-borderless'>\n" );
	printf ( "<tr>\n" );
	printf ( "<td colspan='2'><%s>Sign up</%s></td>\n", STORY_TITLE, STORY_TITLE );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>Your name</td>\n" );
	printf ( "<td><input type='search' name='SignupName' size='35' maxlength='30'></td>\n" );
	printf ( "</tr>\n" );


	printf ( "<tr>\n" );
	printf ( "<td colspan='2'>\n" );
	printf ( "Your email address has to be verified before you can log in.\n" );
	printf ( "<ul>\n" );
	printf ( "<li>A verification email will be sent.</li>\n" );
	printf ( "<li>The email will expire in one hour.</li>\n" );
	printf ( "<li>You must verify from the same device (IP address).</li>\n" );
	printf ( "</ul>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>Email address</td>\n" );
	printf ( "<td><input type='search' name='SignupEmail' size='85' maxlength='80'></td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td colspan='2'>Password 6 to 30 characters, contain three of upper, lower, numbers and punctuation.</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>Password</td>\n" );
	printf ( "<td><input type='password' name='SignupPassword' size='30' maxlength='24'></td>\n" );
	printf ( "</tr>\n" );
	
	printf ( "<tr>\n" );
	printf ( "<td>Confirm Password</td>\n" );
	printf ( "<td><input type='password' name='ConfirmPassword' size='30' maxlength='24'></td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>Promo Code</td>\n" );
	printf ( "<td><input type='search' name='RefPromoCode' size='14' maxlength='9'></td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td colspan='2'>\n" );
	printf ( "This website places a cookie on your computer in order to:\n" );
	printf ( "<ul>\n" );
	printf ( "<li>Enable you to log in.</li>\n" );
	printf ( "<li>Keep you logged in between sessions</li>\n" );
	printf ( "<li>Manage your subsciptions</li>\n" );
	printf ( "</ul>\n" );
	printf ( "<input type='checkbox' name='SignupCookie' id='SignupCookie' checked>\n" );
	printf ( "&emsp;I will accept cookies from you</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td colspan='2'>\n" );
	printf ( "Investing in the securities market is inherently risky:\n" );
	printf ( "<ul>\n" );
	printf ( "<li>I am personally responsible for my investing decisions.</li>\n" );
	printf ( "<li>I understand that Silver Hammer Software recieves market data from third parties.</li>\n" );
	printf ( "<li>I understand that sometimes there may be mistakes in the data.</li>\n" );
	printf ( "<li>I agree to never blame or sue Silver Hammer Software or any associated third parties for any loss I may suffer in the securities markets.</li>\n" );
	printf ( "</ul>\n" );
	printf ( "<input type='checkbox' name='AcceptAllRisks' id='AcceptAllRisks'>\n" );
	printf ( "&emsp;I accept and agree with the above statements.</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td colspan='2'>\n" );
	printf ( "Silver Hammer Software is not a Financial Advisor.<br>\n" );
	printf ( "<input type='checkbox' name='NotAFinancialAdvisor' id='NotAFinancialAdvisor'>\n" );
	printf ( "&emsp;I agree with the above statements.</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td colspan='2'>\n" );
	printf ( "<input type='checkbox' name='DoFurtherResearch' id='DoFurtherResearch'>\n" );
	printf ( "&emsp;I will do further research before making any investment decisions.</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td colspan='2'>\n" );
	printf ( "<input type='checkbox' name='TermsAndConditions' id='TermAndConditions'>\n" );
	printf ( "&emsp;I have read and agree to <a target='_blank' href='TermsAndConditions.pdf'>Terms and Conditions.</a></td>" );
	printf ( "</tr>\n" );

#ifdef ELECTRONIC_SIGNATURE
	printf ( "<tr>\n" );
	printf ( "<td colspan='2'>\n" );
	printf ( "Electronic signatures begin with '/s/' and end with '/'. For example: /s/First Last/<br>\n" );
	printf ( "My signature&emsp;<input type='search' name='MySignature' id='MySignature' size='30' value='/s/first last/'>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );
#endif

	printf ( "<tr>\n" );
	printf ( "<td colspan='2' align='center'><input type='submit' name='SubmitSignup' value='Sign up'></td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>\n" );
	printf ( "</div>\n" );

}

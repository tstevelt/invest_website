/*----------------------------------------------------------------------------
	Program : invest/ProcessPayment.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Process a payment.  upgrade user from free to paid if needed.
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

static void GenPromoCode ()
{
	int		xo;
	
	srand ( time ( NULL ) );

	// digits 48 - 57 -- start at 50 to avoid zero and one
	// letters 65 - 90
	for ( xo = 0; xo < 6; xo++ )
	{
		xmember.xmmypromo[xo] = (char) random_range ( 65, 90 );
	}
	for ( ; xo < 9; xo++ )
	{
		xmember.xmmypromo[xo] = (char) random_range ( 50, 57 );
	}
	xmember.xmmypromo[xo] = '\0';

	if ( nsStrlen ( xmember.xmmypromo ) != PROMO_CODE_LENGTH )
	{
		fprintf ( stderr, "Promo Code not %d characters\n", PROMO_CODE_LENGTH );
		exit ( 0 );
	}

}

void ProcessPayment ()
{
	int		Count = 0;
	char	Fragment[128];
	int		Year, Month, Day, FileOkay;
	char	fn[128];
	FILE	*fp;
	time_t	tvec;

	sprintf ( WhereClause, "member.id = %ld", CookieMember.xid );
	if ( LoadMember ( &MySql, WhereClause, &xmember, 0 ) != 1 )
	{
		SafeError ( COLOR_ERROR, "Cannot load member record for %ld", CookieMember.xid );
		return;
	}

	sprintf ( WhereClause, "PYtransaction = '%s'", Payment.Transaction );
	if ( dbySelectCount ( &MySql, "payments", WhereClause, LogFileName ) > 0 )
	{
		SafeError ( COLOR_ERROR, "This payment (%s) has already been entered",  Payment.Transaction );
		return;
	}

	sprintf ( Statement, "update member set" );

	switch ( xmember.xmrole[0] )
	{
		case ROLE_FREE:
		case ROLE_BETA:
		case ROLE_EXPIRED:
			if (( Payment.Amount == INTRODUCTORY_PRICE ) ||
				( Payment.Amount == ANNUAL_PRICE       ))
			{
				sprintf ( Fragment, " Mrole = '%c'", ROLE_PAID );
			}
			else if ( Payment.Amount == PREMIUM_PRICE )
			{
				sprintf ( Fragment, " Mrole = '%c'", ROLE_PREMIUM );
			}
			strcat ( Statement, Fragment );
			Count++;
			break;
		case ROLE_PAID:
		case ROLE_PREMIUM:
		case ROLE_ADMIN:
			break;
	}

	/*----------------------------------------------------------
		set original subscription date if null
		Msuborig   | date             | YES  |     | NULL    |                |
	----------------------------------------------------------*/
	if (( nsStrlen  ( xmember.xmsuborig              ) == 0 ) || 
		( nsStrncmp ( xmember.xmsuborig, "NULL", 4   ) == 0 ) ||
		( nsStrncmp ( xmember.xmsuborig, "(null)", 6 ) == 0 ) ||
		( nsStrncmp ( xmember.xmsuborig, "0000", 4   ) == 0 ))
	{
		sprintf ( Fragment, "%s Msuborig = '%s'", Count == 0 ? "" : ",", Today );
		strcat ( Statement, Fragment );
		Count++;
	}

	/*----------------------------------------------------------
		save this subscription informatnion, credit remaining
		time (if any) of previous payment.
		Msubstart  | date             | YES  |     | NULL    |                |
		Msubpaid   | double           | YES  |     | 0       |                |
		Msubexpire | date             | YES  |     | NULL    |                |
		01234567890
		yyyy-mm-dd
	----------------------------------------------------------*/
	if ( nsStrcmp ( Today, xmember.xmsubexpire ) < 0 )
	{
		Year  = nsAtoi (  xmember.xmsubexpire );
		Month = nsAtoi ( &xmember.xmsubexpire[5] );
		Day   = nsAtoi ( &xmember.xmsubexpire[8] );
	}
	else
	{
		Year  = nsAtoi (  Today );
		Month = nsAtoi ( &Today[5] );
		Day   = nsAtoi ( &Today[8] );
	}
	Year++;
	if ( Day > 1 )
	{
		Day--;
	}

	GenPromoCode ();

	sprintf ( Fragment, "%s Msubstart = '%s', Msubpaid = %.2f, Msubexpire = '%04d-%02d-%02d', Mmypromo = '%s'",
		Count == 0 ? "" : ",", Today, Payment.Amount, Year, Month, Day, xmember.xmmypromo );

	strcat ( Statement, Fragment );
	Count++;

	sprintf ( Fragment, " where member.id = %ld", xmember.xid );
	strcat ( Statement, Fragment );

	if ( dbyUpdate ( "invest", &MySql, Statement, 0, LogFileName ) != 1 )
	{
		SafeError ( COLOR_ERROR, "Could not upgrade your member record.  Support has been notified" );
		sprintf ( Fragment, "Failed upgrade %ld %s", xmember.xid, xmember.xmname );
		NotifyAdmin ( Fragment );
		return;
	}
	else
	{
		SafeError ( COLOR_SUCCESS, "Thank-you for upgrading your account!" );
		SafeError ( COLOR_SUCCESS, "Expiration date set to %d/%d/%d", Month, Day, Year );
		SafeError ( COLOR_SUCCESS, "Look for email with Promotional Code %s", xmember.xmmypromo );
		sprintf ( WhereClause, "member.id = %ld", xmember.xid );
		LoadMember ( &MySql, WhereClause, &xmember, 0 );


		/*----------------------------------------------------------
// fixit  send thank-you email with promo code
		----------------------------------------------------------*/
		sprintf ( fn, "/var/local/tmp/member_%d.txt", getpid() );
		if (( fp = fopen ( fn, "w" )) == (FILE *)0 )
		{
			SafeError ( COLOR_ERROR, "Cannot create file to send email." );
		}
		else
		{
			fprintf ( fp, "<html>\n" );

			fprintf ( fp, "<head>\n" );
			fprintf ( fp, "<title>Thanks and Promo Code</title>\n" );
			fprintf ( fp, "</head>\n" );

			fprintf ( fp, "<body color='#FAEBD7'>\n" );

			fprintf ( fp, "<p>\n" );

			fprintf ( fp, "Hello %s,\n", xmember.xmname );
			fprintf ( fp, "<p>\n" );

			fprintf ( fp, "Thank you for upgrading your account on %s.<br>\n", URL_NAME );
			fprintf ( fp, "<p>\n" );

			fprintf ( fp, "We are currently offering a $%d referral fee to anyone you refer<br>\n", REFERRAL_FEE );
			fprintf ( fp, "to our site and who becomes a paid member.  Make sure they use the<br>\n" );
			fprintf ( fp, "promotion code shown below.<br>\n" );
			fprintf ( fp, "<p>\n" );

			fprintf ( fp, "<h3>Code %s</h3>\n", xmember.xmmypromo );

			fprintf ( fp, "Thanks again, Silver Hammer Software\n" );
			fprintf ( fp, "<p>\n" );

			fprintf ( fp, "</body>\n" );
			fprintf ( fp, "</html>\n" );

			nsFclose ( fp );

			SendAttached ( SUPPORT_ADDRESS, xmember.xmemail,
						"", "", 	"Promotional Code from Silver Hammer Software",
							1, fn,
							0, NULL );
		}
	}

	/*----------------------------------------------------------
		record payment specifics
	----------------------------------------------------------*/

	sprintf ( Statement, "insert into payments ( PYmember, PYtransaction, PYstatus, PYamount, PYcurrency, PYitem ) \
		values ( %ld, '%s', '%c', %.2f, '%s', '%s' )",
				xmember.xid,
				Payment.Transaction,
				Payment.Status[0],
				Payment.Amount,
				Payment.Currency,
				Payment.ItemNumber );

	if ( dbyInsert ( "invest", &MySql, Statement, 0, LogFileName ) != 1 )
	{
		SafeError ( COLOR_ERROR, "Payment detail not recorded.  Support has been notified" );
		sprintf ( Fragment, "Upgrade payment $%.2f not recorded %ld %s", Payment.Amount, xmember.xid, xmember.xmname );
		NotifyAdmin ( Fragment );
		return;
	}

	if (( fp = fopen ( LogFileName, "a" )) == (FILE *)0 )
	{
		fp = stderr;
		FileOkay = 0;
	}
	else
	{
		FileOkay = 1;
	}

	tvec = time ( NULL );

	fprintf ( fp, "--------------------------------------------------------------------------------\n" );
	fprintf ( fp, "Member     : %ld  %s\n", xmember.xid, xmember.xmname );
	fprintf ( fp, "Date Time  : %s", asctime(localtime(&tvec)) );
	fprintf ( fp, "Transaction: %s\n", Payment.Transaction );
	fprintf ( fp, "Status     : %s\n", Payment.Status );
	fprintf ( fp, "Amount     : %.2f\n", Payment.Amount );
	fprintf ( fp, "Currency   : %s\n", Payment.Currency );
	fprintf ( fp, "Category   : %s\n", Payment.Category );
	fprintf ( fp, "Item Number: %s\n", Payment.ItemNumber );
	fprintf ( fp, "Signature  : %s\n", Payment.Signature );
	fprintf ( fp, "--------------------------------------------------------------------------------\n" );

	if ( FileOkay )
	{
		nsFclose ( fp );
	}

	sprintf ( Fragment, "Upgrade %ld %s", xmember.xid, xmember.xmname );
	NotifyAdmin ( Fragment );
}

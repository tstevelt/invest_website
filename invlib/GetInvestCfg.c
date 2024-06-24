/*----------------------------------------------------------------------------
	Program : GetInvestCfg.c
	Author  : Tom Stevelt
	Date    : Jul 2021
	Synopsis: get config from $HOME config file. Sets global vars.
	Return  : Succeed or die.

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		02/25/2024	Added PayPal
	tms		06/24/2024	IEX dead

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

void GetInvestCfg( int UseTiingo )
{
	char	*Logname;
	char	fn[1024];
	FILE	*fp;
	char	xbuffer[1024];
	char	*tokens[4];
	int		tokcnt;
	int		GotKey = 0;
	int		GotURL = 0;
	int		GotPaypalKey = 0;
	int		GotPaypalURL = 0;
	int		ErrorCount = 0;
    char	*ptrName;
	char	*ptrValue;

	if ( UseTiingo == 0 )
	{
		fprintf ( stderr, "IEX Cloud is dead.<br>\n" );
		exit ( 1 );
	}

	/*----------------------------------------------------------
		set defaults
	----------------------------------------------------------*/
	if (( Logname = getenv ( "LOGNAME" )) == NULL )
	{
		Logname = "tms";
	}
	sprintf ( Support_Address,   "%s@localhost", Logname );
	sprintf ( Admin_Address,     "%s@localhost", Logname );
	sprintf ( Signup_Address,    "%s@localhost", Logname );
	sprintf ( Webmaster_Address, "%s@localhost", Logname );
	sprintf ( Report_Address,    "%s@localhost", Logname );

	sprintf ( fn, "%s/.Invest.cfg", WORKDIR );

	if (( fp = fopen ( fn, "r" )) == NULL )
	{
		fprintf ( stderr, "%s not found, errno %d %s.\n", fn, errno, strerror(errno) );
		exit ( 1 );
	}

	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != NULL )
	{
		if (( tokcnt = GetTokensW ( xbuffer, tokens, 3 )) < 2 )
		{
			continue;
		}

		if ( UseTiingo == 0 && nsStrcmp ( tokens[0], "IEX_CLOUD_APISTR" ) == 0 )
		{
			snprintf ( env_ApiKey, sizeof(env_ApiKey), "%s", tokens[1] );
			GotKey = 1;
		}
		else if ( UseTiingo == 0 && nsStrcmp ( tokens[0], "IEX_URL" ) == 0 )
		{
			snprintf ( env_ApiURL, sizeof(env_ApiURL), "%s", tokens[1] );
			GotURL = 1;
		}

		else if ( UseTiingo == 1 && nsStrcmp ( tokens[0], "TIINGO_APISTR" ) == 0 )
		{
			snprintf ( env_ApiKey, sizeof(env_ApiKey), "%s", tokens[1] );
			GotKey = 1;
		}
		else if ( UseTiingo == 1 && nsStrcmp ( tokens[0], "TIINGO_URL" ) == 0 )
		{
			snprintf ( env_ApiURL, sizeof(env_ApiURL), "%s", tokens[1] );
			GotURL = 1;
		}
		else if ( nsStrcmp ( tokens[0], "PAYPAL_APISTR" ) == 0 )
		{
			snprintf ( env_PaypalKey, sizeof(env_PaypalKey), "%s", tokens[1] );
			GotPaypalKey = 1;
		}
		else if ( nsStrcmp ( tokens[0], "PAYPAL_URL" ) == 0 )
		{
			snprintf ( env_PaypalURL, sizeof(env_PaypalURL), "%s", tokens[1] );
			GotPaypalURL = 1;
		}

		else if ( nsStrcmp ( tokens[0], "SUPPORT_ADDRESS" ) == 0 )
		{
			snprintf ( Support_Address, sizeof(Support_Address), "%s", tokens[1] );
		}
		else if ( nsStrcmp ( tokens[0], "ADMIN_ADDRESS" ) == 0 )
		{
			snprintf ( Admin_Address, sizeof(Admin_Address), "%s", tokens[1] );
		}
		else if ( nsStrcmp ( tokens[0], "SIGNUP_ADDRESS" ) == 0 )
		{
			snprintf ( Signup_Address, sizeof(Signup_Address), "%s", tokens[1] );
		}
		else if ( nsStrcmp ( tokens[0], "WEBMASTER_ADDRESS" ) == 0 )
		{
			snprintf ( Webmaster_Address, sizeof(Webmaster_Address), "%s", tokens[1] );
		}
		else if ( nsStrcmp ( tokens[0], "REPORT_ADDRESS" ) == 0 )
		{
			snprintf ( Report_Address, sizeof(Report_Address), "%s", tokens[1] );
		}
	}
	
	nsFclose ( fp );

	if ( GotKey == 0 )
	{
		if ( UseTiingo == 0 )
		{
			ptrValue = "IEX_CLOUD_APISTR";
		}
		else
		{
			ptrValue = "TIINGO_APISTR";
		}
    	if (( ptrName = getenv ( ptrValue )) == NULL )
		{
			fprintf ( stderr, "%s not found in %s\n", ptrValue, fn );
			fprintf ( stderr, "%s not found in environment\n", ptrValue );
			ErrorCount++;
		}
		else
		{
			snprintf ( env_ApiKey, sizeof(env_ApiKey), "%s", ptrName );
		}
	}

	if ( GotURL == 0 )
	{
		if ( UseTiingo == 0 )
		{
			ptrValue = "IEX_URL";
		}
		else
		{
			ptrValue = "TIINGO_URL";
		}
    	if (( ptrName = getenv ( ptrValue )) == NULL )
		{
			fprintf ( stderr, "%s not found in %s\n", ptrValue, fn );
			fprintf ( stderr, "%s not found in environment\n", ptrValue );
			ErrorCount++;
		}
		else
		{
			snprintf ( env_ApiURL, sizeof(env_ApiURL), "%s", ptrName );
		}
	}

	if ( GotPaypalKey == 0 )
	{
		ptrValue = "PAYPAL_APISTR";

    	if (( ptrName = getenv ( ptrValue )) == NULL )
		{
			printf ( "%s not found in %s<br>\n", ptrValue, fn );
			printf ( "%s not found in environment<br>\n", ptrValue );
			fflush ( stdout );
			ErrorCount++;
		}
		else
		{
			snprintf ( env_PaypalKey, sizeof(env_PaypalKey), "%s", ptrName );
		}
	}

	if ( GotPaypalURL == 0 )
	{
		ptrValue = "PAYPAL_URL";

    	if (( ptrName = getenv ( ptrValue )) == NULL )
		{
			printf ( "%s not found in %s<br>\n", ptrValue, fn );
			printf ( "%s not found in environment<br>\n", ptrValue );
			fflush ( stdout );
			ErrorCount++;
		}
		else
		{
			snprintf ( env_PaypalURL, sizeof(env_PaypalURL), "%s", ptrName );
		}
	}
	if ( ErrorCount )
	{
		exit ( 1 );
	}
}

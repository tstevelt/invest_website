/*----------------------------------------------------------------------------
	Program : invlib/GetFundamentalsTiingo.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Get fundamentals information from Tiingo
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

int GetFundamentalsTiingo ( const char *Ticker, const char Type, const int Days, char *Buffer, size_t BufferSize )
{
	static	int			firstpass = 1;
	static	DATEVAL		Today;
	static	CURL		*curl;
			CURLcode   	curlRV;
			DATEVAL		StartDate;
			char		cmdline[1024];
			char		TempFileName[256];
			FILE		*tfp;
			int			nr;
			int			Debug = 0;

	if ( firstpass == 1 )
	{
		if (( curl = curl_easy_init () ) == NULL )
		{
			fprintf ( stderr, "curl init failed<br>\n" );
			exit ( 1 );
		}

		CurrentDateval ( &Today );
		firstpass = 0;
	}

	DateAdd ( &Today, (int) (0 - Days), &StartDate );

	if ( Type == 'S' )
	{
		/*---------------------------------------------------------------------------
			curl -s "https://api.tiingo.com/tiingo/fundamentals/$TICKER/statements?token=$APISTR" 
		---------------------------------------------------------------------------*/
		snprintf ( cmdline, sizeof(cmdline),
			"%s/fundamentals/%s/statements?startDate=%04d-%02d-%02d&token=%s",
				env_ApiURL,
				Ticker,
				StartDate.year4, StartDate.month, StartDate.day,
				env_ApiKey );
	}
	else if ( Type == 'D' )
	{
		/*---------------------------------------------------------------------------
			https://api.tiingo.com/tiingo/fundamentals/$TICKER/daily?startDate=$DATE&endDate=$DATE&tok
		---------------------------------------------------------------------------*/
		snprintf ( cmdline, sizeof(cmdline),
			"%s/fundamentals/%s/daily?startDate=%04d-%02d-%02d&token=%s",
				env_ApiURL,
				Ticker,
				StartDate.year4, StartDate.month, StartDate.day,
				env_ApiKey );
	}
	else
	{
		fprintf ( stderr, "Unknown Type %c<br>\n", Type );
		exit ( 1  );
	}
			
	if ( Debug )
	{
		printf ( "%s<br>\n", cmdline );
	}

	sprintf ( TempFileName, "%s/%s_fundamentals.json", TEMPDIR, Ticker );

	if (( tfp = fopen ( TempFileName, "w" )) == (FILE *)0 )
	{
		printf ( "cannot create: %s<br>\n", TempFileName );
		exit ( 1 );
	}

	curl_easy_setopt ( curl, CURLOPT_URL, cmdline );
	curl_easy_setopt ( curl, CURLOPT_WRITEDATA, tfp );

	if (( curlRV = curl_easy_perform ( curl )) != CURLE_OK )
	{
		printf ( "perform failed: %s<br>\n", curl_easy_strerror(curlRV) );
		exit ( 1 );
	}

	nsFclose ( tfp );

	/*----------------------------------------------------------
		leave curl init'ed
	xxx	curl_easy_cleanup ( curl );
	----------------------------------------------------------*/

	if (( tfp = fopen ( TempFileName, "r" )) == (FILE *)0 )
	{
		printf ( "cannot reopen: %s<br>\n", TempFileName );
		exit ( 1 );
	}

	nr = fread ( Buffer, BufferSize, 1, tfp );

	nsFclose ( tfp );

	return ( nr );
}

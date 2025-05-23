/*----------------------------------------------------------------------------
	Program : invest/PaintFundamentals.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Get fundamentals from SEC and display
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
/*----------------------------------------------------------
	https://www.sec.gov/edgar/sec-api-documentation
----------------------------------------------------------*/

#include	"invest.h"

void PaintFundamentals ( char *Ticker, char *Name, char *Type, char *CikCode )
{
	char	*TempFile = "/var/local/tmp/cik.json";
	char	CommandStart[128];
	char	cmdline[1024];

	printf ( "<div class='chart'>\n" );
	printf ( "<pre>\n" );

	printf ( "%s<br>\n", Name );

	/*---------------------------------------------------------------------------
		all command start with curl and user agent:
			curl -s --user-agent tstevelt@silverhammersoftware.com 

		submissions:
			https://data.sec.gov/submissions/CIK%s.json

		company concepts:
			https://data.sec.gov/api/xbrl/companyconcept/CIK%s/us-gaap/AccountsPayableCurrent.json

		company facts:
			https://data.sec.gov/api/xbrl/companyfacts/CIK%s.json

		frames:
			this is not company specific, but gives all companies at the requested date
	---------------------------------------------------------------------------*/

	sprintf ( CommandStart, "/usr/bin/curl -s --user-agent tstevelt@silverhammersoftware.com" );

	snprintf ( cmdline, sizeof(cmdline), 
			"%s 'https://data.sec.gov/api/xbrl/companyfacts/CIK%s.json' > %s",
				CommandStart, CikCode, TempFile );

	system ( cmdline );

	// snprintf ( cmdline, sizeof(cmdline), "/usr/local/bin/JsonTree %s -array last -error -wrap 100", TempFile );
	snprintf ( cmdline, sizeof(cmdline), "/usr/local/bin/JsonFake %s", TempFile );
	ForkAndExec ( cmdline, PRINT_DESTINATION_STDOUT );

	unlink ( TempFile );

	printf ( "</pre>\n" );
	printf ( "</div>\n" );
}

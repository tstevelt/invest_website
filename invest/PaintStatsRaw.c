/*----------------------------------------------------------------------------
	Program : invest/PaintStatsRaw.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Get and displays stats from IEX.  On portfolio:securies.
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

void PaintStatsRaw ( char *Ticker, char *Name )
{
	static	int		firstpass = 1;
	char	*TempFile = "/var/local/tmp/stats.json";
	char	cmdline[1024];
	FILE	*fp;
	char	xbuffer[10240];
	int		nr;
	int		inNumber;
	int		sawDecimal;

	if ( firstpass )
	{
		GetInvestCfg ( 0 );
		firstpass = 0;
	}

	printf ( "<div class='chart'>\n" );
	printf ( "<pre>\n" );

	printf ( "%s<br>\n", Name );

	sprintf ( cmdline, "curl -s %s/stock/%s/stats?token=%s > %s", env_ApiURL, Ticker, env_ApiKey, TempFile );

	system ( cmdline );

	/*----------------------------------------------------------
		JsonTree does not work on this file
	----------------------------------------------------------*/

	if (( fp = fopen ( TempFile, "r" )) == NULL )
	{
		printf ( "Cannot open tempfile\n" );
	}
	else
	{
		nr = fread ( xbuffer, 1, sizeof(xbuffer), fp );
		fclose ( fp );

		inNumber = sawDecimal = 0;

		for ( int xi = 0; xi < nr; xi++ )
		{
			switch ( xbuffer[xi] )
			{
				case '{':
				case '}':
				case '"':
					continue;
				case ':':
					printf  ( " = " );
					inNumber = sawDecimal = 0;
					break;
				default:
					if ( strncmp ( &xbuffer[xi], ",\"", 2 ) == 0 )
					{
						printf ( "\n" );
						inNumber = sawDecimal = 0;
					}
					else
					{
						if ( inNumber && xbuffer[xi] == '.' )
						{
							sawDecimal++;
						}
						else if ( xbuffer[xi] >= '0' && xbuffer[xi] <= '9' )
						{
							inNumber = 1;
							if ( sawDecimal )
							{
								sawDecimal++;
							}
						}

						if ( inNumber && sawDecimal == 6 )
						{
							while ( xbuffer[xi] >= '0' && xbuffer[xi] <= '9' )
							{
								xi++;
							}
							xi--;
							break;
						}
						fputc ( (int) xbuffer[xi], stdout );
					}
					break;
			}
		}
	}

	unlink ( TempFile );

	printf ( "</pre>\n" );
	printf ( "</div>\n" );
}

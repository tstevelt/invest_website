/*----------------------------------------------------------------------------
	Program : invlib/IEX_RateLimit.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: IEX implemented Rate Limit as of May 1, 2023.
				Appears to be 5 requests per minute
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

#include 	<sys/timeb.h>

#include	"invprivate.h"
#include	"invlib.h"

double	TotalNapTime = 0.0;

void IEX_RateLimit ( int Debug )
{
	static	int			FirstPass = 1;
	static	double		Current = 0.0;
	static	double		Previous = 0.0;
			struct timeb		tp;
			int			MilliSeconds;

	ftime ( &tp );
	Current = tp.time + (double)tp.millitm / 1000.0;

	if ( Debug )
	{
		printf ( "IEX_RateLimit: current %.4f  previous %.4f", Current, Previous );
	}

	if ( FirstPass == 1 )
	{
		FirstPass = 0;
		Previous = Current;
		if ( Debug )
		{
			printf ( "firstpass\n" );
		}
		return;
	}

	if ( Current - Previous < 0.25 )
	{
		MilliSeconds = (int)(1000.0 * (0.25 - (Current - Previous)));
		if ( Debug )
		{
			printf ( " nap %d total %.4f", MilliSeconds, TotalNapTime );
		}
		if ( MilliSeconds > 0 )
		{
			nap ( MilliSeconds );
			TotalNapTime = TotalNapTime + MilliSeconds;
		}
	}

	ftime ( &tp );
	Previous = tp.time + (double)tp.millitm / 1000.0;

	if ( Debug )
	{
		printf ( "\n" );
	}

	return;
}

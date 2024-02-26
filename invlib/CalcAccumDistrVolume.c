/*----------------------------------------------------------------------------
	Program : invlib/CalcAccumDistrVolume.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Given number of days, calculate the A/D ratio
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

int CalcAccumDistrVolume ( MYSQL *MySql, char Ticker[], char *LastDate, int Days, double *Ratio,  double *AvgVolumeUp, double *AvgVolumeDown )
{
	int		ndx;
	int		UpCount, DownCount;
	double	Diff, Percent;

	*Ratio = *AvgVolumeUp = *AvgVolumeDown = 0.0;

	sprintf ( WhereClause, "Hticker = '%s' and Hdate <= '%s'", Ticker , LastDate );

	LoadHistoryArray ( MySql, Ticker, LastDate, SORT_ASCEND, (Days+1) );
	if (HistoryCount < Days )
	{
		return ( -1 );
	}

	UpCount = DownCount = 0;

	for ( ndx = 1; ndx < HistoryCount; ndx++ )
	{
		Diff = HistoryArray[ndx].Close - HistoryArray[ndx-1].Close;
		Percent = 100.0 * Diff /  HistoryArray[ndx-1].Close;

		if ( HistoryArray[ndx].Volume > HistoryArray[ndx-1].Volume )
		{
			if ( Percent > 0.2 )
			{
				*AvgVolumeUp += HistoryArray[ndx].Volume;
				UpCount++;
			}
			else if ( Percent < -0.2 )
			{
				*AvgVolumeDown += HistoryArray[ndx].Volume;
				DownCount++;
			}
		}
	}

	*AvgVolumeUp /= (double) UpCount;
	*AvgVolumeDown /= (double) DownCount;

	if ( DownCount + UpCount > 0 )
	{
		*Ratio = AccumDistrFormula ( UpCount, DownCount );
	}

	return ( 0 );
}

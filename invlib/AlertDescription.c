/*----------------------------------------------------------------------------
	Program : invlib/AlertDescription.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Given alert type code, return description
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

char *AlertDescription ( char AlertType )
{
	switch ( AlertType )
	{
		case ALERT_NONE: return ( "None" );
		case ALERT_ABOVE_AMT: return ( "Above" );
		case ALERT_BELOW_AMT: return ( "Below" );
		case ALERT_TRAIL_STOP: return ( "Trailing Stop" );
		case ALERT_BENCHMARK: return ( "Benchmark" );
		case ALERT_ABOVE_MA5: return ( "Above 5 day MA" );
		case ALERT_ABOVE_MA10: return ( "Above 10 day MA" );
		case ALERT_ABOVE_MA20: return ( "Above 20 day MA" );
		case ALERT_ABOVE_MA50: return ( "Above 50 day MA" );
		case ALERT_ABOVE_MA100: return ( "Above 100 day MA" );
		case ALERT_ABOVE_MA200: return ( "Above 200 day MA" );
		case ALERT_BELOW_MA5: return ( "Below 5 day MA" );
		case ALERT_BELOW_MA10: return ( "Below 10 day MA" );
		case ALERT_BELOW_MA20: return ( "Below 20 day MA" );
		case ALERT_BELOW_MA50: return ( "Below 50 day MA" );
		case ALERT_BELOW_MA100: return ( "Below 100 day MA" );
		case ALERT_BELOW_MA200: return ( "Below 200 day MA" );
		case ALERT_MA5_DECREASE: return ( "5 day MA downtrend" );
		case ALERT_MA10_DECREASE: return ( "10 day MA downtrend" );
		case ALERT_MA20_DECREASE: return ( "20 day MA downtrend" );
		case ALERT_MA50_DECREASE: return ( "50 day MA downtrend" );
		case ALERT_MA5_INCREASE: return ( "5 day MA uptrend" );
		case ALERT_MA10_INCREASE: return ( "10 day MA uptrend" );
		case ALERT_MA20_INCREASE: return ( "20 day MA uptrend" );
		case ALERT_MA50_INCREASE: return ( "50 day MA uptrend" );
	}

	return ( "UNKNOWN ALERT" );
}

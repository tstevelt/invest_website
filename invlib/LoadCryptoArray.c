/*----------------------------------------------------------------------------
	Program : invlib/LoadCryptoArray.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Load this table
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

static int LoadHowMany;
static int MyPrintErrors = 0;
static char Sortby = SORT_ASCEND;

static int IntoArray ()
{
	if ( CryptoCount >= MAXHISTRECS )
	{
		if ( MyPrintErrors )
		{
			fprintf ( stderr, "LoadCryptoArray: Exceeds MAXHISTRECS\n" );
		}
		return ( -1 );
	}
	
	CryptoArray[CryptoCount].CryptoTime = xcrypto.xctime;
	CryptoArray[CryptoCount].Volume = xcrypto.xcvolume;
	if ( MungeData == 0 )
	{
		CryptoArray[CryptoCount].Open = xcrypto.xcopen;
		CryptoArray[CryptoCount].Close = xcrypto.xcclose;
		CryptoArray[CryptoCount].High = xcrypto.xchigh;
		CryptoArray[CryptoCount].Low = xcrypto.xclow;
	}
	else
	{
		CryptoArray[CryptoCount].Open = dblMunge(xcrypto.xcopen,-0.25,0.25);
		CryptoArray[CryptoCount].Close = dblMunge(xcrypto.xcclose,-0.25,0.25);
		CryptoArray[CryptoCount].High = dblMunge(xcrypto.xchigh,-0.25,0.25);
		CryptoArray[CryptoCount].Low = dblMunge(xcrypto.xclow,-0.25,0.25);
	}

	CryptoArray[CryptoCount].GotRecord = 0;
	CryptoArray[CryptoCount].Average[AVG_5]   = 0.0;
	CryptoArray[CryptoCount].Average[AVG_10]  = 0.0;
	CryptoArray[CryptoCount].Average[AVG_20]  = 0.0;
	CryptoArray[CryptoCount].Average[AVG_50]  = 0.0;
	CryptoArray[CryptoCount].Average[AVG_100] = 0.0;
	CryptoArray[CryptoCount].Average[AVG_200] = 0.0;
	CryptoArray[CryptoCount].AvgVol           = 0;
	CryptoArray[CryptoCount].RSI              = 0.0;
	CryptoArray[CryptoCount].Flag = 0;
	// fixit -- crypto does not have an ID column as of 09/25/2023
	CryptoArray[CryptoCount].CryptoID = -1;
	CryptoCount++;

	if (LoadHowMany > 0 && CryptoCount >=LoadHowMany )
	{
		return ( -1 );
	}

	return ( 0 );
}

static int cmprec ( CRYPTO_RECORD *a, CRYPTO_RECORD *b )
{
	if ( Sortby == SORT_ASCEND )
	{
		if ( a->CryptoTime < b->CryptoTime )
		{
			return ( -1 );
		}
		else if ( a->CryptoTime > b->CryptoTime )
		{
			return ( 1 );
		}
	}
	else
	{
		if ( a->CryptoTime < b->CryptoTime )
		{
			return ( 1 );
		}
		else if ( a->CryptoTime > b->CryptoTime )
		{
			return ( -1 );
		}
	}
	return ( 0 );
}

int LoadCryptoArray ( MYSQL *MySql, char *Ticker, int OrderBy, int HowMany )
{
	char	MyWhereClause[128];

	LoadHowMany = HowMany;
	CryptoCount = 0;

	switch ( OrderBy )
	{
		case SORT_ASCEND:
		case SORT_DESCEND:
			Sortby = OrderBy;
			break;
		default:
			if ( MyPrintErrors )
			{
				fprintf ( stderr, "Invalid OrderBy\n" );
				return ( 0 );
			}
			break;
	}

	sprintf ( MyWhereClause, "Cticker = '%s'", Ticker );
	LoadCryptoCB ( MySql, MyWhereClause, "Ctime desc", &xcrypto, (int(*)()) IntoArray, MyPrintErrors );

	if ( CryptoCount > 1 )
	{
		qsort ( CryptoArray, CryptoCount, sizeof(CRYPTO_RECORD), (int(*)()) cmprec );
	}

	return ( CryptoCount );
}

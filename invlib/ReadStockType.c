/*----------------------------------------------------------------------------
	Program : ReadStockType.c
	Author  : Tom Stevelt
	Date    : Apr 2022
	Synopsis: Convert IEXCLOUD issueType to SHS stock type
	Called by: InsertStock and UpdateCompany
	Return  : 

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		09/10/2022	Mappped all others to either stock or ETF.

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

/*----------------------------------------------------------
type 	refers to the common issue type
ad - ADR
re - REIT
cs - Common Stock
et - ETF
lp - Limited Partnerships
ps - Preferred Stock
si - Secondary Issue
oef - Open Ended Fund
cef - Closed Ended Fund
ce - Closed end fund
wt - Warrant
ut - Unit
struct - Structured Product
----------------------------------------------------------*/

char ReadStockType ( char *token )
{
	char	StockType;

	if (( nsStrcmp ( token, "cs" ) == 0 ) ||
		( nsStrcmp ( token, "lp" ) == 0 ) ||
		( nsStrcmp ( token, "ps" ) == 0 ) ||
		( nsStrcmp ( token, "wt" ) == 0 ) ||
		( nsStrcmp ( token, "ut" ) == 0 ) ||
		( nsStrcmp ( token, "si" ) == 0 ))
	{
		StockType = STYPE_STOCK;
	}
	else if ( nsStrcmp ( token, "ad" ) == 0 )
	{
		StockType = STYPE_ADR;
	}
	else if (( nsStrcmp ( token, "et"  ) == 0 ) ||
			 ( nsStrcmp ( token, "oef" ) == 0 ) ||
			 ( nsStrcmp ( token, "cef" ) == 0 ) ||
			 ( nsStrcmp ( token, "struct" ) == 0 ) ||
			 ( nsStrcmp ( token, "ce"  ) == 0 ))
	{
		StockType = STYPE_ETF;
	}
	else if ( nsStrcmp ( token, "re" ) == 0 )
	{
		StockType = STYPE_REIT;
	}
	else
	{
		StockType = STYPE_OTHER;
	}

	return ( StockType );
}

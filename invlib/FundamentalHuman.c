/*----------------------------------------------------------------------------
	Program : invlib/FundamentalHuman.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Given mysql field name, return human readable description
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

char * FundamentalHuman ( char *Field )
{
	if ( nsStrstr ( Field, "ticker" ) != NULL )
	{
		return ( "TICKER" );
	}
	else if ( nsStrcmp ( Field, "Sname" ) == 0 )
	{
		return ( "COMPANY" );
	}
	else if ( nsStrcmp ( Field, "INDname" ) == 0 )
	{
		return ( "INDUSTRY" );
	}
	else if ( nsStrcmp ( Field, "Febitda" ) == 0 )
	{
		return ( "EBITDA" );
	}
	else if ( nsStrcmp ( Field, "Fentval" ) == 0 )
	{
		return ( "ENT VALUE" );
	}
	else if ( nsStrcmp ( Field, "Fvalue" ) == 0 )
	{
		return ( "ENT VAL/EBITDA" );
	}
	else if ( nsStrcmp ( Field, "Fepsttm" ) == 0 )
	{
		return ( "EPS (ttm)" );
	}
	else if ( nsStrcmp ( Field, "Fpettm" ) == 0 )
	{
		return ( "P/E (ttm)" );
	}
	else if ( nsStrcmp ( Field, "Fepsfwd" ) == 0 )
	{
		return ( "EPS (fwd)" );
	}
	else if ( nsStrcmp ( Field, "Fpefwd" ) == 0 )
	{
		return ( "P/E (fwd)" );
	}
	else if ( nsStrcmp ( Field, "Fpb" ) == 0 )
	{
		return ( "P/B" );
	}
	else if ( nsStrcmp ( Field, "Fmargin" ) == 0 )
	{
		return ( "PRFT MRGN" );
	}
	else if ( nsStrcmp ( Field, "Fshares" ) == 0 )
	{
		return ( "OUTSTANDING SHARES" );
	}
	else if ( nsStrcmp ( Field, "Fyield" ) == 0 )
	{
		return ( "DIV YIELD" );
	}
	else if ( nsStrcmp ( Field, "Fbeta" ) == 0 )
	{
		return ( "BETA (2Y Weekly)" );
	}
	else if ( nsStrcmp ( Field, "Fgrow5" ) == 0 )
	{
		return ( "GROW (5yr)" );
	}
	else if ( nsStrcmp ( Field, "Fgrow1" ) == 0 )
	{
		return ( "GROW (1yr)" );
	}
	else if ( nsStrcmp ( Field, "Ftarget" ) == 0 )
	{
		return ( "TARGET $" );
	}
	else if ( nsStrcmp ( Field, "Freco" ) == 0 )
	{
		return ( "RECOMMEND" );
	}
	else if ( nsStrcmp ( Field, "Fquick" ) == 0 )
	{
		return ( "QUICK RATIO" );
	}
	else if ( nsStrcmp ( Field, "Fdebteq" ) == 0 )
	{
		return ( "TOTAL DEBT/EQUITY" );
	}
	else if ( nsStrcmp ( Field, "FreturnA" ) == 0 )
	{
		return ( "RETURN ASSETS" );
	}
	else if ( nsStrcmp ( Field, "Ffreecash" ) == 0 )
	{
		return ( "FREE CASH FLOW (ttm)" );
	}
	else if ( nsStrcmp ( Field, "Ffcfgrow" ) == 0 )
	{
		return ( "FCF GROW" );
	}
	else if ( nsStrcmp ( Field, "Ffcfcv" ) == 0 )
	{
		return ( "FCF GROW FIT" );
	}
	else if ( nsStrcmp ( Field, "Frevenuegrow" ) == 0 )
	{
		return ( "REV GROW (ttm)" );
	}
	else if ( nsStrcmp ( Field, "Fearngrow" ) == 0 )
	{
		return ( "EARN GROW (ttm)" );
	}
	else if ( nsStrcmp ( Field, "Favgret3" ) == 0 )
	{
		return ( "AVG RETURN (3yr)" );
	}
	else if ( nsStrcmp ( Field, "Favgret5" ) == 0 )
	{
		return ( "AVG RETURN (5yr)" );
	}
	else if ( nsStrcmp ( Field, "Ftotasst" ) == 0 )
	{
		return ( "TOTAL ASSETS" );
	}
	else if ( nsStrcmp ( Field, "Fmktcap" ) == 0 )
	{
		return ( "MARKET CAP" );
	}
	else if ( nsStrcmp ( Field, "Fcountry" ) == 0 )
	{
		return ( "COUNTRY" );
	}
	else if ( nsStrcmp ( Field, "Finception" ) == 0 )
	{
		return ( "FUND START" );
	}
	else if ( nsStrcmp ( Field, "Fvolume" ) == 0 )
	{
		return ( "AVG DAILY VOL (50d)" );
	}
	else if ( nsStrcmp ( Field, "Fexpratio" ) == 0 )
	{
		return ( "EXP RATIO" );
	}
	else if ( nsStrcmp ( Field, "Fmstar" ) == 0 )
	{
		return ( "M* RATING" );
	}
	else if ( nsStrcmp ( Field, "Ffactset" ) == 0 )
	{
		return ( "FACTSET RATING" );
	}
	else if ( nsStrcmp ( Field, "Fupdated" ) == 0 )
	{
		return ( "UPDATED" );
	}
	
	return ( "Unknown" );
}

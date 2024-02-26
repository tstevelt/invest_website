/*----------------------------------------------------------------------------
	Program : invlib/LoadFundamentalCB.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Load this table, with CallBack function
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

static  char    Statement[MAXSTATEMENT];

int LoadFundamentalCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XFUNDAMENTAL *ptrFundamental, int (*CallBack)(), int PrintErrors )
{
	DBY_QUERY     *qryFundamental;
	int		NumberOfRows = 0;

	// ErrorCount = 0;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from fundamental", FIELDS_FUNDAMENTAL );

	if ( WhereClause != NULL && nsStrlen ( WhereClause ) > 1 && nsStrcmp ( WhereClause, "all" ) != 0 )
	{
		strcat ( Statement, " where " );
		strcat ( Statement, WhereClause );
	}

	if ( OrderByClause != NULL && nsStrlen ( OrderByClause ) > 1 )
	{
		strcat ( Statement, " order by " );
		strcat ( Statement, OrderByClause );
	}

#ifdef DEBUG
#define DEBUG
	printf ( "LoadFundamentalCB: %s<br>\n", Statement );
	fflush ( stdout );
#endif 

	qryFundamental = dbySelect ( "LoadFundamentalCB", MySql, Statement, LogFileName );

#ifdef DEBUG
	printf ( "LoadFundamentalCB: %s<br>\n", PtrToStr ( (char *) qryFundamental ) );
	fflush ( stdout );
#endif 

	if ( qryFundamental == (DBY_QUERY *) 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadFundamentalCB select returned NULL. %s\n", WhereClause );
		}
		return ( -1 );
	}
	else if (( NumberOfRows = qryFundamental->NumRows ) == 0 )
	{
		if ( PrintErrors )
		{
			fprintf ( stderr, "LoadFundamentalCB select returned zero rows. %s\n", WhereClause );
		}
		dbyFreeQuery (  qryFundamental );
		qryFundamental = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( qryFundamental->EachRow = mysql_fetch_row ( qryFundamental->Result )) != NULL )
		{

			snprintf ( ptrFundamental->xfticker, sizeof(ptrFundamental->xfticker), "%s", qryFundamental->EachRow[0] );
			ptrFundamental->xfebitda      = nsAtol( qryFundamental->EachRow[1] );
			ptrFundamental->xfentval      = nsAtol( qryFundamental->EachRow[2] );
			ptrFundamental->xfvalue       = nsAtof( qryFundamental->EachRow[3] );
			ptrFundamental->xfepsttm      = nsAtof( qryFundamental->EachRow[4] );
			ptrFundamental->xfpettm       = nsAtof( qryFundamental->EachRow[5] );
			ptrFundamental->xfepsfwd      = nsAtof( qryFundamental->EachRow[6] );
			ptrFundamental->xfpefwd       = nsAtof( qryFundamental->EachRow[7] );
			ptrFundamental->xfpb          = nsAtof( qryFundamental->EachRow[8] );
			ptrFundamental->xfmargin      = nsAtof( qryFundamental->EachRow[9] );
			ptrFundamental->xfshares      = nsAtol( qryFundamental->EachRow[10] );
			ptrFundamental->xfyield       = nsAtof( qryFundamental->EachRow[11] );
			ptrFundamental->xfbeta        = nsAtof( qryFundamental->EachRow[12] );
			ptrFundamental->xfgrow5       = nsAtof( qryFundamental->EachRow[13] );
			ptrFundamental->xfgrow1       = nsAtof( qryFundamental->EachRow[14] );
			ptrFundamental->xftarget      = nsAtof( qryFundamental->EachRow[15] );
			ptrFundamental->xfreco        = nsAtof( qryFundamental->EachRow[16] );
			ptrFundamental->xfquick       = nsAtof( qryFundamental->EachRow[17] );
			ptrFundamental->xfdebteq      = nsAtof( qryFundamental->EachRow[18] );
			ptrFundamental->xfreturna     = nsAtof( qryFundamental->EachRow[19] );
			ptrFundamental->xffreecash    = nsAtol( qryFundamental->EachRow[20] );
			ptrFundamental->xfrevenuegrow = nsAtof( qryFundamental->EachRow[21] );
			ptrFundamental->xfearngrow    = nsAtof( qryFundamental->EachRow[22] );
			ptrFundamental->xfupdated     = nsAtol( qryFundamental->EachRow[23] );

			ptrFundamental->xftotasst = nsAtol( qryFundamental->EachRow[24] );
			ptrFundamental->xfavgret3 = nsAtof( qryFundamental->EachRow[25] );
			ptrFundamental->xfavgret5 = nsAtof( qryFundamental->EachRow[26] );
			ptrFundamental->xfmktcap = nsAtol( qryFundamental->EachRow[27] );
			snprintf ( ptrFundamental->xfcountry, sizeof(ptrFundamental->xfcountry), "%s", qryFundamental->EachRow[28] );
			snprintf ( ptrFundamental->xftype, sizeof(ptrFundamental->xftype), "%s", qryFundamental->EachRow[29] );

			ptrFundamental->xfvolume = nsAtol( qryFundamental->EachRow[30] );
			snprintf ( ptrFundamental->xfinception, sizeof(ptrFundamental->xfinception), "%s", qryFundamental->EachRow[31] );

			ptrFundamental->xfexpratio = nsAtof( qryFundamental->EachRow[32] );

			ptrFundamental->xfdividend = nsAtof( qryFundamental->EachRow[33] );
			ptrFundamental->xfbvps     = nsAtof( qryFundamental->EachRow[34] );
			sprintf ( ptrFundamental->xfframe, "%-8.8s", qryFundamental->EachRow[35] );

			ptrFundamental->xffactset[0] = qryFundamental->EachRow[36][0];
			ptrFundamental->xfmstar[0]   = qryFundamental->EachRow[37][0];

			ptrFundamental->xfequity   = nsAtof( qryFundamental->EachRow[38] );
			ptrFundamental->xfminority = nsAtof( qryFundamental->EachRow[39] );
			ptrFundamental->xfdebtlt   = nsAtof( qryFundamental->EachRow[40] );
			ptrFundamental->xfdebtnc   = nsAtof( qryFundamental->EachRow[41] );
			ptrFundamental->xfliab     = nsAtof( qryFundamental->EachRow[42] );
			ptrFundamental->xfpayables = nsAtof( qryFundamental->EachRow[43] );
			ptrFundamental->xfcurasst  = nsAtof( qryFundamental->EachRow[44] );
			ptrFundamental->xfcash     = nsAtof( qryFundamental->EachRow[45] );
			ptrFundamental->xftreasury = nsAtof( qryFundamental->EachRow[46] );

			ptrFundamental->xffcfgrow = nsAtof( qryFundamental->EachRow[47] );
			ptrFundamental->xffcfcv   = nsAtof( qryFundamental->EachRow[48] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrFundamental ) != 0 )
				{
					break;
				}
			}
		}
	}

	dbyFreeQuery (  qryFundamental );
	qryFundamental = (DBY_QUERY *) 0;

	return ( NumberOfRows );
}

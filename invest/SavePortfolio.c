/*----------------------------------------------------------------------------
	Program : invest/SavePortfolio.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Save new/updated portfolio record.
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

static	XPORTFOLIO	xtest;

#ifdef CAN_GET_DIVIDENDS
static	long	DividendCount;
static	char	DividendMinDate[11];
static	char	DividendMaxDate[11];

static int GetDates ( DBY_QUERY *Query )
{
	DividendCount = nsAtol ( Query->EachRow[0] );
	if ( DividendCount > 0 )
	{
		sprintf ( DividendMinDate, "%s", Query->EachRow[1] );
		sprintf ( DividendMaxDate, "%s", Query->EachRow[2] );
	}

	return ( 0 );
}

static void GetDividends ()
{
	int		GoGetThem = 0;
	char	cmdline[1204];

	sprintf ( Statement, "select count(*), min(Dexdate), max(Dexdate) from dividend where Dticker = '%s'", xportfolio.xpticker );

	if ( dbySelectCB ( "invest", &MySql, Statement, (int(*)()) GetDates, LogFileName ) != 1 )
	{
		GoGetThem = 1;
	}
	else if ( DividendCount == 0 )
	{
		GoGetThem = 1;
	}
	else
	{
		if ( nsStrcmp ( xportfolio.xpdate, DividendMinDate ) < 0 )
		{
			GoGetThem = 1;
		}
		if ( nsStrcmp ( DividendMaxDate, Today ) > 30 )
		{
			GoGetThem = 1;
		}
	}

	if ( GoGetThem )
	{
		sprintf ( cmdline, "/usr/local/bin/getdiv -ticker %s", xportfolio.xpticker );
		ForkAndExec ( cmdline, PRINT_DESTINATION_SILENT );

		if ( dbySelectCB ( "invest", &MySql, Statement, (int(*)()) GetDates, LogFileName ) == 1 )
		{
			if ( nsStrcmp ( xportfolio.xpdate, DividendMinDate ) < 0 )
			{
				SafeError ( COLOR_WARN, "Earliest dividend %s is after your purchase date", DividendMinDate );
			}
		}
		else
		{
			SafeError ( COLOR_WARN, "No dividends found for %s", xportfolio.xpticker );
		}
	}
}
#endif

void SavePortfolio ( int RunMode )
{
	int		DoUpdate;

	switch ( RunMode )
	{
		case MODE_INSERT_PORTFOLIO:
			snprintf ( Statement, sizeof(Statement),
				"insert into portfolio ( Pmember, Pticker, Pdate, Pbenchmark ) values ( %ld, '%s', '%s', 'SPY' )",
						CookieMember.xid, xportfolio.xpticker, xportfolio.xpdate );
						
			if ( dbyInsert ( "SavePortfolio", &MySql, Statement, 0, LogFileName ) < 1 )
			{
				SafeError ( COLOR_ERROR, "SavePortfolio insert failed." );
				break;
			}
			
			SafeError ( COLOR_SUCCESS, "Added %s %s.", xportfolio.xpticker, xstock.xsname );

			sprintf ( WhereClause, "Pmember = %ld and Pticker = '%s' and Pdate = '%s'",
						CookieMember.xid, xportfolio.xpticker, xportfolio.xpdate );

			LoadPortfolio ( &MySql, WhereClause, &xtest, 0 );

			PortfolioID = xtest.xid;

			/* fall through */

		case MODE_UPDATE_PORTFOLIO:
			if ( RunMode == MODE_UPDATE_PORTFOLIO )
			{
				sprintf ( WhereClause, "portfolio.id = %ld",  PortfolioID );
				LoadPortfolio ( &MySql, WhereClause, &xtest, 0 );
			}
				
			sprintf ( WhereClause, "Sticker = '%s'", xtest.xpticker );
			LoadStock ( &MySql, WhereClause, &xstock, 0, 0 );

			if ( nsStrcmp ( xportfolio.xpdate, xstock.xslast ) > 0 )
			{
				sprintf ( WhereClause, "Hticker = '%s' and Hdate = '%s'", xstock.xsticker, xportfolio.xpdate );
			}
			else
			{
				sprintf ( WhereClause, "Hticker = '%s' and Hdate = '%s'", xstock.xsticker, xstock.xslast );
			}
			HistoryCount = LoadHistory ( &MySql, WhereClause, &xhistory, 0 );
		
			if ( nsStrlen ( xportfolio.xpoptexp  ) < 1 )
			{
				snprintf ( xportfolio.xpoptexp, sizeof(xportfolio.xpoptexp), "0000-00-00" );
			}

DiffYak ( "Pticker",    xportfolio.xpticker,    xtest.xpticker,     sizeof(xtest.xpticker),    DATATYPE_STRING );
DiffYak ( "Pdate",      xportfolio.xpdate,      xtest.xpdate ,      sizeof(xtest.xpdate),      DATATYPE_STRING );
DiffYak ( "Pshares",   &xportfolio.xpshares,    &xtest.xpshares,    sizeof(xtest.xpshares),    DATATYPE_DOUBLE );
DiffYak ( "Pprice",    &xportfolio.xpprice,     &xtest.xpprice,     sizeof(xtest.xpprice),     DATATYPE_DOUBLE );
DiffYak ( "Poptexp",    xportfolio.xpoptexp,    xtest.xpoptexp,     sizeof(xtest.xpoptexp),    DATATYPE_STRING );
DiffYak ( "Pbroker",    xportfolio.xpbroker,    xtest.xpbroker,     sizeof(xtest.xpbroker),    DATATYPE_STRING );
DiffYak ( "Pbenchmark", xportfolio.xpbenchmark, xtest.xpbenchmark,  sizeof(xtest.xpbenchmark), DATATYPE_STRING );

// DiffYak ( "Palert",    &xportfolio.xpalert,     &xtest.xpalert,     sizeof(xtest.xpalert),     DATATYPE_DOUBLE );

			if (( nsStrcmp ( xportfolio.xpticker,    xtest.xpticker    ) != 0 ) ||
				( nsStrcmp ( xportfolio.xpdate,      xtest.xpdate      ) != 0 ) ||
				( fabs ( xportfolio.xpshares - xtest.xpshares ) > 0.01 )       ||
				( fabs ( xportfolio.xpprice - xtest.xpprice ) > 0.01 )         ||
				( nsStrcmp ( xportfolio.xpoptexp,    xtest.xpoptexp    ) != 0 ) ||
				( xportfolio.xpbucket !=          xtest.xpbucket       ) ||
				( nsStrcmp ( xportfolio.xpbroker,    xtest.xpbroker    ) != 0 ))
			{
				snprintf ( Statement, sizeof(Statement),
"update portfolio set Pticker = '%s', Pdate = '%s', Pshares = %.4f, Pprice = %.4f, \
 Poptexp = '%s', Pbroker = '%s', Pbucket = %d  \
 where portfolio.id = %ld",
						xportfolio.xpticker, 
						xportfolio.xpdate,
						xportfolio.xpshares,
						xportfolio.xpprice,
						xportfolio.xpoptexp,
						xportfolio.xpbroker,
						xportfolio.xpbucket,
						PortfolioID );

				if ( dbyUpdate ( "SavePortfolio", &MySql, Statement, 0, LogFileName ) < 1 )
				{
					SafeError ( COLOR_ERROR, "SavePortfolio update 1 failed." );
					break;
				}
				else
				{
					SafeError ( COLOR_NORMAL, "SavePortfolio update okay." );
				}

#ifdef CAN_GET_DIVIDENDS
				if (( RunMode == MODE_INSERT_PORTFOLIO                    ) ||
					( nsStrcmp ( xportfolio.xpticker, xtest.xpticker ) != 0 ) ||
					( nsStrcmp ( xportfolio.xpdate,   xtest.xpdate   ) != 0 ))
				{
					GetDividends ();
				}
#endif
			}

			if ( xportfolio.xpalerttype[0] !=  xtest.xpalerttype[0] )
			{
				snprintf ( Statement, sizeof(Statement),
"update portfolio set Palerttype = '%c', Palertprice = 0.0, Ptrailpct = 0.0, Ptrailvalue = 0.0, Palert = 0.0 where portfolio.id = %ld",
						xportfolio.xpalerttype[0], PortfolioID );

				if ( dbyUpdate ( "SavePortfolio", &MySql, Statement, 0, LogFileName ) < 1 )
				{
					SafeError ( COLOR_ERROR, "SavePortfolio update 2 failed." );
					break;
				}
			}

			DoUpdate = 0;
			switch ( xportfolio.xpalerttype[0] )
			{
				case ALERT_ABOVE_AMT:
				case ALERT_BELOW_AMT:
					if ( xportfolio.xpalertprice !=  xtest.xpalertprice )
					{
						snprintf ( Statement, sizeof(Statement),
"update portfolio set Palertprice = %.2f, Ptrailpct = 0.0, Ptrailvalue = 0.0, Palert = 0.0 where portfolio.id = %ld",
								xportfolio.xpalertprice, PortfolioID );

						DoUpdate = 1;
					}
					break;

				case ALERT_TRAIL_STOP:
					if ( xportfolio.xptrailpct != xtest.xptrailpct )
					{
						if ( HistoryCount == 1 )
						{
							xportfolio.xptrailvalue = xhistory.xhclose * ( 1.0 - xportfolio.xptrailpct / 100.0 );
						}
						else
						{
							xportfolio.xptrailvalue = xportfolio.xpprice * ( 1.0 - xportfolio.xptrailpct / 100.0 );
						}

						snprintf ( Statement, sizeof(Statement),
"update portfolio set Palertprice = 0.0, Ptrailpct = %.2f, Ptrailvalue = %.2f, Palert = 0.0 where portfolio.id = %ld",
								xportfolio.xptrailpct, xportfolio.xptrailvalue, PortfolioID );

						DoUpdate = 1;
					}
					break;

				case ALERT_BENCHMARK:
					if (( nsStrcmp ( xportfolio.xpbenchmark, xtest.xpbenchmark ) != 0 ) ||
						( xportfolio.xpalert !=            xtest.xpalert     ))
					{
						snprintf ( Statement, sizeof(Statement),
"update portfolio set Palertprice = 0.0, Ptrailpct = 0.0, Ptrailvalue = 0.0, Pbenchmark = '%s', Palert = %.2f where portfolio.id = %ld",
								xportfolio.xpbenchmark, xportfolio.xpalert, PortfolioID );

						DoUpdate = 1;
					}
					break;

				case ALERT_NONE:
				case ALERT_ABOVE_MA5:
				case ALERT_ABOVE_MA10:
				case ALERT_ABOVE_MA20:
				case ALERT_ABOVE_MA50:
				case ALERT_ABOVE_MA100:
				case ALERT_ABOVE_MA200:
				case ALERT_BELOW_MA5:
				case ALERT_BELOW_MA10:
				case ALERT_BELOW_MA20:
				case ALERT_BELOW_MA50:
				case ALERT_BELOW_MA100:
				case ALERT_BELOW_MA200:
				default:
					break;
			}

			if ( DoUpdate )
			{
// SafeError ( COLOR_NORMAL, Statement );

				if ( dbyUpdate ( "SavePortfolio", &MySql, Statement, 0, LogFileName ) < 1 )
				{
					SafeError ( COLOR_ERROR, "SavePortfolio update 3 failed." );
					break;
				}
			}

			if ( RunMode ==  MODE_INSERT_PORTFOLIO )
			{
				if ( PortfolioRecordCount + 1 < PortfolioLimit )
				{
					/*----------------------------
						add mode
					----------------------------*/
					PortfolioID = 0;
				}
				else
				{
					/*----------------------------
						edit mode
					----------------------------*/
					SafeError ( COLOR_ERROR, "Portfolio is full (%d)", PortfolioLimit );

					snprintf ( WhereClause, sizeof(WhereClause),
						"Pmember = %ld and Pticker = '%s' and Pdate = '%s'", CookieMember.xid, xportfolio.xpticker, xportfolio.xpdate );

					LoadPortfolio ( &MySql, WhereClause, &xtest, 0 );
					PortfolioID = xtest.xid;
				}
			}
			break;
// History
		case MODE_DELETE_PORTFOLIO:
			snprintf ( Statement, sizeof(Statement),
				"delete from portfolio where portfolio.id = %ld", PortfolioID );

			if ( dbyDelete ( "SavePortfolio", &MySql, Statement, 0, LogFileName ) < 1 )
			{
				SafeError ( COLOR_ERROR, "SavePortfolio delete failed." );
			}
			else
			{
				PortfolioID = 0;
			}
			break;
	}

}

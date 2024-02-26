/*----------------------------------------------------------------------------
	Program : invest/TakeAction.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Do what the user requested.
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

static	 char	cmdline[1024];

void TakeAction ( char Format )
{
	char	fn[128];
	FILE	*fp;
//	time_t		tvec;
//	struct tm	*tm;
	DATEVAL	dvStart;
	DATEVAL	dvEnd;
	int		xi, xl, ndx, len, ToScreen = 0;
	char	Fragment[128];
	long	rv;
	
	if ( Format == RPT_FORMAT_HTML || Format == RPT_FORMAT_PDF_VIEW )
	{
		/*----------------------------------------------------------
			paint error / messages if any and BAIL
		----------------------------------------------------------*/
		if ( ErrorCount > 0 )
		{
			int		DivColor;

			DivColor = ErrorColor ();

			switch ( DivColor )
			{
				case COLOR_ERROR:
					printf ( "<div class='error border-error'>\n" );
					break;
				case COLOR_WARN:
					printf ( "<div class='error border-warn'>\n" );
					break;
				case COLOR_SUCCESS:
					printf ( "<div class='error border-success'>\n" );
					break;
				case COLOR_NORMAL:
					printf ( "<div class='error border-black'>\n" );
					break;
			}
			for ( xi = 0; xi < ErrorCount; xi++ )
			{
				printf ( "%s<br>\n", ErrorArray[xi].Message );
			}
			printf ( "</div>\n" );

			return;
		}
	}

	switch ( RunMode )
	{
		case MODE_UNKNOWN:
		case MODE_PAINT_PASSWORD:
		case MODE_PAINT_EDIT_PORTFOLIO:
		case MODE_PAINT_EDIT_WATCHLIST:
		case MODE_PAINT_EDIT_CASHBAL:
		case MODE_PAINT_IMPORT_WATCHLIST:
		case MODE_PAINT_IMPORT_PORTFOLIO:
		case MODE_PAINT_PORTFOLIO_AND_WATCHLIST:
		case MODE_CLEAR_STOCK:
			return;

		case MODE_REPORT_REGRESS:
		case MODE_REPORT_VOLUME_ACCUM:
		case MODE_REPORT_VOLUME_INCREASE:
		case MODE_REPORT_PORTFOLIO:
		case MODE_REPORT_WATCHLIST:
		case MODE_REPORT_EQUITY_FUNDAMENTALS:
		case MODE_REPORT_ETF_FUNDAMENTALS:
		case MODE_REPORT_AD_HISTORY:
		case MODE_REPORT_52_WEEK:
		case MODE_REPORT_MA_CROSSOVER:
		case MODE_REPORT_MA_RATIO:
		case MODE_REPORT_DI_CROSSOVER:
		case MODE_REPORT_ADX_TREND:
		case MODE_REPORT_BREAKOUT:
		case MODE_REPORT_SEC_IND_ALLOCATION:
		case MODE_REPORT_ASSET_ALLOCATION:
		case MODE_REPORT_RELATIVE_STRENGTH:
		case MODE_REPORT_COOPER:
		case MODE_REPORT_VALUE:
		case MODE_REPORT_CTB_STRENGTH:
			ndx = RunMode - SCREENER_AND_REPORT_OFFSET;
			break;
		default:
			ndx = -1;
			break;
	}

	switch ( RunMode )
	{
		case MODE_SIGNUP:
			Signup ();
			break;

		case MODE_VERIFY:
			Verify ();
			break;

		case MODE_LOGIN:
			Login ();
			break;

		case MODE_FORGOT:
			Forgot ();
			break;

		case MODE_CHANGE_EMAIL:
			SaveEmail ();
			break;

		case MODE_SAVE_PROFILE:
			SaveProfile ();
			break;
			
		case MODE_SAVE_PASSWORD:
		case MODE_CHANGE_PASSWORD:
			SavePassword ();
			break;

		case MODE_LOGOUT:
			Logout ();
			break;

		case MODE_CONTACT:
			sprintf ( fn, "/tmp/member_%d.txt", getpid() );
			if (( fp = fopen ( fn, "w" )) == (FILE *)0 )
			{
				SafeError ( COLOR_ERROR, "Cannot create file to send email." );
				return;
			}

			fprintf ( fp, "This is a message from %s:\n", URL_NAME );
			
			if ( CookieMember.xid == 0 )
			{
				fprintf ( fp, "Sent from IP ADDRESS %s\n", Remote_Address );
				if ( nsStrlen ( ContactEmail ) > 1 )
				{
					fprintf ( fp, "Email address %s\n", ContactEmail );
				}
				else
				{
					fprintf ( fp, "No email address given\n" );
				}
			}
			else
			{
				fprintf ( fp, "Send from member %s\n", xmember.xmname );
				fprintf ( fp, "Email address is %s\n", xmember.xmemail );
			}

			fprintf ( fp, "\n" );

			fprintf ( fp, "%s\n\n", ContactMessage );
			nsFclose ( fp );

			SendAttached ( WEBMASTER_ADDRESS, SUPPORT_ADDRESS,
						"", "", 	ContactSubject,
							0, fn,
							0, NULL );
			
			unlink ( fn );

			break;
		
		case MODE_REPORT_REGRESS:
#ifdef DEBUG
#define DEBUG
			SafeError ( COLOR_ERROR, 
				"%d, DateRange %c, StockIndex %c, TopCount %d, ReportFormat %c",
						ndx, DateRange[ndx], StockIndex[ndx], TopCount[ndx], ReportFormat[ndx] );
#endif
//			tvec = time ( NULL );
//			tm = localtime ( &tvec );
//			dvEnd.year  = tm->tm_year + 1900;
//			dvEnd.month = tm->tm_mon + 1;
//			dvEnd.day   = tm->tm_mday;

			CurrentDateval ( &dvEnd );

#ifdef DEBUG
SafeError ( COLOR_ERROR, "current: year %d month %d day %d", dvEnd.year, dvEnd.month, dvEnd.day );
SafeError ( COLOR_ERROR, "ndx %d DateRange %c", ndx, DateRange[ndx] );
#endif
			switch ( DateRange[ndx] )
			{
				case DATE_RANGE_SIX_MONTHS:
					if ( dvEnd.month > 7 )
					{
						dvStart.year4  = dvEnd.year4;
						dvStart.month = dvEnd.month - 6;
						dvStart.day   = dvEnd.day;
					}
					else
					{
						dvStart.year4  = dvEnd.year4 - 1;
						dvStart.month = dvEnd.month + 6;
						dvStart.day   = dvEnd.day;
					}
					if ( dvStart.day > 28 )
					{
						dvStart.day = 28;
					}
#ifdef DEBUG
SafeError ( COLOR_ERROR, "start: year %d month %d day %d", dvStart.year4, dvStart.month, dvStart.day );
#endif
					break;

				case DATE_RANGE_ONE_YEAR:
					dvStart.year4  = dvEnd.year4 - 1;
					dvStart.month = dvEnd.month;
					dvStart.day   = dvEnd.day;
					break;

				case DATE_RANGE_TWO_YEARS:
					dvStart.year4  = dvEnd.year4 - 2;
					dvStart.month = dvEnd.month;
					dvStart.day   = dvEnd.day;
					break;
			}

			sprintf ( cmdline, "rpt_regress %04d-%02d-%02d %04d-%02d-%02d %c -top %d -member %ld -sort %c",
				dvStart.year4,
				dvStart.month,
				dvStart.day,
				dvEnd.year4,
				dvEnd.month,
				dvEnd.day,
				StockIndex[ndx],
				TopCount[ndx],
				CookieMember.xid,
				SortBy[ndx] );

			
			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
				case RPT_FORMAT_APPEND_W: strcat ( cmdline, " -fmt aw" ); break;
				case RPT_FORMAT_REPLACE_W: strcat ( cmdline, " -fmt rw" ); break;
				case RPT_FORMAT_TEMP: strcat ( cmdline, " -fmt tmp" ); break;
			}

#ifdef DEBUG
SafeError ( COLOR_ERROR, "%s", cmdline );
#endif
			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_EQUITY_FUNDAMENTALS:
		case MODE_REPORT_ETF_FUNDAMENTALS:
			snprintf ( Statement, sizeof(Statement), "select Fticker, Sname" );

			if ( nsStrchr ( StockIndexList, 'A' ) != NULL )
			{
				nsStrcat ( Statement, ", Fcountry" );
			}

			/*----------------------------
				field list
			----------------------------*/
			for ( xi = 0; xi < ScreenerMinMaxCount; xi++ )
			{
				nsStrcat ( Statement, "," );
				nsStrcat ( Statement, ScreenerMinMaxArray[xi].Field );
			}

			/*----------------------------
				start where clause
			----------------------------*/
			nsStrcat ( Statement, " from stock, fundamental where Sticker = Fticker" );

			/*----------------------------
				stock index list
			----------------------------*/
			if ( nsStrchr ( StockIndexList, 'E' ) != NULL )
			{
				nsStrcat ( Statement, " and Stype = 'E'" );
			}
			else if ( nsStrchr ( StockIndexList, 'O' ) != NULL )
			{
nsStrcat ( Statement, 
	" and Stype != 'E' and Sdj != 'Y' and Ssp500 != 'Y' and Srussell != '1' and Srussell != '2' and Snasdaq != 'Y'" );
			}
			else 
			{
				len = nsStrlen ( StockIndexList ); 

				if ( len == 1 )
				{
					nsStrcat ( Statement, " and" );
				}
				else
				{
					nsStrcat ( Statement, " and (" );
				}

				for ( xi = 0; xi < len; xi++ )
				{
					if ( xi > 0 )
					{
						nsStrcat ( Statement, " or" );
					}
					switch ( StockIndexList[xi] )
					{
						case 'D':
							nsStrcat ( Statement, " Sdj = 'Y'" );
							break;
						case 'S':
							nsStrcat ( Statement, " Ssp500 = 'Y'" );
							break;
						case '1':
							nsStrcat ( Statement, " Srussell = '1'" );
							break;
						case '2':
							nsStrcat ( Statement, " Srussell = '2'" );
							break;
						case 'N':
							nsStrcat ( Statement, " Snasdaq = 'Y'" );
							break;
						case 'A':
							nsStrcat ( Statement, " Stype = 'A'" );
							break;
						case 'E':
							nsStrcat ( Statement, " Stype = 'E'" );
							break;
					}
				}
			}
			if ( len > 1 )
			{
				nsStrcat ( Statement, ")" );
			}

			/*----------------------------
				ranges
			----------------------------*/
			for ( xi = 0; xi < ScreenerMinMaxCount; xi++ )
			{
				switch ( ScreenerMinMaxArray[xi].DataType )
				{
					case DATATYPE_STRING:
						if ( nsStrlen(ScreenerMinMaxArray[xi].Minimum.String) > 0 )
						{
							sprintf ( Fragment, " and %s >= '%s'", ScreenerMinMaxArray[xi].Field, ScreenerMinMaxArray[xi].Minimum.String );
							nsStrcat ( Statement, Fragment );
						}
						if ( nsStrlen(ScreenerMinMaxArray[xi].Maximum.String) > 0 )
						{
							sprintf ( Fragment, " and %s <= '%s'", ScreenerMinMaxArray[xi].Field, ScreenerMinMaxArray[xi].Maximum.String );
							nsStrcat ( Statement, Fragment );
						}
						break;
					case DATATYPE_DOUBLE:
						if ( ScreenerMinMaxArray[xi].Minimum.Double != 0.0 )
						{
							sprintf ( Fragment, " and %s >= %f", ScreenerMinMaxArray[xi].Field, ScreenerMinMaxArray[xi].Minimum.Double );
							nsStrcat ( Statement, Fragment );
						}
						if ( ScreenerMinMaxArray[xi].Maximum.Double != 0.0 )
						{
							sprintf ( Fragment, " and %s <= %f", ScreenerMinMaxArray[xi].Field, ScreenerMinMaxArray[xi].Maximum.Double );
							nsStrcat ( Statement, Fragment );
						}
						break;
					case DATATYPE_INT:
						if ( ScreenerMinMaxArray[xi].Minimum.Integer != 0.0 )
						{
							sprintf ( Fragment, " and %s >= %d", ScreenerMinMaxArray[xi].Field, ScreenerMinMaxArray[xi].Minimum.Integer );
							nsStrcat ( Statement, Fragment );
						}
						if ( ScreenerMinMaxArray[xi].Maximum.Integer != 0.0 )
						{
							sprintf ( Fragment, " and %s <= %d", ScreenerMinMaxArray[xi].Field, ScreenerMinMaxArray[xi].Maximum.Integer );
							nsStrcat ( Statement, Fragment );
						}
						break;
				}

			}
			nsStrcat ( Statement, ";" );

#ifdef DEBUG
#define DEBUG
			printf ( "TakeAction: %s<br>\n", Statement );
#endif
			sprintf ( fn, "%s/rpt_fund_%d.sql", TEMPDIR, getpid() );
			if (( fp = fopen ( fn, "w" )) == (FILE *)0 )
			{
				SafeError ( COLOR_ERROR, "Cannot create SQL script file" );
				return;
			}

			fprintf ( fp, "%s", Statement );

			nsFclose ( fp );

			sprintf ( cmdline, "rpt_fund -script %s -member %ld", fn, CookieMember.xid );
// fixit --- add debug option on screen for admins

			switch ( ReportFormat[ndx] )
			{
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
			}
#ifdef DEBUG
			printf ( "%s<br>\n", cmdline );
#endif
#undef DEBUG
			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );

			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_AD_HISTORY:
			switch ( DateRange[ndx] )
			{
				case DATE_RANGE_ONE_MONTH:
					sprintf ( cmdline, "rpt_ad_hist -1mo -member %ld", CookieMember.xid );
					break;

				case DATE_RANGE_SIX_MONTHS:
					sprintf ( cmdline, "rpt_ad_hist -6mo -member %ld", CookieMember.xid );
					break;

				case DATE_RANGE_ONE_YEAR:
					sprintf ( cmdline, "rpt_ad_hist -1yr -member %ld", CookieMember.xid );
					break;
			}

			
			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
			}

#ifdef DEBUG
SafeError ( COLOR_ERROR, "%s", cmdline );
#endif
			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_52_WEEK:
			sprintf ( cmdline, "rpt_near52 %c -member %ld -months %d -within %d -dip %d", 
					StockIndex[ndx], 
					CookieMember.xid,
					NumberOfDays[ndx],
					NumberOfDays1[ndx],
					NumberOfDays2[ndx] );
			
			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
				case RPT_FORMAT_APPEND_W: strcat ( cmdline, " -fmt aw" ); break;
				case RPT_FORMAT_REPLACE_W: strcat ( cmdline, " -fmt rw" ); break;
				case RPT_FORMAT_TEMP: strcat ( cmdline, " -fmt tmp" ); break;
			}

			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;


		case MODE_REPORT_VOLUME_ACCUM:
		case MODE_REPORT_VOLUME_INCREASE:
			sprintf ( cmdline, "rpt_volume %c %c -member %ld -pcnt %.2f -vol %d",
				RunMode == MODE_REPORT_VOLUME_ACCUM ? 'A' : 'I',
				StockIndex[ndx],
				CookieMember.xid,
				TopPercent[ndx],
				MinimumVolume[ndx] );

			switch ( RunMode )
			{
				case MODE_REPORT_VOLUME_ACCUM:
#ifdef CRAP
					snprintf ( Fragment, sizeof(Fragment), " -days %d -method %c",
						NumberOfDays[ndx],
						CalcVolumeMethod[ndx] == VOLUME_METHOD_OPEN_CLOSE ? 'O' : 'P' );
#endif
					snprintf ( Fragment, sizeof(Fragment), " -days %d", NumberOfDays[ndx] );
					break;

				case MODE_REPORT_VOLUME_INCREASE:
					sprintf ( Fragment, " -compare %d %d", NumberOfDays2[ndx], NumberOfDays1[ndx] );
					break;
			}
			strcat ( cmdline, Fragment );

			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
				case RPT_FORMAT_APPEND_W: strcat ( cmdline, " -fmt aw" ); break;
				case RPT_FORMAT_REPLACE_W: strcat ( cmdline, " -fmt rw" ); break;
				case RPT_FORMAT_TEMP: strcat ( cmdline, " -fmt tmp" ); break;
			}

			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_PORTFOLIO:
			sprintf ( cmdline, "rpt_portfolio -member %ld", CookieMember.xid );
			switch ( ReportStyle[ndx] )
			{
				case STYLE_BASE:  strcat ( cmdline, " -style base" );  break;
				case STYLE_FUNDAMENTAL:  strcat ( cmdline, " -style fundamental" );  break;
				case STYLE_TODAY:  strcat ( cmdline, " -style today" );  break;
				case STYLE_ALERTS: strcat ( cmdline, " -style alerts" ); break;
				case STYLE_FULL: strcat ( cmdline, " -style full" ); break;
				case STYLE_OVERNIGHT:  strcat ( cmdline, " -style overnight" );  break;
				case STYLE_YOY:  strcat ( cmdline, " -style yoy" );  break;
				case STYLE_MSTAR: strcat ( cmdline, " -style mstar" ); break;
				case STYLE_CUSTOM: strcat ( cmdline, " -style custom" ); break;
				case STYLE_BUCKETS: 
					sprintf ( cmdline, "rpt_buckets -member %ld", CookieMember.xid );
					break;
				default:
					// SafeError ( COLOR_ERROR, "TakeAction: unknown style %d.", ReportStyle[ndx] );
					printf ( "TakeAction: unknown style %d.<br>\n", ReportStyle[ndx] );
					return;
			}
 				//-type {all*|stock|etf|fixed|cash}
			switch ( StockType[ndx] )
			{
				case 'A': strcat ( cmdline, " -type all" ); break;
				case 'S': strcat ( cmdline, " -type stock" ); break;
				case 'E': strcat ( cmdline, " -type etf" ); break;
				case 'F': strcat ( cmdline, " -type fixed" ); break;
				case 'C': strcat ( cmdline, " -type cash" ); break;
			}
			if (( xl = nsStrlen ( Broker[ndx] )) > 0 )
			{
				for ( xi = 0; xi < xl; xi++ )
				{
					if ( Broker[ndx][xi] == ' ' )
					{
						Broker[ndx][xi] = '_';
					}
				}
				sprintf ( Fragment, " -broker %s", Broker[ndx] );
				strcat ( cmdline, Fragment );
			}
			switch ( SortBy[ndx] )
			{
				case SORTBY_TICKER:   strcat ( cmdline, " -sort ticker" ); break;
				case SORTBY_GAIN:     strcat ( cmdline, " -sort gain" ); break;
				case SORTBY_RETURN:   strcat ( cmdline, " -sort roi" ); break;
				case SORTBY_COMPARED: strcat ( cmdline, " -sort compared" ); break;
			}
			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
				case RPT_FORMAT_APPEND_W: strcat ( cmdline, " -fmt aw" ); break;
				case RPT_FORMAT_REPLACE_W: strcat ( cmdline, " -fmt rw" ); break;
				case RPT_FORMAT_TEMP: strcat ( cmdline, " -fmt tmp" ); break;
			}
			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_SEC_IND_ALLOCATION:
			sprintf ( cmdline, "rpt_allocation -member %ld", CookieMember.xid );
			switch ( ReportStyle[ndx] )
			{
				case STYLE_BASE:   strcat ( cmdline, " -style sector" );  break;
				case STYLE_ALERTS: strcat ( cmdline, " -style industry" ); break;
				case STYLE_FULL:   strcat ( cmdline, " -style both" ); break;
			}
			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
			}
			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_ASSET_ALLOCATION:
			sprintf ( cmdline, "rpt_target -member %ld", CookieMember.xid );
			InsertRptlog ( cmdline );
			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
			}
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_WATCHLIST:
			sprintf ( cmdline, "rpt_watchlist -member %ld", CookieMember.xid );
			switch ( ReportStyle[ndx] )
			{
				case STYLE_BASE:  		strcat ( cmdline, " -style base" );  break;
				case STYLE_FUNDAMENTAL: strcat ( cmdline, " -style fundamental" );  break;
			}
			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
			}
			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_MA_CROSSOVER:
			sprintf ( cmdline, "rpt_macross %c -avgs %d %d -days %d -type %c -member %ld", 
					StockIndex[ndx], NumberOfDays1[ndx], NumberOfDays2[ndx], NumberOfDays[ndx], BuySellAll[ndx], CookieMember.xid );
			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
				case RPT_FORMAT_APPEND_W: strcat ( cmdline, " -fmt aw" ); break;
				case RPT_FORMAT_REPLACE_W: strcat ( cmdline, " -fmt rw" ); break;
				case RPT_FORMAT_TEMP: strcat ( cmdline, " -fmt tmp" ); break;
			}
			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_MA_RATIO:
			sprintf ( cmdline, "rpt_maratio %c -avgs %d %d -days %d -minimum %.2f -member %ld", 
					StockIndex[ndx], 
					NumberOfDays1[ndx], NumberOfDays2[ndx], 
					NumberOfDays[ndx], 
					TopPercent[ndx],
					CookieMember.xid );

			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
				case RPT_FORMAT_APPEND_W: strcat ( cmdline, " -fmt aw" ); break;
				case RPT_FORMAT_REPLACE_W: strcat ( cmdline, " -fmt rw" ); break;
				case RPT_FORMAT_TEMP: strcat ( cmdline, " -fmt tmp" ); break;
			}

			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_DI_CROSSOVER:
/*---------------------------------------------------------------------------
USAGE: rpt_dicross -index  index  -member ID [options]
USAGE: rpt_dicross -ticker TICKER -member ID [options]
index:
 S = S&P 500
 D = Dow Jones Industrials
 N = Nasdaq 100
 M = Midcap (Russell 1000 ex S&P 500)
 1 = Russell 1000
 2 = Russell 2000
 3 = Russell 3000
 A = All stocks
 O = Not indexed
 -member id
 -fmt = csv (default), txt, html, xlsx, pdf (n/a), aw, rw or tmp
Options:
 -days #       = cross over during past # of days, default 1
 -buyadx #     = minimum ADX for buy, default 30.0
 -buyamt #     = Dollar to buy, default 1000.0
 -minshares #  = Minimum shares to buy. Default 10
 -d   = Debug
---------------------------------------------------------------------------*/
			sprintf ( cmdline, "rpt_dicross -index %c -member %ld -days %d -buyadx %.2f -buyamt %d -minshares %d", 
					StockIndex[ndx], 
					CookieMember.xid,
					NumberOfDays[ndx],
					TopPercent[ndx],
					NumberOfDays1[ndx], 
					NumberOfDays2[ndx] );

			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
				case RPT_FORMAT_APPEND_W: strcat ( cmdline, " -fmt aw" ); break;
				case RPT_FORMAT_REPLACE_W: strcat ( cmdline, " -fmt rw" ); break;
				case RPT_FORMAT_TEMP: strcat ( cmdline, " -fmt tmp" ); break;
			}
			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_ADX_TREND:
/*---------------------------------------------------------------------------
USAGE: rpt_adx -index  index  -member ID [options]
USAGE: rpt_adx -ticker TICKER -member ID [options]
index:
 S = S&P 500
 D = Dow Jones Industrials
 N = Nasdaq 100
 M = Midcap (Russell 1000 ex S&P 500)
 1 = Russell 1000
 2 = Russell 2000
 3 = Russell 3000
 A = All stocks
 O = Not indexed
 P = Portfolio
 W = Watchlist
 Z = Temp file
 -member id
 -fmt = csv (default), txt, html, xlsx, pdf (n/a), aw, rw or tmp
Options:
 -minadx #     = minimum ADX, default 20.0
 -min_di #     = minimum DI difference, default 10.0
 -type B|S|A  = looking for Buy, Sell or All
 -d   = Debug
---------------------------------------------------------------------------*/
			sprintf ( cmdline, "rpt_adx -index %c -type %c -member %ld -minadx %d -min_di %d", 
					StockIndex[ndx], 
					BuySellAll[ndx],
					CookieMember.xid,
					NumberOfDays1[ndx], 
					NumberOfDays2[ndx] );

			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
				case RPT_FORMAT_APPEND_W: strcat ( cmdline, " -fmt aw" ); break;
				case RPT_FORMAT_REPLACE_W: strcat ( cmdline, " -fmt rw" ); break;
				case RPT_FORMAT_TEMP: strcat ( cmdline, " -fmt tmp" ); break;
			}
			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_BREAKOUT:
//printf ( "ndx %d format %c<br>\n", ndx, ReportFormat[ndx] );

			sprintf ( cmdline, "rpt_breakout %c -member %ld -volume %.0f -minvol %d -price %d", 
				StockIndex[ndx], CookieMember.xid, TopPercent[ndx], MinimumVolume[ndx], NumberOfDays[ndx] );

			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
				case RPT_FORMAT_APPEND_W: strcat ( cmdline, " -fmt aw" ); break;
				case RPT_FORMAT_REPLACE_W: strcat ( cmdline, " -fmt rw" ); break;
				case RPT_FORMAT_TEMP: strcat ( cmdline, " -fmt tmp" ); break;
			}
//printf ( "ndx %d format %c<br>\n", ndx, ReportFormat[ndx] );
//printf ( "cmd %s<br>\n", cmdline );
			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_RELATIVE_STRENGTH:
			sprintf ( cmdline, "rpt_rsi %c -member %ld", 
				StockIndex[ndx], CookieMember.xid );

			switch ( SortBy[ndx] )
			{
				case SORTBY_NONE:     strcat ( cmdline, " -group none" ); break;
				case SORTBY_SECTOR:   strcat ( cmdline, " -group sector" ); break;
				case SORTBY_INDUSTRY: strcat ( cmdline, " -group industry" ); break;
			}

			switch ( TopCountOrMinimumRS )
			{
				case 'T':
					sprintf ( Fragment, " -top %d", Value );
					break;
				case 'B':
					sprintf ( Fragment, " -bot %d", Value );
					break;
				case 'N':
					sprintf ( Fragment, " -min %d", Value );
					break;
				case 'X':
					sprintf ( Fragment, " -max %d", Value );
					break;
			}

			strcat ( cmdline, Fragment );

			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
				case RPT_FORMAT_APPEND_W: strcat ( cmdline, " -fmt aw" ); break;
				case RPT_FORMAT_REPLACE_W: strcat ( cmdline, " -fmt rw" ); break;
				case RPT_FORMAT_TEMP: strcat ( cmdline, " -fmt tmp" ); break;
			}
//printf ( "ndx %d format %c<br>\n", ndx, ReportFormat[ndx] );
//printf ( "cmd %s<br>\n", cmdline );
			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_CTB_STRENGTH:
			sprintf ( cmdline, "rpt_ctb %c -member %ld", StockIndex[ndx], CookieMember.xid );

//			switch ( SortBy[ndx] )
//			{
//				case SORTBY_NONE:     strcat ( cmdline, " -group none" ); break;
//				case SORTBY_SECTOR:   strcat ( cmdline, " -group sector" ); break;
//				case SORTBY_INDUSTRY: strcat ( cmdline, " -group industry" ); break;
//			}

			switch ( TopCountOrMinimumRS )
			{
				case 'T':
					sprintf ( Fragment, " -top %d", Value );
					break;
				case 'B':
					sprintf ( Fragment, " -bot %d", Value );
					break;
				case 'N':
					sprintf ( Fragment, " -min %d", Value );
					break;
				case 'X':
					sprintf ( Fragment, " -max %d", Value );
					break;
			}

			strcat ( cmdline, Fragment );

			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
				case RPT_FORMAT_APPEND_W: strcat ( cmdline, " -fmt aw" ); break;
				case RPT_FORMAT_REPLACE_W: strcat ( cmdline, " -fmt rw" ); break;
				case RPT_FORMAT_TEMP: strcat ( cmdline, " -fmt tmp" ); break;
			}
//printf ( "ndx %d format %c<br>\n", ndx, ReportFormat[ndx] );
//printf ( "cmd %s<br>\n", cmdline );

//printf ( "%s<br>\n", cmdline );
//break;

			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_COOPER:
			sprintf ( cmdline, "rpt_cooper %c -member %ld", 
				StockIndex[ndx], CookieMember.xid );

			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
				case RPT_FORMAT_APPEND_W: strcat ( cmdline, " -fmt aw" ); break;
				case RPT_FORMAT_REPLACE_W: strcat ( cmdline, " -fmt rw" ); break;
				case RPT_FORMAT_TEMP: strcat ( cmdline, " -fmt tmp" ); break;
			}

			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_REPORT_VALUE:
			sprintf ( cmdline, "rpt_value -member %ld -index %c -currentAAA %.2f -range %.2f", 
				CookieMember.xid, StockIndex[ndx], CorpAAA, RangeBHS );

			switch ( ReportFormat[ndx] )
			{
				case 'C': strcat ( cmdline, " -fmt csv" ); break;
				case 'T': strcat ( cmdline, " -fmt txt" ); break;
				case 'H': strcat ( cmdline, " -fmt html" ); ToScreen = 1; PaintReportTop(); break;
				case 'P': strcat ( cmdline, " -fmt pdf" ); break;
				case 'X': strcat ( cmdline, " -fmt xlsx" ); break;
				// case RPT_FORMAT_APPEND_W: strcat ( cmdline, " -fmt aw" ); break;
				// case RPT_FORMAT_REPLACE_W: strcat ( cmdline, " -fmt rw" ); break;
				// case RPT_FORMAT_TEMP: strcat ( cmdline, " -fmt tmp" ); break;
			}

/*----------------------------------------------------------
	fixit	 -currentAAA #.# current yield of AAA corp bonds, default 3.0 percent
	fixit	 -range #.#, percent for Buy/Hold/Sell, default is 15.0 percent
----------------------------------------------------------*/

			InsertRptlog ( cmdline );
			ForkAndExec ( cmdline, ReportFormat[ndx] == 'H' ? PRINT_DESTINATION_STDOUT : PRINT_DESTINATION_ERROR_ARRAY );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_PRINT_SHEET:
			sprintf ( cmdline, "portdtl -port %ld -fmt html", PortfolioID );
			ToScreen = 1;
			PaintReportTop ();
			ForkAndExec ( cmdline, PRINT_DESTINATION_STDOUT );
			if ( ToScreen ) { PaintReportBottom (); }
			break;

		case MODE_TV_CHART:
printf ( "<div id='tv-widget-chart'>\n" );
printf ( "<!--\n" );
printf ( "TradingView Widget BEGIN\n" );
printf ( "-->\n" );
printf ( "<script src='https://d1pbjj5jf5bdxg.cloudfront.net/packs/tradingview-0d8f743e38e5a9dfe978.js' defer='defer'></script>\n" );
printf ( "<script>\n" );
printf ( "  $(document).ready(function() {\n" );
printf ( "    new TradingView.widget({\n" );
printf ( "      'container_id': 'tv-widget-chart',\n" );
printf ( "      'width': '100%%',\n" );
printf ( "      'height': '400',\n" );
printf ( "      'symbol': '%s',\n", xportfolio.xpticker );
printf ( "      'interval': 'D',\n" );
printf ( "      'timezone': 'exchange',\n" );
printf ( "      'theme': 'White',\n" );
printf ( "      'style': '1',\n" );
printf ( "      'locale': 'en',\n" );
printf ( "      'toolbar_bg': '#f1f3f6',\n" );
printf ( "      'allow_symbol_change': true,\n" );
printf ( "      'hideideas': true,\n" );
printf ( "      'studies': ['MASimple@tv-basicstudies'],\n" );
printf ( "      'show_popup_button': true,\n" );
printf ( "      'popup_width': '1000',\n" );
printf ( "      'popup_height': '650',\n" );
printf ( "      'referral_id': '4009'\n" );
printf ( "    });\n" );
printf ( "  });\n" );
printf ( "</script>\n" );
printf ( "<!--\n" );
printf ( "TradingView Widget END\n" );
printf ( "-->\n" );
printf ( "</div>\n" );

			break;

		case MODE_WATCHLIST_CHART:
			PaintChart ( RunMode, xwatchlist.xwticker );
			break;

		case MODE_PORTFOLIO_CHART:
			PaintChart ( RunMode, xportfolio.xpticker );
			break;

		case MODE_COMBINED_CHART:
			PaintCombinedChart ( RunMode, xportfolio.xpticker );
			break;

		case MODE_STOCK_CHART:
			PaintChart ( RunMode, MaintStock.xsticker );
			break;

		case MODE_STOCK_DIVIDENDS:
			PaintStockDividends ( RunMode, MaintStock.xsticker );
			break;

		case MODE_PORTFOLIO_DIVIDENDS:
			PaintStockDividends ( RunMode, xportfolio.xpticker );
			break;

		case MODE_STOCK_FUNDAMENTALS_RAW:
			sprintf ( WhereClause, "Sticker = '%s'", MaintStock.xsticker );
			LoadStock ( &MySql, WhereClause, &MaintStock, 0, 0 );
			PaintFundamentals ( MaintStock.xsticker, MaintStock.xsname, MaintStock.xstype, MaintStock.xscik );
			break;

		case MODE_STOCK_FUNDAMENTALS_FMT:
			printf ( "Not finished<br>\n" );
			break;

		case MODE_STOCK_STATS_RAW:
			sprintf ( WhereClause, "Sticker = '%s'", MaintStock.xsticker );
			LoadStock ( &MySql, WhereClause, &MaintStock, 0, 0 );
			PaintStatsRaw ( MaintStock.xsticker, MaintStock.xsname );
			break;

		case MODE_CHART_PREVIOUS:
		case MODE_CHART_NEXT:
			PaintChart ( RunMode, " " );
			break;

		case MODE_SAVE_STOCK:
			SaveStock ();
			break;

		case MODE_DELETE_STOCK:
			DeleteTickersArray[0] = MaintStock.xsticker;
			DeleteTickersCount = 1;
			/* FALL-THROUGH */
		case MODE_DELETE_STOCKS:
			DeleteStocks ();
			break;

		case MODE_UPDATE_WATCHLIST:
		case MODE_DELETE_WATCHLIST:
		case MODE_INSERT_WATCHLIST:
			SaveWatchlist ( RunMode );
			RunMode = MODE_PAINT_EDIT_WATCHLIST;
			break;

		case MODE_IMPORT_WATCHLIST:
			RunMode = MODE_PAINT_EDIT_WATCHLIST;
			break;

		case MODE_IMPORT_PORTFOLIO:
			RunMode = MODE_PAINT_EDIT_PORTFOLIO;
			break;

		case MODE_UPDATE_PORTFOLIO:
		case MODE_DELETE_PORTFOLIO:
		case MODE_INSERT_PORTFOLIO:
			SavePortfolio ( RunMode );
			RunMode = MODE_PAINT_EDIT_PORTFOLIO;
			break;

		case MODE_FIND_STOCK:
			sprintf ( WhereClause, "Sticker = '%s'", MaintStock.xsticker );
			if ( LoadStock ( &MySql, WhereClause, &MaintStock, 0, 0 ) != 1 )
			{
				SafeError ( COLOR_ERROR, "%s is not on file.", MaintStock.xsticker );
				SafeError ( COLOR_ERROR, "You may add it if you have all the information." );
				break;
			}
			/*----------------------------------------------------------
				load fundamentals
			----------------------------------------------------------*/
			sprintf ( WhereClause, "Fticker = '%s'", MaintStock.xsticker );
			if ( LoadFundamental ( &MySql, WhereClause, &xfundamental, 0 ) != 1 )
			{
				memset ( &xfundamental, '\0', sizeof(xfundamental) );
			}

			LoadSystem ( &MySql, &xsystem, 0 );
			xhistory.xhclose = 0.0;

			sprintf ( WhereClause, "sector.id = %ld", MaintStock.xssector );
			if (( rv = LoadSector ( &MySql, WhereClause, &xsector, 1 )) != 1 )
			{
				sprintf ( xsector.xsecname, "not found" );
			}

			sprintf ( WhereClause, "industry.id = %ld", MaintStock.xsindustry );
			if (( rv = LoadIndustry ( &MySql, WhereClause, &xindustry, 1 )) != 1 )
			{
				sprintf ( xindustry.xindname, "not found" );
			}

			if ( MaintStock.xslast[0] > '0' )
			{
				sprintf ( WhereClause, "Hticker = '%s' and Hdate = '%s'", MaintStock.xsticker, MaintStock.xslast );
				if ( LoadHistory ( &MySql, WhereClause, &xhistory, 0 ) != 1 )
				{
					SafeError ( COLOR_ERROR, "Could not find history for %s on %s.", MaintStock.xsticker, MaintStock.xslast );
				}
				sprintf ( WhereClause, "Aticker = '%s' and Adate = '%s'", MaintStock.xsticker, MaintStock.xslast );
				if ( LoadAverage ( &MySql, WhereClause, &xaverage, 0 ) != 1 )
				{
					SafeError ( COLOR_ERROR, "Could not find averages for %s on %s.", MaintStock.xsticker, MaintStock.xslast );
				}
			}
			break;

		case MODE_INSERT_STOCK:
			sprintf ( Statement, 
"insert into stock (Sticker,Sname,Stype,Stype2,Sdj,Snasdaq,Ssp500,Srussell,Sdomfor) values ( '%s','%s','%c','%c','%c','%c','%c','%c','%c')",
					MaintStock.xsticker, MaintStock.xsname, MaintStock.xstype[0], MaintStock.xstype2[0],
					MaintStock.xsdj[0], MaintStock.xsnasdaq[0], MaintStock.xssp500[0], MaintStock.xsrussell[0],
					MaintStock.xsdomfor[0] );

			rv = dbyInsert ( "invest", &MySql, Statement, 0, LogFileName );

			if ( rv != 1 )
			{
				SafeError ( COLOR_ERROR, "Cannot add %s.", MaintStock.xsticker );
			}
			else
			{
				sprintf ( cmdline, "getdata -one %s -2yr -slast -fmt csv -q", MaintStock.xsticker );
				ForkAndExec ( cmdline, PRINT_DESTINATION_ERROR_ARRAY );

				sprintf ( cmdline, "getfundSEC -ticker %s -v", MaintStock.xsticker );
				ForkAndExec ( cmdline, PRINT_DESTINATION_ERROR_ARRAY );

				// new stock, don't have CIK number
				// sprintf ( cmdline, "UpdateFundamental -ticker %s -v", MaintStock.xsticker );
				// ForkAndExec ( cmdline, PRINT_DESTINATION_ERROR_ARRAY );

				sprintf ( cmdline, "UpdateAverage -ticker %s A", MaintStock.xsticker );
				ForkAndExec ( cmdline, PRINT_DESTINATION_ERROR_ARRAY );

				sprintf ( WhereClause, "Sticker = '%s'", MaintStock.xsticker );
				LoadStock ( &MySql, WhereClause, &MaintStock, 0, 0 );

				LoadSystem ( &MySql, &xsystem, 0 );
			}

			break;

		case MODE_REMOVE_ALL_WATCHLIST:
		case MODE_REMOVE_NO_ALERT_WATCHLIST:
			sprintf ( Statement, "delete from watchlist where Wmember = %ld", CookieMember.xid );
			if ( RunMode == MODE_REMOVE_NO_ALERT_WATCHLIST )
			{
				sprintf ( Fragment, " and Walerttype = '%c'", ALERT_NONE );
				strcat ( Statement, Fragment );
			}
			rv = dbyDelete ( "invest", &MySql, Statement, 0, LogFileName );
			if ( rv < 1 )
			{
				SafeError ( COLOR_ERROR, "Remove stocks from watchlist failed" );
			}
			break;

		case MODE_PROCESS_PAYMENT:
			ProcessPayment ();
			break;

#ifdef STUFF
		case MODE_ORDER_ENTRY_BUY:
		case MODE_ORDER_ENTRY_DRIP:
		case MODE_ORDER_ENTRY_SELL:
		case MODE_ORDER_ENTRY_CLEAR:
		case MODE_ORDER_ENTRY_PAINT:
			break;

		case MODE_CASH_ACCOUNT:
		case MODE_CASH_ACCOUNT_SAVE:
		case MODE_CASH_ACCOUNT_CLEAR:
			break;

		case MODE_CASH_ACCOUNT_CONFIRM:
			SaveCashAccount ();
			break;
#endif

		case MODE_UPDATE_CASHBAL:
		case MODE_DELETE_CASHBAL:
		case MODE_INSERT_CASHBAL:
			SaveCashBalance ();
			break;
		
		default:
			SafeError ( COLOR_ERROR, "TakeAction: unknown code %d.", RunMode );
			break;
	}
}

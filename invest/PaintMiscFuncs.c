/*----------------------------------------------------------------------------
	Program : invest/PaintMiscFuncs.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Miscellaneous paint functions, mostly used by stories on
				the screeners and portfolio pages.
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

void PushSignup ()
{
	if ( CookieMember.xid )
	{
		return;
	}

	sprintf ( WhereClause, "Mipaddr = '%s'", getenv("REMOTE_ADDR") );
	if ( LoadMember ( &MySql, WhereClause, &xmember, 0 ) < 1 )
	{
		/*----------------------------------------------------------
			This some fancy shit, because javascript function
			PushSignup and PushLogin did not work on iPhone/Safari
		----------------------------------------------------------*/
		printf ( "<a href='invest.cgi?MenuIndex=6' class='btn btn-danger btn-lg'>Sign Up!</a>\n" );
	}
	else
	{
		printf ( "<a href='invest.cgi?MenuIndex=7' class='btn btn-success btn-lg'>Login!</a>\n" );
	}
}

void PrintUpgradeMessage ( int WhichMessage )
{
	switch ( WhichMessage )
	{
		case 1:
			printf ( "Free membership, with limited access.<br>\n" );
			break;
		case 2:
			printf ( "You have run %ld reports.<br>\n", RptlogRecordCount );
			break;
	}

	printf ( "<b>Upgrade to full access for introductory price of $%d/year.</b><br>\n", INTRODUCTORY_PRICE );
	printf ( "<b>When you UPGRADE your account you can earn $%d for each paid referral!</b><br>", REFERRAL_FEE  );
}

void PaintStartReportStory ( char *HeadingCode, char *Title, int IsIn )
{
	static	int		StoryNumber = 1;

	printf ( "\n<!----  start of story ---->\n\n" );
	printf ( "    <div class='panel panel-default'>\n" );
	printf ( "      <div class='panel-heading'>\n" );
	printf ( "        <%s class='panel-title'>\n", HeadingCode );
	printf ( "          <a data-toggle='collapse' data-parent='#accordion' href='#collapse%d'>%s</a>\n", StoryNumber, Title );
	printf ( "        </%s>\n", HeadingCode );
	printf ( "      </div>\n" );
	printf ( "    <div id='collapse%d' class='panel-collapse collapse%s'>\n", StoryNumber, IsIn ? " in" : "" );
	printf ( "      <div class='panel-body'>\n" );

	printf ("<table class='table-condensed table-borderless'>\n" );

	StoryNumber++;
}

void PaintEndReportStory ()
{
	printf ("</table>\n" );
	printf ( "      </div>\n" );
	printf ( "    </div>\n" );
	printf ( "    </div>\n" );
	printf ( "\n<!----  end of story ---->\n\n" );
}

//  -type {all*|stock|etf|fixed|cash}
void PaintSelectStype ( int RunMode )
{
	int		ndx = RunMode - SCREENER_AND_REPORT_OFFSET;

	printf ("<tr>\n" );
	printf ("<td class='report-table'>\n" );
	printf ("Stock type\n" );
	printf ("</td>\n" );
	printf ("<td class='report-table' colspan='3'>\n" );
	printf ("<select name='StockType_%d'>\n", RunMode );
	printf ("<option value='all'%s>All Securities</option>\n",   StockType[ndx] == 'A' ? " selected" : "" );
	printf ("<option value='stock'%s>Stock and ADRs</option>\n", StockType[ndx] == 'S' ? " selected" : "" );
	printf ("<option value='etf'%s>ETF and Funds</option>\n",    StockType[ndx] == 'E' ? " selected" : "" );
	printf ("<option value='fixed'%s>Bonds and CDs</option>\n",  StockType[ndx] == 'F' ? " selected" : "" );
	printf ("<option value='cash'%s>Cash</option>\n",            StockType[ndx] == 'C' ? " selected" : "" );

	printf ("</select>\n" );
	printf ("</td>\n" );
	printf ("</tr>\n" );
}

void PaintStockUniverse ( int RunMode )
{
	int		ndx = RunMode - SCREENER_AND_REPORT_OFFSET;

	printf ("<tr>\n" );
	printf ("<td class='report-table'>\n" );
	printf ("Stock universe\n" );
	printf ("</td>\n" );
	printf ("<td class='report-table' colspan='3'>\n" );
	printf ("<select name='StockIndex_%d'>\n", RunMode );
	printf ("<option value='S'%s>S&P 500</option>\n", StockIndex[ndx] == 'S' ? " selected" : "" );
	printf ("<option value='D'%s>Dow Jones Industrials</option>\n", StockIndex[ndx] == 'D' ? " selected" : "" );
	printf ("<option value='N'%s>Nasdaq 100</option>\n", StockIndex[ndx] == 'N' ? " selected" : "" );
	// printf ("<option value='M'%s>Midcap (Russell 1000 ex S&P 500)</option>\n", StockIndex[ndx] == 'M' ? " selected" : "" );
	printf ("<option value='M'%s>Midcap</option>\n", StockIndex[ndx] == 'M' ? " selected" : "" );
	printf ("<option value='1'%s>Russell 1000</option>\n", StockIndex[ndx] == '1' ? " selected" : "" );
	printf ("<option value='2'%s>Russell 2000</option>\n", StockIndex[ndx] == '2' ? " selected" : "" );
	printf ("<option value='3'%s>Russell 3000</option>\n", StockIndex[ndx] == '3' ? " selected" : "" );
	printf ("<option value='A'%s>All stocks</option>\n", StockIndex[ndx] == 'A' ? " selected" : "" );
	printf ("<option value='O'%s>Stocks not indexed</option>\n", StockIndex[ndx] == 'O' ? " selected" : "" );
	if ( CookieMember.xid == 0 || PortfolioRecordCount > 0 )
	{
		printf ("<option value='P'%s>My Portfolio</option>\n", StockIndex[ndx] == 'P' ? " selected" : "" );
	}
	if ( CookieMember.xid == 0 || WatchlistRecordCount > 0 )
	{
		printf ("<option value='W'%s>My Watchlist</option>\n", StockIndex[ndx] == 'W' ? " selected" : "" );
	}
	if ( CookieMember.xid == 0 || TempRecordCount > 0 )
	{
		printf ("<option value='Z'%s>Temp File</option>\n", StockIndex[ndx] == 'Z' ? " selected" : "" );
	}
	printf ("</select>\n" );
	printf ("</td>\n" );
	printf ("</tr>\n" );
}

void PaintVolumeCalcMethod ( int RunMode )
{
#ifdef CRAP
	int		ndx = RunMode - SCREENER_AND_REPORT_OFFSET;

	printf ("<tr>\n" );
	printf ("<td class='report-table'>\n" );
	printf ("Volume calculation method\n" );
	printf ("</td>\n" );
	printf ("<td class='report-table' colspan='3'>\n" );
	printf ("<select name='CalcVolumeMethod_%d'>\n", RunMode );

	printf ("<option value='%d'%s>%s</option>\n", 
		VOLUME_METHOD_OPEN_CLOSE, 
		CalcVolumeMethod[ndx] == VOLUME_METHOD_OPEN_CLOSE ? " selected" : "", 
		MethodString(VOLUME_METHOD_OPEN_CLOSE,2) );

	printf ("<option value='%d'%s>%s</option>\n", 
		VOLUME_METHOD_PREVIOUS_CLOSE, 
		CalcVolumeMethod[ndx] == VOLUME_METHOD_PREVIOUS_CLOSE ? " selected" : "", 
		MethodString(VOLUME_METHOD_PREVIOUS_CLOSE,2) );

	printf ("</select>\n" );
	printf ("</td>\n" );
	printf ("</tr>\n" );
#endif
}

void PaintDateRange ( int RunMode )
{
	int		ndx = RunMode - SCREENER_AND_REPORT_OFFSET;

	printf ("<tr>\n" );
	printf ("<td class='report-table'>\n" );
	printf ("Date range\n" );
	printf ("</td>\n" );
	printf ("<td class='report-table'>\n" );
	printf ("<select name='DateRange_%d'>\n", RunMode );
	switch ( RunMode )
	{
		case MODE_REPORT_REGRESS:
			printf ("<option value='%c'%s>Past Six Months</option>\n", DATE_RANGE_SIX_MONTHS, DateRange[ndx] == DATE_RANGE_SIX_MONTHS ? " selected" : "" );
			printf ("<option value='%c'%s>Past One Year</option>\n",   DATE_RANGE_ONE_YEAR,   DateRange[ndx] == DATE_RANGE_ONE_YEAR   ? " selected" : "" );
			printf ("<option value='%c'%s>Past Two Years</option>\n",  DATE_RANGE_TWO_YEARS,  DateRange[ndx] == DATE_RANGE_TWO_YEARS  ? " selected" : "" );
			break;

		case MODE_REPORT_AD_HISTORY:
			printf ("<option value='%c'%s>Past One Month</option>\n", DATE_RANGE_ONE_MONTH, DateRange[ndx] == DATE_RANGE_ONE_MONTH ? " selected" : "" );
			printf ("<option value='%c'%s>Past Six Months</option>\n", DATE_RANGE_SIX_MONTHS, DateRange[ndx] == DATE_RANGE_SIX_MONTHS ? " selected" : "" );
			printf ("<option value='%c'%s>Past One Year</option>\n",   DATE_RANGE_ONE_YEAR,   DateRange[ndx] == DATE_RANGE_ONE_YEAR   ? " selected" : "" );
			break;

	}
#ifdef HAVE_CUSTOM_DATES
	printf ("<option value='C'>Custom Date</option>\n" );
#endif
	printf ("</select>\n" );
	printf ("</td>\n" );
#ifdef HAVE_CUSTOM_DATES
	printf ("<td class='report-table'><input type='search' name='StartDate_%d' id='StartDate_%d'></td>\n", RunMode, RunMode );
	printf ("<td class='report-table'><input type='search' name='EndDate_%d' id='EndDate_%d'></td>\n", RunMode, RunMode );
#else
	printf ( "<td colspan='2'>&nbsp;</td>\n" );
#endif
	printf ("</tr>\n" );
}

void PaintTopCount ( int RunMode, int Default )
{
	int		ndx = RunMode - SCREENER_AND_REPORT_OFFSET;

	if ( TopCount[ndx] < 1 )
	{
		TopCount[ndx] = Default;
	}

	printf ("<tr>\n" );
	printf ("<td class='report-table'>Top count</td>\n" );
	printf ("<td class='report-table' colspan='3'>" );
	printf ( "<input type='search' name='TopCount_%d'", RunMode );
	printf ( " value='%d'", TopCount[ndx] );
	printf ( "></td>\n" );
	printf ("</tr>\n" );
}

void PaintTopPercent ( int RunMode, char *Label, double Default )
{
	int		ndx = RunMode - SCREENER_AND_REPORT_OFFSET;

	if ( TopPercent[ndx] < 0.01 )
	{
		TopPercent[ndx] = Default;
	}

	printf ("<tr>\n" );
	printf ("<td class='report-table'>%s</td>\n", Label );
	printf ("<td class='report-table' colspan='3'>" );
	printf ( "<input type='search' name='TopPercent_%d'", RunMode );
	printf ( " value='%.2f'", TopPercent[ndx] );
	printf ( "></td>\n" );
	printf ("</tr>\n" );
}

void PaintMinimumVolume ( int RunMode, int Default )
{
	int		ndx = RunMode - SCREENER_AND_REPORT_OFFSET;

	if ( MinimumVolume[ndx] < 1 )
	{
		MinimumVolume[ndx] = Default;
	}

	printf ("<tr>\n" );
	printf ("<td class='report-table'>Minimum volume</td>\n" );
	printf ("<td class='report-table' colspan='3'>" );
	printf ( "<input type='search' name='MinimumVolume_%d'", RunMode );
	printf ( " value='%d'", MinimumVolume[ndx] );
	printf ( "></td>\n" );
	printf ("</tr>\n" );
}

void PaintNumberOfDays ( int RunMode, char *Label, int Default )
{
	int		ndx = RunMode - SCREENER_AND_REPORT_OFFSET;

	if ( NumberOfDays[ndx] < 1 )
	{
		NumberOfDays[ndx] = Default;
	}

	printf ("<tr>\n" );
	printf ("<td class='report-table'>%s</td>\n", Label );
	printf ("<td class='report-table' colspan='3'>" );
	printf ( "<input type='search' name='NumberOfDays_%d'", RunMode );
	printf ( " value='%d'", NumberOfDays[ndx] );
	printf ( "></td>\n" );
	printf ("</tr>\n" );
}

void PaintTwoNumberOfDays ( int RunMode, char *Label1, int Default1, char *Label2, int Default2 )
{
	int		ndx = RunMode - SCREENER_AND_REPORT_OFFSET;

	if ( NumberOfDays1[ndx] < 1 )
	{
		NumberOfDays1[ndx] = Default1;
	}
	if ( NumberOfDays2[ndx] < 1 )
	{
		NumberOfDays2[ndx] = Default2;
	}

	printf ("<tr>\n" );
	printf ("<td class='report-table'>%s</td>\n", Label1 );
	printf ("<td class='report-table' colspan='3'>" );
	printf ( "<input type='search' name='NumberOfDays1_%d'", RunMode );
	printf ( " value='%d'", NumberOfDays1[ndx] );
	printf ( "></td>\n" );
	printf ("</tr>\n" );

	printf ("<tr>\n" );
	printf ("<td class='report-table'>%s</td>\n", Label2 );
	printf ("<td class='report-table' colspan='3'>" );
	printf ( "<input type='search' name='NumberOfDays2_%d'", RunMode );
	printf ( " value='%d'", NumberOfDays2[ndx] );
	printf ( "></td>\n" );
	printf ("</tr>\n" );
}

void PaintTwoSelectorOfDays ( int RunMode, char *Label1, int Array1[], int Default1, char *Label2, int Array2[], int Default2 )
{
	int		ndx = RunMode - SCREENER_AND_REPORT_OFFSET;
	int		xa;

	if ( NumberOfDays1[ndx] < 1 )
	{
		NumberOfDays1[ndx] = Default1;
	}
	if ( NumberOfDays2[ndx] < 1 )
	{
		NumberOfDays2[ndx] = Default2;
	}

	printf ( "<tr>\n" );
	printf ( "<td class='report-table'>%s</td>\n", Label1 );
	printf ( "<td class='report-table' colspan='3'>" );
	printf ( "<select  name='NumberOfDays1_%d'>\n", RunMode );
	for ( xa = 0; Array1[xa]; xa++ )
	{
		printf ( "<option value='%d'%s>%d</option>\n", Array1[xa], Array1[xa] == NumberOfDays1[ndx] ? " selected" : "", Array1[xa] );
	}
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td class='report-table'>%s</td>\n", Label2 );
	printf ( "<td class='report-table' colspan='3'>" );
	printf ( "<select  name='NumberOfDays2_%d'>\n", RunMode );
	for ( xa = 0; Array2[xa]; xa++ )
	{
		printf ( "<option value='%d'%s>%d</option>\n", Array2[xa], Array2[xa] == NumberOfDays2[ndx] ? " selected" : "", Array2[xa] );
	}
	printf ( "</sselct>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );
}

void PaintSortBy ( int RunMode, int OptionArray[], int OptionCount )
{
	int		ndx = RunMode - SCREENER_AND_REPORT_OFFSET;
	int		xo, selected;
	char	*label;

	printf ("<tr>\n" );
	printf ("<td class='report-table'>Sort by</td>\n" );
	printf ("<td class='report-table' colspan='3'>\n" );
	printf ("<select name='SortBy_%d'>\n", RunMode );

	for ( xo = 0; xo < OptionCount; xo++ )
	{
		if (SortBy[ndx] == OptionArray[xo] )
		{
			selected = 1;
		}
		else
		{
			selected = 0;
		}
		
		switch ( OptionArray[xo] )
		{
			case SORTBY_RETURN:
				label = "Return";
				break;
			case SORTBY_CORREL:
				label = "Correlation Coefficient";
				break;
			case SORTBY_VARIANCE:
				label = "Variance";
				break;
			case SORTBY_COMPARED:
				label = "Compared";
				break;
			case SORTBY_GAIN:
				label = "Gain";
				break;
			case SORTBY_TICKER:
				label = "Ticker";
				break;
			case SORTBY_NONE:
				label = "None";
				break;
			case SORTBY_SECTOR:
				label = "Sector";
				break;
			case SORTBY_INDUSTRY:
				label = "Industry";
				break;
		}

		printf ("<option value='%c'%s>%s</option>\n",  OptionArray[xo], selected ? " selected" : "", label );
	}

	printf ("</select>\n" );
	printf ("</td>\n" );
	printf ("</tr>\n" );
}

void PaintFormat ( int RunMode, int IncludeWatchlist )
{
	int		ndx = RunMode - SCREENER_AND_REPORT_OFFSET;

	printf ("<tr>\n" );
	printf ("<td class='report-table'>Format/Destination</td>\n" );
	printf ("<td class='report-table' colspan='3'>\n" );

   printf ("<select name='ReportFormat_%d' id='ReportFormat_%d'>\n", RunMode, RunMode );

	printf ("<option value='%c'%s>Screen</option>\n", RPT_FORMAT_HTML, ReportFormat[ndx] == RPT_FORMAT_HTML ? " selected" : "" );
	if ( CookieMember.xid != DEMO_MEMBER )
	{
		if ( RunMode == MODE_REPORT_ASSET_ALLOCATION )
		{
			printf ("<option value='%c'%s>Text</option>\n", RPT_FORMAT_TEXT, ReportFormat[ndx] == RPT_FORMAT_TEXT ? " selected" : "" );
		}
		else
		{
			printf ("<option value='%c'%s>CSV</option>\n",  RPT_FORMAT_CSV, ReportFormat[ndx] == RPT_FORMAT_CSV ? " selected" : "" );
			printf ("<option value='%c'%s>Text</option>\n", RPT_FORMAT_TEXT, ReportFormat[ndx] == RPT_FORMAT_TEXT ? " selected" : "" );
#ifdef DONT_HAVE_THESE_YET
			printf ("<option value='%c'%s>PDF</option>\n", RPT_FORMAT_PDF_VIEW,  ReportFormat[ndx] ==  RPT_FORMAT_PDF_VIEW ? " selected" : "" );
#endif
			printf ("<option value='%c'%s>Excel</option>\n", RPT_FORMAT_EXCEL, ReportFormat[ndx] == RPT_FORMAT_EXCEL ? " selected" : "" );
		}
	}
	if ( IncludeWatchlist )
	{
		printf ("<option value='%c'%s>Add to Watchlist</option>\n", RPT_FORMAT_APPEND_W, ReportFormat[ndx] == RPT_FORMAT_APPEND_W ? " selected" : "" );
		printf ("<option value='%c'%s>Replace Watchlist</option>\n", RPT_FORMAT_REPLACE_W, ReportFormat[ndx] == RPT_FORMAT_REPLACE_W ? " selected" : "" );
		printf ("<option value='%c'%s>Temp File</option>\n", RPT_FORMAT_TEMP, ReportFormat[ndx] == RPT_FORMAT_TEMP ? " selected" : "" );
	}
	printf ("</select>\n" );
	printf ("</td>\n" );
	printf ("</tr>\n" );
}

void PaintSubmit ( int RunMode, char *Label, int Columns )
{
	printf ("<tr>\n" );
	printf ("<td>&nbsp;</td>\n" );
	printf ("<td class='report-table' align='left' colspan='%d'>\n", Columns - 1 );
	printf ( "<input type='submit' name='submit_%d' value='%s' onclick='javascript:SubmitTarget(\"ReportFormat_%d\");'>\n", RunMode, Label, RunMode );
	printf ("</td>\n" );
	printf ("</tr>\n" );
}

void PaintPaypal ()
{
// TYPE	char		env_PaypalKey[65];
// TYPE	char		env_PaypalURL[65];
	// printf ( "<form action='https://www.paypal.com/cgi-bin/webscr' method='post' target='_top'>\n" );
	printf ( "<form action='%s' method='post' target='_top'>\n", env_PaypalURL );

	printf ( "<input type='hidden' name='cmd' value='_s-xclick'>\n" );

	// printf ( "<input type='hidden' name='hosted_button_id' value='ACNYRARQAUG6Y'>\n" );
	printf ( "<input type='hidden' name='hosted_button_id' value='%s'>\n", env_PaypalKey );

	printf ( "<input type='image' src='https://www.paypalobjects.com/en_US/i/btn/btn_subscribeCC_LG.gif' border='0' name='submit' alt='PayPal - The safer, easier way to pay online!'>\n" );
	printf ( "<img alt='Paypal Button' border='0' src='https://www.paypalobjects.com/en_US/i/scr/pixel.gif' width='1' height='1'>\n" );
	printf ( "</form>\n" );
}

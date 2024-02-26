/*----------------------------------------------------------------------------
	Program : invest/PaintTableDescription.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint descriptions of various features on portfolio page
				for anonymous user (or not logged in).
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

void PaintTableDescription ( char *WhichDescription )
{
	if ( nsStrcmp ( WhichDescription, "watchlist" ) == 0 )
	{
		printf ( "<div class='story'>\n" );
		printf ( "<%s>Watchlist fields</%s>\n", STORY_TITLE, STORY_TITLE );
		printf ( "<ul>\n" );
		printf ( "<li>Ticker</li>\n" );
		printf ( "<li>Alert Type: None, above or below Value, above or below one of six moving averages.</li>\n" );
		printf ( "<li>Alert Value</li>\n" );
		printf ( "</ul>\n" );
		printf ( "</div>\n" );
	}
	else if ( nsStrcmp ( WhichDescription, "watchlist_report" ) == 0 )
	{
		printf ( "<div class='story'>\n" );
		printf ( "<%s>Watchlist Report</%s>\n", STORY_TITLE, STORY_TITLE );

		printf ( "<div class='large hidden-md hidden-xs hidden-sm'>\n" );
		printf ( "<img alt='Watchlist Report' src='SampleWatchlistReport.png'>\n" );
		printf ( "</div>\n" );

		printf ( "<div class='small hidden-lg'>\n" );
		printf ( "These are the fields on the Watchlist Report.  A sample report if shown on devices with wider screens.\n" );
		printf ( "<ul>\n" );
		printf ( "<li>TICKER</li>\n" );
		printf ( "<li>COMPANY</li>\n" );
		printf ( "<li>PCT-CHANGE</li>\n" );
		printf ( "<li>CURRENT</li>\n" );
		printf ( "<li>ALERT</li>\n" );
		printf ( "<li>PRICE/AVG</li>\n" );
		printf ( "<li>DIFF</li>\n" );
		printf ( "<li>PERCENT</li>\n" );
		printf ( "</ul>\n" );
		printf ( "</div>\n" );

		printf ( "</div>\n" );
	}
	else if ( nsStrcmp ( WhichDescription, "portfolio" ) == 0 )
	{
		printf ( "<div class='story'>\n" );
		printf ( "<%s>Portfolio fields</%s>\n", STORY_TITLE, STORY_TITLE );
		printf ( "<ul>\n" );
		printf ( "<li>Ticker</li>\n" );
		printf ( "<li>Purchase Date</li>\n" );
		printf ( "<li>Shares</li>\n" );
		printf ( "<li>Cost</li>\n" );
		printf ( "<li>Benchmark Security - This is used to measure performance, for example SPY, DIA, QQQ, IWM or EFA.</li>\n" );
		printf ( "<li>Benchmark Alert - If set, this value triggers alert if security is falling behind benchmark.</li>\n" );
		printf ( "<li>Trailing Alert Percentage - Raises dollar value on rising stock price, triggers alert if security falls below dollar value.</li>\n" );
		printf ( "<li>Trailing Alert Dollar Value - Automatically calculated.</li>\n" );
		printf ( "<li>Option Expiration Date - If you have an option on a security, this turn off alerts.</li>\n" );
		printf ( "<li>Broker</li>\n" );
		printf ( "<li>Tax Sheltered</li>\n" );
		printf ( "</ul>\n" );
		printf ( "</div>\n" );
	}
	else if ( nsStrcmp ( WhichDescription, "portfolio_report" ) == 0 )
	{
		printf ( "<div class='story'>\n" );
		printf ( "<%s>Portfolio Report</%s>\n", STORY_TITLE, STORY_TITLE );

		printf ( "There are three versions: Basic, Full and Alerts.\n" );

		printf ( "<div class='large hidden-md hidden-xs hidden-sm'>\n" );
printf ( "<p>Basic:<br>\n" );
		printf ( "<img alt='' src='PortfolioReportBasic.png'>\n" );
printf ( "<p>Full:<br>\n" );
		printf ( "<img alt='' src='PortfolioReportFull.png'>\n" );
printf ( "<p>Alert:<br>\n" );
		printf ( "<img alt='' src='PortfolioReportAlert.png'>\n" );
		printf ( "</div>\n" );

		printf ( "<div class='small hidden-lg'>\n" );

printf ( "Basic:\n" );

		printf ( "<ul>\n" );
		printf ( "<li>TICKER</li>\n" );
		printf ( "<li>SHARES</li>\n" );
		printf ( "<li>COST</li>\n" );
		printf ( "<li>INVEST</li>\n" );
		printf ( "<li>CURRENT</li>\n" );
		printf ( "<li>VALUE</li>\n" );
		printf ( "<li>UNREALIZED</li>\n" );
		printf ( "<li>ROI</li>\n" );
		printf ( "</ul>\n" );

printf ( "Full:\n" );
		printf ( "<ul>\n" );
		printf ( "<li>TICKER</li>\n" );
		printf ( "<li>PURCHDATE</li>\n" );
		printf ( "<li>SHARES</li>\n" );
		printf ( "<li>COST</li>\n" );
		printf ( "<li>INVEST</li>\n" );
		printf ( "<li>CURRENT</li>\n" );
		printf ( "<li>VALUE</li>\n" );
		printf ( "<li>UNREALIZED</li>\n" );
		printf ( "<li>ROI</li>\n" );
		printf ( "<li>TTM-YLD</li>\n" );
		printf ( "<li>TOT-YLD</li>\n" );
		printf ( "<li>TOT-ROI</li>\n" );
		printf ( "<li>A/D-RATIO</li>\n" );
		printf ( "<li>OPTEXP</li>\n" );
		printf ( "<li>BROKER</li>\n" );
		printf ( "<li>SHELTERED</li>\n" );
		printf ( "</ul>\n" );

printf ( "Alert:\n" );
		printf ( "<ul>\n" );
		printf ( "<li>TICKER</li>\n" );
		printf ( "<li>SHARES</li>\n" );
		printf ( "<li>COST</li>\n" );
		printf ( "<li>CURRENT</li>\n" );
		printf ( "<li>ROI</li>\n" );
		printf ( "<li>TOT-ROI</li>\n" );
		printf ( "<li>TRAIL-PCT</li>\n" );
		printf ( "<li>TRAIL-AMT</li>\n" );
		printf ( "<li>CLOSE-PCT</li>\n" );
		printf ( "<li>50MA-PCT</li>\n" );
		printf ( "<li>BNCH</li>\n" );
		printf ( "<li>BNCH-ROI</li>\n" );
		printf ( "<li>CMP-BNCH</li>\n" );
		printf ( "<li>ALERT</li>\n" );
		printf ( "<li>OPTEXP</li>\n" );
		printf ( "</ul>\n" );

		printf ( "</div>\n" );

		printf ( "</div>\n" );
	}
	else if ( nsStrcmp ( WhichDescription, "alert" ) == 0 )
	{
		printf ( "<div class='story'>\n" );
		printf ( "<%s>Alerts system</%s>\n", STORY_TITLE, STORY_TITLE );
		printf ( "<ul>\n" );
		printf ( "<li>System includes two different types of alert on portfolio.</li>\n" );
		printf ( "<li>System includes another alert type on watchlist.</li>\n" );
		printf ( "<li>No sell order is placed. You are notified by email.</li>\n" );
		printf ( "<li>Benchmark alert includes the estimated yield since purchase date (unlike a trailing stop).</li>\n" );
		printf ( "<li>So if a stock is trading 15%% below its purchase price, but has been paying a 4%% dividend for years, it may be a keeper.</li>\n" );
		printf ( "</ul>\n" );
		printf ( "<p>\n" );
		printf ( "Sample Alert -- stock basically flat after several years. But trails benchmark significantly, even though it pays a good dividend.\n" );
		printf ( "<p>\n" );


/*----------------------------------------------------------
	bootstrap
	xs <768px
	sm  768 - 991
	md  992 - 1199
	lg >1200
----------------------------------------------------------*/
		printf ( "<div class='hidden-xs hidden-sm'>\n" );
		printf ( "<img src='SampleAlert.png' alt='Sample Alert Image'>\n" );
		printf ( "</div>\n" );
		printf ( "<div class='hidden-md hidden-lg'>\n" );
		printf ( "<img src='SampleAlert.png' width='95%%' alt='Sample Alert Image'>\n" );
		printf ( "</div>\n" );

		printf ( "<p>\n" );
		printf ( "<ul>\n" );
		printf ( "<li>If alert value is zero, then security will not be compared to benchmark.</li>\n" );
		printf ( "<li>Alert value is normally negative, to find when a security is not keeping up with its benchmark.</li>\n" );
		printf ( "<li>Alert values can be positive, to catch securities which are way above their benchmark before they fall back.</li>\n" );
		printf ( "</ul>\n" );
		printf ( "</div>\n" );
	}
	else
	{
		SafeError ( COLOR_ERROR, "PaintTableDescription: unknown table name" );
	}
}

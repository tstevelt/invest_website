/*----------------------------------------------------------------------------
	Program : invest/PaintTop.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Initial HTML, etc output to start a page.
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

void PaintTop ()
{
	WEBPARMS	*ptrWebParms = webInitParms();

	webContentHtml ();

	ptrWebParms->WP_Title = URL_NAME;
	webStartHead ( ptrWebParms );

	printf ( "<meta name='description' content='Silver Hammer Software LLC, Investment Tools, Screeners and Custom Programming' lang='en-US'/>\n" );
	printf ( "<meta name='robots' content='noodp,noydir'/>\n" );

	printf ( "<meta name='viewport' content='width=device-width, initial-scale=1'>\n" );
	printf ( "<meta name='keywords' content='invest,screeners,alerts,programming,trading,stocks,market,quantitative,retire,retirement,personal finance,swing,swing trade,investment club,stock market education,learn to trade,cup,handle,technical'/>\n");
	printf ( "<meta property='og:title' content='Silver Hammer Software LLC'/>\n" );
	printf ( "<meta property='og:description' content='Silver Hammer Software LLC, Investment Tools, Screeners and Custom Programming'/>\n" );
	printf ( "<meta property='og:url' content='https://stocks.silverhammersoftware.com'/>\n" );
	printf ( "<meta property='og:type' content='website'/>\n" );
	printf ( "<meta property='og:site_name' content='Silver Hammer Software LLC'/>\n" );

	// printf ( "<meta name='color-scheme' content='dark light'>\n" );

	/*---------------------------------------------------------------------------
		!!! NOTE:  META VIEWPORT MUST BE BEFORE CSS STYLE SHEETS !!!
	---------------------------------------------------------------------------*/

	printf ( "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'>\n" );
	printf ( "<link rel='canonical' href='http://stocks.silverhammersoftware.com'/>\n" );
	printf ( "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script>\n" );
	printf ( "<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>\n" );
	printf ( "<link rel='stylesheet' href='my_navbar_colors.css?version=%s'>\n", CSS_VERSION );
	printf ( "<link rel='stylesheet' href='invest.css?version=%s'>\n", CSS_VERSION );

	printf ( "<script src='invest.js'></script>\n" );
	printf ( "<script src='chart.js'></script>\n" );
	printf ( "<script src='portfolio.js'></script>\n" );

	printf ( "<script>\n" );
	printf ( "(function(i,s,o,g,r,a,m){\n" );
	printf ( "i['GoogleAnalyticsObject']=r;\n" );
	printf ( "i[r]=i[r]||function(){(i[r].q=i[r].q||[]).push(arguments)},\n" );
	printf ( "i[r].l=1*new Date();\n" );
	printf ( "a=s.createElement(o),m=s.getElementsByTagName(o)[0];\n" );
	printf ( "a.async=1;\n" );
	printf ( "a.src=g;\n" );
	printf ( "m.parentNode.insertBefore(a,m)\n" );
	printf ( "})(window,document,'script','//www.google-analytics.com/analytics.js','ga');\n" );
	printf ( "ga('create', 'UA-138395741-1', 'auto');\n" );
	printf ( "ga('send', 'pageview');\n" );
	printf ( "</script>\n" );

	webHeadToBody ( ptrWebParms );

	webFreeParms ( ptrWebParms );

	printf ( "<form name='invest' id='invest' action='invest.cgi' target='_self' method='POST' enctype='application/x-www-form-urlencoded'>\n" );

}

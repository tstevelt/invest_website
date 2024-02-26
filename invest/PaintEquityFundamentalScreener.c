/*----------------------------------------------------------------------------
	Program : invest/PaintEquityFundamentalScreener.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint EQUITY fundamentals screen.
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

void PaintEquityFundamentalScreener ()
{
	/*----------------------------------------------------------
		stock universe check boxes
	----------------------------------------------------------*/
	printf ( "<tr><td>Universe</td><td colspan='2'>" );
	printf ( "<input type='checkbox'       id='cb_fund_D' name='cb_fund_D'>&nbsp;Dow Jones" );
	printf ( "&emsp;<input type='checkbox' id='cb_fund_S' name='cb_fund_S'>&nbsp;SP 500" );
	printf ( "&emsp;<input type='checkbox' id='cb_fund_1' name='cb_fund_1'>&nbsp;Russell 1000" );
	printf ( "&emsp;<input type='checkbox' id='cb_fund_2' name='cb_fund_2'>&nbsp;Russell 2000" );
	printf ( "&emsp;<input type='checkbox' id='cb_fund_N' name='cb_fund_N'>&nbsp;NASDAQ 100" );
	printf ( "&emsp;<input type='checkbox' id='cb_fund_A' name='cb_fund_A'>&nbsp;ADR" );
	printf ( "&emsp;<input type='checkbox' id='cb_fund_O' name='cb_fund_O'>&nbsp;Other" );
	printf ( "</td></tr>" );

	printf ( "<tr><th align='left'>FIELD</td><th align='left'>MINIMUM</td><th align='left'>MAXIMUM</td></tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fmktcap") );
	printf ( "<td><input name='Fmktcap_min_%d'></td> <td><input name='Fmktcap_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fmktcap_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fmktcap\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fentval") );
	printf ( "<td><input name='Fentval_min_%d'></td> <td><input name='Fentval_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fentval_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fentval\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fvalue") );
	printf ( "<td><input name='Fvalue_min_%d'></td> <td><input name='Fvalue_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fvalue_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fvalue\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fepsttm") );
	printf ( "<td><input name='Fepsttm_min_%d'></td> <td><input name='Fepsttm_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fepsttm_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fepsttm\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fpettm") );
	printf ( "<td><input name='Fpettm_min_%d'></td> <td><input name='Fpettm_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fpettm_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fpettm\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fpb") );
	printf ( "<td><input name='Fpb_min_%d'></td> <td><input name='Fpb_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fpb_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fpb\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fyield") );
	printf ( "<td><input name='Fyield_min_%d'></td> <td><input name='Fyield_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fyield_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fyield\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("FreturnA") );
	printf ( "<td><input name='FreturnA_min_%d'></td> <td><input name='FreturnA_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='FreturnA_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=FreturnA\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fmargin") );
	printf ( "<td><input name='Fmargin_min_%d'></td> <td><input name='Fmargin_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fmargin_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fmargin\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fbeta") );
	printf ( "<td><input name='Fbeta_min_%d'></td> <td><input name='Fbeta_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fbeta_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fbeta\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fquick") );
	printf ( "<td><input name='Fquick_min_%d'></td> <td><input name='Fquick_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fquick_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fquick\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fdebteq") );
	printf ( "<td><input name='Fdebteq_min_%d'></td> <td><input name='Fdebteq_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fdebteq_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fdebteq\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Frevenuegrow") );
	printf ( "<td><input name='Frevenuegrow_min_%d'></td> <td><input name='Frevenuegrow_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Frevenuegrow_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Frevenuegrow\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fearngrow") );
	printf ( "<td><input name='Fearngrow_min_%d'></td> <td><input name='Fearngrow_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fearngrow_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fearngrow\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Ffcfgrow") );
	printf ( "<td><input name='Ffcfgrow_min_%d'></td> <td><input name='Ffcfgrow_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Ffcfgrow_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Ffcfgrow\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Ffcfcv") );
	printf ( "<td><input name='Ffcfcv_min_%d'></td> <td><input name='Ffcfcv_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Ffcfcv_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Ffcfcv\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fvolume") );
	printf ( "<td><input name='Fvolume_min_%d'></td> <td><input name='Fvolume_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fvolume_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fvolume\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

#ifdef HAVE_ANALYST
	printf ( "<tr><td>%s</td>", FundamentalHuman("Fepsfwd") );
	printf ( "<td><input name='Fepsfwd_min_%d'></td> <td><input name='Fepsfwd_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fepsfwd_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fepsfwd\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fpefwd") );
	printf ( "<td><input name='Fpefwd_min_%d'></td> <td><input name='Fpefwd_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fpefwd_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fpefwd\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fgrow5") );
	printf ( "<td><input name='Fgrow5_min_%d'></td> <td><input name='Fgrow5_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fgrow5_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fgrow5\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fgrow1") );
	printf ( "<td><input name='Fgrow1_min_%d'></td> <td><input name='Fgrow1_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fgrow1_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Fgrow1\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Ftarget") );
	printf ( "<td><input name='Ftarget_min_%d'></td> <td><input name='Ftarget_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Ftarget_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Ftarget\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Freco") );
	printf ( "<td><input name='Freco_min_%d'></td> <td><input name='Freco_max_%d'></td>", MODE_REPORT_EQUITY_FUNDAMENTALS, MODE_REPORT_EQUITY_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Freco_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=S&MinMax=Freco\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

#endif

}

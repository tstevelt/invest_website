/*----------------------------------------------------------------------------
	Program : invest/PaintETF_FundamentalScreener.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint ETF fundamentals screen.
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

void PaintETF_FundamentalScreener ()
{
	printf ( "<input type='hidden' id='cb_fund_E' name='cb_fund_E' value='Y'>\n" );

	printf ( "<tr><th align='left'>FIELD</td><th align='left'>MINIMUM</td><th align='left'>MAXIMUM</td></tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fyield") );
	printf ( "<td><input name='Fyield_min_%d'></td> <td><input name='Fyield_max_%d'></td>", MODE_REPORT_ETF_FUNDAMENTALS, MODE_REPORT_ETF_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fyield_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=E&MinMax=Fyield\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Favgret3") );
	printf ( "<td><input name='Favgret3_min_%d'></td> <td><input name='Favgret3_max_%d'></td>", MODE_REPORT_ETF_FUNDAMENTALS, MODE_REPORT_ETF_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Favgret3_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=E&MinMax=Favgret3\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Favgret5") );
	printf ( "<td><input name='Favgret5_min_%d'></td> <td><input name='Favgret5_max_%d'></td>", MODE_REPORT_ETF_FUNDAMENTALS, MODE_REPORT_ETF_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Favgret5_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=E&MinMax=Favgret5\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fexpratio") );
	printf ( "<td><input name='Fexpratio_min_%d'></td> <td><input name='Fexpratio_max_%d'></td>", MODE_REPORT_ETF_FUNDAMENTALS, MODE_REPORT_ETF_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fexpratio_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=E&MinMax=Fexpratio\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fmstar") );
	printf ( "<td><input name='Fmstar_min_%d'></td> <td><input name='Fmstar_max_%d'></td>", MODE_REPORT_ETF_FUNDAMENTALS, MODE_REPORT_ETF_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fmstar_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=E&MinMax=Fmstar\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Ffactset") );
	printf ( "<td><input name='Ffactset_min_%d'></td> <td><input name='Ffactset_max_%d'></td>", MODE_REPORT_ETF_FUNDAMENTALS, MODE_REPORT_ETF_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Ffactset_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=E&MinMax=Ffactset\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fmktcap") );
	printf ( "<td><input name='Fmktcap_min_%d'></td> <td><input name='Fmktcap_max_%d'></td>", MODE_REPORT_ETF_FUNDAMENTALS, MODE_REPORT_ETF_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fmktcap_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=E&MinMax=Fmktcap\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Ftotasst") );
	printf ( "<td><input name='Ftotasst_min_%d'></td> <td><input name='Ftotasst_max_%d'></td>", MODE_REPORT_ETF_FUNDAMENTALS, MODE_REPORT_ETF_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Ftotasst_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=E&MinMax=Ftotasst\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

	printf ( "<tr><td>%s</td>", FundamentalHuman("Fvolume") );
	printf ( "<td><input name='Fvolume_min_%d'></td> <td><input name='Fvolume_max_%d'></td>", MODE_REPORT_ETF_FUNDAMENTALS, MODE_REPORT_ETF_FUNDAMENTALS );
	printf ( "<td><input type='button' id='Fvolume_pop' value='?'" );
	printf ( " onclick='javascript:window.open(\"fminmax.cgi?Type=E&MinMax=Fvolume\",\"popup\",\"width=400,height=200\");'</td>" );
	printf ( "</tr>\n" );

}

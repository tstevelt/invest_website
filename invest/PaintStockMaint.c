/*----------------------------------------------------------------------------
	Program : invest/PaintStockMaint.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Paint stock maintenance screen.
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

void PaintStockMaint ()
{
	int		ScreenLevel;
	int		Days;
	DATEVAL	StockDate;
	DATEVAL	SystemDate;
	int		CanInsertStock;

	Days = 0;
	switch ( RunMode )
	{
		case MODE_FIND_STOCK:
		case MODE_INSERT_STOCK:
			ScreenLevel = 2;
			if ( StrToDatevalFmt ( MaintStock.xslast, DATEFMT_YYYY_MM_DD, &StockDate ) != 0 )
			{
				break;
			}
			if ( StrToDatevalFmt ( xsystem.xystktime, DATEFMT_YYYY_MM_DD, &SystemDate ) != 0 )
			{
				break;
			}
			Days = DateDiff ( &StockDate, &SystemDate );	
			break;
		case MODE_CLEAR_STOCK:
			memset ( &MaintStock, '\0', sizeof(MaintStock) );
		default:
			ScreenLevel = 1;
			break;
	}

	CanInsertStock = 0;
	switch ( xmember.xmrole[0] )
	{
		case ROLE_PAID:
		case ROLE_BETA:
		case ROLE_PREMIUM:
		case ROLE_ADMIN:
			CanInsertStock = 1;
			break;
		case ROLE_EXPIRED:
		case ROLE_FREE:
		default:
			break;
	}

	printf ( "<table class='%s'>\n", TABLE_CLASS );

	if ( ScreenLevel == 1 )
	{
		printf ( "<tr>\n" );
		printf ( "<td colspan='4'>\n" );
		printf ( "Enter TICKER and click [Find] to see stock information.</td>\n" );
		printf ( "</tr>\n" );

		if ( CanInsertStock && CookieMember.xid != DEMO_MEMBER )
		{
			printf ( "<tr>\n" );
			printf ( "<td colspan='4'>\n" );
			printf ( "Enter all information and click [Insert] to add a missing stock..</td>\n" );
			printf ( "</tr>\n" );
		}
	}
	else
	{
		printf ( "<tr>\n" );
		printf ( "<td colspan='4'>\n" );
		printf ( "Click [Clear] to find another company..</td>\n" );
		printf ( "</tr>\n" );
	}

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "TICKER</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' id='field_Sticker' name='field_Sticker' size='20' maxlength='20'" );
	if ( ScreenLevel == 2 )
	{
		printf ( " value='%s'", MaintStock.xsticker );
	}
	printf ( ">\n" );
	printf ( "</td>\n" );
	printf ( "<td>\n" );
	printf ( "DJI</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='field_Sdj'>\n" );
	printf ( "<option value='N'%s>No</option>\n",  MaintStock.xsdj[0] == 'Y' ? "" : " selected" );
	printf ( "<option value='Y'%s>Yes</option>\n", MaintStock.xsdj[0] == 'Y' ? " selected" : "" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "COMPANY</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='field_Sname' size='40' maxlength='80'" );
	if ( ScreenLevel == 2 )
	{
		printf ( " value='%s'", MaintStock.xsname );
	}
	printf ( ">\n" );
	printf ( "</td>\n" );
	printf ( "<td>\n" );
	printf ( "SP500</td>\n" );
	printf ( "<td>\n" );
//	printf ( "<input type='search' name='field_Ssp500' size='4' maxlength='1'>\n" );
	printf ( "<select name='field_Ssp500'>\n" );
	printf ( "<option value='N'%s>No</option>\n",  MaintStock.xssp500[0] == 'Y' ? "" : " selected" );
	printf ( "<option value='Y'%s>Yes</option>\n", MaintStock.xssp500[0] == 'Y' ? " selected" : "" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "LATEST PRICE</td>\n" );
	if ( Days < 3 )
	{
		printf ( "<td>\n" );
	}
	else
	{
		printf ( "<td class='bg-danger'>\n" );
	}

	if ( xhistory.xhclose > 0.01 )
	{
		printf ( "%.2f %s\n", xhistory.xhclose, MaintStock.xslast );
	}
	else
	{
		printf ( "&nbsp;" );
	}
	printf ( "</td>\n" );
// here
	printf ( "<td>\n" );
	printf ( "RUSSELL</td>\n" );
	printf ( "<td>\n" );
//	printf ( "<input type='search' name='field_Srussell' size='4' maxlength='1'>\n" );
	printf ( "<select name='field_Srussell'>\n" );
	printf ( "<option value='N'%s>No</option>\n",   nsStrchr("12",MaintStock.xsrussell[0]) ? "" : " selected" );
	printf ( "<option value='1'%s>1000</option>\n", MaintStock.xsrussell[0] == '1' ? " selected" : "" );
	printf ( "<option value='2'%s>2000</option>\n", MaintStock.xsrussell[0] == '2' ? " selected" : "" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );

	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "52 WEEK HIGH</td>\n" );
	printf ( "<td>\n" );
	if ( MaintStock.xshigh52 > 0.01 )
	{
		printf ( "%.2f %s\n", MaintStock.xshigh52, MaintStock.xsdate52 );
	}
	else
	{
		printf ( "&nbsp;" );
	}
	printf ( "</td>\n" );
	printf ( "<td>\n" );
	printf ( "NASDAQ</td>\n" );
	printf ( "<td>\n" );
//	printf ( "<input type='search' name='field_Snasdaq' size='4' maxlength='1'>\n" );
	printf ( "<select name='field_Snasdaq'>\n" );
	printf ( "<option value='N'%s>No</option>\n",  MaintStock.xsnasdaq[0] == 'Y' ? "" : " selected" );
	printf ( "<option value='Y'%s>Yes</option>\n", MaintStock.xsnasdaq[0] == 'Y' ? " selected" : "" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );

	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "STD DEV</td>\n" );
	printf ( "<td>\n" );
	printf ( "%.2f", xaverage.xastddev );
	if ( MaintStock.xsclose > 0 )
	{
		double Percent = 100.0 * xaverage.xastddev / MaintStock.xsclose;
		printf ( "&nbsp;(%.2f%%)", Percent );
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "YIELD</td>\n" );
	printf ( "<td>\n" );
	printf ( "%.2f\n", xfundamental.xfyield );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "RSI</td>\n" );
	printf ( "<td>\n" );
	printf ( "%.2f\n", xaverage.xarsi );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "DOMESTIC / FOREIGN</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='field_Sdomfor'>\n" );
	printf ( "<option value='%c'%s>Domestic</option>\n", 'D', MaintStock.xsdomfor[0] == 'D' ? " selected" : "" );
	printf ( "<option value='%c'%s>Foreign</option>\n",  'F', MaintStock.xsdomfor[0] == 'F' ? " selected" : "" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "<td>&nbsp;\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "TYPE</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='field_Stype'>\n" );
	printf ( "<option value='%c'%s>Stock</option>\n",     STYPE_STOCK, MaintStock.xstype[0] == STYPE_STOCK ? " selected" : "" );
	printf ( "<option value='%c'%s>Preferred</option>\n", STYPE_PREFER, MaintStock.xstype[0] == STYPE_PREFER ? " selected" : "" );
	printf ( "<option value='%c'%s>ETF</option>\n",       STYPE_ETF, MaintStock.xstype[0] == STYPE_ETF ? " selected" : "" );
	printf ( "<option value='%c'%s>Bond</option>\n",      STYPE_BOND, MaintStock.xstype[0] == STYPE_BOND ? " selected" : "" );
	printf ( "<option value='%c'%s>ADR</option>\n",       STYPE_ADR, MaintStock.xstype[0] == STYPE_ADR ? " selected" : "" );
	printf ( "<option value='%c'%s>REIT</option>\n",      STYPE_REIT, MaintStock.xstype[0] == STYPE_REIT ? " selected" : "" );
	printf ( "<option value='%c'%s>FOREX</option>\n",     STYPE_FX, MaintStock.xstype[0] == STYPE_FX ? " selected" : "" );
	printf ( "<option value='%c'%s>CRYPTO</option>\n",    STYPE_CRYPTO, MaintStock.xstype[0] == STYPE_CRYPTO ? " selected" : "" );
	printf ( "<option value='%c'%s>INDEX</option>\n",     STYPE_INDEX, MaintStock.xstype[0] == STYPE_INDEX ? " selected" : "" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "<td>&nbsp;\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "SIZE/TYPE2</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='field_Stype2'>\n" );
	printf ( "<option value='%c'%s>Large-Cap</option>\n", STYPE2_LARGE_CAP, MaintStock.xstype2[0] == STYPE2_LARGE_CAP ? " selected" : "" );
	printf ( "<option value='%c'%s>Mid-Cap</option>\n",   STYPE2_MID_CAP, MaintStock.xstype2[0] == STYPE2_MID_CAP ? " selected" : "" );
	printf ( "<option value='%c'%s>Small-Cap</option>\n", STYPE2_SMALL_CAP, MaintStock.xstype2[0] == STYPE2_SMALL_CAP ? " selected" : "" );
	printf ( "<option value='%c'%s>Developed</option>\n", STYPE2_DEVELOPED, MaintStock.xstype2[0] == STYPE2_DEVELOPED ? " selected" : "" );
	printf ( "<option value='%c'%s>Emerging</option>\n",  STYPE2_EMERGING, MaintStock.xstype2[0] == STYPE2_EMERGING ? " selected" : "" );
	printf ( "<option value='%c'%s>Other</option>\n",     STYPE2_OTHER, MaintStock.xstype2[0] == STYPE2_OTHER ? " selected" : "" );
	printf ( "<option value='%c'%s>Times 100</option>\n",  STYPE2_HUNDREDTH, MaintStock.xstype2[0] == STYPE2_HUNDREDTH ? " selected" : "" );
	printf ( "<option value='%c'%s>Times 10000</option>\n",STYPE2_BASIS, MaintStock.xstype2[0] == STYPE2_BASIS ? " selected" : "" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "<td>&nbsp;\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	// fixit make editable?
	printf ( "<tr>\n" );
	printf ( "<td>EXCHANGE</td>\n" );
	printf ( "<td>%s</td>\n", MaintStock.xsexchange );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>Edgar CIK</td>\n" );
	printf ( "<td>%s</td>\n", MaintStock.xscik );
	printf ( "</tr>\n" );

	switch (  MaintStock.xstype[0] )
	{
		case STYPE_STOCK:
		case STYPE_ADR:
		case STYPE_REIT:
		case STYPE_PREFER:
// fixit make editable?
			printf ( "<tr>\n" );
			printf ( "<td>SECTOR</td>\n" );
			printf ( "<td>%s</td>\n", xsector.xsecname );
			printf ( "</tr>\n" );

			printf ( "<tr>\n" );
			printf ( "<td>INDUSTRY</td>\n" );
			printf ( "<td>%s</td>\n", xindustry.xindname );
			printf ( "</tr>\n" );
			break;
	}

	printf ( "<tr>\n" );
	printf ( "<td colspan='4'>\n" );
	if ( lastchr ( MaintStock.xsticker, sizeof(MaintStock.xsticker) ) > 0 )
	{
		printf ( "&emsp;<input type='submit' name='submitClear' value='Clear' %s>\n", SET_SELF );

		printf ( "&emsp;" );
		printf ( "<input type='submit' name='SubmitStockChart' value='Chart' onclick='javascript:SetNamed(\"sheet\");'>\n" );

		printf ( "&emsp;" );
		printf ( "<input type='submit' name='SubmitStockDividends' value='Dividends' onclick='javascript:SetNamed(\"sheet\");'>\n" );

		printf ( "&emsp;" );
		printf ( "<input type='button' id='SubmitStockFundFmt' value='Fundamentals'" );
		printf ( " onclick='javascript:ShowFundamentals(\"field_Sticker\",\"\");'>\n" );

		if ( nsAtol(MaintStock.xscik) > 0 )
		{
			printf ( "&emsp;" );
			printf ( "<input type='submit' name='SubmitStockFundRaw' value='Fundamentals (raw)' onclick='javascript:SetNamed(\"sheet\");'>\n" );
		}

		printf ( "&emsp;" );
		printf ( "<input class='disabled' type='submit' name='SubmitStockStatsRaw' value='Stats (raw)' onclick='javascript:SetNamed(\"sheet\");' disabled>\n" );

		printf ( "&emsp;" );
		printf ( "<input type='submit' name='SubmitSaveStock' value='Save' %s>\n", SET_SELF );

		printf ( "&emsp;" );
		printf ( "<input type='submit' name='SubmitDeleteStock' value='Delete' %s>\n", SET_SELF );
	}
	if ( ScreenLevel == 1 )
	{
		printf ( "&emsp;<input type='submit' name='submitFind' value='Find' %s>\n", SET_SELF );

		if ( CanInsertStock )
		{
			if ( CookieMember.xid == DEMO_MEMBER )
			{
				printf ( "&emsp;<input type='button' value='Insert Disabled'>\n" );
			}
			else
			{
				printf ( "&emsp;<input type='submit' name='submitInsert' value='Insert' %s>\n", SET_SELF );
			}
		}
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>\n" );
}

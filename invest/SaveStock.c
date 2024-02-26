/*----------------------------------------------------------------------------
	Program : invest/SaveStock.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Save new/changed stock record.
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

static int DebugSaveStock = 0;

void SaveStock ()
{
	switch ( RunMode )
	{
		case MODE_SAVE_STOCK:
			sprintf ( Statement, 
"update stock set Sname = '%s', Sdj = '%c', Ssp500 = '%c', Srussell = '%c', Snasdaq = '%c', Stype = '%c', Stype2 = '%c', Sdomfor = '%c' where stock.Sticker = '%s'",
					MaintStock.xsname,
					MaintStock.xsdj[0],
					MaintStock.xssp500[0],
					MaintStock.xsrussell[0],
					MaintStock.xsnasdaq[0],
					MaintStock.xstype[0],
					MaintStock.xstype2[0],
					MaintStock.xsdomfor[0],
					MaintStock.xsticker );

			if ( DebugSaveStock )
			{
				SafeError ( COLOR_SUCCESS, Statement );
				printf ( "%s<br>", Statement );
			}
			else
			{
				if ( dbyUpdate ( "invest", &MySql, Statement, 0, LogFileName ) < 1 )
				{
					SafeError ( COLOR_ERROR, "Save stock failed." );
					return;
				}

				SafeError ( COLOR_SUCCESS, "Saved stock okay." );
			}
			break;
	}
}

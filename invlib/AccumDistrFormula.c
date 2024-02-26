/*----------------------------------------------------------------------------
	Program : invlib/AccumDistrFormula.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Calculate Accumulate - Distribution ratio
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

double AccumDistrFormula ( int UpCount, int DownCount )
{
	double	Ratio;

	/*----------------------------------------------------------
		Up/(Up+Down)		yields numbers from 0.0 to 1.0
		(Up-Down)/(Up+Down)	yields numbers from -1.0 to 1.0
	----------------------------------------------------------*/
	Ratio = (double) UpCount / (double) (UpCount + DownCount);

	return ( Ratio );
}

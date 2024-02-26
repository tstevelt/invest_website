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

function RunPage ( Number )
{
	var elem = document.getElementById ( 'MenuIndex' );
	elem.value = Number;
	document.invest.submit();
}

function SetSelf ()
{
	window.document.invest.target = '_self';
}

function SetNamed ( Name )
{
	window.document.invest.target = Name;
}

function SetTarget ( Object )
{
	var MyFormat = Object.value;
	if ( MyFormat == 'H' ||  MyFormat == 'V' )
	{
		window.document.invest.target = '_blank';
	}
	else
	{
		window.document.invest.target = '_self';
	}
}

function ShowFundamentals ( Field_Ticker, FocusOjb )
{
	var TickerElem = document.getElementById ( Field_Ticker );
	var FocusElem = document.getElementById ( FocusOjb );

	// alert ( TickerElem.value );
	// alert ( FocusElem.value );

	var command = 'fundamentals.cgi?Ticker=' + TickerElem.value;
	// alert ( command );

	window.open(command, "popup", "width=500,height=800" );

	FocusElem.focus();
}

function SubmitTarget ( ReportFormatID )
{
	var elem = document.getElementById ( ReportFormatID );
	var MyFormat = '?';

	MyFormat = elem.value;

	if ( MyFormat == 'H' ||  MyFormat == 'V' )
	{
		window.document.invest.target = '_blank';
	}
	else
	{
		window.document.invest.target = '_self';
	}


	return ( true );
}

function AlertTypeToggle ( SelectorObj )
{
	var elemAlertPrice   = document.getElementById ( 'Palertprice' );
	var elemTrailPercent = document.getElementById ( 'Ptrailpct' );
	var elemBenchTicker  = document.getElementById ( 'Pbenchmark' );
	var elemBenchAlert   = document.getElementById ( 'Palert' ); 

	elemAlertPrice.disabled = true;
	elemTrailPercent.disabled = true;
	elemBenchTicker.disabled = true;
	elemBenchAlert.disabled = true;

	switch ( SelectorObj.value )
	{
		case 'A':
		case 'B':
			elemAlertPrice.disabled = false;
			break;
		case 'C':
			elemTrailPercent.disabled = false;
			break;
		case 'D':
			elemBenchTicker.disabled = false;
			elemBenchAlert.disabled = false;
			break;
	}
}


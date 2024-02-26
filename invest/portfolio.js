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


function FormatNumber ( BigNumber )
{
	var WorkNumber = 0.0;
	var FormattedNumber = '';

	if ( BigNumber < 10000 )
	{
		return ( BigNumber );
	}
	else if ( BigNumber < 1000000 )
	{
		WorkNumber = BigNumber / 1000.0;
		FormattedNumber = WorkNumber.toFixed(2) + ' K';
	}
	else
	{
		WorkNumber = BigNumber / 1000000.0;
		FormattedNumber = WorkNumber.toFixed(2) + ' M';
	}

	return ( FormattedNumber );
}

function DrawPortfolioChart ()
{
	var stockCanvas = document.getElementById ( 'stockCanvas' );
	var stockCtx = stockCanvas.getContext('2d');
	var leftmargin = 100.0;
	var margin = 20.0;
	var increment = (StockChartWidth - leftmargin - margin) / (History.length + 1);
	var tick_x = 0;
	var top_y = 0;
	var bottom_y = 0;
	var stockdatarange = StockMaxHigh - StockMinLow;
	var stockarearange = StockChartHeight - 2 * margin;
	var percent = 0.0;
	var StartIndex = 4;
	var IncIndex = 10;

	/*----------------------------------------------------------
		portfolio chart - stacked bars
	----------------------------------------------------------*/
	stockCtx.beginPath(); 
	stockCtx.strokeStyle = '#000000';

	stockCtx.moveTo(leftmargin,margin);
	stockCtx.lineTo(leftmargin,StockChartHeight-margin);
	stockCtx.stroke();

	stockCtx.moveTo(leftmargin,StockChartHeight-margin);
	stockCtx.lineTo(StockChartWidth-margin,StockChartHeight-margin);
	stockCtx.stroke();

	/*----------------------------------------------------------
		draw stacked bars

		e e e e 
		e e e e 
		e e e e 
		b b b b 
		b b b b
		c c c c
	----------------------------------------------------------*/
	var ThisBarHeight = 0;
	var PrevStartY = 0;
	var StartX, StartY;
	var Value = 0.0;
	var TotalY = StockChartHeight - 2 * margin;
	// console.log ( increment );
	if ( increment > 200 )
	{
		increment = 200;
	}
	var BarWidth = increment * 0.75;

	for ( var ndx = 0; ndx < History.length; ndx++ )
	{
		StartX = leftmargin + increment*0.25 + ndx*increment;

		/*----------------------------------------------------------
			cash
		----------------------------------------------------------*/
		Value = History[ndx].Cash;
		stockCtx.fillStyle = "red";

		percent = Value / stockdatarange;
		ThisBarHeight = TotalY * percent;

		StartY = margin + TotalY - ThisBarHeight;

		stockCtx.fillRect ( StartX, StartY, BarWidth, ThisBarHeight );
		// console.log ( 'cash', StartX, StartY, BarWidth, ThisBarHeight, percent );

		PrevStartY = StartY;

		/*----------------------------------------------------------
			bonds
		----------------------------------------------------------*/
		Value = History[ndx].Bonds;
		stockCtx.fillStyle = "green";

		percent = Value / stockdatarange;
		ThisBarHeight = TotalY * percent;

		StartY = PrevStartY - ThisBarHeight;

		stockCtx.fillRect ( StartX, StartY, BarWidth, ThisBarHeight );
		// console.log ( 'bonds', StartX, StartY, BarWidth, ThisBarHeight, percent );

		PrevStartY = StartY;

		/*----------------------------------------------------------
			equity
		----------------------------------------------------------*/
		Value = History[ndx].Equity;
		stockCtx.fillStyle = "gold";

		percent = Value / stockdatarange;
		ThisBarHeight = TotalY * percent;

		StartY = PrevStartY - ThisBarHeight;

		stockCtx.fillRect ( StartX, StartY, BarWidth, ThisBarHeight );
		// console.log ( 'equity', StartX, StartY, BarWidth, ThisBarHeight, percent );
	}

	/*----------------------------------------------------------
		write dates across bottom of stock chart
	----------------------------------------------------------*/
	stockCtx.fillStyle = "black";
	stockCtx.strokeStyle = '#CCCCCC';

	stockCtx.beginPath(); 
	stockCtx.font = '15px Arial';
	stockCtx.textAlign = 'center';

	if ( History.length < 200 )
	{
		StartIndex = 4;
		IncIndex = 10;
	}
	else if ( History.length < 330 )
	{
		StartIndex = 6;
		IncIndex = 22;
	}
	else 
	{
		StartIndex = 10;
		IncIndex = 44;
	}

	for ( ndx = StartIndex; ndx < History.length; ndx += IncIndex )
	{
		tick_x = leftmargin + increment*0.25 + ndx*increment;
		stockCtx.fillText ( History[ndx].Date, tick_x, StockChartHeight - 3 );

		top_y = 0;
		bottom_y = StockChartHeight - margin;

		stockCtx.moveTo(tick_x,top_y);
		stockCtx.lineTo(tick_x,bottom_y);
	}
	stockCtx.stroke();

	/*----------------------------------------------------------
		write y-axis prices
	----------------------------------------------------------*/
	stockCtx.beginPath(); 
	stockCtx.font = '15px Arial';
	stockCtx.textBaseline = "middle"; 
	stockCtx.textAlign = "end"; 
	for ( ndx = 0; ndx < Yaxis.length; ndx++ )
	{
		tick_x = leftmargin - 3;

		percent = (Yaxis[ndx] - StockMinLow) / stockdatarange;
		top_y =  StockChartHeight - margin - stockarearange * percent;
		stockCtx.fillText ( Yaxis[ndx], tick_x, top_y);

		stockCtx.moveTo(leftmargin,top_y);
		stockCtx.lineTo(StockChartWidth - margin,top_y);

	}
	stockCtx.stroke();

}

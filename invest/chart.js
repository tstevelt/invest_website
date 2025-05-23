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

function DrawStockChart ()
{
	var stockCanvas = document.getElementById ( 'stockCanvas' );
	var stockCtx = stockCanvas.getContext('2d');
	var leftmargin = 100.0;
	var margin = 20.0;
	var increment = (StockChartWidth - leftmargin - margin) / (History.length + 1);
	var tick_x = 0;
	var tick_y = 0;
	var tick_w = increment / 3.0;
	var open_y = 0;
	var high_y = 0;
	var low_y = 0;
	var close_y = 0;
	var avg_y = 0;
	var top_x = 0;
	var top_y = 0;
	var bottom_y = 0;
	var volume_h = 0;
	var stockdatarange = StockMaxHigh - StockMinLow;
	var stockarearange = StockChartHeight - 2 * margin;
	if ( VolumeMaxHigh > 0 )
	{
		var volumeCanvas = document.getElementById ( 'volumeCanvas' );
		var volumeCtx = volumeCanvas.getContext('2d');
		var volumedatarange = VolumeMaxHigh;
		var volumearearange = VolumeChartHeight - 2 * margin;
	}
	var percent = 0.0;
	var StartIndex = 4;
	var IncIndex = 10;

	/*----------------------------------------------------------
		stock chart - ohlc, slow and fast averages.
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
		draw red and green OHLC ticks
	----------------------------------------------------------*/
	for ( var ndx = 0; ndx < History.length; ndx++ )
	{
		tick_x = leftmargin+(ndx+1)*increment;

		stockCtx.beginPath(); 
		if ( History[ndx].Open <= History[ndx].Close )
		{
			stockCtx.strokeStyle = '#00CC00';
		}
		else
		{
			stockCtx.strokeStyle = '#CC0000';
		}

		/* paint horizontal open */
		percent = (History[ndx].Open - StockMinLow) / stockdatarange;
		open_y = StockChartHeight - margin - stockarearange * percent;

		stockCtx.moveTo(tick_x,open_y);
		stockCtx.lineTo(tick_x-tick_w,open_y);

		/* paint horizontal close */
		percent = (History[ndx].Close - StockMinLow) / stockdatarange;
		close_y = StockChartHeight - margin - stockarearange * percent;

		stockCtx.moveTo(tick_x,close_y);
		stockCtx.lineTo(tick_x+tick_w,close_y);

		/* paint vertical high low */
		percent = (History[ndx].Low - StockMinLow) / stockdatarange;
		low_y = StockChartHeight - margin - stockarearange * percent;
		percent = (History[ndx].High - StockMinLow) / stockdatarange;
		high_y = StockChartHeight - margin - stockarearange * percent;

		stockCtx.moveTo(tick_x,low_y);
		stockCtx.lineTo(tick_x,high_y);
		stockCtx.stroke();
	}

	/*----------------------------------------------------------
		draw slow moving average - dark gray
	----------------------------------------------------------*/
	stockCtx.beginPath(); 
	stockCtx.strokeStyle = '#333333';
	for ( ndx = 0; ndx < History.length - 1; ndx++ )
	{
		if ( History[ndx].Slow < 0.1 || History[ndx+1].Slow  < 0.1)
		{
			continue;
		}
		tick_x = leftmargin+(ndx+1)*increment;
		percent = (History[ndx].Slow - StockMinLow) / stockdatarange;
		avg_y = StockChartHeight - margin - stockarearange * percent;
		stockCtx.moveTo(tick_x,avg_y);

		tick_x = leftmargin+(ndx+2)*increment;
		percent = (History[ndx+1].Slow - StockMinLow) / stockdatarange;
		avg_y = StockChartHeight - margin - stockarearange * percent;
		stockCtx.lineTo(tick_x,avg_y);
	}
	stockCtx.stroke();

	/*----------------------------------------------------------
		draw fast moving average - dark orange
	----------------------------------------------------------*/
	stockCtx.beginPath(); 
	stockCtx.strokeStyle = '#FF8C00';
	for ( ndx = 0; ndx < History.length - 1; ndx++ )
	{
		if ( History[ndx].Fast < 0.1 || History[ndx+1].Fast < 0.1 )
		{
			continue;
		}
		tick_x = leftmargin+(ndx+1)*increment;
		percent = (History[ndx].Fast - StockMinLow) / stockdatarange;
		avg_y = StockChartHeight - margin - stockarearange * percent;
		stockCtx.moveTo(tick_x,avg_y);

		tick_x = leftmargin+(ndx+2)*increment;
		percent = (History[ndx+1].Fast - StockMinLow) / stockdatarange;
		avg_y = StockChartHeight - margin - stockarearange * percent;
		stockCtx.lineTo(tick_x,avg_y);
	}
	stockCtx.stroke();
	
	/*----------------------------------------------------------
		bollinger bands if one of the MA match STDDEV period
	----------------------------------------------------------*/
	var DrawBollinger = 0;
	var HighBand = 0;
	var LowBand = 0;
	if ( SlowDays == 50 )
	{
		DrawBollinger = 1;
		stockCtx.strokeStyle = '#333333';
	}
	else if ( FastDays == 50 )
	{
		DrawBollinger = 1;
		stockCtx.strokeStyle = '#FF8C00';
	}
	if ( DrawBollinger )
	{
		stockCtx.beginPath(); 

		for ( ndx = 0; ndx < History.length - 1; ndx++ )
		{
			if ( SlowDays == 50 )
			{
				tick_x = leftmargin+(ndx+1)*increment;
				percent = (History[ndx].Slow + 2.0 * History[ndx].SD - StockMinLow) / stockdatarange;
				avg_y = StockChartHeight - margin - stockarearange * percent;
				stockCtx.moveTo(tick_x,avg_y);

				tick_x = leftmargin+(ndx+2)*increment;
				percent = (History[ndx+1].Slow + 2.0 * History[ndx].SD - StockMinLow) / stockdatarange;
				avg_y = StockChartHeight - margin - stockarearange * percent;
				stockCtx.lineTo(tick_x,avg_y);

				tick_x = leftmargin+(ndx+1)*increment;
				percent = (History[ndx].Slow - 2.0 * History[ndx].SD - StockMinLow) / stockdatarange;
				avg_y = StockChartHeight - margin - stockarearange * percent;
				stockCtx.moveTo(tick_x,avg_y);

				tick_x = leftmargin+(ndx+2)*increment;
				percent = (History[ndx+1].Slow - 2.0 * History[ndx].SD - StockMinLow) / stockdatarange;
				avg_y = StockChartHeight - margin - stockarearange * percent;
				stockCtx.lineTo(tick_x,avg_y);

			}
			else
			{
				tick_x = leftmargin+(ndx+1)*increment;
				percent = (History[ndx].Fast + 2.0 * History[ndx].SD - StockMinLow) / stockdatarange;
				avg_y = StockChartHeight - margin - stockarearange * percent;
				stockCtx.moveTo(tick_x,avg_y);

				tick_x = leftmargin+(ndx+2)*increment;
				percent = (History[ndx+1].Fast + 2.0 * History[ndx].SD - StockMinLow) / stockdatarange;
				avg_y = StockChartHeight - margin - stockarearange * percent;
				stockCtx.lineTo(tick_x,avg_y);

				tick_x = leftmargin+(ndx+1)*increment;
				percent = (History[ndx].Fast - 2.0 * History[ndx].SD - StockMinLow) / stockdatarange;
				avg_y = StockChartHeight - margin - stockarearange * percent;
				stockCtx.moveTo(tick_x,avg_y);

				tick_x = leftmargin+(ndx+2)*increment;
				percent = (History[ndx+1].Fast - 2.0 * History[ndx].SD - StockMinLow) / stockdatarange;
				avg_y = StockChartHeight - margin - stockarearange * percent;
				stockCtx.lineTo(tick_x,avg_y);

			}
		}

		stockCtx.stroke();
	}


	/*----------------------------------------------------------
		draw linear regression channel - dark blue
	----------------------------------------------------------*/
	stockCtx.beginPath(); 
	stockCtx.strokeStyle = '#00008B';
	tick_x = leftmargin;
	percent = (StartIntercept - StockMinLow) / stockdatarange;
	tick_y = StockChartHeight - margin - stockarearange * percent;
	stockCtx.moveTo(tick_x,tick_y);
	tick_x = leftmargin + (History.length - 1) * increment;
	percent = (EndIntercept - StockMinLow) / stockdatarange;
	tick_y = StockChartHeight - margin - stockarearange * percent;
	stockCtx.lineTo(tick_x,tick_y);
	stockCtx.stroke();

	stockCtx.beginPath(); 
	stockCtx.strokeStyle = '#00008B';
	tick_x = leftmargin;
	percent = (StartIntercept + StdDev - StockMinLow) / stockdatarange;
	tick_y = StockChartHeight - margin - stockarearange * percent;
	stockCtx.moveTo(tick_x,tick_y);
	tick_x = leftmargin + (History.length - 1) * increment;
	percent = (EndIntercept + StdDev - StockMinLow) / stockdatarange;
	tick_y = StockChartHeight - margin - stockarearange * percent;
	stockCtx.lineTo(tick_x,tick_y);
	stockCtx.stroke();

	stockCtx.beginPath(); 
	stockCtx.strokeStyle = '#00008B';
	tick_x = leftmargin;
	percent = (StartIntercept - StdDev - StockMinLow) / stockdatarange;
	tick_y = StockChartHeight - margin - stockarearange * percent;
	stockCtx.moveTo(tick_x,tick_y);
	tick_x = leftmargin + (History.length - 1) * increment;
	percent = (EndIntercept - StdDev - StockMinLow) / stockdatarange;
	tick_y = StockChartHeight - margin - stockarearange * percent;
	stockCtx.lineTo(tick_x,tick_y);
	stockCtx.stroke();

	/*----------------------------------------------------------
		write dates across bottom of stock chart
	----------------------------------------------------------*/
	stockCtx.beginPath(); 
	stockCtx.strokeStyle = '#CCCCCC';
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
		tick_x = leftmargin+(ndx+1)*increment;
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
	stockCtx.strokeStyle = '#CCCCCC';
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
	
	/*----------------------------------------------------------
		tms 04/19/2025 currency pairs do not have volume info.
	----------------------------------------------------------*/
	if ( VolumeMaxHigh < 1 )
	{
		return;
	}

	/*----------------------------------------------------------
		volume chart - daily bars and moving average
	----------------------------------------------------------*/
	volumeCtx.beginPath(); 
	volumeCtx.strokeStyle = '#000000';

	volumeCtx.moveTo(leftmargin,margin);
	volumeCtx.lineTo(leftmargin,VolumeChartHeight-margin);
	volumeCtx.stroke();

	volumeCtx.moveTo(leftmargin,VolumeChartHeight-margin);
	volumeCtx.lineTo(VolumeChartWidth-margin,VolumeChartHeight-margin);
	volumeCtx.stroke();

	/*----------------------------------------------------------
		draw red and green volume bars
	----------------------------------------------------------*/
	for ( ndx = 0; ndx < History.length; ndx++ )
	{
		tick_x = leftmargin+(ndx+1)*increment;

		if ( History[ndx].Open <= History[ndx].Close )
		{
			volumeCtx.fillStyle = '#00CC00';
		}
		else
		{
			volumeCtx.fillStyle = '#CC0000';
		}

		top_x = tick_x - increment * 0.4;

		percent = History[ndx].Volume / volumedatarange;
		top_y = VolumeChartHeight - margin - volumearearange * percent;

		volume_h = volumearearange * percent;

		volumeCtx.fillRect ( top_x, top_y, increment * 0.8, volume_h );

	}

	/*----------------------------------------------------------
		write verical marks on volume chart
	----------------------------------------------------------*/
	volumeCtx.beginPath(); 
	volumeCtx.strokeStyle = '#CCCCCC';
	for ( ndx = StartIndex; ndx < History.length; ndx += IncIndex )
	{
		tick_x = leftmargin+(ndx+1)*increment;

		top_y = 0;
		bottom_y = VolumeChartHeight - margin;

		volumeCtx.moveTo(tick_x,top_y);
		volumeCtx.lineTo(tick_x,bottom_y);
	}
	volumeCtx.stroke();

	/*----------------------------------------------------------
		write max volume in left margin
		write starting average volume in left margin
	----------------------------------------------------------*/
	volumeCtx.beginPath(); 
	volumeCtx.fillStyle = '#000000';
	volumeCtx.font = '15px Arial';
	volumeCtx.textBaseline = "middle"; 
	volumeCtx.textAlign = "end"; 
	tick_x = leftmargin - 3;
	top_y =  margin;
	volumeCtx.fillText ( FormatNumber(VolumeMaxHigh), tick_x, top_y);

	tick_x = leftmargin - 3;
	percent = (History[0].Avg) / volumedatarange;
	top_y =  VolumeChartHeight - margin - volumearearange * percent;
	volumeCtx.fillText ( FormatNumber(History[0].Avg), tick_x, top_y);

	/*----------------------------------------------------------
		draw volume moving average in dark orange
	----------------------------------------------------------*/
	volumeCtx.beginPath(); 
	volumeCtx.strokeStyle = '#FF8C00';
	for ( ndx = 0; ndx < History.length - 1; ndx++ )
	{
		tick_x = leftmargin+(ndx+1)*increment;
		percent = (History[ndx].Avg) / volumedatarange;
		avg_y = VolumeChartHeight - margin - volumearearange * percent;
		volumeCtx.moveTo(tick_x,avg_y);

		tick_x = leftmargin+(ndx+2)*increment;
		percent = (History[ndx+1].Avg) / volumedatarange;
		avg_y = VolumeChartHeight - margin - volumearearange * percent;
		volumeCtx.lineTo(tick_x,avg_y);
	}
	volumeCtx.stroke();
}

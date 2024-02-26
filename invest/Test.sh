#     Stock market website
# 
#     Copyright (C)  2019-2024 Tom Stevelt
# 
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU Affero General Public License as
#     published by the Free Software Foundation, either version 3 of the
#     License, or (at your option) any later version.
# 
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU Affero General Public License for more details.
# 
#     You should have received a copy of the GNU Affero General Public License
#     along with this program.  If not, see <https://www.gnu.org/licenses/>.


export REMOTE_ADDR=192.22.33.44

#export HTTP_COOKIE='invest=k7cR1mm1eeggmJsb:1:1:1464'
export HTTP_COOKIE='invest=tIrtrYEhJxR8lL88:1:1:1792'

#export QUERY_STRING='MenuIndex=2&StockIndex=S&DateRange=Y&TopCount=10&ReportFormat=C&submitRegress=Report'

gdb /var/www/vhosts/shs_stocks/html/invest.cgi 


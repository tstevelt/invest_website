/*----------------------------------------------------------------------------
	Program : invlib/SetMemberLimits.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Set limits for various member roles
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

#include	"invprivate.h"
#include	"invlib.h"

void SetMemberLimits ( char MemberRole )
{
	switch ( MemberRole )
	{
		case ROLE_ADMIN:
		case ROLE_PAID:
		case ROLE_BETA:
			PortfolioLimit = PAID_MEMBER_PORTFOLIO_LIMIT;
			WatchlistLimit = PAID_MEMBER_WATCHLIST_LIMIT;
			ReportLimit    = PAID_UNLIMITED;
			break;
		case ROLE_FREE:
		case ROLE_EXPIRED:
		default:
			PortfolioLimit = FREE_MEMBER_PORTFOLIO_LIMIT;
			WatchlistLimit = FREE_MEMBER_WATCHLIST_LIMIT;
			ReportLimit    = FREE_MEMBER_REPORTS_LIMIT;
			break;
	}
}

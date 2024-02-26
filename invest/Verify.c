/*----------------------------------------------------------------------------
	Program : invest/Verify.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: User has verified their email address.
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

int Verify ()
{
	DBY_QUERY     *qryMember;
	int			rv = 0;
	long		Affected;

	/*----------------------------------------------------------
		select name and status by  id and ipaddr
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select Mname, Mstatus from member where member.id = %ld and Mipaddr = '%s'",
			VerifyID, Remote_Address );

	qryMember = dbySelect ( "Verify", &MySql, Statement, LogFileName );
	if ( qryMember == (DBY_QUERY *) 0 )
	{
		SafeError ( COLOR_ERROR, "1. Verify select returned NULL." );
		rv = -1;
	}
	else if ( qryMember->NumRows == 0 )
	{
		SafeError ( COLOR_ERROR, "Verify select returned zero rows." );
		SafeError ( COLOR_ERROR, "Did you verify from the same computer that you signed up on?" );
		rv = -2;
	}
	else 
	{
		/*---------------------------------------------------------
			check status
		----------------------------------------------------------*/
		qryMember->EachRow = mysql_fetch_row ( qryMember->Result );
		switch ( qryMember->EachRow[1][0] )
		{
			case 'P':
				/*----------------------------------------------------------
					update to 'V'
				----------------------------------------------------------*/
				snprintf ( Statement, sizeof(Statement),
					"update member set Mstatus = '%c' where member.id = %ld", STATUS_VERIFIED, VerifyID );

				Affected = dbyUpdate ( "Verify", &MySql, Statement, 0, LogFileName );
				if ( Affected == 0 )
				{
					SafeError ( COLOR_ERROR, "Verify failed." );
					rv = -3;
				}
				else
				{
					char	Message[128];

					SafeError ( COLOR_SUCCESS, "Thank you for verifying your email, %s", qryMember->EachRow[0] );
					SafeError ( COLOR_SUCCESS, "When you UPGRADE your account you can earn $%d for each paid referral!", REFERRAL_FEE  );

					sprintf ( Message, "%ld %s verified email address", VerifyID, qryMember->EachRow[0] );
					NotifyAdmin ( Message );
				}
				break;

			case 'V':
				SafeError ( COLOR_ERROR, "%s is already verified!", qryMember->EachRow[0] );
				break;

			default:	
				SafeError ( COLOR_ERROR, "Unknown member.Mstatus [%c]", qryMember->EachRow[1][0] );
				rv = -4;
				break;
		}
	}

	return ( rv );
}

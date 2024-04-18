/*----------------------------------------------------------------------------
	Program : invlib/LoadMemberCB.c
	Author  : Tom Stevelt
	Date    : 2019-2024
	Synopsis: Load this table, with CallBack function
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

static  char    Statement[MAXSTATEMENT];

int LoadMemberCB ( MYSQL *MySql, char * WhereClause, char * OrderByClause, XMEMBER *ptrMember, int (*CallBack)(), int Verbose )
{
	static	DBY_QUERY     *qryMember;

	/*----------------------------------------------------------
		select standard field list by given where clause
	----------------------------------------------------------*/
	snprintf ( Statement, sizeof(Statement),
		"select %s from member", FIELDS_MEMBER );

	if ( WhereClause != NULL && nsStrlen ( WhereClause ) > 1 && nsStrcmp ( WhereClause, "all" ) != 0 )
	{
		strcat ( Statement, " where " );
		strcat ( Statement, WhereClause );
	}

	if ( OrderByClause != NULL && nsStrlen ( OrderByClause ) > 1 )
	{
		strcat ( Statement, " order by " );
		strcat ( Statement, OrderByClause );
	}

	qryMember = dbySelect ( "LoadMember", MySql, Statement, LogFileName );
	if ( qryMember == (DBY_QUERY *) 0 )
	{
		if ( Verbose )
		{
			SafeError ( COLOR_ERROR, "LoadMember select returned NULL." );
		}
		return ( -1 );
	}
	else if ( qryMember->NumRows == 0 )
	{
		if ( Verbose )
		{
			SafeError ( COLOR_ERROR, "LoadMember select returned zero rows." );
		}
		return ( 0 );
	}
	else 
	{
		while (( qryMember->EachRow = mysql_fetch_row ( qryMember->Result )) != NULL )
		{

// id, Mname, Memail, Mpager, Mpassword, Mstatus, Mrole, Mipaddr, Minsdt, Mlogdt, Msuborig, Msubstart, Msubpaid, Msubexpire, Mmypromo, Mrefpromo, Mrefpaid, Mchrtnum, Mchrtslow, Mchrtfast"
// #define FIELDS_MEMBER  "id,Mname,Memail,Mpager,Mpassword,Mstatus,Mrole,Mipaddr,Minsdt,Mlogdt,Msuborig,Msubstart,Msubpaid,Msubexpire,Mmypromo,Mrefpromo,Mrefpaid,Mchrtnum,Mchrtslow,Mchrtfast,McashT,MbondT,MdomT,MforT"

			ptrMember->xid         =  nsAtol( qryMember->EachRow[0] );
			snprintf ( ptrMember->xmname, sizeof(ptrMember->xmname), "%s", qryMember->EachRow[1] );
			snprintf ( ptrMember->xmemail, sizeof(ptrMember->xmemail), "%s", qryMember->EachRow[2] );
			snprintf ( ptrMember->xmpager, sizeof(ptrMember->xmpager), "%s", qryMember->EachRow[3] ? qryMember->EachRow[3] : " " );
			snprintf ( ptrMember->xmpassword, sizeof(ptrMember->xmpassword), qryMember->EachRow[4] );
			ptrMember->xmstatus[0] =  qryMember->EachRow[5][0];
			ptrMember->xmrole[0]   =  qryMember->EachRow[6][0];
			sprintf ( ptrMember->xmipaddr, "%-16.16s", qryMember->EachRow[7] );
			sprintf ( ptrMember->xminsdt, "%-10.10s", qryMember->EachRow[8] );
			sprintf ( ptrMember->xmlogdt, "%-10.10s", qryMember->EachRow[9] );

			sprintf ( ptrMember->xmsuborig,   "%-10.10s", qryMember->EachRow[10] );
			sprintf ( ptrMember->xmsubstart,  "%-10.10s", qryMember->EachRow[11] );
			ptrMember->xmsubpaid =                  nsAtof( qryMember->EachRow[12] );
			sprintf ( ptrMember->xmsubexpire, "%-10.10s", qryMember->EachRow[13] );
			snprintf ( ptrMember->xmmypromo, sizeof(ptrMember->xmmypromo), "%s", qryMember->EachRow[14] ? qryMember->EachRow[14] : " " );
			snprintf ( ptrMember->xmrefpromo, sizeof(ptrMember->xmrefpromo), "%s", qryMember->EachRow[15] ? qryMember->EachRow[15] : " " );
			ptrMember->xmrefpaid =                  nsAtof( qryMember->EachRow[16] );
			ptrMember->xmchrtnum =  nsAtoi( qryMember->EachRow[17] );
			ptrMember->xmchrtslow =  nsAtoi( qryMember->EachRow[18] );
			ptrMember->xmchrtfast =  nsAtoi( qryMember->EachRow[19] );

// McashT,MbondT,MdomT,MforT
			ptrMember->xmcasht = nsAtof( qryMember->EachRow[20] );
			ptrMember->xmbondt = nsAtof( qryMember->EachRow[21] );
			ptrMember->xmdomt  = nsAtof( qryMember->EachRow[22] );
			ptrMember->xmfort  = nsAtof( qryMember->EachRow[23] );

			if ( CallBack != NULL )
			{
				if ( CallBack ( ptrMember ) != 0 )
				{
					break;
				}
			}
		}
	}

	return ( qryMember->NumRows );
}

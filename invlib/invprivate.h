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


#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<ctype.h>

#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <libgen.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <sys/types.h>
#include <curl/curl.h>
// TYPE    CURL        *curl;
// TYPE    CURLcode    curlRV;

#include	"shslib.h"
#include	"rptlib.h"
#include	"dbylib.h"

#define		STOCK
#define		AVERAGE
#define		HISTORY
#define		CRYPTO
#define		PORTFOLIO
#define		PORTHIST
#define		CASHBAL
#define		MEMBER
#define		WATCHLIST
#define		TEMP
#define		SYSTEM
#define		MARKET
#define		DIVIDEND
#define		INDUSTRY
#define		SECTOR
#define		FUNDAMENTAL
#include	"fileinvest.h"

/*----------------------------------------------------------
	mysql and dbylib stuff
----------------------------------------------------------*/
TYPE  MYSQL   MySql;
TYPE	DBY_OPTIONS	DbyOptions;
TYPE	char		WhereClause[1024];
TYPE	char		OrderByClause[1024];
#ifdef MAIN
//TYPE	char	*LogFileName = "/var/local/invest.log";
TYPE	char	*LogFileName = WORKDIR + "/invest.log";
#else
TYPE	char	*LogFileName;
#endif


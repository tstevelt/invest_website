/*--------------------------------------------------------
	fileinvest.h for schema invest.schema
--------------------------------------------------------*/
#ifndef FILEINVEST_H
#define FILEINVEST_H

#ifndef TYPE
#ifdef MAIN
#define  TYPE   /* */
#else
#define  TYPE   extern
#endif
#endif

#ifdef AVERAGE
#define TABLE_average 1
typedef struct
{
	long	xaid;
	char 	xaticker[21];
	char 	xadate[12];
	double	xama5;
	double	xama10;
	double	xama20;
	double	xama50;
	double	xama100;
	double	xama200;
	long	xavol50;
	double	xarsi;
	double	xastddev;
	double	xactb;
} XAVERAGE;
TYPE XAVERAGE xaverage;
#define FIELDS_AVERAGE  "Aid,Aticker,Adate,Ama5,Ama10,Ama20,Ama50,Ama100,Ama200,Avol50,Arsi,Astddev,Actb"
#endif

#ifdef CASHBAL
#define TABLE_cashbal 2
typedef struct
{
	unsigned long	xid;
	unsigned long	xcbmember;
	char 	xcbacct[31];
	char 	xcbdate[12];
	double	xcbamount;
	char 	xcbcomment[61];
} XCASHBAL;
TYPE XCASHBAL xcashbal;
#define FIELDS_CASHBAL  "id,CBmember,CBacct,CBdate,CBamount,CBcomment"
#endif

#ifdef CRYPTO
#define TABLE_crypto 3
typedef struct
{
	char 	xcticker[21];
	long	xctime;
	double	xcopen;
	double	xchigh;
	double	xclow;
	double	xcclose;
	long	xcvolume;
} XCRYPTO;
TYPE XCRYPTO xcrypto;
#define FIELDS_CRYPTO  "Cticker,Ctime,Copen,Chigh,Clow,Cclose,Cvolume"
#endif

#ifdef DIVIDEND
#define TABLE_dividend 4
typedef struct
{
	unsigned long	xid;
	char 	xdticker[21];
	char 	xdexdate[12];
	double	xdamount;
	char 	xdcurr[5];
} XDIVIDEND;
TYPE XDIVIDEND xdividend;
#define FIELDS_DIVIDEND  "id,Dticker,Dexdate,Damount,Dcurr"
#endif

#ifdef FUNDAMENTAL
#define TABLE_fundamental 5
typedef struct
{
	char 	xfticker[21];
	long	xfebitda;
	long	xfentval;
	double	xfvalue;
	double	xfepsttm;
	double	xfpettm;
	double	xfepsfwd;
	double	xfpefwd;
	double	xfpb;
	double	xfmargin;
	long	xfshares;
	double	xfyield;
	double	xfbeta;
	double	xfgrow5;
	double	xfgrow1;
	double	xftarget;
	double	xfreco;
	double	xfquick;
	double	xfdebteq;
	double	xfreturna;
	long	xffreecash;
	double	xfrevenuegrow;
	double	xfearngrow;
	unsigned long	xfupdated;
	long	xftotasst;
	double	xfavgret3;
	double	xfavgret5;
	long	xfmktcap;
	char 	xfcountry[21];
	char 	xftype[11];
	long	xfvolume;
	char 	xfinception[12];
	double	xfexpratio;
	double	xfdividend;
	double	xfbvps;
	char 	xfframe[9];
	char 	xffactset[2];
	char 	xfmstar[2];
	double	xfequity;
	double	xfminority;
	double	xfdebtlt;
	double	xfdebtnc;
	double	xfliab;
	double	xfpayables;
	double	xfcurasst;
	double	xfcash;
	double	xftreasury;
	double	xffcfgrow;
	double	xffcfcv;
} XFUNDAMENTAL;
TYPE XFUNDAMENTAL xfundamental;
#define FIELDS_FUNDAMENTAL  "Fticker,Febitda,Fentval,Fvalue,Fepsttm,Fpettm,Fepsfwd,Fpefwd,Fpb,Fmargin,Fshares,Fyield,Fbeta,Fgrow5,Fgrow1,Ftarget,Freco,Fquick,Fdebteq,FreturnA,Ffreecash,Frevenuegrow,Fearngrow,Fupdated,Ftotasst,Favgret3,Favgret5,Fmktcap,Fcountry,Ftype,Fvolume,Finception,Fexpratio,Fdividend,Fbvps,Fframe,Ffactset,Fmstar,Fequity,Fminority,FdebtLT,FdebtNC,Fliab,Fpayables,Fcurasst,Fcash,Ftreasury,Ffcfgrow,Ffcfcv"
#endif

#ifdef HISTORY
#define TABLE_history 6
typedef struct
{
	long	xhid;
	char 	xhticker[21];
	char 	xhdate[12];
	double	xhopen;
	double	xhhigh;
	double	xhlow;
	double	xhclose;
	long	xhvolume;
} XHISTORY;
TYPE XHISTORY xhistory;
#define FIELDS_HISTORY  "Hid,Hticker,Hdate,Hopen,Hhigh,Hlow,Hclose,Hvolume"
#endif

#ifdef INDUSTRY
#define TABLE_industry 7
typedef struct
{
	unsigned long	xid;
	char 	xindname[201];
} XINDUSTRY;
TYPE XINDUSTRY xindustry;
#define FIELDS_INDUSTRY  "id,INDname"
#endif

#ifdef MARKET
#define TABLE_market 8
typedef struct
{
	char 	xmktdate[12];
	char 	xmktindex[2];
	short	xmktaccum;
	short	xmktdist;
} XMARKET;
TYPE XMARKET xmarket;
#define FIELDS_MARKET  "MKTdate,MKTindex,MKTaccum,MKTdist"
#endif

#ifdef MEMBER
#define TABLE_member 9
typedef struct
{
	unsigned long	xid;
	char 	xmname[31];
	char 	xmemail[81];
	char 	xmpager[61];
	char 	xmpassword[43];
	char 	xmstatus[2];
	char 	xmrole[2];
	char 	xmipaddr[17];
	char 	xminsdt[12];
	char 	xmlogdt[12];
	char 	xmsuborig[12];
	char 	xmsubstart[12];
	double	xmsubpaid;
	char 	xmsubexpire[12];
	char 	xmmypromo[11];
	char 	xmrefpromo[11];
	double	xmrefpaid;
	long	xmchrtnum;
	long	xmchrtslow;
	long	xmchrtfast;
	double	xmcasht;
	double	xmbondt;
	double	xmdomt;
	double	xmfort;
} XMEMBER;
TYPE XMEMBER xmember;
#define FIELDS_MEMBER  "id,Mname,Memail,Mpager,Mpassword,Mstatus,Mrole,Mipaddr,Minsdt,Mlogdt,Msuborig,Msubstart,Msubpaid,Msubexpire,Mmypromo,Mrefpromo,Mrefpaid,Mchrtnum,Mchrtslow,Mchrtfast,McashT,MbondT,MdomT,MforT"
#endif

#ifdef PAYMENTS
#define TABLE_payments 10
typedef struct
{
	unsigned long	xid;
	unsigned long	xpymember;
	char 	xpytransaction[41];
	char 	xpystatus[2];
	double	xpyamount;
	char 	xpycurrency[7];
	char 	xpyitem[5];
} XPAYMENTS;
TYPE XPAYMENTS xpayments;
#define FIELDS_PAYMENTS  "id,PYmember,PYtransaction,PYstatus,PYamount,PYcurrency,PYitem"
#endif

#ifdef PORTFOLIO
#define TABLE_portfolio 11
typedef struct
{
	unsigned long	xid;
	char 	xpticker[21];
	char 	xpdate[12];
	double	xpshares;
	double	xpprice;
	char 	xpbenchmark[21];
	double	xpalert;
	char 	xpoptexp[12];
	char 	xpbroker[11];
	unsigned long	xpmember;
	double	xptrailpct;
	double	xptrailvalue;
	char 	xpalerttype[2];
	double	xpalertprice;
	unsigned short	xpbucket;
} XPORTFOLIO;
TYPE XPORTFOLIO xportfolio;
#define FIELDS_PORTFOLIO  "id,Pticker,Pdate,Pshares,Pprice,Pbenchmark,Palert,Poptexp,Pbroker,Pmember,Ptrailpct,Ptrailvalue,Palerttype,Palertprice,Pbucket"
#endif

#ifdef PORTHIST
#define TABLE_porthist 12
typedef struct
{
	unsigned long	xphmember;
	char 	xphdate[12];
	double	xphinvest;
	double	xphequity;
	double	xphbonds;
	double	xphcash;
} XPORTHIST;
TYPE XPORTHIST xporthist;
#define FIELDS_PORTHIST  "PHmember,PHdate,PHinvest,PHequity,PHbonds,PHcash"
#endif

#ifdef RPTLOG
#define TABLE_rptlog 13
typedef struct
{
	unsigned long	xid;
	unsigned long	xrmember;
	char 	xrdatetime[21];
	char 	xrcommand[31];
} XRPTLOG;
TYPE XRPTLOG xrptlog;
#define FIELDS_RPTLOG  "id,Rmember,Rdatetime,Rcommand"
#endif

#ifdef SECTOR
#define TABLE_sector 14
typedef struct
{
	unsigned long	xid;
	char 	xsecname[201];
} XSECTOR;
TYPE XSECTOR xsector;
#define FIELDS_SECTOR  "id,SECname"
#endif

#ifdef STOCK
#define TABLE_stock 15
typedef struct
{
	char 	xsticker[21];
	char 	xsname[81];
	char 	xslast[12];
	double	xsclose;
	char 	xstype[2];
	char 	xssp500[2];
	char 	xsrussell[2];
	char 	xsdj[2];
	char 	xsnasdaq[2];
	char 	xscheck[2];
	double	xshigh52;
	char 	xsdate52[12];
	unsigned long	xsindustry;
	char 	xsexchange[7];
	unsigned long	xssector;
	long	xsnews;
	char 	xsdomfor[2];
	char 	xstype2[2];
	char 	xscik[11];
} XSTOCK;
TYPE XSTOCK xstock;
#define FIELDS_STOCK  "Sticker,Sname,Slast,Sclose,Stype,Ssp500,Srussell,Sdj,Snasdaq,Scheck,Shigh52,Sdate52,Sindustry,Sexchange,Ssector,Snews,Sdomfor,Stype2,Scik"
#endif

#ifdef SYSTEM
#define TABLE_system 16
typedef struct
{
	unsigned long	xid;
	long	xydivcnt;
	char 	xydivtime[21];
	long	xystkcnt;
	char 	xystktime[21];
} XSYSTEM;
TYPE XSYSTEM xsystem;
#define FIELDS_SYSTEM  "id,Ydivcnt,Ydivtime,Ystkcnt,Ystktime"
#endif

#ifdef TEMP
#define TABLE_temp 17
typedef struct
{
	unsigned long	xid;
	unsigned long	xtmember;
	char 	xtticker[21];
} XTEMP;
TYPE XTEMP xtemp;
#define FIELDS_TEMP  "id,Tmember,Tticker"
#endif

#ifdef WATCHLIST
#define TABLE_watchlist 18
typedef struct
{
	unsigned long	xid;
	char 	xwticker[21];
	unsigned long	xwmember;
	char 	xwalerttype[2];
	double	xwalertprice;
} XWATCHLIST;
TYPE XWATCHLIST xwatchlist;
#define FIELDS_WATCHLIST  "id,Wticker,Wmember,Walerttype,Walertprice"
#endif

#endif

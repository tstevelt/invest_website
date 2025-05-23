
PROG=invest

LIB = invest_64.a

XLIB = /usr/local/lib64/weblib.a /usr/local/lib64/invlib.a /usr/local/lib64/libta_lib.a /usr/local/lib64/dbylib.a /usr/local/lib64/shslib.a /usr/lib64/libmysqlclient.so  -lcurl -lz -lssl -luuid -lcrypto -lm

DEF = /usr/local/include

WEBDIR = /var/www/vhosts/shs_stocks/html
WEBSTATE = /var/local/webstate/invest
PRG =$(WEBDIR)/$(PROG).cgi

FILES = \
	$(LIB)(invest.o)			\
	$(LIB)(ChkInput.o)		\
	$(LIB)(DeleteStocks.o)	\
	$(LIB)(Forgot.o)		\
	$(LIB)(GetContents.o)	\
	$(LIB)(GetInput.o)		\
	$(LIB)(GetReportCount.o)	\
	$(LIB)(InsertRptlog.o)	\
	$(LIB)(Login.o)			\
	$(LIB)(Logout.o)		\
	$(LIB)(NotifyAdmin.o)	\
	$(LIB)(PaintAccumDistro.o)	\
	$(LIB)(PaintAdmin.o)	\
	$(LIB)(PaintBottom.o)	\
	$(LIB)(PaintChart.o)		\
	$(LIB)(PaintCombinedChart.o)		\
	$(LIB)(PaintContact.o)	\
	$(LIB)(PaintStockDividends.o)	\
	$(LIB)(PaintEditPortfolio.o)	\
	$(LIB)(PaintEditWatchlist.o)	\
	$(LIB)(PaintEditCashbal.o)	\
	$(LIB)(PaintFundamentals.o)	\
	$(LIB)(PaintStatsRaw.o)	\
	$(LIB)(PaintETF_FundamentalScreener.o)	\
	$(LIB)(PaintEquityFundamentalScreener.o)	\
	$(LIB)(PaintHome.o)		\
	$(LIB)(PaintImportWatchlist.o)		\
	$(LIB)(PaintImportPortfolio.o)		\
	$(LIB)(PaintMiscFuncs.o)	\
	$(LIB)(PaintMovingAverages.o)	\
	$(LIB)(PaintMovingAverages.o)	\
	$(LIB)(PaintNews.o)	\
	$(LIB)(PaintPassword.o)	\
	$(LIB)(PaintPortfolioAndWatchlist.o)	\
	$(LIB)(PaintProfile.o)	\
	$(LIB)(PaintReportBottom.o)	\
	$(LIB)(PaintReports.o)	\
	$(LIB)(PaintReportTop.o)	\
	$(LIB)(PaintScreen.o)	\
	$(LIB)(PaintSignup.o)	\
	$(LIB)(PaintSpecial.o)	\
	$(LIB)(PaintStats.o)	\
	$(LIB)(PaintStockMaint.o)	\
	$(LIB)(PaintTableDescription.o)	\
	$(LIB)(PaintTop.o)		\
	$(LIB)(ProcessPayment.o)	\
	$(LIB)(SaveCashBalance.o)		\
	$(LIB)(SaveEmail.o)		\
	$(LIB)(SavePassword.o)			\
	$(LIB)(SavePortfolio.o)	\
	$(LIB)(SaveProfile.o)	\
	$(LIB)(SaveStock.o)	\
	$(LIB)(SaveWatchlist.o)	\
	$(LIB)(SessionFuncs.o)	\
	$(LIB)(Signup.o)		\
	$(LIB)(TakeAction.o)	\
	$(LIB)(Verify.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc $(LDFLAGS) -o $(PRG) $(LIB) $(XLIB)

$(LIB): $(FILES)

$(FILES): invest.h $(DEF)/fileinvest.h $(DEF)/invlib.h $(DEF)/shslib.h $(DEF)/weblib.h $(DEF)/webCookie.h $(DEF)/dbylib.h

js:
	cp -pv invest.js $(WEBDIR)/invest.js	
	cp -pv chart.js  $(WEBDIR)/chart.js	
	cp -pv portfolio.js  $(WEBDIR)/portfolio.js	

css:
	cp -pv invest.css $(WEBDIR)/invest.css	
	cp -pv my_navbar_colors.css $(WEBDIR)/my_navbar_colors.css

contents_live:
	find LIVE -name 'contents.db' -newer LIVE_CONTENTS_STAMP -exec cp -pv {} $(WEBSTATE) \;
	find LIVE -name '*.html' -newer LIVE_CONTENTS_STAMP -exec cp -pv {} $(WEBSTATE) \;
	find LIVE -name '*.pdf'  -newer LIVE_CONTENTS_STAMP -exec cp -pv {} $(WEBDIR) \;
	find LIVE -name '*.png'  -newer LIVE_CONTENTS_STAMP -exec cp -pv {} $(WEBDIR) \;
	touch LIVE_CONTENTS_STAMP

contents_git:
	find GIT -name 'contents.db' -newer GIT_CONTENTS_STAMP -exec cp -pv {} $(WEBSTATE) \;
	find GIT -name '*.html' -newer GIT_CONTENTS_STAMP -exec cp -pv {} $(WEBSTATE) \;
	find GIT -name '*.pdf'  -newer GIT_CONTENTS_STAMP -exec cp -pv {} $(WEBDIR) \;
	find GIT -name '*.png'  -newer GIT_CONTENTS_STAMP -exec cp -pv {} $(WEBDIR) \;
	touch GIT_CONTENTS_STAMP

clean:
	rm -f $(LIB)
	touch -t 200101010800 LIVE_CONTENTS_STAMP
	touch -t 200101010800 GIT_CONTENTS_STAMP

all:
	make clean
	make
	make css
	make js
	# make contents

.PRECIOUS: $(LIB)


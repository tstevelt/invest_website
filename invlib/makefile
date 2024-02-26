
PROG = invlib

ifeq ($(LDFLAGS),"-m32")
LIB = /usr/local/lib/invlib.a
else
LIB = /usr/local/lib64/invlib.a
endif

DEF = /usr/local/include

FILES = \
	$(LIB)(AccumDistrFormula.o)	\
	$(LIB)(AlertDescription.o)	\
	$(LIB)(CalcAccumDistrVolume.o)	\
	$(LIB)(CreateStock.o)	\
	$(LIB)(GetInvestCfg.o)		\
	$(LIB)(GetFundamentalsTiingo.o)		\
	$(LIB)(FundamentalHuman.o)		\
	$(LIB)(IEX_RateLimit.o)		\
	$(LIB)(IndexName.o)		\
	$(LIB)(InPortfolio.o)	\
	$(LIB)(IsBenchmark.o)	\
	$(LIB)(LoadAverageCB.o)	\
	$(LIB)(LoadAverage.o)	\
	$(LIB)(LoadCashbalCB.o)	\
	$(LIB)(LoadCryptoArray.o)	\
	$(LIB)(LoadCryptoArrays.o)	\
	$(LIB)(LoadCryptoCB.o)	\
	$(LIB)(LoadCrypto.o)	\
	$(LIB)(LoadDividendCB.o)	\
	$(LIB)(LoadDividendSum.o)	\
	$(LIB)(LoadFundamentalCB.o)	\
	$(LIB)(LoadFundamental.o)		\
	$(LIB)(LoadHistoryArray.o)	\
	$(LIB)(LoadHistoryArrays.o)	\
	$(LIB)(LoadHistoryCB.o)	\
	$(LIB)(LoadHistory.o)	\
	$(LIB)(LoadIndustryCB.o)	\
	$(LIB)(LoadIndustry.o)	\
	$(LIB)(LoadMarketCB.o)	\
	$(LIB)(LoadMarket.o)	\
	$(LIB)(LoadMemberCB.o)	\
	$(LIB)(LoadMember.o)	\
	$(LIB)(LoadPortfolioCB.o)	\
	$(LIB)(LoadPortfolio.o)	\
	$(LIB)(LoadPorthistCB.o)	\
	$(LIB)(LoadPorthist.o)	\
	$(LIB)(LoadSectorCB.o)	\
	$(LIB)(LoadSector.o)	\
	$(LIB)(LoadStockCB.o)	\
	$(LIB)(LoadStock.o)		\
	$(LIB)(LoadSystem.o)	\
	$(LIB)(LoadTempCB.o)	\
	$(LIB)(LoadTemp.o)	\
	$(LIB)(LoadWatchlistCB.o)	\
	$(LIB)(LoadWatchlist.o)	\
	$(LIB)(ManyDecimalDouble.o)	\
	$(LIB)(OrderEntryBuy.o)	\
	$(LIB)(OrderEntrySell.o)	\
	$(LIB)(PrintErrors.o)	\
	$(LIB)(ReadStockType.o)	\
	$(LIB)(SetMemberLimits.o)	\
	$(LIB)(SetWhereClause.o)	\
	$(LIB)(StartMySQL.o)	\
	$(LIB)(ToTemp.o)	\
	$(LIB)(ToWatchlist.o)	\
	$(LIB)(UsageFuncs.o)


.SILENT:

$(LIB): $(FILES)
	make include

$(FILES): invlib.h invprivate.h fileinvest.h $(DEF)/shslib.h $(DEF)/rptlib.h $(DEF)/dbylib.h $(DEF)/fileinvest.h

include: $(DEF)/invlib.h $(DEF)/fileinvest.h

$(DEF)/invlib.h: invlib.h 
	cp -pv invlib.h $(DEF)/invlib.h

$(DEF)/fileinvest.h: fileinvest.h 
	cp -pv fileinvest.h $(DEF)/fileinvest.h

strip:
	ls -l $(LIB)
	strip --strip-unneeded $(LIB)
	ls -l $(LIB)

clean:
	rm -f $(LIB)
	rm -f $(DEF)/invlib.h $(DEF)/fileinvest.h

all:
	make clean
	make
	make include

.PRECIOUS: $(LIB)


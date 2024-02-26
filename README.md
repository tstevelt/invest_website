## Investment Research and Portfolio Tracking Web Site

This is the first of four repoistories for the Invest website.  The other three will be uploaded soon.
Stay tuned for     | Description
------------------ | ----------------------------------------------------------------
invest_reports     | Reports called by invest.cgi
invest_programs    | Programs called by invest.cgi or run at the command line
invest_extra       | Additional programs for stock market research or db maintenance

The live website is located at <https://stocks.silverhammersoftware.com>

### Features
Anonymous user
    Accumulate/Distribution Ratios - See below - Past 10 days. FREE, no sign-in needed.
    Market Moving Averages - Market Segments Temperature Chart, on the Screeners page. Shows 6 moving averages for 16 benchmarks.

Registered member - FREE
    No credit card required!
    Privacy guaranteed. Email address used only to deliver reports to your inbox.
    We will never share you email address with anyone.
    No ads. Other sites bombard free users with ads. We don't.
    All the same features as a paid user, so you can evaluate our service.
    Limits: 5 securities in portfolio and watchlist.
    Reports: 50 reports past 30 days.

Paid subscriber
    Maximum of 500 securities in portfolio
    Maximum of 500 securities in watchlist
    Portfolio and WatchList Alerts sent daily based on your setup for each stock
    Unlimited referral fees. You can earn more than the cost of a subscription!

### Notes
Contains two directories, a library (invlib) and source code for the website (invest).
To make invest.cgi will also require these libraries:
Library            | Description
------------------ | ----------------------------------------------------------------
weblib.a           | Located at github tstevelt/weblib
dbylib.a           | Located at github tstevelt/dbylib
shslib.a           | Located at github tstevelt/shslib
libta_lib.a        | Technical Analysis library
libmysqlclient.so  | Maria DB library

To use this website you will need a mysql/mariadb database. The schema is located in the invlib directory.
You will also need data. I recommend IEX cloud.




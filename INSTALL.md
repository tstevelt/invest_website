
## Investment Research and Portfolio Installation

In order to build the website on your server, you wiil need some additional libraries.

### Requirements

Library            | Description
------------------ | ----------------------------------------------------------------
<https://github.com/tstevelt/weblib> | Library of HTML, JavaScript and CSS functions
<https://github.com/tstevelt/dbylib> | Library of function to simplify mysql programming.
https://github.com/tstevelt/shslib> | Low level functions for dates, strings, etc. 
<https://github.com/TA-Lib/ta-lib> | Technical Analysis library
libmysqlclient.so  | MariaDB or MySql

### Additional programs

The website calls other programs and also has command line utilities used by the admin to maintain the system.

Other repos        | Description
------------------ | ----------------------------------------------------------------
<a href='https://github.com/tstevelt/invest_programs'>invest_reports</a>     | Reports called by invest.cgi
<a href='https://github.com/tstevelt/invest_reports'>invest_programs</a>    | Programs to load data, update the database, etc.
<a href='https://github.com/tstevelt/invest_extra'>invest_extra</a>     | Additional programs for stock market research or db maintenance.

The live website is located at <https://stocks.silverhammersoftware.com>





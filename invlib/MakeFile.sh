#!/bin/sh
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

mv fileinvest.h fileinvest.h.bak

mysqldump -h 127.0.0.1 -d invest > invest.schema

dbymkfile invest.schema -dates S 

echo "1,\$s/filemysql/fileinvest/g"  > script
echo "1,\$s/FILEMYSQL/FILEINVEST/g" >> script
sedit filemysql.h
rm script

mv filemysql.h fileinvest.h

# cat fileinvest.h

diff /usr/local/include/fileinvest.h fileinvest.h 

cp -pv fileinvest.h /usr/local/include/fileinvest.h



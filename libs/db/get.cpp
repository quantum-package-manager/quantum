#include "get.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

std::string url;

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   
   for(i = 0; i<argc; i++){
	   std::string name = azColName[i];
	   if(name == "URL"){
	      url = argv[i];
	   }
   }
   return 0;
}

int run_sql(std::string pkg, std::string version) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   const char* data = "";

   /* Open database */
   rc = sqlite3_open("quantum.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   }

   /* Create SQL statement */
   std::string sql;
   if(version == "default") {
      sql = "SELECT * FROM PACKAGE\n"
                        "WHERE NAME='";

      sql.append(pkg);
      sql.append("'\nORDER BY URL ASC\nLIMIT 1;");
   } else {
      sql = "SELECT * from PACKAGE "
	   "WHERE NAME='";

      sql.append(pkg);
      sql.append("' AND VERSION='");
      sql.append(version);
      sql.append("'; ");
   }

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }

   sqlite3_close(db);
   return 0;
}

std::string get_url(std::string package, std::string version = "default"){
        run_sql(package, version);
	     return url;
}
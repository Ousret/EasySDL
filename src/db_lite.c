/**
 * \file db_lite.c
 * \brief EasySDL: Database management for esave.c
 * \author TAHRI Ahmed
 * \version 0.1
 * \date 08-02-2015
 *
 * EasySDL est une extension de la librairie SDL standard
 * Utilise SQLite3
 * Fortement inspiré de https://www.sqlite.org/cvstrac/wiki?p=BlobExample
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ESDL.h"

sqlite3 *db = NULL;

/*
** Create the blobs table in database db. Return an SQLite error code.
*/ 
int createBlobTable(sqlite3 *db){
	const char *zSql = "CREATE TABLE sdata(id INTEGER PRIMARY KEY AUTOINCREMENT, param TEXT, value BLOB)";
	return sqlite3_exec(db, zSql, 0, 0, 0);
}

int dropBlobTable(sqlite3 *db){
	const char *zSql = "DROP TABLE IF EXISTS sdata";
	return sqlite3_exec(db, zSql, 0, 0, 0);
}

int db_open(char * filename) {
	
	int rc = sqlite3_open(filename, &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "<! Fatal> EasySDL: Cannot open data file: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 0;
    }else{
    	createBlobTable(db);
    }
    
    return 1;
    
}

void db_close() {
	
	sqlite3_close(db);
    db = NULL;
    
}

/*
** Store a blob in database db. Return an SQLite error code.
**
** This function inserts a new row into the blobs table. The 'key' column
** of the new row is set to the string pointed to by parameter zKey. The
** blob pointed to by zBlob, size nBlob bytes, is stored in the 'value' 
** column of the new row.
*/ 
int writeBlob(
	sqlite3 *db,                   /* Database to insert data into */
	const char *zKey,              /* Null-terminated key string */
	const unsigned char *zBlob,    /* Pointer to blob of data */
	int nBlob                      /* Length of data pointed to by zBlob */
){
  	
  	  
	const char *zSql = "INSERT INTO sdata(param, value) VALUES(?, ?)";
	sqlite3_stmt *pStmt;
	int rc;
	 
  do {
    /* Compile the INSERT statement into a virtual machine. */
    rc = sqlite3_prepare(db, zSql, -1, &pStmt, 0);
      
    if( rc!=SQLITE_OK ){
      return rc;
    }
	  
    /* Bind the key and value data for the new table entry to SQL variables
    ** (the ? characters in the sql statement) in the compiled INSERT 
    ** statement. 
    **
    ** NOTE: variables are numbered from left to right from 1 upwards.
    ** Passing 0 as the second parameter of an sqlite3_bind_XXX() function 
    ** is an error.
    */
    //sqlite3_bind_int(pStmt, 1, (int) sqlite3_last_insert_rowid (db));
    sqlite3_bind_text(pStmt, 1, zKey, -1, SQLITE_STATIC);
    sqlite3_bind_blob(pStmt, 2, zBlob, nBlob, SQLITE_STATIC);
	 
    /* Call sqlite3_step() to run the virtual machine. Since the SQL being
    ** executed is not a SELECT statement, we assume no data will be returned.
    */
    rc = sqlite3_step(pStmt);
    //assert( rc!=SQLITE_ROW );
	 
    /* Finalize the virtual machine. This releases all memory and other
    ** resources allocated by the sqlite3_prepare() call above.
    */
    rc = sqlite3_finalize(pStmt);
	 
    /* If sqlite3_finalize() returned SQLITE_SCHEMA, then try to execute
    ** the statement again.
    */
  } while( rc==SQLITE_SCHEMA );
	 
  return rc;
}

/*
** Read a blob from database db. Return an SQLite error code.
*/ 
int readBlob(
	sqlite3 *db,               /* Database containing blobs table */
	const char *zKey,          /* Null-terminated key to retrieve blob for */
	unsigned char **pzBlob,    /* Set *pzBlob to point to the retrieved blob */
	int *pnBlob                /* Set *pnBlob to the size of the retrieved blob */
){
	 
	const char *zSql = "SELECT value FROM sdata WHERE param = ?";
	sqlite3_stmt *pStmt;
	int rc;
	 
  /* In case there is no table entry for key zKey or an error occurs, 
  ** set *pzBlob and *pnBlob to 0 now.
  */
	 
  
    /* Compile the SELECT statement into a virtual machine. */
    rc = sqlite3_prepare(db, zSql, -1, &pStmt, 0);
     
    if( rc!=SQLITE_OK ){
      return rc;
    }
	 
    /* Bind the key to the SQL variable. */
    sqlite3_bind_text(pStmt, 1, zKey, -1, SQLITE_STATIC);
	 
    /* Run the virtual machine. We can tell by the SQL statement that
    ** at most 1 row will be returned. So call sqlite3_step() once
    ** only. Normally, we would keep calling sqlite3_step until it
    ** returned something other than SQLITE_ROW.
    */
    rc = sqlite3_step(pStmt);
    if( rc==SQLITE_ROW ){
      /* The pointer returned by sqlite3_column_blob() points to memory
      ** that is owned by the statement handle (pStmt). It is only good
      ** until the next call to an sqlite3_XXX() function (e.g. the 
      ** sqlite3_finalize() below) that involves the statement handle. 
      ** So we need to make a copy of the blob into memory obtained from 
      ** malloc() to return to the caller.
      */
       
      *pnBlob = sqlite3_column_bytes(pStmt, 0);
      *pzBlob = (unsigned char *)malloc(*pnBlob);
       
      memcpy(*pzBlob, sqlite3_column_blob(pStmt, 0), *pnBlob);
       
    }

    /* Finalize the statement (this releases resources allocated by 
    ** sqlite3_prepare() ).
    */
     
    sqlite3_finalize(pStmt);

    /* If sqlite3_finalize() returned SQLITE_SCHEMA, then try to execute
    ** the statement all over again.
    */
  
	 
  return rc;
}

/*
** Read a blob from database db. Return an SQLite error code.
*/ 
int readText(
	sqlite3 *db,               /* Database containing blobs table */
	int zID,          /* Null-terminated key to retrieve blob for */
	unsigned char **pzBlob,    /* Set *pzBlob to point to the retrieved blob */
	int *pnBlob                /* Set *pnBlob to the size of the retrieved blob */
){
	
	const char *zSql = "SELECT param FROM sdata WHERE id = ?";
	 
	sqlite3_stmt *pStmt;
	 
	int rc;

  /* In case there is no table entry for key zKey or an error occurs, 
  ** set *pzBlob and *pnBlob to 0 now.
  */

    /* Compile the SELECT statement into a virtual machine. */
    rc = sqlite3_prepare(db, zSql, -1, &pStmt, 0);
     
    if( rc!=SQLITE_OK ){
      return rc;
    }
	 
    /* Bind the key to the SQL variable. */
    sqlite3_bind_int(pStmt, 1, zID);
	 
    /* Run the virtual machine. We can tell by the SQL statement that
    ** at most 1 row will be returned. So call sqlite3_step() once
    ** only. Normally, we would keep calling sqlite3_step until it
    ** returned something other than SQLITE_ROW.
    */
    rc = sqlite3_step(pStmt);
    if( rc==SQLITE_ROW ){
      /* The pointer returned by sqlite3_column_blob() points to memory
      ** that is owned by the statement handle (pStmt). It is only good
      ** until the next call to an sqlite3_XXX() function (e.g. the 
      ** sqlite3_finalize() below) that involves the statement handle. 
      ** So we need to make a copy of the blob into memory obtained from 
      ** malloc() to return to the caller.
      */
      *pnBlob = sqlite3_column_bytes(pStmt, 0);
      *pzBlob = malloc(sizeof(unsigned char)*(*pnBlob));
       
      //memcpy(*pzBlob, sqlite3_column_text(pStmt, 0), *pnBlob);
      unsignedchar_memcpy(*pzBlob, (unsigned char *) sqlite3_column_text(pStmt, 0), *pnBlob);
      
    }

    /* Finalize the statement (this releases resources allocated by 
    ** sqlite3_prepare() ).
    */
    sqlite3_finalize(pStmt);
     
 
	 
  return rc;
}

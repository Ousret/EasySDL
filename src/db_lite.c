/*
*	Database management with SQLite3
*	Part of EasySDL
*	Written by TAHRI Ahmed @Ousret
*	tahri.spitajoke.com
*	Alpha..
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

//#include "db_lite.h"

sqlite3 *db = NULL;
sqlite3_stmt *res = NULL;
char *err_msg = NULL;

int db_open(char * filename) {
	
	int rc = sqlite3_open(filename, &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "<! Fatal> EasySDL: Cannot open data file: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 0;
    }
    
    /* Checking if table SDATA exist.. */
    char *sql = "SELECT count(type) FROM sqlite_master WHERE type='table' AND name='SDATA';";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    int step = sqlite3_step(res);
    
    if (step != SQLITE_ROW) {
        
        sql = "CREATE TABLE SDATA(Id INTEGER PRIMARY KEY, Param TEXT, Value TEXT);";
        rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    	if (rc != SQLITE_OK ) {
    		return 0;
    	}   
    	
    	sqlite3_step(res);
    	
    } 
    
    return 1;
    
}

unsigned char * db_getkeyvalue(unsigned char * key) {
	
	if (!db || !key) return NULL;
	int rc = 0;
	unsigned char *qresult = NULL;
	
	/* We have to check if something is already there.. */
	char *sql = "SELECT Value FROM SDATA WHERE Param = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc != SQLITE_OK) return NULL;
    sqlite3_bind_text(res, 1, (char*) key, strlen((char*) key)+1, SQLITE_STATIC);
    
    int step = sqlite3_step(res);
    
    if (step == SQLITE_ROW) {
        qresult = malloc(sizeof(unsigned char)*strlen((char*) sqlite3_column_text(res, 0)));
        strcpy((char*) qresult, (char*) sqlite3_column_text(res, 0));
        sqlite3_finalize(res);
        
        return qresult;
    } 
    
    return NULL;
    
}

int db_setkeyvalue(char *key, char *value) {
	
	int rc = 0;
	/* We have to check if something is already there.. */
	char *sql = "SELECT Value FROM SDATA WHERE Param = ?";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) return 0;
    
    sqlite3_bind_text(res, 1, key, strlen(key)+1, SQLITE_STATIC);
    int step = sqlite3_step(res);
    
    if (step == SQLITE_ROW) {
        
        /*printf("%s: ", sqlite3_column_text(res, 0));
        printf("%s\n", sqlite3_column_text(res, 1));*/
        
    }else{
    	
    	//Set new line..
    	sql = "INSERT INTO SDATA (Id, Param, Value) VALUES (Id+1, '?', '?');";
    	rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    	if (rc != SQLITE_OK) return 0;
    	
    	sqlite3_bind_text(res, 1, key, strlen(key)+1, SQLITE_STATIC);
    	sqlite3_bind_text(res, 2, value, strlen(key)+1, SQLITE_STATIC);
    	
    	sqlite3_step(res);
    	
    } 

    sqlite3_finalize(res);
	
	return 1;
}

void db_close() {
	
	sqlite3_close(db);
    db = NULL;
    
}
#include <stdio.h>
#include <sqlite3.h>

int main(int argc, char **argv){
	sqlite3 *db; //the database
	sqlite3_stmt    *stmt_updt; //the update statement

  	int rc;

  	if( argc!=3 ){
    	fprintf(stderr, "Usage: %s <database file> <update query>\n", argv[0]);
    	return(1);
  	}

  	rc = sqlite3_open(argv[1], &db);
  	if( rc ){
    	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    	sqlite3_close(db);
    	return(1);
  	}

	char *sql_updt = argv[2];

	rc = sqlite3_prepare_v2(db, sql_updt, -1, &stmt_updt, 0);
	if (rc != SQLITE_OK) {  
        fprintf(stderr, "Preparation failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    if ((rc = sqlite3_step(stmt_updt)) != SQLITE_DONE){
    	fprintf(stderr, "Update failed: %s\n", sqlite3_errmsg(db));
    	sqlite3_close(db);
    	return 1;    	
	}

    sqlite3_finalize(stmt_updt); //always finalize a statement
}
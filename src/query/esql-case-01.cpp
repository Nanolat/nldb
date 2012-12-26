/*
 * (C) Copyright 2012 ThankyouSoft (http://ThankyouSoft.com/) and others.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Contributors:
 *     Kangmo Kim
 *     Saryong Kang
 */

#include <nldb/nldb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <txbase/tx_assert.h>
#include <arpa/inet.h>
/* EXEC SQL BEGIN DECLARE SECTION */

/*
 DECLARE TABLE dept
 (
 dept_no   int NOT NULL PRIMARY KEY,
 name      char(32) NOT NULL,
 address   char(64) NOT NULL,
 );
 */
#define ESQL_table_id_dept (1)
#define ESQL_not_null_dept__dept_no (1)
#define ESQL_not_null_dept__name    (1)
#define ESQL_not_null_dept__address (1)

typedef struct dept {
	typedef struct key_t {
		int dept_no;
	} key_t;
	key_t key;

	typedef struct value_t {
		char name[32];
		char address[64];
	} value_t;
	value_t value;
} dept;

static nldb_table_t esql_table_dept;

/*
 DECLARE TABLE emp
 (
 emp_no    int NOT NULL PRIMARY KEY,
 dept_no   int NOT NULL,
 name      char(16) NOT NULL,
 age       int
 );
 */

#define ESQL_table_id_emp (2)
#define ESQL_not_null_emp__emp_no  (1)
#define ESQL_not_null_emp__dept_no (1)
#define ESQL_not_null_emp__name    (1)

typedef struct emp {
	typedef struct key_t {
		int emp_no;
	} key_t;

	key_t key;

	typedef struct value_t {
		int dept_no;
		char name[16];
		int age;
	} value_t;

	value_t value;
} emp;

static nldb_table_t esql_table_emp;

/*
 DECLARE TABLE dept_emp
 (
 dept_no   int NOT NULL,
 emp_no    int NOT NULL,
 join_year int,
 CONSTRAINT pk_dept_emp PRIMARY KEY (dept_no, emp_no)
 );
 */

#define ESQL_table_id_dept_emp (3)
#define ESQL_not_null_dept_emp__dept_no  (1)
#define ESQL_not_null_dept_emp__emp_no (1)

typedef struct dept_emp {
	typedef struct key_t {
		int dept_no;
		int emp_no;
	} key_t;
	key_t key;

	typedef struct value_t {
		int join_year;
	} value_t;
	value_t value;

} dept_emp;

static nldb_table_t esql_table_dept_emp;

static nldb_db_t db;

static nldb_tx_t tx;

static struct SQLCA {
	nldb_rc_t SQLCODE;
} SQLCA;
/* EXEC SQL END DECLARE SECTION */

#define CHECK_ERROR(code)  \
		if (code != NLDB_OK) { \
			printf("Fatal Error : %d, in File %s at line %d\n", code, __FILE__, __LINE__ ); \
			exit(-1); \
		}

void open_db_as_master() {
	/*
	 EXEC SQL OPEN DATABASE 1 AS MASTER { MASTER_IP='127.0.0.1', MASTER_PORT=7604 };
	 */
	SQLCA.SQLCODE = nldb_init();
	if (SQLCA.SQLCODE == NLDB_OK) {
		nldb_replication_master_op_t master_op = { "127.0.0.1", 7604 };
		SQLCA.SQLCODE = nldb_db_open(1, &master_op, NULL, &db);
	}

	CHECK_ERROR(SQLCA.SQLCODE);
}

void open_db_as_slave() {
	/*
	 EXEC SQL OPEN DATABASE 1 AS SLAVE { MASTER_IP='76.4.3.1', MASTER_PORT=7604 };
	 */
	SQLCA.SQLCODE = nldb_init();
	if (SQLCA.SQLCODE == NLDB_OK) {
		nldb_replication_slave_op_t slave_op = { "76.4.3.1", 7604 };
		SQLCA.SQLCODE = nldb_db_open(1, NULL, &slave_op, &db);
	}

	CHECK_ERROR(SQLCA.SQLCODE);
}

void open_db_as_slave_and_master() {
	/*
	 EXEC SQL OPEN DATABASE 1 AS SLAVE  { MASTER_IP='76.4.3.1', MASTER_PORT=7604 }
	 AS MASTER { MASTER_IP='127.0.0.1', MASTER_PORT=622 };
	 */
	SQLCA.SQLCODE = nldb_init();
	if (SQLCA.SQLCODE == NLDB_OK) {
		nldb_replication_slave_op_t slave_op = { "76.4.3.1", 7604 };
		nldb_replication_master_op_t master_op = { "127.0.0.1", 622 };
		SQLCA.SQLCODE = nldb_db_open(1, &master_op, &slave_op, &db);
	}

	CHECK_ERROR(SQLCA.SQLCODE);
}

void open_db(char * db_open_option) {
	if (strcmp(db_open_option, "master") == 0) {
		open_db_as_master();
	} else if (strcmp(db_open_option, "slave") == 0) {
		open_db_as_slave();
	} else if (strcmp(db_open_option, "slave-master") == 0) {
		open_db_as_slave_and_master();
	} else {
		printf(
				"The database open option should be \"master\", \"slave\", or \"slave-master\". ");
		exit(-1);
	}
}

void close_db() {
	/*
	 EXEC SQL CLOSE DATABASE;
	 */
	SQLCA.SQLCODE = nldb_db_close(db);
	if (SQLCA.SQLCODE == NLDB_OK) {
		SQLCA.SQLCODE = nldb_destroy();
	}

	CHECK_ERROR(SQLCA.SQLCODE);
}

void open_tables() {
	/*
	 EXEC SQL OPEN TABLE dept;
	 */
	SQLCA.SQLCODE = nldb_table_open(db, ESQL_table_id_dept, &esql_table_dept);

	CHECK_ERROR(SQLCA.SQLCODE);

	/*
	 EXEC SQL OPEN TABLE emp;
	 */
	SQLCA.SQLCODE = nldb_table_open(db, ESQL_table_id_emp, &esql_table_emp);

	CHECK_ERROR(SQLCA.SQLCODE);

	/*
	 EXEC SQL OPEN TABLE dept_emp;
	 */
	SQLCA.SQLCODE = nldb_table_open(db, ESQL_table_id_dept_emp,
			&esql_table_dept_emp);

	CHECK_ERROR(SQLCA.SQLCODE);
}

void close_tables() {
	/*
	 EXEC SQL CLOSE TABLE dept;
	 */
	SQLCA.SQLCODE = nldb_table_close(esql_table_dept);

	CHECK_ERROR(SQLCA.SQLCODE);

	/*
	 EXEC SQL CLOSE TABLE emp;
	 */
	SQLCA.SQLCODE = nldb_table_close(esql_table_emp);

	CHECK_ERROR(SQLCA.SQLCODE);

	/*
	 EXEC SQL CLOSE TABLE dept_emp;
	 */
	SQLCA.SQLCODE = nldb_table_close(esql_table_dept_emp);

	CHECK_ERROR(SQLCA.SQLCODE);
}

void create_tables() {
	/*
	 EXEC SQL CREATE TABLE dept { PLUGIN = NLDB_TABLE_PERSISTENT };
	 */
	SQLCA.SQLCODE = nldb_table_create(db, ESQL_table_id_dept,
			NLDB_TABLE_PERSISTENT);

	CHECK_ERROR(SQLCA.SQLCODE);

	/*
	 EXEC SQL CREATE TABLE emp { PLUGIN = NLDB_TABLE_PERSISTENT };
	 */
	SQLCA.SQLCODE = nldb_table_create(db, ESQL_table_id_emp,
			NLDB_TABLE_PERSISTENT);

	CHECK_ERROR(SQLCA.SQLCODE);

	/*
	 EXEC SQL CREATE TABLE dept_emp { PLUGIN = NLDB_TABLE_VOLATILE };
	 */
	SQLCA.SQLCODE = nldb_table_create(db, ESQL_table_id_dept_emp,
			NLDB_TABLE_PERSISTENT);

	CHECK_ERROR(SQLCA.SQLCODE);
}

void drop_tables() {
	/*
	 EXEC SQL DROP TABLE dept;
	 */
	SQLCA.SQLCODE = nldb_table_drop(db, ESQL_table_id_dept);

	CHECK_ERROR(SQLCA.SQLCODE);

	/*
	 EXEC SQL DROP TABLE emp;
	 */
	SQLCA.SQLCODE = nldb_table_drop(db, ESQL_table_id_emp);

	CHECK_ERROR(SQLCA.SQLCODE);

	/*
	 EXEC SQL DROP TABLE dept_emp;
	 */
	SQLCA.SQLCODE = nldb_table_drop(db, ESQL_table_id_dept_emp);

	CHECK_ERROR(SQLCA.SQLCODE);
}

void create_db() {
	/*
	 EXEC SQL CREATE DATABASE { ID = 1 };
	 */
	SQLCA.SQLCODE = nldb_destroy();
	if (SQLCA.SQLCODE == NLDB_OK) {
		SQLCA.SQLCODE = nldb_db_create(1);
	}

	CHECK_ERROR(SQLCA.SQLCODE);
}

void drop_db() {
	/*
	 EXEC SQL DROP DATABASE { ID = 1 };
	 */
	SQLCA.SQLCODE = nldb_db_drop(1);
	if (SQLCA.SQLCODE == NLDB_OK) {
		SQLCA.SQLCODE = nldb_destroy();
	}

	CHECK_ERROR(SQLCA.SQLCODE);
}

#define HQ_DEPT_NO  (1)
#define DEV_DEPT_NO (2)
#define MKT_DEPT_NO (3)

void insert_into_dept() {
	dept dept_hq = { { HQ_DEPT_NO }, { "HQ", "Youido, Seoul, Korea" } };
	/*
	 EXEC SQL INSERT INTO dept VALUES :dept_hq ;
	 */
	{
		dept::key_t __key;
		// NLDB requires each key to be encoded with big endian.
		__key.dept_no = htonl(dept_hq.key.dept_no);
		nldb_key_t nldb_key = { &__key, sizeof(__key) };
		nldb_value_t nldb_value = { &dept_hq.value, sizeof(dept_hq.value) };
		SQLCA.SQLCODE = nldb_table_put(tx, esql_table_dept, nldb_key,
				nldb_value);
	}
	CHECK_ERROR(SQLCA.SQLCODE);

	dept dept_dev = { { DEV_DEPT_NO }, { "Development", "Jeju Island, Korea" } };
	/*
	 EXEC SQL INSERT INTO dept VALUES :dept_development ;
	 */

	dept dept_mkt =
	{ { MKT_DEPT_NO }, { "Marketing", "Santa Clara, CA, U.S." } };
	/*
	 EXEC SQL INSERT INTO dept VALUES :dept_marketing ;
	 */

}

void insert_into_emp() {
	emp emp_kangmo = { { 1 }, { DEV_DEPT_NO, "Kangmo Kim", 20 } };
	/*
	 EXEC SQL INSERT INTO emp VALUES :emp_kangmo ;
	 */

	emp emp_saryong = { { 2 }, { HQ_DEPT_NO, "Saryong Kang", 22 } };
	/*
	 EXEC SQL INSERT INTO emp VALUES :emp_saryong ;
	 */

	emp emp_jinwoo = { { 3 }, { DEV_DEPT_NO, "Jinwoo Lee", 25 } };
	/*
	 EXEC SQL INSERT INTO emp VALUES :emp_jinwoo ;
	 */

	emp emp_kyunga = { { 4 }, { MKT_DEPT_NO, "Kyunga Lee", 24 } };
	/*
	 EXEC SQL INSERT INTO emp VALUES :emp_kyunga ;
	 */

	emp emp_zombie = { { 5 }, { DEV_DEPT_NO, "Zombie Jackson", 25 } };
	/*
	 EXEC SQL INSERT INTO emp VALUES :emp_zombie ;
	 */
}

void insert_into_dept_emp() {
	/*
	 EXEC SQL INSERT INTO dept_emp
	 SELECT d.dept_no, e.emp_no, 2010
	 FROM emp e INNER JOIN dept d ON e.dept_no = d.dept_no ;
	 */
	// generated code running nested loop join
	{
		nldb_cursor_t cursor_emp;
		nldb_cursor_t cursor_dept;
		nldb_rc_t rc;
		rc = nldb_cursor_open(tx, esql_table_emp, &cursor_emp);
		if (rc == NLDB_OK) {
			rc = nldb_cursor_open(tx, esql_table_dept, &cursor_dept);
			if (rc != NLDB_OK) {
				SQLCA.SQLCODE = rc;
				goto close_cursor_emp;
			}
			{
				// data=NULL, length=0 means to search from the minimum key
				nldb_key_t emp_min_key = { NULL, 0 };
				rc = nldb_cursor_seek_forward(cursor_emp, emp_min_key);
				if (rc != NLDB_OK) {
					SQLCA.SQLCODE = rc;
					goto close_cursor_dept;
				}
				while (1) {
					nldb_key_t nldb_emp_key;
					nldb_value_t nldb_emp_value;
					rc = nldb_cursor_move_forward(cursor_emp, &nldb_emp_key,
							&nldb_emp_value);
					if (rc == NLDB_ERROR_END_OF_ITERATION) {
						break;
					}
					emp::key_t * emp_key = (emp::key_t *) nldb_emp_key.data;
					emp::value_t * emp_value =
							(emp::value_t *) nldb_emp_value.data;
					tx_assert( emp_key);
					tx_assert( emp_value);
					tx_assert( sizeof(*emp_key) == nldb_emp_key.length);
					tx_assert( sizeof(*emp_value) == nldb_emp_value.length);

					while (1) {
						nldb_key_t dept_min_key = { NULL, 0 };
						rc = nldb_cursor_seek_forward(cursor_dept,
								dept_min_key);
						if (rc != NLDB_OK) {
							SQLCA.SQLCODE = rc;
							goto close_cursor_dept;
						}

						nldb_key_t nldb_dept_key;
						nldb_value_t nldb_dept_value;
						rc = nldb_cursor_move_forward(cursor_dept,
								&nldb_dept_key, &nldb_dept_value);
						if (rc == NLDB_ERROR_END_OF_ITERATION) {
							break;
						}

						struct dept::key_t * dept_key =
								(dept::key_t *) nldb_dept_key.data;
						struct dept::value_t * dept_value =
								(dept::value_t *) nldb_dept_value.data;

						tx_assert( dept_key);
						tx_assert( dept_value);
						tx_assert( sizeof(*dept_key) == nldb_dept_key.length);
						tx_assert(
								sizeof(*dept_value) == nldb_dept_value.length);

						if (emp_value->dept_no == dept_key->dept_no) {

							struct dept_emp dept_emp_record = { {
									dept_key->dept_no, emp_key->emp_no },
									{ 2010 } };
							// The same code generated by EXEC SQL INSERT INTO dept VALUES :dept_emp_record ;
							{
								// NLDB requires each key to be encoded with big endian.
								// However, both dept_key->dept_no, emp_key->emp_no are already in big endian, as they are returned from the nldb interface.
								// So, no need to convert these keys to big endian.
								nldb_key_t nldb_key = { &dept_emp_record.key,
										sizeof(dept_emp_record.key) };
								nldb_value_t nldb_value = {
										&dept_emp_record.value,
										sizeof(dept_emp_record.value) };
								SQLCA.SQLCODE = nldb_table_put(tx,
										esql_table_dept, nldb_key, nldb_value);
							}
							if (SQLCA.SQLCODE != NLDB_OK) {
								goto close_cursor_dept;
							}
						}
					}
				}
			}
			close_cursor_dept: rc = nldb_cursor_close(cursor_dept);
			if (rc != NLDB_OK)
				SQLCA.SQLCODE = rc;
		}
		close_cursor_emp: rc = nldb_cursor_close(cursor_emp);
		if (rc != NLDB_OK)
			SQLCA.SQLCODE = rc;
	}

	// generated code for nested index join
	{
		nldb_cursor_t cursor_emp;
		nldb_rc_t rc;
		rc = nldb_cursor_open(tx, esql_table_emp, &cursor_emp);
		if (rc == NLDB_OK) {
			// data=NULL, length=0 means to search from the minimum key
			nldb_key_t emp_min_key = { NULL, 0 };
			rc = nldb_cursor_seek_forward(cursor_emp, emp_min_key);
			if (rc != NLDB_OK) {
				SQLCA.SQLCODE = rc;
				goto close_cursor_emp2;
			}
			while (1) {
				nldb_key_t nldb_emp_key;
				nldb_value_t nldb_emp_value;
				rc = nldb_cursor_move_forward(cursor_emp, &nldb_emp_key,
						&nldb_emp_value);
				if (rc == NLDB_ERROR_END_OF_ITERATION) {
					break;
				}
				struct emp::key_t * emp_key = (emp::key_t *) nldb_emp_key.data;
				struct emp::value_t * emp_value =
						(emp::value_t *) nldb_emp_value.data;
				tx_assert( emp_key);
				tx_assert( emp_value);
				tx_assert( sizeof(*emp_key) == nldb_emp_key.length);
				tx_assert( sizeof(*emp_value) == nldb_emp_value.length);

				// Get the matching dept
				{
					struct dept::key_t dept_key;
					dept_key.dept_no = htonl(emp_value->dept_no);

					nldb_key_t nldb_dept_key = { &dept_key, sizeof(dept_key) };
					nldb_value_t nldb_dept_value;
					rc = nldb_table_get(tx, esql_table_dept, nldb_dept_key,
							&nldb_dept_value);
					if (rc == NLDB_ERROR_KEY_NOT_FOUND) {
						continue;
					}

					struct dept_emp dept_emp_record = { { dept_key.dept_no,
							emp_key->emp_no }, { 2010 } };
					// The same code generated by EXEC SQL INSERT INTO dept VALUES :dept_emp_record ;
					{
						// NLDB requires each key to be encoded with big endian.
						// However, both dept_key->dept_no, emp_key->emp_no are already in big endian, as they are returned from the nldb interface.
						// So, no need to convert these keys to big endian.
						nldb_key_t nldb_key = { &dept_emp_record.key,
								sizeof(dept_emp_record.key) };
						nldb_value_t nldb_value = { &dept_emp_record.value,
								sizeof(dept_emp_record.value) };
						SQLCA.SQLCODE = nldb_table_put(tx, esql_table_dept,
								nldb_key, nldb_value);
					}
					if (SQLCA.SQLCODE != NLDB_OK) {
						goto close_cursor_emp2;
					}
				}
			}
		}
		close_cursor_emp2: rc = nldb_cursor_close(cursor_emp);
		if (rc != NLDB_OK)
			SQLCA.SQLCODE = rc;
	}
}

void update() {
	/*
	 EXEC SQL UPDATE emp
	 SET age = age + 1
	 WHERE name <> 'Kangmo Kim' ;
	 */
	{
		nldb_cursor_t cursor_emp;
		nldb_rc_t rc;
		rc = nldb_cursor_open(tx, esql_table_emp, &cursor_emp);
		if (rc == NLDB_OK) {
			nldb_key_t emp_min_key = { NULL, 0 };
			rc = nldb_cursor_seek_forward(cursor_emp, emp_min_key);
			if (rc != NLDB_OK) {
				SQLCA.SQLCODE = rc;
				goto close_cursor_emp;
			}
			while (1) {
				nldb_key_t nldb_emp_key;
				nldb_value_t nldb_emp_value;
				rc = nldb_cursor_move_forward(cursor_emp, &nldb_emp_key,
						&nldb_emp_value);
				if (rc == NLDB_ERROR_END_OF_ITERATION) {
					break;
				}
				struct emp::key_t * emp_key = (emp::key_t *) nldb_emp_key.data;
				struct emp::value_t * emp_value =
						(emp::value_t *) nldb_emp_value.data;
				tx_assert( emp_key);
				tx_assert( emp_value);
				tx_assert( sizeof(*emp_key) == nldb_emp_key.length);
				tx_assert( sizeof(*emp_value) == nldb_emp_value.length);

				if (strcmp(emp_value->name, "Kangmo Kim") != 0) {

					emp_value->age = emp_value->age + 1;

					{
						// NLDB requires each key to be encoded with big endian.
						// However, emp_key->emp_no is already in big endian, as they are returned from the nldb interface.
						// So, no need to convert these keys to big endian.

						nldb_key_t nldb_emp_key = { emp_key, sizeof(*emp_key) };
						nldb_value_t nldb_emp_value = { emp_value,
								sizeof(*emp_value) };

						SQLCA.SQLCODE = nldb_table_put(tx, esql_table_emp,
								nldb_emp_key, nldb_emp_value);

						if (SQLCA.SQLCODE != NLDB_OK) {
							goto close_cursor_emp;
						}
					}
				}
			}
		}

		close_cursor_emp: rc = nldb_cursor_close(cursor_emp);
		if (rc != NLDB_OK)
			SQLCA.SQLCODE = rc;
	}
	CHECK_ERROR(SQLCA.SQLCODE);

	const char * old_hq_name = "HQ";
	const char * new_hq_name = "Headquarter";

	/*
	 EXEC SQL UPDATE dept
	 SET name = :new_hq_name
	 WHERE name = :old_hq_name ;
	 */
	{
		nldb_cursor_t cursor_emp;
		nldb_rc_t rc;
		rc = nldb_cursor_open(tx, esql_table_emp, &cursor_emp);
		if (rc == NLDB_OK) {
			nldb_key_t emp_min_key = { NULL, 0 };
			rc = nldb_cursor_seek_forward(cursor_emp, emp_min_key);
			if (rc != NLDB_OK) {
				SQLCA.SQLCODE = rc;
				goto close_cursor_emp2;
			}
			while (1) {
				nldb_key_t nldb_emp_key;
				nldb_value_t nldb_emp_value;
				rc = nldb_cursor_move_forward(cursor_emp, &nldb_emp_key,
						&nldb_emp_value);
				if (rc == NLDB_ERROR_END_OF_ITERATION) {
					break;
				}
				struct emp::key_t * emp_key = (emp::key_t *) nldb_emp_key.data;
				struct emp::value_t * emp_value =
						(emp::value_t *) nldb_emp_value.data;
				tx_assert( emp_key);
				tx_assert( emp_value);
				tx_assert( sizeof(*emp_key) == nldb_emp_key.length);
				tx_assert( sizeof(*emp_value) == nldb_emp_value.length);

				// BUGBUG need to use memcmp because emp_value->name is not '\0' terminated.
				if (strcmp(emp_value->name, old_hq_name) == 0) {
					// BUGBUG make sure 'name' buffer does not overflow.
					// BUGBUG need to append space padding in case new_hq_name is less than the size of 'name' field.
					memcmp(emp_value->name, new_hq_name,
							sizeof(emp_value->name));

					{
						// NLDB requires each key to be encoded with big endian.
						// However, emp_key->emp_no is already in big endian, as they are returned from the nldb interface.
						// So, no need to convert these keys to big endian.

						nldb_key_t nldb_emp_key = { emp_key, sizeof(*emp_key) };
						nldb_value_t nldb_emp_value = { emp_value,
								sizeof(*emp_value) };

						SQLCA.SQLCODE = nldb_table_put(tx, esql_table_emp,
								nldb_emp_key, nldb_emp_value);

						if (SQLCA.SQLCODE != NLDB_OK) {
							goto close_cursor_emp2;
						}
					}
				}
			}
		}

		close_cursor_emp2: rc = nldb_cursor_close(cursor_emp);
		if (rc != NLDB_OK)
			SQLCA.SQLCODE = rc;
	}
	CHECK_ERROR(SQLCA.SQLCODE);

}

void select() {
	/*
	 EXEC SQL SELECT *
	 FROM emp
	 WHERE emp_no >= 2
	 FOR EACH RESULT (e)
	 {
	 printf("==================\n");
	 printf("emp_no : %d\n",  e.emp_no);
	 printf("dept_no : %d\n", e.dept_no);
	 printf("name : %s\n",    e.name);
	 printf("age : %d\n",     e.age);

	 if (e.emp_no > 4)
	 break;
	 };
	 */

	{
		nldb_cursor_t cursor_emp;
		nldb_rc_t rc;
		rc = nldb_cursor_open(tx, esql_table_emp, &cursor_emp);
		if (rc == NLDB_OK) {
			struct emp::key_t __key;
			__key.emp_no = htonl(2);
			nldb_key_t nldb_emp_start_key = { &__key, sizeof(__key) };

			rc = nldb_cursor_seek_forward(cursor_emp, nldb_emp_start_key);
			if (rc != NLDB_OK) {
				SQLCA.SQLCODE = rc;
				goto close_cursor_emp;
			}
			while (1) {
				nldb_key_t nldb_emp_key;
				nldb_value_t nldb_emp_value;
				rc = nldb_cursor_move_forward(cursor_emp, &nldb_emp_key,
						&nldb_emp_value);
				if (rc == NLDB_ERROR_END_OF_ITERATION) {
					break;
				}
				struct emp::key_t * emp_key = (emp::key_t *) nldb_emp_key.data;
				struct emp::value_t * emp_value =
						(emp::value_t *) nldb_emp_value.data;
				tx_assert( emp_key);
				tx_assert( emp_value);
				tx_assert( sizeof(*emp_key) == nldb_emp_key.length);
				tx_assert( sizeof(*emp_value) == nldb_emp_value.length);

				// The code snippet following the FOR EACH RESULT clause
				{
					printf("==================\n");
					printf("emp_no : %d\n", emp_key->emp_no);
					printf("dept_no : %d\n", emp_value->dept_no);
					printf("name : %s\n", emp_value->name);
					printf("age : %d\n", emp_value->age);

					if (emp_key->emp_no > 4)
						break;
				}
			}
		}

		close_cursor_emp: rc = nldb_cursor_close(cursor_emp);
		if (rc != NLDB_OK)
			SQLCA.SQLCODE = rc;
	}
	CHECK_ERROR(SQLCA.SQLCODE);

}

void delete_from_emp() {
	/*
	 EXEC SQL DELETE FROM emp
	 WHERE emp_no = 1 ;
	 */
	{
		struct emp::key_t __key;
		__key.emp_no = htonl(1);

		nldb_key_t nldb_emp_key = { &__key, sizeof(__key) };
		SQLCA.SQLCODE = nldb_table_del(tx, esql_table_emp, nldb_emp_key);
	}
	CHECK_ERROR(SQLCA.SQLCODE);

	/*
	 EXEC SQL DELETE FROM emp
	 WHERE name = 'Zombie Jackson' ;
	 */
	{
		nldb_cursor_t cursor_emp;
		nldb_rc_t rc;
		rc = nldb_cursor_open(tx, esql_table_emp, &cursor_emp);
		if (rc == NLDB_OK) {
			nldb_key_t emp_min_key = { NULL, 0 };
			rc = nldb_cursor_seek_forward(cursor_emp, emp_min_key);
			if (rc != NLDB_OK) {
				SQLCA.SQLCODE = rc;
				goto close_cursor_emp;
			}
			while (1) {
				nldb_key_t nldb_emp_key;
				nldb_value_t nldb_emp_value;
				rc = nldb_cursor_move_forward(cursor_emp, &nldb_emp_key,
						&nldb_emp_value);
				if (rc == NLDB_ERROR_END_OF_ITERATION) {
					break;
				}
				struct emp::key_t * emp_key = (emp::key_t *) nldb_emp_key.data;
				struct emp::value_t * emp_value =
						(emp::value_t *) nldb_emp_value.data;
				tx_assert( emp_key);
				tx_assert( emp_value);
				tx_assert( sizeof(*emp_key) == nldb_emp_key.length);
				tx_assert( sizeof(*emp_value) == nldb_emp_value.length);

				if (strcmp(emp_value->name, "Zombie Jackson") == 0) {

					{
						// NLDB requires each key to be encoded with big endian.
						// However, emp_key->emp_no is already in big endian, as they are returned from the nldb interface.
						// So, no need to convert these keys to big endian.

						nldb_key_t nldb_emp_key = { emp_key, sizeof(*emp_key) };
						SQLCA.SQLCODE = nldb_table_del(tx, esql_table_emp,
								nldb_emp_key);

						if (SQLCA.SQLCODE != NLDB_OK) {
							goto close_cursor_emp;
						}
					}
					break;
				}
			}
		}

		close_cursor_emp: rc = nldb_cursor_close(cursor_emp);
		if (rc != NLDB_OK)
			SQLCA.SQLCODE = rc;
	}
	CHECK_ERROR(SQLCA.SQLCODE);

	/*
	 EXEC SQL DELETE FROM emp
	 WHERE emp_no < 4 ;
	 */
	{
		nldb_cursor_t cursor_emp;
		nldb_rc_t rc;
		rc = nldb_cursor_open(tx, esql_table_emp, &cursor_emp);
		if (rc == NLDB_OK) {
			nldb_key_t emp_min_key = { NULL, 0 };
			rc = nldb_cursor_seek_forward(cursor_emp, emp_min_key);
			if (rc != NLDB_OK) {
				SQLCA.SQLCODE = rc;
				goto close_cursor_emp2;
			}
			while (1) {
				nldb_key_t nldb_emp_key;
				nldb_value_t nldb_emp_value;
				rc = nldb_cursor_move_forward(cursor_emp, &nldb_emp_key,
						&nldb_emp_value);
				if (rc == NLDB_ERROR_END_OF_ITERATION) {
					break;
				}
				struct emp::key_t * emp_key = (emp::key_t *) nldb_emp_key.data;
				struct emp::value_t * emp_value =
						(emp::value_t *) nldb_emp_value.data;
				tx_assert( emp_key);
				tx_assert( emp_value);
				tx_assert( sizeof(*emp_key) == nldb_emp_key.length);
				tx_assert( sizeof(*emp_value) == nldb_emp_value.length);

				if (emp_value->age < 4) {
					{
						// NLDB requires each key to be encoded with big endian.
						// However, emp_key->emp_no is already in big endian, as they are returned from the nldb interface.
						// So, no need to convert these keys to big endian.

						nldb_key_t nldb_emp_key = { emp_key, sizeof(*emp_key) };
						SQLCA.SQLCODE = nldb_table_del(tx, esql_table_emp,
								nldb_emp_key);

						if (SQLCA.SQLCODE != NLDB_OK) {
							goto close_cursor_emp2;
						}
					}
					break;
				}
			}
		}

		close_cursor_emp2: rc = nldb_cursor_close(cursor_emp);
		if (rc != NLDB_OK)
			SQLCA.SQLCODE = rc;
	}
	CHECK_ERROR(SQLCA.SQLCODE);

}

void begin_transaction() {
	/*
	 EXEC SQL BEGIN TRANSACTION;
	 */
	SQLCA.SQLCODE = nldb_tx_init(db, &tx);
	if (SQLCA.SQLCODE == NLDB_OK) {
		SQLCA.SQLCODE = nldb_tx_begin(tx);
	}

	CHECK_ERROR(SQLCA.SQLCODE);
}

void abort_transaction() {
	/*
	 EXEC SQL ABORT TRANSACTION;
	 */
	SQLCA.SQLCODE = nldb_tx_abort(tx);
	if (SQLCA.SQLCODE == NLDB_OK) {
		SQLCA.SQLCODE = nldb_tx_destroy(tx);
	}

	CHECK_ERROR(SQLCA.SQLCODE);
}

void commit_transaction() {
	/*
	 EXEC SQL COMMIT TRANSACTION;
	 */
	SQLCA.SQLCODE = nldb_tx_commit(tx);
	if (SQLCA.SQLCODE == NLDB_OK) {
		SQLCA.SQLCODE = nldb_tx_destroy(tx);
	}

	CHECK_ERROR(SQLCA.SQLCODE);
}

void print_usage() {
	printf("\nquery <option> \n");
	printf(
			"Where option is create-database, drop-database, master, slave, or slave-master.\n\n");
	printf("1. create-database - Create a database, create tables in it. \n ");
	printf(
			"2. drop-database   - Drop thables in the created database, drop the database. \n");
	printf("3. master - Open the database as the master. \n");
	printf("4. slave - Open the database creates a slave. \n");
	printf(
			"5. slave-master - Open the database creates a slave. Become a new master as part of a replication tree. \n");
}

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		print_usage();
		return -1;
	}

	// db_open_option should be master, slave, or slave-master
	char * mode = argv[1];

	if (strcmp(mode, "create-database") == 0) {
		create_db();
		create_tables();
	} else if (strcmp(mode, "drop-database") == 0) {
		drop_tables();
		drop_db();
	} else {
		open_db(mode);

		open_tables();

		begin_transaction();
		{
			insert_into_emp();
		}
		abort_transaction();

		begin_transaction();
		{
			insert_into_emp();
			insert_into_dept();
			insert_into_dept_emp();
			select();

			update();
			select();

			delete_from_emp();
			select();
		}
		commit_transaction();

		close_tables();

		close_db();
	}

	return 0;
}

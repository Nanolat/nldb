/*
 * (C) Copyright 2012 ThankyouSoft (http://ThankyouSoft.com/) and Nanolat(http://Nanolat.com).
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the NOSL(Nanolat Open Source License) v1.0.
 * 
 * Simple Description :
 * - If you are in USA, Canada, or Germany, it is *likely* that you get free LGPL v2.1.
 * - In other countries, you are *likely* under GPL v3, but you can by LGPL v2.1 from Nanolat Co., Ltd.
 * - For free software, you *definitely* get free LGPL v2.1 whereever you are.
 * 
 * Detailed Description :
 * - In the hope of seeing increasing open source contributions in countries 
 *   that do not have enough open source contributors, Nanolat Open Source License 
 *   provides free LGPL v2.1 only to companies and people of countries 
 *   that have at least three KudoRank 10 contributors in http://www.ohloh.net/people. 
 *   The list of countries are updated by the beginning of a year. 
 *   In 2013, these countries are USA, Canada, and Germany.
 *
 * - For companies and people of other countries, you are under GPL v3.0, 
 *   but you can buy LGPL v2.1 license for a product from Nanolat Co., Ltd.
 *
 * - However, for non-commercial usage, you get free LGPL v2.1 whereever you are.
 *
 * - A company is of a country if the head of office is in the country.
 *
 * - A person is of a country if the nationality of the person is the country.
 *
 * - A country has a KudoRank 10 contributor 
 *   if the contributor's location is the country in http://www.ohloh.net/.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 *
 * Contributors:
 *     Kangmo Kim
 */


#if defined(WIN32) || defined(_WIN64)
#include <WinSock2.h>
#include "../thrift/lib/cpp/src/windows/GetTimeOfDay.h"
#else
# include <sys/time.h>
#endif

#include <txbase/tx_assert.h>

#include <nldb/nldb.h>

#include <iostream>
#include <stdio.h>

// memcpy
#include <string.h>

// htonl
#include <arpa/inet.h>

#define RECORDS_PER_TX (1L)
//#define TEST_TRANSACTIONS   (10000000L)
#define TEST_TRANSACTIONS   (1000000L)
#define TEST_DATA_COUNT (RECORDS_PER_TX + TEST_TRANSACTIONS)

#define KEY_SIZE   (8)
#define VALUE_SIZE (128)

typedef struct tbl_key {
	char data[ KEY_SIZE ];
} tbl_key ;
typedef struct tbl_value{
	char data[ VALUE_SIZE ];
} tbl1_value ;

typedef unsigned char rnd_number_t;
//typedef nldb_uint64_t rnd_number_t;

rnd_number_t * rndNumsA;
tbl_key * rndKeysA;
tbl_value * rndValuesA;

#include <random>


// Fill data into char array with the given random value
void fillData(unsigned int dataNum, char * data, size_t size, rnd_number_t rndNum )
{
	// The size should be multiple of sizeof(rnd_number_t)
	tx_assert(size % sizeof(rnd_number_t) == 0);

	size_t original_size = size;

	while (size > 0)
	{
		memcpy(data, &rndNum, sizeof(rnd_number_t));
		data += sizeof(rnd_number_t);
		size -= sizeof(rnd_number_t);
	}

	unsigned int keyPostfix = htonl(dataNum);

	if (original_size >= sizeof(keyPostfix))
	{
		// To make the key unique, copy the dataNum to the end of the key.
		memcpy(data - sizeof(keyPostfix), &keyPostfix, sizeof(keyPostfix));
	}
}
void generateRandomNumbers()
{
	rndNumsA = new rnd_number_t [TEST_DATA_COUNT];
	rndKeysA = new tbl_key [TEST_DATA_COUNT];
	rndValuesA = new tbl_value [TEST_DATA_COUNT];

	std::default_random_engine generator;
	std::uniform_int_distribution<nldb_uint64_t> distribution(1,((nldb_uint64_t)1LL)<<63);

	for (unsigned int i = 0; i< TEST_DATA_COUNT; i++ )
	{
		rnd_number_t rndValue = (rnd_number_t) distribution(generator);
  	    rndNumsA[i] = rndValue;                   // simulate rolling a die
  	    fillData( i, rndKeysA[i].data, sizeof(rndKeysA[i].data), rndValue);
  	    fillData( i, rndValuesA[i].data, sizeof(rndValuesA[i].data), rndValue);
	}

}

const int TEST_DB_ID = 1;
const int TC_TABLE_ID = 1;
const int VOLATILE_TABLE_ID = 2;
const int PERSISTENT_TABLE_ID = 3;
const int REPLICATION_PORT = 9090;

static void fatal_exit(const char* error, nldb_rc_t rc) {
	fprintf(stderr, "[code=%d]%s\n",rc, error);

	exit(-1);
}
static void unexpected_result(const char* error, nldb_rc_t actual_rc, nldb_rc_t expected_rc) {
	fprintf(stderr, "%s [expected=%d, actual=%d]\n",error, expected_rc, actual_rc);
	tx_assert(0);
}


static void unexpected_value(const char* error) {
	fprintf(stderr, "error = %s\n",error);
}


static void create_tables(nldb_db_t & db)
{
	nldb_rc_t rc;

	rc = nldb_table_create(db, TC_TABLE_ID, NLDB_TABLE_TC);
	if (rc) fatal_exit("Failed to create a table with TC_TABLE_ID", rc);

	rc = nldb_table_create(db, VOLATILE_TABLE_ID, NLDB_TABLE_VOLATILE);
	if (rc) fatal_exit("Failed to create a table with VOLATILE_TABLE_ID", rc);

	rc = nldb_table_create(db, PERSISTENT_TABLE_ID, NLDB_TABLE_PERSISTENT);
	if (rc) fatal_exit("Failed to create a table with NLDB_TABLE_PERSISTENT", rc);
}

static void drop_tables(nldb_db_t & db)
{
	nldb_rc_t rc;

	rc = nldb_table_drop(db, TC_TABLE_ID);
	if (rc) fatal_exit("Failed to drop a table with TC_TABLE_ID", rc);

	rc = nldb_table_drop(db, VOLATILE_TABLE_ID);
	if (rc) fatal_exit("Failed to drop a table with VOLATILE_TABLE_ID", rc);

	rc = nldb_table_drop(db, PERSISTENT_TABLE_ID);
	if (rc) fatal_exit("Failed to drop a table with NLDB_TABLE_PERSISTENT", rc);
}

static void put_random_records( nldb_tx_t & tx, nldb_table_t & table, int data_index, size_t rnd_count, const char * data_message, nldb_rc_t expected_rc )
{
	nldb_rc_t rc;

	// put a random record
	for (size_t i=data_index; i<data_index+rnd_count; i++) {
		tbl_key & key = rndKeysA[i];

		tbl_value & value = rndValuesA[i];

		nldb_key_t nldb_key = { &key, sizeof(key) };
		nldb_value_t nldb_value = { &value, sizeof(value) };

		rc = nldb_table_put( tx, table, nldb_key, nldb_value);
		if (rc != expected_rc)
		{
			 unexpected_result("put : got an unexpected result", rc, expected_rc);
		}
	}
}


static void get_random_records( nldb_tx_t & tx, nldb_table_t & table, int data_index, size_t rnd_count, const char * data_message, nldb_rc_t expected_rc )
{
	nldb_rc_t rc;

	// put a random record
	for (size_t i=data_index; i< data_index + rnd_count; i++) {
		tbl_key & key = rndKeysA[i];
		tbl_value & expected_value = rndValuesA[i];

		nldb_key_t nldb_key = { &key, sizeof(key) };

		nldb_value_t actual_value;

		rc = nldb_table_get( tx, table, nldb_key, & actual_value, NULL/* nldb_order_t */);
		if (rc != expected_rc)
		{
			 unexpected_result("get : got an unexpected result", rc, expected_rc);
		}

		if ( rc == 0 )
		{
			tx_assert( sizeof( expected_value ) == actual_value.length );

			if ( memcmp( &expected_value, actual_value.data, actual_value.length ) != 0 )
			{
				unexpected_value("get : got an unexpected value!");
			}
		}
	}
}

static void del_random_records( nldb_tx_t & tx, nldb_table_t & table, int data_index, size_t rnd_count, nldb_rc_t expected_rc )
{
	nldb_rc_t rc;

	// put a random record
	for (size_t i=data_index; i<data_index+rnd_count; i++) {

		tbl_key & key = rndKeysA[i];
		nldb_key_t nldb_key = { &key, sizeof(key) };

		rc = nldb_table_del( tx, table, nldb_key);
		if (rc != expected_rc)
		{
			 unexpected_result("del : got an unexpected result", rc, expected_rc);
		}
	}
}

typedef void (*test_func_t)(nldb_db_t & db, nldb_table_t & table);

void measure_performance(nldb_db_t & db, nldb_table_t & table, const char * test_name, test_func_t test_func )
{
    struct timeval start_time, end_time;
	gettimeofday(&start_time, NULL);

	// run test function
	{
		test_func(db, table);
	}

	nldb_rc_t rc = nldb_db_wait_for_replication_publishers( db );
	if (rc) fatal_exit("Failed to wait for replication publishers", rc);

	gettimeofday(&end_time, NULL);

    double start, end;
    start = start_time.tv_sec + ((double) start_time.tv_usec / 1000000);
    end = end_time.tv_sec + ((double) end_time.tv_usec / 1000000);

    std::cout.precision(15);
    std::cout << test_name << " performance: ";
    std::cout << (TEST_TRANSACTIONS * 1.0) / (end - start)
              << " transactions/secs" << std::endl;
}

typedef struct test_case_t {
	const char * test_name;
	test_func_t test_func;
} perf_case_t;

void put_test(nldb_db_t & db, nldb_table_t & table) {
	nldb_tx_t tx;
	nldb_rc_t rc = nldb_tx_init( db, &tx );
	if (rc) fatal_exit("Failed to initialize a transaction", rc);

	for (int loop = 0 ; loop < TEST_TRANSACTIONS; loop ++ )
	{
		rc = nldb_tx_begin(tx);
		if (rc) fatal_exit("Failed to begin a transaction", rc);

		(void)put_random_records( tx, table, loop, RECORDS_PER_TX, "ThankyouSoft", NLDB_OK);

		rc = nldb_tx_commit(tx);
		if (rc) fatal_exit("Failed to commit a transaction", rc);
	}

	rc = nldb_tx_destroy( tx );
	if (rc) fatal_exit("Failed to destroy a transaction", rc);
}

void get_test(nldb_db_t & db, nldb_table_t & table) {
	nldb_tx_t tx;
	nldb_rc_t rc = nldb_tx_init( db, &tx );
	if (rc) fatal_exit("Failed to initialize a transaction", rc);

	for (int loop = 0 ; loop < TEST_TRANSACTIONS; loop ++ )
	{
		rc = nldb_tx_begin(tx);
		if (rc) fatal_exit("Failed to begin a transaction", rc);

		(void)get_random_records( tx, table, loop, RECORDS_PER_TX, "ThankyouSoft", NLDB_OK);

		rc = nldb_tx_commit(tx);
		if (rc) fatal_exit("Failed to commit a transaction", rc);
	}

	rc = nldb_tx_destroy( tx );
	if (rc) fatal_exit("Failed to destroy a transaction", rc);
}


void forward_cursor_test(nldb_db_t & db, nldb_table_t & table) {
	nldb_tx_t tx;
	nldb_rc_t rc = nldb_tx_init( db, &tx );
	if (rc) fatal_exit("Failed to initialize a transaction", rc);

	rc = nldb_tx_begin(tx);
	if (rc) fatal_exit("Failed to begin a transaction", rc);

	nldb_cursor_t cursor;

	rc = nldb_cursor_open(tx, table, &cursor);
	if (rc == NLDB_ERROR_UNSUPPORTED_FEATURE ) {
		printf("cursor feature is not supported for this table type.\n");
		goto on_error;
	}
	if (rc) fatal_exit("Failed to open a cursor", rc);

	{
		tbl_key min_key;
		fillData(0, min_key.data, sizeof(min_key), 0);

		nldb_key_t nldb_min_key = {min_key.data, sizeof(min_key)};

		rc = nldb_cursor_seek_forward( cursor, nldb_min_key );
		tx_assert( rc == NLDB_OK );

		nldb_key_t key;
		nldb_value_t value;

		tbl_key prev_key;
		memset( &prev_key, 0, sizeof(prev_key));

		nldb_rc_t rc;
		while( ( rc = nldb_cursor_move_forward( cursor,  &key,  &value ) ) == 0 )
		{
			tx_assert( key.length == sizeof(prev_key));
			tx_assert( memcmp( &prev_key, key.data, key.length) <= 0 );

			memcpy( &prev_key, key.data, key.length );

			rc = nldb_key_free( table, key );
			tx_assert( rc == NLDB_OK );

			rc = nldb_value_free( table, value );
			tx_assert( rc == NLDB_OK );
		}

		if ( rc != NLDB_ERROR_END_OF_ITERATION ) {
			printf("Invalid return code for nldb_cursor_move_forward => %d\n", rc);
			tx_assert( 0 );
		}
	}

	rc = nldb_cursor_close( cursor );
	if (rc) fatal_exit("Failed to close a cursor", rc);

on_error:
	rc = nldb_tx_commit(tx);
	if (rc) fatal_exit("Failed to commit a transaction", rc);

	rc = nldb_tx_destroy( tx );
	if (rc) fatal_exit("Failed to destroy a transaction", rc);
}

void backward_cursor_test(nldb_db_t & db, nldb_table_t & table) {
	nldb_tx_t tx;
	nldb_rc_t rc = nldb_tx_init( db, &tx );
	if (rc) fatal_exit("Failed to initialize a transaction", rc);

	rc = nldb_tx_begin(tx);
	if (rc) fatal_exit("Failed to begin a transaction", rc);

	nldb_cursor_t cursor;

	rc = nldb_cursor_open(tx, table, &cursor);
	if (rc == NLDB_ERROR_UNSUPPORTED_FEATURE ) {
		printf("cursor feature is not supported for this table type.\n");
		goto on_error;
	}
	if (rc) fatal_exit("Failed to open a cursor", rc);

	{
		tbl_key max_key;
		fillData(0xFFFFFFFFU, max_key.data, sizeof(max_key), 0xFFU);

		nldb_key_t nldb_max_key = {max_key.data, sizeof(max_key)};

		rc = nldb_cursor_seek_backward( cursor, nldb_max_key );
		tx_assert( rc == NLDB_OK );

		nldb_key_t key;
		nldb_value_t value;

		tbl_key prev_key;
		memset( &prev_key, 0xFFU, sizeof(prev_key));

		nldb_rc_t rc;
		while( ( rc = nldb_cursor_move_backward( cursor,  &key,  &value ) ) == 0 )
		{
			tx_assert( key.length == sizeof(prev_key));
			tx_assert( memcmp( &prev_key, key.data, key.length) >= 0 );

			memcpy( &prev_key, key.data, key.length );

			rc = nldb_key_free( table, key );
			tx_assert( rc == NLDB_OK );

			rc = nldb_value_free( table, value );
			tx_assert( rc == NLDB_OK );
		}

		if ( rc != NLDB_ERROR_END_OF_ITERATION ) {
			printf("Invalid return code for nldb_cursor_move_forward => %d\n", rc);
			tx_assert( 0 );
		}
	}

	rc = nldb_cursor_close( cursor );
	if (rc) fatal_exit("Failed to close a cursor", rc);

on_error:
	rc = nldb_tx_commit(tx);
	if (rc) fatal_exit("Failed to commit a transaction", rc);

	rc = nldb_tx_destroy( tx );
	if (rc) fatal_exit("Failed to destroy a transaction", rc);
}


void del_test(nldb_db_t & db, nldb_table_t & table) {
	nldb_tx_t tx;
	nldb_rc_t rc = nldb_tx_init( db, &tx );
	if (rc) fatal_exit("Failed to initialize a transaction", rc);

	for (int loop = 0 ; loop < TEST_TRANSACTIONS; loop ++ )
	{
		rc = nldb_tx_begin(tx);
		if (rc) fatal_exit("Failed to begin a transaction", rc);

		(void)del_random_records( tx, table, loop, RECORDS_PER_TX, NLDB_OK);

		rc = nldb_tx_commit(tx);
		if (rc) fatal_exit("Failed to commit a transaction", rc);
	}

	rc = nldb_tx_destroy( tx );
	if (rc) fatal_exit("Failed to destroy a transaction", rc);
}

#if defined(DEBUG)

extern bool __print_nodes_on_table_close;
extern bool __check_consistency_on_table_close;

void print_node_test(nldb_db_t & db, nldb_table_t & table) {
	__print_nodes_on_table_close = true;
	__check_consistency_on_table_close = true;

	nldb_tx_t tx;
	nldb_rc_t rc = nldb_tx_init( db, &tx );
	if (rc) fatal_exit("Failed to initialize a transaction", rc);

	// Insert 100 records
	for (int loop = 0 ; loop < 100; loop ++ )
	{
		rc = nldb_tx_begin(tx);
		if (rc) fatal_exit("Failed to begin a transaction", rc);

		(void)put_random_records( tx, table, loop, RECORDS_PER_TX, "ThankyouSoft", NLDB_OK);

		rc = nldb_tx_commit(tx);
		if (rc) fatal_exit("Failed to commit a transaction", rc);
	}

	rc = nldb_tx_destroy( tx );
	if (rc) fatal_exit("Failed to destroy a transaction", rc);

}

#endif /* DEBUG */




// List of test cases
test_case_t tests[] = {
	{"put", put_test},
	{"get", get_test},
	{"forward_cursor", forward_cursor_test},
	{"backward_cursor", backward_cursor_test},
	{"del", del_test},
	/*
#if defined(DEBUG)
	{"print_nodes", print_node_test},
#endif
   */
	{NULL, NULL}
};

// List of tables to test for each test case
typedef struct table_desc_t {
	const char * table_desc;
	int    table_id;
}table_desc_t;

table_desc_t tables[] = {
	{"Array Tree", VOLATILE_TABLE_ID },
	{"Tokyo Cabinet", TC_TABLE_ID },
	{"LevelDB", PERSISTENT_TABLE_ID },
	{NULL, -1 },
};

int llcep_nldb_master(int arc, char** argv) {

	generateRandomNumbers();

	nldb_rc_t rc;
	rc = nldb_init();
	if (rc) fatal_exit("Failed to initialize LLDB system", rc);

	// BUGBUG : The master hangs if it runs in the same db path with a slave which is running.
	// Fix : Need to show an error message "An instance is already up and running on this db path"
	rc = nldb_db_create( TEST_DB_ID );
	if (rc) fatal_exit("Failed to create LLDB", rc);

	nldb_replication_master_op_t master_op = { "*", REPLICATION_PORT };

	nldb_db_t db;
	rc = nldb_db_open( TEST_DB_ID, & master_op, NULL/*slave*/, &db );
	if (rc) fatal_exit("Failed to open LLDB", rc);

	create_tables( db );

	for (int i=0; tables[i].table_desc != NULL; i++)
	{
		printf("\nRunning tests with table type : %s\n", tables[i].table_desc );

		nldb_table_t table;
		nldb_rc_t rc = nldb_table_open(db, tables[i].table_id, &table);
		if (rc) fatal_exit("Failed to open table", rc);

		for (int j=0; tests[j].test_name != NULL; j++) {
			measure_performance(db, table, tests[j].test_name, tests[j].test_func);
		}

		rc = nldb_table_close(table);
		if (rc) fatal_exit("Failed to open table", rc);
	}

	printf("Test Finished.\n");

	drop_tables( db );

	rc = nldb_db_close( db );
	if (rc) fatal_exit("Failed to close LLDB", rc);

	rc = nldb_db_drop( TEST_DB_ID );
	if (rc) fatal_exit("Failed to drop LLDB", rc);

	rc = nldb_destroy();
	if (rc) fatal_exit("Failed to finalize LLDB instance", rc);

    return EXIT_SUCCESS;
}

nldb_rc_t replication_trigger_handler(const nldb_trigger_type_t trigger_type, const nldb_table_id_t & table_id, const nldb_key_t & key, const nldb_value_t & value)
{
	printf("Type:%s, Table Id:%d, Key length:%d, Value length:%d\n", (trigger_type==TT_PUT)?"PUT":(trigger_type==TT_DEL)?"DEL":"UNKNOWN", table_id, key.length, value.length);
	return NLDB_OK;
}

int llcep_nldb_slave(int arc, char** argv) {

	nldb_rc_t rc;
	rc = nldb_init();
	if (rc) fatal_exit("Failed to initialize LLDB system", rc);

	rc = nldb_db_create( TEST_DB_ID );
	if (rc) fatal_exit("Failed to create LLDB", rc);

	nldb_replication_slave_op_t slave_op = { {"localhost", REPLICATION_PORT},  };
	slave_op.trigger_handler = NULL;
//	slave_op.trigger_handler = & replication_trigger_handler;

	nldb_db_t db;
	rc = nldb_db_open( TEST_DB_ID, NULL /*master*/, & slave_op/*slave*/, & db );
	if (rc) fatal_exit("Failed to open LLDB", rc);

	create_tables( db );

	printf("Press Enter to start dropping tables, closing DB, dropping DB\n");
	(void)getchar();

	drop_tables( db );

	rc = nldb_db_close( db );
	if (rc) fatal_exit("Failed to close LLDB", rc);

	rc = nldb_db_drop( TEST_DB_ID );
	if (rc) fatal_exit("Failed to drop LLDB", rc);

    return EXIT_SUCCESS;
}


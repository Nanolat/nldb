/*
 * (C) Copyright 2012, 2013 ThankyouSoft (http://ThankyouSoft.com/) and Nanolat(http://Nanolat.com).
 *                                                      Writen by Kangmo Kim ( kangmo@nanolat.com )
 *
 * =================
 * Apache v2 License
 * =================
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Contributors : 
 *   Kangmo Kim
 */

#include "nldb/nldb.h"
#include "nldb_server.h"

#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	if ( argc != 2) 
	{
		printf("Usage : tests <option>\n");
		printf("        option => nldb-master : run nldb master\n");
		printf("        option => nldb-slave  : run nldb slave\n");
		return -1;
	}

	char * mode = argv[1];
	if (strcmp( mode, "nldb-master") == 0 ) {
		return llcep_nldb_master(argc, argv);
	}

	if (strcmp( mode, "nldb-slave") == 0 ) {
		return llcep_nldb_slave(argc, argv);
	}

	printf("Unsupported mode : %s\n", mode);
}


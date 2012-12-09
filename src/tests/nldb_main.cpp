// llcep_server.cpp : Defines the entry point for the console application.
//

#include "nldb/nldb.h"
#include "nldb_server.h"

#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	if ( argc != 2) 
	{
		printf("Usage : llcep_server <option>\n");
		printf("        option => lldb-master : run lldb master\n");
		printf("        option => lldb-slave  : run lldb slave\n");
		return -1;
	}

	char * mode = argv[1];
	if (strcmp( mode, "lldb-master") == 0 ) {
		return llcep_nldb_master(argc, argv);
	}

	if (strcmp( mode, "lldb-slave") == 0 ) {
		return llcep_nldb_slave(argc, argv);
	}

	printf("Unsupported mode : %s\n", mode);
}


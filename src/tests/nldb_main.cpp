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


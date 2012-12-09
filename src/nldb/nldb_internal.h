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

#ifndef _O_NLDB_INTERNAL_H_
#define _O_NLDB_INTERNAL_H_ (1)

// All internal symbols are defined in this header. 
// This header is not exposed as part of the library interface header files.

// The initial transaction ID which is assigned to the first transaction that calls begin function.
#define INITIAL_TRANSACTION_ID (1)

// For linux platform, we need to define _snprintf, which exists only in windows platform.
#define _snprintf snprintf

#endif



/****************************************************************************
 * Copyright (C) 2010-2013 GGA Software Services LLC
 *
 * This file is part of Indigo toolkit.
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 3 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ***************************************************************************/

#ifndef __indigo_bingo__
#define __indigo_bingo__

#include "indigo.h"

CEXPORT int bingoCreateDatabaseFile (const char *location, const char *type, const char *options);
CEXPORT int bingoLoadDatabaseFile (const char *location);
CEXPORT int bingoCloseDatabase (int db);

//
// Record insertion/deletion
//
CEXPORT int bingoInsertRecordObj (int db, int obj);
CEXPORT int bingoDeleteRecord (int db, int index);

// Search methods that returns search object
// Search object is an iterator 
CEXPORT int bingoSearchSub (int db, int query_obj, const char *options);
CEXPORT int bingoSearchSim (int db, int query_obj, float min, float max, const char *options);

//
// Search object methods
//
CEXPORT int bingoNext (int search_obj);
CEXPORT int bingoGetCurrentIndex (int search_obj);
// This method return IndigoObject that represents current object.
// After calling bingoNext this object automatically points to the next found result
CEXPORT int bingoGetObject (int search_obj);
CEXPORT int bingoEndSearch (int search_obj);

#endif // __indigo_bingo__

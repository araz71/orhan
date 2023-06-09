#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "DatabaseWrapper.h"
#include "SqliteDatabase.h"
#include "RedisDatabase.h"

namespace orhan
{
using Database = DatabaseWrapper<RedisDatabase>;
}
#endif

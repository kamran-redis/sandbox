#include "intervaltree.h"
#include "redismodule.h"

#define INTERVALTREETYPE_ENCODING_VERSION 0
#define COMMAND_NAME "it"
#define TYPE_NAME "intrvl-ky"
#define LOG_LEVEL "notice"

static RedisModuleType *IntervalTreeType;

struct function_ctx {
  RedisModuleCtx *ctx;
  int count;
};

struct function_aof {
  RedisModuleIO *aof;
  RedisModuleString *key;
};

struct function_rdb {
  RedisModuleIO *rdb;
};

// static void printargs(RedisModuleCtx *ctx, char *s, RedisModuleString **argv,
//                       int argc) {
//   RedisModule_Log(ctx, LOG_LEVEL, "%s ", s);
//   for (int i = 0; i < argc; i++)
//     RedisModule_Log(ctx, LOG_LEVEL, "\t %s",
//                     RedisModule_StringPtrLen(argv[i], NULL));
// }

/************************************************
 *  it.add key lo hi value
 ***********************************************/
int IntervalTreeTypeAdd_RedisCommand(RedisModuleCtx *ctx,
                                     RedisModuleString **argv, int argc) {
  RedisModule_AutoMemory(ctx);
  // printargs(ctx, "ADD", argv, argc);
  if (argc != 5) return RedisModule_WrongArity(ctx);
  RedisModuleKey *key =
      RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ | REDISMODULE_WRITE);
  int type = RedisModule_KeyType(key);
  if (type != REDISMODULE_KEYTYPE_EMPTY &&
      RedisModule_ModuleTypeGetType(key) != IntervalTreeType) {
    return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
  }
  long long lo;
  if ((RedisModule_StringToLongLong(argv[2], &lo) != REDISMODULE_OK)) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: lo must be a signed 64 bit integer");
  }

  long long hi;
  if ((RedisModule_StringToLongLong(argv[3], &hi) != REDISMODULE_OK)) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: hi must be a signed 64 bit integer");
  }

  long long value;
  if ((RedisModule_StringToLongLong(argv[4], &value) != REDISMODULE_OK)) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid values: value must be a signed 64 bit integer");
  }

  if (lo > hi) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: lo must be lower than hi");
  }

  /* Create an empty value object if the key is currently empty. */
  IntervalTree *it;
  if (type == REDISMODULE_KEYTYPE_EMPTY) {
    // RedisModule_Log(ctx, LOG_LEVEL, "\t %s", "Creating new key");
    it = intervalTreeCreate();
    RedisModule_ModuleTypeSetValue(key, IntervalTreeType, it);
  } else {
    it = RedisModule_ModuleTypeGetValue(key);
  }

  /* Insert the new element. */
  intervalTreePut(it, lo, hi, value);
  RedisModule_SignalKeyAsReady(ctx, argv[1]);

  RedisModule_ReplyWithLongLong(ctx, intervaltreeCount(it));
  RedisModule_ReplicateVerbatim(ctx);
  return REDISMODULE_OK;
}

/************************************************
 *  it.rem key lo hi
 ***********************************************/
int IntervalTreeTypeRem_RedisCommand(RedisModuleCtx *ctx,
                                     RedisModuleString **argv, int argc) {
  RedisModule_AutoMemory(ctx);
  // printargs(ctx, "REM", argv, argc);

  if (argc != 4) return RedisModule_WrongArity(ctx);
  RedisModuleKey *key =
      RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ | REDISMODULE_WRITE);
  int type = RedisModule_KeyType(key);
  if (type != REDISMODULE_KEYTYPE_EMPTY &&
      RedisModule_ModuleTypeGetType(key) != IntervalTreeType) {
    return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
  }

  long long lo;
  if ((RedisModule_StringToLongLong(argv[2], &lo) != REDISMODULE_OK)) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: lo must be a signed 64 bit integer");
  }

  long long hi;
  if ((RedisModule_StringToLongLong(argv[3], &hi) != REDISMODULE_OK)) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: hi must be a signed 64 bit integer");
  }

  if (lo > hi) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: lo must be lower than hi");
  }

  // if key does not exist return 0
  IntervalTree *it;
  if (type == REDISMODULE_KEYTYPE_EMPTY) {
    // RedisModule_Log(ctx, LOG_LEVEL, "\t %s", "Key does not exist");
    RedisModule_ReplyWithLongLong(ctx, 0);
    return REDISMODULE_OK;
  } else {
    it = RedisModule_ModuleTypeGetValue(key);
  }

  // delete the node
  intervaltreeDelete(it, lo, hi);
  // delete key if length is zero
  if (!intervaltreeCount(it)) {
    // RedisModule_Log(ctx, LOG_LEVEL, "\t %s", "Deleting key");
    RedisModule_DeleteKey(key);
  }

  RedisModule_SignalKeyAsReady(ctx, argv[1]);

  RedisModule_ReplyWithLongLong(ctx, intervaltreeCount(it));
  RedisModule_ReplicateVerbatim(ctx);
  return REDISMODULE_OK;
}

/************************************************
 *  it.inter key lo hi
 ***********************************************/
int IntervalTreeTypeInter_RedisCommand(RedisModuleCtx *ctx,
                                       RedisModuleString **argv, int argc) {
  RedisModule_AutoMemory(ctx);

  if (argc != 4) return RedisModule_WrongArity(ctx);
  RedisModuleKey *key = RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ);
  int type = RedisModule_KeyType(key);
  if (type != REDISMODULE_KEYTYPE_EMPTY &&
      RedisModule_ModuleTypeGetType(key) != IntervalTreeType) {
    return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
  }

  long long lo;
  if ((RedisModule_StringToLongLong(argv[2], &lo) != REDISMODULE_OK)) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: lo must be a signed 64 bit integer");
  }

  long long hi;
  if ((RedisModule_StringToLongLong(argv[3], &hi) != REDISMODULE_OK)) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: hi must be a signed 64 bit integer");
  }

  if (lo > hi) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: lo must be lower than hi");
  }

  // if key does not exist return 0
  IntervalTree *it;
  if (type == REDISMODULE_KEYTYPE_EMPTY) {
    RedisModule_ReplyWithLongLong(ctx, 0);
    return REDISMODULE_OK;
  }
  it = RedisModule_ModuleTypeGetValue(key);
  return RedisModule_ReplyWithLongLong(ctx, intervalTreeContains(it, lo, hi));
}

static bool searchout(const node *x, void *f_data) {
  struct function_ctx *fctx = ((struct function_ctx *)f_data);

  RedisModule_ReplyWithArray(fctx->ctx, 3);
  RedisModule_ReplyWithLongLong(fctx->ctx, x->lo);
  RedisModule_ReplyWithLongLong(fctx->ctx, x->hi);
  RedisModule_ReplyWithLongLong(fctx->ctx, x->value);
  fctx->count = fctx->count + 1;
  return true;
}

/************************************************
 *  it.search key lo hi count
 ***********************************************/
int IntervalTreeTypeSearch_RedisCommand(RedisModuleCtx *ctx,
                                        RedisModuleString **argv, int argc) {
  RedisModule_AutoMemory(ctx);

  if (argc != 5) return RedisModule_WrongArity(ctx);
  RedisModuleKey *key = RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ);
  int type = RedisModule_KeyType(key);
  if (type != REDISMODULE_KEYTYPE_EMPTY &&
      RedisModule_ModuleTypeGetType(key) != IntervalTreeType) {
    return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
  }

  long long lo;
  if ((RedisModule_StringToLongLong(argv[2], &lo) != REDISMODULE_OK)) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: lo must be a signed 64 bit integer");
  }

  long long hi;
  if ((RedisModule_StringToLongLong(argv[3], &hi) != REDISMODULE_OK)) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: hi must be a signed 64 bit integer");
  }

  if (lo > hi) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: lo must be lower than hi");
  }

  long long count;
  if ((RedisModule_StringToLongLong(argv[4], &count) != REDISMODULE_OK)) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: count must be an integer ");
  }
  if (!(count >= 0)) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: count must 0 or greater");
  }

  // if key does not exist return empty array
  if (type == REDISMODULE_KEYTYPE_EMPTY) {
    return RedisModule_ReplyWithEmptyArray(ctx);
    ;
  }

  IntervalTree *it;
  it = RedisModule_ModuleTypeGetValue(key);

  RedisModule_ReplyWithArray(ctx, REDISMODULE_POSTPONED_ARRAY_LEN);
  struct function_ctx fctx = {ctx, 0};
  IntervalTreeSearch(it, lo, hi, count, searchout, &fctx);
  RedisModule_ReplySetArrayLength(ctx, fctx.count);
  return REDISMODULE_OK;
}

/************************************************
 *
 ***********************************************/
static bool rangeread(const node *x, const node *p, char *side, int level,
                      void *f_data) {
  REDISMODULE_NOT_USED(side);
  REDISMODULE_NOT_USED(level);
  REDISMODULE_NOT_USED(p);

  struct function_ctx *fctx = ((struct function_ctx *)f_data);

  RedisModule_ReplyWithArray(fctx->ctx, 3);
  RedisModule_ReplyWithLongLong(fctx->ctx, x->lo);
  RedisModule_ReplyWithLongLong(fctx->ctx, x->hi);
  RedisModule_ReplyWithLongLong(fctx->ctx, x->value);
  fctx->count = fctx->count + 1;
  return true;
}

/************************************************
 *  it.range key start count
 * TODO: incomplete use iterator, right now returning all
 ***********************************************/
int IntervalTreeTypeRange_RedisCommand(RedisModuleCtx *ctx,
                                       RedisModuleString **argv, int argc) {
  RedisModule_AutoMemory(ctx);
  if (argc != 4) {
    return RedisModule_WrongArity(ctx);
  }
  RedisModuleKey *key =
      RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ | REDISMODULE_WRITE);
  int type = RedisModule_KeyType(key);
  if (type != REDISMODULE_KEYTYPE_EMPTY &&
      RedisModule_ModuleTypeGetType(key) != IntervalTreeType) {
    return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
  }

  // TODO: check for positive value
  long long start;
  if ((RedisModule_StringToLongLong(argv[2], &start) != REDISMODULE_OK)) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: start must greater than 0");
  }
  // TODO: check for positive value
  long long count;
  if ((RedisModule_StringToLongLong(argv[3], &count) != REDISMODULE_OK)) {
    return RedisModule_ReplyWithError(
        ctx, "ERR invalid value: count must be greater than 0 ");
  }

  // if key does not exist reply with empty array
  if (type == REDISMODULE_KEYTYPE_EMPTY) {
    return RedisModule_ReplyWithEmptyArray(ctx);
  }

  IntervalTree *it = RedisModule_ModuleTypeGetValue(key);

  RedisModule_ReplyWithArray(ctx, REDISMODULE_POSTPONED_ARRAY_LEN);
  struct function_ctx fctx = {ctx, 0};
  intervaltreeInOrder(it, rangeread, &fctx);
  RedisModule_ReplySetArrayLength(ctx, fctx.count);
  return REDISMODULE_OK;
}

/************************************************
 *  it.healthcheck key
 ***********************************************/
int IntervalTreeTypeHealthCheck_RedisCommand(RedisModuleCtx *ctx,
                                             RedisModuleString **argv,
                                             int argc) {
  RedisModule_AutoMemory(ctx);
  if (argc != 2) {
    return RedisModule_WrongArity(ctx);
  }
  RedisModuleKey *key = RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ);
  int type = RedisModule_KeyType(key);
  if (type != REDISMODULE_KEYTYPE_EMPTY &&
      RedisModule_ModuleTypeGetType(key) != IntervalTreeType) {
    return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
  }

  if (type == REDISMODULE_KEYTYPE_EMPTY) {
    return RedisModule_ReplyWithEmptyArray(ctx);
  }

  IntervalTree *it = RedisModule_ModuleTypeGetValue(key);

  RedisModule_ReplyWithArray(ctx, 10);
  RedisModule_ReplyWithCString(ctx, "balanced");
  RedisModule_ReplyWithLongLong(ctx, intervaltreeIsBalanced(it));
  RedisModule_ReplyWithCString(ctx, "bst");
  RedisModule_ReplyWithLongLong(ctx, intervaltreeIsBST(it));
  RedisModule_ReplyWithCString(ctx, "23");
  RedisModule_ReplyWithLongLong(ctx, intervaltreeIs23(it));
  RedisModule_ReplyWithCString(ctx, "maxconsistent");
  RedisModule_ReplyWithLongLong(ctx, intervaltreeIsMaxConistent(it));
  RedisModule_ReplyWithCString(ctx, "sizeconsistent");
  RedisModule_ReplyWithLongLong(ctx, intervaltreeIsSizeConsistent(it));
  return REDISMODULE_OK;
}

/************************************************
 *  it.height key
 ***********************************************/
int IntervalTreeTypeHeight_RedisCommand(RedisModuleCtx *ctx,
                                        RedisModuleString **argv, int argc) {
  RedisModule_AutoMemory(ctx);
  if (argc != 2) {
    return RedisModule_WrongArity(ctx);
  }
  RedisModuleKey *key = RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ);
  int type = RedisModule_KeyType(key);
  if (type != REDISMODULE_KEYTYPE_EMPTY &&
      RedisModule_ModuleTypeGetType(key) != IntervalTreeType) {
    return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
  }

  if (type == REDISMODULE_KEYTYPE_EMPTY) {
    return RedisModule_ReplyWithLongLong(ctx, 0);
  }

  IntervalTree *it = RedisModule_ModuleTypeGetValue(key);

  return RedisModule_ReplyWithLongLong(ctx, intervaltreeHeight(it));
}

/************************************************
 *  it.count key
 ***********************************************/
int IntervalTreeTypeCount_RedisCommand(RedisModuleCtx *ctx,
                                       RedisModuleString **argv, int argc) {
  RedisModule_AutoMemory(ctx);
  if (argc != 2) {
    return RedisModule_WrongArity(ctx);
  }
  RedisModuleKey *key = RedisModule_OpenKey(ctx, argv[1], REDISMODULE_READ);
  int type = RedisModule_KeyType(key);
  if (type != REDISMODULE_KEYTYPE_EMPTY &&
      RedisModule_ModuleTypeGetType(key) != IntervalTreeType) {
    return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
  }

  if (type == REDISMODULE_KEYTYPE_EMPTY) {
    return RedisModule_ReplyWithLongLong(ctx, 0);
  }

  IntervalTree *it = RedisModule_ModuleTypeGetValue(key);

  return RedisModule_ReplyWithLongLong(ctx, intervaltreeCount(it));
}

/************************************************
 *
 ***********************************************/
void *IntervalTreeTypeRDBLoad(RedisModuleIO *rdb, int encver) {
  // TODO version check
  REDISMODULE_NOT_USED(rdb);
  REDISMODULE_NOT_USED(encver);
  IntervalTree *it = intervalTreeCreate();
  uint64_t count = RedisModule_LoadUnsigned(rdb);
  while (count--) {
    intervalTreePut(it, RedisModule_LoadUnsigned(rdb),
                    RedisModule_LoadUnsigned(rdb),
                    RedisModule_LoadUnsigned(rdb));
  }
  return it;
}

/************************************************
 *
 * TODO: Make more efficient, remove recursion every add is log(n) rather than 1
 ***********************************************/
static bool rdbsave(const node *x, const node *p, char *side, int level,
                    void *f_data) {
  REDISMODULE_NOT_USED(side);
  REDISMODULE_NOT_USED(level);
  REDISMODULE_NOT_USED(p);
  struct function_rdb *fctx = ((struct function_rdb *)f_data);
  RedisModule_SaveSigned(fctx->rdb, x->lo);
  RedisModule_SaveSigned(fctx->rdb, x->hi);
  RedisModule_SaveSigned(fctx->rdb, x->value);
  return true;
}

void IntervalTreeTypeRDBSave(RedisModuleIO *rdb, void *value) {
  IntervalTree *it = value;
  RedisModule_SaveUnsigned(rdb, intervaltreeCount(it));
  struct function_rdb frdb = {rdb};
  intervaltreeInOrder(it, rdbsave, &frdb);
}

static bool aofrewrite(const node *x, const node *p, char *side, int level,
                       void *f_data) {
  REDISMODULE_NOT_USED(side);
  REDISMODULE_NOT_USED(level);
  REDISMODULE_NOT_USED(p);
  struct function_aof *fctx = ((struct function_aof *)f_data);
  RedisModule_EmitAOF(fctx->aof, "IT.ADD", "slll", fctx->key, x->lo, x->hi,
                      x->value);
  return true;
}

/************************************************
 *
 ***********************************************/
void IntervalTreeTypeAOFRewrite(RedisModuleIO *aof, RedisModuleString *key,
                                void *value) {
  IntervalTree *it = value;
  struct function_aof faof = {aof, key};
  intervaltreeInOrder(it, aofrewrite, &faof);
}

/************************************************
 *
 ***********************************************/
size_t IntervalTreeTypeMemUsage(const void *value) {
  const IntervalTree *it = value;

  return sizeof(it) + intervaltreeCount(it) * sizeof(struct node);
}

/************************************************
 *
 ***********************************************/
void IntervalTreeTypeFree(void *value) { intervalTreeRelease(value); }

/************************************************
 *
 ***********************************************/
int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv,
                       int argc) {
  REDISMODULE_NOT_USED(argv);
  REDISMODULE_NOT_USED(argc);

  if (RedisModule_Init(ctx, COMMAND_NAME, 1, REDISMODULE_APIVER_1) ==
      REDISMODULE_ERR)
    return REDISMODULE_ERR;

  RedisModuleTypeMethods tm = {.version = REDISMODULE_TYPE_METHOD_VERSION,
                               .rdb_load = IntervalTreeTypeRDBLoad,
                               .rdb_save = IntervalTreeTypeRDBSave,
                               .aof_rewrite = IntervalTreeTypeAOFRewrite,
                               .free = IntervalTreeTypeFree,
                               .mem_usage = IntervalTreeTypeMemUsage};

  IntervalTreeType = RedisModule_CreateDataType(
      ctx, TYPE_NAME, INTERVALTREETYPE_ENCODING_VERSION, &tm);
  if (IntervalTreeType == NULL) return REDISMODULE_ERR;

  if (RedisModule_CreateCommand(ctx, "it.add", IntervalTreeTypeAdd_RedisCommand,
                                "write deny-oom", 1, 1, 1) == REDISMODULE_ERR)
    return REDISMODULE_ERR;

  if (RedisModule_CreateCommand(ctx, "it.rem", IntervalTreeTypeRem_RedisCommand,
                                "write deny-oom", 1, 1, 1) == REDISMODULE_ERR)
    return REDISMODULE_ERR;

  if (RedisModule_CreateCommand(ctx, "it.inter",
                                IntervalTreeTypeInter_RedisCommand, "readonly",
                                1, 1, 1) == REDISMODULE_ERR)
    return REDISMODULE_ERR;

  if (RedisModule_CreateCommand(ctx, "it.search",
                                IntervalTreeTypeSearch_RedisCommand, "readonly",
                                1, 1, 1) == REDISMODULE_ERR)
    return REDISMODULE_ERR;

  if (RedisModule_CreateCommand(ctx, "it.range",
                                IntervalTreeTypeRange_RedisCommand, "readonly",
                                1, 1, 1) == REDISMODULE_ERR)
    return REDISMODULE_ERR;

  if (RedisModule_CreateCommand(ctx, "it.healthcheck",
                                IntervalTreeTypeHealthCheck_RedisCommand,
                                "readonly", 1, 1, 1) == REDISMODULE_ERR)
    return REDISMODULE_ERR;

  if (RedisModule_CreateCommand(ctx, "it.height",
                                IntervalTreeTypeHeight_RedisCommand, "readonly",
                                1, 1, 1) == REDISMODULE_ERR)
    return REDISMODULE_ERR;

  if (RedisModule_CreateCommand(ctx, "it.count",
                                IntervalTreeTypeCount_RedisCommand, "readonly",
                                1, 1, 1) == REDISMODULE_ERR)
    return REDISMODULE_ERR;

  return REDISMODULE_OK;
}

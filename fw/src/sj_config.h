/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_FW_SRC_SJ_CONFIG_H_
#define CS_FW_SRC_SJ_CONFIG_H_

#include <stdbool.h>

#include "common/mbuf.h"
#include "frozen/frozen.h"
#include "mongoose/mongoose.h"

/*
 * The usage pattern is this:
 * 1. Create an empty config struct at the beginning.
 * 2. Load the defaults.
 * 3. Then, apply overrides.
 *
 * When override is applied, previously allocated values are freed.
 * See ../test/unit_test.c for an example.
 */

bool sj_conf_check_access(const struct mg_str key, const char *acl);

enum sj_conf_type {
  CONF_TYPE_INT = 0,
  CONF_TYPE_BOOL = 1,
  CONF_TYPE_STRING = 2,
  CONF_TYPE_OBJECT = 3,
};

struct sj_conf_entry {
  enum sj_conf_type type;
  const char *key;
  union {
    int offset;
    int num_desc;
  };
};

/*
 * Parses config in 'json' into 'cfg' according to rules defined in 'schema' and
 * checking keys against 'acl'.
 */
bool sj_conf_parse(const struct mg_str json, const char *acl,
                   const struct sj_conf_entry *schema, void *cfg);

/*
 * Emit config in 'cfg' according to rules in 'schema'.
 * Keys are only emitted if their values are different from 'base'.
 * If 'base' is NULL then all keys are emitted.
 */
typedef void (*sj_conf_emit_cb_t)(struct mbuf *data, void *param);
void sj_conf_emit_cb(const void *cfg, const void *base,
                     const struct sj_conf_entry *schema, bool pretty,
                     struct mbuf *out, sj_conf_emit_cb_t cb, void *cb_param);
bool sj_conf_emit_f(const void *cfg, const void *base,
                    const struct sj_conf_entry *schema, bool pretty,
                    const char *fname);

/*
 * Frees any resources allocated in 'cfg'.
 */
void sj_conf_free(const struct sj_conf_entry *schema, void *cfg);

const struct sj_conf_entry *sj_conf_find_schema_entry(
    const char *path, const struct sj_conf_entry *obj);

void sj_conf_set_str(char **vp, const char *v);

#endif /* CS_FW_SRC_SJ_CONFIG_H_ */

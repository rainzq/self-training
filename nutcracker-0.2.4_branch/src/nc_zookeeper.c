/*
 * twemproxy - A fast and lightweight proxy for memcached protocol.
 * Copyright (C) 2011 Twitter, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sys/un.h>

#include <nc_core.h>
#include <nc_server.h>
#include <nc_string.h>
#include <nc_zookeeper.h>
#include "c-client-src/zookeeper.h"

#define PATH_LEN 512

static zhandle_t *zh = NULL;
static int zookeeper_connected_flag = 0;

static void
conn_callback(zhandle_t *zzh, int type, int state, const char *path, void *ctx)
{
    if (state == ZOO_CONNECTED_STATE) {
        zookeeper_connected_flag = 1;
    }
}

static zhandle_t *
nc_zoo_connect(struct string addrstr)
{
    int times = 10;
    uint32_t timeout = 1; // seconds
    int i;
    for (i = 0; i < times; ++i) {
        zh = zookeeper_init((const char *)addrstr.data, conn_callback, 1000, 0, 0, 0);
        if (!zh) {
            log_error("zookeeper init error at %.*s", addrstr.len, addrstr.data);
            return NULL;
        }
        sleep(1); // wait for the callback to set zookeeper_connected_flag
        if (zookeeper_connected_flag) { // connected
            return zh;
        } else {
            ++timeout;
            zookeeper_close(zh);
            zookeeper_connected_flag = 0;
            zh = NULL;
        }
    }
    log_error("connection to zookeeper timeout at %.*s", addrstr.len, addrstr.data);
    return NULL;
}

static rstatus_t 
register_to_zookeeper(struct string addrstr, struct string node, struct string pool_addrstr)
{
    int rc;
    char ephemeral_path[PATH_LEN];
    if (!zh) {
        zh = nc_zoo_connect(addrstr);
        if (!zh) {
            log_error("cann't connect to zookeeper: %.*s",
                       addrstr.len, addrstr.data);
            return NC_ERROR;
        }
    }
    rc = zoo_exists(zh, (const char *)node.data, 0, NULL);
    if (rc == ZNONODE) {
        rc = zoo_create(zh, (const char *)node.data, NULL, -1, &ZOO_OPEN_ACL_UNSAFE, 0, NULL, 0);
        if (rc != ZOK) {
            log_error("zookeeper create node %.*s failed",
                       node.len, node.data);
            return NC_ERROR;
        }
    } else if (rc != ZOK) {
        log_error("zoo_exists return error: %d", rc);
        return NC_ERROR;
    }

    // zoo path directory is created
    // we create server pool node ip:port as ephemeral node
    ASSERT(PATH_LEN > node.len + 1 + pool_addrstr.len);
    strncpy(ephemeral_path, (const char *)node.data, node.len);
    ephemeral_path[node.len] = '\0';
    strncat(ephemeral_path, "/", 1);
    strncat(ephemeral_path, (const char *)pool_addrstr.data, pool_addrstr.len);

    log_debug(LOG_NOTICE, "zookeeper ephemeral node path: %s", ephemeral_path);

    rc = zoo_create(zh, ephemeral_path, NULL, -1, &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0);
    if (rc != ZOK) {
        log_error("zookeeper create ephemeral node: %s failed", ephemeral_path);
        return NC_ERROR;
    }

    return NC_OK;
}

rstatus_t
server_pool_each_register_to_zookeeper(void *elem, void *data)
{
    rstatus_t status;
    struct server_pool *pool = elem;

    if (pool->zk_addrstr.len > 0 && pool->zk_node.len > 0) {
        status = register_to_zookeeper(pool->zk_addrstr, pool->zk_node, pool->addrstr);
        if (status != NC_OK) {
            log_error("register to zookeeper failed: '%.*s %.*s' "
                      "'%.*s' in %s pool %"PRIu32" '%.*s'"
                      " with %"PRIu32" servers", 
                      pool->zk_addrstr.len, pool->zk_addrstr.data, 
                      pool->zk_node.len, pool->zk_node.data, 
                      pool->addrstr.len, pool->addrstr.data, 
                      pool->redis ? "redis" : "memcache",
                      pool->idx, pool->name.len, pool->name.data,
                      array_n(&pool->server));
            return status;
        }
    } else {
        log_debug(LOG_NOTICE, "zookeeper address is empty: '%.*s %.*s' "
                "'%.*s' in %s pool %"PRIu32" '%.*s'"
                " with %"PRIu32" servers", 
                pool->zk_addrstr.len, pool->zk_addrstr.data, 
                pool->zk_node.len, pool->zk_node.data, 
                pool->addrstr.len, pool->addrstr.data, 
                pool->redis ? "redis" : "memcache",
                pool->idx, pool->name.len, pool->name.data,
                array_n(&pool->server));

        return NC_OK;
    }

    log_debug(LOG_NOTICE, "register to zookeeper success: '%.*s %.*s' "
                          "'%.*s' in %s pool %"PRIu32" '%.*s'"
                          " with %"PRIu32" servers", 
                          pool->zk_addrstr.len, pool->zk_addrstr.data, 
                          pool->zk_node.len, pool->zk_node.data, 
                          pool->addrstr.len, pool->addrstr.data, 
                          pool->redis ? "redis" : "memcache",
                          pool->idx, pool->name.len, pool->name.data,
                          array_n(&pool->server));

    return NC_OK;
}

rstatus_t
server_pool_register_to_zookeeper(struct context *ctx)
{
    rstatus_t status;

    ASSERT(array_n(&ctx->pool) != 0);

    status = array_each(&ctx->pool, server_pool_each_register_to_zookeeper, NULL);
    if (status != NC_OK) {
        return status;
    }

    log_debug(LOG_VVERB, "init proxy with %"PRIu32" pools",
              array_n(&ctx->pool));

    return NC_OK;
}

rstatus_t
zookeeper_deinit(void)
{
    if (!zh) {
        zookeeper_close(zh);
        zh = 0;
    }
    zookeeper_connected_flag = 0;

    return NC_OK;
}

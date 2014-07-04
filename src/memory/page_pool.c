/*!The Treasure Box Library
 * 
 * TBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * TBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with TBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2009 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        page_pool.c
 * @ingroup     memory
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "page_pool"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "page_pool.h"
#include "impl/static_page_pool.h"
#include "impl/native_page_pool.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// is native pool?
#define tb_page_pool_is_native(pool)        (((tb_size_t)(pool)) & 0x1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_page_pool_ref_t tb_page_pool_init(tb_byte_t* data, tb_size_t size)
{
    // init pool
    return (data && size)? tb_static_page_pool_init(data, size) : tb_native_page_pool_init();
}
tb_void_t tb_page_pool_exit(tb_page_pool_ref_t pool)
{
    // check
    tb_assert_and_check_return(pool);

    // exit pool
    if (tb_page_pool_is_native(pool)) tb_native_page_pool_exit(pool);
    else tb_static_page_pool_exit(pool);
}
tb_void_t tb_page_pool_clear(tb_page_pool_ref_t pool)
{
    // check
    tb_assert_and_check_return(pool);

    // clear pool
    if (tb_page_pool_is_native(pool)) tb_native_page_pool_clear(pool);
    else tb_static_page_pool_clear(pool);
}
tb_pointer_t tb_page_pool_malloc_(tb_page_pool_ref_t pool, tb_size_t size __tb_debug_decl__)
{
    // check
    tb_assert_and_check_return_val(pool && size, tb_null);

    // the page size
    tb_size_t pagesize = tb_page_size();
    tb_assert_and_check_return_val(pagesize, tb_null);

    // check size
    tb_assert_and_check_return_val(!(size & (pagesize - 1)), tb_null);

    // malloc data
    tb_pointer_t data = tb_page_pool_is_native(pool)? tb_native_page_pool_malloc(pool, size __tb_debug_args__) : tb_static_page_pool_malloc(pool, size __tb_debug_args__);
    tb_assertf_and_check_return_val(data, tb_null, "malloc(%lu) failed!", size);

    // check data
    tb_assert_and_check_return_val(!(((tb_size_t)data) & (pagesize - 1)), tb_null);

    // ok
    return data;
}
tb_pointer_t tb_page_pool_malloc0_(tb_page_pool_ref_t pool, tb_size_t size __tb_debug_decl__)
{
    // check
    tb_assert_and_check_return_val(pool && size, tb_null);

    // the page size
    tb_size_t pagesize = tb_page_size();
    tb_assert_and_check_return_val(pagesize, tb_null);

    // check size
    tb_assert_and_check_return_val(!(size & (pagesize - 1)), tb_null);

    // malloc0 data
    tb_pointer_t data = tb_page_pool_is_native(pool)? tb_native_page_pool_malloc(pool, size __tb_debug_args__) : tb_static_page_pool_malloc(pool, size __tb_debug_args__);
    tb_assertf_and_check_return_val(data, tb_null, "malloc0(%lu) failed!", size);

    // check data
    tb_assert_and_check_return_val(!(((tb_size_t)data) & (pagesize - 1)), tb_null);

    // clear it
    tb_memset(data, 0, size);

    // ok
    return data;
}
tb_pointer_t tb_page_pool_nalloc_(tb_page_pool_ref_t pool, tb_size_t item, tb_size_t size __tb_debug_decl__)
{
    // check
    tb_assert_and_check_return_val(pool && size, tb_null);

    // the page size
    tb_size_t pagesize = tb_page_size();
    tb_assert_and_check_return_val(pagesize, tb_null);

    // check size
    tb_assert_and_check_return_val(!(size & (pagesize - 1)), tb_null);

    // nalloc data
    tb_pointer_t data = tb_page_pool_is_native(pool)? tb_native_page_pool_malloc(pool, item * size __tb_debug_args__) : tb_static_page_pool_malloc(pool, item * size __tb_debug_args__);
    tb_assertf_and_check_return_val(data, tb_null, "nalloc(%lu, %lu) failed!", item, size);

    // check data
    tb_assert_and_check_return_val(!(((tb_size_t)data) & (pagesize - 1)), tb_null);

    // ok
    return data;
}
tb_pointer_t tb_page_pool_nalloc0_(tb_page_pool_ref_t pool, tb_size_t item, tb_size_t size __tb_debug_decl__)
{
    // check
    tb_assert_and_check_return_val(pool && size, tb_null);

    // the page size
    tb_size_t pagesize = tb_page_size();
    tb_assert_and_check_return_val(pagesize, tb_null);

    // check size
    tb_assert_and_check_return_val(!(size & (pagesize - 1)), tb_null);

    // nalloc0 data
    tb_pointer_t data = tb_page_pool_is_native(pool)? tb_native_page_pool_malloc(pool, item * size __tb_debug_args__) : tb_static_page_pool_malloc(pool, item * size __tb_debug_args__);
    tb_assertf_and_check_return_val(data, tb_null, "nalloc0(%lu, %lu) failed!", item, size);

    // check data
    tb_assert_and_check_return_val(!(((tb_size_t)data) & (pagesize - 1)), tb_null);

    // clear it
    tb_memset(data, 0, item * size);

    // ok
    return data;
}
tb_pointer_t tb_page_pool_ralloc_(tb_page_pool_ref_t pool, tb_pointer_t data, tb_size_t size __tb_debug_decl__)
{
    // check
    tb_assert_and_check_return_val(pool && size, tb_null);

    // the page size
    tb_size_t pagesize = tb_page_size();
    tb_assert_and_check_return_val(pagesize, tb_null);

    // check size
    tb_assert_and_check_return_val(!(size & (pagesize - 1)), tb_null);

    // check data
    tb_assert_and_check_return_val(!(((tb_size_t)data) & (pagesize - 1)), tb_null);

    // ralloc data
    tb_pointer_t p = tb_page_pool_is_native(pool)? tb_native_page_pool_ralloc(pool, data, size __tb_debug_args__) : tb_static_page_pool_ralloc(pool, data, size __tb_debug_args__);
    tb_assertf_and_check_return_val(p, tb_null, "ralloc(%p, %lu) failed!", data, size);

    // check data
    tb_assert_and_check_return_val(!(((tb_size_t)p) & (pagesize - 1)), tb_null);

    // ok
    return p;
}
tb_bool_t tb_page_pool_free_(tb_page_pool_ref_t pool, tb_pointer_t data __tb_debug_decl__)
{
    // check
    tb_assert_and_check_return_val(pool && data, tb_false);

    // the page size
    tb_size_t pagesize = tb_page_size();
    tb_assert_and_check_return_val(pagesize, tb_false);

    // check data
    tb_assert_and_check_return_val(!(((tb_size_t)data) & (pagesize - 1)), tb_false);

    // free data
    tb_bool_t ok = tb_page_pool_is_native(pool)? tb_native_page_pool_free(pool, data __tb_debug_args__) : tb_static_page_pool_free(pool, data __tb_debug_args__);
    tb_assertf_and_check_return_val(ok, tb_false, "free(%p) failed!", data);

    // ok
    return ok;
}
#ifdef __tb_debug__
tb_size_t tb_page_pool_data_size(tb_page_pool_ref_t pool, tb_cpointer_t data)
{
    // check
    tb_assert_and_check_return_val(pool && data, 0);

    // the data size
    return tb_page_pool_is_native(pool)? tb_native_page_pool_data_size(pool, data) : tb_static_page_pool_data_size(pool, data);
}
tb_void_t tb_page_pool_data_dump(tb_page_pool_ref_t pool, tb_cpointer_t data, tb_char_t const* prefix)
{
    // check
    tb_assert_and_check_return(pool && data);

    // dump the data size
    return tb_page_pool_is_native(pool)? tb_native_page_pool_data_dump(pool, data, prefix) : tb_static_page_pool_data_dump(pool, data, prefix);
}
tb_void_t tb_page_pool_dump(tb_page_pool_ref_t pool)
{
    // check
    tb_assert_and_check_return(pool);

    // dump the pool
    return tb_page_pool_is_native(pool)? tb_native_page_pool_dump(pool) : tb_static_page_pool_dump(pool);
}
#endif
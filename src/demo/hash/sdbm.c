/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t tb_demo_hash_sdbm_main(tb_int_t argc, tb_char_t** argv)
{
    // trace
    tb_trace_i("[sdbm]: %lx", tb_sdbm_make_from_cstr(argv[1], 0));
    return 0;
}
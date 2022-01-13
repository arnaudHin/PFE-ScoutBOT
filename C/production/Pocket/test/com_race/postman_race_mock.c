/**
 * @file  race_protocol_mock.c
 * @author Brient Nathan
 * @version 1.0
 * @date 
 * @brief mock fonction og race_protocol
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stddef.h> 
#include <stdint.h> 
#include <sys/socket.h>
#include <string.h>
#include "cmocka.h"

int __wrap_write(int __fd, const void *__buf, size_t __n)
{
    function_called();
    check_expected(__fd);
    check_expected(__buf);
    check_expected(__n);
    return (int) mock();

}

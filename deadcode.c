#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "stdlib.h"
#include "php.h"
#include "php_ini.h"
#include "zend_extensions.h"
#include "php_deadcode.h"
#include "tools.h"

zend_module_entry deadcode_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
    PHP_DEADCODE_EXTNAME,
    NULL,
    NULL,
    PHP_MSHUTDOWN(deadcode),
    PHP_RINIT(deadcode),
    NULL,
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_DEADCODE_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_DEADCODE
ZEND_GET_MODULE(deadcode)
#endif

PHP_RINIT_FUNCTION(deadcode)
{
	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_DO_FCALL);
	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_DO_FCALL_BY_NAME);
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(deadcode)
{
	return SUCCESS;
}

int deadcode_common_override_handler(ZEND_OPCODE_HANDLER_ARGS)
{
	mark_line("deadcode_common_override_handler", execute_data);
	return ZEND_USER_OPCODE_DISPATCH;
}


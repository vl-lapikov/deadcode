#ifndef PHP_DEADCODE_H
#define PHP_DEADCODE_H 1
#ifdef ZTS
#include "TSRM.h"
#endif

#define PHP_DEADCODE_VERSION "1.0"
#define PHP_DEADCODE_EXTNAME "deadcode"

#define DEADCODE_NAME       "Dead code"
#define DEADCODE_VERSION    "0.1"
#define DEADCODE_AUTHOR     "Vladimir Lapikov"
#define DEADCODE_COPYRIGHT  "Copyright (c) 2002-2014 by Vladimir Lapikov"
#define DEADCODE_COPYRIGHT_SHORT "Copyright (c) 2002-2014"
#define DEADCODE_URL        "https://github.com/"
#define DEADCODE_URL_FAQ    "https://github.com/"


#ifdef TEMP
#include "Zend/zend_globals_macros.h"
#include "Zend/zend_API.h"
#endif

int deadcode_common_override_handler(ZEND_OPCODE_HANDLER_ARGS);

/* Needed for code coverage as Zend doesn't always add EXT_STMT when expected */
#define DEADCODE_SET_OPCODE_OVERRIDE_COMMON(oc) \
	zend_set_user_opcode_handler(oc, deadcode_common_override_handler);

PHP_MSHUTDOWN_FUNCTION(deadcode);
PHP_RINIT_FUNCTION(deadcode);

extern zend_module_entry deadcode_module_entry;
#define phpext_deadcode_ptr &deadcode_module_entry

#endif

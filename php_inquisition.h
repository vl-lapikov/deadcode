#ifndef PHP_INQUISITION_H
#define PHP_INQUISITION_H 1
#ifdef ZTS
#include "TSRM.h"
#endif

#define PHP_INQUISITION_VERSION "1.0"
#define PHP_INQUISITION_EXTNAME "inquisition"

#define INQUISITION_NAME       "Dead code"
#define INQUISITION_VERSION    "0.1"
#define INQUISITION_AUTHOR     "Vladimir Lapikov"
#define INQUISITION_COPYRIGHT  "Copyright (c) 2002-2014 by Vladimir Lapikov"
#define INQUISITION_COPYRIGHT_SHORT "Copyright (c) 2002-2014"
#define INQUISITION_URL        "https://github.com/"
#define INQUISITION_URL_FAQ    "https://github.com/"


#ifdef TEMP
#include "Zend/zend_globals_macros.h"
#include "Zend/zend_API.h"
#endif

/* execution redirection functions */
zend_op_array* (*old_compile_file)(zend_file_handle* file_handle, int type TSRMLS_DC);
zend_op_array* inquisition_compile_file(zend_file_handle*, int TSRMLS_DC);

zend_op_array* (*old_compile_string)(zend_file_handle* file_handle, int type TSRMLS_DC);
zend_op_array* inquisition_compile_string(zend_file_handle*, int TSRMLS_DC);

#if PHP_VERSION_ID < 50500
void (*inquisition_old_execute)(zend_op_array *op_array TSRMLS_DC);
void inquisition_execute(zend_op_array *op_array TSRMLS_DC);

void (*inquisition_old_execute_internal)(zend_execute_data *current_execute_data, int return_value_used TSRMLS_DC);
void inquisition_execute_internal(zend_execute_data *current_execute_data, int return_value_used TSRMLS_DC);
#else
void (*inquisition_old_execute_ex)(zend_execute_data *execute_data TSRMLS_DC);
void inquisition_execute_ex(zend_execute_data *execute_data TSRMLS_DC);

void (*inquisition_old_execute_internal)(zend_execute_data *current_execute_data, struct _zend_fcall_info *fci, int return_value_used TSRMLS_DC);
void inquisition_execute_internal(zend_execute_data *current_execute_data, struct _zend_fcall_info *fci, int return_value_used TSRMLS_DC);
#endif

int inquisition_common_override_handler(ZEND_OPCODE_HANDLER_ARGS);

/* Needed for code coverage as Zend doesn't always add EXT_STMT when expected */
#define INQUISITION_SET_OPCODE_OVERRIDE_COMMON(oc) \
	zend_set_user_opcode_handler(oc, inquisition_common_override_handler);

PHP_MSHUTDOWN_FUNCTION(inquisition);
PHP_RINIT_FUNCTION(inquisition);

extern zend_module_entry inquisition_module_entry;
#define phpext_inquisition_ptr &inquisition_module_entry

#endif

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
	/* Redirect compile and execute functions to our own */
	old_compile_file = zend_compile_file;
	zend_compile_file = deadcode_compile_file;

#if PHP_VERSION_ID < 50500
	deadcode_old_execute = zend_execute;
	zend_execute = deadcode_execute;
#else
	deadcode_old_execute_ex = zend_execute_ex;
	zend_execute_ex = deadcode_execute_ex;
#endif

	deadcode_old_execute_internal = zend_execute_internal;
	zend_execute_internal = deadcode_execute_internal;

	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_NEW);

//	/* Overload opcodes for code coverage */
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_JMP);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_JMPZ);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_JMPZ_EX);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_JMPNZ);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_IS_IDENTICAL);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_IS_NOT_IDENTICAL);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_IS_EQUAL);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_IS_NOT_EQUAL);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_IS_SMALLER);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_IS_SMALLER_OR_EQUAL);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_BOOL_NOT);
//
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_ADD);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_SUB);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_MUL);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_DIV);
//
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_ADD_ARRAY_ELEMENT);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_RETURN);
//#if PHP_VERSION_ID >= 50400
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_RETURN_BY_REF);
//#endif
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_EXT_STMT);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_RAISE_ABSTRACT_ERROR);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_SEND_VAR);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_SEND_VAR_NO_REF);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_SEND_VAL);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_NEW);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_EXT_FCALL_BEGIN);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_CATCH);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_BOOL);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_ADD_CHAR);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_ADD_STRING);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_INIT_ARRAY);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_DIM_R);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_OBJ_R);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_OBJ_W);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_OBJ_FUNC_ARG);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_DIM_FUNC_ARG);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_DIM_UNSET);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_OBJ_UNSET);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_CLASS);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_CONSTANT);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_CONCAT);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_ISSET_ISEMPTY_DIM_OBJ);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_PRE_INC_OBJ);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_SWITCH_FREE);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_QM_ASSIGN);
//#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 3) || PHP_MAJOR_VERSION >= 6
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_DECLARE_LAMBDA_FUNCTION);
//#endif
//#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 4)
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_ADD_TRAIT);
//	DEADCODE_SET_OPCODE_OVERRIDE_COMMON(ZEND_BIND_TRAITS);
//#endif

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(deadcode)
{
	/* Reset compile, execute and error callbacks */
	zend_compile_file = old_compile_file;

	return SUCCESS;
}

/* {{{ zend_op_array srm_compile_file (file_handle, type)
 *    This function provides a hook for the execution of bananas */
zend_op_array *deadcode_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC)
{
	zend_op_array *op_array;

	op_array = old_compile_file(file_handle, type TSRMLS_CC);

	return op_array;
}
/* }}} */

#if PHP_VERSION_ID < 50500
void deadcode_execute(zend_op_array *op_array TSRMLS_DC)
{
	zend_execute_data    *edata = EG(current_execute_data);
#else
void deadcode_execute_ex(zend_execute_data *execute_data TSRMLS_DC)
{
	zend_op_array        *op_array = execute_data->op_array;
	zend_execute_data    *edata = execute_data->prev_execute_data;
#endif

	mark_line("deadcode_execute_ex", execute_data);

#if PHP_VERSION_ID < 50500
	deadcode_old_execute(op_array TSRMLS_CC);
#else
	deadcode_old_execute_ex(execute_data TSRMLS_CC);
#endif
	return;
}

#if PHP_VERSION_ID < 50500
void deadcode_execute_internal(zend_execute_data *current_execute_data, int return_value_used TSRMLS_DC)
#else
void deadcode_execute_internal(zend_execute_data *current_execute_data, struct _zend_fcall_info *fci, int return_value_used TSRMLS_DC)
#endif
{
	zend_execute_data    *edata = EG(current_execute_data);
	zend_op              *cur_opcode;

	zend_op_array        *op_array = edata->op_array;

#if PHP_VERSION_ID < 50500
	if (deadcode_old_execute_internal) {
		deadcode_old_execute_internal(current_execute_data, return_value_used TSRMLS_CC);
	} else {
		execute_internal(current_execute_data, return_value_used TSRMLS_CC);
	}
#else
	if (deadcode_old_execute_internal) {
		deadcode_old_execute_internal(current_execute_data, fci, return_value_used TSRMLS_CC);
	} else {
		execute_internal(current_execute_data, fci, return_value_used TSRMLS_CC);
	}
#endif
}

int deadcode_common_override_handler(ZEND_OPCODE_HANDLER_ARGS)
{
	mark_line("deadcode_common_override_handler", execute_data);

//	if (XG(do_code_coverage)) {
//		zend_op *cur_opcode;
//		int      lineno;
//		char    *file;
//
//		zend_op_array *op_array = execute_data->op_array;
//
//		cur_opcode = *EG(opline_ptr);
//		lineno = cur_opcode->lineno;
//
//		file = (char *)op_array->filename;
//
//		deadcode_count_line(file, lineno, 0, 0 TSRMLS_CC);
//	}
	return ZEND_USER_OPCODE_DISPATCH;
}

ZEND_DLEXPORT void deadcode_statement_call(zend_op_array *op_array)
{
	TSRMLS_FETCH();

	zend_printf("deadcode_statement_call!");

	if (!EG(current_execute_data)) {
		return;
	}

	mark_line("deadcode_statement_call", EG(current_execute_data));
}

#ifndef ZEND_EXT_API
#define ZEND_EXT_API    ZEND_DLEXPORT
#endif
ZEND_EXTENSION();

ZEND_DLEXPORT zend_extension zend_extension_entry = {
	DEADCODE_NAME,
	DEADCODE_VERSION,
	DEADCODE_AUTHOR,
	DEADCODE_URL_FAQ,
	DEADCODE_COPYRIGHT_SHORT,
	NULL,
	NULL,
	NULL,           /* activate_func_t */
	NULL,           /* deactivate_func_t */
	NULL,           /* message_handler_func_t */
	NULL,           /* op_array_handler_func_t */
	deadcode_statement_call, /* statement_handler_func_t */
	NULL,           /* fcall_begin_handler_func_t */
	NULL,           /* fcall_end_handler_func_t */
	NULL,   /* op_array_ctor_func_t */
	NULL,           /* op_array_dtor_func_t */
	STANDARD_ZEND_EXTENSION_PROPERTIES
};


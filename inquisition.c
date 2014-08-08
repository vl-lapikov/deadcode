#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "stdlib.h"
#include "php.h"
#include "php_ini.h"
#include "zend_extensions.h"
#include "php_inquisition.h"
#include "tools.h"

zend_module_entry inquisition_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_INQUISITION_EXTNAME,
    NULL,
    NULL,
    PHP_MSHUTDOWN(inquisition),
    PHP_RINIT(inquisition),
    NULL,
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_INQUISITION_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_INQUISITION
ZEND_GET_MODULE(inquisition)
#endif

PHP_RINIT_FUNCTION(inquisition)
{
	/* Redirect compile and execute functions to our own */
	old_compile_file = zend_compile_file;
	zend_compile_file = inquisition_compile_file;

#if PHP_VERSION_ID < 50500
	inquisition_old_execute = zend_execute;
	zend_execute = inquisition_execute;
#else
	inquisition_old_execute_ex = zend_execute_ex;
	zend_execute_ex = inquisition_execute_ex;
#endif

	inquisition_old_execute_internal = zend_execute_internal;
	zend_execute_internal = inquisition_execute_internal;

	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_NEW);

//	/* Overload opcodes for code coverage */
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_JMP);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_JMPZ);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_JMPZ_EX);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_JMPNZ);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_IS_IDENTICAL);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_IS_NOT_IDENTICAL);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_IS_EQUAL);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_IS_NOT_EQUAL);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_IS_SMALLER);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_IS_SMALLER_OR_EQUAL);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_BOOL_NOT);
//
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_ADD);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_SUB);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_MUL);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_DIV);
//
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_ADD_ARRAY_ELEMENT);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_RETURN);
//#if PHP_VERSION_ID >= 50400
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_RETURN_BY_REF);
//#endif
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_EXT_STMT);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_RAISE_ABSTRACT_ERROR);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_SEND_VAR);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_SEND_VAR_NO_REF);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_SEND_VAL);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_NEW);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_EXT_FCALL_BEGIN);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_CATCH);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_BOOL);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_ADD_CHAR);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_ADD_STRING);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_INIT_ARRAY);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_DIM_R);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_OBJ_R);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_OBJ_W);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_OBJ_FUNC_ARG);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_DIM_FUNC_ARG);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_DIM_UNSET);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_OBJ_UNSET);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_CLASS);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_FETCH_CONSTANT);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_CONCAT);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_ISSET_ISEMPTY_DIM_OBJ);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_PRE_INC_OBJ);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_SWITCH_FREE);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_QM_ASSIGN);
//#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 3) || PHP_MAJOR_VERSION >= 6
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_DECLARE_LAMBDA_FUNCTION);
//#endif
//#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 4)
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_ADD_TRAIT);
//	INQUISITION_SET_OPCODE_OVERRIDE_COMMON(ZEND_BIND_TRAITS);
//#endif

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(inquisition)
{
	/* Reset compile, execute and error callbacks */
	zend_compile_file = old_compile_file;

	return SUCCESS;
}

/* {{{ zend_op_array srm_compile_file (file_handle, type)
 *    This function provides a hook for the execution of bananas */
zend_op_array *inquisition_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC)
{
	zend_op_array *op_array;

	op_array = old_compile_file(file_handle, type TSRMLS_CC);

	return op_array;
}
/* }}} */

#if PHP_VERSION_ID < 50500
void inquisition_execute(zend_op_array *op_array TSRMLS_DC)
{
	zend_execute_data    *edata = EG(current_execute_data);
#else
void inquisition_execute_ex(zend_execute_data *execute_data TSRMLS_DC)
{
	zend_op_array        *op_array = execute_data->op_array;
	zend_execute_data    *edata = execute_data->prev_execute_data;
#endif

	mark_line("inquisition_execute_ex", execute_data);

#if PHP_VERSION_ID < 50500
	inquisition_old_execute(op_array TSRMLS_CC);
#else
	inquisition_old_execute_ex(execute_data TSRMLS_CC);
#endif
	return;
}

#if PHP_VERSION_ID < 50500
void inquisition_execute_internal(zend_execute_data *current_execute_data, int return_value_used TSRMLS_DC)
#else
void inquisition_execute_internal(zend_execute_data *current_execute_data, struct _zend_fcall_info *fci, int return_value_used TSRMLS_DC)
#endif
{
	zend_execute_data    *edata = EG(current_execute_data);
	zend_op              *cur_opcode;

	zend_op_array        *op_array = edata->op_array;

#if PHP_VERSION_ID < 50500
	if (inquisition_old_execute_internal) {
		inquisition_old_execute_internal(current_execute_data, return_value_used TSRMLS_CC);
	} else {
		execute_internal(current_execute_data, return_value_used TSRMLS_CC);
	}
#else
	if (inquisition_old_execute_internal) {
		inquisition_old_execute_internal(current_execute_data, fci, return_value_used TSRMLS_CC);
	} else {
		execute_internal(current_execute_data, fci, return_value_used TSRMLS_CC);
	}
#endif
}

int inquisition_common_override_handler(ZEND_OPCODE_HANDLER_ARGS)
{
	mark_line("inquisition_common_override_handler", execute_data);

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
//		inquisition_count_line(file, lineno, 0, 0 TSRMLS_CC);
//	}
	return ZEND_USER_OPCODE_DISPATCH;
}

ZEND_DLEXPORT void inquisition_statement_call(zend_op_array *op_array)
{
	TSRMLS_FETCH();

	zend_printf("inquisition_statement_call!");

	if (!EG(current_execute_data)) {
		return;
	}

	mark_line("inquisition_statement_call", EG(current_execute_data));
}

#ifndef ZEND_EXT_API
#define ZEND_EXT_API    ZEND_DLEXPORT
#endif
ZEND_EXTENSION();

ZEND_DLEXPORT zend_extension zend_extension_entry = {
	INQUISITION_NAME,
	INQUISITION_VERSION,
	INQUISITION_AUTHOR,
	INQUISITION_URL_FAQ,
	INQUISITION_COPYRIGHT_SHORT,
	NULL,
	NULL,
	NULL,           /* activate_func_t */
	NULL,           /* deactivate_func_t */
	NULL,           /* message_handler_func_t */
	NULL,           /* op_array_handler_func_t */
	inquisition_statement_call, /* statement_handler_func_t */
	NULL,           /* fcall_begin_handler_func_t */
	NULL,           /* fcall_end_handler_func_t */
	NULL,   /* op_array_ctor_func_t */
	NULL,           /* op_array_dtor_func_t */
	STANDARD_ZEND_EXTENSION_PROPERTIES
};


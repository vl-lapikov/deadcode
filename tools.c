#include "zend_extensions.h"

void dump_op_array(char * type, zend_op_array *op_array )
{
	zend_printf("Where: %s\n", type);
	zend_printf("type: %X\n", op_array->type);
	if (op_array->function_name) {
		zend_printf("function_name: %s\n", op_array->function_name);
	}
	if (op_array->scope) {
		zend_printf("\tscope->type: %X\n", op_array->scope->type);
		zend_printf("\tscope->name: %s\n", op_array->scope->name);
		zend_printf("\tscope->parent: %p\n", op_array->scope->parent);
		zend_printf("\tscope->refcount: %d\n", op_array->scope->refcount);
		zend_printf("\tscope->ce_flags: %d\n", op_array->scope->ce_flags);
		zend_printf("\tscope->ce_flags: %d\n", op_array->scope->ce_flags);
	}
	if (op_array->line_start) {
		zend_printf("line_start: %d\n", op_array->line_start);
	}
	if (op_array->line_end) {
		zend_printf("line_end: %d\n", op_array->line_end);
	}
	zend_printf("filename: %s\n", op_array->filename);
	zend_printf("fn_flags: %d\n", op_array->fn_flags);
	if (op_array->prototype) {
		zend_printf("prototype: %p\n", op_array->prototype);
	}
	if (op_array->num_args)
		zend_printf("num_args: %d\n", op_array->num_args);

	if (op_array->required_num_args)
		zend_printf("required_num_args: %d\n", op_array->required_num_args);

	if (op_array->arg_info)
		zend_printf("arg_info: %p\n", op_array->arg_info);
	zend_printf("\n");
}

char* get_opcode(int opcode_number) {
	switch (opcode_number) {
		case ZEND_DO_FCALL:
			return "DO_FCALL";
		case ZEND_DO_FCALL_BY_NAME:
			return "DO_FCALL_BY_NAME";
		default:
			return "UNKNOWN";
	}
}

void mark_line(char * function, zend_execute_data *execute_data TSRMLS_DC) {
	zend_printf(
		"%s (line %d)\nOPCODE: %s\n",
		execute_data->op_array->filename,
		execute_data->opline->lineno,
		get_opcode(execute_data->opline->opcode)
	);
}


#include <ofc/sema.h>


static ofc_sema_stmt_t* ofc_sema_stmt_simple(
	ofc_parse_stmt_e type)
{
	ofc_sema_stmt_e st;
	switch (type)
	{
		case OFC_PARSE_STMT_CONTINUE:
			st = OFC_SEMA_STMT_CONTINUE;
			break;

		default:
			return NULL;
	}

	ofc_sema_stmt_t* stmt
		= (ofc_sema_stmt_t*)malloc(
			sizeof(ofc_sema_stmt_t));
	if (!stmt) return NULL;

	stmt->type = st;
	return stmt;
}


bool ofc_sema_stmt(
	ofc_sema_scope_t* scope,
	const ofc_parse_stmt_t* stmt)
{
	if (!stmt)
		return false;

	if (stmt->label != 0)
	{
		if (ofc_sema_label_map_find(
			scope->label, stmt->label))
		{
			/* TODO - Error: Duplicate label definition. */
			return false;
		}
	}

	ofc_sema_stmt_t* s = NULL;
	switch (stmt->type)
	{
		case OFC_PARSE_STMT_ASSIGNMENT:
			s = ofc_sema_stmt_assignment(
				scope, stmt);
			break;

		case OFC_PARSE_STMT_CONTINUE:
			s = ofc_sema_stmt_simple(stmt->type);
			break;

		default:
			/* TODO - Error: Unsuported statement. */
			return false;
	}

	/* Statement analysis failed,
	   should already have printed an error. */
	if (!s) return false;

	unsigned offset
		= ofc_sema_stmt_list_count(
			scope->stmt);

	if (!ofc_sema_stmt_list_add(
		scope->stmt, s))
	{
		ofc_sema_stmt_delete(s);
		return false;
	}

	if (stmt->label != 0)
	{
		if (!ofc_sema_label_map_add_stmt(
			scope->label, stmt->label, offset))
		{
			/* This should never happen. */
			abort();
		}
	}

	return true;
}

void ofc_sema_stmt_delete(
	ofc_sema_stmt_t* stmt)
{
	if (!stmt)
		return;

	switch (stmt->type)
	{
		case OFC_SEMA_STMT_ASSIGNMENT:
			ofc_sema_expr_delete(
				stmt->assignment.expr);
			break;
		default:
			break;
	}

	free(stmt);
}



ofc_sema_stmt_list_t* ofc_sema_stmt_list_create(void)
{
	ofc_sema_stmt_list_t* list
		= (ofc_sema_stmt_list_t*)malloc(
			sizeof(ofc_sema_stmt_list_t));
	if (!list) return NULL;

	list->count = 0;
	list->stmt  = NULL;
	return list;
}

ofc_sema_stmt_t* ofc_sema_stmt_alloc(
	ofc_sema_stmt_t stmt)
{
	ofc_sema_stmt_t* astmt
		= (ofc_sema_stmt_t*)malloc(
			sizeof(ofc_sema_stmt_t));
	if (!astmt) return NULL;

	*astmt = stmt;
	return astmt;
}

void ofc_sema_stmt_list_delete(
	ofc_sema_stmt_list_t* list)
{
	if (!list)
		return;

	unsigned i;
	for (i = 0; i < list->count; i++)
		ofc_sema_stmt_delete(list->stmt[i]);
	free(list->stmt);

	free(list);
}

bool ofc_sema_stmt_list_add(
	ofc_sema_stmt_list_t* list,
	ofc_sema_stmt_t* stmt)
{
	if (!list || !stmt)
		return false;

	ofc_sema_stmt_t** nstmt
		= (ofc_sema_stmt_t**)realloc(list->stmt,
			(sizeof(ofc_sema_stmt_t*) * (list->count + 1)));
	if (!nstmt) return NULL;

	list->stmt = nstmt;
	list->stmt[list->count++] = stmt;
	return true;
}

unsigned ofc_sema_stmt_list_count(
	const ofc_sema_stmt_list_t* list)
{
	return (list ? list->count : 0);
}
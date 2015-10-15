#ifndef __ofc_sema_type_h__
#define __ofc_sema_type_h__

typedef enum
{
	OFC_SEMA_TYPE_LOGICAL,
	OFC_SEMA_TYPE_INTEGER,
	OFC_SEMA_TYPE_REAL,
	OFC_SEMA_TYPE_COMPLEX,
	OFC_SEMA_TYPE_BYTE,
	OFC_SEMA_TYPE_STRUCTURE,
	OFC_SEMA_TYPE_POINTER,
	OFC_SEMA_TYPE_ARRAY,
} ofc_sema_type_e;

struct ofc_sema_type_s
{
	ofc_sema_type_e type;

    bool is_static;
	bool is_volatile;
	bool is_automatic;

	union
	{
		struct
		{
			ofc_sema_array_t* array;
			const ofc_sema_type_t* subtype;
		};

		unsigned kind;

		const ofc_sema_structure_t* structure;
	};
};

const ofc_sema_type_t* ofc_sema_type_create_primitive(
	ofc_sema_type_e type, unsigned kind,
	bool is_static, bool is_automatic, bool is_volatile);
const ofc_sema_type_t* ofc_sema_type_create_byte(
	bool is_static, bool is_automatic, bool is_volatile);
const ofc_sema_type_t* ofc_sema_type_create_structure(
	const ofc_sema_structure_t* structure,
	bool is_static, bool is_automatic, bool is_volatile);
const ofc_sema_type_t* ofc_sema_type_create_pointer(
	ofc_sema_type_t* target,
	bool is_static, bool is_automatic, bool is_volatile);
const ofc_sema_type_t* ofc_sema_type_create_array(
	ofc_sema_type_t* type, ofc_sema_array_t* array,
	bool is_static, bool is_automatic, bool is_volatile);

uint8_t ofc_sema_type_hash(
	const ofc_sema_type_t* type);

bool ofc_sema_type_compare(
	const ofc_sema_type_t* a,
	const ofc_sema_type_t* b);

unsigned ofc_sema_type_size(const ofc_sema_type_t* type);

bool ofc_sema_type_is_integer(const ofc_sema_type_t* type);
bool ofc_sema_type_is_logical(const ofc_sema_type_t* type);

#endif
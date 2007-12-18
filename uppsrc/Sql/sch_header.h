// struct

#define CODETYPE(Table, b) \
struct S_##Table b { \
public:\
	void Shrink(); \
	void Clear(); \
	static const char         TableName[]; \
	static const SqlSet&      ColumnSet(); \
	static SqlSet             ColumnSet(const String& prefix); \
	static SqlSet             Of(SqlId table); \
	void FieldLayoutRaw(FieldOperator& f, const String& prefix = String()); \
	void FieldLayout(FieldOperator& f); \
	operator Fields()              { return callback(this, &S_##Table::FieldLayout); } \
	S_##Table();

#define TYPE(Table)                  CODETYPE(Table, __NIL)
#define TYPE_I(Table, b)             CODETYPE(Table, : public S_##b)
#define TWO_BASES(b1, b2)            : public S_##b1, public S_##b2
#define TYPE_II(Table, b1, b2)       CODETYPE(Table, TWO_BASES(b1, b2));
#define THREE_BASES(b1, b2, b3)      : public S_##b1, public S_##b2, public S_##b3
#define TYPE_III(Table, b1, b2, b3)  CODETYPE(Table, THREE_BASES(b1, b2, b3));

#define VAR(type, x)             S_##type x;

#define COLUMN(type, ctype, name, width, prec) \
enum { name##_WIDTH = width, name##_PRECISION = prec }; ctype name;

#define COLUMN_ARRAY(type, ctype, name, width, prec, items) \
enum { name##_WIDTH = width, name##_PRECISION = prec }; ctype name[items];

#define END_TYPE                 };

#include SCHEMADIALECT

#undef CODETYPE
#undef TWO_BASES
#undef THREE_BASES

// SqlId

#define DOID(x)                  extern SqlId x;

#include SCHEMADIALECT

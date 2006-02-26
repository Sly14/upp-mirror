#define INT(x)                     COLUMN("integer", int, x, 0, 0)
#define INT_ARRAY(x, items)        COLUMN_ARRAY("integer", int, x, 0, 0, items)
#define INT_(x)                    COLUMN_("integer", int, x, 0, 0)
#define INT_ARRAY_(x, items)       COLUMN_ARRAY_("integer", int, x, 0, 0, items)

#define DOUBLE(x)                  COLUMN("real", double, x, 0, 0)
#define DOUBLE_ARRAY(x, items)     COLUMN_ARRAY("real", double, x, 0, 0, items)
#define DOUBLE_(x)                 COLUMN_("real", double, x, 0, 0)
#define DOUBLE_ARRAY_(x, items)    COLUMN_ARRAY_("real", double, x, 0, 0, items)

#define STRING(x, n)               COLUMN("text", String, x, 0, 0)
#define STRING_ARRAY(x, n, items)  COLUMN_ARRAY("text", String, x, 0, 0, items)
#define STRING_(x, n)              COLUMN_("text", String, x, 0, 0)
#define STRING_ARRAY_(x, n, items) COLUMN_ARRAY_("text", String, x, 0, 0, items)

#define DATE(x)                    COLUMN("date", Date, x, 0, 0)
#define DATE_ARRAY(x, items)       COLUMN_ARRAY("date", Date, x, 0, 0, items)
#define DATE_(x)                   COLUMN_("date", Date, x, 0, 0)
#define DATE_ARRAY_(x, items)      COLUMN_ARRAY_("date", Date, x, 0, 0, items)

#define TIME(x)                    COLUMN("date", Time, x, 0, 0)
#define TIME_ARRAY(x, items)       COLUMN_ARRAY("date", Time, x, 0, 0, items)
#define TIME_(x)                   COLUMN_("date", Time, x, 0, 0)
#define TIME_ARRAY_(x, items)      COLUMN_ARRAY_("date", Time, x, 0, 0, items)

#define BOOL(x)                    COLUMN("text", bool, x, 0, 0)
#define BOOL_ARRAY(x, items)       COLUMN_ARRAY("text", bool, x, 0, 0, items)
#define BOOL_(x)                   COLUMN_("text", bool, x, 0, 0)
#define BOOL_ARRAY_(x, items)      COLUMN_ARRAY_("text", bool, x, 0, 0, items)

#define BLOB(x)                    COLUMN("blob", String, x, 0, 0)
#define BLOB_(x)                   COLUMN_("blob", String, x, 0, 0)

#define PRIMARY_KEY                INLINE_ATTRIBUTE("primary key")
#define AUTO_INCREMENT             INLINE_ATTRIBUTE("autoincrement")
#define NOT_NULL                   INLINE_ATTRIBUTE("not null")

#define INDEX                      ATTRIBUTE("create index IDX_@x on @t(@c);", \
                                             "drop index IDX_@x;")
#define UNIQUE                     ATTRIBUTE("create unique index IDX_@x on @t(@c);", \
                                             "drop index IDX_@x;")

#define REFERENCES(x)              ATTRIBUTE("alter table @t add (constraint FK_@x foreign key "\
                                             "(@c) references " #x ");",\
                                             "alter table @t drop constraint FK_@x;")
#define REFERENCES_CASCADE(x)      ATTRIBUTE("alter table @t add (constraint FK_@x foreign key "\
                                             "(@c) references " #x " on delete cascade);",\
                                             "alter table @t drop constraint FK_@x;")
#define REFERENCES_(n, x)          ATTRIBUTE("alter table @t add (constraint FK_@x$" #n " foreign key "\
                                             "(@c) references " #x ");",\
                                             "alter table @t drop constraint FK_@x$" #n ";")
#define REFERENCES_CASCADE_(n, x)  ATTRIBUTE("alter table @t add (constraint FK_@x$" #n " foreign key "\
                                             "(@c) references " #x " on delete cascade);",\
                                             "alter table @t drop constraint FK_@x$" #n ";")

#define DUAL_PRIMARY_KEY(k1, k2)   INLINE_ATTRIBUTE(", primary key (" #k1 ", " #k2 ")")

#define DUAL_UNIQUE(k1, k2)        ATTRIBUTE("alter table @t add constraint DQ_@t unique "\
                                             "(" #k1 ", " #k2 ");",\
                                             "alter table @t drop constraint DQ_@t;")

#define UNIQUE_LIST(u, l)          ATTRIBUTE("alter table @t add constraint UQ_@t$" #u " unique "\
                                             "(" l ");",\
                                             "alter table @t drop constraint UQ_@t$" #u ";")

#define SQLCHECK(n, ct)            ATTRIBUTE("alter table @t add constraint CHK_@t$" #n " check "\
                                             "(" ct ");",\
                                             "alter table @t drop constraint CHK_@t$" #n ";")

#define TIMESTAMP(ts)              SCHEMA("-- " ts "\n\n", NULL)

#define COMMENT(txt)               SCHEMA("-- " #txt "\n", NULL)


#include <Sql/sch_model.h>


#undef INT
#undef INT_ARRAY
#undef INT_
#undef INT_ARRAY_

#undef DOUBLE
#undef DOUBLE_ARRAY
#undef DOUBLE_
#undef DOUBLE_ARRAY_

#undef STRING
#undef STRING_ARRAY
#undef STRING_
#undef STRING_ARRAY_

#undef BLOB
#undef BLOB_

#undef PRIMARY_KEY
#undef AUTO_INCREMENT
#undef NOT_NULL

#undef INDEX
#undef UNIQUE

#undef TIMESTAMP
#undef COMMENT

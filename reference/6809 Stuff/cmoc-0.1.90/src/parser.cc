/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.yy"

/*  CMOC - A C-like cross-compiler
    Copyright (C) 2003-2016 Pierre Sarrazin <http://sarrazip.com/>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DeclarationSequence.h"
#include "WordConstantExpr.h"
#include "RealConstantExpr.h"
#include "DWordConstantExpr.h"
#include "IdentifierExpr.h"
#include "VariableExpr.h"
#include "FunctionCallExpr.h"
#include "StringLiteralExpr.h"
#include "BinaryOpExpr.h"
#include "UnaryOpExpr.h"
#include "CastExpr.h"
#include "CompoundStmt.h"
#include "IfStmt.h"
#include "SwitchStmt.h"
#include "LabeledStmt.h"
#include "WhileStmt.h"
#include "ForStmt.h"
#include "JumpStmt.h"
#include "Scope.h"
#include "FunctionDef.h"
#include "FormalParameter.h"
#include "FormalParamList.h"
#include "AssemblerStmt.h"
#include "TranslationUnit.h"
#include "ObjectMemberExpr.h"
#include "ClassDef.h"
#include "ConditionalExpr.h"
#include "DeclarationSpecifierList.h"
#include "Pragma.h"
#include "CommaExpr.h"
#include "FunctionPointerCast.h"

#include <string.h>
#include <time.h>

using namespace std;

int yyparse(void);
int yylex(void);
void yyerror(const char *msg);
extern char *yytext;

extern string sourceFilename;  // defined in lexer.ll
extern int lineno;  // defined in lexer.ll
int numErrors = 0;  // error counter for yyerror()
int numWarnings = 0;
static bool doubleTypeWarningIssued = false;

void _PARSERTRACE(int parserLineNo, const char *fmt, ...);
#define PARSERTRACE(...) _PARSERTRACE(__LINE__, __VA_ARGS__)


#line 141 "parser.cc"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    STRLIT = 259,
    PRAGMA = 260,
    CHARLIT = 261,
    REAL = 262,
    TYPE_NAME = 263,
    INT = 264,
    CHAR = 265,
    SHORT = 266,
    LONG = 267,
    FLOAT = 268,
    DOUBLE = 269,
    SIGNED = 270,
    UNSIGNED = 271,
    VOID = 272,
    PLUS_PLUS = 273,
    MINUS_MINUS = 274,
    IF = 275,
    ELSE = 276,
    WHILE = 277,
    DO = 278,
    FOR = 279,
    EQUALS_EQUALS = 280,
    BANG_EQUALS = 281,
    LOWER_EQUALS = 282,
    GREATER_EQUALS = 283,
    AMP_AMP = 284,
    PIPE_PIPE = 285,
    LT_LT = 286,
    GT_GT = 287,
    BREAK = 288,
    CONTINUE = 289,
    RETURN = 290,
    ASM = 291,
    NORTS = 292,
    VERBATIM_ASM = 293,
    STRUCT = 294,
    UNION = 295,
    PLUS_EQUALS = 296,
    MINUS_EQUALS = 297,
    ASTERISK_EQUALS = 298,
    SLASH_EQUALS = 299,
    PERCENT_EQUALS = 300,
    LT_LT_EQUALS = 301,
    GT_GT_EQUALS = 302,
    CARET_EQUALS = 303,
    AMP_EQUALS = 304,
    PIPE_EQUALS = 305,
    RIGHT_ARROW = 306,
    INTERRUPT = 307,
    SIZEOF = 308,
    ELLIPSIS = 309,
    TYPEDEF = 310,
    ENUM = 311,
    SWITCH = 312,
    CASE = 313,
    DEFAULT = 314,
    REGISTER = 315,
    GOTO = 316,
    EXTERN = 317,
    STATIC = 318,
    CONST = 319,
    VOLATILE = 320,
    AUTO = 321,
    FUNC_RECEIVES_FIRST_PARAM_IN_REG = 322,
    FUNC_USES_GCC6809_CALL_CONV = 323
  };
#endif
/* Tokens.  */
#define ID 258
#define STRLIT 259
#define PRAGMA 260
#define CHARLIT 261
#define REAL 262
#define TYPE_NAME 263
#define INT 264
#define CHAR 265
#define SHORT 266
#define LONG 267
#define FLOAT 268
#define DOUBLE 269
#define SIGNED 270
#define UNSIGNED 271
#define VOID 272
#define PLUS_PLUS 273
#define MINUS_MINUS 274
#define IF 275
#define ELSE 276
#define WHILE 277
#define DO 278
#define FOR 279
#define EQUALS_EQUALS 280
#define BANG_EQUALS 281
#define LOWER_EQUALS 282
#define GREATER_EQUALS 283
#define AMP_AMP 284
#define PIPE_PIPE 285
#define LT_LT 286
#define GT_GT 287
#define BREAK 288
#define CONTINUE 289
#define RETURN 290
#define ASM 291
#define NORTS 292
#define VERBATIM_ASM 293
#define STRUCT 294
#define UNION 295
#define PLUS_EQUALS 296
#define MINUS_EQUALS 297
#define ASTERISK_EQUALS 298
#define SLASH_EQUALS 299
#define PERCENT_EQUALS 300
#define LT_LT_EQUALS 301
#define GT_GT_EQUALS 302
#define CARET_EQUALS 303
#define AMP_EQUALS 304
#define PIPE_EQUALS 305
#define RIGHT_ARROW 306
#define INTERRUPT 307
#define SIZEOF 308
#define ELLIPSIS 309
#define TYPEDEF 310
#define ENUM 311
#define SWITCH 312
#define CASE 313
#define DEFAULT 314
#define REGISTER 315
#define GOTO 316
#define EXTERN 317
#define STATIC 318
#define CONST 319
#define VOLATILE 320
#define AUTO 321
#define FUNC_RECEIVES_FIRST_PARAM_IN_REG 322
#define FUNC_USES_GCC6809_CALL_CONV 323

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 73 "parser.yy"

    char *str;
    signed char character;
    double real;
    int integer;
    BasicType basicType;
    const TypeDesc *typeDesc;
    TypeSpecifier *typeSpecifier;
    DeclarationSpecifierList *declarationSpecifierList;
    Tree *tree;
    CompoundStmt *compoundStmt;
    TreeSequence *treeSequence;
    FunctionPointerCast *funcPtrCast;
    FormalParamList *formalParamList;
    FormalParameter *formalParameter;
    DeclarationSequence *declarationSequence;
    Declarator *declarator;
    std::vector<Declarator *> *declaratorVector;
    BinaryOpExpr::Op binop;
    UnaryOpExpr::Op unop;
    Scope *scope;
    FunctionDef *functionDef;
    std::vector<std::string> *strList;
    ClassDef *classDef;
    ClassDef::ClassMember *classMember;
    FilenameAndLineNo *filenameAndLineNo;
    std::vector<ClassDef::ClassMember *> *classMemberList;
    Enumerator *enumerator;
    std::vector<Enumerator *> *enumeratorList;
    TypeQualifierBitFieldVector *typeQualifierBitFieldVector;

#line 361 "parser.cc"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  48
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1087

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  93
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  95
/* YYNRULES -- Number of rules.  */
#define YYNRULES  267
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  440

#define YYUNDEFTOK  2
#define YYMAXUTOK   323


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    90,     2,     2,     2,    89,    83,     2,
      70,    71,    73,    86,    72,    87,    92,    88,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    79,    69,
      84,    76,    85,    80,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    77,     2,    78,    82,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    74,    81,    75,    91,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   151,   151,   152,   156,   162,   171,   172,   173,   177,
     178,   179,   183,   213,   233,   234,   239,   240,   248,   249,
     254,   255,   268,   269,   270,   276,   277,   284,   301,   311,
     325,   344,   348,   349,   353,   354,   358,   359,   360,   361,
     365,   366,   367,   368,   372,   377,   378,   384,   388,   397,
     399,   401,   403,   405,   407,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   425,   426,   427,   430,   431,
     432,   436,   437,   441,   442,   446,   459,   476,   477,   481,
     487,   492,   501,   502,   503,   507,   508,   512,   513,   517,
     518,   519,   520,   521,   522,   523,   527,   539,   540,   544,
     548,   552,   553,   557,   558,   564,   565,   569,   573,   577,
     583,   586,   592,   597,   607,   614,   619,   626,   632,   639,
     645,   658,   684,   685,   690,   691,   695,   699,   700,   704,
     705,   706,   707,   708,   712,   713,   717,   718,   722,   732,
     745,   749,   750,   755,   756,   764,   773,   774,   778,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
     790,   791,   794,   797,   800,   804,   810,   815,   823,   827,
     828,   832,   833,   837,   838,   852,   853,   857,   858,   867,
     868,   869,   870,   871,   872,   873,   874,   875,   876,   877,
     881,   882,   887,   888,   893,   894,   899,   900,   904,   905,
     909,   910,   914,   915,   920,   921,   925,   926,   930,   931,
     933,   938,   939,   940,   941,   945,   946,   950,   951,   955,
     956,   960,   961,   962,   966,   967,   987,   988,   989,   990,
     994,   995,   996,   997,   998,   999,  1003,  1004,  1008,  1009,
    1012,  1015,  1017,  1019,  1021,  1023,  1028,  1029,  1047,  1048,
    1049,  1053,  1054,  1063,  1064,  1068,  1069,  1073,  1074,  1078,
    1082,  1083,  1087,  1091,  1095,  1099,  1100,  1104
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "STRLIT", "PRAGMA", "CHARLIT",
  "REAL", "TYPE_NAME", "INT", "CHAR", "SHORT", "LONG", "FLOAT", "DOUBLE",
  "SIGNED", "UNSIGNED", "VOID", "PLUS_PLUS", "MINUS_MINUS", "IF", "ELSE",
  "WHILE", "DO", "FOR", "EQUALS_EQUALS", "BANG_EQUALS", "LOWER_EQUALS",
  "GREATER_EQUALS", "AMP_AMP", "PIPE_PIPE", "LT_LT", "GT_GT", "BREAK",
  "CONTINUE", "RETURN", "ASM", "NORTS", "VERBATIM_ASM", "STRUCT", "UNION",
  "PLUS_EQUALS", "MINUS_EQUALS", "ASTERISK_EQUALS", "SLASH_EQUALS",
  "PERCENT_EQUALS", "LT_LT_EQUALS", "GT_GT_EQUALS", "CARET_EQUALS",
  "AMP_EQUALS", "PIPE_EQUALS", "RIGHT_ARROW", "INTERRUPT", "SIZEOF",
  "ELLIPSIS", "TYPEDEF", "ENUM", "SWITCH", "CASE", "DEFAULT", "REGISTER",
  "GOTO", "EXTERN", "STATIC", "CONST", "VOLATILE", "AUTO",
  "FUNC_RECEIVES_FIRST_PARAM_IN_REG", "FUNC_USES_GCC6809_CALL_CONV", "';'",
  "'('", "')'", "','", "'*'", "'{'", "'}'", "'='", "'['", "']'", "':'",
  "'?'", "'|'", "'^'", "'&'", "'<'", "'>'", "'+'", "'-'", "'/'", "'%'",
  "'!'", "'~'", "'.'", "$accept", "goal", "translation_unit",
  "external_declaration", "function_definition",
  "kr_parameter_name_list_opt", "kr_parameter_name_list",
  "kr_parameter_list_opt", "kr_parameter_list", "parameter_type_list",
  "parameter_list", "parameter_declaration_list", "parameter_declaration",
  "type_name", "abstract_declarator", "function_pointer_cast_opt",
  "function_pointer_cast", "pointer", "specifier_qualifier_list",
  "compound_stmt", "stmt_list_opt", "declaration",
  "declaration_specifiers", "storage_class_specifier", "type_specifier",
  "type_qualifier", "type_qualifier_list", "struct_or_union_specifier",
  "struct_or_union", "enum_specifier", "enumerator_list", "enumerator",
  "comma_opt", "non_void_basic_type", "basic_type", "save_src_fn",
  "save_line_no", "declarator_list", "init_declarator_list",
  "init_declarator", "declarator", "direct_declarator",
  "kandr_prototype_no_return", "asterisk_sequence", "subscript_list",
  "subscript", "parameter_type_list_opt", "initializer",
  "initializer_list", "struct_declaration_list_opt",
  "struct_declaration_list", "struct_declaration",
  "struct_declarator_list", "struct_declarator", "stmt_list", "stmt",
  "labeled_stmt", "constant_expr", "expr_stmt", "expr_opt", "expr",
  "arg_expr_list", "assignment_expr", "assignment_op", "conditional_expr",
  "logical_or_expr", "logical_and_expr", "inclusive_or_expr",
  "exclusive_or_expr", "and_expr", "equality_expr", "equality_op",
  "rel_expr", "shift_expr", "rel_op", "add_expr", "add_op", "mul_expr",
  "mul_op", "unary_expr", "unary_op", "cast_expr", "postfix_expr",
  "primary_expr", "strlit_seq", "expr_list_opt", "expr_list",
  "selection_stmt", "if_cond", "else_part_opt", "while_stmt", "while_cond",
  "do_while_stmt", "for_init", "for_stmt", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,    59,
      40,    41,    44,    42,   123,   125,    61,    91,    93,    58,
      63,   124,    94,    38,    60,    62,    43,    45,    47,    37,
      33,   126,    46
};
# endif

#define YYPACT_NINF (-227)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-100)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     853,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,
    -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,
       9,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,
      84,   853,  -227,  -227,  -227,    43,  1019,  1019,  1019,  -227,
      12,  -227,  -227,  -227,  -227,  -227,   -49,    69,  -227,  -227,
    -227,  -227,    15,    -8,    57,     5,  -227,   -25,   -27,  -227,
    -227,  -227,    40,  -227,   820,    48,    69,    53,    59,  -227,
      65,  -227,    36,  -227,  -227,    -8,  -227,  -227,    57,  -227,
     564,  -227,   786,   676,   820,    25,   820,   820,    78,   820,
    -227,   153,    59,   676,    69,    88,    97,   -50,   102,  -227,
    -227,  -227,  -227,   112,  -227,  -227,  -227,  -227,  -227,   682,
     682,   701,   539,  -227,   210,  -227,  -227,  -227,  -227,  -227,
    -227,  -227,  -227,    -7,   161,   116,   118,   123,   122,    17,
     119,    74,   -19,  -227,   676,   362,    29,  -227,   194,  -227,
     130,  -227,  -227,    98,   131,   135,  -227,    57,   125,   136,
    -227,   134,   676,   132,    18,  -227,  -227,  -227,  -227,  -227,
     139,   146,   144,  -227,  -227,  -227,  -227,  -227,   920,   151,
     676,     4,  -227,   953,   361,   676,  -227,  -227,   539,  -227,
     154,     6,   103,   147,  -227,  -227,    68,   676,   676,   676,
     676,   676,   676,  -227,  -227,   676,  -227,  -227,  -227,  -227,
     676,   676,   676,  -227,  -227,   676,  -227,  -227,  -227,   676,
    -227,  -227,  -227,  -227,   221,   676,   227,  -227,  -227,  -227,
    1019,   228,  -227,   986,  -227,  -227,   676,  -227,  -227,   676,
    -227,    25,  1019,  -227,   162,  -227,   163,   953,   157,   166,
    -227,   169,   164,   171,   174,   361,   175,   177,   179,   583,
     180,  -227,   181,  -227,  -227,   246,  -227,  -227,   178,  -227,
      43,   361,  -227,  -227,  -227,    72,  -227,  -227,  -227,  -227,
     184,   676,   183,   182,  -227,  -227,  -227,  -227,  -227,   558,
    -227,   161,   -17,   116,   118,   123,   122,    17,   119,    74,
      74,   -19,  -227,   431,  -227,    54,  -227,   187,  -227,  1019,
     189,    57,  -227,  -227,  -227,  -227,  -227,  -227,   -45,  -227,
    -227,   188,  -227,   953,  -227,  -227,   676,   676,   238,  -227,
    -227,  -227,  -227,    77,   258,   676,  -227,  -227,   195,  -227,
    -227,  -227,  -227,  -227,    32,  -227,  -227,  -227,   676,  -227,
    -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,
     676,  -227,   657,   196,  -227,   198,  -227,  -227,  -227,  -227,
     197,   193,   136,   202,   136,   203,   205,  -227,  -227,   111,
     115,   676,   200,  -227,   207,  -227,  -227,  -227,   120,  -227,
    -227,    57,  -227,   361,   361,   361,   676,   450,   212,   190,
     361,   209,   361,   887,  -227,   676,  -227,  -227,   265,  -227,
     218,  -227,   136,   222,   220,  -227,  -227,   219,   223,  -227,
     361,  -227,  -227,   224,  -227,   361,  -227,   229,  -227,   676,
    -227,   233,   234,  -227,  -227,  -227,  -227,   136,   676,  -227,
    -227,   236,  -227,  -227,   676,   235,  -227,  -227,   361,  -227
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,    99,     8,    70,    89,    90,    91,    94,    95,    96,
      92,    93,    98,    59,    60,    10,    77,    78,    55,    58,
       0,    61,    64,    63,    71,    72,    62,    57,    56,     9,
       0,     2,     4,     6,     7,   110,    49,    51,    53,    66,
       0,    69,    97,    65,    11,   100,    81,    82,     1,     5,
     109,    47,     0,    36,   110,     0,   103,   105,   108,    50,
      52,    54,    67,    68,   136,     0,    82,    85,    88,    83,
       0,   122,     0,    38,    73,    37,   107,    48,   110,    99,
       0,    12,     0,   171,   136,   110,    41,    43,     0,   137,
     138,    14,    88,     0,    87,     0,     0,     0,     0,   123,
      39,    74,   104,   105,   100,   246,   251,   248,   247,     0,
       0,     0,     0,   233,     0,   232,   230,   231,   234,   235,
     106,   129,   177,   190,   192,   194,   196,   198,   200,   202,
     206,   208,   215,   236,     0,   219,   224,   238,   249,    16,
      98,    24,   114,     0,     0,    22,    25,   110,     0,   172,
     173,     0,     0,   143,     0,   141,    40,    42,    76,   139,
       0,    15,     0,    86,   168,   219,    84,    80,   127,     0,
     171,     0,   124,   127,    45,     0,   226,   227,     0,   229,
       0,    32,     0,     0,   132,   134,     0,     0,     0,     0,
       0,     0,     0,   204,   205,     0,   212,   214,   211,   213,
       0,     0,     0,   217,   218,     0,   221,   222,   223,     0,
     225,   100,   242,   243,     0,     0,     0,   100,   252,   115,
      18,     0,   112,     0,    28,   111,     0,    75,   144,     0,
     140,   110,    18,    79,    98,   128,     0,   127,     0,     0,
     125,     0,   246,     0,     0,     0,     0,     0,     0,     0,
      59,   164,     0,    99,    99,     0,   169,   150,     0,   149,
     110,    46,   146,   152,   148,     0,   151,   153,   154,   155,
       0,     0,     0,    32,    29,    33,    31,   250,   133,     0,
     130,   193,     0,   195,   197,   199,   201,   203,   207,   209,
     210,   216,   220,     0,   245,     0,   244,     0,   113,    19,
       0,   110,    17,    23,    26,   174,   145,   142,     0,   119,
     120,     0,   126,   127,   117,   100,     0,     0,     0,    99,
     157,   158,   159,     0,     0,     0,   100,   100,     0,    44,
     147,   170,   228,   237,     0,    30,   131,   135,     0,   180,
     181,   182,   183,   184,   188,   189,   185,   186,   187,   179,
       0,   241,     0,     0,    20,    27,   101,   121,    13,   116,
       0,     0,   259,     0,   263,     0,     0,   100,   160,     0,
       0,     0,     0,   156,     0,   191,   178,   239,     0,   175,
      21,   110,   118,     0,     0,     0,     0,   253,     0,     0,
       0,     0,     0,     0,   240,     0,   102,   165,   260,   262,
       0,   266,   255,     0,   254,    99,   163,     0,     0,   258,
       0,   167,    35,     0,   176,     0,   257,     0,   265,     0,
     100,     0,     0,   166,    34,   261,   264,   256,   171,   161,
     162,     0,    99,   100,   253,     0,    99,   100,     0,   267
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -227,  -227,  -227,   276,  -227,  -227,   225,    79,  -227,   -75,
    -227,    11,    89,   137,  -227,    41,  -227,   -42,   -70,   -47,
    -227,     1,     0,  -227,     7,   -23,  -227,  -227,  -227,  -227,
     251,   230,   231,  -227,  -227,   -77,  -100,  -227,  -227,   241,
     -32,  -227,  -227,    50,  -227,   155,  -159,  -101,  -227,   243,
    -227,   240,  -227,    94,  -227,  -226,  -227,   -41,  -227,  -162,
     -78,  -227,   -74,  -227,   -84,  -227,   145,   142,   148,   143,
     150,  -227,   140,   152,  -227,    -6,  -227,   138,  -227,    70,
    -227,   -67,  -227,  -227,  -227,   -98,  -227,  -227,  -227,  -227,
    -227,   -46,  -227,  -227,  -227
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    30,    31,    32,    33,   160,   143,   298,   299,   235,
     145,   300,   146,   180,   273,   274,   275,    54,    85,   257,
     258,   259,   260,    36,    37,    38,    75,    39,    40,    41,
      68,    69,    95,    42,    43,    45,    65,   355,    55,    56,
     103,    58,    44,    72,   171,   172,   236,   120,   186,    88,
      89,    90,   154,   155,   261,   262,   263,   163,   264,   148,
     265,   378,   150,   350,   122,   123,   124,   125,   126,   127,
     128,   195,   129,   130,   200,   131,   205,   132,   209,   133,
     134,   135,   136,   137,   138,   403,   404,   266,   363,   416,
     267,   365,   268,   405,   269
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      35,    34,   104,    57,   174,   149,   121,   144,   238,   164,
      81,    73,    46,   185,   241,    62,   156,   157,    70,   318,
      63,   169,    76,   187,   357,    66,   165,   170,    50,    79,
      74,    35,    34,   100,   182,   330,    59,    60,    61,    97,
     121,    87,   181,    82,   196,   197,    50,   212,   213,    79,
      83,    80,   101,   153,   206,   226,    24,    25,   211,   217,
      50,    87,   338,    87,    87,    53,    87,   210,   228,   207,
     208,    86,    67,   188,    77,   239,   272,    78,   311,    53,
     214,   170,   147,    47,    48,   165,    64,   230,    71,    87,
     231,    86,   149,    86,    86,    52,    86,   182,    53,   -99,
     182,   198,   199,   374,   152,    99,   215,    98,   181,    99,
     282,   293,    51,    52,    84,   224,    53,   297,    91,    86,
     165,   216,   165,   165,   165,   165,   226,    52,   165,    93,
      53,    94,   351,   165,   165,   165,    96,   295,   165,   276,
     279,   331,   292,   280,   226,   306,   368,   193,   194,   226,
     201,   202,   305,   158,   360,    87,   139,   397,   398,   399,
     203,   204,   165,   167,   409,   315,   411,   168,   147,   220,
     221,   323,   173,   147,   277,   226,   326,   327,   337,   176,
     177,   179,   388,   389,   423,    86,   390,   226,    80,   425,
     189,   394,   395,   407,   408,   289,   290,   190,   218,   153,
     191,   219,   222,   225,   333,   121,   192,   223,   226,   227,
     232,   229,   439,   105,   106,   361,   107,   108,   221,   233,
     301,   237,   278,   147,   294,   271,   371,   372,   109,   110,
     296,   302,   301,   309,   310,   312,   313,   147,   362,   364,
     314,   316,   367,   -99,   317,   319,   320,   370,   321,   328,
     324,   325,   272,   329,   375,   332,    71,   352,   354,   359,
     366,   358,   369,   111,   373,   380,   431,   387,   382,   356,
     381,   165,   383,   384,   385,   386,   376,   393,   379,   392,
     112,   406,   183,   113,   114,   184,   415,   164,   410,   417,
     421,   418,   419,   115,   422,   424,   116,   117,   426,   301,
     118,   119,   429,   430,   165,   432,   436,    49,   364,   402,
     353,   308,   304,   147,   335,   270,   161,    92,   413,   102,
     428,   414,   334,   162,   166,   307,   240,   151,   420,   159,
     391,   283,   281,   434,   285,   287,   435,   438,   284,     0,
     400,   427,   286,   291,     0,     0,     0,     0,     0,   396,
     149,     0,   288,     0,     0,   433,   402,     0,     0,   437,
       0,     0,     0,     0,   242,   106,     0,   107,   108,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,   109,
     110,   243,     0,   244,   245,   246,     0,     0,   401,     0,
       0,     0,     0,   147,   247,   248,   249,   250,    14,   251,
      16,    17,     0,   -99,   -99,   -99,   -99,   -99,   -99,   -99,
     -99,   -99,   -99,    18,   111,     0,    19,    20,   252,   253,
     254,    21,   255,    22,    23,    24,    25,    26,    27,    28,
     256,   112,     0,     0,   113,    79,     0,     0,   -99,     0,
       0,     0,     0,     0,   115,     0,     0,   116,   117,     0,
       0,   118,   119,   105,   106,     0,   107,   108,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,   109,   110,
       0,     0,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,     0,     0,     0,     0,    13,    14,     0,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    18,   111,     0,    19,    20,   349,     0,     0,
      21,     0,    22,    23,    24,    25,    26,    27,    28,     0,
     112,     0,     0,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   115,     0,     0,   116,   117,     0,     0,
     118,   119,   105,   106,     0,   107,   108,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,   109,   110,     0,
       0,   105,   106,     0,   107,   108,     0,   105,   106,     0,
     107,   108,     0,     0,     0,     0,   109,   110,    16,    17,
       0,     0,   109,   110,     0,     0,   105,   106,     0,   107,
     108,     0,   111,     0,     0,    20,     0,     0,     0,     0,
       0,   109,   110,    24,    25,     0,     0,     0,     0,   112,
       0,   111,   113,     0,     0,     0,     0,   111,     0,     0,
       0,     0,   115,     0,     0,   116,   117,     0,   112,   118,
     119,   113,   114,   336,   112,     0,   111,   113,   114,     0,
       0,   115,     0,     0,   116,   117,     0,   115,   118,   119,
     116,   117,   322,   112,   118,   119,   113,     0,     0,     0,
     105,   106,     0,   107,   108,     0,   115,     0,     0,   116,
     117,     0,     0,   118,   119,   109,   110,     0,     0,   105,
     106,     0,   107,   108,     0,   105,   106,     0,   107,   108,
       0,     0,     0,     0,   109,   110,     0,     0,     0,     0,
     109,   110,     0,     0,   105,   106,     0,   107,   108,     0,
     111,     0,     0,     0,     0,     0,     0,     0,     0,   109,
     110,     0,     0,     0,     0,     0,     0,   112,   377,   111,
     113,     0,     0,     0,     0,   111,     0,     0,     0,     0,
     115,     0,     0,   116,   117,     0,   112,   118,   119,   113,
       0,     0,   175,     0,   111,   113,     0,     0,     0,   115,
       0,     0,   116,   117,     0,   115,   118,   119,   116,   117,
       0,   178,   118,   119,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   115,     0,     0,   116,   117,   139,
       0,   118,   119,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    11,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    13,    14,     0,    16,    17,     0,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    18,     0,
     141,    19,    20,     0,     0,     0,    21,     0,    22,    23,
      24,    25,    26,    27,    28,     0,     1,   142,     2,    16,
      17,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,     0,     0,    20,     0,     0,     0,
       0,     0,     0,     0,    24,    25,     0,     0,     0,    13,
      14,    15,    16,    17,     0,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    18,     0,     0,    19,    20,
       0,     0,     0,    21,     0,    22,    23,    24,    25,    26,
      27,    28,    29,    13,    14,     0,    16,    17,     3,     4,
       5,     6,     7,     8,     9,    10,    11,   234,     0,    18,
       0,   141,    19,    20,     0,     0,     0,    21,     0,    22,
      23,    24,    25,    26,    27,    28,    13,    14,   412,    16,
      17,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,     0,    18,     0,   141,    19,    20,     0,     0,     0,
      21,     0,    22,    23,    24,    25,    26,    27,    28,    13,
      14,     0,    16,    17,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,     0,    18,     0,   141,    19,    20,
       0,     0,     0,    21,     0,    22,    23,    24,    25,    26,
      27,    28,    13,    14,     0,    16,    17,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,     0,    18,     0,
     303,    19,    20,     0,     0,     0,    21,     0,    22,    23,
      24,    25,    26,    27,    28,    13,    14,     0,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,     0,     0,    19,    20,     0,     0,     0,    21,
       0,    22,    23,    24,    25,    26,    27,    28
};

static const yytype_int16 yycheck[] =
{
       0,     0,    79,    35,   104,    83,    80,    82,   170,    93,
      57,    53,     3,   114,   173,     3,    86,    87,     3,   245,
       8,    71,    54,    30,    69,    74,    93,    77,     3,    74,
      53,    31,    31,    75,   112,   261,    36,    37,    38,     3,
     114,    64,   112,    70,    27,    28,     3,    18,    19,    74,
      77,    76,    75,    85,    73,    72,    64,    65,   135,   136,
       3,    84,    79,    86,    87,    73,    89,   134,   152,    88,
      89,    64,     3,    80,    69,    71,    70,    72,   237,    73,
      51,    77,    82,    74,     0,   152,    74,    69,    73,   112,
      72,    84,   170,    86,    87,    70,    89,   175,    73,    70,
     178,    84,    85,    71,    79,    73,    77,    71,   178,    73,
     188,   211,    69,    70,    74,   147,    73,   217,    70,   112,
     187,    92,   189,   190,   191,   192,    72,    70,   195,    76,
      73,    72,    78,   200,   201,   202,    71,   215,   205,   181,
      72,    69,   209,    75,    72,   229,    69,    25,    26,    72,
      31,    32,   226,    75,   313,   178,     3,   383,   384,   385,
      86,    87,   229,    75,   390,   242,   392,    70,   168,    71,
      72,   249,    70,   173,    71,    72,   253,   254,   279,   109,
     110,   111,    71,    72,   410,   178,    71,    72,    76,   415,
      29,    71,    72,     3,     4,   201,   202,    81,     4,   231,
      82,    71,    71,    78,   271,   279,    83,    72,    72,    75,
      71,    79,   438,     3,     4,   315,     6,     7,    72,    75,
     220,    70,    75,   223,     3,    71,   326,   327,    18,    19,
       3,     3,   232,    71,    71,    78,    70,   237,   316,   317,
      71,    70,   319,    79,    70,    70,    69,   325,    69,     3,
      70,    70,    70,    75,   338,    71,    73,    70,    69,    71,
      22,   308,     4,    53,    69,    69,   428,   367,    71,   301,
      72,   338,    79,    71,    71,    70,   350,    70,   352,    79,
      70,    69,    72,    73,    74,    75,    21,   371,    79,    71,
      71,    69,    72,    83,    71,    71,    86,    87,    69,   299,
      90,    91,    69,    69,   371,    69,    71,    31,   386,   387,
     299,   232,   223,   313,   273,   178,    91,    66,   393,    78,
     420,   395,   272,    92,    94,   231,   171,    84,   405,    89,
     371,   189,   187,   433,   191,   195,   434,   437,   190,    -1,
     386,   419,   192,   205,    -1,    -1,    -1,    -1,    -1,   381,
     428,    -1,   200,    -1,    -1,   432,   434,    -1,    -1,   436,
      -1,    -1,    -1,    -1,     3,     4,    -1,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    24,    -1,    -1,   387,    -1,
      -1,    -1,    -1,   393,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    52,    53,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    -1,    86,    87,    -1,
      -1,    90,    91,     3,     4,    -1,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    52,    53,    -1,    55,    56,    76,    -1,    -1,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    -1,    86,    87,    -1,    -1,
      90,    91,     3,     4,    -1,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,     3,     4,    -1,     6,     7,    -1,     3,     4,    -1,
       6,     7,    -1,    -1,    -1,    -1,    18,    19,    39,    40,
      -1,    -1,    18,    19,    -1,    -1,     3,     4,    -1,     6,
       7,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    18,    19,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    53,    73,    -1,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    86,    87,    -1,    70,    90,
      91,    73,    74,    75,    70,    -1,    53,    73,    74,    -1,
      -1,    83,    -1,    -1,    86,    87,    -1,    83,    90,    91,
      86,    87,    69,    70,    90,    91,    73,    -1,    -1,    -1,
       3,     4,    -1,     6,     7,    -1,    83,    -1,    -1,    86,
      87,    -1,    -1,    90,    91,    18,    19,    -1,    -1,     3,
       4,    -1,     6,     7,    -1,     3,     4,    -1,     6,     7,
      -1,    -1,    -1,    -1,    18,    19,    -1,    -1,    -1,    -1,
      18,    19,    -1,    -1,     3,     4,    -1,     6,     7,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    53,
      73,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    86,    87,    -1,    70,    90,    91,    73,
      -1,    -1,    70,    -1,    53,    73,    -1,    -1,    -1,    83,
      -1,    -1,    86,    87,    -1,    83,    90,    91,    86,    87,
      -1,    70,    90,    91,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    -1,    86,    87,     3,
      -1,    90,    91,    -1,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    52,    -1,
      54,    55,    56,    -1,    -1,    -1,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,     3,    71,     5,    39,
      40,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    36,
      37,    38,    39,    40,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    52,    -1,    -1,    55,    56,
      -1,    -1,    -1,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    36,    37,    -1,    39,    40,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    -1,    52,
      -1,    54,    55,    56,    -1,    -1,    -1,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    36,    37,    71,    39,
      40,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    -1,    52,    -1,    54,    55,    56,    -1,    -1,    -1,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    36,
      37,    -1,    39,    40,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    -1,    52,    -1,    54,    55,    56,
      -1,    -1,    -1,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    36,    37,    -1,    39,    40,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    -1,    52,    -1,
      54,    55,    56,    -1,    -1,    -1,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    36,    37,    -1,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    -1,    55,    56,    -1,    -1,    -1,    60,
      -1,    62,    63,    64,    65,    66,    67,    68
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     5,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    36,    37,    38,    39,    40,    52,    55,
      56,    60,    62,    63,    64,    65,    66,    67,    68,    69,
      94,    95,    96,    97,   114,   115,   116,   117,   118,   120,
     121,   122,   126,   127,   135,   128,     3,    74,     0,    96,
       3,    69,    70,    73,   110,   131,   132,   133,   134,   115,
     115,   115,     3,     8,    74,   129,    74,     3,   123,   124,
       3,    73,   136,   110,   118,   119,   133,    69,    72,    74,
      76,   112,    70,    77,    74,   111,   117,   118,   142,   143,
     144,    70,   123,    76,    72,   125,    71,     3,    71,    73,
     110,   118,   132,   133,   128,     3,     4,     6,     7,    18,
      19,    53,    70,    73,    74,    83,    86,    87,    90,    91,
     140,   155,   157,   158,   159,   160,   161,   162,   163,   165,
     166,   168,   170,   172,   173,   174,   175,   176,   177,     3,
      17,    54,    71,    99,   102,   103,   105,   115,   152,   153,
     155,   142,    79,   133,   145,   146,   111,   111,    75,   144,
      98,    99,   125,   150,   157,   174,   124,    75,    70,    71,
      77,   137,   138,    70,   129,    70,   172,   172,    70,   172,
     106,   111,   153,    72,    75,   140,   141,    30,    80,    29,
      81,    82,    83,    25,    26,   164,    27,    28,    84,    85,
     167,    31,    32,    86,    87,   169,    73,    88,    89,   171,
     174,   128,    18,    19,    51,    77,    92,   128,     4,    71,
      71,    72,    71,    72,   133,    78,    72,    75,   157,    79,
      69,    72,    71,    75,    17,   102,   139,    70,   152,    71,
     138,   139,     3,    20,    22,    23,    24,    33,    34,    35,
      36,    38,    57,    58,    59,    61,    69,   112,   113,   114,
     115,   147,   148,   149,   151,   153,   180,   183,   185,   187,
     106,    71,    70,   107,   108,   109,   110,    71,    75,    72,
      75,   159,   153,   160,   161,   162,   163,   165,   166,   168,
     168,   170,   174,   129,     3,   153,     3,   129,   100,   101,
     104,   115,     3,    54,   105,   155,   157,   146,   100,    71,
      71,   139,    78,    70,    71,   128,    70,    70,   148,    70,
      69,    69,    69,   153,    70,    70,   128,   128,     3,    75,
     148,    69,    71,   174,   136,   108,    75,   140,    79,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    76,
     156,    78,    70,   104,    69,   130,   133,    69,   112,    71,
     139,   129,   153,   181,   153,   184,    22,   128,    69,     4,
     153,   129,   129,    69,    71,   157,   155,    71,   154,   155,
      69,    72,    71,    79,    71,    71,    70,   129,    71,    72,
      71,   150,    79,    70,    71,    72,   133,   148,   148,   148,
     184,   114,   153,   178,   179,   186,    69,     3,     4,   148,
      79,   148,    71,   102,   155,    21,   182,    71,    69,    72,
     128,    71,    71,   148,    71,   148,    69,   153,   129,    69,
      69,   152,    69,   128,   129,   178,    71,   128,   129,   148
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    93,    94,    94,    95,    95,    96,    96,    96,    96,
      96,    96,    97,    97,    98,    98,    99,    99,   100,   100,
     101,   101,   102,   102,   102,   103,   103,   104,   105,   106,
     106,   107,   108,   108,   109,   109,   110,   110,   110,   110,
     111,   111,   111,   111,   112,   113,   113,   114,   114,   115,
     115,   115,   115,   115,   115,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   117,   117,   117,   117,   117,
     117,   118,   118,   119,   119,   120,   120,   121,   121,   122,
     122,   122,   123,   123,   123,   124,   124,   125,   125,   126,
     126,   126,   126,   126,   126,   126,   126,   127,   127,   128,
     129,   130,   130,   131,   131,   132,   132,   133,   133,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   135,   136,   136,   137,   137,   138,   139,   139,   140,
     140,   140,   140,   140,   141,   141,   142,   142,   143,   143,
     144,   145,   145,   146,   146,   146,   147,   147,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   149,   149,   149,   150,   151,
     151,   152,   152,   153,   153,   154,   154,   155,   155,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     157,   157,   158,   158,   159,   159,   160,   160,   161,   161,
     162,   162,   163,   163,   164,   164,   165,   165,   166,   166,
     166,   167,   167,   167,   167,   168,   168,   169,   169,   170,
     170,   171,   171,   171,   172,   172,   172,   172,   172,   172,
     173,   173,   173,   173,   173,   173,   174,   174,   175,   175,
     175,   175,   175,   175,   175,   175,   176,   176,   176,   176,
     176,   177,   177,   178,   178,   179,   179,   180,   180,   181,
     182,   182,   183,   184,   185,   186,   186,   187
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     1,     2,     1,     1,     1,     1,
       1,     1,     3,     8,     0,     1,     1,     3,     0,     1,
       2,     3,     1,     3,     1,     1,     3,     2,     2,     2,
       3,     1,     0,     1,     6,     5,     1,     2,     2,     3,
       2,     1,     2,     1,     5,     0,     1,     2,     3,     1,
       2,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     2,     5,     4,     1,     1,     6,
       5,     2,     0,     1,     3,     1,     3,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       0,     1,     3,     1,     3,     1,     3,     2,     1,     1,
       0,     4,     4,     5,     3,     4,     7,     6,     8,     6,
       6,     8,     1,     2,     1,     2,     3,     0,     1,     1,
       3,     4,     2,     3,     1,     3,     0,     1,     1,     2,
       3,     1,     3,     1,     2,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     2,     2,     2,
       3,     7,     7,     5,     1,     5,     6,     5,     1,     1,
       2,     0,     1,     1,     3,     1,     3,     1,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     1,     1,     3,     1,     3,
       3,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     1,     2,     2,     2,     4,     2,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     5,
       6,     4,     2,     2,     3,     3,     1,     1,     1,     1,
       3,     1,     2,     0,     1,     1,     3,     6,     5,     1,
       0,     2,     5,     1,     7,     2,     1,    16
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 151 "parser.yy"
                                { TranslationUnit::instance().setDefinitionList((yyvsp[0].treeSequence)); }
#line 2020 "parser.cc"
    break;

  case 3:
#line 152 "parser.yy"
                                { TranslationUnit::instance().setDefinitionList(NULL); }
#line 2026 "parser.cc"
    break;

  case 4:
#line 157 "parser.yy"
            {
                (yyval.treeSequence) = new TreeSequence();
                if ((yyvsp[0].tree))
                    (yyval.treeSequence)->addTree((yyvsp[0].tree)); 
            }
#line 2036 "parser.cc"
    break;

  case 5:
#line 163 "parser.yy"
            {
                (yyval.treeSequence) = (yyvsp[-1].treeSequence);
                if ((yyvsp[0].tree))
                    (yyval.treeSequence)->addTree((yyvsp[0].tree));
            }
#line 2046 "parser.cc"
    break;

  case 6:
#line 171 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].functionDef); }
#line 2052 "parser.cc"
    break;

  case 7:
#line 172 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].declarationSequence); }
#line 2058 "parser.cc"
    break;

  case 8:
#line 173 "parser.yy"
                                { auto p = new Pragma((yyvsp[0].str));
                                  (yyval.tree) = p;
                                  free((yyvsp[0].str));
                                  TranslationUnit::instance().processParseTimePragma(*p); }
#line 2067 "parser.cc"
    break;

  case 9:
#line 177 "parser.yy"
                                { (yyval.tree) = NULL; }
#line 2073 "parser.cc"
    break;

  case 10:
#line 178 "parser.yy"
                                { (yyval.tree) = new AssemblerStmt(yytext); }
#line 2079 "parser.cc"
    break;

  case 11:
#line 179 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].tree); }
#line 2085 "parser.cc"
    break;

  case 12:
#line 184 "parser.yy"
            {
                DeclarationSpecifierList *dsl = (yyvsp[-2].declarationSpecifierList);
                Declarator *di = (yyvsp[-1].declarator);

                if (dsl && dsl->hasEnumeratorList())
                {
                    errormsg("enum with enumerated names is not supported in a function's return type");
                    dsl->detachEnumeratorList();
                }
                if (dsl && dsl->isStaticDeclaration() && dsl->isExternDeclaration())
                {
                    errormsg("function definition must not be both static and extern");
                }

                // Example: In byte **f() {}, dsl represents "byte" and
                // di represents **f(). Hence, di contains a pointer level of 2,
                // which is applied to the TypeDesc found in dsl, i.e., "byte".
                // di also contains the name of the function, "f".
                //
                (yyval.functionDef) = new FunctionDef(dsl, *di);
                (yyval.functionDef)->setLineNo(di->getSourceFilename(), di->getLineNo());
                (yyval.functionDef)->setBody((yyvsp[0].compoundStmt));

                if (dsl && dsl->isTypeDefinition() && di->getId().empty())
                    errormsg("invalid typedef (type being defined may already be defined)");

                delete di;
                delete dsl;
            }
#line 2119 "parser.cc"
    break;

  case 13:
#line 214 "parser.yy"
            {
                Declarator di((yyvsp[-7].str), 0, (yyvsp[-6].str), (yyvsp[-5].integer));  // pass ID, save_src_fn, save_line_no
                di.processKAndRFunctionParameters(*(yyvsp[-3].strList), (yyvsp[-1].formalParamList));  // deletes $7, keeps no ref to $5

                (yyval.functionDef) = new FunctionDef(NULL, di);
                (yyval.functionDef)->setLineNo(di.getSourceFilename(), di.getLineNo());
                (yyval.functionDef)->setBody((yyvsp[0].compoundStmt));  // compound_stmt

                delete (yyvsp[-3].strList);  // delete kr_parameter_name_list_opt
                free((yyvsp[-6].str));  // save_src_fn
                free((yyvsp[-7].str));  // ID
            }
#line 2136 "parser.cc"
    break;

  case 14:
#line 233 "parser.yy"
                                        { (yyval.strList) = new std::vector<std::string>(); }
#line 2142 "parser.cc"
    break;

  case 15:
#line 234 "parser.yy"
                                        { (yyval.strList) = (yyvsp[0].strList); }
#line 2148 "parser.cc"
    break;

  case 16:
#line 239 "parser.yy"
                                        { (yyval.strList) = new std::vector<std::string>(); (yyval.strList)->push_back((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 2154 "parser.cc"
    break;

  case 17:
#line 240 "parser.yy"
                                        { (yyval.strList) = (yyvsp[-2].strList); (yyval.strList)->push_back((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 2160 "parser.cc"
    break;

  case 18:
#line 248 "parser.yy"
                                        { (yyval.formalParamList) = new FormalParamList(); }
#line 2166 "parser.cc"
    break;

  case 19:
#line 249 "parser.yy"
                                        { (yyval.formalParamList) = (yyvsp[0].formalParamList); }
#line 2172 "parser.cc"
    break;

  case 20:
#line 254 "parser.yy"
                                        { (yyval.formalParamList) = (yyvsp[-1].formalParamList); }
#line 2178 "parser.cc"
    break;

  case 21:
#line 256 "parser.yy"
                            {
                                (yyval.formalParamList) = (yyvsp[-2].formalParamList); 
                                // Move trees from parameter_declaration_list (a FormalParamList) to $$.
                                for (auto it = (yyvsp[-1].formalParamList)->begin(); it != (yyvsp[-1].formalParamList)->end(); ++it)
                                    (yyval.formalParamList)->addTree(*it);
                                (yyvsp[-1].formalParamList)->clear();  // detach the trees from TreeSequence that is about to get destroyed
                                delete (yyvsp[-1].formalParamList);
                            }
#line 2191 "parser.cc"
    break;

  case 22:
#line 268 "parser.yy"
                                     { (yyval.formalParamList) = (yyvsp[0].formalParamList); }
#line 2197 "parser.cc"
    break;

  case 23:
#line 269 "parser.yy"
                                     { (yyval.formalParamList) = (yyvsp[-2].formalParamList); (yyval.formalParamList)->endWithEllipsis(); }
#line 2203 "parser.cc"
    break;

  case 24:
#line 270 "parser.yy"
                                     { (yyval.formalParamList) = new FormalParamList(); (yyval.formalParamList)->endWithEllipsis(); }
#line 2209 "parser.cc"
    break;

  case 25:
#line 276 "parser.yy"
                                     { (yyval.formalParamList) = new FormalParamList(); if ((yyvsp[0].formalParameter)) (yyval.formalParamList)->addTree((yyvsp[0].formalParameter)); }
#line 2215 "parser.cc"
    break;

  case 26:
#line 278 "parser.yy"
                                     { (yyval.formalParamList) = (yyvsp[-2].formalParamList); if ((yyvsp[0].formalParameter)) (yyval.formalParamList)->addTree((yyvsp[0].formalParameter)); }
#line 2221 "parser.cc"
    break;

  case 27:
#line 285 "parser.yy"
                {
                    DeclarationSpecifierList *dsl = (yyvsp[-1].declarationSpecifierList);
                    (yyval.formalParamList) = new FormalParamList();
                    std::vector<Declarator *> *declaratorVector = (yyvsp[0].declaratorVector);
                    for (Declarator *declarator : *declaratorVector)
                    {
                        FormalParameter *fp = declarator->createFormalParameter(*dsl);
                        (yyval.formalParamList)->addTree(fp);
                    }
                    delete dsl;
                    deleteVectorElements(*declaratorVector);  // delete the Declarator objects
                    delete declaratorVector;
                }
#line 2239 "parser.cc"
    break;

  case 28:
#line 302 "parser.yy"
                {
                    DeclarationSpecifierList *dsl = (yyvsp[-1].declarationSpecifierList);
                    (yyval.formalParameter) = (yyvsp[0].declarator)->createFormalParameter(*dsl);
                    delete dsl;
                    delete (yyvsp[0].declarator);
                }
#line 2250 "parser.cc"
    break;

  case 29:
#line 312 "parser.yy"
                {
                    if ((yyvsp[0].funcPtrCast))  // if func ptr cast, or ptr to ptr to func, etc.
                    {
                        (yyval.typeDesc) = TranslationUnit::getTypeManager().getFunctionPointerType(
                                (yyvsp[-1].declarationSpecifierList)->getTypeDesc(), *(yyvsp[0].funcPtrCast)->getFormalParamList(),
                                (yyvsp[-1].declarationSpecifierList)->isInterruptServiceFunction(), (yyvsp[-1].declarationSpecifierList)->getCallConvention());
                        (yyval.typeDesc) = TranslationUnit::getTypeManager().getPointerTo((yyval.typeDesc), (yyvsp[0].funcPtrCast)->getPointerLevel() - 1);
                        delete (yyvsp[0].funcPtrCast);
                    }
                    else
                        (yyval.typeDesc) = (yyvsp[-1].declarationSpecifierList)->getTypeDesc();
                    delete (yyvsp[-1].declarationSpecifierList);
                }
#line 2268 "parser.cc"
    break;

  case 30:
#line 326 "parser.yy"
                {
                    const TypeDesc *td = TranslationUnit::getTypeManager().getPointerTo((yyvsp[-2].declarationSpecifierList)->getTypeDesc(), *(yyvsp[-1].typeQualifierBitFieldVector));
                    if ((yyvsp[0].funcPtrCast))
                    {
                        (yyval.typeDesc) = TranslationUnit::getTypeManager().getFunctionPointerType(
                                td, *(yyvsp[0].funcPtrCast)->getFormalParamList(),
                                (yyvsp[-2].declarationSpecifierList)->isInterruptServiceFunction(), (yyvsp[-2].declarationSpecifierList)->getCallConvention());
                        (yyval.typeDesc) = TranslationUnit::getTypeManager().getPointerTo((yyval.typeDesc), (yyvsp[0].funcPtrCast)->getPointerLevel() - 1);
                        delete (yyvsp[0].funcPtrCast);
                    }
                    else
                        (yyval.typeDesc) = td;
                    delete (yyvsp[-1].typeQualifierBitFieldVector);  /* originally created by 'pointer' rule */
                    delete (yyvsp[-2].declarationSpecifierList);
                }
#line 2288 "parser.cc"
    break;

  case 31:
#line 344 "parser.yy"
                        { (yyval.typeQualifierBitFieldVector) = (yyvsp[0].typeQualifierBitFieldVector); }
#line 2294 "parser.cc"
    break;

  case 32:
#line 348 "parser.yy"
                                                { (yyval.funcPtrCast) = NULL; }
#line 2300 "parser.cc"
    break;

  case 33:
#line 349 "parser.yy"
                                                { (yyval.funcPtrCast) = (yyvsp[0].funcPtrCast); }
#line 2306 "parser.cc"
    break;

  case 34:
#line 353 "parser.yy"
                                                              { (yyval.funcPtrCast) = new FunctionPointerCast((yyvsp[-4].integer), (yyvsp[-1].formalParamList)); }
#line 2312 "parser.cc"
    break;

  case 35:
#line 354 "parser.yy"
                                                              { (yyval.funcPtrCast) = new FunctionPointerCast((yyvsp[-3].integer), new FormalParamList()); }
#line 2318 "parser.cc"
    break;

  case 36:
#line 358 "parser.yy"
                                        { (yyval.typeQualifierBitFieldVector) = new TypeQualifierBitFieldVector(); (yyval.typeQualifierBitFieldVector)->push_back(0); }
#line 2324 "parser.cc"
    break;

  case 37:
#line 359 "parser.yy"
                                        { (yyval.typeQualifierBitFieldVector) = new TypeQualifierBitFieldVector(); (yyval.typeQualifierBitFieldVector)->push_back(TypeQualifierBitField((yyvsp[0].integer))); }
#line 2330 "parser.cc"
    break;

  case 38:
#line 360 "parser.yy"
                                        { (yyval.typeQualifierBitFieldVector) = (yyvsp[0].typeQualifierBitFieldVector); (yyval.typeQualifierBitFieldVector)->push_back(0); }
#line 2336 "parser.cc"
    break;

  case 39:
#line 361 "parser.yy"
                                        { (yyval.typeQualifierBitFieldVector) = (yyvsp[0].typeQualifierBitFieldVector); (yyval.typeQualifierBitFieldVector)->push_back((yyvsp[-1].integer)); }
#line 2342 "parser.cc"
    break;

  case 40:
#line 365 "parser.yy"
                                                { (yyval.declarationSpecifierList) = (yyvsp[0].declarationSpecifierList); (yyval.declarationSpecifierList)->add(*(yyvsp[-1].typeSpecifier)); delete (yyvsp[-1].typeSpecifier); }
#line 2348 "parser.cc"
    break;

  case 41:
#line 366 "parser.yy"
                                                { (yyval.declarationSpecifierList) = new DeclarationSpecifierList(); (yyval.declarationSpecifierList)->add(*(yyvsp[0].typeSpecifier)); delete (yyvsp[0].typeSpecifier); }
#line 2354 "parser.cc"
    break;

  case 42:
#line 367 "parser.yy"
                                                { (yyval.declarationSpecifierList) = (yyvsp[0].declarationSpecifierList); if ((yyvsp[-1].integer) != -1) (yyval.declarationSpecifierList)->add(DeclarationSpecifierList::Specifier((yyvsp[-1].integer))); }
#line 2360 "parser.cc"
    break;

  case 43:
#line 368 "parser.yy"
                                                { (yyval.declarationSpecifierList) = new DeclarationSpecifierList(); if ((yyvsp[0].integer) != -1) (yyval.declarationSpecifierList)->add(DeclarationSpecifierList::Specifier((yyvsp[0].integer))); }
#line 2366 "parser.cc"
    break;

  case 44:
#line 373 "parser.yy"
                        { (yyval.compoundStmt) = (yyvsp[-1].compoundStmt); (yyval.compoundStmt)->setLineNo((yyvsp[-3].str), (yyvsp[-2].integer)); free((yyvsp[-3].str)); }
#line 2372 "parser.cc"
    break;

  case 45:
#line 377 "parser.yy"
                        { (yyval.compoundStmt) = new CompoundStmt(); }
#line 2378 "parser.cc"
    break;

  case 46:
#line 378 "parser.yy"
                        { (yyval.compoundStmt) = (yyvsp[0].compoundStmt); }
#line 2384 "parser.cc"
    break;

  case 47:
#line 385 "parser.yy"
                        {
                            (yyval.declarationSequence) = TranslationUnit::instance().createDeclarationSequence((yyvsp[-1].declarationSpecifierList), NULL);  // deletes $1
                        }
#line 2392 "parser.cc"
    break;

  case 48:
#line 389 "parser.yy"
                        {
                            (yyval.declarationSequence) = TranslationUnit::instance().createDeclarationSequence((yyvsp[-2].declarationSpecifierList), (yyvsp[-1].declaratorVector));  // deletes $1 and $2
                        }
#line 2400 "parser.cc"
    break;

  case 49:
#line 398 "parser.yy"
            { (yyval.declarationSpecifierList) = new DeclarationSpecifierList(); if ((yyvsp[0].integer) != -1) (yyval.declarationSpecifierList)->add(DeclarationSpecifierList::Specifier((yyvsp[0].integer))); }
#line 2406 "parser.cc"
    break;

  case 50:
#line 400 "parser.yy"
            { (yyval.declarationSpecifierList) = (yyvsp[0].declarationSpecifierList); if ((yyvsp[-1].integer) != -1) (yyval.declarationSpecifierList)->add(DeclarationSpecifierList::Specifier((yyvsp[-1].integer))); }
#line 2412 "parser.cc"
    break;

  case 51:
#line 402 "parser.yy"
            { (yyval.declarationSpecifierList) = new DeclarationSpecifierList(); (yyval.declarationSpecifierList)->add(*(yyvsp[0].typeSpecifier)); delete (yyvsp[0].typeSpecifier); }
#line 2418 "parser.cc"
    break;

  case 52:
#line 404 "parser.yy"
            { (yyval.declarationSpecifierList) = (yyvsp[0].declarationSpecifierList); (yyval.declarationSpecifierList)->add(*(yyvsp[-1].typeSpecifier)); delete (yyvsp[-1].typeSpecifier); }
#line 2424 "parser.cc"
    break;

  case 53:
#line 406 "parser.yy"
            { (yyval.declarationSpecifierList) = new DeclarationSpecifierList(); if ((yyvsp[0].integer) != -1) (yyval.declarationSpecifierList)->add(DeclarationSpecifierList::Specifier((yyvsp[0].integer))); }
#line 2430 "parser.cc"
    break;

  case 54:
#line 408 "parser.yy"
            { (yyval.declarationSpecifierList) = (yyvsp[0].declarationSpecifierList); if ((yyvsp[-1].integer) != -1) (yyval.declarationSpecifierList)->add(DeclarationSpecifierList::Specifier((yyvsp[-1].integer))); }
#line 2436 "parser.cc"
    break;

  case 55:
#line 412 "parser.yy"
                    { (yyval.integer) = DeclarationSpecifierList::INTERRUPT_SPEC; }
#line 2442 "parser.cc"
    break;

  case 56:
#line 413 "parser.yy"
                                       { (yyval.integer) = DeclarationSpecifierList::GCCCALL_SPEC; }
#line 2448 "parser.cc"
    break;

  case 57:
#line 414 "parser.yy"
                                       { (yyval.integer) = DeclarationSpecifierList::FUNC_RECEIVES_FIRST_PARAM_IN_REG_SPEC; }
#line 2454 "parser.cc"
    break;

  case 58:
#line 415 "parser.yy"
                    { (yyval.integer) = DeclarationSpecifierList::TYPEDEF_SPEC; }
#line 2460 "parser.cc"
    break;

  case 59:
#line 416 "parser.yy"
                    { (yyval.integer) = DeclarationSpecifierList::ASSEMBLY_ONLY_SPEC; }
#line 2466 "parser.cc"
    break;

  case 60:
#line 417 "parser.yy"
                    { (yyval.integer) = DeclarationSpecifierList::NO_RETURN_INSTRUCTION; }
#line 2472 "parser.cc"
    break;

  case 61:
#line 418 "parser.yy"
                    { (yyval.integer) = -1; /* not supported, ignored */ }
#line 2478 "parser.cc"
    break;

  case 62:
#line 419 "parser.yy"
                    { (yyval.integer) = -1; /* not supported, ignored */ }
#line 2484 "parser.cc"
    break;

  case 63:
#line 420 "parser.yy"
                    { (yyval.integer) = DeclarationSpecifierList::STATIC_SPEC; }
#line 2490 "parser.cc"
    break;

  case 64:
#line 421 "parser.yy"
                    { (yyval.integer) = DeclarationSpecifierList::EXTERN_SPEC; }
#line 2496 "parser.cc"
    break;

  case 65:
#line 425 "parser.yy"
                                    { (yyval.typeSpecifier) = new TypeSpecifier((yyvsp[0].typeDesc), "", NULL); }
#line 2502 "parser.cc"
    break;

  case 66:
#line 426 "parser.yy"
                                    { (yyval.typeSpecifier) = new TypeSpecifier((yyvsp[0].typeDesc), "", NULL); }
#line 2508 "parser.cc"
    break;

  case 67:
#line 427 "parser.yy"
                                    { const TypeDesc *td = TranslationUnit::getTypeManager().getClassType((yyvsp[0].str), (yyvsp[-1].integer) == UNION, true);
                                      (yyval.typeSpecifier) = new TypeSpecifier(td, "", NULL);
                                      free((yyvsp[0].str)); }
#line 2516 "parser.cc"
    break;

  case 68:
#line 430 "parser.yy"
                                    { (yyval.typeSpecifier) = new TypeSpecifier((yyvsp[0].typeDesc), "", NULL); }
#line 2522 "parser.cc"
    break;

  case 69:
#line 431 "parser.yy"
                                    { (yyval.typeSpecifier) = (yyvsp[0].typeSpecifier); }
#line 2528 "parser.cc"
    break;

  case 70:
#line 432 "parser.yy"
                                    { (yyval.typeSpecifier) = new TypeSpecifier((yyvsp[0].typeDesc), "", NULL); }
#line 2534 "parser.cc"
    break;

  case 71:
#line 436 "parser.yy"
                                    { (yyval.integer) = DeclarationSpecifierList::CONST_QUALIFIER; }
#line 2540 "parser.cc"
    break;

  case 72:
#line 437 "parser.yy"
                                    { (yyval.integer) = DeclarationSpecifierList::VOLATILE_QUALIFIER; TranslationUnit::instance().enableVolatileWarning(); }
#line 2546 "parser.cc"
    break;

  case 73:
#line 441 "parser.yy"
                                            { (yyval.integer) = ((yyvsp[0].integer) == DeclarationSpecifierList::CONST_QUALIFIER ? CONST_BIT : VOLATILE_BIT); }
#line 2552 "parser.cc"
    break;

  case 74:
#line 442 "parser.yy"
                                            { (yyval.integer) = (yyvsp[-1].integer) | ((yyvsp[0].integer) == DeclarationSpecifierList::CONST_QUALIFIER ? CONST_BIT : VOLATILE_BIT); }
#line 2558 "parser.cc"
    break;

  case 75:
#line 447 "parser.yy"
                    {
                        ClassDef *classDef = (yyvsp[-1].classDef);
                        assert(classDef);
                        classDef->setName((yyvsp[-3].str));
                        classDef->setUnion((yyvsp[-4].integer) == UNION);
                        TranslationUnit::instance().getGlobalScope().declareClass(classDef);
                        const TypeDesc *td = TranslationUnit::getTypeManager().getClassType((yyvsp[-3].str), (yyvsp[-4].integer) == UNION, true);
                        assert(td);
                        classDef->setTypeDesc(td);
                        (yyval.typeDesc) = td;
                        free((yyvsp[-3].str));
                    }
#line 2575 "parser.cc"
    break;

  case 76:
#line 460 "parser.yy"
                    {
                        string anonStructName = "AnonStruct_" + (yyvsp[-1].classDef)->getLineNo();
                        ClassDef *classDef = (yyvsp[-1].classDef);
                        assert(classDef);
                        classDef->setName(anonStructName);
                        classDef->setUnion((yyvsp[-3].integer) == UNION);
                        TranslationUnit::instance().getGlobalScope().declareClass(classDef);
                        const TypeDesc *td = TranslationUnit::getTypeManager().getClassType(anonStructName, (yyvsp[-3].integer) == UNION, true);
                        assert(td);
                        classDef->setTypeDesc(td);
                        (yyval.typeDesc) = td;
                    }
#line 2592 "parser.cc"
    break;

  case 77:
#line 476 "parser.yy"
                    { (yyval.integer) = STRUCT; }
#line 2598 "parser.cc"
    break;

  case 78:
#line 477 "parser.yy"
                    { (yyval.integer) = UNION;  }
#line 2604 "parser.cc"
    break;

  case 79:
#line 482 "parser.yy"
                        {
                            const TypeDesc *td = TranslationUnit::getTypeManager().getIntType(WORD_TYPE, true);
                            (yyval.typeSpecifier) = new TypeSpecifier(td, (yyvsp[-4].str), (yyvsp[-2].enumeratorList));
                            free((yyvsp[-4].str));
                        }
#line 2614 "parser.cc"
    break;

  case 80:
#line 488 "parser.yy"
                        {
                            const TypeDesc *td = TranslationUnit::getTypeManager().getIntType(WORD_TYPE, true);
                            (yyval.typeSpecifier) = new TypeSpecifier(td, "", (yyvsp[-2].enumeratorList));
                        }
#line 2623 "parser.cc"
    break;

  case 81:
#line 493 "parser.yy"
                        {
                            const TypeDesc *td = TranslationUnit::getTypeManager().getIntType(WORD_TYPE, true);
                            (yyval.typeSpecifier) = new TypeSpecifier(td, (yyvsp[0].str), NULL);
                            free((yyvsp[0].str));
                        }
#line 2633 "parser.cc"
    break;

  case 82:
#line 501 "parser.yy"
                                            { (yyval.enumeratorList) = new vector<Enumerator *>(); }
#line 2639 "parser.cc"
    break;

  case 83:
#line 502 "parser.yy"
                                            { (yyval.enumeratorList) = new vector<Enumerator *>(); (yyval.enumeratorList)->push_back((yyvsp[0].enumerator)); }
#line 2645 "parser.cc"
    break;

  case 84:
#line 503 "parser.yy"
                                            { (yyval.enumeratorList) = (yyvsp[-2].enumeratorList); (yyval.enumeratorList)->push_back((yyvsp[0].enumerator)); }
#line 2651 "parser.cc"
    break;

  case 85:
#line 507 "parser.yy"
                                            { (yyval.enumerator) = new Enumerator((yyvsp[0].str), NULL, getSourceLineNo()); free((yyvsp[0].str)); }
#line 2657 "parser.cc"
    break;

  case 86:
#line 508 "parser.yy"
                                            { (yyval.enumerator) = new Enumerator((yyvsp[-2].str), (yyvsp[0].tree),   getSourceLineNo()); free((yyvsp[-2].str)); }
#line 2663 "parser.cc"
    break;

  case 89:
#line 517 "parser.yy"
                { (yyval.typeDesc) = TranslationUnit::getTypeManager().getIntType(WORD_TYPE, true); }
#line 2669 "parser.cc"
    break;

  case 90:
#line 518 "parser.yy"
                { (yyval.typeDesc) = TranslationUnit::getTypeManager().getIntType(BYTE_TYPE, TranslationUnit::instance().isCharSignedByDefault()); }
#line 2675 "parser.cc"
    break;

  case 91:
#line 519 "parser.yy"
                { (yyval.typeDesc) = TranslationUnit::getTypeManager().getIntType(WORD_TYPE, true); }
#line 2681 "parser.cc"
    break;

  case 92:
#line 520 "parser.yy"
                { (yyval.typeDesc) = TranslationUnit::getTypeManager().getSizelessType(true);  }
#line 2687 "parser.cc"
    break;

  case 93:
#line 521 "parser.yy"
                { (yyval.typeDesc) = TranslationUnit::getTypeManager().getSizelessType(false); }
#line 2693 "parser.cc"
    break;

  case 94:
#line 522 "parser.yy"
                { (yyval.typeDesc) = TranslationUnit::getTypeManager().getLongType(true); }
#line 2699 "parser.cc"
    break;

  case 95:
#line 523 "parser.yy"
                {
                    TranslationUnit::instance().warnIfFloatUnsupported();
                    (yyval.typeDesc) = TranslationUnit::getTypeManager().getRealType(false);
                }
#line 2708 "parser.cc"
    break;

  case 96:
#line 527 "parser.yy"
                {
                    TranslationUnit::instance().warnIfFloatUnsupported();
                    (yyval.typeDesc) = TranslationUnit::getTypeManager().getRealType(false);
                    if (!doubleTypeWarningIssued)
                    {
                        warnmsg("`double' is an alias for `float' for this compiler");
                        doubleTypeWarningIssued = true;
                    }
                }
#line 2722 "parser.cc"
    break;

  case 97:
#line 539 "parser.yy"
                            { (yyval.typeDesc) = (yyvsp[0].typeDesc); }
#line 2728 "parser.cc"
    break;

  case 98:
#line 540 "parser.yy"
                            { (yyval.typeDesc) = TranslationUnit::getTypeManager().getVoidType(); }
#line 2734 "parser.cc"
    break;

  case 99:
#line 544 "parser.yy"
                                { (yyval.str) = strdup(sourceFilename.c_str()); }
#line 2740 "parser.cc"
    break;

  case 100:
#line 548 "parser.yy"
                                { (yyval.integer) = lineno; }
#line 2746 "parser.cc"
    break;

  case 101:
#line 552 "parser.yy"
                                        { (yyval.declaratorVector) = new std::vector<Declarator *>(); (yyval.declaratorVector)->push_back((yyvsp[0].declarator)); }
#line 2752 "parser.cc"
    break;

  case 102:
#line 553 "parser.yy"
                                        { (yyval.declaratorVector) = (yyvsp[-2].declaratorVector); (yyval.declaratorVector)->push_back((yyvsp[0].declarator)); }
#line 2758 "parser.cc"
    break;

  case 103:
#line 557 "parser.yy"
                                                { (yyval.declaratorVector) = new std::vector<Declarator *>(); (yyval.declaratorVector)->push_back((yyvsp[0].declarator)); }
#line 2764 "parser.cc"
    break;

  case 104:
#line 558 "parser.yy"
                                                { (yyval.declaratorVector) = (yyvsp[-2].declaratorVector); (yyval.declaratorVector)->push_back((yyvsp[0].declarator)); }
#line 2770 "parser.cc"
    break;

  case 105:
#line 564 "parser.yy"
                                        { (yyval.declarator) = (yyvsp[0].declarator); }
#line 2776 "parser.cc"
    break;

  case 106:
#line 565 "parser.yy"
                                        { (yyval.declarator) = (yyvsp[-2].declarator); (yyval.declarator)->setInitExpr((yyvsp[0].tree)); }
#line 2782 "parser.cc"
    break;

  case 107:
#line 569 "parser.yy"
                                        {
                                            (yyval.declarator) = (yyvsp[0].declarator);
                                            (yyval.declarator)->setPointerLevel((yyvsp[-1].typeQualifierBitFieldVector));  // ownership of $1 transferred to the Declarator
                                        }
#line 2791 "parser.cc"
    break;

  case 108:
#line 573 "parser.yy"
                                        { (yyval.declarator) = (yyvsp[0].declarator); }
#line 2797 "parser.cc"
    break;

  case 109:
#line 578 "parser.yy"
            {
                (yyval.declarator) = new Declarator((yyvsp[0].str), 0, sourceFilename, lineno);
                free((yyvsp[0].str));
            }
#line 2806 "parser.cc"
    break;

  case 110:
#line 583 "parser.yy"
            {
                (yyval.declarator) = new Declarator(string(), 0, sourceFilename, lineno);
            }
#line 2814 "parser.cc"
    break;

  case 111:
#line 587 "parser.yy"
            {
                (yyval.declarator) = (yyvsp[-3].declarator);
                (yyval.declarator)->checkForFunctionReturningArray();
                (yyval.declarator)->addArraySizeExpr((yyvsp[-1].tree));
            }
#line 2824 "parser.cc"
    break;

  case 112:
#line 593 "parser.yy"
            {
                (yyval.declarator) = (yyvsp[-3].declarator);
                (yyval.declarator)->setFormalParamList((yyvsp[-1].formalParamList));
            }
#line 2833 "parser.cc"
    break;

  case 113:
#line 598 "parser.yy"
            {
                assert((yyvsp[-2].strList) != NULL);
                assert((yyvsp[0].formalParamList) != NULL);

                (yyval.declarator) = (yyvsp[-4].declarator);
                (yyval.declarator)->processKAndRFunctionParameters(*(yyvsp[-2].strList), (yyvsp[0].formalParamList));  // deletes $5, keeps no ref to $3
                
                delete (yyvsp[-2].strList);  // kr_parameter_name_list [vector<string> *]
            }
#line 2847 "parser.cc"
    break;

  case 114:
#line 608 "parser.yy"
            {
                (yyval.declarator) = (yyvsp[-2].declarator);
                FormalParamList *fpl = new FormalParamList();
                fpl->endWithEllipsis(true);  // ellipsis is implied
                (yyval.declarator)->setFormalParamList(fpl);
            }
#line 2858 "parser.cc"
    break;

  case 115:
#line 615 "parser.yy"
            {
                (yyval.declarator) = (yyvsp[-3].declarator);
                (yyval.declarator)->setFormalParamList(new FormalParamList());
            }
#line 2867 "parser.cc"
    break;

  case 116:
#line 620 "parser.yy"
            {
                (yyval.declarator) = new Declarator((yyvsp[-4].str), (yyvsp[-5].integer), sourceFilename, lineno);
                (yyval.declarator)->setAsFunctionPointer((yyvsp[-1].formalParamList));  // takes ownership of FormalParamList
                free((yyvsp[-4].str));
                TranslationUnit::checkForEllipsisWithoutNamedArgument((yyvsp[-1].formalParamList));
            }
#line 2878 "parser.cc"
    break;

  case 117:
#line 627 "parser.yy"
            {
                (yyval.declarator) = new Declarator(string(), (yyvsp[-4].integer), sourceFilename, lineno);
                (yyval.declarator)->setAsFunctionPointer((yyvsp[-1].formalParamList));  // takes ownership of FormalParamList
                TranslationUnit::checkForEllipsisWithoutNamedArgument((yyvsp[-1].formalParamList));
            }
#line 2888 "parser.cc"
    break;

  case 118:
#line 633 "parser.yy"
            {
                (yyval.declarator) = new Declarator((yyvsp[-5].str), (yyvsp[-6].integer), sourceFilename, lineno);
                (yyval.declarator)->setAsArrayOfFunctionPointers((yyvsp[-1].formalParamList), (yyvsp[-4].treeSequence));  // takes ownership of FormalParamList ($7), deletes $4
                free((yyvsp[-5].str));
                TranslationUnit::checkForEllipsisWithoutNamedArgument((yyvsp[-1].formalParamList));
            }
#line 2899 "parser.cc"
    break;

  case 119:
#line 640 "parser.yy"
            {
                (yyval.declarator) = new Declarator((yyvsp[-4].str), 0, sourceFilename, lineno);
                free((yyvsp[-4].str));
                (yyval.declarator)->setFormalParamList(new FormalParamList());
            }
#line 2909 "parser.cc"
    break;

  case 120:
#line 646 "parser.yy"
            {
                (yyval.declarator) = new Declarator((yyvsp[-4].str), 0, sourceFilename, lineno);
                (yyval.declarator)->setAsFunctionPointer((yyvsp[-1].formalParamList));  // takes ownership of FormalParamList
                free((yyvsp[-4].str));
                TranslationUnit::checkForEllipsisWithoutNamedArgument((yyvsp[-1].formalParamList));
            }
#line 2920 "parser.cc"
    break;

  case 121:
#line 659 "parser.yy"
            {
                // Make a Declarator from the function ID and the parameters.
                auto di = new Declarator((yyvsp[-7].str), 0, (yyvsp[-6].str), (yyvsp[-5].integer));  // pass ID, save_src_fn, save_line_no
                di->processKAndRFunctionParameters(*(yyvsp[-3].strList), (yyvsp[-1].formalParamList));  // deletes $7, keeps no ref to $5

                auto v = new std::vector<Declarator *>();
                v->push_back(di);  // 'v' owns 'di'

                // Make the return type 'int'.
                const TypeDesc *intTD = TranslationUnit::getTypeManager().getIntType(WORD_TYPE, true);
                TypeSpecifier intSpecifier(intTD, "", NULL);
                auto dsl = new DeclarationSpecifierList();
                dsl->add(intSpecifier);

                (yyval.tree) = TranslationUnit::instance().createDeclarationSequence(dsl, v);  // deletes 'dsl' and 'v'

                (yyval.tree)->setLineNo((yyvsp[-6].str), (yyvsp[-5].integer));

                delete (yyvsp[-3].strList);  // delete kr_parameter_name_list_opt
                free((yyvsp[-6].str));  // save_src_fn
                free((yyvsp[-7].str));  // ID
            }
#line 2947 "parser.cc"
    break;

  case 122:
#line 684 "parser.yy"
                                    { (yyval.integer) = 1; }
#line 2953 "parser.cc"
    break;

  case 123:
#line 685 "parser.yy"
                                    { ++(yyval.integer); }
#line 2959 "parser.cc"
    break;

  case 124:
#line 690 "parser.yy"
                                     { (yyval.treeSequence) = new TreeSequence(); (yyval.treeSequence)->addTree((yyvsp[0].tree)); }
#line 2965 "parser.cc"
    break;

  case 125:
#line 691 "parser.yy"
                                     { (yyval.treeSequence) = (yyvsp[-1].treeSequence); (yyval.treeSequence)->addTree((yyvsp[0].tree)); }
#line 2971 "parser.cc"
    break;

  case 126:
#line 695 "parser.yy"
                                     { (yyval.tree) = (yyvsp[-1].tree); }
#line 2977 "parser.cc"
    break;

  case 127:
#line 699 "parser.yy"
                                     { (yyval.formalParamList) = new FormalParamList(); (yyval.formalParamList)->endWithEllipsis(true); /* implied ellipsis */ }
#line 2983 "parser.cc"
    break;

  case 128:
#line 700 "parser.yy"
                                     { (yyval.formalParamList) = (yyvsp[0].formalParamList); }
#line 2989 "parser.cc"
    break;

  case 129:
#line 704 "parser.yy"
                                     { (yyval.tree) = (yyvsp[0].tree); }
#line 2995 "parser.cc"
    break;

  case 130:
#line 705 "parser.yy"
                                     { (yyval.tree) = (yyvsp[-1].treeSequence); }
#line 3001 "parser.cc"
    break;

  case 131:
#line 706 "parser.yy"
                                     { (yyval.tree) = (yyvsp[-2].treeSequence); }
#line 3007 "parser.cc"
    break;

  case 132:
#line 707 "parser.yy"
                    { (yyval.tree) = new TreeSequence(); }
#line 3013 "parser.cc"
    break;

  case 133:
#line 708 "parser.yy"
                    { (yyval.tree) = new TreeSequence(); }
#line 3019 "parser.cc"
    break;

  case 134:
#line 712 "parser.yy"
                                            { (yyval.treeSequence) = new TreeSequence(); (yyval.treeSequence)->addTree((yyvsp[0].tree)); }
#line 3025 "parser.cc"
    break;

  case 135:
#line 713 "parser.yy"
                                            { (yyval.treeSequence) = (yyvsp[-2].treeSequence); (yyval.treeSequence)->addTree((yyvsp[0].tree)); }
#line 3031 "parser.cc"
    break;

  case 136:
#line 717 "parser.yy"
                                 { (yyval.classDef) = new ClassDef(); }
#line 3037 "parser.cc"
    break;

  case 137:
#line 718 "parser.yy"
                                 { (yyval.classDef) = (yyvsp[0].classDef); }
#line 3043 "parser.cc"
    break;

  case 138:
#line 723 "parser.yy"
                {
                    (yyval.classDef) = new ClassDef();
                    if ((yyvsp[0].classMemberList))
                        for (std::vector<ClassDef::ClassMember *>::iterator it = (yyvsp[0].classMemberList)->begin(); it != (yyvsp[0].classMemberList)->end(); ++it)
                            (yyval.classDef)->addDataMember(*it);
                    else
                        assert(0);
                    delete (yyvsp[0].classMemberList);  // destroy the std::vector<ClassDef::ClassMember *>
                }
#line 3057 "parser.cc"
    break;

  case 139:
#line 733 "parser.yy"
                {
                    (yyval.classDef) = (yyvsp[-1].classDef);
                    if ((yyvsp[0].classMemberList))
                        for (std::vector<ClassDef::ClassMember *>::iterator it = (yyvsp[0].classMemberList)->begin(); it != (yyvsp[0].classMemberList)->end(); ++it)
                            (yyval.classDef)->addDataMember(*it);
                    else
                        assert(0);
                    delete (yyvsp[0].classMemberList);  // destroy the std::vector<ClassDef::ClassMember *>
                }
#line 3071 "parser.cc"
    break;

  case 140:
#line 745 "parser.yy"
                                                             { (yyval.classMemberList) = ClassDef::createClassMembers((yyvsp[-2].declarationSpecifierList), (yyvsp[-1].declaratorVector)); }
#line 3077 "parser.cc"
    break;

  case 141:
#line 749 "parser.yy"
                                                    { (yyval.declaratorVector) = new std::vector<Declarator *>(); if ((yyvsp[0].declarator)) (yyval.declaratorVector)->push_back((yyvsp[0].declarator)); }
#line 3083 "parser.cc"
    break;

  case 142:
#line 750 "parser.yy"
                                                    { (yyval.declaratorVector) = (yyvsp[-2].declaratorVector); if ((yyvsp[0].declarator)) (yyval.declaratorVector)->push_back((yyvsp[0].declarator)); }
#line 3089 "parser.cc"
    break;

  case 143:
#line 755 "parser.yy"
                                        { (yyval.declarator) = (yyvsp[0].declarator); }
#line 3095 "parser.cc"
    break;

  case 144:
#line 757 "parser.yy"
                {
                    (yyval.declarator) = NULL;
                    Declarator temp("<unnamed>", 0, sourceFilename, lineno);
                    temp.setBitFieldWidth(*(yyvsp[0].tree));
                    temp.checkBitField(NULL);
                    delete (yyvsp[0].tree);
                }
#line 3107 "parser.cc"
    break;

  case 145:
#line 765 "parser.yy"
                {
                    (yyval.declarator) = (yyvsp[-2].declarator);
                    (yyval.declarator)->setBitFieldWidth(*(yyvsp[0].tree));
                    delete (yyvsp[0].tree);
                }
#line 3117 "parser.cc"
    break;

  case 146:
#line 773 "parser.yy"
                        { (yyval.compoundStmt) = new CompoundStmt(); if ((yyvsp[0].tree) != NULL) (yyval.compoundStmt)->addTree((yyvsp[0].tree)); }
#line 3123 "parser.cc"
    break;

  case 147:
#line 774 "parser.yy"
                        { (yyval.compoundStmt) = (yyvsp[-1].compoundStmt); if ((yyvsp[0].tree) != NULL) (yyval.compoundStmt)->addTree((yyvsp[0].tree)); }
#line 3129 "parser.cc"
    break;

  case 148:
#line 778 "parser.yy"
                        { (yyval.tree) = (yyvsp[0].tree); }
#line 3135 "parser.cc"
    break;

  case 149:
#line 779 "parser.yy"
                        { (yyval.tree) = ((yyvsp[0].declarationSequence) ? (yyvsp[0].declarationSequence) : new TreeSequence()); }
#line 3141 "parser.cc"
    break;

  case 150:
#line 780 "parser.yy"
                        { (yyval.tree) = (yyvsp[0].compoundStmt); }
#line 3147 "parser.cc"
    break;

  case 151:
#line 781 "parser.yy"
                        { (yyval.tree) = (yyvsp[0].tree); }
#line 3153 "parser.cc"
    break;

  case 152:
#line 782 "parser.yy"
                        { (yyval.tree) = (yyvsp[0].tree); }
#line 3159 "parser.cc"
    break;

  case 153:
#line 783 "parser.yy"
                        { (yyval.tree) = (yyvsp[0].tree); }
#line 3165 "parser.cc"
    break;

  case 154:
#line 784 "parser.yy"
                        { (yyval.tree) = (yyvsp[0].tree); }
#line 3171 "parser.cc"
    break;

  case 155:
#line 785 "parser.yy"
                        { (yyval.tree) = (yyvsp[0].tree); }
#line 3177 "parser.cc"
    break;

  case 156:
#line 786 "parser.yy"
                        { (yyval.tree) = new JumpStmt((yyvsp[-1].str)); free((yyvsp[-1].str)); }
#line 3183 "parser.cc"
    break;

  case 157:
#line 787 "parser.yy"
                        { (yyval.tree) = new JumpStmt(JumpStmt::BRK, NULL); }
#line 3189 "parser.cc"
    break;

  case 158:
#line 788 "parser.yy"
                        { (yyval.tree) = new JumpStmt(JumpStmt::CONT, NULL); }
#line 3195 "parser.cc"
    break;

  case 159:
#line 789 "parser.yy"
                        { (yyval.tree) = new JumpStmt(JumpStmt::RET, NULL); }
#line 3201 "parser.cc"
    break;

  case 160:
#line 790 "parser.yy"
                        { (yyval.tree) = new JumpStmt(JumpStmt::RET, (yyvsp[-1].tree)); }
#line 3207 "parser.cc"
    break;

  case 161:
#line 792 "parser.yy"
                        { (yyval.tree) = new AssemblerStmt((yyvsp[-4].str), (yyvsp[-2].str), true);
                          free((yyvsp[-4].str)); free((yyvsp[-2].str)); }
#line 3214 "parser.cc"
    break;

  case 162:
#line 795 "parser.yy"
                        { (yyval.tree) = new AssemblerStmt((yyvsp[-4].str), (yyvsp[-2].str), false);
                          free((yyvsp[-4].str)); free((yyvsp[-2].str)); }
#line 3221 "parser.cc"
    break;

  case 163:
#line 798 "parser.yy"
                        { (yyval.tree) = new AssemblerStmt((yyvsp[-2].str), "", false);
                          free((yyvsp[-2].str)); }
#line 3228 "parser.cc"
    break;

  case 164:
#line 800 "parser.yy"
                        { (yyval.tree) = new AssemblerStmt(yytext); }
#line 3234 "parser.cc"
    break;

  case 165:
#line 805 "parser.yy"
                                    {
                                      (yyval.tree) = new LabeledStmt((yyvsp[-4].str), TranslationUnit::instance().generateLabel('L'), (yyvsp[0].tree));
                                      (yyval.tree)->setLineNo((yyvsp[-3].str), (yyvsp[-2].integer));
                                      free((yyvsp[-4].str)); free((yyvsp[-3].str));
                                    }
#line 3244 "parser.cc"
    break;

  case 166:
#line 811 "parser.yy"
                                    {
                                      (yyval.tree) = new LabeledStmt((yyvsp[-2].tree), (yyvsp[0].tree));
                                      (yyval.tree)->setLineNo((yyvsp[-4].str), (yyvsp[-3].integer));
                                    }
#line 3253 "parser.cc"
    break;

  case 167:
#line 816 "parser.yy"
                                    {
                                      (yyval.tree) = new LabeledStmt((yyvsp[0].tree));
                                      (yyval.tree)->setLineNo((yyvsp[-3].str), (yyvsp[-2].integer));
                                    }
#line 3262 "parser.cc"
    break;

  case 168:
#line 823 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3268 "parser.cc"
    break;

  case 169:
#line 827 "parser.yy"
                                { (yyval.tree) = new TreeSequence(); }
#line 3274 "parser.cc"
    break;

  case 170:
#line 828 "parser.yy"
                                { (yyval.tree) = (yyvsp[-1].tree); }
#line 3280 "parser.cc"
    break;

  case 171:
#line 832 "parser.yy"
                                { (yyval.tree) = NULL; }
#line 3286 "parser.cc"
    break;

  case 172:
#line 833 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3292 "parser.cc"
    break;

  case 173:
#line 837 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3298 "parser.cc"
    break;

  case 174:
#line 838 "parser.yy"
                                {
                                    Tree *left = (yyvsp[-2].tree);
                                    Tree *right = (yyvsp[0].tree);
                                    if (CommaExpr *ts = dynamic_cast<CommaExpr *>(left))
                                    {
                                        ts->addTree(right);
                                        (yyval.tree) = ts;
                                    }
                                    else
                                        (yyval.tree) = new CommaExpr(left, right);
                                }
#line 3314 "parser.cc"
    break;

  case 175:
#line 852 "parser.yy"
                                            { (yyval.treeSequence) = new TreeSequence(); (yyval.treeSequence)->addTree((yyvsp[0].tree)); }
#line 3320 "parser.cc"
    break;

  case 176:
#line 853 "parser.yy"
                                            { (yyvsp[-2].treeSequence)->addTree((yyvsp[0].tree)); (yyval.treeSequence) = (yyvsp[-2].treeSequence); }
#line 3326 "parser.cc"
    break;

  case 177:
#line 857 "parser.yy"
                                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3332 "parser.cc"
    break;

  case 178:
#line 863 "parser.yy"
                { (yyval.tree) = new BinaryOpExpr((yyvsp[-1].binop), (yyvsp[-4].tree), (yyvsp[0].tree)); free((yyvsp[-3].str)); }
#line 3338 "parser.cc"
    break;

  case 179:
#line 867 "parser.yy"
                                                { (yyval.binop) = BinaryOpExpr::ASSIGNMENT; }
#line 3344 "parser.cc"
    break;

  case 180:
#line 868 "parser.yy"
                                                { (yyval.binop) = BinaryOpExpr::INC_ASSIGN; }
#line 3350 "parser.cc"
    break;

  case 181:
#line 869 "parser.yy"
                                                { (yyval.binop) = BinaryOpExpr::DEC_ASSIGN; }
#line 3356 "parser.cc"
    break;

  case 182:
#line 870 "parser.yy"
                                                { (yyval.binop) = BinaryOpExpr::MUL_ASSIGN; }
#line 3362 "parser.cc"
    break;

  case 183:
#line 871 "parser.yy"
                                                { (yyval.binop) = BinaryOpExpr::DIV_ASSIGN; }
#line 3368 "parser.cc"
    break;

  case 184:
#line 872 "parser.yy"
                                                { (yyval.binop) = BinaryOpExpr::MOD_ASSIGN; }
#line 3374 "parser.cc"
    break;

  case 185:
#line 873 "parser.yy"
                                                { (yyval.binop) = BinaryOpExpr::XOR_ASSIGN; }
#line 3380 "parser.cc"
    break;

  case 186:
#line 874 "parser.yy"
                                                { (yyval.binop) = BinaryOpExpr::AND_ASSIGN; }
#line 3386 "parser.cc"
    break;

  case 187:
#line 875 "parser.yy"
                                                { (yyval.binop) = BinaryOpExpr::OR_ASSIGN; }
#line 3392 "parser.cc"
    break;

  case 188:
#line 876 "parser.yy"
                                                { (yyval.binop) = BinaryOpExpr::LEFT_ASSIGN; }
#line 3398 "parser.cc"
    break;

  case 189:
#line 877 "parser.yy"
                                                { (yyval.binop) = BinaryOpExpr::RIGHT_ASSIGN; }
#line 3404 "parser.cc"
    break;

  case 190:
#line 881 "parser.yy"
                                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3410 "parser.cc"
    break;

  case 191:
#line 883 "parser.yy"
                                                { (yyval.tree) = new ConditionalExpr((yyvsp[-4].tree), (yyvsp[-2].tree), (yyvsp[0].tree)); }
#line 3416 "parser.cc"
    break;

  case 192:
#line 887 "parser.yy"
                                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3422 "parser.cc"
    break;

  case 193:
#line 889 "parser.yy"
                { (yyval.tree) = new BinaryOpExpr(BinaryOpExpr::LOGICAL_OR, (yyvsp[-2].tree), (yyvsp[0].tree)); }
#line 3428 "parser.cc"
    break;

  case 194:
#line 893 "parser.yy"
                                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3434 "parser.cc"
    break;

  case 195:
#line 895 "parser.yy"
                { (yyval.tree) = new BinaryOpExpr(BinaryOpExpr::LOGICAL_AND, (yyvsp[-2].tree), (yyvsp[0].tree)); }
#line 3440 "parser.cc"
    break;

  case 196:
#line 899 "parser.yy"
                                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3446 "parser.cc"
    break;

  case 197:
#line 901 "parser.yy"
                { (yyval.tree) = new BinaryOpExpr(BinaryOpExpr::BITWISE_OR, (yyvsp[-2].tree), (yyvsp[0].tree)); }
#line 3452 "parser.cc"
    break;

  case 198:
#line 904 "parser.yy"
                                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3458 "parser.cc"
    break;

  case 199:
#line 906 "parser.yy"
                { (yyval.tree) = new BinaryOpExpr(BinaryOpExpr::BITWISE_XOR, (yyvsp[-2].tree), (yyvsp[0].tree)); }
#line 3464 "parser.cc"
    break;

  case 200:
#line 909 "parser.yy"
                                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3470 "parser.cc"
    break;

  case 201:
#line 911 "parser.yy"
                { (yyval.tree) = new BinaryOpExpr(BinaryOpExpr::BITWISE_AND, (yyvsp[-2].tree), (yyvsp[0].tree)); }
#line 3476 "parser.cc"
    break;

  case 202:
#line 914 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3482 "parser.cc"
    break;

  case 203:
#line 916 "parser.yy"
                                { (yyval.tree) = new BinaryOpExpr((yyvsp[-1].binop), (yyvsp[-2].tree), (yyvsp[0].tree)); }
#line 3488 "parser.cc"
    break;

  case 204:
#line 920 "parser.yy"
                                { (yyval.binop) = BinaryOpExpr::EQUALITY; }
#line 3494 "parser.cc"
    break;

  case 205:
#line 921 "parser.yy"
                                { (yyval.binop) = BinaryOpExpr::INEQUALITY; }
#line 3500 "parser.cc"
    break;

  case 206:
#line 925 "parser.yy"
                                        { (yyval.tree) = (yyvsp[0].tree); }
#line 3506 "parser.cc"
    break;

  case 207:
#line 926 "parser.yy"
                                        { (yyval.tree) = new BinaryOpExpr((yyvsp[-1].binop), (yyvsp[-2].tree), (yyvsp[0].tree)); }
#line 3512 "parser.cc"
    break;

  case 208:
#line 930 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3518 "parser.cc"
    break;

  case 209:
#line 931 "parser.yy"
                                { (yyval.tree) = new BinaryOpExpr(
                                        BinaryOpExpr::LEFT_SHIFT, (yyvsp[-2].tree), (yyvsp[0].tree)); }
#line 3525 "parser.cc"
    break;

  case 210:
#line 933 "parser.yy"
                                { (yyval.tree) = new BinaryOpExpr(
                                        BinaryOpExpr::RIGHT_SHIFT, (yyvsp[-2].tree), (yyvsp[0].tree)); }
#line 3532 "parser.cc"
    break;

  case 211:
#line 938 "parser.yy"
                                { (yyval.binop) = BinaryOpExpr::INFERIOR; }
#line 3538 "parser.cc"
    break;

  case 212:
#line 939 "parser.yy"
                                { (yyval.binop) = BinaryOpExpr::INFERIOR_OR_EQUAL; }
#line 3544 "parser.cc"
    break;

  case 213:
#line 940 "parser.yy"
                                { (yyval.binop) = BinaryOpExpr::SUPERIOR; }
#line 3550 "parser.cc"
    break;

  case 214:
#line 941 "parser.yy"
                                { (yyval.binop) = BinaryOpExpr::SUPERIOR_OR_EQUAL; }
#line 3556 "parser.cc"
    break;

  case 215:
#line 945 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3562 "parser.cc"
    break;

  case 216:
#line 946 "parser.yy"
                                { (yyval.tree) = new BinaryOpExpr((yyvsp[-1].binop), (yyvsp[-2].tree), (yyvsp[0].tree)); }
#line 3568 "parser.cc"
    break;

  case 217:
#line 950 "parser.yy"
                                { (yyval.binop) = BinaryOpExpr::ADD; }
#line 3574 "parser.cc"
    break;

  case 218:
#line 951 "parser.yy"
                                { (yyval.binop) = BinaryOpExpr::SUB; }
#line 3580 "parser.cc"
    break;

  case 219:
#line 955 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3586 "parser.cc"
    break;

  case 220:
#line 956 "parser.yy"
                                { (yyval.tree) = new BinaryOpExpr((yyvsp[-1].binop), (yyvsp[-2].tree), (yyvsp[0].tree)); }
#line 3592 "parser.cc"
    break;

  case 221:
#line 960 "parser.yy"
                                { (yyval.binop) = BinaryOpExpr::MUL; }
#line 3598 "parser.cc"
    break;

  case 222:
#line 961 "parser.yy"
                                { (yyval.binop) = BinaryOpExpr::DIV; }
#line 3604 "parser.cc"
    break;

  case 223:
#line 962 "parser.yy"
                                { (yyval.binop) = BinaryOpExpr::MOD; }
#line 3610 "parser.cc"
    break;

  case 224:
#line 966 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3616 "parser.cc"
    break;

  case 225:
#line 967 "parser.yy"
                                {
                                    RealConstantExpr *rce;
                                    DWordConstantExpr *dwce;
                                    if ((yyvsp[-1].unop) == UnaryOpExpr::NEG && (rce = dynamic_cast<RealConstantExpr *>((yyvsp[0].tree))) != NULL)
                                    {
                                        // We have the negation of a real constant.
                                        // Simplify by negating the value in the RealConstantExpr and getting rid of the negation operator.
                                        rce->negateValue();
                                        (yyval.tree) = rce;
                                    }
                                    else if ((yyvsp[-1].unop) == UnaryOpExpr::NEG && (dwce = dynamic_cast<DWordConstantExpr *>((yyvsp[0].tree))) != NULL)
                                    {
                                        dwce->negateValue();
                                        (yyval.tree) = dwce;
                                    }
                                    else
                                    {
                                        (yyval.tree) = new UnaryOpExpr((yyvsp[-1].unop), (yyvsp[0].tree));
                                    }
                                }
#line 3641 "parser.cc"
    break;

  case 226:
#line 987 "parser.yy"
                                { (yyval.tree) = new UnaryOpExpr(UnaryOpExpr::PREINC, (yyvsp[0].tree)); }
#line 3647 "parser.cc"
    break;

  case 227:
#line 988 "parser.yy"
                                { (yyval.tree) = new UnaryOpExpr(UnaryOpExpr::PREDEC, (yyvsp[0].tree)); }
#line 3653 "parser.cc"
    break;

  case 228:
#line 989 "parser.yy"
                                { (yyval.tree) = new UnaryOpExpr((yyvsp[-1].typeDesc)); }
#line 3659 "parser.cc"
    break;

  case 229:
#line 990 "parser.yy"
                                { (yyval.tree) = new UnaryOpExpr(UnaryOpExpr::SIZE_OF, (yyvsp[0].tree)); }
#line 3665 "parser.cc"
    break;

  case 230:
#line 994 "parser.yy"
                        { (yyval.unop) = UnaryOpExpr::IDENTITY; }
#line 3671 "parser.cc"
    break;

  case 231:
#line 995 "parser.yy"
                        { (yyval.unop) = UnaryOpExpr::NEG; }
#line 3677 "parser.cc"
    break;

  case 232:
#line 996 "parser.yy"
                        { (yyval.unop) = UnaryOpExpr::ADDRESS_OF; }
#line 3683 "parser.cc"
    break;

  case 233:
#line 997 "parser.yy"
                        { (yyval.unop) = UnaryOpExpr::INDIRECTION; }
#line 3689 "parser.cc"
    break;

  case 234:
#line 998 "parser.yy"
                        { (yyval.unop) = UnaryOpExpr::BOOLEAN_NEG; }
#line 3695 "parser.cc"
    break;

  case 235:
#line 999 "parser.yy"
                        { (yyval.unop) = UnaryOpExpr::BITWISE_NOT; }
#line 3701 "parser.cc"
    break;

  case 236:
#line 1003 "parser.yy"
                                    { (yyval.tree) = (yyvsp[0].tree); }
#line 3707 "parser.cc"
    break;

  case 237:
#line 1004 "parser.yy"
                                    { (yyval.tree) = new CastExpr((yyvsp[-2].typeDesc), (yyvsp[0].tree)); }
#line 3713 "parser.cc"
    break;

  case 238:
#line 1008 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3719 "parser.cc"
    break;

  case 239:
#line 1010 "parser.yy"
                        { (yyval.tree) = new FunctionCallExpr((yyvsp[-4].tree), new TreeSequence());
                          free((yyvsp[-3].str)); }
#line 3726 "parser.cc"
    break;

  case 240:
#line 1013 "parser.yy"
                        { (yyval.tree) = new FunctionCallExpr((yyvsp[-5].tree), (yyvsp[-1].treeSequence));
                          free((yyvsp[-4].str)); }
#line 3733 "parser.cc"
    break;

  case 241:
#line 1016 "parser.yy"
                        { (yyval.tree) = new BinaryOpExpr(BinaryOpExpr::ARRAY_REF, (yyvsp[-3].tree), (yyvsp[-1].tree)); }
#line 3739 "parser.cc"
    break;

  case 242:
#line 1018 "parser.yy"
                        { (yyval.tree) = new UnaryOpExpr(UnaryOpExpr::POSTINC, (yyvsp[-1].tree)); }
#line 3745 "parser.cc"
    break;

  case 243:
#line 1020 "parser.yy"
                        { (yyval.tree) = new UnaryOpExpr(UnaryOpExpr::POSTDEC, (yyvsp[-1].tree)); }
#line 3751 "parser.cc"
    break;

  case 244:
#line 1022 "parser.yy"
                        { (yyval.tree) = new ObjectMemberExpr((yyvsp[-2].tree), (yyvsp[0].str), true); free((yyvsp[0].str)); }
#line 3757 "parser.cc"
    break;

  case 245:
#line 1024 "parser.yy"
                        { (yyval.tree) = new ObjectMemberExpr((yyvsp[-2].tree), (yyvsp[0].str), false); free((yyvsp[0].str)); }
#line 3763 "parser.cc"
    break;

  case 246:
#line 1028 "parser.yy"
                        { (yyval.tree) = new IdentifierExpr((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 3769 "parser.cc"
    break;

  case 247:
#line 1029 "parser.yy"
                        {
                            bool isHexOrBin = (yytext[0] == '0' && (tolower(yytext[1]) == 'x' || tolower(yytext[1]) == 'b'));
                            double value = (yyvsp[0].real);
                            if (!isHexOrBin
                                    && (strchr(yytext, '.') || strchr(yytext, 'e') || strchr(yytext, 'E')))  // if point or exponential
                            {
                                (yyval.tree) = new RealConstantExpr(value, yytext);
                            }
                            else if (strchr(yytext, 'l') || strchr(yytext, 'L') || value > 65535.0 || value <= -32769.0)
                            {
                                bool uSuffix = (strchr(yytext, 'u') || strchr(yytext, 'U'));
                                (yyval.tree) = new DWordConstantExpr(value, !uSuffix && value <= 0x7FFFFFFFUL);
                            } 
                            else
                            {
                                (yyval.tree) = new WordConstantExpr(value, yytext);
                            }
                        }
#line 3792 "parser.cc"
    break;

  case 248:
#line 1047 "parser.yy"
                        { (yyval.tree) = new WordConstantExpr((int8_t) (yyvsp[0].character), false, TranslationUnit::instance().isCharSignedByDefault()); }
#line 3798 "parser.cc"
    break;

  case 249:
#line 1048 "parser.yy"
                        { (yyval.tree) = new StringLiteralExpr((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 3804 "parser.cc"
    break;

  case 250:
#line 1049 "parser.yy"
                        { (yyval.tree) = (yyvsp[-1].tree); }
#line 3810 "parser.cc"
    break;

  case 251:
#line 1053 "parser.yy"
                        { (yyval.str) = (yyvsp[0].str); }
#line 3816 "parser.cc"
    break;

  case 252:
#line 1054 "parser.yy"
                        {
                            (yyval.str) = (char *) malloc(strlen((yyvsp[-1].str)) + strlen((yyvsp[0].str)) + 1);
                            strcpy((yyval.str), (yyvsp[-1].str));
                            strcat((yyval.str), (yyvsp[0].str));
                            free((yyvsp[-1].str)); free((yyvsp[0].str));
                        }
#line 3827 "parser.cc"
    break;

  case 253:
#line 1063 "parser.yy"
                        { (yyval.treeSequence) = NULL; }
#line 3833 "parser.cc"
    break;

  case 254:
#line 1064 "parser.yy"
                        { (yyval.treeSequence) = (yyvsp[0].treeSequence); }
#line 3839 "parser.cc"
    break;

  case 255:
#line 1068 "parser.yy"
                                { (yyval.treeSequence) = new TreeSequence(); (yyval.treeSequence)->addTree((yyvsp[0].tree)); }
#line 3845 "parser.cc"
    break;

  case 256:
#line 1069 "parser.yy"
                                { (yyval.treeSequence) = (yyvsp[-2].treeSequence); (yyval.treeSequence)->addTree((yyvsp[0].tree)); }
#line 3851 "parser.cc"
    break;

  case 257:
#line 1073 "parser.yy"
                                                { (yyval.tree) = new IfStmt((yyvsp[-3].tree), (yyvsp[-1].tree), (yyvsp[0].tree)); }
#line 3857 "parser.cc"
    break;

  case 258:
#line 1074 "parser.yy"
                                                { (yyval.tree) = new SwitchStmt((yyvsp[-2].tree), (yyvsp[0].tree)); }
#line 3863 "parser.cc"
    break;

  case 259:
#line 1078 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3869 "parser.cc"
    break;

  case 260:
#line 1082 "parser.yy"
                                { (yyval.tree) = NULL; }
#line 3875 "parser.cc"
    break;

  case 261:
#line 1083 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3881 "parser.cc"
    break;

  case 262:
#line 1087 "parser.yy"
                                        { (yyval.tree) = new WhileStmt((yyvsp[-2].tree), (yyvsp[0].tree), false); }
#line 3887 "parser.cc"
    break;

  case 263:
#line 1091 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].tree); }
#line 3893 "parser.cc"
    break;

  case 264:
#line 1095 "parser.yy"
                                                { (yyval.tree) = new WhileStmt((yyvsp[-2].tree), (yyvsp[-5].tree), true); }
#line 3899 "parser.cc"
    break;

  case 265:
#line 1099 "parser.yy"
                                { (yyval.tree) = (yyvsp[-1].treeSequence); }
#line 3905 "parser.cc"
    break;

  case 266:
#line 1100 "parser.yy"
                                { (yyval.tree) = (yyvsp[0].declarationSequence); }
#line 3911 "parser.cc"
    break;

  case 267:
#line 1108 "parser.yy"
                                { if ( (yyvsp[-11].tree))  (yyvsp[-11].tree)->setLineNo((yyvsp[-13].str), (yyvsp[-12].integer));
                                  if ( (yyvsp[-8].tree))  (yyvsp[-8].tree)->setLineNo((yyvsp[-10].str), (yyvsp[-9].integer));
                                  if ((yyvsp[-4].treeSequence)) (yyvsp[-4].treeSequence)->setLineNo((yyvsp[-6].str), (yyvsp[-5].integer));
                                  (yyval.tree) = new ForStmt((yyvsp[-11].tree), (yyvsp[-8].tree), (yyvsp[-4].treeSequence), (yyvsp[0].tree));
                                  free((yyvsp[-13].str)); free((yyvsp[-10].str)); free((yyvsp[-6].str)); free((yyvsp[-2].str));
                                }
#line 3922 "parser.cc"
    break;


#line 3926 "parser.cc"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1116 "parser.yy"


#if 0
void _PARSERTRACE(int parserLineNo, const char *fmt, ...)
{
    printf("# P%d U%d: ", parserLineNo, lineno);
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    putchar('\n');
    fflush(stdout);
}
#endif

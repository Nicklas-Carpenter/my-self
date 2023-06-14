#include <alloca.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG_MSG

/*** Lexer ***/

enum SymType {
	UNDEF_SYM,
	EOS,
	INT_SYM,
	BIN_OP_SYM
};

union SymVal {
	int as_int;
	char as_bin_op;
};

typedef struct {
	enum SymType type;
	union SymVal value;
} Symbol;

char lex_buf[64];

static FILE *text;

static Symbol *cur_sym = NULL;
static Symbol *lookahead_symbol = NULL;
static char cur_char = '\0';

#ifdef DEBUG_MSG
int (*d_puts)(const char *s) = puts;
int (*d_printf)(const char *restrict format, ...) = printf;
#else
void d_puts() {}
void d_printf() {}
#endif // DEBUG_MSG

// Print a formatted error message from errno
void pr_error(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	
	char err_str[1024];
	snprintf(err_str, 1024, format, ap); 

	perror(err_str);
	va_end(ap);
}

// Read a single ASCII character from the input stream.
static inline int read_char(char *buf)
{
	return fread(buf, 1, 1, text);
}

// Read the next few digits from the input stream and copy them to the lexer's
// buffer
static void read_number(char first)
{
	lex_buf[0] = first;

	size_t bytes_read = read_char(&cur_char);
	if (bytes_read < 0)
		pr_error("%s (line %d)", __func__, __LINE__);
	int i = 1;
	while(bytes_read > 0 && isdigit(cur_char)) {
		bytes_read = read_char(&cur_char);
		if (bytes_read < 0)
			pr_error("%s (line %d)", __func__, __LINE__);

		lex_buf[i++] = cur_char;
	}

	lex_buf[i] = '\0';
}

// Create an integer symbol with the given integer value
Symbol *int_symbol(int value) {
	Symbol *symbol = malloc(sizeof(Symbol));

	if (symbol == NULL) {
		pr_error("%s (line %d)", __func__, __LINE__);	
	}
		
	*symbol = (Symbol){
		.type = INT_SYM,
		.value = value,
	};

	return symbol;
}

// Create a binary op symbol with the given operator character
Symbol *bin_op_symbol(char op) {
	Symbol *symbol = malloc(sizeof(Symbol));

	if (symbol == NULL) {
		pr_error("%s (line %d)", __func__, __LINE__);	
	}
		
	*symbol = (Symbol){
		.type = BIN_OP_SYM,
		.value = op,
	};

	return symbol;
}

// Return a symbol denoting the end of the stream (EOS)
Symbol *eos_symbol() {
	Symbol *symbol = malloc(sizeof(Symbol));

	if (symbol == NULL) {
		pr_error("%s (line %d)", __func__, __LINE__);	
	}
	symbol->type = EOS;		

	return symbol;
}
	
// Get the next symbol. Used for both lookahead and direct use.
Symbol *next()
{
	size_t bytes_read;

	while(bytes_read = read_char(&cur_char)) {
		if (bytes_read < 0)
			pr_error("%s (line %d)", __func__, __LINE__);

		if (isspace(cur_char)) {
			continue;
		}

		if (isdigit(cur_char)) {
			read_number(cur_char);
			d_puts("Found INT sym");
			return int_symbol(atoi(lex_buf));
		} else if (cur_char == '+') {
			d_puts("Found BIN_OP sym");
			return bin_op_symbol(cur_char);
		} else {
			d_printf("%s (line %d): Unexpected character %c\n",
				__func__, __LINE__, cur_char);
		}
	}

	d_puts("Reached end of stream");
	return eos_symbol();
}

// Lex and return the next symbol from the input stream but do not overwrite
// the current symbol (yet)
Symbol* lookahead_sym()
{
	// Prevent loss of symbol if lookahead_sym is called more than once
	// between calls to next_sym
	if (lookahead_symbol == NULL) 
		return (lookahead_symbol = next());	

	return lookahead_symbol;
}

// Advance the current symbol to the next symbol lexed from the stream
void next_sym()
{
	if (lookahead_symbol != NULL) {
		cur_sym = lookahead_symbol;
		lookahead_symbol = NULL;
	} else {
		cur_sym = next();
	}
}

/*** Parser ***/

enum AstNodeType {
	UNDEF_NODE,
	BIN_MSG_NODE,
	INT_NODE,
};

#define AST_NODE_CLASS enum AstNodeType type

typedef struct {
	AST_NODE_CLASS;
} AstNode;

typedef struct {
	AST_NODE_CLASS;
	AstNode *rcvr;
	Symbol *bin_op;
	AstNode *arg;
} BinMsgAstNode;

typedef struct {
	AST_NODE_CLASS;
	Symbol *value;
} IntAstNode;

AstNode *int_ast_node(Symbol *sym)
{
	d_puts("Constructing int AST node");
	IntAstNode *node = malloc(sizeof(IntAstNode));
	*node = (IntAstNode){.type = INT_NODE, .value = sym};
	return (AstNode *)node;
}

AstNode *bin_msg_ast_node(AstNode *rcvr, Symbol *op, AstNode *arg)
{
	d_puts("Constructing Binary message AST node");
	BinMsgAstNode *node = malloc(sizeof(BinMsgAstNode));
	*node = (BinMsgAstNode){
			.type = BIN_MSG_NODE,
			.rcvr = rcvr,
			.bin_op = op,
			.arg = arg
	};

	return (AstNode *)node;
}

// Parse an expression from input stream
AstNode *parse_expr()
{
	d_puts("Parsing expression");
	next_sym();

	switch(cur_sym->type) {
	case INT_SYM:
		d_puts("Found int symbol");
		if (lookahead_sym()->type == BIN_OP_SYM) {
			d_puts("Lookahead is a bin op");
			AstNode *rcvr = int_ast_node(cur_sym);

			next_sym();
			Symbol *op = cur_sym;

			AstNode *arg = parse_expr();

			return bin_msg_ast_node(rcvr, op, arg);
		}

		d_puts("Not bin op expression");
		return int_ast_node(cur_sym);
	default:
		d_puts("Unrecognized expression");
	}

	return NULL;
}

// Parse the input. Currently we only support expressions.
AstNode *parse_prog()
{
	d_puts("Parsing prog");
	return parse_expr();
}

int main() {
	text = fopen("./test.self", "r");
	if (text < 0) 
		pr_error("%s (line %d). Failed to open file", __func__, __LINE__);

	parse_prog();
	return 0;
}

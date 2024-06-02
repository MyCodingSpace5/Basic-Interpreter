namespace lol
{
    enum States
    {
        START,
        NUMBER,
        STRING,
        UNDEFINED,
        NULL_STATE, 
        COMPARISON,
        DELIMITER,
        VARIABLE,
        OPERATOR,
        END,
        FUNCTION,
    };

    enum Formatting
    {
        BOOLEAN,
        TRUE,
        FALSE,
        RETURN
    };

    struct Token
    {
        States v;
        long a; 
    };

    struct AbstractSyntaxTree
    {
        int returnValue;
        Formatting context;
        Token* amp;
        int ampSize;
    };

    struct CompleteTree
    {
        AbstractSyntaxTree* o;
        int oSize;
        Formatting finalCondition;
        long* m;
        int mSize;
    };

    struct SymbolTable
    {
        const States m = VARIABLE;
        long* a;
        int aSize;
    };

    class Lexer
    {
    public:
        Lexer(const char* source) : source(source), position(0) {}
        Token* tokenize(int& tokenCount);

    private:
        const char* source;
        int position;

        char currentChar();
        void advance();
        void skipWhitespace();
        Token createNumber();
        Token createString();
        Token createIdentifierOrKeyword();
        Token createOperator();
        Token createDelimiter();
    };

    char Lexer::currentChar()
    {
        return source[position];
    }

    void Lexer::advance()
    {
        position++;
    }

    void Lexer::skipWhitespace()
    {
        while (source[position] == ' ' || source[position] == '\t' || source[position] == '\n' || source[position] == '\r')
        {
            advance();
        }
    }

    Token Lexer::createNumber()
    {
        long value = 0;
        while (source[position] >= '0' && source[position] <= '9')
        {
            value = value * 10 + (source[position] - '0');
            advance();
        }
        return { NUMBER, value };
    }

    Token Lexer::createString()
    {
        advance();
        long value = 0;
        while (source[position] != '"' && source[position] != '\0')
        {
            value = value * 256 + source[position];
            advance();
        }
        advance(); 
        return { STRING, value };
    }

    Token Lexer::createIdentifierOrKeyword()
    {
        long value = 0;
        while ((source[position] >= 'a' && source[position] <= 'z') || (source[position] >= 'A' && source[position] <= 'Z') || (source[position] >= '0' && source[position] <= '9') || source[position] == '_')
        {
            value = value * 256 + source[position]; 
            advance();
        }
        return { VARIABLE, value };
    }

    Token Lexer::createOperator()
    {
        char op = currentChar();
        advance();
        return { OPERATOR, op };
    }

    Token Lexer::createDelimiter()
    {
        char delim = currentChar();
        advance();
        return { DELIMITER, delim };
    }

    Token* Lexer::tokenize(int& tokenCount)
    {
        Token* tokens = new Token[100]; 
        tokenCount = 0;

        while (source[position] != '\0')
        {
            skipWhitespace();

            if (source[position] >= '0' && source[position] <= '9')
            {
                tokens[tokenCount++] = createNumber();
            }
            else if (source[position] == '"')
            {
                tokens[tokenCount++] = createString();
            }
            else if ((source[position] >= 'a' && source[position] <= 'z') || (source[position] >= 'A' && source[position] <= 'Z') || source[position] == '_')
            {
                tokens[tokenCount++] = createIdentifierOrKeyword();
            }
            else if (source[position] == '+' || source[position] == '-' || source[position] == '*' || source[position] == '/')
            {
                tokens[tokenCount++] = createOperator();
            }
            else if (source[position] == ';' || source[position] == ',' || source[position] == '(' || source[position] == ')')
            {
                tokens[tokenCount++] = createDelimiter();
            }
            else
            {
                tokens[tokenCount++] = { UNDEFINED, source[position] };
                advance();
            }
        }

        tokens[tokenCount++] = { END, 0 };
        return tokens;
    }

    void iteration(int iterator, CompleteTree& v, Formatting context, SymbolTable& o, int position)
    {
        if (position >= v.oSize || iterator >= v.o[position].ampSize)
            return;

        switch (v.o[position].amp[iterator].v)
        {
            case END:
                iteration(iterator, v, context, o, position + 1);
                break;
            case START:
                iteration(iterator + 1, v, context, o, position);
                break;
            case STRING:
                v.o[position].returnValue += v.o[position].amp[iterator].a;
                iteration(iterator + 1, v, context, o, position);
                break;
            case NUMBER:
                v.o[position].returnValue += v.o[position].amp[iterator].a;
                iteration(iterator + 1, v, context, o, position);
                break;
            case DELIMITER:
                iteration(iterator + 1, v, context, o, position);
                break;
            case COMPARISON:
                v.o[position].returnValue -= v.o[position].amp[iterator].a;
                v.o[position].context = BOOLEAN;
                iteration(iterator + 1, v, context, o, position);
                break;
            case FUNCTION:
                v.o[position].returnValue = o.a[position + iterator];
                v.o[position].context = RETURN;
                iteration(iterator + 1, v, context, o, position);
                break;
            case VARIABLE:
                v.o[position].returnValue += v.o[position].amp[iterator].a;
                iteration(iterator + 1, v, context, o, position);
                break;
            default:
                break;
        }
    }
    AbstractSyntaxTree Parser(Token a[],  int line)
    {
        const int token_size = 12;
        AbstractSyntaxTree v;
        v.amp = a;
        long m = (long)(&a);
        v.ampSize = m/token_size;
        return v;
    }
    void context_preservation(int iterator, CompleteTree& v, int value)
    {
        if (iterator >= v.oSize)
            return;

        value += v.o[iterator].returnValue;
        v.finalCondition = v.o[iterator].context;
        context_preservation(iterator + 1, v, value);
    }

    struct SemanticState
    {
        long a;
        Formatting b;
    };

    struct SemanticResult
    {
        Formatting a;
        bool prone;
    };

    void SemanticAnalysis(CompleteTree &a)
    {
        SemanticState v;
        v.a = (long)a.m;
        v.b = a.finalCondition;
        bool validation;
        int value; 
        switch(v.b)
        {
            case BOOLEAN:
                validation = v.a;
                break;
            case TRUE:
                validation = v.a;
                break;
            case FALSE:
                validation = v.a;
                break;
            case RETURN:
                value = (int)(v.a);
                break;
            default:
                break;               
        }
    }
    
}

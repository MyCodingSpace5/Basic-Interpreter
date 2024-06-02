int main() {
    const char* source = "5*(3+2)";

    Lexer lexer(source);
    int tokenCount;
    Token* tokens = lexer.tokenize(tokenCount);

    // Parse the tokens
    AbstractSyntaxTree ast = Parser(tokens, tokenCount);

    CompleteTree completeTree;
    completeTree.o = &ast;
    completeTree.oSize = 1; 
    completeTree.finalCondition = BOOLEAN; 

    
    SemanticAnalysis(completeTree);

    
    if (completeTree.finalCondition == BOOLEAN) {
        cout << "Semantic analysis result: Boolean" << endl;
    }
    else if (completeTree.finalCondition == TRUE) {
        cout << "Semantic analysis result: True" << endl;
    }
    else if (completeTree.finalCondition == FALSE) {
        cout << "Semantic analysis result: False" << endl;
    }
    else if (completeTree.finalCondition == RETURN) {
        cout << "Semantic analysis result: Return with value " << completeTree.m[0] << endl;
    }


    delete[] tokens;

    return 0;
}

#include "parserTests.h"
#include "../include/parser.h"
#include "../include/tokenizer.h"
#include "../library/include/filesystem.h"


// - - - @Asher: expectTests does not support nodes. So, I am just printing the nodes for now
// - - - Check Assignment Statements - - -
u8 checkParserAssignment()
{
    Program program;
    std::vector<Node*> statements;
    program.statements = statements;
    
    // - - - For simple assign Statement
    const char* customString = "Plag myVar = 1";
    std::vector<Token> tokens = tokenize(customString);
    
    createLinearAllocator(1024 * 1024, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");

    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);
    program.statements.clear();
    FORGE_LOG_DEBUG("==============================================================");
    
    customString = "Plag myVar = 1+2";
    tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, NULL, &program.allocator);
    produceAST(&tokens, &program);

    FORGE_LOG_DEBUG("Printing all nodes");
    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");

    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);

    FORGE_LOG_INFO("ASSIGNMNET STATEMENT TESTS COMPLETED");
    return 1;
}

// - - - Check Return Statements - - -
u8 checkParserReturn()
{
    Program program;
    std::vector<Node*> statements;
    program.statements = statements;

    // - - - For simple return statement
    const char* customString = "return 1";
    std::vector<Token> tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");

    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);
    program.statements.clear();
    FORGE_LOG_DEBUG("==============================================================");

    customString = "return 1+2";
    tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");

    for(u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);
    FORGE_LOG_INFO("RETURN STATEMENT TESTS COMPLETED");
    return 1;
}

// - - - Check Expression Statements - - -
u8 checkParserExpression()
{
    Program program;
    std::vector<Node*> statements;
    program.statements = statements;

    // - - - For simple expression statement
    const char* customString = "1+2-3";
    std::vector<Token> tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");


    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);
    program.statements.clear();
    FORGE_LOG_DEBUG("==============================================================");

    customString = "1+2*3/4";
    tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");

    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);\
    program.statements.clear();
    FORGE_LOG_DEBUG("==============================================================");

    customString = "1 > 2";
    tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");

    for(u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);
    program.statements.clear();

    FORGE_LOG_INFO("EXPRESSION STATEMENT TESTS COMPLETED");
    return 1;
}

// - - - Check If Block - - -
u8 checkParserIfBlock()
{
    Program program;
    std::vector<Node*> statements;
    program.statements = statements;

    // - - - For simple if block
    const char* customString = "if (1 > 2) { return 1 }";
    std::vector<Token> tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");


    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);
    program.statements.clear();
    FORGE_LOG_DEBUG("==============================================================");

    customString = "if (1 > 2) { return 1 } else { return 2 }";
    tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");


    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);

    FORGE_LOG_INFO("IF BLOCK TESTS COMPLETED");
    return 1;
}

// - - - Check Function Block - - -
u8 checkParserFunctionBlock()
{
    Program program;
    std::vector<Node*> statements;
    program.statements = statements;

    // - - - For simple function block
    const char* customString = "Fn() { return 1 }";
    std::vector<Token> tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");

    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);
    program.statements.clear();
    FORGE_LOG_DEBUG("==============================================================");

    customString = "Fn(x,y) { return 1+2 }";
    tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");


    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);

    FORGE_LOG_INFO("FUNCTION BLOCK TESTS COMPLETED");
    return 1;
}

int main()
{
    registerTest(checkParserAssignment, "Check if we can parse assignment statements");
    registerTest(checkParserReturn, "Check if we can parse return statements");
    registerTest(checkParserExpression, "Check if we can parse expression statements");
    registerTest(checkParserIfBlock, "Check if we can parse if blocks");
    registerTest(checkParserFunctionBlock, "Check if we can parse function blocks");
    runTests();
    return 0;
}
#ifndef iiit_SCANNER_HPP
#define iiit_SCANNER_HPP

#include "common.hpp"

// - - - Turns raw '.iiitd' source text into a stream of Tokens, one at a time, for the compiler to consume later.
class _Scanner{
public:
 typedef enum{
  // - - - single character tokens
  OPEN_PARANTHESIS, CLOSE_PARANTHESIS,
  OPEN_BRACE, CLOSE_BRACE,
  COMMA,
  ADD, SUB, MUL, DIV,

  // - - - one or two character tokens
  NOT, NOT_EQUALS,
  ASSIGN, EQUALS,
  GREATER, GREATER_EQUAL,
  LESSER, LESSER_EQUAL,

  // - - - literals
  IDENTIFIER, NUMBER, STRING,

  // - - - keywords (see tokenizer on the eval/parser branches for why these are what they are)
  IF, ELSE, FOR, WHILE, RETURN, FUNCTION, PLAG, DAC, TRUE, FALSE,

  // - - - housekeeping
  TOKEN_ERROR, TOKEN_EOF,
  TOKEN_TYPES_COUNT,
 } _TOKEN_TYPE;

 struct _TOKEN{
  _TOKEN_TYPE mTYPE;
  const char* mSTART;
  int         mLENGTH;
  int         mLINE;
 };

 void   _initSCANNER(const char* _SOURCE);
 _TOKEN _scanTOKEN();

private:
 const char* mSTART;
 const char* mCURRENT;
 int         mLINE;

 bool _isAtEND();
 char _advance();
 char _peek();
 char _peekNEXT();
 bool _match(char _EXPECTED);
 void _skipWHITESPACE();

 _TOKEN _makeTOKEN(_TOKEN_TYPE _TYPE);
 _TOKEN _errorTOKEN(const char* _MESSAGE);

 _TOKEN      _string();
 _TOKEN      _number();
 _TOKEN      _identifier();
 _TOKEN_TYPE _identifierTYPE();
 _TOKEN_TYPE _checkKEYWORD(int _START, int _LENGTH, const char* _REST, _TOKEN_TYPE _TYPE);
};

using SCANNER = _Scanner;
using TOKEN   = _Scanner::_TOKEN;
#endif

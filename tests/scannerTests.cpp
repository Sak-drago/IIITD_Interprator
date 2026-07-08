#include "../src/virtual_machine/scanner.hpp"
#include <utils/testManager.hpp>

static u8 punctuationAndOperatorsTest()
{
  SCANNER _scanner;
  _scanner._initSCANNER("(){},+-*/ ! != = == < <= > >=");

  _Scanner::_TOKEN_TYPE _expected[] = {
    SCANNER::OPEN_PARANTHESIS, SCANNER::CLOSE_PARANTHESIS,
    SCANNER::OPEN_BRACE,       SCANNER::CLOSE_BRACE,
    SCANNER::COMMA,
    SCANNER::ADD, SCANNER::SUB, SCANNER::MUL, SCANNER::DIV,
    SCANNER::NOT, SCANNER::NOT_EQUALS,
    SCANNER::ASSIGN, SCANNER::EQUALS,
    SCANNER::LESSER, SCANNER::LESSER_EQUAL,
    SCANNER::GREATER, SCANNER::GREATER_EQUAL,
  };

  for(auto _type : _expected)
  {
    TOKEN _token = _scanner._scanTOKEN();
    EXPECT_TO_BE(_type, _token.mTYPE);
  }

  EXPECT_TO_BE(SCANNER::TOKEN_EOF, _scanner._scanTOKEN().mTYPE);
  return 0;
}

static u8 keywordsTest()
{
  SCANNER _scanner;
  _scanner._initSCANNER("agar ya for while return Plag dac real cap Fn notAKeyword");

  _Scanner::_TOKEN_TYPE _expected[] = {
    SCANNER::IF, SCANNER::ELSE, SCANNER::FOR, SCANNER::WHILE, SCANNER::RETURN,
    SCANNER::PLAG, SCANNER::DAC, SCANNER::TRUE, SCANNER::FALSE, SCANNER::FUNCTION,
    SCANNER::IDENTIFIER,
  };

  for(auto _type : _expected)
  {
    TOKEN _token = _scanner._scanTOKEN();
    EXPECT_TO_BE(_type, _token.mTYPE);
  }

  return 0;
}

static u8 numbersAndStringsTest()
{
  SCANNER _scanner;
  _scanner._initSCANNER("42 3.14 \"hello iiitd\"");

  TOKEN _number = _scanner._scanTOKEN();
  EXPECT_TO_BE(SCANNER::NUMBER, _number.mTYPE);
  EXPECT_TO_BE(2, _number.mLENGTH);

  TOKEN _float = _scanner._scanTOKEN();
  EXPECT_TO_BE(SCANNER::NUMBER, _float.mTYPE);
  EXPECT_TO_BE(4, _float.mLENGTH);

  TOKEN _string = _scanner._scanTOKEN();
  EXPECT_TO_BE(SCANNER::STRING, _string.mTYPE);
  EXPECT_TO_BE(13, _string.mLENGTH); // - - - includes the surrounding quotes

  return 0;
}

static u8 commentsAndLineTrackingTest()
{
  SCANNER _scanner;
  _scanner._initSCANNER("agar // this whole line is noise\nya");

  TOKEN _if = _scanner._scanTOKEN();
  EXPECT_TO_BE(SCANNER::IF, _if.mTYPE);
  EXPECT_TO_BE(1, _if.mLINE);

  TOKEN _else = _scanner._scanTOKEN();
  EXPECT_TO_BE(SCANNER::ELSE, _else.mTYPE);
  EXPECT_TO_BE(2, _else.mLINE);

  return 0;
}

static u8 unterminatedStringTest()
{
  SCANNER _scanner;
  _scanner._initSCANNER("\"never closed");
  TOKEN _token = _scanner._scanTOKEN();
  EXPECT_TO_BE(SCANNER::TOKEN_ERROR, _token.mTYPE);
  return 0;
}

int main()
{
  registerTest(punctuationAndOperatorsTest,  "Single/double char tokens scan correctly");
  registerTest(keywordsTest,                 "Language keywords resolve to their token types");
  registerTest(numbersAndStringsTest,        "Numbers (int + float) and strings scan correctly");
  registerTest(commentsAndLineTrackingTest,  "Line comments are skipped, line numbers tracked across newlines");
  registerTest(unterminatedStringTest,       "Unterminated string produces a TOKEN_ERROR");
  runTests();
}

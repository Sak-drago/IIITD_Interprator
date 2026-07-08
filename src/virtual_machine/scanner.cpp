#include "scanner.hpp"
#include <cstring>

void _Scanner::_initSCANNER(const char* _SOURCE)
{
  this->mSTART   = _SOURCE;
  this->mCURRENT = _SOURCE;
  this->mLINE    = 1;
}

bool _Scanner::_isAtEND()
{
  return *this->mCURRENT == '\0';
}

char _Scanner::_advance()
{
  this->mCURRENT++;
  return this->mCURRENT[-1];
}

char _Scanner::_peek()
{
  return *this->mCURRENT;
}

char _Scanner::_peekNEXT()
{
  if(this->_isAtEND()) return '\0';
  return this->mCURRENT[1];
}

bool _Scanner::_match(char _EXPECTED)
{
  if(this->_isAtEND())            return false;
  if(*this->mCURRENT != _EXPECTED) return false;
  this->mCURRENT++;
  return true;
}

_Scanner::_TOKEN _Scanner::_makeTOKEN(_TOKEN_TYPE _TYPE)
{
  _TOKEN _token;
  _token.mTYPE   = _TYPE;
  _token.mSTART  = this->mSTART;
  _token.mLENGTH = static_cast<int>(this->mCURRENT - this->mSTART);
  _token.mLINE   = this->mLINE;
  return _token;
}

_Scanner::_TOKEN _Scanner::_errorTOKEN(const char* _MESSAGE)
{
  _TOKEN _token;
  _token.mTYPE   = _Scanner::TOKEN_ERROR;
  _token.mSTART  = _MESSAGE;
  _token.mLENGTH = static_cast<int>(std::strlen(_MESSAGE));
  _token.mLINE   = this->mLINE;
  return _token;
}

void _Scanner::_skipWHITESPACE()
{
  for(;;)
  {
    char _c = this->_peek();
    switch(_c)
    {
      case ' ':
      case '\r':
      case '\t':
        this->_advance();
        break;
      case '\n':
        this->mLINE++;
        this->_advance();
        break;
      case '/':
        // - - - Comments run till end of line, there is no block comment syntax (yet).
        if(this->_peekNEXT() == '/')
        {
          while(this->_peek() != '\n' && !this->_isAtEND()) this->_advance();
        }
        else
        {
          return;
        }
        break;
      default:
        return;
    }
  }
}

_Scanner::_TOKEN _Scanner::_string()
{
  while(this->_peek() != '"' && !this->_isAtEND())
  {
    if(this->_peek() == '\n') this->mLINE++;
    this->_advance();
  }

  if(this->_isAtEND()) return this->_errorTOKEN("Unterminated string.");

  // - - - the closing quote
  this->_advance();
  return this->_makeTOKEN(_Scanner::STRING);
}

_Scanner::_TOKEN _Scanner::_number()
{
  while(std::isdigit(this->_peek())) this->_advance();

  // - - - look for a fractional part
  if(this->_peek() == '.' && std::isdigit(this->_peekNEXT()))
  {
    this->_advance();
    while(std::isdigit(this->_peek())) this->_advance();
  }

  return this->_makeTOKEN(_Scanner::NUMBER);
}

_Scanner::_TOKEN_TYPE _Scanner::_checkKEYWORD(int _START, int _LENGTH, const char* _REST, _TOKEN_TYPE _TYPE)
{
  if(this->mCURRENT - this->mSTART == _START + _LENGTH &&
     std::memcmp(this->mSTART + _START, _REST, _LENGTH) == 0)
  {
    return _TYPE;
  }
  return _Scanner::IDENTIFIER;
}

_Scanner::_TOKEN_TYPE _Scanner::_identifierTYPE()
{
  // - - - Little trie matching the language's keywords. See eval branch tokenizer.cpp for the source of truth.
  switch(this->mSTART[0])
  {
    case 'a': return this->_checkKEYWORD(1, 3, "gar",  _Scanner::IF);       // agar
    case 'y': return this->_checkKEYWORD(1, 1, "a",    _Scanner::ELSE);     // ya
    case 'f': return this->_checkKEYWORD(1, 2, "or",   _Scanner::FOR);      // for
    case 'w': return this->_checkKEYWORD(1, 4, "hile", _Scanner::WHILE);    // while
    case 'd': return this->_checkKEYWORD(1, 2, "ac",   _Scanner::DAC);      // dac
    case 'c': return this->_checkKEYWORD(1, 2, "ap",   _Scanner::FALSE);    // cap
    case 'P': return this->_checkKEYWORD(1, 3, "lag",  _Scanner::PLAG);     // Plag
    case 'F': return this->_checkKEYWORD(1, 1, "n",    _Scanner::FUNCTION); // Fn
    case 'r':
      if(this->mCURRENT - this->mSTART > 1)
      {
        switch(this->mSTART[1])
        {
          case 'e':
            if(this->mCURRENT - this->mSTART > 2)
            {
              switch(this->mSTART[2])
              {
                case 't': return this->_checkKEYWORD(3, 3, "urn", _Scanner::RETURN); // return
                case 'a': return this->_checkKEYWORD(3, 1, "l",   _Scanner::TRUE);   // real
              }
            }
            break;
        }
      }
      break;
  }
  return _Scanner::IDENTIFIER;
}

_Scanner::_TOKEN _Scanner::_identifier()
{
  while(std::isalpha(this->_peek()) || std::isdigit(this->_peek()) || this->_peek() == '_')
  {
    this->_advance();
  }
  return this->_makeTOKEN(this->_identifierTYPE());
}

_Scanner::_TOKEN _Scanner::_scanTOKEN()
{
  this->_skipWHITESPACE();
  this->mSTART = this->mCURRENT;

  if(this->_isAtEND()) return this->_makeTOKEN(_Scanner::TOKEN_EOF);

  char _c = this->_advance();

  if(std::isalpha(_c) || _c == '_') return this->_identifier();
  if(std::isdigit(_c))              return this->_number();

  switch(_c)
  {
    case '(': return this->_makeTOKEN(_Scanner::OPEN_PARANTHESIS);
    case ')': return this->_makeTOKEN(_Scanner::CLOSE_PARANTHESIS);
    case '{': return this->_makeTOKEN(_Scanner::OPEN_BRACE);
    case '}': return this->_makeTOKEN(_Scanner::CLOSE_BRACE);
    case ',': return this->_makeTOKEN(_Scanner::COMMA);
    case '+': return this->_makeTOKEN(_Scanner::ADD);
    case '-': return this->_makeTOKEN(_Scanner::SUB);
    case '*': return this->_makeTOKEN(_Scanner::MUL);
    case '/': return this->_makeTOKEN(_Scanner::DIV);
    case '"': return this->_string();
    case '!': return this->_makeTOKEN(this->_match('=') ? _Scanner::NOT_EQUALS    : _Scanner::NOT);
    case '=': return this->_makeTOKEN(this->_match('=') ? _Scanner::EQUALS        : _Scanner::ASSIGN);
    case '<': return this->_makeTOKEN(this->_match('=') ? _Scanner::LESSER_EQUAL  : _Scanner::LESSER);
    case '>': return this->_makeTOKEN(this->_match('=') ? _Scanner::GREATER_EQUAL : _Scanner::GREATER);
  }

  return this->_errorTOKEN("Unexpected character.");
}

#include "../include/evaluator.h"
#include <cctype>
#include <cstdint>
#include <random>
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>

static i32          anonymousVarCount = 0;
static std::string  stdLib[]          = {"bol", "bata"};
typedef enum STD_LIB_FUNCTIONS
{
  BOL,
  BATA,
  RANDOM,
  STD_LIB_FUNCTION_COUNT
} STD_LIB_FUNCTIONS;

typedef enum ParsedType 
{ 
  TYPE_INT, 
  TYPE_FLOAT, 
  TYPE_BOOL, 
  TYPE_INVALID 
} ParsedType;

template<typename T>
Data* assignValue(Data* DATA, const T& VALUE)
{
  *reinterpret_cast<T*>(DATA->memory) = VALUE;
  return DATA;
}

ParsedType getType(const std::string& INPUT, i64& OUTPUT_INT, f64& OUTPUT_FLOAT, bool& OUTPUT_BOOL)
{
  if (INPUT == "real")  { OUTPUT_BOOL = true;  return TYPE_BOOL; }
  if (INPUT == "cap")   { OUTPUT_BOOL = false; return TYPE_BOOL; }

  try 
  {
    size_t pos;
    OUTPUT_INT = std::stoll(INPUT, &pos);
    if (pos == INPUT.length()) return TYPE_INT;
  } catch (...) {}

  try 
  {
    size_t pos;
    OUTPUT_FLOAT = std::stod(INPUT, &pos);
    if (pos == INPUT.length()) return TYPE_FLOAT;
  } catch (...) {}

  return TYPE_INVALID;
}

static inline Data evaluateStdLib(const Node* FUNCTION_NODE, STD_LIB_FUNCTIONS FUNCTION)
{
  std::vector<FunctionParameter> parameters = FUNCTION_NODE->context.functionContext.parameters;

  switch (FUNCTION)
  {
    case BOL :
    {
      if (parameters.size() != 1)
      {
        raiseException("bol function takes only one argument. Sample use:\n"
                       "Plag bhai = 'Asher'\nbol('Hello {bhai}')\nOutput:\nHello Asher");
      }

      std::string toParse = parameters[0].name;
      std::string toPrint;

      if (toParse.size() < 3 || toParse.front() != '\'' || toParse.back() != '\'')
      {
        raiseException("bol function takes a string in single quotes ' as input");
      }

      toParse = toParse.substr(1, toParse.size() - 2); // remove outer quotes
      std::string var;
      bool        inVar = false;

      for (char c : toParse)
      {
        if (c == '{')
        {
          if (inVar || !var.empty())
            raiseException("Variables in a string must be enclosed within {}");
          inVar = true;
          continue;
        }
        else if (c == '}')
        {
          if (!inVar)
            raiseException("Mismatched closing brace '}' in string");

          auto that = runtime.variables.find(var);
          if (that == runtime.variables.end())
          {
            raiseException(("variable : " + var + " does not exist").c_str());
          }

          toPrint += getDataValueStr(&that->second);
          var.clear();
          inVar = false;
          continue;
        }

        if (inVar)
          var += c;
        else
          toPrint += c;
      }

      bol(toPrint.c_str());

      Data ret;
      ret.memory    = NULL;
      ret.type      = Null;
      ret.refCount  = 0;
      return ret;
    }

    case BATA :
    {
      if (!parameters.empty()) raiseException("bata function doesn't take any parameters");

      std::string input;
      std::cin >> input;

      // - - - trim input
      auto start = input.begin(), end = input.end();
      while (start != end && std::isspace(*start)) ++start;
      while (end != start && std::isspace(*(end - 1))) --end;
      input = std::string(start, end);

      std::string name = "anonymous_bata_" + std::to_string(anonymousVarCount++);
      i64 intVal;
      f64 floatVal;
      bool boolVal;

      switch (getType(input, intVal, floatVal, boolVal))
      {
        case TYPE_BOOL:
          return *assignValue(createVariable(name, Bool), boolVal);
        case TYPE_INT:
          return *assignValue(createVariable(name, Integer), intVal);
        case TYPE_FLOAT:
          return *assignValue(createVariable(name, Float), floatVal);
        default:
          raiseException("Could not parse input to Integer, Float, or Bool");
      }
    }

    case RANDOM :
    {
      if (parameters.empty() || parameters.size() > 3)
        raiseException("random function takes 1 to 3 parameters (type, lowerBound, upperBound)");

      std::string type = parameters[0].name;
      std::string name = "anonymous_random_" + std::to_string(anonymousVarCount++);
      std::mt19937 rng(std::random_device{}());

      using RandomHandler = std::function<Data*(const std::vector<FunctionParameter>&, std::mt19937&, std::string&)>;

      static const std::unordered_map<std::string, RandomHandler> randomHandlers = 
      {
        { "Bool", [](const auto& PARAMS, auto& RNG, std::string& NAME) -> Data*
          {
            if (PARAMS.size() != 1)   raiseException("random 'Bool' does not take bounds");
            std::uniform_int_distribution<int> dist(0, 1);
            return assignValue(createVariable(NAME, Bool), dist(RNG) == 1);
          }
        },

        { "Integer", [](const auto& PARAMS, auto& RNG, std::string& NAME) -> Data*
          {
            i64 min = INT64_MIN, max = INT64_MAX;
            if (PARAMS.size() >= 2)  min = std::stoll(PARAMS[1].name);
            if (PARAMS.size() == 3)  max = std::stoll(PARAMS[2].name);
            if (min >= max)          raiseException("random Integer: min must be less than max");
            std::uniform_int_distribution<i64> dist(min, max);
            return assignValue(createVariable(NAME, Integer), dist(RNG));
          }
        },

        { "Float", [](const auto& PARAMS, auto& RNG, std::string& NAME) -> Data*
          {
            f64 min = -1e10, max = 1e10;
            if (PARAMS.size() >= 2)
              min = std::stod(PARAMS[1].name);
            if (PARAMS.size() == 3)
              max = std::stod(PARAMS[2].name);
            if (min >= max)
              raiseException("random Float: min must be less than max");
            std::uniform_real_distribution<f64> dist(min, max);
            return assignValue(createVariable(NAME, Float), dist(RNG));
          }
        }
      };

      auto handler = randomHandlers.find(type);
      if (handler == randomHandlers.end()) raiseException("random supports only 'Integer', 'Float', or 'Bool'");

      return *(handler->second(parameters, rng, name));
    }

    default : break;
  }

  raiseException("Unknown STD_LIB_FUNCTION passed to evaluateStdLib");
  return Data{};
}

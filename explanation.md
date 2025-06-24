# HOW THE EVALUATOR IS SUPPOSED TO WORK-


## What it does (program.h)
* It tries to generate an exit message by iterating over the AST generated
  by the parser.
* An exit message tells you how the program ran, was there a syntax error,
  out of memory, did we fail in setting up, or did everything go as planned

### What is a program?
* A Program is what the evaluator is trying to evaluate and build
* **File** output               : the stdout of the program, the output, the result of the code, all the print statements
* **LinearAllocator** allocator : _for the parser to generate nodes_
* **vector** statements         : _parser generated statements_
* **vector** functions          : _parser generated functions_
* **unordered_map** variables   : all the variables in a program, a map from the name of the variable to **Data**
* **LinearAllocator** stack     : for sotring pointers to variables in current use (i.e. non-zero ref count)

### What is a Data?
* Analogy : 
  | Program | Unit |
  | -------------- | --------------- |
  | Tokenizer | Token |
  | Parser | Node |
  | Evaluator | Data | 
* Data to Evaluator is Node to Parser
* **DataType** type : what kind of variable is it, is it a number, a float, a bool, a list, a string etc
* **void*** memory  : where is the value of the variable, eg :  number 3, memory is pointer to the bytes where the number '3' lives
* **i32** refCount  : for garbage collection, how many times, have we used this variable in the current scope

---

## How does evaluation happen (evaluator.h and evaluator.cpp)
Basically here is what happens-
* When the run function is called-
  1. The runtime is created-
    * the stdout file is created
    * garbage collector is started with just 1 page of memory (WARN: arbitrary, its about just 4 kb, might want to do it in proportion to the number of assignmnet statements)
    * a linear allocator is initialized for storing the variables in current use
    * a map is created to find variables by name in O(1) time
  2. for all the nodes in runtime -> evaluate
  3. save the output to the file and exit

### Evaluate function
```C++
Data evaluate(const Node* NODE, const char* NAME)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot evaluate a NULL node");
  switch (NODE->type)
  {
    case NODE_TYPE_NUMBER                   : return evaluateNumberNode     (NODE, NAME);
    case NODE_TYPE_BINARY_OPERATOR          : return evaluateBinaryNode     (NODE, NAME);
    case NODE_TYPE_BOOLEAN                  : return evaluateBooleanNode    (NODE, NAME);
//    case NODE_TYPE_ASSIGNMENT               : return evaluateAssignmentNode (NODE, NAME);
    case NODE_TYPE_VARIABLE                 : return evaluateVariableNode   (NODE, NAME);
    case NODE_TYPE_IF                       : return evaluateIfNode         (NODE, NAME);
    default : raiseException(std::string("Type not handled yet : " + std::string(getNodeString((Node*) NODE).c_str())).c_str());
    exit(1);
  }
}
```
* Just a big switch case for evaluating each type of Node
* If an evaluation ever failes, the function **raiseException** is called,
  this function is not fleshed out yet, it is supposed to stop the evaluator at the point, write the exception to the file and exit
* It returns a Data
* It needs a string in case, the variable created is not temporary, i.e. can be referenced by the user again
  eg : 1 + 2, here the variables 1 and 2 are temporary and cannot be accessed again in the program
       x = 1, here the variable x is not temporary and can be used again and we ought to save it with the name _x_


## Evaluation of Nodes-
### Variable Node Evaluation-
* find the variable in the map
* if found, return teh data, remember the map is **name->variable** i.e. **string->Data**
* if not found, raiseException

### If Node Evaluation-
* acquire if node condition
* evaluate the condition itself (recursive call on **evaluate**)
* if the decision is not a bool Data, then raise an exception, (this can happen if the user enters 1+2 in the if condition, the parser will put the conditional as a binaryNode not a boolean one)
* if the decision is **real**, in a for loop evaluate the **consequence** statments
* if the decision is **cap**,  in a for loop evaluate the **alternative** statments
* Since the If statement itself ought not to be used as a return type in a statement, it itself returns a NULL Data.
* A Null data does not have any memory, any refcount, any place in the stack or variables map
  i.e this is illegal
  ```IIIT-D Lang
  1 = agar(real) {...}
  ```

### Number Node / Boolean Node Evaluation-
* take the string representation of the number and determine, what kind a number is it
  * if it is not a number at all, raiseException (WARN : Code left as a TODO)
  * Create an anonymous temporary variable, assign the numeric value or float value in that memory
  * Similar for Boolean Nodes except that we dont need to check a string, the parser gives us a bool value to use


### Binary Node Evaluation-
* The O(n^3) coding for O(1) evaluation
* get the left and the right nodes and evaluate them (recursive call on **evalute**)
* check if the resulting type of the left and right Data are compatible, eg: "Hello World" + [1, 2, 3] are incompatible but 5 % 4 is compatible
* if they are not compatible, raiseException
* rest is just to perform the operation requested and result the final value as Data


**The rest are not implemented yet, but here is the basic plan-**
### Function Nodes
* Differentiate between function defs and function calls
* when creating the runtime, make a map of function names to function nodes,
* Add to this map, language given functions like bolo and puucho (printf and scanf of our lang)
* When a function call happens, check if the function exists in the map, 
* if it doesnt, raiseException
* if it does, evaluate the corresponding function defintion node

### Prefix Nodes
* similar to binary nodes, but unary

### Assignment Nodes
* Probably the most imporant node
* first check if the variable exists
  * if the variable exists, we are reassigning a variable and dont need to request more memory from garbage collector
  * if the variable doesnt exist, create one
* check if we are to do a Plag assignment
  * If yes
    * a Plag Data type is just a Data*, 
    * follow the data pointer until you reach a data which itself is not Plag, this is the original
    * increase refcount of the original
  * If no, 
    * copy the value from the memory of that Data into the new Data 
* increase the refCount of the created variable 

eg : 
  x = 1, // - - - new assignmnet
  Data { type = NUMBER, value = some int*, refCount = 1}
  Plag y = x -> Data {type = Plag, value = Data* of x, refCount = 1 (x goes to 2)}


## After done, we make garbage collection multithreaded and tokenizer, parser end evaluator mulithreaded


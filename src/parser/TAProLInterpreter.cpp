#include "TAProLInterpreter.hpp"

#include "TAProLLexer.h"
#include "TAProLListenerCPPImpl.hpp"

using namespace antlr4;
using namespace taprol;

namespace exatn {

namespace parser {

void TAProLInterpreter::interpret(const std::string &src) {
  std::ostringstream cpp_source;
  interpret(src, cpp_source);
  std::cout
      << "#DEBUG(exatn::parser::cpp): C++ source generated from TAProL source:"
      << std::endl;
  std::cout << cpp_source.str();
}

void TAProLInterpreter::interpret(const std::string &src,
                                  std::ostream &output) {

  // Setup the Antlr Parser
  ANTLRInputStream input(src);
  TAProLLexer lexer(&input);
  lexer.removeErrorListeners();
  lexer.addErrorListener(new TAProLErrorListener());

  CommonTokenStream tokens(&lexer);
  TAProLParser parser(&tokens);
  parser.removeErrorListeners();
  parser.addErrorListener(new TAProLErrorListener());

  // Walk the Parse Tree
  tree::ParseTree *tree = parser.taprolsrc();

  TAProLListenerCPPImpl listener(output);
  tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
}

} // namespace parser

} // namespace exatn

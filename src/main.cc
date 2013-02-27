#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "optionparser.h"

struct Arg: public option::Arg
{
  static void printError(const char* msg1, const option::Option& opt, const char* msg2)
  {
    fprintf(stderr, "%s", msg1);
    fwrite(opt.name, opt.namelen, 1, stderr);
    fprintf(stderr, "%s", msg2);
  }

  static option::ArgStatus Required(const option::Option& option, bool msg)
  {
    if (option.arg != 0)
      return option::ARG_OK;

    if (msg) printError("Option '", option, "' requires an argument\n");
    return option::ARG_ILLEGAL;
  }

  static option::ArgStatus NonEmpty(const option::Option& option, bool msg)
  {
    if (option.arg != 0 && option.arg[0] != 0)
      return option::ARG_OK;

    if (msg) printError("Option '", option, "' requires a non-empty argument\n");
    return option::ARG_ILLEGAL;
  }
};



enum optionIndex { UNKNOWN, HELP, INPUT, OUTPUT };
const option::Descriptor usage[] =
{
 {UNKNOWN, 0, "", "",option::Arg::None, "USAGE: parser [options]\n\n"
                                        "Options:" },
 {HELP, 0, "", "help", option::Arg::None, "  --help  \tPrint usage and exit." },
 {INPUT, 0, "i", "input", Arg::Required, "  --input, -i  \tSpecify input file." },
 {OUTPUT, 0, "o", "output", Arg::Required, "  --output, -o  \tSpecify output file." },
 {0,0,0,0,0,0}
};



int main(int argc, char* argv[])
{
  argc -=(argc > 0); argv += (argc > 0); // skip program name argv[0] if present
  option::Stats  stats(usage, argc, argv);
  option::Option* options = new option::Option[stats.options_max];
  option::Option* buffer  = new option::Option[stats.buffer_max];
  option::Parser parse(usage, argc, argv, options, buffer);

  if (parse.error())
    return 1;

  if (options[HELP] || argc == 0) {
    option::printUsage(std::cout, usage);
    return 0;
  }

  string input;
  string output;

  if(!options[INPUT])
  {
    option::printUsage(cout, usage);
    return 0;
  }

  Parser p(options[INPUT].arg);

  Ast::List<Ast::Statement *> *ast = p.parse();

  if(options[OUTPUT])
  {
    ast->printVar(options[OUTPUT].arg, "ast");
  }
  else
  {
    ast->printVar(cout, "ast");
  }

  delete ast;

  return 0;
}

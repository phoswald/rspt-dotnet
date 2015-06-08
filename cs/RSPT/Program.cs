using System;
using System.Collections.Generic;
using System.IO;

namespace RSPT
{
    public class Program
    {
        static void Main(string[] args)
        {
            if(args.Length == 0) {
                Console.WriteLine("RSPT - Really Simple Parser Tool - (C) 2015 Philip Oswald");
                Console.WriteLine("Usage: $ RSPT.exe -gen=cs   <grammar.txt> <output.cs>");
                Console.WriteLine("                  -gen=cpp  <grammar.txt> <output.h>");
                Console.WriteLine("                  -gen=java <grammar.txt> <output.java>");
                Console.WriteLine("                  -par=txt  <grammar.txt> <input.txt> <output.txt>");
                Console.WriteLine("Where:");
                Console.WriteLine("    -gen=cs   generates a parser for the given grammar in C#");
                Console.WriteLine("    -gen=cpp  generates a parser for the given grammar in C++");
                Console.WriteLine("    -gen=java generates a parser for the given grammar in Java");
                Console.WriteLine("    -par=txt  parses the input using the given grammar");
                Console.WriteLine("Notes:");
                Console.WriteLine("  All parsers are top down (recursive descent) parsers that");
                Console.WriteLine("  can parse non-left recursive LL(x) grammars. Grammars contain rules,");
                Console.WriteLine("  actions to transform machting input into output and instructions to");
                Console.WriteLine("  customize the code generator or interpreter.");
                return;
            }

            int i = 0;
            while(i < args.Length) {
                if(args[i].StartsWith("-gen=")) {
                    if(i+2 >= args.Length) {
                        Console.WriteLine("ERROR: Not enough arguments for -gen=...");
                        break;
                    }
                    Generate(args[i+1], args[i+2], args[i].Substring(5));
                    i += 3;

                } else if(args[i] == "-par=txt") {
                    if(i+3 >= args.Length) {
                        Console.WriteLine("ERROR: Not enough arguments for -par=...");
                        break;
                    }
                    Parse(args[i+1], args[i+2], args[i+3]);
                    i += 4;

                } else {
                    Console.WriteLine("ERROR: Invalid argument '{0}'", args[i]);
                    break;
                }
            }
        }

        private static void Generate(string grammarFile, string parserFile, string type)
        {
            Console.WriteLine("Generating parser '{0}' from grammar '{1}'.", parserFile, grammarFile);

            try {
                using(TextReader grammarStream  = new StreamReader(new FileStream(grammarFile, FileMode.Open, FileAccess.Read))) {
                    Grammar grammar = new Grammar(grammarStream);
                    using(TextWriter parserStream = new StreamWriter(new FileStream(parserFile, FileMode.Create))) {
                        Generator generator; 
                        if(type == "cs") {
                            generator = new GeneratorRecursiveCS(grammar);
                        } else if(type == "cpp") {
                            generator = new GeneratorRecursiveCPP(grammar);
                        } else if(type == "java") {
                            generator = new GeneratorRecursiveJava(grammar);
                        } else {
                            throw new Exception(string.Format("Invalid generator type '{0}'.", type));
                        }
                        generator.Generate(parserStream);
                    }
                }
            } catch(Exception ex) {
                Console.WriteLine("ERROR: Failed to generate parser: {0}", ex.Message);
                return;
            }
        }

        private static void Parse(string grammarFile, string inputFile, string outputFile)
        {
            Console.WriteLine("Parsing input '{0}' using grammar '{1}' into '{2}'.", inputFile, grammarFile, outputFile);

            try {
                using(TextReader grammarStream  = new StreamReader(new FileStream(grammarFile, FileMode.Open, FileAccess.Read))) {
                    Grammar grammar = new Grammar(grammarStream);
                    using(TextReader inputStream  = new StreamReader(new FileStream(inputFile, FileMode.Open, FileAccess.Read))) {
                        using(TextWriter outputStream = new StreamWriter(new FileStream(outputFile, FileMode.Create))) {
                            Interpreter parser = new Interpreter(grammar); 
                            string input  = inputStream.ReadToEnd();
                            string output = parser.Parse(input);
                            if(output != null) {
                                outputStream.Write(output.ToCharArray());
                            }
                        }
                    }
                }
            } catch(Exception ex) {
                Console.WriteLine("ERROR: Failed to parse: {0}", ex.Message);
                return;
            }
        }
    }
}

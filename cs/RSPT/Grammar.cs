using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace RSPT
{
    public class Grammar
    {
        public readonly Dictionary<string, SymbolNonTerm> Index    = new Dictionary<string,SymbolNonTerm>();
        public readonly List<SymbolNonTerm>               NonTerms = new List<SymbolNonTerm>();
        public readonly List<SymbolNonTerm>               Exports  = new List<SymbolNonTerm>();
        public readonly List<string> Includes = new List<string>(); // a list of C++ include or C# using directives
        public string                Namespace;                     // the C++ or C# namespace
        public string                Class;                         // the C++ or C# class 
        public string                Type;                          // C++ or C# type for input symbols
        public readonly List<string> Codes = new List<string>();    // a list of C++ or C# source code fragments 

        public Grammar(TextReader reader) {
            List<string> tokens = Tokenize(reader);
            Parse(tokens);
        }

        private List<string> Tokenize(TextReader reader) 
        {
            List<string>  tokens = new List<string>();
            StringBuilder token  = new StringBuilder();
            Stack<char>   brace  = new Stack<char>();
            while(true) {
                int i = reader.Read();
                if(i == -1) {
                    break;
                }
                char c = (char) i;
                if(brace.Count > 0 && c == brace.Peek()) { // closing brace
                    brace.Pop();
                    token.Append(c);
                } else if((c == ' ' || c == '\t' || c == '\r' || c == '\n') && brace.Count == 0) { // whitespace (not inside brace)
                    if(token.Length > 0) {
                        tokens.Add(token.ToString());
                        token = new StringBuilder();
                    }
                } else if(c == '#' && brace.Count == 0) { // comment (not inside brace)
                    if(token.Length > 0) {
                        tokens.Add(token.ToString());
                        token = new StringBuilder();
                    }
                    do {
                        i = reader.Read();
                    } while(i != -1 && i != '\r' && i != '\n');
                } else if((c == '\"' || c == '\'')) { // quoted string
                    token.Append(c);
                    char quote = c;
                    while(true) {
                        i = reader.Read();
                        if(i == -1 || i == '\r' || i == '\n') {
                            throw new Exception(string.Format("Unexpected EOL, expected '{0}'.", quote));
                        }
                        c = (char) i;
                        if(c == quote) {
                            token.Append(c);
                            break;
                        } else if(c == '\\') {
                            i = reader.Read();
                            switch(i) {
                                case '\\': c = '\\'; break;
                                case '\'': c = '\''; break;
                                case '\"': c = '\"'; break;
                                case 't':  c = '\t'; break;
                                case 'r':  c = '\r'; break;
                                case 'n':  c = '\n'; break;
                                default: throw new Exception("Invalid escape sequence, expected one of: \\ \' \" t r n .");
                            }
                            token.Append(c);
                        } else {
                            token.Append(c);
                        }
                    }
                } else if(c == '(') { // opening brace 
                    brace.Push(')');
                    token.Append(c);
                } else if(c == '{') { // opening brace
                    brace.Push('}');
                    token.Append(c);
                } else if(c == '[') { // opening brace
                    brace.Push(']');
                    token.Append(c);
                } else if(c == '<' && brace.Count == 0) { // opening brace (only at top level)
                    brace.Push('>');
                    token.Append(c);
                } else { // normal character
                    token.Append(c);
                }
            }
            if(token.Length > 0) {
                tokens.Add(token.ToString());
            }
            if(brace.Count > 0) {
                throw new Exception(string.Format("Unexpected EOF, expected '{0}.", brace.Peek()));
            }
            return tokens;
        }

        private void Parse(List<string> tokens)
        {
            int  pos = 0;
            bool exp = false;
            while(pos < tokens.Count) {
                string symbol = tokens[pos];
                if(symbol == "<export>") {
                    exp = true;
                } else if(symbol.StartsWith("<include:") && symbol[symbol.Length-1] == '>') {
                    Includes.Add(symbol.Substring(9, symbol.Length-10));
                } else if(symbol.StartsWith("<namespace:") && symbol[symbol.Length-1] == '>') {
                    Namespace = symbol.Substring(11, symbol.Length-12);
                } else if(symbol.StartsWith("<class:") && symbol[symbol.Length-1] == '>') {
                    Class = symbol.Substring(7, symbol.Length-8);
                } else if(symbol.Length > 2 && symbol[0] == '{' && symbol[symbol.Length-1] == '}') {
                    Codes.Add(symbol.Substring(1, symbol.Length-2));
                } else {
                    SymbolNonTerm sym = GetNonTerm(symbol);
                    NonTerms.Add(sym);
                    if(exp) {
                        Exports.Add(sym);
                        exp = false;
                    }
                    pos++;
                    if(tokens[pos] == ":") {
                       sym.Type = tokens[pos+1];
                       pos+=2;
                    }
                    if(tokens[pos] != "=") {
                        throw new Exception(string.Format("{0}: Unexptected token '{1}'. Exptected: '='.", sym.Name, tokens[pos]));
                    }
                    do {
                        symbol = tokens[++pos];
                        List<Symbol> rule = new List<Symbol>();
                        sym.Rules.Add(rule);
                        while(symbol != "|" && symbol != ";") {
                            if(symbol.Length >= 2 && symbol[0] == '\'' && symbol[symbol.Length-1] == '\'') {
                                rule.Add(new SymbolTerm(symbol));
                            } else if(symbol.Length > 2 && symbol[0] == '{' && symbol[symbol.Length-1] == '}') {
                                rule.Add(new SymbolCode(symbol));
                            } else if(symbol.Length > 2 && symbol[0] == '<' && symbol[symbol.Length-1] == '>') {
                                rule.Add(new SymbolInstr(symbol));
                            } else {
                                rule.Add(GetNonTerm(symbol));
                            }
                            symbol = tokens[++pos];
                        }
                    } while(symbol != ";");
                }
                pos++;
            }
            foreach(SymbolNonTerm sym in NonTerms) {
                if(sym.Rules.Count == 0) {
                    throw new Exception(string.Format("{0}: Symbol is used but not defined.", sym.Name));
                }
            }
        }

        private SymbolNonTerm GetNonTerm(string text)
        {
            if(!IsIdent(text)) {
                throw new Exception(string.Format("Invalid non terminal symbol name '{0}'.", text));
            }
            SymbolNonTerm symbol;
            if(!Index.TryGetValue(text, out symbol)) {
                symbol = new SymbolNonTerm(text);
                Index.Add(text, symbol);
            }
            return symbol;
        }

        private bool IsIdent(string text) 
        {
            bool first = true;
            foreach(char c in text) {
                if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') || (!first && c >= '0' && c <= '9'))) {
                    return false;
                }
                first = false;
            }
            return true;
        }
    }
}
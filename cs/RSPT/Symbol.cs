using System;
using System.Collections.Generic;

namespace RSPT
{
    public abstract class Symbol { // base class for NTS and TS and other special symbols

        public readonly string Token;

        protected Symbol(string token) {
            Token = token;
        }
    }

    public class SymbolNonTerm : Symbol { // a NTS is defined by a set of rules

        public readonly List<List<Symbol>> Rules = new List<List<Symbol>>();
        public string                      Type; // C++ or C# type for output 

        public SymbolNonTerm(string token) : base(token) { }

        public string Name { 
            get { return Token; }
        }
    }

    public class SymbolTerm : Symbol { // a TS is a sequence of input symbols 

        public SymbolTerm(string token) : base(token) { }

        public string Text { 
            get { return Token.Substring(1, Token.Length-2); }
        }
    }

    public class SymbolCode : Symbol { // a C++ or C# source code fragment

        public SymbolCode(string token) : base(token) { }
    
        public string Code { 
            get { return Token.Substring(1, Token.Length-2).Trim(' '); }
        }
    }

    public class SymbolInstr : Symbol { // an instruction modifes the following TS or NTS

        public SymbolInstr(string token) : base(token) { }

        public Instruction Instruction {
            get { 
                if(Token.Substring(1, 3)              == "to:")    return Instruction.TO;
                if(Token.Substring(1, Token.Length-2) == "set")    return Instruction.SET;
                if(Token.Substring(1, Token.Length-2) == "range")  return Instruction.RANGE;
                if(Token.Substring(1, Token.Length-2) == "notset") return Instruction.NOTSET;
                throw new Exception(string.Format("Invalid instruction {0}.", Token));
            }
        }

        public string ToResult {
            get { return Token.Substring(4, Token.Length-5); }
        }
    }

    public enum Instruction {
        TO,     // <to:xxx> overrides the result of the following NTS. 
        SET,    // <set> interprets the following TS as a set of characters
        RANGE,  // <range> interprets the following TS as a range of characters
        NOTSET, // <notset> interprets the following TS as a set of excluded characters
    }
}
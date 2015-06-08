using System;
using System.Collections.Generic;

namespace RSPT
{
    public class Interpreter
    {
        private readonly Grammar _grammar;

        public Interpreter(Grammar grammar) { _grammar = grammar; }

        public string Parse(string input)
        {
            if(_grammar.Exports.Count != 1) {
                throw new Exception("The grammar must export exactly one NTS.");
            }

            int pos = 0;
            List<char> output = new List<char>();
            if(ParseNT(_grammar.Exports[0], input.ToCharArray(), ref pos, output) && pos == input.Length) {
                return new string(output.ToArray());
            } else {
                string next = input.Length-pos <= 50 ? input.Substring(pos, input.Length-pos) : input.Substring(pos, 50) + "...";
                throw new Exception("Error at offset " + (pos+1) + ": Cannot handle '" + next + "'.");
            }
        }

        private bool ParseNT(SymbolNonTerm sym, char[] input, ref int pos, List<char> output)
        {
            foreach(List<Symbol> rule in sym.Rules) {
                List<int>        posAry    = new List<int>();
                List<List<char>> outputAry = new List<List<char>>();

                bool ok = true;
                int pos2 = pos;
                posAry.Add(pos2);

                string ins_to  = null;
                bool   ins_set = false;
                bool   ins_rge = false;
                foreach(Symbol sym2 in rule) {
                    if(sym2 is SymbolNonTerm) {
                        SymbolNonTerm sym2nt = sym2 as SymbolNonTerm;
                        outputAry.Add(new List<char>());
                        if(!ParseNT(sym2nt, input, ref pos2, outputAry[outputAry.Count-1] /*TODO*/)) {
                            ok = false;
                            break;
                        }
                        posAry.Add(pos2);
                        ins_to = null;

                    } else if(sym2 is SymbolTerm) {
                        SymbolTerm sym2t = sym2 as SymbolTerm;
                        if(ins_set) { 
                            if(!ParseTSET(input, ref pos2, sym2t.Text)) {
                                ok = false;
                                break;
                            }
                        } else if(ins_rge) {
                            if(!ParseTRGE(input, ref pos2, sym2t.Text[0], sym2t.Text[1])) {
                                ok = false;
                                break;
                            }
                        } else {
                            if(!ParseTS(input, ref pos2, sym2t.Text)) {
                                ok = false;
                                break;
                            }
                        }
                        posAry.Add(pos2);
                        ins_set = false;
                        ins_rge = false;

                    } else if(sym2 is SymbolCode) {
                        InterpretCode((sym2 as SymbolCode).Code, input, posAry, outputAry, output);

                    } else if(sym2 is SymbolInstr) {
                        SymbolInstr sym2i = sym2 as SymbolInstr;
                        switch(sym2i.Instruction) {
                            case Instruction.TO:    ins_to  = sym2i.ToResult; break;
                            case Instruction.SET:   ins_set = true;           break;
                            case Instruction.RANGE: ins_rge = true;           break;
                        }
                    }
                }
                if(ok) {
                    pos = pos2;
                    return true;
                }
            }
            return false;
        }

        private bool ParseTS(char[] input, ref int pos, string s)
        {
            foreach(char c in s) {
                if(pos >= input.Length || input[pos] != c) return false;
                pos++;
            }
            return true;
        }

        private bool ParseTSET(char[] input, ref int pos, string s)
        {
            foreach(char c in s) {
                if(pos < input.Length && input[pos] == c) {
                    pos++;
                    return true;
                }
            }
            return false;
        }

        private bool ParseTRGE(char[] input, ref int pos, char c1, char c2)
        {
            if(pos >= input.Length || input[pos] < c1 || input[pos] > c2) return false;
            pos++;
            return true;
        }

        private void InterpretCode(string code, char[] input, List<int> posAry, List<List<char>> outputAry, List<char> output)
        {
            for(int i = 0; i < code.Length; i++) {
                char c = code[i];
                if(c == '$') {
                    if(i+2 < code.Length && code[i+1] == 'i' && code[i+2] >= '1' && code[i+2] <= '9') {
                        int i1 = posAry[code[i+2]-'1'];
                        int i2 = posAry[code[i+2]-'0'];
                        for(int j = i1; j < i2; j++) {
                            output.Add(input[j]);
                        }                            
                        i+=2;
                    } else if(i+1 < code.Length && code[i+1] >= '1' && code[i+1] <= '9') {
                        output.AddRange(outputAry[code[i+1]-'1']);
                        i++;
                    } else if(i+1 < code.Length && code[i+1] == '$') {
                        output.Add('$');
                        i++;
                    } else {
                        throw new Exception("Invalid code '"+code+"'.");
                    }
                } else {
                    output.Add(c);
                }
            }
        }
    }
}

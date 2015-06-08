using System;
using System.Text;
using System.IO;

namespace RSPT
{
    public abstract class Generator
    {
        protected readonly Grammar _grammar;
        protected string           _indent = "";

        protected Generator(Grammar grammar) {
            _grammar = grammar;
        }

        protected void Indent(int i) { _indent = i>0 ? _indent + new string(' ', i) : _indent.Substring(-i); }

        protected string Quote(string text) 
        {
            StringBuilder sb = new StringBuilder();
            foreach(char c in text) {
                switch(c) {
                    case '\\': sb.Append("\\\\"); break;
                    case '\"': sb.Append("\\\""); break;
                    case '\'': sb.Append("\\\'"); break;
                    case '\r': sb.Append("\\r");  break;
                    case '\n': sb.Append("\\n");  break;
                    case '\t': sb.Append("\\t");  break;
                    default:   sb.Append(c);      break;
                }
            }
            return sb.ToString();
        }

        protected string QuoteComment(string text)
        {
            return Quote(text.Replace("/", " / "));
        }

        public abstract void Generate(TextWriter writer);
    }
}
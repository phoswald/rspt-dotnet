#include "stdafx.h"
#include "CalculatorConsole.h"
#include "Parser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if(argc <= 1) {
		_tprintf(_T("CalculatorConsole -- An expression evaluator to showcase RSPT (the Really Simple Parser Tool)\n"));
		_tprintf(_T("Syntax: $ CalculatorConsole <expression_1> [<expression_2> ... <expression_n>]\n"));
		return 2;
	}

	Parsers::CCalculatorParser p;

	for(int i = 1; i < argc; i++) {
		CString input = argv[i];
		CString result;
		int     error = 0;
		if(!p.Parse_ROOT(input, input.GetLength(), result, error)) {
            CString next = input.GetLength()-error <= 50 ? input.Mid(error, input.GetLength()-error) : input.Mid(error, 50) + "...";
			CString msg; msg.Format(_T("Error at offset %i: Cannot handle '%s'."), error+1, next);
			_tprintf(_T("Error: %s\n"), msg);
		} else {
			_tprintf(_T("Result: %s\n"), result);
		}
	}

	return 0;
}

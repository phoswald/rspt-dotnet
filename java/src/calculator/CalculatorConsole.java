package calculator;

import java.io.IOException;

import rspt.runtime.Ref;
import rspt.runtime.Ref_int;

public class CalculatorConsole {
	
	public static void main(String[] args) {
		try {
			System.out.println("CalculatorConsole -- An expression evaluator to showcase RSPT (the Really Simple Parser Tool)");
			System.out.println("(Enter expressions to calculate or 'exit' to quit)");
			CalculatorParser parser = new CalculatorParser();	
			while(true) {
				String      input  = readLine();
				Ref<String> result = new Ref<String>(null);
				Ref_int     error  = new Ref_int(0);
				if(input.equals("exit")) {
					break;
				}
				if(!parser.Parse_ROOT(input.toCharArray(), result, error)) {
		            String next = input.length()-error.val <= 50 ? input.substring(error.val, input.length()) : input.substring(error.val, error.val + 50) + "...";
					String msg = String.format("Error at offset %d: Cannot handle '%s'.", error.val+1, next);
					System.out.println(msg);
				} else {
					String msg = "Result: " + result.val;
					System.out.println(msg);
				}
			}
		} catch(IOException | RuntimeException e) {
			e.printStackTrace();
		}
	}
	
	private static String readLine() throws IOException {
		StringBuilder sb = new StringBuilder();
		while(true) {
			int b = System.in.read();
			if(b == -1 || b == '\n')
				break;
			if(b == '\r')
				continue;
			sb.append((char) b);
		}
		return sb.toString();
	}
}

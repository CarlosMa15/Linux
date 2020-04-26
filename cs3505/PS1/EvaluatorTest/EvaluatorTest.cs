using System;

namespace FormulaEvaluator
{
    class Program
    {
        public static int lookUpVariable(String v)
        {
            return 7;
        }
        
        static void Main(string[] args)
        {
            try
            {
                Console.WriteLine(Evaluator.Evaluate("2+5*7)", s => 0));
            } catch(Exception e)
            {

            }
            Console.WriteLine("6: "+Evaluator.Evaluate("x1+(x2+(x3+(x4+(x5+x6))))", s => 1));
            Console.WriteLine("12: "+Evaluator.Evaluate("((((x1+x2)+x3)+x4)+x5)+x6", s => 2));
            Console.WriteLine("6: "+Evaluator.Evaluate("y1 *3-8/2+4*(8-9*2)/14*x7", s => (s == "x7") ? 1 : 4));
            Console.WriteLine("0: " + Evaluator.Evaluate("a4-a4*a4/a4", s => 3));
            Console.Read();
        }
    }
}

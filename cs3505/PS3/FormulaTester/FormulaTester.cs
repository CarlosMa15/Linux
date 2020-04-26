using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using SpreadsheetUtilities;
using System.Text.RegularExpressions;
using System.Collections.Generic;

namespace FormulaTester
{
    [TestClass]
    public class FormulaTester
    {
        //All The test from This point on Are the exact test from PS1
        //Adapted to PS3 with scientific notation numbers and decimal
        //number amoung them

        [TestMethod()]
        public void TestRepeatedVar()
        {
            Formula f = new Formula("a4-a4*a4/a4");
            Assert.AreEqual(0, (double)f.Evaluate( s => 3));
        }

        [TestMethod()]
        public void TestComplexNestedParensLeft()
        {
            Formula f = new Formula("((((x1+x2)+x3)+x4)+x5)+x6");
            Assert.AreEqual(12, (double)f.Evaluate( s => 2));
        }

        [TestMethod()]
        public void TestComplexNestedParensRight()
        {
            Formula f = new Formula("x1+(x2+(x3+(x4+(x5+x6))))");
            Assert.AreEqual(6, (double)f.Evaluate( s => 1));
        }

        [TestMethod()]
        public void TestComplexMultiVar()
        {
            Formula f = new Formula("y1*3-8/2+4*(8-9*2)/14*x7");
            Assert.AreEqual(5.14, (double)f.Evaluate( s => (s == "x7") ? 1 : 4),0.005);
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestEmpty()
        {
            Formula f = new Formula("");
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestParensNoOperator()
        {
            Formula f = new Formula("5+7+(5)8");
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestPlusInvalidVariable()
        {
            Formula f = new Formula("5+xx", normalizer, validator);
            
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestInvalidVariable()
        {
            Formula f = new Formula("XX", normalizer, validator);
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestExtraParentheses()
        {
            Formula f = new Formula("2+5*7)");
            
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestExtraOperator()
        {
            Formula f = new Formula("2+5+");
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestSingleOperator()
        {
            Formula f = new Formula("+");
           
        }

        [TestMethod()]
        public void TestComplexAndParentheses()
        {
            Formula f = new Formula("2+3.000*5e0+(3.00+4.00*8e0)*5+2");
            Assert.AreEqual(194, (double)f.Evaluate( s => 0));
        }

        [TestMethod()]
        public void TestComplexTimesParentheses()
        {
            Formula f = new Formula("2.000+3e0*(3.00+5)");
            Assert.AreEqual(26, (double)f.Evaluate( s => 0));
        }

        [TestMethod()]
        public void TestPlusComplex()
        {
            Formula f = new Formula("2+(3+5*9)");
            Assert.AreEqual(50, (double)f.Evaluate( s => 0));
        }

        [TestMethod()]
        public void TestPlusParentheses()
        {
            Formula f = new Formula("2e0+(3.000+5)");
            Assert.AreEqual(10, (double)f.Evaluate( s => 0));
        }

        [TestMethod()]
        public void TestTimesParentheses()
        {
            Formula f = new Formula("2e0*(3.00+5.5)");
            Assert.AreEqual(17, (double)f.Evaluate( s => 0));
        }

        [TestMethod()]
        public void TestParenthesesTimes()
        {
            Formula f = new Formula("(2e0+6.00)*3");
            Assert.AreEqual(24, (double)f.Evaluate( s => 0));
        }

        [TestMethod()]
        public void TestOrderOperationsDecimalSN()
        {
            Formula f = new Formula("2.5+6*3e0");
            Assert.AreEqual(20.5, (double)f.Evaluate( s => 0));
        }


        [TestMethod()]
        public void TestLeftToRightSN()
        {
            Formula f = new Formula("5e0*3.00+.25");
            Assert.AreEqual(15.25, (double)f.Evaluate(s => 0));
        }

        [TestMethod()]
        public void TestLeftToRightDecimal()
        {
            Formula f = new Formula("5.00*3.00+.25");
            Assert.AreEqual(15.25, (double)f.Evaluate(s => 0));
        }

        [TestMethod()]
        public void TestLeftToRight()
        {
            Formula f = new Formula("2*6+3");
            Assert.AreEqual(15, (double)f.Evaluate( s => 0));
        }

        [TestMethod()]
        public void TestUnknownVariable()
        {
            Formula f = new Formula("2+X1");
            Object result = f.Evaluate( s => { throw new ArgumentException("Unknown variable"); });
            Assert.IsTrue(result is FormulaError);
        }

        [TestMethod()]
        public void TestArithmeticWithVariableSN()
        {
            Formula f = new Formula("5e1+X1");
            Assert.AreEqual(54, (double)f.Evaluate(s => 4));
        }

        [TestMethod()]
        public void TestArithmeticWithVariable()
        {
            Formula f = new Formula("2.5+X1");
            Assert.AreEqual(6.5,(double)f.Evaluate( s => 4));
        }

        [TestMethod()]
        public void TestDivisionSN()
        {
            Formula f = new Formula("5e2/2");
            Assert.AreEqual(250, (double)f.Evaluate(s => 0));
        }

        [TestMethod()]
        public void TestDivisionDecimal()
        {
            Formula f = new Formula("9.00/3.00");
            Assert.AreEqual(3,(double)f.Evaluate( s => 0));
        }

        [TestMethod()]
        public void TestDivision()
        {
            Formula f = new Formula("16/2");
            Assert.AreEqual(8, (double)f.Evaluate( s => 0));
        }

        [TestMethod()]
        public void TestMultiplicationSN()
        {
            Formula f = new Formula("5e1*5e1");
            Assert.AreEqual(2500, (double)f.Evaluate(s => 0));
        }

        [TestMethod()]
        public void TestMultiplicationDecimal()
        {
            Formula f = new Formula(".5*.5");
            Assert.AreEqual(.25, (double)f.Evaluate(s => 0));
        }

        [TestMethod()]
        public void TestMultiplication()
        {
            Formula f = new Formula("2*4");
            Assert.AreEqual(8, (double)f.Evaluate( s => 0));
        }

        [TestMethod()]
        public void TestSubtractionScientificNotation()
        {
            Formula f = new Formula("5e2-5e1");
            Assert.AreEqual(450, (double)f.Evaluate(s => 0));
        }

        [TestMethod()]
        public void TestSubtractionDecimal()
        {
            Formula f = new Formula("10.75-5.25");
            Assert.AreEqual(5.5, (double)f.Evaluate(s => 0));
        }

        [TestMethod()]
        public void TestSubtraction()
        {
            Formula f = new Formula("18-10");
            Assert.AreEqual(8, (double)f.Evaluate( s => 0));
        }

        [TestMethod()]
        public void TestAdditionSN()
        {
            Formula f = new Formula("5e2+5e1");
            Assert.AreEqual(550, (double)f.Evaluate(s => 0));
        }

        [TestMethod()]
        public void TestAdditionDecimal()
        {
            Formula f = new Formula("5.5+5.25");
            Assert.AreEqual(10.75, (double)f.Evaluate(s => 0));
        }

        [TestMethod()]
        public void TestAddition()
        {
            Formula f = new Formula("5+3");
            Assert.AreEqual(8, (double)f.Evaluate( s => 0));
        }

        [TestMethod()]
        public void TestSingleNumber()
        {
            Formula f = new Formula("5"); 
            Assert.AreEqual(5, (double)f.Evaluate(s => 0));
        }

        [TestMethod()]
        public void TestSingleVariable()
        {
            Formula f = new Formula("X5");
            Assert.AreEqual(13, (double)f.Evaluate( s => 13));
        }

        //This is the end of all the tests From PS1 adapted to PS3
        //With SN number and decimal numbers in them also adapted
        //the exceptions to PS3
        

        [TestMethod]
        public void TestScientificNotation2()
        {
            Formula f = new Formula("5e2 + 5e1");
            double expected = 550;
            double actual = (double)f.Evaluate(lookup);
            Assert.AreEqual(expected,actual,0.0001);
        }

        [TestMethod]
        public void TestGetReason()
        {
            Formula f = new Formula("1/0");
            FormulaError result = (FormulaError)f.Evaluate(lookup);
            String Reason = result.Reason;
            Assert.AreEqual(Reason, "Divition By Zero Is Not Allowed");
        }

        [TestMethod]
        public void TestHashCodeFirstTestFromSkeleton()
        {
            Formula f1 = new Formula("x1+y2", normalizer, s => true);
            Formula f2 = new Formula("X1  +  Y2");
            Assert.IsTrue(f1.GetHashCode() == f2.GetHashCode());
        }

        [TestMethod]
        public void TestOperatorNotEqualsFourthTestFromSkeleton()
        {
            Assert.IsFalse(new Formula("2.0 + x7") != new Formula("2.000 + x7"));
        }

        [TestMethod]
        public void TestOperatorNotEqualsThirdTestFromSkeleton()
        {
            Assert.IsTrue(new Formula("x1+y2") != new Formula("y2+x1"));
        }


        [TestMethod]
        public void TestOperatorNotEqualsSecondTestFromSkeleton()
        {
            Assert.IsTrue(new Formula("x1+y2") != new Formula("X1+Y2"));
        }

        [TestMethod]
        public void TestOperatorNotEqualsFirstTestFromSkeleton()
        {
            Assert.IsFalse(new Formula("x1+y2", normalizer, s => true) != new Formula("X1  +  Y2"));
        }

        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestInvalidTokens()
        {
            Formula f = new Formula("$ + 8");
        }

        [TestMethod]
        public void TestOperatorEqualsFirstTestFromSkeleton()
        {
            Formula f1 = new Formula("x1+y2", normalizer, s => true);
            Formula f2 = new Formula("X1  +  Y2");
            Assert.IsTrue(f1 == f2);
        }

        [TestMethod]
        public void TestOperatorEqualsSecondTestFromSkeleton()
        {
            Assert.IsFalse(new Formula("x1+y2") == new Formula("X1+Y2"));
        }

        [TestMethod]
        public void TestOperatorEqualsThirdTestFromSkeleton()
        {
            Assert.IsFalse(new Formula("x1+y2") == new Formula("y2+x1"));
        }

        [TestMethod]
        public void TestOperatorEqualsFourthTestFromSkeleton()
        {
            Assert.IsTrue(new Formula("2.0 + x7") == new Formula("2.000 + x7"));
        }

        [TestMethod]
        public void TestDivisionByZero()
        {
            Formula f = new Formula("1/0");
            Object result = (f.Evaluate(lookup));
            Assert.IsTrue(result is FormulaError);
        }

        [TestMethod]
        public void TestUnFoundVariable()
        {
            Formula f = new Formula("Aaaaaaa12345678");
            Object result = (f.Evaluate(lookup));
            Assert.IsTrue(result is FormulaError);
        }

        [TestMethod]
        public void TestEqualsFirstTestFromSkeleton()
        {
            Assert.IsTrue(new Formula("x1+y2", normalizer, s => true).Equals(new Formula("X1  +  Y2")));
        }

        [TestMethod]
        public void TestEqualsFourthTestFromSkeleton()
        {
            Assert.IsTrue(new Formula("2.0 + x7").Equals(new Formula("2.000 + x7")));
        }

        [TestMethod]
        public void TestEqualsSecondTestFromSkeleton()
        {
            Assert.IsFalse(new Formula("x1+y2").Equals(new Formula("X1+Y2")));
        }

        [TestMethod]
        public void TestEqualsThirdTestFromSkeleton()
        {
            Assert.IsFalse(new Formula("x1+y2").Equals(new Formula("y2+x1")));
        }

        /// <summary>
        /// method to identify variables
        /// </summary>
        /// <param name="variable">variable to identify</param>
        /// <returns>variables value</returns>
        private static double lookup(String variable)
        {
            if (variable == "Aaaaaaa12345678")
            {
                throw new ArgumentException();
            } else
            {
                return 7;
            }
        }
        /// <summary>
        /// Checks is a string is one number follow by one number
        /// </summary>
        /// <param name="variable">The string to check</param>
        /// <returns>true if string is one letter followed with one number,
        /// false otherwise</returns>
        private static bool validator(String variable)
        {
            return Regex.IsMatch(variable, "^[a-zA-Z][0-9]$");
        }

        /// <summary>
        /// Takes in a string and converts all letters to upper case
        /// </summary>
        /// <param name="Item">The String with letters</param>
        /// <returns></returns>
        private static String normalizer(String Item)
        {
            Item = Item.ToUpper();
            return Item;
        }

        /// <summary>
        /// Test Constructor One with Numbers
        /// </summary>
        [TestMethod]
        public void TestConstructorOneNumbers()
        {
            Formula formula = new Formula("1 + 1");

            Assert.AreEqual("1+1", formula.ToString());
        }
        
        /// <summary>
        /// Tests constructor two with numbers
        /// </summary>
        [TestMethod]
        public void TestConstructortwoNumbers()
        {
            Formula formula = new Formula("1 + 1",normalizer,validator);

            Assert.AreEqual("1+1", formula.ToString());
        }

        /// <summary>
        /// Test Constructor One with Variables
        /// </summary>
        [TestMethod]
        public void TestConstructorOneVariables()
        {
            Formula formula = new Formula("x + Y");
            Assert.AreEqual("x+Y", formula.ToString());
        }

        /// <summary>
        /// Test constructor two with variables
        /// </summary>
        [TestMethod]
        public void TestConstructortwoVariables()
        {
            Formula formula = new Formula("x1 + Y1",normalizer,validator);
            Assert.AreEqual("X1+Y1", formula.ToString());
        }

        /// <summary>
        /// Tests toString Method with Canvas Example
        /// </summary>
        [TestMethod]
        public void TestToString()
        {
            Formula formula = new Formula("x + Y");
            Assert.AreEqual("x+Y", formula.ToString());
        }

        /// <summary>
        /// test format Exception with bad starting token
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestContructorOneException()
        {
            Formula formula = new Formula(")");
        }

        /// <summary>
        /// test formatExcption with validator, canvas example
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestContrucTwoExceptCanvas1()
        {
            Formula formula = new Formula("x + y3", normalizer,validator);
        }

        /// <summary>
        /// test FormatException from syntactically incorrect
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestContrucTwoExceptCanvas2()
        {
            Formula formula = new Formula("2x+y3", normalizer, validator);
        }

        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestLastTokenRule()
        {
            Formula f = new Formula("*");
        }

        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestOneTokenRule()
        {
            Formula f = new Formula("");
        }

        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void testExtraFollowingRule()
        {
            Formula f = new Formula("1)(");
        }

        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestParentheseBalance()
        {
            Formula f = new Formula("(((1))");
        }

        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestRightParenthesesRule()
        {
            Formula f = new Formula("(1))");
        }

        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestAnyValueinFormula()
        {
            Formula f = new Formula("()");
        }

        [TestMethod]
        public void testEvaluteWithNumbers()
        {
            Formula f = new Formula("1+1",normalizer,validator);
            double expected = 2;
            double delta = 0.0005;
            double actual = (double)f.Evaluate(lookup);
            Assert.AreEqual(expected, actual, delta);
        }

        [TestMethod]
        public void TestEvaluteComplex()
        {
            Formula f = new Formula("(20+50)+ (10/2) - (5*9)+(A7)", normalizer, validator);
            double expected = 37;
            double delta = 0.0005;
            double actual = (double)f.Evaluate(lookup);
            Assert.AreEqual(expected, actual, delta);
        }

        [TestMethod]
        public void TestEvaluteComplex1()
        {
            Formula f = new Formula("(20+30+50) / (2*5*2)", normalizer, validator);
            double expected = 5;
            double delta = 0.0005;
            double actual = (double)f.Evaluate(lookup);
            Assert.AreEqual(expected, actual, delta);
        }

        [TestMethod]
        public void TestEvaluteComplex2()
        {
            Formula f = new Formula("(20+50)+ (10/2) - (5*9)-(A7)", normalizer, validator);
            double expected = 23;
            double delta = 0.0005;
            double actual = (double)f.Evaluate(lookup);
            Assert.AreEqual(expected, actual, delta);
        }

        [TestMethod]
        public void TestgetVariables()
        {
            Formula f = new Formula("(20+50)+ (10/2) - (5*9)+(a7)", normalizer, validator);
            List<String> variables = new List<String>(f.GetVariables());
            Assert.IsTrue(variables.Contains("A7"));
        }

        [TestMethod]
        public void TestScientificNotationAddEvaluate()
        {
            Formula f = new Formula("5e-2 + 1");
            Assert.AreEqual(1.05,(double)f.Evaluate(lookup));
        }

        [TestMethod]
        public void TestScientificNotationAdd()
        {
            Formula f = new Formula("5e-2 + 1");
            String actual = f.ToString();
            String expected = "0.05+1";
            Assert.AreEqual(actual, expected);
        }

        [TestMethod]
        public void TestScientificNotation()
        {
            Formula f = new Formula("5e-2");
            String actual = f.ToString();
            String expected = "0.05";
            Assert.AreEqual(actual, expected);
        }

        [TestMethod]
        public void TestEqualskeletonExOne()
        {
            Assert.IsTrue(new Formula("x1+y2", normalizer, s => true).Equals(new Formula("X1  +  Y2")));
        }

        [TestMethod]
        public void TestEqualskeletonExfour()
        {
            Assert.IsTrue(new Formula("2.0 + x7").Equals(new Formula("2.000 + x7")));
        }

        [TestMethod]
        public void TestOperatorEqualsOneIsNull()
        {
            Formula f1 = new Formula("1+1");
            Formula f2 = null;
            Assert.IsFalse(f1 == f2);
        }

        [TestMethod]
        public void TestOperatorEqualsOtherIsNull()
        {
            Formula f1 = null;
            Formula f2 = new Formula("1+1");
            Assert.IsFalse(f1 == f2);
        }

        [TestMethod]
        public void TestOperatorNotEqualsOneIsNull()
        {
            Formula f1 = new Formula("1+1");
            Formula f2 = null;
            Assert.IsTrue(f1 != f2);
        }

        [TestMethod]
        public void TestOperatorNotEqualsOtherIsNull()
        {
            Formula f1 = null;
            Formula f2 = new Formula("1+1");
            Assert.IsTrue(f1 != f2);
        }

        [TestMethod]
        public void TestOperatorEqualsBothNull()
        {
            Formula f1 = null;
            Formula f2 = null;
            Assert.IsTrue(f1 == f2);
        }

        [TestMethod]
        public void TestOperatorNotEqualsBothNull()
        {
            Formula f1 = null;
            Formula f2 = null;
            Assert.IsFalse(f1 != f2);
        }

        [TestMethod]
        public void TestEqualsObjNull()
        {
            Formula f1 = new Formula("1");
            Formula f2 = null;
            Assert.IsFalse(((Formula)f1).Equals(f2));
        }

        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestInvalidToken()
        {
            Formula f = new Formula("1 + _$");
        }

        // Normalizer tests
        [TestMethod()]
        public void TestNormalizerGetVars()
        {
            Formula f = new Formula("2+x1", s => s.ToUpper(), s => true);
            HashSet<string> vars = new HashSet<string>(f.GetVariables());

            Assert.IsTrue(vars.SetEquals(new HashSet<string> { "X1" }));
        }

        [TestMethod()]
        public void TestNormalizerEquals()
        {
            Formula f = new Formula("2+x1", s => s.ToUpper(), s => true);
            Formula f2 = new Formula("2+X1", s => s.ToUpper(), s => true);

            Assert.IsTrue(f.Equals(f2));
        }

        [TestMethod()]
        public void TestNormalizerToString()
        {
            Formula f = new Formula("2+x1", s => s.ToUpper(), s => true);
            Formula f2 = new Formula(f.ToString());

            Assert.IsTrue(f.Equals(f2));
        }

        // Validator tests
        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestValidatorFalse()
        {
            Formula f = new Formula("2+x1", s => s, s => false);
        }

        [TestMethod()]
        public void TestValidatorX1()
        {
            Formula f = new Formula("2+x", s => s, s => (s == "x"));
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestValidatorX2()
        {
            Formula f = new Formula("2+y1", s => s, s => (s == "x"));
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestValidatorX3()
        {
            Formula f = new Formula("2+x1", s => s, s => (s == "x"));
        }


        // Simple tests that return FormulaErrors
        [TestMethod()]
        public void TestUnknownVariable1()
        {
            Formula f = new Formula("2+X1");
            Assert.IsInstanceOfType(f.Evaluate(s => { throw new ArgumentException("Unknown variable"); }), typeof(FormulaError));
        }

        [TestMethod()]
        public void TestDivideByZero()
        {
            Formula f = new Formula("5/0");
            Assert.IsInstanceOfType(f.Evaluate(s => 0), typeof(FormulaError));
        }

        [TestMethod()]
        public void TestDivideByZeroVars()
        {
            Formula f = new Formula("(5 + X1) / (X1 - 3)");
            Assert.IsInstanceOfType(f.Evaluate(s => 3), typeof(FormulaError));
        }


        // Tests of syntax errors detected by the constructor
        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestSingleOperator1()
        {
            Formula f = new Formula("+");
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestExtraOperator1()
        {
            Formula f = new Formula("2+5+");
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestExtraCloseParen()
        {
            Formula f = new Formula("2+5*7)");
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestExtraOpenParen()
        {
            Formula f = new Formula("((3+5*7)");
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestNoOperator()
        {
            Formula f = new Formula("5x");
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestNoOperator2()
        {
            Formula f = new Formula("5+5x");
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestNoOperator3()
        {
            Formula f = new Formula("5+7+(5)8");
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestNoOperator4()
        {
            Formula f = new Formula("5 5");
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestDoubleOperator()
        {
            Formula f = new Formula("5 + + 3");
        }

        [TestMethod()]
        [ExpectedException(typeof(FormulaFormatException))]
        public void TestEmpty1()
        {
            Formula f = new Formula("");
        }

        // Some more complicated formula evaluations
        [TestMethod()]
        public void TestComplex1()
        {
            Formula f = new Formula("y1*3-8/2+4*(8-9*2)/14*x7");
            Assert.AreEqual(5.14285714285714, (double)f.Evaluate(s => (s == "x7") ? 1 : 4), 1e-9);
        }

        [TestMethod()]
        public void TestRightParens()
        {
            Formula f = new Formula("x1+(x2+(x3+(x4+(x5+x6))))");
            Assert.AreEqual(6, (double)f.Evaluate(s => 1), 1e-9);
        }

        [TestMethod()]
        public void TestComplexLeftParens()
        {
            Formula f = new Formula("((((x1+x2)+x3)+x4)+x5)+x6");
            Assert.AreEqual(12, (double)f.Evaluate(s => 2), 1e-9);
        }

        [TestMethod()]
        public void TestRepeatedVar1()
        {
            Formula f = new Formula("a4-a4*a4/a4");
            Assert.AreEqual(0, (double)f.Evaluate(s => 3), 1e-9);
        }

        // Test of the Equals method
        [TestMethod()]
        public void TestEqualsBasic()
        {
            Formula f1 = new Formula("X1+X2");
            Formula f2 = new Formula("X1+X2");
            Assert.IsTrue(f1.Equals(f2));
        }

        [TestMethod()]
        public void TestEqualsWhitespace()
        {
            Formula f1 = new Formula("X1+X2");
            Formula f2 = new Formula(" X1  +  X2   ");
            Assert.IsTrue(f1.Equals(f2));
        }

        [TestMethod()]
        public void TestEqualsDouble()
        {
            Formula f1 = new Formula("2+X1*3.00");
            Formula f2 = new Formula("2.00+X1*3.0");
            Assert.IsTrue(f1.Equals(f2));
        }

        [TestMethod()]
        public void TestEqualsComplex()
        {
            Formula f1 = new Formula("0.01 + X5 + 17.00 * 19 ");
            Formula f2 = new Formula("   0.0100  +     X5+ 17 * 19.00000 ");
            Assert.IsTrue(f1.Equals(f2));
        }

        [TestMethod()]
        public void TestEqualsNullAndString()
        {
            Formula f = new Formula("2");
            Assert.IsFalse(f.Equals(null));
            Assert.IsFalse(f.Equals(""));
        }


        // Tests of == operator
        [TestMethod()]
        public void TestEq()
        {
            Formula f1 = new Formula("2");
            Formula f2 = new Formula("2");
            Assert.IsTrue(f1 == f2);
        }

        [TestMethod()]
        public void TestEqFalse()
        {
            Formula f1 = new Formula("2");
            Formula f2 = new Formula("5");
            Assert.IsFalse(f1 == f2);
        }

        [TestMethod()]
        public void TestEqNull()
        {
            Formula f1 = new Formula("2");
            Formula f2 = new Formula("2");
            Assert.IsFalse(null == f1);
            Assert.IsFalse(f1 == null);
            Assert.IsTrue(f1 == f2);
        }


        // Tests of != operator
        [TestMethod()]
        public void TestNotEq()
        {
            Formula f1 = new Formula("2");
            Formula f2 = new Formula("2");
            Assert.IsFalse(f1 != f2);
        }

        [TestMethod()]
        public void TestNotEqTrue()
        {
            Formula f1 = new Formula("2");
            Formula f2 = new Formula("5");
            Assert.IsTrue(f1 != f2);
        }


        // Test of ToString method
        [TestMethod()]
        public void TestString()
        {
            Formula f = new Formula("2*5");
            Assert.IsTrue(f.Equals(new Formula(f.ToString())));
        }


        // Tests of GetHashCode method
        [TestMethod()]
        public void TestHashCode()
        {
            Formula f1 = new Formula("2*5");
            Formula f2 = new Formula("2*5");
            Assert.IsTrue(f1.GetHashCode() == f2.GetHashCode());
        }

        // Technically the hashcodes could not be equal and still be valid,
        // extremely unlikely though. Check their implementation if this fails.
        [TestMethod()]
        public void TestHashCodeFalse()
        {
            Formula f1 = new Formula("2*5");
            Formula f2 = new Formula("3/8*2+(7)");
            Assert.IsTrue(f1.GetHashCode() != f2.GetHashCode());
        }

        [TestMethod()]
        public void TestHashCodeComplex()
        {
            Formula f1 = new Formula("2 * 5 + 4.00 - _x");
            Formula f2 = new Formula("2*5+4-_x");
            Assert.IsTrue(f1.GetHashCode() == f2.GetHashCode());
        }


        // Tests of GetVariables method
        [TestMethod()]
        public void TestVarsNone()
        {
            Formula f = new Formula("2*5");
            Assert.IsFalse(f.GetVariables().GetEnumerator().MoveNext());
        }

        [TestMethod()]
        public void TestVarsSimple()
        {
            Formula f = new Formula("2*X2");
            List<string> actual = new List<string>(f.GetVariables());
            HashSet<string> expected = new HashSet<string>() { "X2" };
            Assert.AreEqual(actual.Count, 1);
            Assert.IsTrue(expected.SetEquals(actual));
        }

        [TestMethod()]
        public void TestVarsTwo()
        {
            Formula f = new Formula("2*X2+Y3");
            List<string> actual = new List<string>(f.GetVariables());
            HashSet<string> expected = new HashSet<string>() { "Y3", "X2" };
            Assert.AreEqual(actual.Count, 2);
            Assert.IsTrue(expected.SetEquals(actual));
        }

        [TestMethod()]
        public void TestVarsDuplicate()
        {
            Formula f = new Formula("2*X2+X2");
            List<string> actual = new List<string>(f.GetVariables());
            HashSet<string> expected = new HashSet<string>() { "X2" };
            Assert.AreEqual(actual.Count, 1);
            Assert.IsTrue(expected.SetEquals(actual));
        }

        [TestMethod()]
        public void TestVarsComplex()
        {
            Formula f = new Formula("X1+Y2*X3*Y2+Z7+X1/Z8");
            List<string> actual = new List<string>(f.GetVariables());
            HashSet<string> expected = new HashSet<string>() { "X1", "Y2", "X3", "Z7", "Z8" };
            Assert.AreEqual(actual.Count, 5);
            Assert.IsTrue(expected.SetEquals(actual));
        }

        // Tests to make sure there can be more than one formula at a time
        [TestMethod()]
        public void TestMultipleFormulae()
        {
            Formula f1 = new Formula("2");
            Formula f2 = new Formula("3");
            Assert.IsTrue(f1.ToString().IndexOf("2") >= 0);
            Assert.IsTrue(f2.ToString().IndexOf("3") >= 0);
        }

        // Repeat this test to increase its weight
        [TestMethod()]
        public void TestMultipleFormulaeB()
        {
            Formula f1 = new Formula("2");
            Formula f2 = new Formula("3");
            Assert.IsTrue(f1.ToString().IndexOf("2") >= 0);
            Assert.IsTrue(f2.ToString().IndexOf("3") >= 0);
        }

        [TestMethod()]
        public void TestMultipleFormulaeC()
        {
            Formula f1 = new Formula("2");
            Formula f2 = new Formula("3");
            Assert.IsTrue(f1.ToString().IndexOf("2") >= 0);
            Assert.IsTrue(f2.ToString().IndexOf("3") >= 0);
        }

        [TestMethod()]
        public void TestMultipleFormulaeD()
        {
            Formula f1 = new Formula("2");
            Formula f2 = new Formula("3");
            Assert.IsTrue(f1.ToString().IndexOf("2") >= 0);
            Assert.IsTrue(f2.ToString().IndexOf("3") >= 0);
        }

        [TestMethod()]
        public void TestMultipleFormulaeE()
        {
            Formula f1 = new Formula("2");
            Formula f2 = new Formula("3");
            Assert.IsTrue(f1.ToString().IndexOf("2") >= 0);
            Assert.IsTrue(f2.ToString().IndexOf("3") >= 0);
        }

        // Stress test for constructor
        [TestMethod()]
        public void TestConstructor()
        {
            // NOTE: Grading tests replaced 7e-5 with 0.00007
            Formula f = new Formula("(((((2+3*X1)/(7e-5+X2-X4))*X5+.0005e+92)-8.2)*3.14159) * ((x2+3.1)-.00000000008)");
        }

        // This test is repeated to increase its weight
        [TestMethod()]
        public void TestConstructorB()
        {
            Formula f = new Formula("(((((2+3*X1)/(7e-5+X2-X4))*X5+.0005e+92)-8.2)*3.14159) * ((x2+3.1)-.00000000008)");
        }

        [TestMethod()]
        public void TestConstructorC()
        {
            Formula f = new Formula("(((((2+3*X1)/(7e-5+X2-X4))*X5+.0005e+92)-8.2)*3.14159) * ((x2+3.1)-.00000000008)");
        }

        [TestMethod()]
        public void TestConstructorD()
        {
            Formula f = new Formula("(((((2+3*X1)/(7e-5+X2-X4))*X5+.0005e+92)-8.2)*3.14159) * ((x2+3.1)-.00000000008)");
        }

        // Stress test for constructor
        [TestMethod()]
        public void TestConstructorE()
        {
            Formula f = new Formula("(((((2+3*X1)/(7e-5+X2-X4))*X5+.0005e+92)-8.2)*3.14159) * ((x2+3.1)-.00000000008)");
        }
    }
}

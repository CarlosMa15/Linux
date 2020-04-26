// Assignment: PS3
// Date: September 16, 2017
// Author: Carlos Martinz

// Skeleton written by Joe Zachary for CS 3500, September 2013
// Read the entire skeleton carefully and completely before you
// do anything else!

// Version 1.1 (9/22/13 11:45 a.m.)

// Change log:
//  (Version 1.1) Repaired mistake in GetTokens
//  (Version 1.1) Changed specification of second constructor to
//                clarify description of how validation works

// (Daniel Kopta) 
// Version 1.2 (9/10/17) 

// Change log:
//  (Version 1.2) Changed the definition of equality with regards
//                to numeric tokens


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace SpreadsheetUtilities
{
    /// <summary>
    /// Represents formulas written in standard infix notation using standard precedence
    /// rules.  The allowed symbols are non-negative numbers written using double-precision 
    /// floating-point syntax; variables that consist of a letter or underscore followed by 
    /// zero or more letters, underscores, or digits; parentheses; and the four operator 
    /// symbols +, -, *, and /.  
    /// 
    /// Spaces are significant only insofar that they delimit tokens.  For example, "xy" is
    /// a single variable, "x y" consists of two variables "x" and y; "x23" is a single variable; 
    /// and "x 23" consists of a variable "x" and a number "23".
    /// 
    /// Associated with every formula are two delegates:  a normalizer and a validator.  The
    /// normalizer is used to convert variables into a canonical form, and the validator is used
    /// to add extra restrictions on the validity of a variable (beyond the standard requirement 
    /// that it consist of a letter or underscore followed by zero or more letters, underscores,
    /// or digits.)  Their use is described in detail in the constructor and method comments.
    /// </summary>
    public class Formula
    {
        /// <summary>
        /// This is a stack used to hold the integer values that a equation might have
        /// </summary>
        private Stack<double> valuesStack = new Stack<double>();

        /// <summary>
        /// This is a stack used to hold the operator that an equation might have
        /// </summary>
        private  Stack<String> operatorsStack = new Stack<String>();

        /// <summary>
        /// this int keeps track of the number of operators in the operator stack,
        /// so the user knows not to remove from a stack that is empty
        /// </summary>
        private int operatorsCounter = 0;

        /// <summary>
        /// this keeps track of the number of values in the value stack,
        /// so the user knows not to remove from a stack that is empty
        /// </summary>
        private int valueCounter = 0;

        /// <summary>
        /// This is a checker used to make sure we only use a value once
        /// </summary>
        private bool MultDivchecker = true;

        /// <summary>
        /// this a variable that will return the answer to the math equation
        /// </summary>
        private double result = 0;

        /// <summary>
        /// This holds the tokens of the equation
        /// </summary>
        private IEnumerable<String> tokens;

        /// <summary>
        /// This hashset contains all your variables of your equation
        /// </summary>
        private HashSet<String> variables = new HashSet<string>();

        /// <summary>
        /// This is a String that holds the normalized equation that we were given,
        /// </summary>
        private String formulaString;

        /// <summary>
        /// Creates a Formula from a string that consists of an infix expression written as
        /// described in the class comment.  If the expression is syntactically invalid,
        /// throws a FormulaFormatException with an explanatory Message.
        /// 
        /// The associated normalizer is the identity function, and the associated validator
        /// maps every string to true.  
        /// </summary>
        public Formula(String formula) :
            this(formula, s => s, s => true)
        {
            
        }

        /// <summary>
        /// Creates a Formula from a string that consists of an infix expression written as
        /// described in the class comment.  If the expression is syntactically incorrect,
        /// throws a FormulaFormatException with an explanatory Message.
        /// 
        /// The associated normalizer and validator are the second and third parameters,
        /// respectively.  
        /// 
        /// If the formula contains a variable v such that normalize(v) is not a legal variable, 
        /// throws a FormulaFormatException with an explanatory message. 
        /// 
        /// If the formula contains a variable v such that isValid(normalize(v)) is false,
        /// throws a FormulaFormatException with an explanatory message.
        /// 
        /// Suppose that N is a method that converts all the letters in a string to upper case, and
        /// that V is a method that returns true only if a string consists of one letter followed
        /// by one digit.  Then:
        /// 
        /// new Formula("x2+y3", N, V) should succeed
        /// new Formula("x+y3", N, V) should throw an exception, since V(N("x")) is false
        /// new Formula("2x+y3", N, V) should throw an exception, since "2x+y3" is syntactically incorrect.
        /// </summary>
        public Formula(String formula, Func<string, string> normalize, Func<string, bool> isValid)
        {

            isSyntacticallyValid(formula, normalize);
            
            foreach(String el in variables)
            {
                if(!isValid(el))
                {
                    throw new FormulaFormatException("According to your validator, One or more" +
                        " variables don't seem to be valid");
                }
            }

            this.formulaString = this.formulaString.Replace(" ", "");
        }

        /// <summary>
        /// This methods checks if the equation is a vaild formula following the Syntax and Syntax Errors
        /// rules from PS3 canvas assignment page. The class also to that an equation has at least one
        /// number or variable, so "()" is not a valid argument according to PS1
        /// 
        /// This methodas as long  as isSyntacticallyValid should also create a set containing your varibles 
        /// </summary>
        /// <param name="equation">The string equation we are checking</param>
        /// <returns></returns>
        private void isSyntacticallyValid(String equation,Func<String,String> norm)
        {
            this.tokens = GetTokens(equation);
            //Right Parentheses Rule
            //Balanced Parentheses Rule
            int leftParentheses = 0;
            int rightParentheses = 0;

            //One Token Rule
            if (tokens.Count() == 0)
            {
                throw new FormulaFormatException("equation seems to Violates The One Token Rule");
            }

            //Ending Token Rule
            String lastToken = tokens.Last();
            if (lastToken.Contains("(") || lastToken.Contains("+") || Regex.IsMatch(lastToken,"^[-]$")
                || lastToken.Contains("*") || lastToken.Contains("/"))
            {
                throw new FormulaFormatException("equation seems to Violates Ending Token Rule");
            }

            //Starting Token Rule
            String firstToken = tokens.First();
            if (firstToken.Contains(")") || firstToken.Contains("+") || Regex.IsMatch(firstToken.Trim(),"^[-]$")
                || firstToken.Contains("*") || firstToken.Contains("/"))
            {
                throw new FormulaFormatException("equation seems to Violates The Starting Token Rule");
            }

            //Extra Following Rule
            bool followingTokenChecker = false;
            //Parenthesis Following Rule
            bool follwoingParChecker = false;
            // parsed number
            double number;

            foreach (String el in tokens)
            {
                if(double.TryParse(el,out number))
                {
                    this.formulaString = this.formulaString + number.ToString();
                }
                else
                {
                    // If Passes Valid token
                    // Adds elements to our set of variables a noralized to String to our String
                    if (Regex.IsMatch(el, "^[_]") || Regex.IsMatch(el, "^[a-zA-Z]"))
                    {
                        if (el.Trim().Length == 1)
                        {
                            this.variables.Add(norm(el));
                            this.formulaString = this.formulaString + norm(el);
                        } else if(el.Trim().Length > 0 && Regex.IsMatch(el, "^[_|a-zA-Z][a-zA-Z0-9|_]+$"))
                        {
                            this.variables.Add(norm(el));
                            this.formulaString = this.formulaString + norm(el);
                        }

                    // Else checks of token is +,-,*,/,(,)
                    } else if (Regex.IsMatch(el, "[+*)(-]") || el.Contains("/"))
                    {
                        // Adds el to formulaString
                        this.formulaString = this.formulaString + el;
                    } else
                    {   // Token is invald throw exception
                        throw new FormulaFormatException("The equation contains invalid token.");
                    }
                }
                //Extra Following Rule
                if (followingTokenChecker)
                {
                    if(el.Contains("(") || Regex.IsMatch(el, "[a-zA-Z0-9]"))
                    {
                        throw new FormulaFormatException("equation seems to Violates The Extra Following Rule");
                    }
                    followingTokenChecker = false;
                }
                //Extra Following Rule
                if (el.Contains(")") || Regex.IsMatch(el, "[a-zA-Z0-9]"))
                {
                    followingTokenChecker = true;
                }
                //Parenthesis Following Rule
                if (follwoingParChecker)
                {
                    if(el.Contains(")") || Regex.IsMatch(el.Trim(), "^[-]$") || Regex.IsMatch(el.Trim(), "^[+]$") ||
                    el.Contains("*") || el.Contains("/"))
                    {
                        throw new FormulaFormatException("equation seems to Violates The Parenthesis Following Rule");
                    }
                    follwoingParChecker = false;
                }
                //Parenthesis Following Rule
                if (el.Contains("(") || Regex.IsMatch(el.Trim(),"^[-]$") || Regex.IsMatch(el.Trim(), "^[+]$") ||
                    el.Contains("*") || el.Contains("/"))
                {
                    follwoingParChecker = true;
                }

                //Right Parentheses Rule
                //Balanced Parentheses Rule
                if (el.Contains("("))
                {
                    leftParentheses += 1;
                }
                //Right Parentheses Rule
                //Balanced Parentheses Rule
                if (el.Contains(")"))
                {
                    rightParentheses += 1;
                }

                //Right Parentheses Rule
                if (rightParentheses > leftParentheses)
                {
                    throw new FormulaFormatException("equation seems to Violates the Right Parentheses Rule");
                }
            }

            this.formulaString = this.formulaString.Replace(" ","");
           
            //Balanced Parentheses Rule
            if (leftParentheses != rightParentheses)
            {
                throw new FormulaFormatException("equation seems to Violates the Balanced Parentheses Rule");
            }
        }

        /// <summary>
        /// Evaluates this Formula, using the lookup delegate to determine the values of
        /// variables.  When a variable symbol v needs to be determined, it should be looked up
        /// via lookup(normalize(v)). (Here, normalize is the normalizer that was passed to 
        /// the constructor.)
        /// 
        /// For example, if L("x") is 2, L("X") is 4, and N is a method that converts all the letters 
        /// in a string to upper case:
        /// 
        /// new Formula("x+7", N, s => true).Evaluate(L) is 11
        /// new Formula("x+7").Evaluate(L) is 9
        /// 
        /// Given a variable symbol as its parameter, lookup returns the variable's value 
        /// (if it has one) or throws an ArgumentException (otherwise).
        /// 
        /// If no undefined variables or divisions by zero are encountered when evaluating 
        /// this Formula, the value is returned.  Otherwise, a FormulaError is returned.  
        /// The Reason property of the FormulaError should have a meaningful explanation.
        ///
        /// This method should never throw an exception.
        /// </summary>
        public object Evaluate(Func<string, double> lookup)
        {
            List<String> substrings = new List<String>(tokens);
            try // Try used to catch Divivtion by Zero other things should be caught by the
                // Constructor before creating formula
            {
                foreach (String el in substrings)
                {
                    // If next token is an Integer, blank space, or Variable
                    if (!Regex.IsMatch(el.Trim(), "^[+]$") && !Regex.IsMatch(el.Trim(), "^[-]$") && !el.Contains("/") && !el.Contains("*")
                        && !el.Contains("(") && !el.Contains(")"))
                    {
                        MultDivchecker = true;
                        String possibleDouble = el.Trim();

                        // Checks if token was not a blank space, if blank space do nothing
                        if (possibleDouble.Length > 0)
                        {

                            int numberOfLetters = 0;
                            numberOfLetters = Regex.Matches(possibleDouble, @"[a-zA-Z]").Count;

                            double doubleValue;

                            //Checks if variable or Scientific notation
                            if (numberOfLetters > 0)
                            {
                                double parseValue; // Checks Scientific notation
                                if (double.TryParse(possibleDouble, out parseValue))
                                {
                                    doubleValue = parseValue;
                                }
                                else // it is a variable
                                {   try
                                    {
                                        doubleValue = lookup(possibleDouble);
                                    } catch (Exception)
                                    {
                                        return new FormulaError("Lookup did Not Find Any Value for your Variable");
                                    }
                                }
                            }
                            else // Else it is a number to parse
                            {
                                doubleValue = double.Parse(possibleDouble);
                            }
                            //Checks the operator stack for multiplication
                            if (operatorsCounter > 0)
                            {
                                String peek = operatorsStack.Peek();

                                if (peek.Contains("*") || peek.Contains("/"))
                                {
                                    double secondValue = valuesStack.Pop();
                                    valueCounter -= 1;
                                    MultDivchecker = false;

                                    multAndDiv(doubleValue, peek, secondValue);
                                }
                            }
                            if (operatorsCounter > 0)
                            {
                                //Checks the operator stack for multiplication if element has not yet been used
                                String peek = operatorsStack.Peek();
                                if (!peek.Contains("*") || !peek.Contains("/"))
                                {
                                    if (MultDivchecker)
                                    {
                                        valuesStack.Push(doubleValue);
                                        valueCounter += 1;
                                    }
                                }
                            }
                            else
                            { //adds value to stack if nothing happened to it or is result
                                if (MultDivchecker)
                                {
                                    valuesStack.Push(doubleValue);
                                    valueCounter += 1;
                                }
                            }
                        }
                    }

                    // If next token is - or +
                    else if (Regex.IsMatch(el.Trim(), "^[+]$") || Regex.IsMatch(el.Trim(), "^[-]$"))
                    {

                        if (operatorsCounter > 0)
                        {
                            String peek = operatorsStack.Peek();

                            addAndSub(peek);
                        }

                        //Adds operator to stack
                        operatorsStack.Push(el);
                        operatorsCounter += 1;
                    } 

                    //If next token is * or /
                    else if (el.Contains("*") || el.Contains("/"))
                    {
                        operatorsCounter += 1;
                        operatorsStack.Push(el);
                    } 
                    
                    // If next token is (
                    else if (el.Contains("("))
                    {
                        operatorsCounter += 1;
                        operatorsStack.Push(el);
                    } 
                    
                    // If next token is )
                    else if (el.Contains(")"))
                    {
                        if (operatorsCounter > 0)
                        {  // subtract or add if needed
                            String peek = operatorsStack.Peek();
                            
                            addAndSub(peek);

                            if (operatorsCounter > 0)
                            {
                                peek = operatorsStack.Peek();

                                if (peek.Contains("("))
                                {
                                    operatorsStack.Pop();
                                    operatorsCounter -= 1;
                                }
                            }
                            if (operatorsCounter > 0)
                            { // Multiply or divide if needed
                                peek = operatorsStack.Peek();

                                if (peek.Contains("*") || peek.Contains("/"))
                                {
                                    double firstValue = valuesStack.Pop();
                                    double secondValue = valuesStack.Pop();
                                    valueCounter -= 2;

                                    multAndDiv(firstValue, peek, secondValue);
                                }
                            }
                        }
                    }
                }

                // Checks if only one thing in value stack to finish
                if (valueCounter == 1)
                {
                    result = valuesStack.Pop();
                    valueCounter -= 1;
                    
                }

                // Checks if two things left from one final step before finishing
                if (operatorsCounter == 1)
                {
                    if (valueCounter == 2)
                    {
                        String peek = operatorsStack.Peek();
                        double firstValue = valuesStack.Pop();
                        double secondValue = valuesStack.Pop();
                        valueCounter -= 2;

                        if (peek.Contains("+"))
                        {
                            double sum = firstValue + secondValue;


                            operatorsStack.Pop();
                            operatorsCounter -= 1;

                            result = sum;
                           
                        }
                        if (peek.Contains("-"))
                        {
                            double difference = secondValue - firstValue;

                            operatorsStack.Pop();
                            operatorsCounter -= 1;

                            result = difference;
                            
                        }
                    }
                }
            }

            // Catches any divition by zero to return FormulaError
            catch (DivideByZeroException e)
            {
                return new FormulaError("Divition By Zero Is Not Allowed");
            }

            // Returns Result
            return result;
        }

        /// <summary>
        /// This methods adds or subtracts to numbers
        /// </summary>
        /// <param name="peek">A String that determins weather we add or subtracts</param>
        private void addAndSub(string peek)
        {
            if (peek.Contains("+") || peek.Contains("-"))
            {
                double firstValue = valuesStack.Pop();
                double secondValue = valuesStack.Pop();
                valueCounter -= 2;

                if (peek.Contains("+"))
                {
                    double sum = firstValue + secondValue;
                    valuesStack.Push(sum);
                    valueCounter += 1;

                    operatorsStack.Pop();
                    operatorsCounter -= 1;
                }
                if (peek.Contains("-"))
                {
                    double difference = secondValue - firstValue;
                    valuesStack.Push(difference);
                    valueCounter += 1;

                    operatorsStack.Pop();
                    operatorsCounter -= 1;
                }
            }
        }

        /// <summary>
        /// This method Multiplies or divides two number depending on the operater presented
        /// </summary>
        /// <param name="integerValue">this is either one of the factors or the divisor</param>
        /// <param name="peek">this is a string that determins weather we will be 
        /// dividing or multiplying</param>
        /// <param name="secondValue"> this is one of the factors or numurator(divident)</param>
        private void multAndDiv(double integerValue, string peek, double secondValue)
        {
            if (peek.Contains("*"))
            {
                double product = integerValue * secondValue;
                valuesStack.Push(product);
                valueCounter += 1;

                operatorsStack.Pop();
                operatorsCounter -= 1;
            }
            if (peek.Contains("/"))
            {
                if(integerValue == 0)
                {
                    throw new DivideByZeroException();
                }
                double quotiont = secondValue / integerValue;
                valuesStack.Push(quotiont);
                valueCounter += 1;

                operatorsStack.Pop();
                operatorsCounter -= 1;
            }
        }

        /// <summary>
        /// Enumerates the normalized versions of all of the variables that occur in this 
        /// formula.  No normalization may appear more than once in the enumeration, even 
        /// if it appears more than once in this Formula.
        /// 
        /// For example, if N is a method that converts all the letters in a string to upper case:
        /// 
        /// new Formula("x+y*z", N, s => true).GetVariables() should enumerate "X", "Y", and "Z"
        /// new Formula("x+X*z", N, s => true).GetVariables() should enumerate "X" and "Z".
        /// new Formula("x+X*z").GetVariables() should enumerate "x", "X", and "z".
        /// </summary>
        public IEnumerable<String> GetVariables()
        {
            return new HashSet<String>(this.variables);
        }

        /// <summary>
        /// Returns a string containing no spaces which, if passed to the Formula
        /// constructor, will produce a Formula f such that this.Equals(f).  All of the
        /// variables in the string should be normalized.
        /// 
        /// For example, if N is a method that converts all the letters in a string to upper case:
        /// 
        /// new Formula("x + y", N, s => true).ToString() should return "X+Y"
        /// new Formula("x + Y").ToString() should return "x+Y"
        /// </summary>
        public override string ToString()
        {
            return this.formulaString;
        }

        /// <summary>
        /// If obj is null or obj is not a Formula, returns false.  Otherwise, reports
        /// whether or not this Formula and obj are equal.
        /// 
        /// Two Formulae are considered equal if they consist of the same tokens in the
        /// same order.  To determine token equality, all tokens are compared as strings 
        /// except for numeric tokens and variable tokens.
        /// Numeric tokens are considered equal if they are equal after being "normalized" 
        /// by C#'s standard conversion from string to double, then back to string. This 
        /// eliminates any inconsistencies due to limited floating point precision.
        /// Variable tokens are considered equal if their normalized forms are equal, as 
        /// defined by the provided normalizer.
        /// 
        /// For example, if N is a method that converts all the letters in a string to upper case:
        ///  
        /// new Formula("x1+y2", N, s => true).Equals(new Formula("X1  +  Y2")) is true
        /// new Formula("x1+y2").Equals(new Formula("X1+Y2")) is false
        /// new Formula("x1+y2").Equals(new Formula("y2+x1")) is false
        /// new Formula("2.0 + x7").Equals(new Formula("2.000 + x7")) is true
        /// </summary>
        public override bool Equals(object obj)
        {
            if (!(obj is Formula) || obj is null) {
                return false;
            }
            if (obj.ToString().Equals(this.ToString()))
            {
                return true;
            } else
            {
                return false;
            }
        }

        /// <summary>
        /// Reports whether f1 == f2, using the notion of equality from the Equals method.
        /// Note that if both f1 and f2 are null, this method should return true.  If one is
        /// null and one is not, this method should return false.
        /// </summary>
        public static bool operator ==(Formula f1, Formula f2)
        {
            if ((f1 is null) && (f2 is null))
            {
                return true;
            }
            else if ((f1 is null) || (f2 is null))
            {
                return false;
            }
            if ((f1 is Formula) && (f2 is Formula) && f1.Equals(f2))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// Reports whether f1 != f2, using the notion of equality from the Equals method.
        /// Note that if both f1 and f2 are null, this method should return false.  If one is
        /// null and one is not, this method should return true.
        /// </summary>
        public static bool operator !=(Formula f1, Formula f2)
        {
            if ((f1 is null) && (f2 is null))
            {
                return false;
            }
            if ((f1 is null) && !(f2 is null))
            {
                return true;
            }
            if ((f2 is null) && !(f1 is null))
            {
                return true;
            }

            if ((f1 is Formula) && (f2 is Formula) && f1.Equals(f2))
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        /// <summary>
        /// Returns a hash code for this Formula.  If f1.Equals(f2), then it must be the
        /// case that f1.GetHashCode() == f2.GetHashCode().  Ideally, the probability that two 
        /// randomly-generated unequal Formulae have the same hash code should be extremely small.
        /// </summary>
        public override int GetHashCode()
        {
            return this.formulaString.GetHashCode();
        }

        /// <summary>
        /// Given an expression, enumerates the tokens that compose it.  Tokens are left paren;
        /// right paren; one of the four operator symbols; a string consisting of a letter or underscore
        /// followed by zero or more letters, digits, or underscores; a double literal; and anything that doesn't
        /// match one of those patterns.  There are no empty tokens, and no token contains white space.
        /// </summary>
        private static IEnumerable<string> GetTokens(String formula)
        {
            // Patterns for individual tokens
            String lpPattern = @"\(";
            String rpPattern = @"\)";
            String opPattern = @"[\+\-*/]";
            String varPattern = @"[a-zA-Z_](?: [a-zA-Z_]|\d)*";
            String doublePattern = @"(?: \d+\.\d* | \d*\.\d+ | \d+ ) (?: [eE][\+-]?\d+)?";
            String spacePattern = @"\s+";

            // Overall pattern
            String pattern = String.Format("({0}) | ({1}) | ({2}) | ({3}) | ({4}) | ({5})",
                                            lpPattern, rpPattern, opPattern, varPattern, doublePattern, spacePattern);

            // Enumerate matching tokens that don't consist solely of white space.
            foreach (String s in Regex.Split(formula, pattern, RegexOptions.IgnorePatternWhitespace))
            {
                if (!Regex.IsMatch(s, @"^\s*$", RegexOptions.Singleline))
                {
                    yield return s;
                }
            }

        }
    }

    /// <summary>
    /// Used to report syntactic errors in the argument to the Formula constructor.
    /// </summary>
    public class FormulaFormatException : Exception
    {
        /// <summary>
        /// Constructs a FormulaFormatException containing the explanatory message.
        /// </summary>
        public FormulaFormatException(String message)
            : base(message)
        {
        }
    }

    /// <summary>
    /// Used as a possible return value of the Formula.Evaluate method.
    /// </summary>
    public struct FormulaError
    {
        /// <summary>
        /// Constructs a FormulaError containing the explanatory reason.
        /// </summary>
        /// <param name="reason"></param>
        public FormulaError(String reason)
            : this()
        {
            Reason = reason;
        }

        /// <summary>
        ///  The reason why this FormulaError was created.
        /// </summary>
        public string Reason { get; private set; }
    }
}
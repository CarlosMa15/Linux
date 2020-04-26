/**
 * Author: Carlos Martinez
 * UID: u0969416
 * Assignment: PS1
 */

using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace FormulaEvaluator
{
    /// <summary>
    /// This class is used to evaluate math equations
    /// The method only evaluates equations with tokens: +,-,*,/,(,), positives integers 
    /// including zero, and variables in the form of a letter or more followed by a 
    /// number or more, should throw an argument exception if invalid argument divition
    /// by zero, or invalid variable. The class also has a delegate that allows the user
    /// to create a function to evaluate variable that an equation might have.
    /// </summary>
    public static class Evaluator
    {

        private static Stack<int> valuesStack = new Stack<int>();
        /// <summary>
        /// This is a stack used to hold the operator that an equation might have
        /// </summary>
        private static Stack<String> operatorsStack = new Stack<String>();

        /// <summary>
        /// this int keeps track of the number of operators in the operator stack,
        /// so the user knows not to remove from a stack that is empty
        /// </summary>
        private static int operatorsCounter = 0;

        /// <summary>
        /// this keeps track of the number of values in the value stack,
        /// so the user knows not to remove from a stack that is empty
        /// </summary>
        private static int valueCounter = 0;

        /// <summary>
        /// This is a checker used to make sure we only use a value once
        /// </summary>
        private static bool MultDivchecker = true;

        /// <summary>
        /// this a variable that will return the answer to the math equation
        /// </summary>
        private static int result = 0;

        /// <summary>
        /// The result is set to zero, this checks that we went through the last steps
        /// even if we don't find nothing, it makes sure for example if argument "()"
        /// is past it throughs an exeption insead of zero.
        /// </summary>
        private static bool checkerResult = false;

        /// <summary>
        /// This is a delegate that allows the user to create a function to look up a variable
        /// </summary>
        /// <param name="v">The variable that you are trying to look up in the form of any
        /// number of letters followed by any number of number(at least one letter and one number)</param>
        /// <returns></returns>
        public delegate int Lookup(String v);




        /// <summary>
        /// This method takes a string math equation and solves it returning the answer.
        /// The method only evaluates equations with tokens: +,-,*,/,(,), positives integers 
        /// including zero, and variables in the form of a letter or more followed by a 
        /// number or more, should throw an argument exception if invalid argument divition
        /// by zero, or invalid variable
        /// </summary>
        /// <param name="exp">The math equation that will be solved</param>
        /// <param name="variableEvaluator">a function used to look up a variable
        /// and return its value</param>
        /// <returns></returns>
        public static int Evaluate(String exp, Lookup variableEvaluator)
        {
             valuesStack = new Stack<int>();
        /// <summary>
        /// This is a stack used to hold the operator that an equation might have
        /// </summary>
        operatorsStack = new Stack<String>();

        /// <summary>
        /// this int keeps track of the number of operators in the operator stack,
        /// so the user knows not to remove from a stack that is empty
        /// </summary>
        operatorsCounter = 0;

        /// <summary>
        /// this keeps track of the number of values in the value stack,
        /// so the user knows not to remove from a stack that is empty
        /// </summary>
         valueCounter = 0;

        /// <summary>
        /// This is a checker used to make sure we only use a value once
        /// </summary>
         MultDivchecker = true;

        /// <summary>
        /// this a variable that will return the answer to the math equation
        /// </summary>
         result = 0;

        /// <summary>
        /// The result is set to zero, this checks that we went through the last steps
        /// even if we don't find nothing, it makes sure for example if argument "()"
        /// is past it throughs an exeption insead of zero.
        /// </summary>
        checkerResult = false;
        string[] substrings = Regex.Split(exp, "(\\()|(\\))|(-)|(\\+)|(\\*)|(/)");

            try
            {
                foreach (String el in substrings)
                {
                    // If next token is an Integer, blank space, or Variable
                    if (!el.Contains("+") && !el.Contains("-") && !el.Contains("/") && !el.Contains("*")
                        && !el.Contains("(") && !el.Contains(")"))
                    {
                        MultDivchecker = true;
                        String possibleInteger = el.Trim();

                        // Checks if token was not a blank space
                        if (possibleInteger.Length > 0)
                        {
                            //Checks if the integer or varible have any unique symbols
                            if (Regex.IsMatch(possibleInteger, "[^a-zA-Z0-9]"))
                            {
                                throw new ArgumentException("Your agument does not seem to be valid, " +
                                    "invalid variables");
                            }

                            int numberOfLetters = 0;
                            numberOfLetters = Regex.Matches(possibleInteger, @"[a-zA-Z]").Count;

                            int integerValue;

                            if (numberOfLetters > 0)
                            {
                                if (Regex.IsMatch(possibleInteger, "[0-9]+[a-zA-Z]+") || 
                                    !Regex.IsMatch(possibleInteger, "[a-zA-Z]+[0-9]+"))
                                {
                                    throw new ArgumentException("Your agument does not seem to be valid, " +
                                    "invalid variables");
                                }
                                    integerValue = variableEvaluator(possibleInteger);
                            }
                            else
                            {
                                integerValue = int.Parse(possibleInteger);
                            }

                            if (operatorsCounter > 0)
                            {
                                String peek = operatorsStack.Peek();

                                if (peek.Contains("*") || peek.Contains("/"))
                                {
                                    int secondValue = valuesStack.Pop();
                                    valueCounter -= 1;
                                    MultDivchecker = false;

                                    multAndDiv(integerValue, peek, secondValue);
                                }
                            }
                            if (operatorsCounter > 0)
                            {
                                String peek = operatorsStack.Peek();
                                if (!peek.Contains("*") || !peek.Contains("/"))
                                {
                                    if (MultDivchecker)
                                    {
                                        valuesStack.Push(integerValue);
                                        valueCounter += 1;
                                    }
                                }
                            }
                            else
                            {
                                if (MultDivchecker)
                                {
                                    valuesStack.Push(integerValue);
                                    valueCounter += 1;
                                }
                            }
                        }
                    }

                    // If next token is - or +
                    else if (el.Contains("+") || el.Contains("-"))
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
                    else if (el.Contains("*") || el.Contains("/"))
                    {
                        operatorsCounter += 1;
                        operatorsStack.Push(el);
                    }
                    else if (el.Contains("("))
                    {
                        operatorsCounter += 1;
                        operatorsStack.Push(el);
                    }
                    else if (el.Contains(")"))
                    {
                        if (operatorsCounter > 0)
                        {
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
                            {
                                peek = operatorsStack.Peek();

                                if (peek.Contains("*") || peek.Contains("/"))
                                {
                                    int firstValue = valuesStack.Pop();
                                    int secondValue = valuesStack.Pop();
                                    valueCounter -= 2;

                                    multAndDiv(firstValue, peek, secondValue);
                                }
                            }
                        }
                    }
                }


                if (valueCounter == 1)
                {
                    result = valuesStack.Pop();
                    valueCounter -= 1;
                    checkerResult = true;
                }

                if (operatorsCounter == 1)
                {
                    if (valueCounter == 2)
                    {
                        String peek = operatorsStack.Peek();
                        int firstValue = valuesStack.Pop();
                        int secondValue = valuesStack.Pop();
                        valueCounter -= 2;

                        if (peek.Contains("+"))
                        {
                            int sum = firstValue + secondValue;
                            

                            operatorsStack.Pop();
                            operatorsCounter -= 1;

                            result = sum;
                            checkerResult = true;
                        }
                        if (peek.Contains("-"))
                        {
                            int difference = secondValue - firstValue;

                            operatorsStack.Pop();
                            operatorsCounter -= 1;

                            result = difference;
                            checkerResult = true;
                        }
                    }
                }
            if (checkerResult == false)
                {
                    throw new System.ArgumentException("Your argument does seem to be valid");
                }


            if (valueCounter != 0 || operatorsCounter != 0)
                {
                    throw new System.ArgumentException("Your argument does seem to be valid");
                }

            } catch (DivideByZeroException e)
            {
                throw new System.ArgumentException("Divivision by Zero is not allowed");
            } catch (InvalidOperationException e)
            {
                throw new System.ArgumentException("Your argument does not seem to be valid");
            }

            return result;
        }

        /// <summary>
        /// This methods adds or subtracts to numbers
        /// </summary>
        /// <param name="peek">A String that determins weather we add or subtracts</param>
        private static void addAndSub(string peek)
        {
            if (peek.Contains("+") || peek.Contains("-"))
            {
                int firstValue = valuesStack.Pop();
                int secondValue = valuesStack.Pop();
                valueCounter -= 2;

                if (peek.Contains("+"))
                {
                    int sum = firstValue + secondValue;
                    valuesStack.Push(sum);
                    valueCounter += 1;

                    operatorsStack.Pop();
                    operatorsCounter -= 1;
                }
                if (peek.Contains("-"))
                {
                    int difference = secondValue - firstValue;
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
        private static void multAndDiv(int integerValue, string peek, int secondValue)
        {
            if (peek.Contains("*"))
            {
                int product = integerValue * secondValue;
                valuesStack.Push(product);
                valueCounter += 1;

                operatorsStack.Pop();
                operatorsCounter -= 1;
            }
            if (peek.Contains("/"))
            {
                int quotiont = secondValue / integerValue;
                valuesStack.Push(quotiont);
                valueCounter += 1;

                operatorsStack.Pop();
                operatorsCounter -= 1;
            }
        }
    }
}

//Carlos Martinez
// PS5
using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using SS;
using SpreadsheetUtilities;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace PS4Tester
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void TestCirExCarlos4()
        {
            AbstractSpreadsheet sheet = new Spreadsheet(isValid, Normilize, "default");
            sheet.SetContentsOfCell("A1", "=A1");
            Assert.AreEqual("#CECV", (string)sheet.GetCellValue("A1"));
        }

        /// <summary>
        /// This is Carlos's Test on circular exception 3
        /// </summary>
        [TestMethod]
        public void TestCirExCarlos3()
        {
            AbstractSpreadsheet sheet = new Spreadsheet(isValid, Normilize, "default");
            sheet.SetContentsOfCell("A1", "=B1");
            sheet.SetContentsOfCell("B1", "1");

            Assert.AreEqual(1, (double)sheet.GetCellValue("A1"));
            Assert.AreEqual(1, (double)sheet.GetCellValue("B1"));

            sheet.SetContentsOfCell("B1", "=A1");
            Assert.AreEqual("#CECV", (string)sheet.GetCellValue("A1"));
            Assert.AreEqual("#CECV", (string)sheet.GetCellValue("B1"));
        }

        /// <summary>
        /// This is Carlos's Test on circular exception 2
        /// </summary>
        [TestMethod]
        public void TestCirExCarlos2()
        {
            AbstractSpreadsheet sheet = new Spreadsheet(isValid, Normilize, "default");
            sheet.SetContentsOfCell("A1", "=B1");
            sheet.SetContentsOfCell("B1", "=A1");
            Assert.AreEqual("#CECV", (string)sheet.GetCellValue("A1"));
            Assert.AreEqual("#CECV", (string)sheet.GetCellValue("B1"));
        }

        /// <summary>
        /// This is Carlos's Test on circular exception 1
        /// </summary>
        [TestMethod]
        public void TestCirExCarlos()
        {
            AbstractSpreadsheet sheet = new Spreadsheet(isValid, Normilize, "default");
            sheet.SetContentsOfCell("A1", "=A1");
            Assert.AreEqual("#CECV", (string)sheet.GetCellValue("A1"));
        }

        [TestMethod]
        public void TestValue()
        {
            AbstractSpreadsheet sheet = new Spreadsheet(isValid,Normilize,"default");
            sheet.SetContentsOfCell("b1","=A1 * 5");
            sheet.SetContentsOfCell("A1", "5");
            Assert.AreEqual(25, (double)sheet.GetCellValue("B1"));
            Assert.AreEqual(5, (double)sheet.GetCellValue("a1"));
        }

        // SETTING CELL TO A STRING
        [TestMethod()]
        [ExpectedException(typeof(ArgumentNullException))]
        public void TestSetNullStringVal()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A8", (string)null);
        }

        private bool isValid(String name)
        {
            int nameLength = name.Length;

            if (nameLength == 0 || nameLength == 1)
            {
                return false;
            }
            else
            {
                if (Regex.IsMatch(name, "^[a-zA-Z]+[0-9]+$"))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

        }

        private String Normilize(String variable)
        {
            return variable.ToUpper();
        }

        /// <summary>
        /// This tests third constructor expect no exceptions
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(SpreadsheetReadWriteException))]
        public void TestThirdConstructor()
        {
            AbstractSpreadsheet sheet = new Spreadsheet("file.text",isValid, Normilize, "1.1");
        }

        /// <summary>
        /// This tests second constructor expect no exceptions
        /// </summary>
        [TestMethod]
        public void TestSecondConstructor()
        {
            AbstractSpreadsheet sheet = new Spreadsheet(isValid,Normilize,"1.1");
        }

        /// <summary>
        /// this method test invalid in get cell value
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestInvalidNameGetCellValue()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.GetCellValue("$");
        }

        /// <summary>
        /// this tests invalid name exception on method
        /// get cell value
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestNullNameGetCellValue()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.GetCellValue(null);
        }

        /// <summary>
        /// This tests the method get cell value
        /// </summary>
        [TestMethod]
        public void TestGetValue()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("B1", "=A1 + 1");
            sheet.SetContentsOfCell("C1", "=B1 + 1");
            sheet.SetContentsOfCell("D1", "=C1 + 1");
            ISet<String> set = sheet.SetContentsOfCell("A1", "=E1 + F1");
            Assert.IsTrue(sheet.GetCellValue("A1") is FormulaError);
            sheet.SetContentsOfCell("E1", "1");
            sheet.SetContentsOfCell("F1", "1");
            Assert.AreEqual((double)sheet.GetCellValue("E1"), 1);
            Assert.AreEqual((double)sheet.GetCellValue("F1"), 1);
            Assert.AreEqual(2,(double)sheet.GetCellValue("A1"));
            Assert.AreEqual(3, (double)sheet.GetCellValue("B1"));
            Assert.AreEqual(4, (double)sheet.GetCellValue("C1"));
            Assert.AreEqual(5, (double)sheet.GetCellValue("D1"));
            sheet.SetContentsOfCell("F1", "2");
            Assert.AreEqual((double)sheet.GetCellValue("F1"), 2);
            Assert.AreEqual(3, (double)sheet.GetCellValue("A1"));
            Assert.AreEqual(4, (double)sheet.GetCellValue("B1"));
            Assert.AreEqual(5, (double)sheet.GetCellValue("C1"));
            Assert.AreEqual(6, (double)sheet.GetCellValue("D1"));
            Assert.AreEqual("", sheet.GetCellValue("G1"));
        }

        /// <summary>
        /// tests the ISet of Formula set cell content
        /// </summary>
        [TestMethod]
        public void TestISetofFormula()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("B1", "=A1 + 1");
            sheet.SetContentsOfCell("C1", "=B1 + 1");
            sheet.SetContentsOfCell("D1", "=C1 + 1");
            ISet<String> set = sheet.SetContentsOfCell("A1", "=E1 + F1");
            Assert.IsTrue(set.Contains("A1"));
            Assert.IsTrue(set.Contains("B1"));
            Assert.IsTrue(set.Contains("C1"));
            Assert.IsTrue(set.Contains("D1"));
            Assert.IsFalse(set.Contains("E1"));
            Assert.IsFalse(set.Contains("F1"));
        }

        /// <summary>
        /// tests the ISet of String set  cell content
        /// </summary>
        [TestMethod]
        public void TestISetofString()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("B1", "=A1 + 1");
            sheet.SetContentsOfCell("C1", "=B1 + 1");
            sheet.SetContentsOfCell("D1", "=C1 + 1");
            ISet<String> set = sheet.SetContentsOfCell("A1", "Hi");
            Assert.IsTrue(set.Contains("A1"));
            Assert.IsTrue(set.Contains("B1"));
            Assert.IsTrue(set.Contains("C1"));
            Assert.IsTrue(set.Contains("D1"));
        }

        /// <summary>
        /// tests the ISet of double set cell content
        /// </summary>
        [TestMethod]
        public void TestISetofDouble()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("B1", "=A1 + 1");
            sheet.SetContentsOfCell("C1", "=B1 + 1");
            sheet.SetContentsOfCell("D1", "=C1 + 1");
            ISet<String> set = sheet.SetContentsOfCell("A1", "1");
            Assert.IsTrue(set.Contains("A1"));
            Assert.IsTrue(set.Contains("B1"));
            Assert.IsTrue(set.Contains("C1"));
            Assert.IsTrue(set.Contains("D1"));
        }

        /*
        /// <summary>
        /// test circular exception from formula
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(CircularException))]
        public void TestCircularException2()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("A1", "=B1");
            sheet.SetContentsOfCell("B1", "=C1 + D1");
            sheet.SetContentsOfCell("B1", "=A1");
        }
        */

        /// <summary>
        /// test circular exception from double
        /// </summary>
        /*
        [TestMethod]
        [ExpectedException(typeof(CircularException))]
        public void TestCircularException1()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("A1", "=B1");
            sheet.SetContentsOfCell("B1", "12");
            sheet.SetContentsOfCell("B1", "=A1");
        }
        */

        /// <summary>
        /// test circular exception from empty string
        /// </summary>
        /*
        [TestMethod]
        [ExpectedException(typeof(CircularException))]
        public void TestCircularException()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("A1", "=B1");
            sheet.SetContentsOfCell("B1", "=A1");
        }
        */

        /// <summary>
        /// test set cell content to formula from a formula
        /// </summary>
        [TestMethod]
        public void TestSetCellToFormulaFromFormula()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("A1", "=B1 + C1");
            ISet<String> set = sheet.SetContentsOfCell("A1", "=D1 + E1");
            Assert.IsTrue(set.Contains("A1"));
            Assert.IsFalse(set.Contains("B1"));
            Assert.IsFalse(set.Contains("C1"));

        }

        /// <summary>
        /// tests that cells is empty after modifing a cell and changing it back to ""
        /// </summary>
        [TestMethod]
        public void TestSetCellToEmptyStringFromFormulaEmptyList()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("A1", "=B1 + C1");
            ISet<String> set = sheet.SetContentsOfCell("A1", "");
            IEnumerable<String> list = sheet.GetNamesOfAllNonemptyCells();
            int size = 0;
            foreach (String el in list)
            {
                size++;
            }
            Assert.AreEqual(0, size);
        }

        /// <summary>
        /// test set cell to empty string from formula
        /// </summary>
        [TestMethod]
        public void TestSetCellToEmptyStringFromFormula()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("A1", "=B1 + C1");
            ISet<String> set = sheet.SetContentsOfCell("A1", "");
            Assert.IsTrue(set.Contains("A1"));
            Assert.IsFalse(set.Contains("B1"));
            Assert.IsFalse(set.Contains("C1"));
        }

        /// <summary>
        /// Test set cell to non empty string from formula
        /// </summary>
        [TestMethod]
        public void TestSetCellToNonEmptyStringFromFormula()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("A1", "=B1 + C1");
            ISet<String> set = sheet.SetContentsOfCell("A1", "Hi");
            Assert.IsTrue(set.Contains("A1"));
            Assert.IsFalse(set.Contains("B1"));
            Assert.IsFalse(set.Contains("C1"));
        }

        /// <summary>
        /// test set cell to  double from a formula
        /// </summary>
        [TestMethod]
        public void TestSetCellToDoubleFromFormula()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("A1", "=B1 + C1");
            ISet<String> set = sheet.SetContentsOfCell("A1", "1");
            Assert.IsTrue(set.Contains("A1"));
            Assert.IsFalse(set.Contains("B1"));
            Assert.IsFalse(set.Contains("C1"));
        }

        /// <summary>
        /// test invalid name in set cell content
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestInvalidNameSetCellFormula()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("1", "=1");
        }

        /// <summary>
        /// test null name in set cell content
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestNullNameSetCellFormula()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell(null, "=1");
        }

        /// <summary>
        /// test invalid name in set cell content
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestInvalidNameSetCellDouble()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("1", "2");
        }

        /// <summary>
        /// test null name in set cell content
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestNullNameSetCellDouble()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell(null, "2");
        }

        /// <summary>
        /// test invalid name in set cell content
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestInvalidNameSetCellString()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("1", "Hi");
        }

        /// <summary>
        /// test null name in set cell content
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestNullNameSetCellString()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell(null, "HI");
        }

        /// <summary>
        /// test invalid name in get cell content
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestInvalidNameLegnthTwo()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.GetCellContents("_$");
        }

        /// <summary>
        /// Test that exception is thrown
        /// </summary>
        [TestMethod]
        public void TestvalidName()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.GetCellContents("_");
        }

        /// <summary>
        /// Test that exception is thrown
        /// </summary>
        [TestMethod]
        public void TestvalidName1()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.GetCellContents("A");
        }

        /// <summary>
        /// tests invalid name with digit
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestInvalidNameDigit()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.GetCellContents("1");
        }

        /// <summary>
        /// test invalid name of an empty string
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestInvalidNameEmptyString()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.GetCellContents("");
        }

        /// <summary>
        /// Test the cell content of a cell that we did nothing to
        /// </summary>
        [TestMethod]
        public void TestCellContentOfNotMofidiedCell()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            Assert.AreEqual("", (String)sheet.GetCellContents("A1"));
        }

        /// <summary>
        /// test than exception is thrown when argument is null
        /// get cell content
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestNullNameInGetCellContent()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.GetCellContents(null);
        }

        /// <summary>
        /// test an expectetion is thrown when name is
        /// invallid in get cell content
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestInvalidNameInGetCellContent()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.GetCellContents("$");
        }

        /// <summary>
        /// Tests that the cell content is set to the double
        /// in the set cell content to double
        /// </summary>
        [TestMethod]
        public void TestSetContentToDouble()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("A1", "1");
            Assert.AreEqual(1, (double)sheet.GetCellContents("A1"));
        }

        /// <summary>
        /// test that the get names if nonemptycells method works
        /// </summary>
        [TestMethod]
        public void TestGetNamesOfAllNonemptyCells()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("A1", "1");
            String actaul = "";
            foreach (String el in sheet.GetNamesOfAllNonemptyCells())
            {
                actaul = el;
            }
            Assert.AreEqual("A1", actaul);
        }

        /// <summary>
        /// test the Iset of the set cell content to double method
        /// </summary>
        [TestMethod]
        public void TestTheISetofSetContentDouble()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            String actual = "";
            foreach (String el in sheet.SetContentsOfCell("A1", "1"))
            {
                actual = el;
            }
            Assert.AreEqual("A1", actual);
        }

        /// <summary>
        /// tests that the set content to string works
        /// </summary>
        [TestMethod]
        public void TestSetContentToString()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("A1", "Carlos");
            Assert.AreEqual("Carlos", sheet.GetCellContents("A1"));
        }

        /// <summary>
        /// tests that the value of the cell works according to the content
        /// </summary>
        [TestMethod]
        public void TestSetContentToFormulaWithValue()
        {
            Formula f = new Formula("1 + A1");
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("A1", "1.5");
            sheet.SetContentsOfCell("B1", "=1 + A1");
            Formula formula = (Formula)sheet.GetCellContents("B1");
            double actual = (double)formula.Evaluate(s => 1.5);
            Assert.AreEqual(2.5, actual);
        }

        /// <summary>
        /// tests that the set cell content to formula works
        /// </summary>
        [TestMethod]
        public void TestSetContentToFormulaWithFormula()
        {
            Formula f = new Formula("1 + A1");
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("A1", "1.5");
            sheet.SetContentsOfCell("B1", "=1 + A1");
            Formula actual = (Formula)sheet.GetCellContents("B1");

            Assert.AreEqual(f.ToString(), actual.ToString());
        }

        /// <summary>
        /// tests write and read from file
        /// </summary>
        /*
        [TestMethod]
        public void TestWriteAndReadFromFile()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("B1", "=A1 + 1");
            sheet.SetContentsOfCell("C1", "=B1 + 1");
            sheet.SetContentsOfCell("D1", "=C1 + 1");
            sheet.SetContentsOfCell("A1", "=E1 + F1");
            sheet.SetContentsOfCell("E1", "1");
            sheet.SetContentsOfCell("F1", "1");
            sheet.SetContentsOfCell("F1", "2");
            sheet.SetContentsOfCell("G1", "Hi");

            String fileName = "C:\\Users\\Carlos Martinez\\Source\\Repos\\cs3500\\PS4\\Spreadsheet\\Spreadsheet.xml";

            sheet.Save(fileName);

            AbstractSpreadsheet sheet1 = new Spreadsheet(fileName, isValid, Normilize, "default");

            Assert.AreEqual(sheet.GetCellContents("A1"), sheet1.GetCellContents("A1"));
            Assert.AreEqual(sheet.GetCellContents("B1"), sheet1.GetCellContents("B1"));
            Assert.AreEqual(sheet.GetCellContents("C1"), sheet1.GetCellContents("C1"));
            Assert.AreEqual(sheet.GetCellContents("D1"), sheet1.GetCellContents("D1"));
            Assert.AreEqual(sheet.GetCellContents("E1"), sheet1.GetCellContents("E1"));
            Assert.AreEqual(sheet.GetCellContents("F1"), sheet1.GetCellContents("F1"));
            Assert.AreEqual(sheet.GetCellContents("G1"), sheet1.GetCellContents("G1"));
            Assert.AreEqual(sheet.GetCellValue("A1"), sheet1.GetCellValue("A1"));
            Assert.AreEqual(sheet.GetCellValue("B1"), sheet1.GetCellValue("B1"));
            Assert.AreEqual(sheet.GetCellValue("C1"), sheet1.GetCellValue("C1"));
            Assert.AreEqual(sheet.GetCellValue("D1"), sheet1.GetCellValue("D1"));
            Assert.AreEqual(sheet.GetCellValue("E1"), sheet1.GetCellValue("E1"));
            Assert.AreEqual(sheet.GetCellValue("F1"), sheet1.GetCellValue("F1"));
            Assert.AreEqual(sheet.GetCellValue("G1"), sheet1.GetCellValue("G1"));
        }
        */

        /// <summary>
        /// test changed from after the spreadsheet is created
        /// </summary>
        /*
        [TestMethod]
        public void TestChangedFromConstructor()
        {
            String fileName = "C:\\Users\\Carlos Martinez\\Source\\Repos\\cs3500\\PS4\\Spreadsheet\\Spreadsheet.xml";

            AbstractSpreadsheet sheet1 = new Spreadsheet(fileName, isValid, Normilize, "default");
            AbstractSpreadsheet sheet = new Spreadsheet();

            Assert.AreEqual(false, sheet1.Changed);
            Assert.AreEqual(false, sheet.Changed);
        }
        */

        /// <summary>
        /// test Changed after a save
        /// </summary>
        /*
        [TestMethod]
        public void TestChangedAfterSaved()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("B1", "=A1 + 1");
            sheet.SetContentsOfCell("C1", "=B1 + 1");
            sheet.SetContentsOfCell("D1", "=C1 + 1");
            sheet.SetContentsOfCell("A1", "=E1 + F1");
            sheet.SetContentsOfCell("E1", "1");
            sheet.SetContentsOfCell("F1", "1");
            sheet.SetContentsOfCell("F1", "2");
            sheet.SetContentsOfCell("G1", "Hi");

            String fileName = "C:\\Users\\Carlos Martinez\\Source\\Repos\\cs3500\\PS4\\Spreadsheet\\Spreadsheet.xml";

            sheet.Save(fileName);

            Assert.AreEqual(false, sheet.Changed);
        }
        */

        /// <summary>
        /// tests the Change method
        /// </summary>
        [TestMethod]
        public void TestChanged()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            sheet.SetContentsOfCell("B1", "=A1 + 1");
            sheet.SetContentsOfCell("C1", "=B1 + 1");
            sheet.SetContentsOfCell("D1", "=C1 + 1");
            sheet.SetContentsOfCell("A1", "=E1 + F1");
            sheet.SetContentsOfCell("E1", "1");
            sheet.SetContentsOfCell("F1", "1");
            sheet.SetContentsOfCell("F1", "2");
            sheet.SetContentsOfCell("G1", "Hi");

            Assert.AreEqual(true, sheet.Changed);
        }

        /// <summary>
        /// tests get version method
        /// </summary>
        /*
        [TestMethod]
        public void TestGetSavedVersion ()
        {
            AbstractSpreadsheet sheet = new Spreadsheet();
            String version = sheet.GetSavedVersion("C:\\Users\\Carlos Martinez\\Source\\Repos\\cs3500\\PS4\\Spreadsheet\\Spreadsheet.xml");
            Assert.AreEqual("default", version);
        }
        */

        /// <summary>
        /// tests name not matching expception
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(SpreadsheetReadWriteException))]
        public void TestNotMatchingVersions ()
        {
            String fileName = "C:\\Users\\Carlos Martinez\\Source\\Repos\\cs3500\\PS4\\Spreadsheet\\Spreadsheet.xml";
            AbstractSpreadsheet sheet1 = new Spreadsheet(fileName, isValid, Normilize, "Default");
        }

        /// <summary>
        /// test iligal start element
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(SpreadsheetReadWriteException))]
        public void TestIligalStartingElement()
        {
            String fileName = "C:\\Users\\Carlos Martinez\\Source\\Repos\\cs3500\\PS4\\Spreadsheet\\iligalStartingelement.xml";
            AbstractSpreadsheet sheet1 = new Spreadsheet(fileName, isValid, Normilize, "default");
        }

        /// <summary>
        /// test no cell start element
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(SpreadsheetReadWriteException))]
        public void TestNocellStartElement()
        {
            String fileName = "C:\\Users\\Carlos Martinez\\Source\\Repos\\cs3500\\PS4\\Spreadsheet\\NoSpreadsheetStartElement.xml";
            AbstractSpreadsheet sheet1 = new Spreadsheet(fileName, isValid, Normilize, "default");
        }

        /// <summary>
        /// test the reverse of the one above
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(SpreadsheetReadWriteException))]
        public void TestNocellStartElementReverse()
        {
            String fileName = "C:\\Users\\Carlos Martinez\\Source\\Repos\\cs3500\\PS4\\Spreadsheet\\NoCellStartElementReverse.xml";
            AbstractSpreadsheet sheet1 = new Spreadsheet(fileName, isValid, Normilize, "default");
        }

        /// <summary>
        /// test not spreadsheet start element
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(SpreadsheetReadWriteException))]
        public void TestNospreadsheetstartingelement()
        {
            String fileName = "C:\\Users\\Carlos Martinez\\Source\\Repos\\cs3500\\PS4\\Spreadsheet\\spreadsheetStartingElement1.xml";
            AbstractSpreadsheet sheet1 = new Spreadsheet(fileName, isValid, Normilize, "default");
        }

        /// <summary>
        /// test no attribute
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(SpreadsheetReadWriteException))]
        public void TestNoAttribute()
        {
            String fileName = "C:\\Users\\Carlos Martinez\\Source\\Repos\\cs3500\\PS4\\Spreadsheet\\NoAttribute.xml";
            AbstractSpreadsheet sheet1 = new Spreadsheet(fileName, isValid, Normilize, "default");
        }

        /// <summary>
        /// tests two spreadsheets starting elements
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(SpreadsheetReadWriteException))]
        public void TestTwoSpreadsheets()
        {
            String fileName = "C:\\Users\\Carlos Martinez\\Source\\Repos\\cs3500\\PS4\\Spreadsheet\\TwoSpreadsheets.xml";
            AbstractSpreadsheet sheet1 = new Spreadsheet(fileName, isValid, Normilize, "default");
        }


        /// <summary>
        /// tests two cell error
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(SpreadsheetReadWriteException))]
        public void TestTwoCellsError()
        {
            String fileName = "C:\\Users\\Carlos Martinez\\Source\\Repos\\cs3500\\PS4\\Spreadsheet\\TwoCellsError.xml";
            AbstractSpreadsheet sheet1 = new Spreadsheet(fileName, isValid, Normilize, "default");
        }

        // EMPTY SPREADSHEETS
        [TestMethod()]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestEmptyGetNull()
        {
            Spreadsheet s = new Spreadsheet();
            s.GetCellContents(null);
        }

        [TestMethod()]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestEmptyGetContents()
        {
            Spreadsheet s = new Spreadsheet();
            s.GetCellContents("1AA");
        }

        [TestMethod()]
        public void TestGetEmptyContents()
        {
            Spreadsheet s = new Spreadsheet();
            Assert.AreEqual("", s.GetCellContents("A2"));
        }

        // SETTING CELL TO A DOUBLE
        [TestMethod()]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestSetNullDouble()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell(null, "1.5");
        }

        [TestMethod()]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestSetInvalidNameDouble()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("1A1A", "1.5");
        }

        [TestMethod()]
        public void TestSimpleSetDouble()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("Z7", "1.5");
            Assert.AreEqual(1.5, (double)s.GetCellContents("Z7"), 1e-9);
        }

        // SETTING CELL TO A STRING
        [TestMethod()]
        [ExpectedException(typeof(ArgumentNullException))]
        public void TestSetNullStringVal1()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A8", (string)null);
        }

        [TestMethod()]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestSetNullStringName()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell(null, "hello");
        }

        [TestMethod()]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestSetSimpleString()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("1AZ", "hello");
        }

        [TestMethod()]
        public void TestSetGetSimpleString()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("Z7", "hello");
            Assert.AreEqual("hello", s.GetCellContents("Z7"));
        }

        // SETTING CELL TO A FORMULA
        [TestMethod()]
        [ExpectedException(typeof(ArgumentNullException))]
        public void TestSetNullFormVal()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A8", (String)null);
        }

        [TestMethod()]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestSetNullFormName()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell(null, "=2");
        }

        [TestMethod()]
        [ExpectedException(typeof(InvalidNameException))]
        public void TestSetSimpleForm()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("1AZ", "=2");
        }

        [TestMethod()]
        public void TestSetGetForm()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("Z7", "=3");
            Formula f = (Formula)s.GetCellContents("Z7");
            Assert.AreEqual(new Formula("3"), f);
            Assert.AreNotEqual(new Formula("2"), f);
        }

        /*
        // CIRCULAR FORMULA DETECTION
        [TestMethod()]
        [ExpectedException(typeof(CircularException))]
        public void TestSimpleCircular()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "=A2");
            s.SetContentsOfCell("A2", "=A1");
        }
        */

        /*
        [TestMethod()]
        [ExpectedException(typeof(CircularException))]
        public void TestComplexCircular()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "=A2+A3");
            s.SetContentsOfCell("A3", "=A4+A5");
            s.SetContentsOfCell("A5", "=A6+A7");
            s.SetContentsOfCell("A7", "=A1+A1");
        }
        */

        /*
        [TestMethod()]
        [ExpectedException(typeof(CircularException))]
        public void TestUndoCircular()
        {
            Spreadsheet s = new Spreadsheet();
            try
            {
                s.SetContentsOfCell("A1", "=A2+A3");
                s.SetContentsOfCell("A2", "15");
                s.SetContentsOfCell("A3", "30");
                s.SetContentsOfCell("A2", "=A3*A1");
            }
            catch (CircularException e)
            {
                Assert.AreEqual(15, (double)s.GetCellContents("A2"), 1e-9);
                throw e;
            }
        }
        */

        // NONEMPTY CELLS
        [TestMethod()]
        public void TestEmptyNames()
        {
            Spreadsheet s = new Spreadsheet();
            Assert.IsFalse(s.GetNamesOfAllNonemptyCells().GetEnumerator().MoveNext());
        }

        [TestMethod()]
        public void TestExplicitEmptySet()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("B1", "");
            Assert.IsFalse(s.GetNamesOfAllNonemptyCells().GetEnumerator().MoveNext());
        }

        [TestMethod()]
        public void TestSimpleNamesString()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("B1", "hello");
            Assert.IsTrue(new HashSet<string>(s.GetNamesOfAllNonemptyCells()).SetEquals(new HashSet<string>() { "B1" }));
        }

        [TestMethod()]
        public void TestSimpleNamesDouble()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("B1", "52.25");
            Assert.IsTrue(new HashSet<string>(s.GetNamesOfAllNonemptyCells()).SetEquals(new HashSet<string>() { "B1" }));
        }

        [TestMethod()]
        public void TestSimpleNamesFormula()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("B1", "=3.5");
            Assert.IsTrue(new HashSet<string>(s.GetNamesOfAllNonemptyCells()).SetEquals(new HashSet<string>() { "B1" }));
        }

        [TestMethod()]
        public void TestMixedNames()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "17.2");
            s.SetContentsOfCell("C1", "hello");
            s.SetContentsOfCell("B1", "=3.5");
            Assert.IsTrue(new HashSet<string>(s.GetNamesOfAllNonemptyCells()).SetEquals(new HashSet<string>() { "A1", "B1", "C1" }));
        }

        // RETURN VALUE OF SET CELL CONTENTS
        [TestMethod()]
        public void TestSetSingletonDouble()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("B1", "hello");
            s.SetContentsOfCell("C1", "=5");
            Assert.IsTrue(s.SetContentsOfCell("A1", "17.2").SetEquals(new HashSet<string>() { "A1" }));
        }

        [TestMethod()]
        public void TestSetSingletonString()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "17.2");
            s.SetContentsOfCell("C1", "=5");
            Assert.IsTrue(s.SetContentsOfCell("B1", "hello").SetEquals(new HashSet<string>() { "B1" }));
        }

        [TestMethod()]
        public void TestSetSingletonFormula()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "17.2");
            s.SetContentsOfCell("B1", "hello");
            Assert.IsTrue(s.SetContentsOfCell("C1", "=5").SetEquals(new HashSet<string>() { "C1" }));
        }

        [TestMethod()]
        public void TestSetChain()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "=A2+A3");
            s.SetContentsOfCell("A2", "6");
            s.SetContentsOfCell("A3", "=A2+A4");
            s.SetContentsOfCell("A4", "=A2+A5");
            Assert.IsTrue(s.SetContentsOfCell("A5", "82.5").SetEquals(new HashSet<string>() { "A5", "A4", "A3", "A1" }));
        }

        // CHANGING CELLS
        [TestMethod()]
        public void TestChangeFtoD()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "=A2+A3");
            s.SetContentsOfCell("A1", "2.5");
            Assert.AreEqual(2.5, (double)s.GetCellContents("A1"), 1e-9);
        }

        [TestMethod()]
        public void TestChangeFtoS()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "=A2+A3");
            s.SetContentsOfCell("A1", "Hello");
            Assert.AreEqual("Hello", (string)s.GetCellContents("A1"));
        }

        [TestMethod()]
        public void TestChangeStoF()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "Hello");
            s.SetContentsOfCell("A1", "=23");
            Assert.AreEqual(new Formula("23"), (Formula)s.GetCellContents("A1"));
            Assert.AreNotEqual(new Formula("24"), (Formula)s.GetCellContents("A1"));
        }

        // STRESS TESTS
        [TestMethod()]
        public void TestStress1()
        {
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "=B1+B2");
            s.SetContentsOfCell("B1", "=C1-C2");
            s.SetContentsOfCell("B2", "=C3*C4");
            s.SetContentsOfCell("C1", "=D1*D2");
            s.SetContentsOfCell("C2", "=D3*D4");
            s.SetContentsOfCell("C3", "=D5*D6");
            s.SetContentsOfCell("C4", "=D7*D8");
            s.SetContentsOfCell("D1", "=E1");
            s.SetContentsOfCell("D2", "=E1");
            s.SetContentsOfCell("D3", "=E1");
            s.SetContentsOfCell("D4", "=E1");
            s.SetContentsOfCell("D5", "=E1");
            s.SetContentsOfCell("D6", "=E1");
            s.SetContentsOfCell("D7", "=E1");
            s.SetContentsOfCell("D8", "=E1");
            ISet<String> cells = s.SetContentsOfCell("E1", "0");
            Assert.IsTrue(new HashSet<string>() { "A1", "B1", "B2", "C1", "C2", "C3", "C4", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "E1" }.SetEquals(cells));
        }

        // Repeated for extra weight
        [TestMethod()]
        public void TestStress1a()
        {
            TestStress1();
        }
        [TestMethod()]
        public void TestStress1b()
        {
            TestStress1();
        }
        [TestMethod()]
        public void TestStress1c()
        {
            TestStress1();
        }

        [TestMethod()]
        public void TestStress2()
        {
            Spreadsheet s = new Spreadsheet();
            ISet<String> cells = new HashSet<string>();
            for (int i = 1; i < 200; i++)
            {
                cells.Add("A" + i);
                Assert.IsTrue(cells.SetEquals(s.SetContentsOfCell("A" + i, "=A" + (i + 1))));
            }
        }
        [TestMethod()]
        public void TestStress2a()
        {
            TestStress2();
        }
        [TestMethod()]
        public void TestStress2b()
        {
            TestStress2();
        }
        [TestMethod()]
        public void TestStress2c()
        {
            TestStress2();
        }

        /*
        [TestMethod()]
        public void TestStress3()
        {
            Spreadsheet s = new Spreadsheet();
            for (int i = 1; i < 200; i++)
            {
                s.SetContentsOfCell("A" + i, "=A" + (i + 1));
            }
            try
            {
                s.SetContentsOfCell("A150", "=A50");
                Assert.Fail();
            }
            catch (CircularException)
            {
            }
        }
        */

        /*
        [TestMethod()]
        public void TestStress3a()
        {
            TestStress3();
        }
        [TestMethod()]
        public void TestStress3b()
        {
            TestStress3();
        }
        [TestMethod()]
        public void TestStress3c()
        {
            TestStress3();
        }
        */

        [TestMethod()]
        public void TestStress4()
        {
            Spreadsheet s = new Spreadsheet();
            for (int i = 0; i < 500; i++)
            {
                s.SetContentsOfCell("A1" + i, "=A1" + (i + 1));
            }
            HashSet<string> firstCells = new HashSet<string>();
            HashSet<string> lastCells = new HashSet<string>();
            for (int i = 0; i < 250; i++)
            {
                firstCells.Add("A1" + i);
                lastCells.Add("A1" + (i + 250));
            }
            Assert.IsTrue(s.SetContentsOfCell("A1249", "25.0").SetEquals(firstCells));
            Assert.IsTrue(s.SetContentsOfCell("A1499", "0").SetEquals(lastCells));
        }

        [TestMethod]
        public void TestSum()
        {
            Spreadsheet s = new Spreadsheet();
            for(int i = 0; i < 10; i++)
            {
                s.SetContentsOfCell("A1" + i, i.ToString());
            }
            s.SetContentsOfCell("B1", "=sum(A10-A19)");
            Assert.AreEqual(45.0, s.GetCellValue("B1"));
            
        }

        [TestMethod]
        public void TestSumReverse()
        {
            
            Spreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("B1", "=sum(A10-A1)");

            for (int i = 1; i <= 10; i++)
            {
                s.SetContentsOfCell("A" + i, "1");
            }
            s.SetContentsOfCell("B1", "=sum(A10-A1)");
            Assert.AreEqual(10.0, s.GetCellValue("B1"));

        }

        [TestMethod]
        public void TestSumReverseTwoLine()
        {
            Spreadsheet s = new Spreadsheet();
            for (int i = 1; i <= 10; i++)
            {
                s.SetContentsOfCell("A" + i, "1");
            }
            for (int i = 1; i <= 10; i++)
            {
                s.SetContentsOfCell("B" + i, "1");
            }
            s.SetContentsOfCell("C1", "=sum(B1-A10)");
            s.SetContentsOfCell("C2", "=sum(B10-A1)");
            s.SetContentsOfCell("C3", "=sum(A2-B1)");
            Assert.AreEqual(20.0, s.GetCellValue("C1"));
            Assert.AreEqual(20.0, s.GetCellValue("C2"));
            Assert.AreEqual(4.0, s.GetCellValue("C3"));

        }

        [TestMethod]
        public void TestSumTwoRows()
        {
            Spreadsheet s = new Spreadsheet();
            for (int i = 1; i < 10; i++)
            {
                s.SetContentsOfCell("A" + i, "1");
            }
            for (int i = 1; i < 10; i++)
            {
                s.SetContentsOfCell("B" + i, "1");
            }
            s.SetContentsOfCell("C1", "=sum(A1-B9)");
            Assert.AreEqual(18.0, s.GetCellValue("C1"));

        }

        [TestMethod]
        public void TestSumMultipleRows()
        {
            Spreadsheet s = new Spreadsheet();
            for (int i = 1; i <= 10; i++)
            {
                for(char c = 'A'; c <= 'Z'; c++)
                {
                    s.SetContentsOfCell(c.ToString() + i, "1");
                }
            }
            
            s.SetContentsOfCell("C11", "=sum(A1-Z10)");
            Assert.AreEqual(260.0, s.GetCellValue("C11"));

        }
    }
}


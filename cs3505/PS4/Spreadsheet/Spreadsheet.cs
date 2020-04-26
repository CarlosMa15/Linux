// Carlos Martinez
// September 22, 2017
// Assignment PS5

using SpreadsheetUtilities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Xml;

namespace SS
{
    public class Spreadsheet : AbstractSpreadsheet
    {
        /// <summary>
        /// this keeps track of any changes to spreadsheet
        /// </summary>
        private bool changeChecker;

        /// <summary>
        /// This keeps track of the path to file name, ""
        /// if there is no path
        /// </summary>
        private String PathToFile;

        /// <summary>
        /// This hold the all the cells, the String is the Key,
        /// and the cell content is the value, the value can be
        /// a String ,Formula, or Double
        /// </summary>
        private Dictionary<String, Cell> cells;

        /// <summary>
        /// This is the dependency graph that hold the relationship
        /// between cells, who is whos dependent or dependee
        /// </summary>
        private DependencyGraph dependencyGraph;

        /// <summary>
        /// This is just a list that holds the cell of the object that exist(been modified)
        /// </summary>
        private HashSet<String> nonEmptyCellCollection;

        // ADDED FOR PS5
        /// <summary>
        /// True if this spreadsheet has been modified since it was created or saved                  
        /// (whichever happened most recently); false otherwise.
        /// </summary>
        public override bool Changed { get => changeChecker; protected set => changeChecker = value; }

        /// <summary>
        /// A non arguement Constructor that Constructs an empty constructor
        /// </summary>
        public Spreadsheet() : base(s => true, s => s, "default")
        {
            this.PathToFile = "";
            this.cells = new Dictionary<string, Cell>();
            this.dependencyGraph = new DependencyGraph();
            this.nonEmptyCellCollection = new HashSet<string>();
            Changed = false;

        }

        /// <summary>
        /// A three paramenter constructor that allows you to to
        /// pass a validator delegate, normalizer delegate and the save 
        /// </summary>
        /// <param name="isValid"></param>
        /// <param name="normalize"></param>
        /// <param name="version"></param>
        public Spreadsheet(Func<string, bool> isValid, Func<string,
            string> normalize, string version)
            : base(isValid, normalize, version)
        {
            this.PathToFile = "";
            this.cells = new Dictionary<string, Cell>();
            this.dependencyGraph = new DependencyGraph();
            this.nonEmptyCellCollection = new HashSet<string>();
            Changed = false;
        }

        /// <summary>
        /// A three parameter constructor that allows you to pass a path to file,
        /// a validator delegate, a normalizer delegate and the version
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="isValid"></param>
        /// <param name="normalize"></param>
        /// <param name="version"></param>
        public Spreadsheet(String filePath, Func<string, bool> isValid,
            Func<string, string> normalize, string version)
            : base(isValid, normalize, version)
        {
            this.PathToFile = filePath;
            this.cells = new Dictionary<string, Cell>();
            this.dependencyGraph = new DependencyGraph();
            this.nonEmptyCellCollection = new HashSet<string>();

            if (GetSavedVersion(filePath) != Version)
            {
                throw new SpreadsheetReadWriteException("Versions don't match");
            }

            bool nameChecker = false;
            String name = "";
            bool contentChecker = false;
            String content = "";
            bool cellChecker = false;
            bool spreadsheetChecker = false;
            HashSet<String> legalElements = new HashSet<String>() { "spreadsheet", "contents", "cell", "name" };

            try
            {
                using (XmlReader reader = XmlReader.Create(filePath))
                {
                    while (reader.Read())
                    {
                        if (reader.IsStartElement())
                        {
                            if (!legalElements.Contains(reader.Name))
                            {
                                throw new FormatException();
                            }
                            switch (reader.Name)
                            {
                                case "spreadsheet":
                                    if (spreadsheetChecker)
                                    {
                                        throw new FormatException();
                                    }
                                    spreadsheetChecker = true;
                                    break;
                                case "cell":
                                    if (cellChecker)
                                    {
                                        throw new FormatException("has multiple a cell inside an cell");
                                    }
                                    cellChecker = true;
                                    break;
                                case "name":
                                    if (!cellChecker || nameChecker)
                                    {
                                        throw new FormatException("has multiple names or no cell");
                                    }
                                    reader.Read();
                                    name = reader.Value;
                                    nameChecker = true;
                                    break;

                                case "contents":
                                    if (!cellChecker || contentChecker)
                                    {
                                        throw new FormatException("has multiple contents or no cell");
                                    }
                                    reader.Read();
                                    content = reader.Value;
                                    contentChecker = true;
                                    break;
                            }

                            if (nameChecker && contentChecker)
                            {
                                SetContentsOfCell(name, content);
                                nameChecker = false;
                                contentChecker = false;
                                cellChecker = false;
                            }
                        }
                    }
                }
            }
            catch (Exception e)
            {
                throw new SpreadsheetReadWriteException("Problem in reading from your file or " + e.Message);
            }

            Changed = false;
        }

        /// <summary>
        /// If name is null or invalid, throws an InvalidNameException.
        /// 
        /// Otherwise, returns the contents (as opposed to the value) of the named cell.  The return
        /// value should be either a string, a double, or a Formula.
        public override object GetCellContents(string name)
        {
            name = Normalize(name);
            if (name == null || !(isValidName(name)))
            {
                throw new InvalidNameException();
            }

            if (cells.ContainsKey(name))
            {
                return cells[name].content;
            }
            else
            {
                return "";
            }
        }

        /// <summary>
        /// Enumerates the names of all the non-empty cells in the spreadsheet.
        /// </summary>
        public override IEnumerable<string> GetNamesOfAllNonemptyCells()
        {
            return new List<String>(this.nonEmptyCellCollection);
        }

        /// <summary>
        /// If name is null or invalid, throws an InvalidNameException.
        /// 
        /// Otherwise, the contents of the named cell becomes number.  The method returns a
        /// set consisting of name plus the names of all other cells whose value depends, 
        /// directly or indirectly, on the named cell.
        /// 
        /// For example, if name is A1, B1 contains A1*2, and C1 contains B1+A1, the
        /// set {A1, B1, C1} is returned.
        /// </summary>
        protected override ISet<string> SetCellContents(string name, double number)
        {
            name = Normalize(name);
            if (name == null || !(isValidName(name)))
            {
                throw new InvalidNameException();
            }

            if (cells.ContainsKey(name))
            {
                isFormulaRemoveDependents(name);
                setCellToDouble(name, number);
                this.nonEmptyCellCollection.Add(name);
            }
            else
            {
                cells.Add(name, new Cell());
                setCellToDouble(name, number);
                this.nonEmptyCellCollection.Add(name);
            }

            bool cirChecker = false;
            IEnumerable<String> cellsToRecalculate = GetCellsToRecalculate(name, ref cirChecker);
            updateValues(cellsToRecalculate);
            Changed = true;
            return new HashSet<String>(cellsToRecalculate);
        }

        /// <summary>
        /// This method set the value of the cell to what ever number is given
        /// </summary>
        /// <param name="name">name of cell</param>
        /// <param name="number">number(content and value)</param>
        protected void setCellToDouble(String name, double number)
        {
            cells[name].content = number;
            cells[name].value = number;
        }

        /// <summary>
        /// Uses the get cells to recalute IEnumable to recalulate the values of the cells
        /// </summary>
        /// <param name="valuesToUpdate"></param>
        private void updateValues(IEnumerable<String> valuesToUpdate)
        {
            foreach (String el in valuesToUpdate)
            {
                if (!cells.ContainsKey(el))
                {
                    break;
                }
                Object content = cells[el].content;
                if (content is Formula)
                {
                    //Formula formula = (Formula)content;

                    Formula formula1 = (Formula)content;
                    String contentToString = formula1.ToString();
                    Formula formula = new Formula(contentToString, Normalize, IsValid);
                    cells[el].value = formula.Evaluate(lookUp);
                }
            }
        }

        /// <summary>
        /// If text is null, throws an ArgumentNullException.
        /// 
        /// Otherwise, if name is null or invalid, throws an InvalidNameException.
        /// 
        /// Otherwise, the contents of the named cell becomes text.  The method returns a
        /// set consisting of name plus the names of all other cells whose value depends, 
        /// directly or indirectly, on the named cell.
        /// 
        /// For example, if name is A1, B1 contains A1*2, and C1 contains B1+A1, the
        /// set {A1, B1, C1} is returned.
        /// </summary>
        protected override ISet<string> SetCellContents(string name, string text)
        {
            name = Normalize(name);
            if (name == null || !(isValidName(name)))
            {
                throw new InvalidNameException();
            }

            if (cells.ContainsKey(name))
            {
                isFormulaRemoveDependents(name);
                if (text == "")
                {
                    emptyingCell(name, text);
                }
                else
                {
                    setCellToNonEmptySring(name, text);
                }
            }
            else
            {
                if (text != "")
                {
                    cells.Add(name, new Cell());
                    setCellToNonEmptySring(name, text);

                }
            }

            bool cirChecker = false;
            IEnumerable<String> cellsToRecalculate = GetCellsToRecalculate(name, ref cirChecker);
            updateValues(cellsToRecalculate);
            Changed = true;
            return new HashSet<String>(cellsToRecalculate);
        }

        /// <summary>
        /// Checks if the content is a formula to remove depenencies 
        /// </summary>
        /// <param name="name">name of cell</param>
        private void isFormulaRemoveDependents(String name)
        {
            if (GetCellContents(name) is Formula)
            {
                Formula formula = (Formula)GetCellContents(name);
                IEnumerable<String> variables = formula.GetVariables();
                foreach (String el in variables)
                {
                    dependencyGraph.RemoveDependency(el, name);
                }

            }
        }

        /// <summary>
        /// This sets the cell properties to String elemnets
        ///  and adds it to the nonemptycell set
        /// </summary>
        /// <param name="name"></param>
        /// <param name="text"></param>
        public void setCellToNonEmptySring(String name, String text)
        {
            setCellToString(name, text);
            if (!this.nonEmptyCellCollection.Contains(name))
            {
                this.nonEmptyCellCollection.Add(name);
            }
        }

        /// <summary>
        /// An Empty cell has an empty String("")
        /// this empties the cell and removes it from
        /// the nonemptycells set
        /// </summary>
        /// <param name="name"></param>
        /// <param name="text"></param>
        public void emptyingCell(String name, String text)
        {
            setCellToString(name, text);
            if (this.nonEmptyCellCollection.Contains(name))
            {
                this.nonEmptyCellCollection.Remove(name);
            }
        }

        /// <summary>
        /// This sets the content of the cell to String elements
        /// </summary>
        /// <param name="name"></param>
        /// <param name="text"></param>
        public void setCellToString(String name, String text)
        {
            cells[name].content = text;
            cells[name].value = text;
        }


        /// <summary>
        /// If the formula parameter is null, throws an ArgumentNullException.
        /// 
        /// Otherwise, if name is null or invalid, throws an InvalidNameException.
        /// 
        /// Otherwise, if changing the contents of the named cell to be the formula would cause a 
        /// circular dependency, throws a CircularException.  (No change is made to the spreadsheet.)
        /// 
        /// Otherwise, the contents of the named cell becomes formula.  The method returns a
        /// Set consisting of name plus the names of all other cells whose value depends,
        /// directly or indirectly, on the named cell.
        /// 
        /// For example, if name is A1, B1 contains A1*2, and C1 contains B1+A1, the
        /// set {A1, B1, C1} is returned.
        /// </summary>
        protected override ISet<string> SetCellContents(string name, Formula formula)
        {
            name = Normalize(name);
            if (name == null || !(isValidName(name)))
            {
                throw new InvalidNameException();
            }

            Object oldContent = "";

            if (cells.ContainsKey(name))
            {
                oldContent = cells[name].content;
                isFormulaRemoveDependents(name);
                setCellsToFormula(name, formula);
            }
            else
            {
                cells.Add(name, new Cell());
                setCellsToFormula(name, formula);
            }

            bool cirChecker = false; // Checks for circular expections

            //try
            //{

            IEnumerable<String> cellsToRecalculate = GetCellsToRecalculate(name, ref cirChecker);
            if (cirChecker)
            {
                updateValuesToCircularException(cellsToRecalculate);
            }
            else
            {
                updateValues(cellsToRecalculate);
            }

            Changed = true; // Change is a checker that keeps tracked if spreadsheet has been changed
            return new HashSet<String>(cellsToRecalculate);

            //}
            /*
            catch (CircularException)
            {
                setCellBackToOriginalContent(name, oldContent);
                throw new CircularException();
            }
            */
        }

        public void updateValuesToCircularException(IEnumerable<string> cellsList)
        {
            foreach (string value in cellsList)
            {
                if(!cells.ContainsKey(value))
                {
                    break;
                }
                cells[value].value = "#CECV";
            }
        }

        /// <summary>
        /// Sets the cell content to Formula and adds dependecies
        /// </summary>
        /// <param name="name"></param>
        /// <param name="formula"></param>
        private void setCellsToFormula(String name, Formula formula)
        {
            cells[name].content = formula;
            cells[name].value = formula.Evaluate(lookUp);
            foreach (String el in formula.GetVariables())
            {
                dependencyGraph.AddDependency(el, name);
            }
            nonEmptyCellCollection.Add(name);
        }

        private void setCellBackToOriginalContent(String name, Object oldContent)
        {
            isFormulaRemoveDependents(name);
            if (oldContent is String)
            {
                SetCellContents(name, (string)oldContent);
            }
            else if (oldContent is double)
            {
                SetCellContents(name, (double)oldContent);
            }
            else setCellsToFormula(name, (Formula)oldContent);
        }

        /// <summary>
        /// If name is null, throws an ArgumentNullException.
        /// 
        /// Otherwise, if name isn't a valid cell name, throws an InvalidNameException.
        /// 
        /// Otherwise, returns an enumeration, without duplicates, of the names of all cells whose
        /// values depend directly on the value of the named cell.  In other words, returns
        /// an enumeration, without duplicates, of the names of all cells that contain
        /// formulas containing name.
        /// 
        /// For example, suppose that
        /// A1 contains 3
        /// B1 contains the formula A1 * A1
        /// C1 contains the formula B1 + A1
        /// D1 contains the formula B1 - C1
        /// The direct dependents of A1 are B1 and C1
        /// </summary>
        protected override IEnumerable<string> GetDirectDependents(string name)
        {
            name = Normalize(name);

            if (name == null || !(isValidName(name)))
            {
                throw new InvalidNameException();
            }

            return dependencyGraph.GetDependents(name);
        }

        /// <summary>
        /// This holds the information of the cell
        /// </summary>
        private class Cell
        {

            /// <summary>
            /// This is the content of the cell
            /// </summary>
            public Object content;

            /// <summary>
            /// This hold the value cell
            /// </summary>
            public Object value;

            /// <summary>
            /// This creates a Cell, empty cell has an empty String
            /// </summary>
            public Cell()
            {
                this.content = "";
                this.value = "";
            }
        }

        /// <summary>
        /// This Method checks if the name that was given was a valid Name
        /// A string is a valid cell name if and only if:
        ///   (1) its first character is an underscore or a letter
        ///   (2) its remaining characters (if any) are underscores and/or letters and/or digits
        /// Note that this is the same as the definition of valid variable from the PS3 Formula class.
        /// Does not check if it is null
        /// </summary>
        /// <param name="name">the name we are test if valid</param>
        /// <returns>true if valid, false, otherwise</returns>
        private bool isValidName(String name)
        {
            int nameLength = name.Length;

            if (nameLength == 0)
            {
                return false;
            }
            else if (nameLength == 1)
            {
                if (Regex.IsMatch(name, "^[a-zA-Z]$") || Regex.IsMatch(name, "^[_]$"))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if (Regex.IsMatch(name, "^[a-zA-Z|_][a-zA-Z0-9|_]+$"))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

        }

        // ADDED FOR PS5
        /// <summary>
        /// Returns the version information of the spreadsheet saved in the named file.
        /// If there are any problems opening, reading, or closing the file, the method
        /// should throw a SpreadsheetReadWriteException with an explanatory message.
        /// </summary>
        public override string GetSavedVersion(string filename)
        {
            try
            {
                using (XmlReader reader = XmlReader.Create(filename))
                {
                    reader.Read();
                    reader.Read();
                    reader.Read();
                    if (!(reader.Name == "spreadsheet") || !(reader.HasAttributes))
                    {
                        throw new FormatException("problem with the spreadsheet starting element");
                    }
                    String attribute = reader.GetAttribute("version");
                    if (attribute != null)
                    {
                        return attribute;
                    }
                    else
                    {
                        throw new FormatException("problem with the spreadsheet version not found");
                    }
                }
            }
            catch (Exception e)
            {
                throw new SpreadsheetReadWriteException("Problem getting your version or " + e.Message);
            }
        }

        // ADDED FOR PS5
        /// <summary>
        /// Writes the contents of this spreadsheet to the named file using an XML format.
        /// The XML elements should be structured as follows:
        /// 
        /// <spreadsheet version="version information goes here">
        /// 
        /// <cell>
        /// <name>
        /// cell name goes here
        /// </name>
        /// <contents>
        /// cell contents goes here
        /// </contents>    
        /// </cell>
        /// 
        /// </spreadsheet>
        /// 
        /// There should be one cell element for each non-empty cell in the spreadsheet.  
        /// If the cell contains a string, it should be written as the contents.  
        /// If the cell contains a double d, d.ToString() should be written as the contents.  
        /// If the cell contains a Formula f, f.ToString() with "=" prepended should be written as the contents.
        /// 
        /// If there are any problems opening, writing, or closing the file, the method should throw a
        /// SpreadsheetReadWriteException with an explanatory message.
        /// </summary>
        public override void Save(string filename)
        {
            this.PathToFile = filename;
            XmlWriterSettings setting = new XmlWriterSettings();
            setting.Indent = true;
            setting.IndentChars = "  ";

            try
            {
                using (XmlWriter xWriter = XmlWriter.Create(filename, setting))
                {
                    xWriter.WriteStartElement("spreadsheet");
                    xWriter.WriteStartAttribute("version");
                    xWriter.WriteString(Version);

                    foreach (String el in nonEmptyCellCollection)
                    {
                        xWriter.WriteStartElement("cell");

                        xWriter.WriteStartElement("name");
                        xWriter.WriteString(el);
                        xWriter.WriteEndElement();

                        xWriter.WriteStartElement("contents");

                        Object content = cells[el].content;
                        if (content is String)
                        {
                            xWriter.WriteString((String)content);
                        }
                        else if (content is double)
                        {
                            xWriter.WriteString(((double)content).ToString());
                        }
                        else
                        {
                            xWriter.WriteString(("=" + (Formula)content).ToString());
                        }

                        xWriter.WriteEndElement();

                        xWriter.WriteEndElement();
                    }

                    xWriter.WriteEndElement();
                    xWriter.Close();
                    Changed = false;
                }
            }
            catch (Exception e)
            {
                throw new SpreadsheetReadWriteException("Was unable to write the xlm file");
            }
        }

        /// <summary>
        /// This methods gets the value of cell
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        public override object GetCellValue(string name)
        {
            name = Normalize(name);

            if (name == null || !(isValidName(name)))
            {
                throw new InvalidNameException();
            }

            if (cells.ContainsKey(name))
            {
                return cells[name].value;
            }
            else
            {
                return "";
            }
        }

        /// <summary>
        /// This is method is the look up delegate required to look
        /// up the value of the variables
        /// </summary>
        /// <param name="variable"></param>
        /// <returns></returns>
        private double lookUp(String variable)
        {
            if (!(GetCellValue(variable) is double) || !cells.ContainsKey(variable))
                throw new ArgumentException("Unknown Variable");
            return (double)cells[variable].value;
        }

        // ADDED FOR PS5
        /// <summary>
        /// If content is null, throws an ArgumentNullException.
        /// 
        /// Otherwise, if name is null or invalid, throws an InvalidNameException.
        /// 
        /// Otherwise, if content parses as a double, the contents of the named
        /// cell becomes that double.
        /// 
        /// Otherwise, if content begins with the character '=', an attempt is made
        /// to parse the remainder of content into a Formula f using the Formula
        /// constructor.  There are then three possibilities:
        /// 
        ///   (1) If the remainder of content cannot be parsed into a Formula, a 
        ///       SpreadsheetUtilities.FormulaFormatException is thrown.
        ///       
        ///   (2) Otherwise, if changing the contents of the named cell to be f
        ///       would cause a circular dependency, a CircularException is thrown.
        ///       
        ///   (3) Otherwise, the contents of the named cell becomes f.
        /// 
        /// ADDED FOR PS6
        /// If the content begins with =sum the CreateSum method is called where it creates
        /// a formula of a ranged sum. e.g. A1-A10 or A10-A1 or A1-B2 or B2-A1.
        /// 
        /// Otherwise, the contents of the named cell becomes content.
        /// 
        /// If an exception is not thrown, the method returns a set consisting of
        /// name plus the names of all other cells whose value depends, directly
        /// or indirectly, on the named cell.
        /// 
        /// For example, if name is A1, B1 contains A1*2, and C1 contains B1+A1, the
        /// set {A1, B1, C1} is returned.
        /// </summary>
        public override ISet<string> SetContentsOfCell(string name, string content)
        {
            if (content is null)
            {
                throw new ArgumentNullException();
            }
            name = Normalize(name);
            double parsedValue = 0;

            if (double.TryParse(content, out parsedValue))
            {
                return SetCellContents(name, parsedValue);
            }
            else if (content == "")
            {
                return SetCellContents(name, content);
            }
            else if (content[0] == '=')
            {
                try
                {
                    switch (content.Substring(1, 3))
                    {
                        case "sum":
                            Formula sum = CreateSumFormula(content.Remove(0, 4));
                            return SetCellContents(name, sum);
                        case "Sum":
                            Formula sum1 = CreateSumFormula(content.Remove(0, 4));
                            return SetCellContents(name, sum1);
                        default:
                            String content1 = content.Remove(0, 1);
                            Formula f = new Formula(content1, Normalize, IsValid);
                            return SetCellContents(name, f);
                    }
                }
                catch (ArgumentOutOfRangeException)
                {
                    String content1 = content.Remove(0, 1);
                    Formula f = new Formula(content1, Normalize, IsValid);
                    return SetCellContents(name, f);
                }

            }
            else
            {
                return SetCellContents(name, content);
            }
        }

        /// <summary>
        /// Creates a formula that is the sum of a range of variables given in the contents in the form
        /// (A1-A10). 
        /// A multi letter range will compute the sum of all the cells is the box with the first term being in one
        /// corner and the second term being in the opposite corner.
        /// </summary>
        /// <param name="content"></param>
        /// <returns></returns>
        private Formula CreateSumFormula(string content)
        {
            StringBuilder formulaBuilder = new StringBuilder();
            string[] splitRange = content.Split('-');
            formulaBuilder.Append('(');

            string[] firstCellSplit = seperateElements(splitRange[0].Remove(0, 1));
            string[] lastCellSplit = seperateElements(splitRange[1].Remove(splitRange[1].Length - 1));

            char firstLetter = char.Parse(Normalize(firstCellSplit[0]));
            char lastLetter = char.Parse(Normalize(lastCellSplit[0]));
            int firstNumber = int.Parse(firstCellSplit[1]);
            int lastNumber = int.Parse(lastCellSplit[1]);

            if(firstNumber > lastNumber && firstLetter >= lastLetter)
            {
                SwapNumbers(ref firstNumber, ref lastNumber);
                SwapChars(ref firstLetter, ref lastLetter);
            }
            if(firstLetter > lastLetter && firstNumber <= lastNumber)
            {
                SwapChars(ref firstLetter, ref lastLetter);
            }

            if (firstLetter <= lastLetter && firstNumber > lastNumber)
            {
                SwapNumbers(ref firstNumber, ref lastNumber);
            }

            for (char letter = firstLetter; letter <= lastLetter; letter++)
            {
                for (int number = firstNumber; number <= lastNumber; number++)
                {
                    formulaBuilder.Append(letter).Append(number).Append('+');
                }
            }

            formulaBuilder.Remove(formulaBuilder.Length - 1, 1);
            formulaBuilder.Append(')');
            return new Formula(formulaBuilder.ToString(),Normalize,IsValid);
        }

        /// <summary>
        /// Helper to swap values of two characters. 
        /// </summary>
        /// <param name="firstChar"></param>
        /// <param name="lastChar"></param>
        private static void SwapChars(ref char firstChar, ref char lastChar)
        {
            char tempChar = lastChar;
            lastChar = firstChar;
            firstChar = tempChar;
        }

        /// <summary>
        /// Helper to swap values of two integer numbers.
        /// </summary>
        /// <param name="firstNumber"></param>
        /// <param name="lastNumber"></param>
        private static void SwapNumbers(ref int firstNumber, ref int lastNumber)
        {
            int tempNum = lastNumber;
            lastNumber = firstNumber;
            firstNumber = tempNum;
        }

        /// <summary>
        /// This method takes a string and seperates it into a
        /// string array of size two, first elements is the letters
        /// ad the second element is the numbers of the string.
        /// </summary>
        /// <param name="name">a variable of leters or more
        /// follows by a number or more</param>
        /// <returns>string array with a letter and a number</returns>
        private String[] seperateElements(String name)
        {
            String letters = "";
            String numbers = "";
            String[] elements = new String[2];

            for (int i = 1; i < name.Length; i++)
            {
                char ch = name[i - 1];
                if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
                {
                    letters = name.Substring(0, i);
                    numbers = name.Replace(letters, "");
                }
            }

            elements[0] = letters;
            elements[1] = numbers;
            return elements;
        }
    }
}

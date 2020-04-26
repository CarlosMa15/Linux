// Carlos Martinez
// September 22, 2017
// Assignment PS4

using SpreadsheetUtilities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace SS
{
    public class Spreadsheet : AbstractSpreadsheet
    {

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

        /// <summary>
        /// A non arguement Constructor that Constructs an empty constructor
        /// </summary>
        public Spreadsheet()
        {
            this.cells = new Dictionary<string, Cell>();
            this.dependencyGraph = new DependencyGraph();
            this.nonEmptyCellCollection = new HashSet<string>();

        }

        /// <summary>
        /// If name is null or invalid, throws an InvalidNameException.
        /// 
        /// Otherwise, returns the contents (as opposed to the value) of the named cell.  The return
        /// value should be either a string, a double, or a Formula.
        public override object GetCellContents(string name)
        {
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
        public override ISet<string> SetCellContents(string name, double number)
        {
            if (name == null || !(isValidName(name)))
            {
                throw new InvalidNameException();
            }

            if(cells.ContainsKey(name))
            {
                setCellToDouble(name, number);
                this.nonEmptyCellCollection.Add(name);
            } else
            {
                cells.Add(name, new Cell());
                setCellToDouble(name, number);
                this.nonEmptyCellCollection.Add(name);
            }

            return new HashSet<String>(GetCellsToRecalculate(name));
        }

        /// <summary>
        /// This method set the value of the cell to what ever number is given
        /// </summary>
        /// <param name="name">name of cell</param>
        /// <param name="number">number(content and value)</param>
        private void setCellToDouble(String name, double number)
        {
            cells[name].content = number;
            //cells[name].value = number;
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
        public override ISet<string> SetCellContents(string name, string text)
        {
            if (name == null || !(isValidName(name)))
            {
                throw new InvalidNameException();
            }

            if(cells.ContainsKey(name))
            {
                if (text == "")
                {
                    emptyingCell(name, text);
                } else
                {
                    setCellToNonEmptySring(name, text);
                }
            } else
            {
                if(text != "")
                {
                    cells.Add(name, new Cell());
                    setCellToNonEmptySring(name, text);

                }
            }

            return new HashSet<String>(GetCellsToRecalculate(name));
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
        public void setCellToString(String name,String text)
        {
            cells[name].content = text;
            //cells[name].value = text;
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
        public override ISet<string> SetCellContents(string name, Formula formula)
        {
            if (name == null || !(isValidName(name)))
            {
                throw new InvalidNameException();
            }

            if(cells.ContainsKey(name))
            {
                setCellsToFormual(name, formula);
            } else
            {
                cells.Add(name, new Cell());
                setCellsToFormual(name, formula);
            }

            return new HashSet<String>(GetCellsToRecalculate(name));
        }

        private void setCellsToFormual(String name, Formula formula)
        {
            cells[name].content = formula;
            //cells[name].value = formula.Evaluate(lookUp);
            foreach (String el in formula.GetVariables()) {
                dependencyGraph.AddDependency(el,name);
            }
        }

        //private double lookUp(String variable)
        //{
            //return (double)cells[variable].value;
        //}

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
            if (name == null || !(isValidName(name)))
            {
                throw new InvalidNameException();
            }

            return dependencyGraph.GetDependents(name);
        }

        /// <summary>
        /// This holds the information of the cell
        /// </summary>
        private class Cell {

            /// <summary>
            /// This is the content of the cell
            /// </summary>
            public Object content;

            /// <summary>
            /// this is the value of the cell
            /// </summary>
            //public Object value;

            /// <summary>
            /// This creates a Cell, empty cell has an empty String
            /// </summary>
            public Cell()
            {
                this.content = "";
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
            } else if (nameLength == 1)
            {
                if (Regex.IsMatch(name,"^[a-zA-Z]$") || Regex.IsMatch(name,"^[_]$"))
                {
                    return true;
                } else
                {
                    return false;
                }
            } else if (nameLength > 1)
            {
                if (Regex.IsMatch(name, "^[a-zA-Z|_][a-zA-Z0-9|_]+$"))
                {
                    return true;
                } else
                {
                    return false;
                }
            } else
            {
                return false;
            }
        }

    }
}

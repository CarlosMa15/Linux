//SP5

using SpreadsheetUtilities;
using SS;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Xml;

namespace PS4TesterWithMain
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine((String)null is null);

            String filePath = "C:\\Users\\Carlos Martinez\\Source\\Repos\\cs3500\\PS4\\Spreadsheet\\Spreadsheet.xml";
            bool nameChecker = false;
            String name = "";
            bool contentChecker = false;
            String content = "";
            using (XmlReader reader = XmlReader.Create(filePath))
            {
                while (reader.Read())
                {
                    if (reader.IsStartElement())
                    {
                        if (reader.HasAttributes)
                        {
                            Console.WriteLine(reader.GetAttribute("version"));
                        }

                        Console.WriteLine("Starting Element : " + reader.Name);
                        switch (reader.Name)
                        {
                            case "name":
                                if (nameChecker)
                                {
                                    throw new FormatException("has multiple names in one cell");
                                }
                                reader.Read();
                                name = reader.Value;
                                Console.WriteLine("Name: " + reader.Value);
                                nameChecker = true;
                                break;

                            case "contents":
                                if (contentChecker)
                                {
                                    throw new FormatException("has multiple names in one cell");
                                }
                                reader.Read();
                                content = reader.Value;
                                Console.WriteLine("Value: " + reader.Value);
                                contentChecker = true;
                                break;
                        }

                        if (nameChecker && contentChecker)
                        {
                            Console.WriteLine("Cell(" + name + "," + content + ")");
                            nameChecker = false;
                            contentChecker = false;
                        }
                    }
                }
            }

            Console.Read();
        }




        private static bool isValid(String name)
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

        private static String Normalize(String variable)
        {
            return variable.ToUpper();
        }

    }
}

using SpreadsheetUtilities;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;

namespace PS2GradingTests
{
    /// <summary>
    ///  This is a test class for DependencyGraphTest
    /// 
    ///  These tests should help guide you on your implementation.  Warning: you can not "test" yourself
    ///  into correctness.  Tests only show incorrectness.  That being said, a large test suite will go a long
    ///  way toward ensuring correctness.
    /// 
    ///  You are strongly encouraged to write additional tests as you think about the required
    ///  functionality of yoru library.
    /// 
    ///</summary>
    [TestClass()]
    public class DependencyGraphTest
    {
        // ************************** TESTS ON EMPTY DGs ************************* //
        /// <summary>
        /// test Replace Dependents with copies
        /// </summary>
        [TestMethod()]
        public void ReplaceDependentsWithNothing()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("A", "1");
            t.AddDependency("A", "2");
            t.AddDependency("A", "3");
            t.AddDependency("A", "4");
            t.AddDependency("A", "5");
            t.AddDependency("A", "6");
            t.AddDependency("A", "7");

            List<String> newDendents = new List<String>();
            

            t.ReplaceDependents("A", newDendents);

            Assert.AreEqual(0, t.Size);
        }

        /// <summary>
        /// test Replace Dependents with nothing
        /// </summary>
        [TestMethod()]
        public void ReplaceDependeesWithNothing()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("1", "A");
            t.AddDependency("2", "A");
            t.AddDependency("3", "A");
            t.AddDependency("4", "A");
            t.AddDependency("5", "A");
            t.AddDependency("6", "A");
            t.AddDependency("7", "A");

            List<String> newDendendees = new List<String>();

            t.ReplaceDependees("A", newDendendees);

            Assert.AreEqual(0, t.Size);
        }

        /// <summary>
        /// test Replace Dependents with copies
        /// </summary>
        [TestMethod()]
        public void ReplaceDependeesWithCopies()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("1", "A");
            t.AddDependency("2", "A");
            t.AddDependency("3", "A");
            t.AddDependency("4", "A");
            t.AddDependency("5", "A");
            t.AddDependency("6", "A");
            t.AddDependency("7", "A");

            List<String> newDendendees = new List<String>();
            newDendendees.Add("11");
            newDendendees.Add("12");
            newDendendees.Add("11");
            newDendendees.Add("13");
            newDendendees.Add("11");
            newDendendees.Add("14");
            newDendendees.Add("11");
            newDendendees.Add("15");
            newDendendees.Add("11");

            t.ReplaceDependees("A", newDendendees);

            Assert.AreEqual(5, t.Size);
        }


        /// <summary>
        /// test Replace Dependents with copies
        /// </summary>
        [TestMethod()]
        public void ReplaceDependentsWithCopies()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("A", "1");
            t.AddDependency("A", "2");
            t.AddDependency("A", "3");
            t.AddDependency("A", "4");
            t.AddDependency("A", "5");
            t.AddDependency("A", "6");
            t.AddDependency("A", "7");

            List<String> newDendents = new List<String>();
            newDendents.Add("11");
            newDendents.Add("12");
            newDendents.Add("11");
            newDendents.Add("13");
            newDendents.Add("11");
            newDendents.Add("14");
            newDendents.Add("11");
            newDendents.Add("15");
            newDendents.Add("11");

            t.ReplaceDependents("A", newDendents);

            Assert.AreEqual(5, t.Size);
        }

        /// <summary>
        /// test Replace Dependents
        /// </summary>
        [TestMethod()]
        public void ReplaceDependentsSizeNine()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("A", "1");
            t.AddDependency("A", "2");
            t.AddDependency("A", "3");
            t.AddDependency("A", "4");
            t.AddDependency("A", "5");
            t.AddDependency("A", "6");
            t.AddDependency("A", "7");

            List<String> newDendents = new List<String>();
            newDendents.Add("b");
            newDendents.Add("a");
            newDendents.Add("c");
            newDendents.Add("d");
            newDendents.Add("e");
            newDendents.Add("f");
            newDendents.Add("g");
            newDendents.Add("h");
            newDendents.Add("i");

            t.ReplaceDependents("A", newDendents);

            Assert.AreEqual(9, t.Size);
        }

        /// <summary>
        /// Testing has dependees
        /// </summary>
        [TestMethod()]
        public void HasDependees()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("1","A");
            t.AddDependency("2","A");
            t.AddDependency("3","A");
            t.AddDependency("4","A");
            t.AddDependency("5","A");
            t.AddDependency("6","A");
            t.AddDependency("7","A");

            Assert.IsTrue(t.HasDependees("A"));
        }

        /// <summary>
        /// Testing has dependents
        /// </summary>
        [TestMethod()]
        public void HasDependents()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("A", "1");
            t.AddDependency("A", "2");
            t.AddDependency("A", "3");
            t.AddDependency("A", "4");
            t.AddDependency("A", "5");
            t.AddDependency("A", "6");
            t.AddDependency("A", "7");

            Assert.IsTrue(t.HasDependents("A"));
        }

        /// <summary>
        /// Testing removing nonexisting elements
        /// </summary>
        [TestMethod()]
        public void RemovingNonexistingElements()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("A", "1");
            t.AddDependency("A", "2");
            t.AddDependency("A", "3");
            t.AddDependency("A", "4");
            t.AddDependency("A", "5");
            t.AddDependency("A", "6");
            t.AddDependency("A", "7");

            t.RemoveDependency("A", "8");
            t.RemoveDependency("A", "9");
            t.RemoveDependency("A", "10");
            t.RemoveDependency("A", "11");
            t.RemoveDependency("A", "12");
            t.RemoveDependency("A", "13");
            t.RemoveDependency("A", "14");

            Assert.AreEqual(7, t.Size);
        }

        /// <summary>
        /// Testing that it is zero after removing more elements than contained
        /// </summary>
        [TestMethod()]
        public void RemoveMoreContaining()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("A", "1");
            t.AddDependency("A", "2");
            t.AddDependency("A", "3");
            t.AddDependency("A", "4");
            t.AddDependency("A", "5");
            t.AddDependency("A", "6");
            t.AddDependency("A", "7");

            t.RemoveDependency("A", "1");
            t.RemoveDependency("A", "2");
            t.RemoveDependency("A", "3");
            t.RemoveDependency("A", "4");
            t.RemoveDependency("A", "5");
            t.RemoveDependency("A", "6");
            t.RemoveDependency("A", "7");

            t.RemoveDependency("A", "1");
            t.RemoveDependency("A", "2");
            t.RemoveDependency("A", "3");
            t.RemoveDependency("A", "4");
            t.RemoveDependency("A", "5");
            t.RemoveDependency("A", "6");
            t.RemoveDependency("A", "7");

            Assert.AreEqual(0, t.Size);
        }

        /// <summary>
        /// Testing that their is only seven Elements
        /// </summary>
        [TestMethod()]
        public void AddingSevenElements()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("A", "1");
            t.AddDependency("A", "2");
            t.AddDependency("A", "3");
            t.AddDependency("A", "4");
            t.AddDependency("A", "5");
            t.AddDependency("A", "6");
            t.AddDependency("A", "7");

            Assert.AreEqual(7, t.Size);
        }

        /// <summary>
        /// Tests that their is only one element in the DG
        /// </summary>
        [TestMethod()]
        public void AddingOneElement()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("A", "B");
            Assert.AreEqual(1, t.Size);
        }

        /// <summary>
        ///Empty graph should contain nothing
        ///</summary>
        [TestMethod()]

        public void ZeroSize()
        {
            DependencyGraph t = new DependencyGraph();
            Assert.AreEqual(0, t.Size);
        }

        /// <summary>
        ///Empty graph should contain nothing
        ///</summary>
        [TestMethod()]
        public void HasNoDependees()
        {
            DependencyGraph t = new DependencyGraph();
            Assert.IsFalse(t.HasDependees("a"));
        }

        /// <summary>
        ///Empty graph should contain nothing
        ///</summary>
        [TestMethod()]
        public void HasNoDependents()
        {
            DependencyGraph t = new DependencyGraph();
            Assert.IsFalse(t.HasDependents("a"));
        }

        /// <summary>
        ///Empty graph should contain nothing
        ///</summary>
        [TestMethod()]
        public void EmptyDependees()
        {
            DependencyGraph t = new DependencyGraph();
            Assert.IsFalse(t.GetDependees("a").GetEnumerator().MoveNext());
        }

        /// <summary>
        ///Empty graph should contain nothing
        ///</summary>
        [TestMethod()]
        public void EmptyDependents()
        {
            DependencyGraph t = new DependencyGraph();
            Assert.IsFalse(t.GetDependents("a").GetEnumerator().MoveNext());
        }

        /// <summary>
        ///Empty graph should contain nothing
        ///</summary>
        [TestMethod()]
        public void EmptyIndexer()
        {
            DependencyGraph t = new DependencyGraph();
            Assert.AreEqual(0, t["a"]);
        }

        /// <summary>
        ///Indexer not empty
        ///</summary>
        [TestMethod()]
        public void NonEmptyIndexer()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("A", "B");
            Assert.AreEqual(1, t["B"]);
        }

        /// <summary>
        ///Indexer not empty with copies
        ///</summary>
        [TestMethod()]
        public void NonEmptyIndexerWithCopies()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("A", "B");
            t.AddDependency("A", "B");
            t.AddDependency("A", "B");
            Assert.AreEqual(1, t["B"]);
        }

        /// <summary>
        ///Indexer not empty with copies
        ///</summary>
        [TestMethod()]
        public void IndexerWithManyDependees()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("A", "B");
            t.AddDependency("C", "B");
            t.AddDependency("D", "B");
            Assert.AreEqual(3, t["B"]);
        }

        /// <summary>
        ///Removing from an empty DG shouldn't fail
        ///</summary>
        [TestMethod()]
        public void RemoveFromEmpty()
        {
            DependencyGraph t = new DependencyGraph();
            t.RemoveDependency("a", "b");
            Assert.AreEqual(0, t.Size);
        }

        /// <summary>
        ///Adding to an empty DG shouldn't fail
        ///</summary>
        [TestMethod()]
        public void AddToEmpty()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("a", "b");
        }

        /// <summary>
        ///Replace on an empty DG shouldn't fail
        ///</summary>
        [TestMethod()]
        public void ReplaceEmptyDependents()
        {
            DependencyGraph t = new DependencyGraph();
            t.ReplaceDependents("a", new HashSet<string>());
            Assert.AreEqual(0, t.Size);
        }

        /// <summary>
        ///Replace on an empty DG shouldn't fail
        ///</summary>
        [TestMethod()]
        public void ReplaceEmptyDependees()
        {
            DependencyGraph t = new DependencyGraph();
            t.ReplaceDependees("a", new HashSet<string>());
            Assert.AreEqual(0, t.Size);
        }


        /**************************** SIMPLE NON-EMPTY TESTS ****************************/

        /// <summary>
        ///Non-empty graph contains something
        ///</summary>
        [TestMethod()]
        public void NonEmptySize()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("a", "b");
            t.AddDependency("a", "c");
            Assert.AreEqual(2, t.Size);
        }

        /// <summary>
        ///Slight variant
        ///</summary>
        [TestMethod()]
        public void AddDuplicate()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("a", "b");
            t.AddDependency("a", "b");
            Assert.AreEqual(1, t.Size);
        }

        /// <summary>
        ///Nonempty graph should contain something
        ///</summary>
        [TestMethod()]
        public void NonEmptyTest3()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("a", "b");
            t.AddDependency("a", "c");
            t.AddDependency("d", "c");
            Assert.IsFalse(t.HasDependees("a"));
            Assert.IsTrue(t.HasDependees("b"));
            Assert.IsTrue(t.HasDependents("a"));
            Assert.IsTrue(t.HasDependees("c"));
        }

        /// <summary>
        ///Nonempty graph should contain something
        ///</summary>
        [TestMethod()]
        public void ComplexGraphCount()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("a", "b");
            t.AddDependency("a", "c");
            t.AddDependency("d", "c");
            HashSet<String> aDents = new HashSet<String>(t.GetDependents("a"));
            HashSet<String> bDents = new HashSet<String>(t.GetDependents("b"));
            HashSet<String> cDents = new HashSet<String>(t.GetDependents("c"));
            HashSet<String> dDents = new HashSet<String>(t.GetDependents("d"));
            HashSet<String> eDents = new HashSet<String>(t.GetDependents("e"));
            HashSet<String> aDees = new HashSet<String>(t.GetDependees("a"));
            HashSet<String> bDees = new HashSet<String>(t.GetDependees("b"));
            HashSet<String> cDees = new HashSet<String>(t.GetDependees("c"));
            HashSet<String> dDees = new HashSet<String>(t.GetDependees("d"));
            HashSet<String> eDees = new HashSet<String>(t.GetDependees("e"));
            Assert.IsTrue(aDents.Count == 2 && aDents.Contains("b") && aDents.Contains("c"));
            Assert.IsTrue(bDents.Count == 0);
            Assert.IsTrue(cDents.Count == 0);
            Assert.IsTrue(dDents.Count == 1 && dDents.Contains("c"));
            Assert.IsTrue(eDents.Count == 0);
            Assert.IsTrue(aDees.Count == 0);
            Assert.IsTrue(bDees.Count == 1 && bDees.Contains("a"));
            Assert.IsTrue(cDees.Count == 2 && cDees.Contains("a") && cDees.Contains("d"));
            Assert.IsTrue(dDees.Count == 0);
            Assert.IsTrue(dDees.Count == 0);
        }

        /// <summary>
        ///Nonempty graph should contain something
        ///</summary>
        [TestMethod()]
        public void ComplexGraphIndexer()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("a", "b");
            t.AddDependency("a", "c");
            t.AddDependency("d", "c");
            Assert.AreEqual(0, t["a"]);
            Assert.AreEqual(1, t["b"]);
            Assert.AreEqual(2, t["c"]);
            Assert.AreEqual(0, t["d"]);
            Assert.AreEqual(0, t["e"]);
        }

        /// <summary>
        ///Removing from a DG 
        ///</summary>
        [TestMethod()]
        public void Remove()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("a", "b");
            t.AddDependency("a", "c");
            t.AddDependency("d", "c");
            t.RemoveDependency("a", "b");
            Assert.AreEqual(2, t.Size);
        }

        /// <summary>
        ///Replace on a DG
        ///</summary>
        [TestMethod()]
        public void ReplaceDependents()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("a", "b");
            t.AddDependency("a", "c");
            t.AddDependency("d", "c");
            t.ReplaceDependents("a", new HashSet<string>() { "x", "y", "z" });
            HashSet<String> aPends = new HashSet<string>(t.GetDependents("a"));
            Assert.IsTrue(aPends.SetEquals(new HashSet<string>() { "x", "y", "z" }));
        }

        /// <summary>
        ///Replace on a DG
        ///</summary>
        [TestMethod()]
        public void ReplaceDependees()
        {
            DependencyGraph t = new DependencyGraph();
            t.AddDependency("a", "b");
            t.AddDependency("a", "c");
            t.AddDependency("d", "c");
            t.ReplaceDependees("c", new HashSet<string>() { "x", "y", "z" });
            HashSet<String> cDees = new HashSet<string>(t.GetDependees("c"));
            Assert.IsTrue(cDees.SetEquals(new HashSet<string>() { "x", "y", "z" }));
        }

        // ************************** STRESS TESTS ******************************** //
        /// <summary>
        ///Using lots of data
        ///</summary>
        [TestMethod()]
        public void StressTest1()
        {
            // Dependency graph
            DependencyGraph t = new DependencyGraph();

            // A bunch of strings to use
            const int SIZE = 100;
            string[] letters = new string[SIZE];
            for (int i = 0; i < SIZE; i++)
            {
                letters[i] = ("" + (char)('a' + i));
            }

            // The correct answers
            HashSet<string>[] dents = new HashSet<string>[SIZE];
            HashSet<string>[] dees = new HashSet<string>[SIZE];
            for (int i = 0; i < SIZE; i++)
            {
                dents[i] = new HashSet<string>();
                dees[i] = new HashSet<string>();
            }

            // Add a bunch of dependencies
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = i + 1; j < SIZE; j++)
                {
                    t.AddDependency(letters[i], letters[j]);
                    dents[i].Add(letters[j]);
                    dees[j].Add(letters[i]);
                }
            }

            // Remove a bunch of dependencies
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = i + 2; j < SIZE; j += 2)
                {
                    t.RemoveDependency(letters[i], letters[j]);
                    dents[i].Remove(letters[j]);
                    dees[j].Remove(letters[i]);
                }
            }

            // Make sure everything is right
            for (int i = 0; i < SIZE; i++)
            {
                Assert.IsTrue(dents[i].SetEquals(new HashSet<string>(t.GetDependents(letters[i]))));
                Assert.IsTrue(dees[i].SetEquals(new HashSet<string>(t.GetDependees(letters[i]))));
            }
        }



        // ********************************** ANOTHER STESS TEST ******************** //
        /// <summary>
        ///Using lots of data with replacement
        ///</summary>
        [TestMethod()]
        public void StressTest8()
        {
            // Dependency graph
            DependencyGraph t = new DependencyGraph();

            // A bunch of strings to use
            const int SIZE = 100;
            string[] letters = new string[SIZE];
            for (int i = 0; i < SIZE; i++)
            {
                letters[i] = ("" + (char)('a' + i));
            }

            // The correct answers
            HashSet<string>[] dents = new HashSet<string>[SIZE];
            HashSet<string>[] dees = new HashSet<string>[SIZE];
            for (int i = 0; i < SIZE; i++)
            {
                dents[i] = new HashSet<string>();
                dees[i] = new HashSet<string>();
            }

            // Add a bunch of dependencies
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = i + 1; j < SIZE; j++)
                {
                    t.AddDependency(letters[i], letters[j]);
                    dents[i].Add(letters[j]);
                    dees[j].Add(letters[i]);
                }
            }

            // Remove a bunch of dependencies
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = i + 2; j < SIZE; j += 2)
                {
                    t.RemoveDependency(letters[i], letters[j]);
                    dents[i].Remove(letters[j]);
                    dees[j].Remove(letters[i]);
                }
            }

            // Replace a bunch of dependents
            for (int i = 0; i < SIZE; i += 4)
            {
                HashSet<string> newDents = new HashSet<String>();
                for (int j = 0; j < SIZE; j += 7)
                {
                    newDents.Add(letters[j]);
                }
                t.ReplaceDependents(letters[i], newDents);

                foreach (string s in dents[i])
                {
                    dees[s[0] - 'a'].Remove(letters[i]);
                }

                foreach (string s in newDents)
                {
                    dees[s[0] - 'a'].Add(letters[i]);
                }

                dents[i] = newDents;
            }

            // Make sure everything is right
            for (int i = 0; i < SIZE; i++)
            {
                Assert.IsTrue(dents[i].SetEquals(new HashSet<string>(t.GetDependents(letters[i]))));
                Assert.IsTrue(dees[i].SetEquals(new HashSet<string>(t.GetDependees(letters[i]))));
            }
        }

        // ********************************** A THIRD STESS TEST ******************** //
        /// <summary>
        ///Using lots of data with replacement
        ///</summary>
        [TestMethod()]
        public void StressTest15()
        {
            // Dependency graph
            DependencyGraph t = new DependencyGraph();

            // A bunch of strings to use
            const int SIZE = 100;
            string[] letters = new string[SIZE];
            for (int i = 0; i < SIZE; i++)
            {
                letters[i] = ("" + (char)('a' + i));
            }

            // The correct answers
            HashSet<string>[] dents = new HashSet<string>[SIZE];
            HashSet<string>[] dees = new HashSet<string>[SIZE];
            for (int i = 0; i < SIZE; i++)
            {
                dents[i] = new HashSet<string>();
                dees[i] = new HashSet<string>();
            }

            // Add a bunch of dependencies
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = i + 1; j < SIZE; j++)
                {
                    t.AddDependency(letters[i], letters[j]);
                    dents[i].Add(letters[j]);
                    dees[j].Add(letters[i]);
                }
            }

            // Remove a bunch of dependencies
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = i + 2; j < SIZE; j += 2)
                {
                    t.RemoveDependency(letters[i], letters[j]);
                    dents[i].Remove(letters[j]);
                    dees[j].Remove(letters[i]);
                }
            }

            // Replace a bunch of dependees
            for (int i = 0; i < SIZE; i += 4)
            {
                HashSet<string> newDees = new HashSet<String>();
                for (int j = 0; j < SIZE; j += 7)
                {
                    newDees.Add(letters[j]);
                }
                t.ReplaceDependees(letters[i], newDees);

                foreach (string s in dees[i])
                {
                    dents[s[0] - 'a'].Remove(letters[i]);
                }

                foreach (string s in newDees)
                {
                    dents[s[0] - 'a'].Add(letters[i]);
                }

                dees[i] = newDees;
            }

            // Make sure everything is right
            for (int i = 0; i < SIZE; i++)
            {
                Assert.IsTrue(dents[i].SetEquals(new HashSet<string>(t.GetDependents(letters[i]))));
                Assert.IsTrue(dees[i].SetEquals(new HashSet<string>(t.GetDependees(letters[i]))));
            }
        }

        [TestMethod]
        public void TestReplaceDependentsWithTokenNotCreatedYet()
        {
            DependencyGraph t = new DependencyGraph();
            List<String> tokens = new List<String>() {"B","C","D","E","f"};
            t.ReplaceDependents("A", tokens);
            Assert.AreEqual(5, t.Size);
        }

        [TestMethod]
        public void TestReplaceDependeesWithTokenNotCreatedYet()
        {
            DependencyGraph t = new DependencyGraph();
            List<String> tokens = new List<String>() { "B", "C", "D", "E", "f" };
            t.ReplaceDependees("A", tokens);
            Assert.AreEqual(5, t.Size);
        }
    }
}

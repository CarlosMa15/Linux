//Carlos Martinez 
// September 15, 2017
// Skeleton implementation written by Joe Zachary for CS 3500, September 2013.
// Version 1.1 (Fixed error in comment for RemoveDependency.)

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SpreadsheetUtilities

{
    /// <summary>
    /// (s1,t1) is an ordered pair of strings
    /// t1 depends on s1; s1 must be evaluated before t1
    /// 
    /// A DependencyGraph can be modeled as a set of ordered pairs of strings.  Two ordered pairs
    /// (s1,t1) and (s2,t2) are considered equal if and only if s1 equals s2 and t1 equals t2.
    /// Recall that sets never contain duplicates.  If an attempt is made to add an element to a 
    /// set, and the element is already in the set, the set remains unchanged.
    /// 
    /// Given a DependencyGraph DG:
    /// 
    ///    (1) If s is a string, the set of all strings t such that (s,t) is in DG is called dependents(s).
    ///        (The set of things that depend on s)    
    ///        
    ///    (2) If s is a string, the set of all strings t such that (t,s) is in DG is called dependees(s).
    ///        (The set of things that s depends on) 
    //
    // For example, suppose DG = {("a", "b"), ("a", "c"), ("b", "d"), ("d", "d")}
    //     dependents("a") = {"b", "c"}
    //     dependents("b") = {"d"}
    //     dependents("c") = {}
    //     dependents("d") = {"d"}
    //     dependees("a") = {}
    //     dependees("b") = {"a"}
    //     dependees("c") = {"a"}
    //     dependees("d") = {"b", "d"}
    /// </summary>

    public class DependencyGraph
    {
        /// <summary>
        /// This dictionary contains the dependents of each elements, key is the element
        /// </summary>
        private Dictionary<String, HashSet<String>> dependentsDictionary;

        /// <summary>
        /// This dictionary contains the dependees of each element, key is the element
        /// </summary>
        private Dictionary<String, HashSet<String>> dependeesDictionary;

        /// <summary>
        /// this keeps track of the number of order pairs that are in the dependency graph
        /// </summary>
        private int orderPairCount;

        /// <summary>
        /// Creates an empty DependencyGraph.
        /// </summary>
        public DependencyGraph()
        {
            this.dependentsDictionary = new Dictionary<string, HashSet<string>>();
            this.dependeesDictionary = new Dictionary<string, HashSet<string>>();
            this.orderPairCount = 0;
        }

        /// <summary>
        /// The number of ordered pairs in the DependencyGraph.
        /// </summary>
        public int Size
        {

            get { return this.orderPairCount; }

        }

        /// <summary>
        /// The size of dependees(s).
        /// This property is an example of an indexer.  If dg is a DependencyGraph, you would
        /// invoke it like this:
        /// dg["a"]
        /// It should return the size of dependees("a")
        /// </summary> 
        public int this[string s]
        {

            get
            {
                if (dependeesDictionary.ContainsKey(s))
                {
                    return dependeesDictionary[s].Count();
                }
                else
                {
                    return 0;
                }
            }
        }

        /// <summary>
        /// Reports whether dependents(s) is non-empty.
        /// </summary>
        public bool HasDependents(string s)
        {
            if (dependentsDictionary.ContainsKey(s))
            {
                int size = dependentsDictionary[s].Count();
                
                if ( size > 0)
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

        /// <summary>
        /// Reports whether dependees(s) is non-empty.
        /// </summary>
        public bool HasDependees(string s)
        {
            if (dependeesDictionary.ContainsKey(s))
            {
                int size = dependeesDictionary[s].Count();

                if (size > 0)
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
                return false;
            }
        }

        /// <summary>
        /// Enumerates dependents(s).
        /// </summary>
        public IEnumerable<string> GetDependents(string s)
        {
            if (dependentsDictionary.ContainsKey(s))
            {
                return new List<String>(dependentsDictionary[s]);
            }
            else
            {
                return new String[0];
            }
        }

        /// <summary>
        /// Enumerates dependees(s).
        /// </summary>
        public IEnumerable<string> GetDependees(string s)
        {
            if (dependeesDictionary.ContainsKey(s))
            {
                return new List<string>(dependeesDictionary[s]);
            }
            else
            {
                return new String[0];
            }
        }

        /// <summary>
        /// <para>Adds the ordered pair (s,t), if it doesn't exist</para>
        /// 
        /// <para>This should be thought of as:</para>   
        /// 
        ///   t depends on s
        ///
        /// </summary>
        /// <param name="s"> s must be evaluated first. T depends on S</param>
        /// <param name="t"> t cannot be evaluated until s is</param>        /// 
        public void AddDependency(string s, string t)
        {
            // Checks the s element in the dependents dictionary
            // adds and updates the dictionary
            if (dependentsDictionary.ContainsKey(s))
            {
                if (dependentsDictionary[s].Add(t))
                {
                    this.orderPairCount += 1;
                }
            }
            else
            {
                dependentsDictionary.Add(s, new HashSet<string>());
                if (dependentsDictionary[s].Add(t))
                {
                    this.orderPairCount += 1;
                }
            }

            // Checks the t element in the dependents dictionary
            // adds and updates the dictionary
            if (!dependentsDictionary.ContainsKey(t))
            {
                dependentsDictionary.Add(t, new HashSet<string>());
            }

            // Checks the t element in the dependees dictionary
            // adds and updates the dictionary
            if (dependeesDictionary.ContainsKey(t))
            {
                dependeesDictionary[t].Add(s);
            } else
            {
                dependeesDictionary.Add(t, new HashSet<string>());
                dependeesDictionary[t].Add(s);
            }

            // Checks the s element in the dependees Dictionary
            // adds and updates the dictionary
            if (!dependeesDictionary.ContainsKey(s))
            {
                dependeesDictionary.Add(s, new HashSet<string>());
            }
        }

        /// <summary>
        /// Removes the ordered pair (s,t), if it exists
        /// </summary>
        /// <param name="s">The dependee</param>
        /// <param name="t">The dependent</param>
        public void RemoveDependency(string s, string t)
        {
            if (dependeesDictionary.ContainsKey(t)) {
                if (dependeesDictionary[t].Contains(s))
                {
                    dependeesDictionary[t].Remove(s);
                    dependentsDictionary[s].Remove(t);
                    this.orderPairCount -= 1;
                }
            }
        }

        /// <summary>
        /// Removes all existing ordered pairs of the form (s,r).  Then, for each
        /// t in newDependents, adds the ordered pair (s,t).
        /// </summary>
        public void ReplaceDependents(string s, IEnumerable<string> newDependents)
        {
            if (dependentsDictionary.ContainsKey(s))
            {
                IEnumerable<string> sDependents = dependentsDictionary[s];

                List<String> copy = new List<string>(sDependents);

                foreach(String el in copy)
                {
                    RemoveDependency(s, el);
                }
            }
            foreach (String el in newDependents)
            {
                AddDependency(s, el);
            }
        }

        /// <summary>
        /// Removes all existing ordered pairs of the form (r,s).  Then, for each 
        /// t in newDependees, adds the ordered pair (t,s).
        /// </summary>
        public void ReplaceDependees(string s, IEnumerable<string> newDependees)
        {
            if (dependeesDictionary.ContainsKey(s))
            {
                IEnumerable<String> sDependees = dependeesDictionary[s];

                List<String> copy = new List<String>(sDependees);

                foreach (String el in copy)
                {
                    RemoveDependency(el, s);
                }
            }

            foreach (String el in newDependees)
            {
                AddDependency(el, s);
            }
        }
    }
}
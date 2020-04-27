'''
September 5, 2019
Carlos Martinez
'''

'''
These are the import statement of stuff I used
'''
from data import Data
import numpy as np
import math
DATA_DIR = 'data/'

'''
This method calculates the entropy of the result column
'''
def entropyResult(entropyResultData_obj):
    column = entropyResultData_obj.get_column('label')
    total = len(column)
    pSum = 0
    eSum = 0

    for row in column:
        if row == 'p':
            pSum = pSum + 1
        else:
            eSum = eSum + 1

    if pSum == 0 or eSum == 0:
        return 0

    pFrac = pSum / total
    eFrac = eSum / total
    return (-eFrac*math.log(eFrac,2)-pFrac*math.log(pFrac,2))

'''
This method returns entropy of a column value 
'''
def entropy(entropyData_obj):
    total = len(entropyData_obj)
    pSum = 0
    eSum = 0

    for row in entropyData_obj:
        if row[1] == 'p':
            pSum = pSum + 1
        else:
            eSum = eSum + 1

    if pSum == 0 or eSum == 0:
        return 0

    pFrac = pSum / total
    eFrac = eSum / total
    return (-eFrac * math.log(eFrac, 2) - pFrac * math.log(pFrac, 2))

'''
This method calculates the expected Entropy
'''
def expectedEntropy(expectedEntropyData_obj, expectedEntropyValues, expectedEntropyAttribute):
    total = len(expectedEntropyData_obj)
    expectedEntropyValue = 0

    for value in expectedEntropyValues:
        entropyValue = entropy(expectedEntropyData_obj.get_row_subset(expectedEntropyAttribute, value).get_column([expectedEntropyAttribute, 'label']))
        size = len(expectedEntropyData_obj.get_row_subset(expectedEntropyAttribute, value).get_column([expectedEntropyAttribute, 'label']))
        expectedEntropyValue = expectedEntropyValue + ((size / total) * entropyValue)

    return expectedEntropyValue

'''
This calculates information gain
'''
def informationGain(infoGainData_obj, infoGainAttributes):
    resultEntropy = entropyResult(infoGainData_obj)

    informationGainResult = {}

    for attribute in infoGainAttributes:
        expectedEntropyResult = expectedEntropy(infoGainData_obj, infoGainData_obj.get_attribute_possible_vals(attribute), attribute)
        informationGainResult[attribute] = resultEntropy - expectedEntropyResult

    return informationGainResult

'''
This method checks if all the labels are the same
'''
def sameLabelChecker(sameLabelCheckerData_obj):
    column = sameLabelCheckerData_obj.get_column('label')
    label = None
    checker = False

    for row in column:
        if checker == False:
            checker = True
            label = row
        else:
            if label != row:
                return False

    return checker

'''
This returns the next attribute based off information gained
'''
def nextRoot(nextRootAttributes, nextRootData_obj):
    informationGainResult = informationGain(nextRootData_obj, nextRootAttributes)
    maxValue = -1
    attributeRoot = "Carlos"

    for attribute in nextRootAttributes:
        if informationGainResult[attribute] > maxValue:
            maxValue = informationGainResult[attribute]
            attributeRoot = attribute

    return attributeRoot

'''
Finds the majority label
'''
def majority(majorityData_obj):
    column = majorityData_obj.get_column('label')
    total = len(column)
    pSum = 0
    eSum = 0

    for row in column:
        if row == 'p':
            pSum = pSum + 1
        else:
            eSum = eSum + 1

    if pSum > eSum:
        return 'p'
    else:
        return 'e'

'''
This returns the expected Label given a row and using a decision tree
'''
def expectedLabel(expectedLabelAttributes, expectedLabelRow, expetedLabelRoot):
    if expetedLabelRoot.value is not None:
        # print(root.type)
        return expetedLabelRoot.value
    else:
        index = expectedLabelAttributes[expetedLabelRoot.attribute].index
        for kid in expetedLabelRoot.kids:
            branch = expectedLabelRow[index + 1]
            if branch == kid.branch:
                return expectedLabel(expectedLabelAttributes, expectedLabelRow, kid)

'''
This calculates the accuracy of a program
'''
def accuracy(accuracyData_obj, accuracyRoot):
    numRows = len(accuracyData_obj)
    rows = accuracyData_obj.raw_data
    correctCounter = 0

    for row in rows:
        expected = expectedLabel(accuracyData_obj.attributes, row, accuracyRoot)
        label = row[0]
        if label  == expected:
            correctCounter += 1

    return (correctCounter / numRows) * 100

'''
This class represents a Node that helps create a tree
attribute = the branch of the tree
type = root or leaf
kids = the children of the root
value = the label of the leaf
'''
class Node():
    def __init__(self, type):
        self.type = type
        self.attribute = None
        self.branch = None
        self.kids = []
        self.value = None

'''
This method creates a decision tree and returns the root node to the tree.
 The method uses the ID3 algorithm, uses the majority when no answer.
'''
def id3(id3Attributes, id3Data_obj):
    id3AttributesCopy = id3Attributes.copy()
    if sameLabelChecker(id3Data_obj):
        leaf = Node("leaf")
        leaf.value = next(iter(id3Data_obj.get_column('label')))
        return leaf
    else:
        root = Node("root")
        attribute = nextRoot(id3AttributesCopy, id3Data_obj)
        root.attribute = attribute
        branches = id3Data_obj.attributes[attribute].possible_vals
        id3AttributesCopy.pop(attribute)
        for branch in branches:
            data_subset = id3Data_obj.get_row_subset(attribute, branch)
            if len(data_subset) == 0:
                label = majority(id3Data_obj)
                leaf = Node("leaf")
                leaf.value = label
                leaf.branch = branch
                root.kids.append(leaf)
            else:
                kid = id3(id3AttributesCopy,data_subset)
                kid.branch = branch
                root.kids.append(kid)

        return root

'''
This method creates a decision tree and returns the root node to the tree.
 The method uses the ID3 algorithm, uses the majority when no answer.
 This has depth control
'''
def id3Depth(id3Attributes, id3Data_obj,depth):
    id3AttributesCopy = id3Attributes.copy()
    if sameLabelChecker(id3Data_obj):
        leaf = Node("leaf")
        leaf.value = next(iter(id3Data_obj.get_column('label')))
        return leaf
    elif depth == 0:
        leaf = Node("leaf")
        label = majority(id3Data_obj)
        leaf.value = label
        return leaf
    else:
        root = Node("root")
        attribute = nextRoot(id3AttributesCopy, id3Data_obj)
        root.attribute = attribute
        branches = id3Data_obj.attributes[attribute].possible_vals
        id3AttributesCopy.pop(attribute)
        for branch in branches:
            data_subset = id3Data_obj.get_row_subset(attribute, branch)
            if len(data_subset) == 0:
                label = majority(id3Data_obj)
                leaf = Node("leaf")
                leaf.value = label
                leaf.branch = branch
                root.kids.append(leaf)
            else:
                kid = id3Depth(id3AttributesCopy,data_subset, depth - 1)
                kid.branch = branch
                root.kids.append(kid)

        return root

'''
This method calculates the depth of the tree
'''
def depth(root,level):
    if root.value is not None:
        return level
    else:
        max = level
        for x in root.kids:
            dep = depth(x,level+1)
            if dep > max:
                max = dep
        return max

def standardDeviation(average, accu):
    sum = 0
    sum = sum + ((accu[0] - average) * (accu[0] - average))
    sum = sum + ((accu[1] - average) * (accu[1] - average))
    sum = sum + ((accu[2] - average) * (accu[2] - average))
    sum = sum + ((accu[3] - average) * (accu[3] - average))
    sum = sum + ((accu[4] - average) * (accu[4] - average))
    sum = sum / 5
    return math.sqrt(sum)


########################################################################################################################

printBool = True
trainData = np.loadtxt(DATA_DIR + 'train.csv', delimiter=',', dtype = str)
trainData_obj = Data(data = trainData)
print("(a) Most common label in the data",majority(trainData_obj))
print("(b) Entropy of the data",entropyResult(trainData_obj))
testData = np.loadtxt(DATA_DIR + 'test.csv', delimiter=',', dtype = str)
testData_obj = Data(data = testData)
attributesSet = trainData_obj.attributes
rootNode = id3(attributesSet, trainData_obj)
ig = informationGain(trainData_obj, trainData_obj.attributes.keys())
print("(c) Best feature and its information gain",rootNode.attribute,ig[rootNode.attribute])
# print("Depth:", depth(rootNode,0))
print("(d) Accuracy on the training set",accuracy(trainData_obj, rootNode),"%")
print("(e) Accuracy on the test set",accuracy(testData_obj, rootNode),"%")

########################################################################################################################
bestDepth = 0
bestaccuracy = 0
depths = [1,2,3,4,5,10,15]
combineFoldsNames = ["fold1234.csv", "fold1235.csv", "fold1245.csv", "fold1345.csv", "fold2345.csv"]
singleFoldNames = ["fold5.csv", "fold4.csv", "fold3.csv", "fold2.csv", "fold1.csv"]

for i in range(7):
    depth = depths[i]
    accuracies = []

    for j in range(5):
        combineFoldName = combineFoldsNames[j]
        singleFoldName = singleFoldNames[j]
        combineFoldData = np.loadtxt(DATA_DIR + combineFoldName, delimiter=',', dtype=str)
        combineFoldData_obj = Data(data=combineFoldData)
        singleFoldData = np.loadtxt(DATA_DIR + singleFoldName, delimiter=',', dtype=str)
        singleFoldData_obj = Data(data=singleFoldData)
        attributesSet = combineFoldData_obj.attributes
        rootID3Combine = id3Depth(attributesSet, combineFoldData_obj, depth)
        thisAccuracy = accuracy(singleFoldData_obj, rootID3Combine)
        accuracies.append(thisAccuracy)

    sum = 0
    for j in range(5):
        sum += accuracies[j]
    average = sum / 5

    if average > bestaccuracy:
        bestDepth = depth
        bestaccuracy = average

    print("(f) Cross-validation accuracies for Depth",depth,":", average)

########################################################################################################################

rootNodeDepth = id3Depth(attributesSet, trainData_obj, 4)
print("(g) Best depth",bestDepth)
print("(h) Accuracy on the test set using the best depth",accuracy(testData_obj, rootNodeDepth),"%")
# print("Depth:", depth(rootNode,0))

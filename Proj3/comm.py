#!/usr/bin/python

import random, sys, locale, string
from optparse import OptionParser

class comm:							#creating a class comm that reads in the files
		def __init__(self, fOne, fTwo, inputOptions):
			self.fOne = fOne
			self.fTwo = fTwo
			self.inputOptions = inputOptions

		def sort(self):
			#first we will cover the option that both the files are sorted
			currF1 = 0
			currF2 = 0 

			fileTabMultiplier = 0		#since python supports multiplying strings to repeat it, I am using this to multiply tabs for proper spacing
			if self.inputOptions.hideOne == True and self.inputOptions.hideTwo == True:
				fileTabMultiplier = -2
			elif self.inputOptions.hideOne == True and self.inputOptions.hideTwo == False:
				fileTabMultiplier = -1
			elif self.inputOptions.hideOne == False and self.inputOptions.hideTwo == True:
				fileTabMultiplier = -1
			
			# one file might be larger than the other, so we will go through both files but stop when we reach the end of
			# one of them. The rest of the words will be unique to either of the files (logically)
			if self.inputOptions.areUnsorted == False:
				while currF1 < len(self.fOne) and currF2 < len(self.fTwo):
					if self.fOne[currF1] < self.fTwo[currF2]:
						if self.inputOptions.hideOne == True:
							sys.stdout.write("")
						else:
							sys.stdout.write("" + self.fOne[currF1])
						currF1+=1
					elif (self.fOne[currF1] > self.fTwo[currF2]):
						if self.inputOptions.hideTwo == True:
							sys.stdout.write("")
						else:
							sys.stdout.write("\t"*(fileTabMultiplier+1) + self.fTwo[currF2])
						currF2+=1
					elif (self.fOne[currF1] == self.fTwo[currF2]):
						if self.inputOptions.hideThree == True:
							sys.stdout.write("")
						else:
							sys.stdout.write("\t"*(fileTabMultiplier+2) + self.fTwo[currF2])
						currF2+=1
						currF1+=1

				#now that we have sorted out the equal parts of the file, if we are at the end of file1
				if currF1 == len(self.fOne):
					while currF2 < len(self.fTwo):
						if self.inputOptions.hideTwo == True:
							sys.stdout.write("")
						else:
							sys.stdout.write("\t"*(fileTabMultiplier+1) + self.fTwo[currF2])
						currF2+=1
				elif currF2 == len(self.fTwo):		#or if we are at the end of the second file and add the first file
					while currF1 < len(self.fOne):
						if self.inputOptions.hideOne == True:
							sys.stdout.write("")
						else:
							sys.stdout.write("" + self.fOne[currF1])
						currF1+=1
			
			#now for the hard part, the unsorted case -> logic will be as follows: check all of file 1 to file 2 then check all of file 2 to file 1
			#put the unique to file 1 and same in both simultaenously. BUT add the ones unique to file2 into a list to be appended at the end
			elif self.inputOptions.areUnsorted == True:
				currF1 = 0
				currF2 = 0
				col2 = []
				col3 = []
				
				fileTabMultiplier = 0		#since python supports multiplying strings to repeat it, I am using this to multiply tabs for proper spacing
				if self.inputOptions.hideOne == True and self.inputOptions.hideTwo == True:
					fileTabMultiplier = -2
				elif self.inputOptions.hideOne == True and self.inputOptions.hideTwo == False:		#same file tab multipliers as the previous section
					fileTabMultiplier = -1
				elif self.inputOptions.hideOne == False and self.inputOptions.hideTwo == True:
					fileTabMultiplier = -1

				for word in self.fOne:										#go through all the words in file 1 and compare to the ones in file 2
					while currF2 < len(self.fTwo):
						if self.fTwo.count(word) == 0:
							if self.inputOptions.hideOne == True:
								sys.stdout.write("")
							else:
								sys.stdout.write("" + word)
							break
						elif word != self.fTwo[currF2]:
							currF2 += 1
						elif word == self.fTwo[currF2]:
							if col3.count(word) < min(self.fOne.count(word), self.fTwo.count(word)):
								col3.append(word)
								if self.inputOptions.hideThree == True:
									sys.stdout.write("")
								else:
									sys.stdout.write("\t"*(fileTabMultiplier+2) + self.fTwo[currF2])
							elif self.fOne.count(word) > self.fTwo.count(word):
								if self.inputOptions.hideOne == True:
									sys.stdout.write("")
								else:
									sys.stdout.write("" + word)
							break
					currF2 = 0
				
				for word2 in self.fTwo:									#go through all the words in file2 and compare to file1 to make sure unique to file2
					while currF1 < len(self.fOne):
						if self.fOne.count(word2) == 0:
							col2.append(word2)
							break
						elif word2 != self.fOne[currF1]:
							currF1 +=1
						elif word2 == self.fOne[currF1]:
							if col3.count(word2) < min(self.fOne.count(word2), self.fTwo.count(word2)):
								col3.append(word)
								if self.inputOptions.hideThree == True:
									sys.stdout.write("")
								else:
									sys.stdout.write("\t"*(fileTabMultiplier+2) + self.fTwo[currF2])
							if self.fTwo.count(word2) > self.fOne.count(word2):
								col2.append(word2)
							break
					currF1 = 0

				if self.inputOptions.hideOne == True and self.inputOptions.hideTwo == False and self.inputOptions.hideThree == False:		#various formatting issues for printing out the tabs
					for k in col2:
						sys.stdout.write("" + k)
				elif self.inputOptions.hideOne == False and self.inputOptions.hideTwo == False and self.inputOptions.hideThree == False:
					for j in col2:
						sys.stdout.write("\t" + j)
				elif self.inputOptions.hideOne == True and self.inputOptions.hideTwo == False and self.inputOptions.hideThree == True:
					for j in col2:
						sys.stdout.write("" + j)
				elif self.inputOptions.hideOne == False and self.inputOptions.hideTwo == False and self.inputOptions.hideThree == True:
					for j in col2:
						sys.stdout.write("\t" + j)

def convertToList(fileName):
	f = open(fileName, 'r')						#reading in the file and converting it into a list
	convertedList = list(f.readlines())
	f.close()
	return convertedList

def main():
	version_msg = "%prog 1.0"
	usage_msg = """%prog [OPTION/S] ... FILE1 ... FILE2
	Compares two files like the comm POSIX command. Takes -1,2,3 and/or -u as options."""	
																									#setting up the parser
	parser = OptionParser(version=version_msg, usage=usage_msg)
	parser.add_option("-1", action="store_true", dest="hideOne", help="Suppress column one, lines unique to file One", default=False)
	parser.add_option("-2", action="store_true", dest="hideTwo", help="Suppress column two, lines unique to file Two", default=False)
	parser.add_option("-3", action="store_true", dest="hideThree", help="Suppress column three, lines that are common to both files", default=False)
	parser.add_option("-u", action="store_true", dest="areUnsorted", help="Compare two unsorted inputs", default=False)
	options, args = parser.parse_args(sys.argv[1:])

	if len(args) != 2:
		parser.error("You either have too few operands or too many. Remember the format is [options] file1 file2")

	try:
		if args[0] == '-':			# one of the inputs MAY be a - which means to read in from standard input
			fileOne = list(sys.stdin.readlines())			
		else:
			fileOne = convertToList(args[0])		# convert the file into a list to sort it easily
		if args[1] == '-':
			fileTwo = list(sys.stdin.readlines())
		else:
			fileTwo = convertToList(args[1])		#convver the second file into a list to sort it easily
		if options.areUnsorted == False	:		#this means that the files should be sorted to work. If not sorted, throw error
			if (fileOne != sorted(fileOne) or fileTwo != sorted(fileTwo)) :
				parser.error("Either one of your files is unsorted. Please sort them first and then put them into this program. Or use the -u command to compare unsorted files")

	except IOError as err:
		errorno, strerror = srr.args
		parser.error("I/O error({0}): {1}".format(errorno, strerror))

	commFiles = comm(fileOne, fileTwo, options)
	commFiles.sort()

if __name__ == "__main__":		#invoking the main method 
	main()
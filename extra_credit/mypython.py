#!/usr/local/bin/python


#this is the extra credit program that creates three files with 10 random characters and outputs them. Outputs 2 random number and outputs their product


#how to do print statements in python

#random function in python
import random;

#list element for characters
alpha=["a","b","c","d","e","f","g","h","i","j","k","l","m","o","p","q","r","s","t","u","v","w","x","y","z"];

print "\n";
#open a file
fo = open("file1.txt","wb")
for x in range(0,10):
	rand=random.randint(0,24);   	#find a random number within the range of 1-26
	index=alpha[rand];		#index that charactet
	print "the character inside first file is  %s" % index
	fo.write(index);		#insert that character into the file
fo.close()		#close the file

print "\n";
fo = open("file2.txt","wb")
for x in range(0,10):
	rand3=random.randint(0,24);   	#find a random number within the range of 1-26
	index2=alpha[rand3];		#index that charactet	
	print "the character inside the second file is %s" % index2
	fo.write(index2);		#insert that character into the file
fo.close()		#close the file

print "\n";
fo = open("file3.txt","wb")
for x in range(0,10):
	rand4=random.randint(0,24);   	#find a random number within the range of 1-26
	index3=alpha[rand4];		#index that charactet
	
	print "the character inside the third file is %s" % index3
	fo.write(index3);		#insert that character into the file
fo.close()		#close the file

print "\n";

rand=random.randint(1,42);
rand2=random.randint(1,42);
product=rand*rand2
print "The first random number is %s" % rand
print "The second random numbe is %s" % rand2
print "The product of the two numbers is %s" %product

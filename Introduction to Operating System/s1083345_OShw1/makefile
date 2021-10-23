all:	
	gcc master.c -o master.out
	gcc mmv.c -o mmv.out
	
clean:	
	rm -f master.out mmv.out happy.tmp happy2.tmp

test:	
	cat blake.txt
	ls
	./master.out
	cat happy.tmp
	ls
	./master.out blake.txt happy2.tmp
	cat happy2.tmp

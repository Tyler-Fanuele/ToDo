todo:main.o
	g++ main.cpp -o todo
mac:mac_main.o
	g++ mac_main.cpp -o todo
clean:
	rm main.o
	rm todo
clean_mac:
	rm mac_main.o
	rm todo


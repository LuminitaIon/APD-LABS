build:
	mpicxx main.cpp -o main
run:
	mpirun -oversubscribe -np 5 ./main tests/in/input1.txt
clean:
	rm main
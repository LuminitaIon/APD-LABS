Ion Luminita Andreea - 335CC
===============================================================================
					==========|TEMA 1 IOCLA|==========
===============================================================================

		---------------------------
			Descriere solutie
		---------------------------

Pornind de la implementarea secventiala a algoritmilor Julia si Mandelbrot si
folosind formulele din laboratorele 2 si 3 pentru variabilele start si end si 
folosind bariera ca si mod de sincronizare a thread-urilor am paralelizat cei
doi algoritmi dati.
Din schelet, am modificat, doar functiile run_julia, run_mandelbrot si main.
Am definit global functia MIN si am facut variabilele int width, height,
int **result si params par globale, deoarece trebuia sa aducem semnaturile 
celor doua functii modificate la forma acceptata de functia unui thread:
void *function_name(void *arg).

===============================================================================

		---------------------------
			Algoritmul Julia
		---------------------------
Am urmat drept model laboratul 3 si am paralelizat Julia folosind formulele
lui start si end. Am paralelizat doar al doilea for, daca il paralelizam (si) pe 
primul aveam sansa la race condition, deoarece la un moment dat ar fi putut sa
se foloseasca aceleasi date(numere complexe in cazul de fata) pentru mai multe
thread-uri.

Dupa for-ul paralelizat, am pus o bariera pentru a astepta ca fiecare thread 
sa se termine inainte de a trece mai departe.

Mai departe, am paralelizat si bucata de cod unde se transforma rezultatul
din coordonate matematice in coordonate ecran. Am putut face acest lucru
pentru a simplifica desenarea coordonatelor si astfel, am trecut din calcul
secvential al coordonatelor, in calcul paralelizat si asa facem mai multe
calcule in acelasi timp.

		---------------------------
		   Algoritmul Mandelbrot
		---------------------------
Am paralelizat facand un analog la algoritmul Julia, deoarece algoritmii sunt
asemanatori. Diferenta este ca aici, numarul complex c va reprezenta pozitia
unui pixel, iar z va incepe de la coordonatele (0,0), la Julia, z are direct 
coordonatele pixelului si a fi actualizat mereu.

Paralelizarea acestui algoritm este la fel cu cea de la algoritmul Julia.

		---------------------------
			      Main
		---------------------------
Am adaugat crearea thread-urilor si am dat join fiecarui thread dupa care
am distrus barierele. Am facut asta pentru fiecare algoritm in parte.
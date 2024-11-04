<!-- markdownlint-disable-file MD028 -->
# Laboratorio 2

In questa lezione impareremo altri concetti base della programmazione tramite il linguaggio _C++_.

---

- [Laboratorio 2](#laboratorio-2)
  - [Area di lavoro](#area-di-lavoro)
  - [Esercizio 1: primi passi con doctest](#esercizio-1-primi-passi-con-doctest)
    - [Implementazione della funzione `sum_n` e dei primi test](#implementazione-della-funzione-sum_n-e-dei-primi-test)
  - [Esercizio 2: minimo comune multiplo](#esercizio-2-minimo-comune-multiplo)
  - [Esercizio 3: conteggio dei numeri multipli di un valore di riferimento](#esercizio-3-conteggio-dei-numeri-multipli-di-un-valore-di-riferimento)
  - [Esercizio 4: espansione in serie di Taylor della funzione coseno](#esercizio-4-espansione-in-serie-di-taylor-della-funzione-coseno)
    - [Una implementazione più efficiente](#una-implementazione-più-efficiente)
  - [Consegna facoltativa del lavoro svolto](#consegna-facoltativa-del-lavoro-svolto)
  - [Approfondimenti ed esercizi](#approfondimenti-ed-esercizi)

---

Gli obiettivi principali di questo laboratorio sono:

- imparare a utilizzare i cicli `for` e `while`
- creare funzioni che eseguono semplici operazioni numeriche
- apprendere i rudimenti dell'utilizzo degli `std::vector`
- familiarizzare con l'attività di unit-testing (avvalendoci di _[doctest](https://github.com/doctest/doctest)_)

Per farlo, realizzeremo alcuni programmi che verificano la correttezza delle funzioni per il calcolo delle seguenti
quantità:

1. `sum_n.cpp`: la somma dei primi _n_ numeri naturali
2. `lcm.cpp`: il minimo comune multiplo tra due numeri interi
3. `count_multiples.cpp`: conteggio del numero di elementi di un vettore che sono multipli di un dato valore
4. `taylor.cpp`: l'espansione in serie di taylor della funzione trigonometrica coseno

Durante il laboratorio vi invitiamo a tenere aperte le
[slide](https://github.com/Programmazione-per-la-Fisica/pf2024/releases/latest) presentate a lezione.

## Area di lavoro

Creiamo una nuova directory di lavoro (ad esempio `pf_labs/lab2`) e aggiungiamo il file`.clang-format`.
Possiamo copiarlo dalla cartella utilizzata per il primo laboratorio, o generarlo ex-novo.

> [!TIP]
> Potete procedere in modo analogo a quanto fatto [durante il primo laboratorio](../lab1/README.md#area-di-lavoro).

In seguito scarichiamo il file
[`doctest.h`](https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h) tramite il seguente comando:

```bash
$ pwd
/home/fferrari/pf_labs/lab2
$ curl https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h -o doctest.h
```

Fatto ciò, l'area di lavoro deve trovarsi in questo stato:

```bash
$ pwd
/home/fferrari/pf_labs/lab2
$ ls -A
.clang-format  doctest.h
```

A questo punto siamo pronti ad aprire VSCode:

```bash
$ pwd
/home/fferrari/pf_labs/lab2
$ code .
```

> [!WARNING]
> Ricordate di aprire l'intera cartella di lavoro, aggiungendo il `.` come argomento del comando `code .`.

## Esercizio 1: primi passi con doctest

In questo esercizio implementeremo in una funzione la formula risolutiva del programma
[`sum_n.cpp`](../lab1/soluzioni/sum_n.cpp), sviluppato durante il primo laboratorio, e ne verificheremo il corretto
funzionamento utilizzando _[doctest](https://github.com/doctest/doctest)_.

Per farlo, cominciamo creando un file che chiameremo `sum_n.cpp`, dove andiamo a scrivere il seguente frammento di
codice:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

TEST_CASE("Testing the sum_n function") { }
```

**Formattiamo** il codice con _clang-format_, **salviamolo**, **compiliamolo** ed **eseguiamolo**:

```bash
$ g++ -Wall -Wextra sum_n.cpp -o sum_n 
$ ./sum_n 
[doctest] doctest version is "2.4.7"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases: 1 | 1 passed | 0 failed | 0 skipped
[doctest] assertions: 0 | 0 passed | 0 failed |
[doctest] Status: SUCCESS!
```

Come possiamo vedere, anche senza aver specificato nessuna funzione `main`, otteniamo un programma funzionante,
**com'è possibile?** Inoltre l'output del programma ci fa notare che **è stato eseguito con successo un _"test case"_**.

> [!IMPORTANT]
> Notate che i test vengono effettuati quando **si esegue** il programma, **non quando lo si compila**.

Analizziamo riga per riga quanto scritto, per chiarire il tutto:

- la direttiva nella prima linea:

  ```c++
  #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
  ```

  definisce un identificatore (`DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN`) che, se presente, richiede a _doctest_ di preparare
  una funzione `main` che si occupa di eseguire i _test case_ implementati.
- la seconda linea:

  ```c++
  #include "doctest.h"
  ```

  non fa altro che includere il file `doctest.h`, che abbiamo scaricato qualche riga sopra (in modo simile a quanto
  fatto per includere `iostream` durante il primo laboratorio).
- l'ultima linea:

  ```c++
  TEST_CASE("Testing the sum_n function") { }
  ```

  definisce il _test case_ a cui il programma si riferisce in fase di esecuzione. Non essendo implementato alcun tipo di
  test (vedremo come farlo a breve), questo risulta "eseguito con successo".
  > Più in dettaglio, `TEST_CASE` è una macro del preprocessore C++.
  > Il preprocessore non sarà oggetto di approfondimento durante il corso e sarà usato in casi molto limitati, quali
  > appunto la dichiarazione di unit test.

### Implementazione della funzione `sum_n` e dei primi test

A questo punto, prima di effettuarne una qualsiasi implementazione della funzione `sum_n` pensiamo a come
**dovrebbe essere fatta sintatticamente** questa funzione e a come **dovrebbe comportarsi**.

> [!IMPORTANT]
> Oltre che a verificare che il codice non venga "rotto" durante la differenti fasi di sviluppo, i test servono ad
> assicurarci che **una funzione si comporti "come vogliamo"**.

> [!IMPORTANT]
> :exclamation: Pertanto, una volta definito il comportamento che ci aspettiamo, è buona norma **implementare i test
> prima di implementare il codice** che questi vanno a verificare.

la funzione `sum_n` deve:

- accettare un intero **non negativo** come argomento
- restituire un intero col calcolo della sommatoria

In _C++_ questo si traduce in `int sum_n(int n) { ... }`.

> [!NOTE]
> Quando trovate dei **punti di sospensione** `...` ci stiamo riferendo **a parti di codice** o testo **che non vogliamo
> riportare**, oppure, come in questo caso, **a parti del programma che voi dovete completare**.

In questa implementazione, però, è formalmente permesso passare come argomento della funzione `sum_n` un numero intero
`n` < 0.

_**Come vogliamo che si comporti la funzione in questo caso?**_

La gestione di casi "problematici" verrà discussa più avanti nel corso. Per ora limitiamoci a decidere che, in caso
venga fornito un argomento negativo, `sum_n` debba sempre restituire il letterale intero `-1`.

A questo punto, abbiamo abbastanza informazioni per definire il comportamento atteso da parte di `sum_n`.

**Prima di implementarla**, procediamo quindi a **implementare alcuni test** per verificare che questa:

- ritorni i valori attesi in un paio di casi che calcoleremo "a mano" in anticipo (es.: `n=9` e `n=10`)
- ritorni `0` per il caso `n=0`
- ritorni `-1` quando inseriamo un paio di numeri negativi (es.: `n=-4` e `n=-5`)

In doctest, verifiche del genere si esprimono secondo la seguente sintassi:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

TEST_CASE("Testing the sum_n function") {
  CHECK(sum_n(10) == 55);
  ...
}
```

> [!NOTE]
> Completate voi l'implementazione dei test.

A questo punto, se **formattiamo**, **salviamo** e proviamo a **compilare**:

```bash
$ g++ -Wall -Wextra sum_n.cpp -o sum_n 
In file included from sum_n.cpp:3:
sum_n.cpp: In function 'void DOCTEST_ANON_FUNC_8()':
sum_n.cpp:6:9: error: 'sum_n' was not declared in this scope
    6 |   CHECK(sum_n(10) == 55);
      |         ^~~~~
...
```

otteniamo il seguente errore che ci ricorda che dobbiamo implementare `sum_n`.

Facciamolo:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

int sum_n(int n) {
  ...
}

TEST_CASE("Testing the sum_n function") {
...
}
```

e ripetiamo la compilazione fino a quando il programma non funziona come atteso, ad esempio:

```bash
$ g++ -Wall -Wextra sum_n.cpp -o sum_n 
$ ./sum_n                                      
[doctest] doctest version is "2.4.7"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases: 1 | 1 passed | 0 failed | 0 skipped
[doctest] assertions: 5 | 5 passed | 0 failed |
[doctest] Status: SUCCESS!
```

> [!TIP]
> Verificare come si comportano i test se, al posto della formula risolutiva implementata durante il primo laboratorio,
> si utilizzano:
>
> ```c++
> int sum{n / 2 * (n + 1)};
> ```
>
> ```c++
> int sum{0.5 * n * (n + 1)};
> ```
>
> :question: Vi aspettavate queste differenze? Sapreste spiegare a cosa sono dovute?

Alla fine dei turni del secondo laboratorio, potrete trovare la soluzione dell'esercizio 1
[al seguente link](soluzioni/sum_n.cpp).

## Esercizio 2: minimo comune multiplo

In questo esercizio vogliamo scrivere una funzione che calcoli il minimo comune multiplo tra due numeri interi.

> [!NOTE]
> Ricordiamo la definizione: il **minimo comune multiplo** di due numeri interi _a_ e _b_, indicato con `lcm(a,b)` è il
> più piccolo numero intero **positivo** multiplo sia di _a_ sia di _b_.
> Nel caso particolare in cui uno tra _a_ o _b_ è uguale a zero, allora si definisce `lcm(a,b)` uguale a zero.

Cominciamo creando un file che chiameremo `lcm.cpp`, dove andiamo a scrivere un frammento di codice analogo a quello
utilizzato per cominciare l'[Esercizio 1](#esercizio-1-primi-passi-con-doctest).

Come prima, **iniziamo** a pensare al comportamento che la funzione **dovrebbe avere**, quindi ai **test da
implementare**:

- Possiamo cominciare col definire il comportamento della funzione `lcm(a,b)` per alcuni numeri interi positivi (es:
  `a=12` e `b=16`, ma aggiungetene un altro paio voi)
- Quando `a=0` e/o `b=0` vogliamo che `lcm(a,b)` restituisca `0`
- Vogliamo infine verificare che la funzione si comporti correttamente quando uno tra _a_ e _b_ è minore di `0`, o
  entrambi lo sono.

Iniziamo quindi a implementare tutti i test:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

TEST_CASE("Testing the lcm function") {
  CHECK(lcm(12,16) == 48);
  ...
}
```

In seguito implementiamo la funzione:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

int lcm(int a, int b) {
  ...
}

TEST_CASE("Testing the lcm function") {
  ...
}
```

Ai fini di questo laboratorio, suggeriamo di implementare il calcolo utilizzando un approccio a "forza bruta":

- inizializziamo la variabile intera `max` al valore massimo tra `a` e `b` (ad esempio, potete inizialmente farlo
  utilizzando un `if`)
- inizializziamo la variabile intera `result` al valore di `max`
- verifichiamo se `result % a` e `result % b` risultano **entrambi** `0`, se non lo sono, incrementiamo `result` di
  `max` e ripetiamo la verifica fino a quando questa non risulta soddisfatta (potreste utilizzare un ciclo `while`)

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

int lcm(int a, int b) {
  ...

  if (a > b) {
    ...
  } else {
    ...
  }
  
  int result{max};

  while(...) {
    ...
  }

  ...
}

TEST_CASE("Testing the lcm function") {
  ...
}
```

> [!NOTE]
> Ovviamente potete procedere nel modo che ritenete più opportuno, a patto di verificare il comportamento atteso
> tramite i test.

**Formattate**, **salvate**, **compilate** il codice ed **eseguitelo**.

Se avete optato per l'approccio suggerito, probabilmente la funzione soddisfa la prima casistica di test, ma potrebbe
fallire i check per i quali _a_ e/o _b_ sono minori o uguali a `0`.

> [!WARNING]
> Pensate voi a modificare la funzione `lcm(a,b)` per gestire opportunamente questi casi.

> [!TIP]
> Per farlo, potrebbe essere utile utilizzare qualche funzione definita nella libreria
> [`cmath`](https://en.cppreference.com/w/cpp/header/cmath), come `std::abs`.

Alla fine dei turni del secondo laboratorio, potrete trovare la soluzione dell'esercizio 2
[al seguente link](soluzioni/lcm.cpp).

## Esercizio 3: conteggio dei numeri multipli di un valore di riferimento

In questo esercizio vogliamo scrivere una funzione che dati due argomenti, uno `std::vector` e un numero intero, conti
quanti elementi dello `std::vector` sono multipli del valore intero dato come argomento.

Creiamo quindi un file chiamato `count_multiples.cpp` e inseriamo uno scheletro minimale del nostro programma:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

TEST_CASE("Testing the count_multiples function") {}
```

Come prima, iniziamo a pensare al comportamento che la funzione dovrebbe avere, quindi ai test da implementare:

- La funzione deve accettare due argomenti, uno `std::vector<int>` e un numero intero `M` e ritornare all'utente un
  valore intero corrispondente al numero di elementi all'interno del vettore che sono multipli di `M`;
- La funzione deve ritornare `-1` se `M = 0`
- La funzione deve ritornare `0` se il vettore è vuoto

Iniziamo quindi a implementare tutti i test necessari:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <vector>

#include "doctest.h"


TEST_CASE("Testing the count_multiples function") {
  std::vector<int> values_empty;
  std::vector<int> values_even = {2, 4, 6, 8}; 
  ...
  CHECK(count_multiples(values_empty,2) == 0);
  ...
  CHECK(count_multiples(values_even, 2) == 4);
  ...
}
```

> [!NOTE]
> Ricordate di aggiungere `#include <vector>` all'inizio del vostro programma per poter utilizzare gli `std::vector`.

In seguito implementiamo la funzione:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <vector>

#include "doctest.h"


int count_multiples(std::vector<int> values, int M) {
  ...
}

TEST_CASE("Testing the count_multiples function") {
  std::vector<int> values_empty;
  std::vector<int> values_even = {2, 4, 6, 8}; 
  ...
  CHECK(count_multiples(values_empty,2) == 0);
  ...
  CHECK(count_multiples(values_even, 2) == 4);
  ...
}
```

**Formattate**, **salvate**, **compilate** il codice ed **eseguitelo**.

> [!IMPORTANT]
> Compilate aggiungendo come _flag_ di compilazione opzionale `-D_GLIBCXX_ASSERTIONS`. Questa indicherà al compilatore
> di controllare che l'accesso agli `std::vector` tramite indici non superi la dimensione del vettore stesso.

**Alcuni suggerimenti** riguardo l'implementazione della funzione:

- Il controllo sul valore dell'intero passato come argomento e sulla dimensione del vettore possono essere **fatti
  all'inizio della funzione**, per evitare operazioni inutili e/o pericolose
- Vi suggeriamo di utilizzare una variabile intera chiamata ad esempio `count` da aggiornare ogni volta che viene
  trovato un multiplo del valore `M` e da ritornare alla fine della funzione
- Potete accedere all'elemento i-esimo del vettore usando il _subscript operator_ `[]`
  
> [!WARNING]
> Ricordate che il conteggio degli elementi all'interno di un vettore **parte da 0**.

> [!CAUTION]
> Prestate molta attenzione a quando accedete a un elemento di vettore! È molto comune commettere errori e accedere ad
> elementi che non esistono utilizzando il _subscript operator_ `[]`

Alla fine dei turni del secondo laboratorio, potrete trovare la soluzione dell'esercizio 3
[al seguente link](soluzioni/count_multiples.cpp).

## Esercizio 4: espansione in serie di Taylor della funzione coseno

L'ultimo esercizio di oggi riguarda la scrittura e il test di una funzione che, per un dato intero positivo o nullo
_N_, calcola l'espansione in [Serie di Taylor](https://it.wikipedia.org/wiki/Serie_di_Taylor) all'ordine _N_ della
funzione trigonometrica $cos(x)$.

$$ cos(x) =  \sum_{n=0}^{\infty} \frac{(-1)^n}{(2n)!} x^{2n} \simeq \sum_{n=0}^{N} \frac{(-1)^n}{(2n)!} x^{2n} $$

Cominciamo creando un file che chiameremo `taylor.cpp`, dove andiamo a scrivere un frammento di codice analogo a quello
utilizzato per cominciare l'[Esercizio 1](#esercizio-1-primi-passi-con-doctest).

Prima di iniziare a pensare a come implementare i test, **notiamo che**, questo caso, **si presta alla scrittura di due
funzioni**:

1. quella che calcola l'espansione in serie di Taylor
2. quella per il calcolo del fattoriale, che verrà utilizzata per costruire la funzione precedente

> [!NOTE]
> Per l'elevamento a potenza, potete utilizzare la funzione `std::pow` definita nella libreria
> [`cmath`](https://en.cppreference.com/w/cpp/header/cmath).

Cominciamo quindi a preparare i test per la funzione `factorial(n)` che:

- deve restituire `1` se `n = 0` o `n = 1`
- deve restituire il prodotto dei numeri da 1 a `n` se `n > 1`
- (come per il primo esercizio) vogliamo che restituisca `-1` se `n < 0`

Cominciate quindi preparando i test:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

TEST_CASE("Testing the factorial function") {
  ...
}
```

completati questi, procedete con la scrittura della funzione `factorial(n)`.

> [!NOTE]
> Notate che:
>
> - nel caso `n > 1`, conosciamo il numero di moltiplicazioni che dobbiamo eseguire (suggeriamo pertanto di utilizzare
>   un ciclo `for` per effettuare il calcolo)
> - sempre nel caso `n > 1`, ci serve inizializzare una variabile in cui andremo a registrare il risultato
>   dell'operazione (`res`) fuori dal ciclo. Andremo ad aggiornare il suo contenuto a ogni interazione del ciclo `for`

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

... factorial(... n) {
  ...

  int res{1};
   for(...) {
    res *= ...
  }

  return res;
}

TEST_CASE("Testing the factorial function") {
  ...
}
```

> [!TIP]
> Se preferite, potete implementare `factorial(n)` come funzione _ricorsiva_.

Una volta validato il comportamento di `factorial(n)`, procediamo a implementare i test relativi all'espansione in
serie di Taylor del coseno.

Suggeriamo di preparare un **nuovo _test case_**:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

...

TEST_CASE("Testing the factorial function") {
  ...
}

TEST_CASE("Testing the taylor function") {
  ...
}
```

Come vogliamo che si comporti la funzione `taylor(x, N)`?

Una possibilità potrebbe essere:

- deve restituire il risultato della sommatoria quando `N >= 0`
- (come negli altri casi) chiediamo che restituisca `-1` se `N < 0`

Ai fini dell'esercizio non ci preoccupiamo troppo del valore di `x` in $[-\pi,\pi]$, prendiamo ad esempio `x = 0.5`.

> [!NOTE]
> Potete espandere voi la lista di test per diversi valori di `x`, se vi rimane tempo.

Per quel che riguarda il valore di `N`, suggeriamo di introdurre dei test per: `N = 0` , `N = 1` ,  `N = 2` , `N = 3`
ed `N = -9`.

A questo punto, occorre notare una cosa: `taylor(x, N)` **restituisce un numero _floating point_**.

Come è stato discusso a lezione, i numeri in virgola mobile sono un'"approssimazione" dei numeri reali: coprono un
intervallo di valori _finito_ e hanno precisione _finita_ (i quali, tra l'altro, sono differenti tra `float` e
`double`).

Potrebbe quindi accadere che, per una data combinazione di `x` ed `N`, la funzione `taylor(x, N)` non restituisca
esattamente il risultato che ci aspettiamo dal calcolo analitico, ma una sua approssimazione. In tal caso, l'uguaglianza
richiesta nei `CHECK` di doctest potrebbe fallire.

_**Come possiamo ovviare a tale problema?**_

Per i confronti di risultati in virgola mobile doctest fornisce la funzionalità di supporto `doctest::Approx`.

Implementate quindi i test, assumendo per i risultati 3 cifre significative:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

...

TEST_CASE("Testing the taylor function") {
  ...
  CHECK(taylor(0.5, 1) == doctest::Approx(0.875));
  ...
}
```

Andate infine a implementare la funzione `taylor(x, N)`:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

int factorial(int n) {
...
}

double taylor(double x, int N) {

...

double sum{};

  for (int n{0}; n <= N; ++n) {
    sum += ...
  }

  ...
}

TEST_CASE("Testing the factorial function") {
  ...
}

TEST_CASE("Testing the taylor function") {
  ...
}
```

> [!NOTE]
> Anche in questo caso suggeriamo di utilizzare un ciclo `for` e una variabile di supporto definita fuori dal ciclo
> nella quale accumulare i risultati delle varie iterazioni.

Arrivando a un'implementazione apparentemente corretta del codice, dovreste comunque ottenere due errori di questo
tipo:

```bash
$ g++ -Wall -Wextra taylor.cpp -o taylor 
$ ./taylor                                       
[doctest] doctest version is "2.4.7"
[doctest] run with "--help" for options
===============================================================================
taylor.cpp:36:
TEST CASE:  Testing the taylor expansion function

taylor.cpp:39: ERROR: CHECK( taylor(0.5, 2) == doctest::Approx(0.877) ) is NOT correct!
  values: CHECK( 0.8776041667 == Approx( 0.877 ) )

taylor.cpp:40: ERROR: CHECK( taylor(0.5, 3) == doctest::Approx(0.877) ) is NOT correct!
  values: CHECK( 0.8775824653 == Approx( 0.877 ) )

===============================================================================
[doctest] test cases: 2 | 1 passed | 1 failed | 0 skipped
[doctest] assertions: 9 | 7 passed | 2 failed |
[doctest] Status: FAILURE!
```

_**A cosa sono dovuti?**_

Quando abbiamo introdotto `doctest::Approx` abbiamo detto che questa funzionalità serve per **gestire correttamente le
approssimazioni dovute alla precisione finita dei calcoli in virgola mobile**.

Però, vi abbiamo poi chiesto di **arrotondare i risultati da immettere nei test alla terza cifra significativa**. Tale
arrotondamento risulta in una precisione molto inferiore rispetto a quella intrinseca di un `double` (o di un `float`),
che non viene coperta dall'uso che abbiamo fatto di `doctest::Approx` finora.

Quando si decide che, per una determinata funzione, ci si accontenta di una precisione minore di quella offerta di
default da `doctest::Approx` si può forzare la precisione richiesta in questo modo:

```c++
  CHECK(taylor(0.5, 2) == doctest::Approx(0.877).epsilon(0.001));
```

### Una implementazione più efficiente

Una volta che avrete controllato che i test siano tutti soddisfatti, procedete col porvi la seguente domanda: _**questa
implementazione del calcolo dell'espansione di Taylor è davvero la migliore possibile?**_

> [!IMPORTANT]
> Molto spesso esistono diverse strade che portano allo stesso risultato, le quali differiscono per alcune scelte di
> implementazione. Queste possono avere conseguenze **molto significative**, ad esempio, sul tempo di esecuzione di un
> programma o sulla sua complessità.

Cerchiamo di semplificare il codice per **evitare di ripetere calcoli inutili**. In questo caso, **una delle parti più
"pesanti"** a livello computazionale è quella relativa al **calcolo del fattoriale**.

Provate a modificare la funzione `double taylor(double x, int N)` in modo da **non ricalcolare interamente il
fattoriale** $(2n)!$ **a ogni iterazione del ciclo `for`**.

> [!IMPORTANT]
> In questo caso specifico possiamo notare che:
>
> - a ogni iterazione del ciclo `for` nella funzione `taylor` vale la relazione: $(2n)! = [2(n-1)]!\ (2n-1)\ (2n)$
> - possiamo registrare il risultato del fattoriale in una variabile `int factorial_2n` che andremo ad aggiornare ad
>   ogni iterazione
> - in questo modo non è più necessario utilizzare la funzione `factorial` all'interno della funzione `taylor`

> [!IMPORTANT]
> I test preparati in precedenza **permettono di verificare che**, anche dopo il cambiamento di implementazione, **la
> funzione continui a comportarsi  nel modo atteso**.

Alla fine dei turni del secondo laboratorio renderemo disponibili entrambe le soluzioni dell'esercizio 4 (sia
[la versione che ripete il calcolo del fattoriale](soluzioni/taylor.cpp), sia
[quella più efficiente](soluzioni/taylor_opt.cpp)).

## Consegna facoltativa del lavoro svolto

Per chi lo desiderasse, è possibile effettuare una consegna **facoltativa** del lavoro svolto durante il laboratorio.
Questa è un'opzione che lasciamo a quegli studenti che, incerti su alcuni punti, vogliono chiarire i loro dubbi.

Le consegne **non verranno valutate** e **non contribuiscono direttamente al risultato dell'esame**.

La consegna deve avvenire, da parte dei singoli studenti, tramite
[questo link](https://virtuale.unibo.it/mod/assign/view.php?id=1368319), il quale prevede il solo caricamento di file
`.zip` o `.tgz`.

Supponendo che tutto il materiale sia nella cartella `lab2` (e supponendo di trovarsi in tale cartella), per creare un
archivio `.zip` procedere come segue:

```bash
$ pwd
/home/fferrari/pf_labs/lab2
$ cd ..
$ zip -r lab2.zip lab2
$ ls
lab2 lab2.zip
```

Per creare un archivio `.tgz` procedere invece come segue:

```bash
$ pwd
/home/fferrari/pf_labs/lab2
$ cd ..
$ tar czvf lab2.tgz lab2
$ ls
lab2 lab2.tgz
```

Tutti coloro che effettuano una consegna facoltativa, sono pregati di **riportare**, **come commento** alla consegna
stessa, **dubbi o domande sull'elaborato per i quali è richiesto feedback** esplicito da parte dei docenti.

## Approfondimenti ed esercizi

Ecco una lista di possibili esercizi che si possono risolvere utilizzando cicli `for` e `while` e applicando un
approccio simile a quello che abbiamo imparato oggi (scrivere una funzione e, quando rilevante, i relativi test):

1. Calcolare, dato l'ordine `N`, lo sviluppo in serie di Taylor della funzione trigonometrica $sen(x)$
2. Data la precisione che si vuole raggiungere (es: `0.0001`), calcolare l'ordine `N` a cui è necessario sviluppare in
   serie di Taylor le funzioni $sen(x)$ e $cos(x)$ per ottenere tale precisione quando i valori dell'angolo `x` pari a
   `0.1`, `0.5` e `1.0`.
3. Creare una nuova funzione `multiples` che invece di contare il numero di elementi di un vettore multipli di un dato
   valore li ritorni all'utente come `std::vector`
4. Calcolare il numero della sequenza di Fibonacci a un dato ordine `n`
5. Calcolare il valore di $\pi$ approssimando a una prodotto finito il
   [Prodotto di Wallis](https://it.wikipedia.org/wiki/Prodotto_di_Wallis)
6. Calcolare il valore di $\pi$ approssimando a una sommatoria finita il seguente integrale:

$$ \pi = \int_{0}^{1} \frac{4}{1+x^{2}}dx \simeq \sum_{i=0}^{n-1} \frac{4}{1+x_{i}^{2}}\Delta x$$

$$ \mathrm{in\ cui}: x_{i} = \Delta x \cdot i$$

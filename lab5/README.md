<!-- markdownlint-disable-file MD028 -->
# Laboratorio 5

L'obiettivo di questo laboratorio è di distribuire su più file il codice
preparato durante il [quarto laboratorio](../lab4/README.md) per
implementare e testare la versione "migliorata" della classe `Regression`.
In aggiunta, utilizzeremo la famiglia di _tool_ _CMake_ per facilitare il
processo di compilazione.

---

- [Laboratorio 5](#laboratorio-5)
  - [Area di lavoro](#area-di-lavoro)
  - [Distribuzione del codice in più file](#distribuzione-del-codice-in-più-file)
    - [Preparazione di `regression.hpp`](#preparazione-di-regressionhpp)
    - [Utilizzo del `namespace pf`](#utilizzo-del-namespace-pf)
    - [Test della _One-Definition Rule_ e uso di _include guard_](#test-della-one-definition-rule-e-uso-di-include-guard)
    - [Utilizzo di `Regression` in più di una _translation-unit_](#utilizzo-di-regression-in-più-di-una-translation-unit)
    - [Definizione di funzioni in `regression.cpp`](#definizione-di-funzioni-in-regressioncpp)
    - [Riposizionamento delle direttive `#include`](#riposizionamento-delle-direttive-include)
  - [Compilazione tramite _CMake_](#compilazione-tramite-cmake)
    - [Installazione di componenti aggiuntive](#installazione-di-componenti-aggiuntive)
    - [Download di file di esempio](#download-di-file-di-esempio)
    - [Comandi per compilazione e test](#comandi-per-compilazione-e-test)
  - [Consegna facoltativa del lavoro svolto](#consegna-facoltativa-del-lavoro-svolto)
  - [Approfondimenti ed esercizi](#approfondimenti-ed-esercizi)

---

## Area di lavoro

Creiamo una nuova directory di lavoro (ad esempio `pf_labs/lab5`) e aggiungiamo
i file `.clang-format` e `doctest.h`. Possiamo copiarli dalla cartella
utilizzata per il quarto laboratorio:

```bash
$ pwd
/home/battilan/pf_labs/lab5
$ cp ../lab4/{doctest.h,.clang-format} .
```

> [!NOTE]
> Se non avete adottato la nomenclatura suggerita durante i laboratori
> precedenti per quanto riguarda le varie directory in cui sono contenuti i file
> sviluppati durante i laboratori, dovrete modificare il _path_ relativo
> indicato sopra.

Andiamo poi a copiare nell'area di lavoro il programma sviluppato durante il
quarto laboratorio.

Siete liberi di scegliere se effettuare una copia del codice che voi stessi
avete sviluppato, o scaricare la soluzione da noi proposta tramite il comando:

```bash
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2024/main/lab4/soluzioni/regression.test.cpp -o regression.test.cpp
```

Fatto ciò, l'area di lavoro deve trovarsi in questo stato:

```bash
$ pwd
/home/battilan/pf_labs/lab5
$ ls -A
.clang-format  doctest.h regression.test.cpp
```

## Distribuzione del codice in più file

Quello che ci prefiggiamo di fare oggi è separare il codice presente
all'interno di `regression.test.cpp`, in modo tale da arrivare in una situazione
in cui ci siano:

- due file (`regression.hpp` e `regression.cpp`) che includono tutto il
  codice necessario per il calcolo della retta di regressione, quindi tutte le
  _class_, _struct_ e le funzioni libere a esclusione dei test;
- un file `regression.test.cpp` contenente solo i test e il supporto per la
  loro esecuzione.

In particolare, vogliamo raggiungere una condizione finale in cui:

- `regression.hpp` contenga la definizione della classe `Regression` e delle
  `struct` `Result` e `Point`, ma includa solo le dichiarazioni delle funzioni
  membro della classe, così come l'overload dell'operatore somma (implementato
  come funzione libera), dell'operatore di confronto (`==`) tra `Point` e della
  funzione libera `fit`;
- `regression.cpp` contenga le definizioni dei metodi relativi a `Regression`
  (`size`, `points`, le diverse funzioni membro `add` e `remove`, `fit` e
  l'overload dell'operatore `+=`), così come la definizione degli operatori
  somma e confronto e la funzione libera `fit`.

Ciò ci permetterà di comprendere l'uso delle _include guard_, della parola
chiave `inline` e di familiarizzare con alcuni errori comuni di compilazione e
_linking_.

Durante il laboratorio vi invitiamo a tenere aperte le
[slide](https://github.com/Programmazione-per-la-Fisica/pf2024/releases/latest)
presentate a lezione.

### Preparazione di `regression.hpp`

Cominciamo creando un nuovo file `regression.hpp`, dove spostiamo tutte le
classi, le funzioni e le direttive _include_ contenute in `regression.test.cpp`
(a eccezione di `#include "doctest.h"`).

Dentro `regression.test.cpp` dovrà rimanere solamente il contenuto relativo ai
test:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("Testing Regression") {
  ...
}
```

Se proviamo a compilare ora `regression.test.cpp` otterremo errori di
compilazione, dovuti alla mancanza di `Regression` e `Point`:

```bash
$ g++ -Wall -Wextra regression.test.cpp -o regression.t
regression.test.cpp: In function ‘void DOCTEST_ANON_FUNC_14()’:
regression.test.cpp:7:3: error: ‘Regression’ was not declared in this scope
    7 |   Regression reg;
      |   ^~~~~~~~~~
regression.test.cpp:8:3: error: ‘Point’ was not declared in this scope
    8 |   Point p1{1., 2.};
      |   ^~~~~
...
```

Per risolverli, è sufficiente aggiungere in `regression.test.cpp` la direttiva
`#include "regression.hpp"`, che si riferisce al file contenente il codice che
abbiamo spostato in precedenza.

> [!TIP]
> Il preprocessore si può immaginare come un motore di copia/incolla del
> contenuto del file incluso nel punto in cui avviene l'inclusione.

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "regression.hpp"

#include "doctest.h"

TEST_CASE("Testing Regression") {
  ...
}
```

Verifichiamo **compilando di nuovo** ed eseguendo i test:

```bash
$ g++ -Wall -Wextra regression.test.cpp -o regression.t
$ ./regression.t
[doctest] doctest version is "2.4.11"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:  2 |  2 passed | 0 failed | 0 skipped
[doctest] assertions: 45 | 45 passed | 0 failed |
[doctest] Status: SUCCESS!
```

> [!IMPORTANT]  
> Notate come il file `regression.hpp` **non sia incluso nel comando di
> compilazione**.

### Utilizzo del `namespace pf`

Come discusso a lezione, nel caso di progetti complessi risulta utile utilizzare
`namespace` per garantire che non ci siano conflitti tra i diversi
identificatori (es.: nomi delle funzioni) utilizzati nelle loro varie parti.

A titolo d'esempio, introduciamo quindi il namespace `pf` per racchiudere,
`Regression`, `Point`, `Results` e l'overload degli operatori.

> [!TIP]
> Spesso un _namespace_ può contenere parti di una libreria eterogenee e diverse
> tra loro (pensate alla _Standard Library_), pertanto il nome di un dato
> namespace è tipicamente "generico" e non necessariamente riflette il nome di
> una funzionalità specifica di una data libreria.

Per farlo, andiamo a racchiudere **tutto il blocco di codice** che abbiamo
appena spostato in `regression.hpp` all'interno del `namespace`:

```c++
namespace pf {

struct Result {
  double A;
  double B;
};

...

class Regression {
  std::vector<Point> points_;

  ...

};
 
...

}   // namespace pf
```

In seguito, andiamo ad anteporre `pf::` prima della costruzione di tutti gli
oggetti dei tipi `Regression` e `Point` in `regression.test.cpp`:

```c++
TEST_CASE("Testing Regression") {
  pf::Regression reg;
  pf::Point p1{1., 2.};
  pf::Point p2{2., 3.};

...

}

...
```

Formattiamo, compiliamo ed eseguiamo i test per verificare che il programma si
comporti come atteso.

### Test della _One-Definition Rule_ e uso di _include guard_

Cosa succede se includiamo `regression.hpp` all'interno di `regression.test.cpp`
**due** volte? Abbiamo visto a lezione che può capitare che lo stesso _header
file_ possa essere incluso, direttamente o indirettamente, più volte nella
stessa _translation unit_ (_TU_).

Proviamo quindi a farlo:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "regression.hpp"
#include "regression.hpp"

#include "doctest.h"

TEST_CASE("Testing Regression") {

  ...

}
```

e compiliamo:

```bash
$ g++ -Wall -Wextra regression.test.cpp -o regression.t
In file included from regression.test.cpp:4:
regression.hpp:5:8: error: redefinition of ‘struct pf::Result’
    5 | struct Result {
      |        ^~~~~~
In file included from regression.test.cpp:3:
regression.hpp:5:8: note: previous definition of ‘struct pf::Result’
    5 | struct Result {
      |        ^~~~~~
regression.hpp:10:8: error: redefinition of ‘struct pf::Point’
   10 | struct Point {
      |        ^~~~~
regression.hpp:10:8: note: previous definition of ‘struct pf::Point’
   10 | struct Point {
      |        ^~~~~
regression.hpp:15:6: error: redefinition of ‘bool pf::operator==(const Point&, const Point&)’
   15 | bool operator==(Point const& lhs, Point const& rhs) {
      |      ^~~~~~~~
...
```

Possiamo notare ridefinizioni, delle struct `Result` e `Point`, degli operatori
`operator==` e `operator+`, nonché della classe `Regression`. Siccome siamo
all'interno della stessa _TU_, il compilatore è in grado di diagnosticare queste
violazioni della _One-Definition Rule_.

Anche in questo caso la soluzione è semplice: inseriamo un'_include guard_ in
`regression.hpp`, in modo tale che, se l'_header file_ viene incluso più volte
nella stessa _TU_, il suo contenuto venga considerato solo la prima volta.

Aggiungiamo quindi **all'inizio** del file `regression.hpp`:

```c++
#ifndef PF_REGRESSION_HPP
#define PF_REGRESSION_HPP

...
```

Inoltre aggiungiamo **alla fine** dello stesso file:

```c++
...

#endif
```

> [!IMPORTANT]  
> `PF_REGRESSION_HPP` è una _macro_ del preprocessore.
>
> Invece di `PF_REGRESSION_HPP` avremmo potuto scegliere un nome qualsiasi, ma è
> necessario garantirne l'unicità tra tutti gli _header file_ che fanno parte
> del nostro progetto, o che appartengono ad altri progetti ma che includiamo.
>
> Scegliere un nome che richiami il nome del file, scritto tutto in lettere
> maiuscole, è una delle convenzioni maggiormente utilizzate (che quindi
> adottiamo anche noi).

A questo punto siamo in grado di compilare ed eseguire correttamente i nostri
test. Ci siamo ricondotti a una situazione stabile:

```bash
$ g++ -Wall -Wextra regression.test.cpp -o regression.t
$ ./regression.t
[doctest] doctest version is "2.4.11"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:  2 |  2 passed | 0 failed | 0 skipped
[doctest] assertions: 45 | 45 passed | 0 failed |
[doctest] Status: SUCCESS!
```

> [!NOTE]  
> Dopo questa verifica, provvediamo a rimuovere dal file `regression.test.cpp`
> lo _statement_ `#include "regression.hpp"` duplicato, che abbiamo utilizzato
> per il test.

### Utilizzo di `Regression` in più di una _translation-unit_

Supponiamo ora che il nostro componente software sia parte, o venga usato, in un
progetto con molti file e che quindi `regression.hpp` possa essere incluso in
più _TU_.

Il modo più semplice per porsi in tale situazione, e quindi verificare che
stiamo facendo le cose per bene, è di aggiungere una _TU_ formata da un file
sorgente che si limiti a includere l'header corrispondente. Questa _TU_ viene
poi integrata nel nostro comando di compilazione.

Creiamo pertanto un file `regression.cpp`, il cui contenuto sia semplicemente:

```c++
#include "regression.hpp"
```

Prima di aggiungerlo al comando di compilazione precedente, proviamo a
compilarlo separatamente (cioè, come singola _TU_).

Questo permette di verificare che l'header file `regression.hpp` sia completo,
che cioè includa tutto ciò che serve alle entità che dichiara/definisce.

```bash
$ g++ -Wall -Wextra -c regression.cpp
$ ls -t | grep regression
regression.o
regression.cpp
regression.t
regression.hpp
regression.test.cpp

```

> [!TIP]
> In questo caso si è aggiunta l'opzione `-c` al comando di compilazione, per
> dire al compilatore di non eseguire la fase di _linking_ e creare solamente il
> _file oggetto_  `regression.o`.

> [!TIP]
> Potete verificare cosa fa il comando `ls -t | grep regression`, più complesso
> di quelli utilizzati fino a ora, tramite
> [https://explainshell.com/](https://explainshell.com/explain?cmd=ls+-t+%7C+grep+regression).

Verificato che sia tutto a posto, possiamo provare a compilare tutto insieme:

```bash
$ g++ -Wall -Wextra regression.test.cpp regression.cpp -o regression.t
/usr/bin/ld: /tmp/ccTFtI5w.o: in function `pf::operator==(pf::Point const&, pf::Point const&)':
regression.cpp:(.text+0x0): multiple definition of `pf::operator==(pf::Point const&, pf::Point const&)'; /tmp/cchOnQSh.o:regression.test.cpp:(.text+0x0): first defined here
/usr/bin/ld: /tmp/ccTFtI5w.o: in function `pf::operator+(pf::Regression const&, pf::Regression const&)':
regression.cpp:(.text+0x50): multiple definition of `pf::operator+(pf::Regression const&, pf::Regression const&)'; /tmp/cchOnQSh.o:regression.test.cpp:(.text+0x50): first defined here
/usr/bin/ld: /tmp/ccTFtI5w.o: in function `pf::fit(pf::Regression const&)':
regression.cpp:(.text+0x124): multiple definition of `pf::fit(pf::Regression const&)'; /tmp/cchOnQSh.o:regression.test.cpp:(.text+0x124): first defined here
collect2: error: ld returned 1 exit status
```

Possiamo notare un errore di _linking_, dovuto agli operatori
`pf::operator==(pf::Point const&, pf::Point const&)`,
`pf::operator+(pf::Regression const&, pf::Regression const&)`, e alla funzione
libera `pf::fit(pf::Regression const&)` definiti al di fuori di classi o
`struct`.

Ciò è dovuto al fatto che, essendo definiti in un _header file_, ed essendo
questo'ultimo incluso in due _TU_, i due operatori `operator==` e `operator+`,
così come la funzione libera `fit`, risultano definiti due volte all'interno
dello stesso programma.

Per risolvere il problema è sufficiente aggiungere la parola chiave `inline` di
fronte alle definizioni delle funzioni definite fuori dalla classe:

```c++
inline bool operator==(Point const& lhs, Point const& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

...

inline Regression operator+(Regression const& l, Regression const& r) {
  Regression sum{l};
  return sum += r;
}

inline auto fit(Regression const& reg) {
  // the following call would fail compilation if the fit method weren't const
  return reg.fit();
}
```

Verifichiamo a questo punto che il codice compili e funzioni correttamente:

```bash
$ g++ -Wall -Wextra regression.test.cpp regression.cpp -o regression.t 
$ ./regression.t
[doctest] doctest version is "2.4.11"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:  2 |  2 passed | 0 failed | 0 skipped
[doctest] assertions: 45 | 45 passed | 0 failed |
[doctest] Status: SUCCESS!
```

### Definizione di funzioni in `regression.cpp`

Come discusso a lezione, la configurazione raggiunta al punto precedente è di
per se sufficiente per ottenere la separazione necessaria per utilizzare
`Regression` all'interno di molteplici _TU_ di uno stesso programma.

Un'altra alternativa è quella di spostare la definizione delle funzioni
contenute in `regression.hpp` all'interno di `regression.cpp`, mantenendo in
`regression.hpp` solo la loro dichiarazione.

**Entrambe le strategie hanno vantaggi e svantaggi**. Siccome il nostro scopo è
quello di impratichirci con tutte le possibili opzioni di suddivisione del
codice, proviamo a implementarle entrambe.

Cominciamo spostando la definizione di `Regression::add(Point const& p)`
all'interno di `regression.cpp`.

Il file `regression.hpp`, a questo punto, dovrà contenere solo la dichiarazione
della funzione:

```c++
...

class Regression {

  ...
  
  void add(Point const& p);
  
  ... 

};

...
```

Mentre il file `regression.cpp`, diventa:

```c++
#include "regression.hpp"

void pf::Regression::add(pf::Point const& p) { points_.push_back(p); }
```

Verifichiamo quindi che compilazione e test vadano a buon fine.

> [!IMPORTANT]
> Notate come abbiamo anteposto `pf::Regression::` alla funzione add, una volta
> che l'abbiamo spostata nel file `regression.cpp`.

Anteporre ovunque `pf::` nella definizione delle diverse funzioni che vogliamo
spostare in `regression.cpp` renderebbe il codice ridondante e difficile da
leggere.

Prima di procedere, andiamo quindi ad includere direttamente nel `namespace pf`
la definizione di `add`, come mostrato qui:

```c++
namespace pf {
  void Regression::add(Point const& p) {  points_.push_back(p); }
  ...
}
```

Verifichiamo che compilazione e test vadano a buon fine, poi procediamo
spostando anche `operator==` e `operator+` (inserendoli sempre all'interno del
`namespace pf`).

> [!NOTE]
> Una volta spostata la definizione degli operatori in `regression.cpp` dobbiamo
> ricordarci di rimuovere la parola chiave `inline`.
>
> :question: Cosa succede se non lo facciamo?

Procediamo dunque spostando anche la definizione delle seguenti funzioni membro
da `regression.hpp` a `regression.cpp`:

```c++
  bool remove(Point const& to_be_removed) {
    ...
  }

  auto fit() const {
    ...
  }

  Regression& operator+=(Regression const& r) {
    ...
  }
```

Ricordandoci, a ogni passaggio, di **compilare e di eseguire i test**.

> [!IMPORTANT]  
> Notiamo come, quando spostiamo il metodo `auto fit() const`, sia necessario
> specificare in maniera esplicita che il tipo di ritorno della funzione è
> `Result`.
>
> :question: Sapreste dire perché?

Giunti a questo punto, abbiamo affrontato assieme tutti i problemi che possono
incorrere durante lo spostamento delle diverse definizioni di funzioni tra
`regression.hpp` a `regression.cpp`.

**Procedete ora in autonomia** spostando il resto delle definizioni di funzione
in `regression.cpp`.

> [!IMPORTANT]
> A questo punto è opportuno considerare che, **qualora non aiutino la
> comprensione del codice**, i nomi dei parametri si possono omettere
> all'interno della dichiarazione di funzione.
>
> Ad esempio, in `regression.hpp`, `void add(Point const& p);` può
> tranquillamente diventare `void add(Point const&);`.
>
> Sperimentate in autonomia con questa possibilità.
>
> :question: Per quali dichiarazioni di funzione la semplificazione vi sembra
> opportuna, per quali ambigua?

### Riposizionamento delle direttive `#include`

Per completare l'ultimo punto dell l'esercizio, andiamo a considerare
un'ultima cosa: visto quanto abbiamo detto sul preprocessore, quanti degli
`#include` all'inizio di `regression.hpp` sono veramente necessari in quel file,
ora che le definizioni delle funzioni sono tutte in `regression.cpp`?

Procedete quindi spostando quante più direttive _include_:

```c++
#ifndef PF_REGRESSION_HPP
#define PF_REGRESSION_HPP

#include <stdexcept>
#include <vector>

...
```

in `regression.cpp`.

> [!NOTE]  
> :question: Perché è buona norma effettuare questa operazione?  

## Compilazione tramite _CMake_

Prima di terminare questo laboratorio, vogliamo illustrare brevemente l'utilizzo
di [_CMake_](https://cmake.org/), strumento che semplifica la compilazione di
progetti costituiti da più file sorgente.

> [!TIP]
> Qualora foste interessati ad approfondire lo studio di _CMake_ potete
> consultare la guida introduttiva disponibile
> [qui](https://crascit.com/professional-cmake/professionalcmake_20th_edition_gettingstarted/).

### Installazione di componenti aggiuntive

Prima di procedere, installiamo il [_ninja build system_](https://ninja-build.org/),
che ci permetterà di generare, contemporaneamente ed in modo consistente,
configurazioni per la compilazione in diverse modalità (in particolare _Debug_ e
_Release_) all'interno di una unica area di _build_.

Procedere con l'installazione è semplice, basta eseguire il seguente comando
in _Ubuntu 24.04_:

```bash
$ sudo apt install ninja-build
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following NEW packages will be installed:
  ninja-build
...
```

oppure, se si utilizzano _macOS_ e _Homebrew_:

```bash
$ brew install ninja
==> Downloading https://formulae.brew.sh/api/formula.jws.json
==> Downloading https://formulae.brew.sh/api/cask.jws.json
==> Downloading https://ghcr.io/v2/homebrew/core/ninja/manifests/1.12.1-1
Already downloaded: /Users/battilan/Library/Caches/Homebrew/downloads/e245d32f82082268a4093a10343e3ef9e2b2eebc81897551ebe10cdea0f45051--ninja-1.12.1-1.bottle_manifest.json
==> Fetching ninja
...
```

Per verificare la corretta installazione del pacchetto eseguite il seguente
comando:

```bash
$ ninja --version
1.11.1
```

### Download di file di esempio

Siccome il nostro scopo non è esplorare "tutte" le funzionalità di _CMake_,
partiamo da un file `CMakeLists.txt` preparato apposta apposta per questo
laboratorio, scaricandolo:

```bash
$ pwd
/home/battilan/pf_labs/lab5
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2024/main/lab5/soluzioni/CMakeLists.txt -o CMakeLists.txt
```

> [!NOTE]
> Il file che abbiamo appena scaricato è stato preparato per funzionare qualora
> si sia seguita l'esatta nomenclatura per i file `.cpp` e `.hpp` proposta in
> questa traccia. Qualora abbiate adottato una convenzione diversa per i nomi
> dei file, modificate opportunamente il file `CMakeLists.txt`.

Dopo averlo aperto, possiamo notare come questo, di proposito, sia stato
commentato in dettaglio in modo da rendere chiaro il funzionamento di ogni
sua parte.

Scarichiamo inoltre un _main program_ minimale, che fa uso della classe
`Regression`.

```bash
$ pwd
/home/battilan/pf_labs/lab5
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2024/main/lab5/soluzioni/main.cpp -o main.cpp
```

> [!IMPORTANT]
> Aprendolo, potete notare che il file `main.cpp` appena scaricato non fa
> praticamente nulla, a parte istanziare un oggetto di tipo `Regression` e
> gestire alcune possibili eccezioni.
>
> Per ora non preoccupatevi di ciò, visto che questo esempio minimale è
> sufficiente per apprendere i rudimenti dell'uso di _CMake_.

> [!NOTE]
> Ulteriori indicazioni per lo sviluppo del _main program_ sono fornite come
> [approfondimento facoltativo](#approfondimenti-ed-esercizi) alla fine della
> traccia di questo laboratorio.

> [!IMPORTANT]
> Infine, la struttura del blocco `try` `catch` presentata nel file `main.cpp`
> verrà discussa in dettaglio nelle prossime lezioni.

### Comandi per compilazione e test

Giunti a questo punto, basta seguire la seguente ricetta, discussa qui passo
dopo passo, per effettuare la compilazione ed eseguire i test.

```bash
$ cmake -S . -B build -G"Ninja Multi-Config"
-- The C compiler identification is GNU 13.3.0
-- The CXX compiler identification is GNU 13.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (1.0s)
-- Generating done (0.1s)
-- Build files have been written to: /home/battilan/pf_labs/lab5/build
```

Questo comando utilizza il file `CMakeLists.txt` presente nella directory
corrente (`.`) per **creare e configurare** un'area di compilazione all'interno
della cartella `build`, ma **non effettua alcuna compilazione**.

> [!TIP]
> In caso il file `CMakeLists.txt` non cambi, basta eseguire il comando appena
> presentato una sola volta.

```bash
$ cmake --build build --config Debug
[6/6] Linking CXX executable Debug/regression.t
```

Questo comando **esegue la compilazione** dell'area presente all'interno della
cartella `build` per la variante _Debug_. Si può procedere in modo simile per la
variante _Release_.

> [!TIP]
> In  caso la compilazione fallisca, o qualora vengano modificati i file
> sorgente (`.cpp` e `.hpp`) su cui stiamo lavorando, basta eseguire di nuovo
> quest'ultimo comando **senza configurare di nuovo l'area di compilazione**.

```bash
$ cmake --build build --config Debug --target test
Running tests...
Test project /home/battilan/pf_labs/lab5/build
    Start 1: regression.t
1/1 Test #1: regression.t .....................   Passed    0.32 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.32 sec
```

Questo comando esegue i test definiti all'interno del file `CMakeLists.txt`
tramite il comando `add_test( ... )`.

> [!TIP]
> Questo approccio, una volta che una lista completa di test è stata
> implementata, semplifica notevolmente la procedura di validazione di progetti
> di grandi dimensioni, che includono più di un singolo eseguibile di test.

Alla fine dei turni del quinto laboratorio, potrete trovare la soluzione
dell'esercizio [al seguente link](soluzioni/).

## Consegna facoltativa del lavoro svolto

Per chi lo desiderasse, è possibile effettuare una consegna **facoltativa** del
lavoro svolto durante il laboratorio. Questa è un'opzione che lasciamo a quegli
studenti che, incerti su alcuni punti, vogliono chiarire i loro dubbi.

Le consegne **non verranno valutate** e **NON contribuiscono al risultato
dell'esame**.

Tutti coloro che effettuano una consegna facoltativa, sono pregati di
**riportare**, **come commento** alla consegna stessa, **dubbi o domande
sull'elaborato per i quali è richiesto feedback** esplicito da parte dei
docenti.

La consegna deve avvenire, da parte dei singoli studenti, tramite
[questo link](https://virtuale.unibo.it/mod/assign/view.php?id=1652615), il
quale prevede il solo caricamento di file `.zip` o `.tgz`.

Supponendo che tutto il materiale sia nella cartella `lab5` (e supponendo di
trovarsi in tale cartella), per creare un archivio `.zip` procedere come segue:

```bash
$ pwd
/home/battilan/pf_labs/lab5
$ cd ..
$ zip -r lab5.zip lab5
$ ls
lab4 lab5.zip
```

Per creare un archivio `.tgz` procedere invece come segue:

```bash
$ pwd
/home/battilan/pf_labs/lab5
$ cd ..
$ tar czvf lab5.tgz lab5
$ ls
lab4 lab5.tgz
```

## Approfondimenti ed esercizi

Per chi fosse interessato a "sperimentare ulteriormente" gli argomenti
presentati in questo laboratorio, viene proposto un possibile approfondimento
**opzionale**:

Provate ad aggiungere un nuovo file `main.cpp` contenente una funzione `main`
che usa i componenti definiti sopra.

Il programma accetta dall'utente comandi:

- `a X Y`: per aggiungere un punto (di coordinate X, Y) al campione. Ad esempio,
  per aggiungere il punto (`1.0`, `9.1`), scrivere `a 1.0 9.1`;
- `r X Y`: per rimuovere un punto al campione. Ad esempio, per
  rimuovere il punto (`1.0`, `9.1`), scrivere `r 1.0 9.1`;
  > In questo caso, ricordatevi di stampare a schermo un messaggio se la
  > rimozione **non** ha avuto successo.
- `c` per eseguire il calcolo della regressione lineare e stampare pendenza e
  intercetta  a schermo.
- `q` per uscire dal programma.

Qualora vengano immessi comandi sconosciuti, il programma deve menzionarlo,
per poi richiedere di nuovo l'inserimento di un comando valido.

Ricordatevi inoltre di gestire eventuali eccezioni e di controllare la
correttezza formale dell'input da tastiera.

Cambiate poi opportunamente il file `CMakeLists.txt` in modo da includere
la compilazione del _main program_.

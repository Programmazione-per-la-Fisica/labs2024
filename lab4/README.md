<!-- markdownlint-disable-file MD028 -->
# Laboratorio 4

In questa lezione riprenderemo il programma sviluppato durante il
[terzo laboratorio](../lab3/README.md) ed andremo ad aggiungervi nuove
funzionalità.
Dove possibile, faremo uso degli strumenti forniti dalla _standard library_ del
_C++_.

---

- [Laboratorio 4](#laboratorio-4)
  - [Area di lavoro](#area-di-lavoro)
  - [Aggiunta di nuove funzionalità alla classe `Regression`](#aggiunta-di-nuove-funzionalità-alla-classe-regression)
    - [Rimozione di un punto già inserito](#rimozione-di-un-punto-già-inserito)
    - [Rimozione di un punto a partire dalle sue coordinate](#rimozione-di-un-punto-a-partire-dalle-sue-coordinate)
    - [Confronto di oggetti di tipo `Point`](#confronto-di-oggetti-di-tipo-point)
  - [Overloading dell'operatore somma](#overloading-delloperatore-somma)
    - [Versione 1: operatore somma come funzione libera](#versione-1-operatore-somma-come-funzione-libera)
    - [Versione 2: operatore somma a partire dall'operatore membro `+=`](#versione-2-operatore-somma-a-partire-dalloperatore-membro-)
  - [Consegna facoltativa del lavoro svolto](#consegna-facoltativa-del-lavoro-svolto)
  - [Approfondimenti ed esercizi](#approfondimenti-ed-esercizi)

---

L'obiettivo principale di questo laboratorio è imparare i rudimenti dell'uso di
`std::vector<>`, implementare la somma di diversi campioni di dati (`Regression`)
tramite l'_overload_ dell'operatore `operator+` ed abituarci a consultare la
documentazione della _standard library_ del _C++_ alla ricerca di strumenti
(es.: _container_ ed _algoritmi_) che semplifichino lo sviluppo dei nostri
programmi.

Durante il laboratorio vi invitiamo a tenere aperte le
[slide](https://github.com/Programmazione-per-la-Fisica/pf2024/releases/latest)
presentate a lezione, così come la _webpage_
[cppreference.com](https://en.cppreference.com/w/).

## Area di lavoro

Creiamo una nuova directory di lavoro (ad esempio `pf_labs/lab4`) e aggiungiamo
i file `.clang-format` e `doctest.h`. Possiamo copiarli dalla cartella utilizzata per il terzo laboratorio:

```bash
$ pwd
/home/fferrari/pf_labs/lab4
$ cp ../lab3/doctest.h .
$ cp ../lab3/.clang-format .
```

> [!NOTE]
> Se non avete seguito la nomenclatura suggerita durante i laboratori precedenti per quanto riguarda le varie directory in cui sono contenuti i file sviluppati durante i laboratori, dovrete modificare il _path_ relativo indicato sopra.

Andiamo infine a copiare nell'area di lavoro il programma sviluppato durante il
terzo laboratorio.

Siete liberi di scegliere se effettuare una copia del codice che voi stessi
avete sviluppato, o scaricare la soluzione da noi proposta tramite il comando:

```bash
$ pwd
/home/fferrari/pf_labs/lab4
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2024/main/lab3/soluzioni/regression.test.cpp -o regression.test.cpp
```

Fatto ciò, l'area di lavoro deve trovarsi in questo stato:

```bash
$ pwd
/home/fferrari/pf_labs/lab4
$ ls -A
.clang-format  doctest.h regression.test.cpp
```

## Aggiunta di nuove funzionalità alla classe `Regression`

Pur svolgendo il compito che ci eravamo inizialmente prefissi, questa prima
versione di `Regression` risulta migliorabile sotto vari aspetti, ad esempio:

- un eventuale utilizzatore della classe potrebbe desiderare
  rimuovere alcuni dei punti introdotti in precedenza
  (es.: perché inseriti in modo incorretto, etc.);
- potrebbe essere utile sommare assieme due campioni di punti esistenti, così da evitare di aggiungere i punti uno alla volta tramite il metodo `add(double x, double y)`, per generare il campione totale sul quale eseguire la regressione.

In entrambi i casi risulta necessario "ricordare" l'intero insieme dei punti che
sono stati aggiunti tramite il metodo `add`.

Per prima cosa, introduciamo una `struct Point` che contenga le coordinate `x` e `y` dei singoli punti:

```c++
struct Point {
    double x;
    double y;
};
```

Fatto questo, andiamo a modificare la classe `Regression` per aggiungere uno
`std::vector<Point>` nella sua parte privata:

```c++
#include <vector>
...
class Regression {
  int N_{0};
  double sum_x_{0.};
  double sum_y_{0.};
  double sum_xy_{0.};
  double sum_x2_{0.};
  std::vector<Point> points_{};
...
};
```

> [!IMPORTANT]
> Ogni volta che incontrerete `...` questo significa che **parti di codice sono state omesse per brevità o che vanno completate da parte vostra**.

Quando aggiungiamo una variabile membro privata, la prima cosa che dobbiamo
chiederci è quale sia il suo impatto sull'**invariante di classe**, cioè la
relazione esistente tra i dati membri di una classe.

Mantenendo tutte le variabili membro finora introdotte in `Regression`,
**in ogni momento**, si dovrebbe garantire che:

- `N_` sia _sempre_ uguale a `points_.size()`;
- i valori delle variabili corrispondenti alle somme parziali siano _sempre_
  allineati con la serie di valori contenuti in `points_`.

Pertanto, preservare tutti questi dati membro renderebbe l'informazione nella
parte privata di `Regression` altamente ridondante, generando una classe
_più difficile da mantenere_.

D'altro canto, rimuovere tutte le variabili relative alle somme parziali ci
imporrebbe di calcolarle a partire da
`points_`, direttamente dentro al metodo `Regression::fit`, il che avrebbe
verosimilmente un impatto negativo sulla sua performance del programma.

Decidiamo di optare per **la rimozione dei dati membro ridondanti,
concettualmente più semplice, quindi più facile da implementare e mantenere**.

> [!IMPORTANT]
> Seguendo questo approccio, non abbiamo in realtà alcuna
> invariante di classe da verificare, potremmo quindi decidere di utilizzare
> una `struct` al posto di una `class`?
>
> Discutetene brevemente tra voi, poi chiedete conferma a docenti e tutor.
>
> Qualunque sia la risposta, allo scopo di illustrare alcune funzionalità del
> linguaggio, oggi procederemo mantenendo `Regression` come `class`.

Rimuoviamo quindi tutte le variabili membro di `Regression` ad eccezione di
`points_` e procediamo con le modifiche necessarie a `regression.test.cpp`.

Scegliamo inoltre di cambiare il metodo `Regression::add` per accettare un punto al posto delle coordinate del punto. Questo cambiamento non è fondamentale, in quanto il codice funzionerebbe anche con la precedente implementazione di `Regression::add`, ma rende il codice più leggibile dato che abbiamo definito la `struct Point`:

```c++
class Regression {
...
  void add(Point const& p) 
  {
    points_.push_back(p);
  }
...
}
```

Notate che questa modifica avrebbe delle conseguenze profonde nel codice, in quanto dovremmo cambiare tutte le occorrenze del metodo `Regression::add(double x, double y)`. Per evitare questo, possiamo scegliere di mantenere anche l'interfaccia che permette l'inserimento di un punto a partire dalle coordinate, ma di implementarla a partire da `Regression::add(Point const& p)`:

```c++
class Regression {
  ...
  void add(Point const& p) 
  {
    points_.push_back(p);
  }
  ...
  void add(double x, double y) 
  {
    ...
  }
}
```

> [!IMPORTANT]
> Aggiungete dei test ulteriori per testare `Regression::add(Point const& p)`. L'implementazione viene lasciata a voi (potete prendere spunto dai test già esistenti).

> [!IMPORTANT]
> Perché è conveniente implementare `Regression::add(double x, double y)` a partire da `Regression::add(Point const& p)`? Discutetene tra di voi e poi chiedete conferma ai tutor o ai docenti.

A questo punto, è necessario ricalcolare le somme all'interno del metodo `Regression::fit`:

```c++
auto fit() const {
  ...
    double sum_x{};
    ...
    for (auto const& p : points_) {
      sum_x += p.x;
      ...
    }
    ...
}
```

> [!IMPORTANT]
> L'implementazione dei cambiamenti è volutamente lasciata in uno stato parziale.

**Completate voi le modifiche** e verificate che il programma compili
e funzioni come deve prima di procedere. In particolare, **tutti i test devono risultare soddisfatti**.

> [!NOTE]
> L'utilizzo dei test ci permette di compiere modifiche anche molto estese al codice, come in questo caso, senza il timore di introdurre errori.

### Rimozione di un punto già inserito

Come discusso poco sopra, la prima funzionalità aggiuntiva che vogliamo
implementare è permettere di rimuovere un punto, **qualora questo risulti
già presente nel nostro campione**.

> [!IMPORTANT]
> Notate che, qualora un punto risultasse inserito più volte, cioè se fossero stati inseriti due punti le cui coordinate sono `(2.,3.)` per entrambi, quello che vogliamo fare è rimuovere **un singolo punto** relativo a quelle coordinate.

Seguiamo pertanto l'approccio utilizzato nel laboratorio precedente, definendo
l'_interfaccia_ tramite la quale vogliamo rendere possibile la rimozione, ed
andiamo ad **implementare i test prima di scrivere il codice** che la effettua.

Aggiungiamo quindi alla fine della nostra lista di test il seguente
_code snippet_:

```c++
TEST_CASE("Testing Regression") {
  Regression reg;
  Point p1{1., 2.};
  Point p2{2., 3.};

  REQUIRE(reg.size() == 0);

  ...

  SUBCASE("Removing an existing point - remove with Point")
  {
    reg.add(p1);
    reg.add(p2);
    CHECK(reg.remove(p1) == true);
    CHECK(reg.size() == 1);
  }

  SUBCASE("Removing a non-existing point - remove with Point")
  {
    reg.add(p1);
    reg.add(p2);
    Point p3{5., 4.};
    CHECK(reg.remove(p3) == false);
    CHECK(reg.size() == 2);
  }
}
```

Implicitamente, l'implementazione proposta suggerisce di aggiungere alla classe
`Regression` una funzione membro `remove` che accetta come argomento un oggetto di tipo `Point` che rappresenta il punto da rimuovere e restituisce un booleano che indica se la rimozione è avvenuta:

```c++
class Regression {
...
    bool remove(Point const& p) 
    { 
      ...
    }
...
};
```

Controllando la documentazione (fatelo ogni volta che avete un dubbio!), potete
verificare che `std::vector<>` fornisce un metodo
[`erase`](https://en.cppreference.com/w/cpp/container/vector/erase),
il quale accetta come argomento un `iterator` relativo all'elemento del
vettore di cui si vuole effettuare la rimozione.

Pertanto vi suggeriamo di iniziare l'implementazione della funzione partendo da
un loop realizzato tramite iteratori:

```c++
class Regression {
...
    bool remove(Point const& p) {
      for (auto it = points_.begin(), end = points_.end(); it != end; ++it) {
        ...
      }
    ...
    }
...
};
```

Completate la modifica procedendo fintanto che il codice non compila ed i test
risultano tutti eseguiti con successo.

> [!IMPORTANT]
> Sebbene non sia sempre opportuno controllare l'uguaglianza tra
> numeri in virgola mobile, in questo caso è corretto farlo poiché non stiamo
> comparando risultati di operazioni che potrebbero differire in funzione della
> precisione di un calcolo.
>
> Ricordate che, dopo una chiamata al metodo `erase`, l'iteratore
> `it` viene invalidato (così come quelli successivi), pertanto non sarà
> possibile utilizzarlo nemmeno per l'operazione di incremento. Questo va
> considerato durante l'implementazione di `remove`.

### Rimozione di un punto a partire dalle sue coordinate

Arrivati a questo punto, potete implementare una nuova funzione `remove` che prenda come argomenti le coordinate _x_ e _y_ del punto da rimuovere invece di un oggetto di tipo `Point`.

Come sempre, partiamo dall'implementazione dei test:

```c++
TEST_CASE("Testing Regression") {
  Regression reg;
  Point p1{1., 2.};
  Point p2{2., 3.};

  REQUIRE(reg.size() == 0);

  ...

  SUBCASE("Removing an existing point - remove with coordinates")
  {
    reg.add(p1);
    reg.add(p2);
    CHECK(reg.remove(1., 2.) == true);
    CHECK(reg.size() == 1);
  }

  SUBCASE("Removing a non-existing point - remove with coordinates")
  {
    reg.add(p1);
    reg.add(p2);
    CHECK(reg.remove(5., 4.) == false);
    CHECK(reg.size() == 2);
  }
}
```

> [!NOTE]
> Come già fatto per il metodo `add`, potete implementare questa funzione a  partire dalla funzione `remove(Point const& p)`.

Quando avrete finito, salvate, compilate e **controllate che tutti i test siano passati con successo**.

### Confronto di oggetti di tipo `Point`

Un metodo migliore per identificare se un punto vada rimosso o no è quello di ridefinire il comportamento dell'operatore `==` per oggetti di tipo `Point`, di modo da non dover confrontare ogni volta le coordinate _x_ e _y_ di due punti, ma i punti stessi.

Procediamo quindi con l'overload dell'operatore `==`, implementandolo come una funzione libera all'esterno della struct `Point`:

```c++
...
struct Point
{
  double x;
  double y;
};

bool operator==(Point const& lhs, Point const& rhs)
{
  ...
}
...
```

> [!IMPORTANT]
> Perché questo approccio risulta essere migliore rispetto al controllo delle singole coordinate per determinare se un punto è uguale ad un altro? Ragionate per esempio su cosa accadrebbe al vostro codice se la struct `Point` rappresentasse punti nello spazio tridimensionale invece che su un piano.

## Overloading dell'operatore somma

In alcuni casi potrebbe risultare utile combinare (o "sommare") tra loro diverse
istanze di `Regression`.

Ad esempio, si potrebbe decidere che diverse campagne di raccolta dati di un
medesimo esperimento vadano analizzate assieme (es.: per ridurre le incertezze
statistiche).

### Versione 1: operatore somma come funzione libera

Iniziamo implementando l'operatore somma come funzione libera.

Come sempre, la prima cosa da fare è implementare tutti i test che riteniamo
necessari, ad esempio:

```c++
...
TEST_CASE("Testing sum of two Regression objects")
{
  Regression reg1;
  reg1.add(0., 1.);
  Regression reg2;
  reg2.add(2., 3.);
  auto sum = reg1 + reg2;
  CHECK(sum.size() == 2);
  if (sum.size() == 2) {
    CHECK(sum.points()[0].x == doctest::Approx(0.0));
    CHECK(sum.points()[0].y == doctest::Approx(1.0));
    CHECK(sum.points()[1].x == doctest::Approx(2.0));
    CHECK(sum.points()[1].y == doctest::Approx(3.0));
  }
}
...
```

Nella nostra implementazione della classe `Regression` il vettore `points_` è
dichiarato come variabile membro `private`, pertanto non possiamo accedervi
direttamente.

Scrivendo il test abbiamo quindi notato che, oltre alla funzione libera
`operator+`, ci serve aggiungere un metodo che ci permetta di leggere i valori
contenuti all'interno di un'istanza di `Regression`, ad esempio:

```c++
class Regression {
...
 public:
...
  const std::vector<Point>& points() const { return points_; }
...
}
```

Nell'implementazione appena proposta possiamo notare che:

1. il metodo `Regression::points` restituisce una _const reference_ al vettore
   `points_`;
2. il metodo `Regression::points` è dichiarato come _const_.

Questa scelta ci permette di evitare di effettuare una copia di `points_` ogni
volta che viene effettuata una chiamata a `Regression::points` ma, al contempo,
permette di garantire che attraverso questa funzione il membro privato
`points_` non venga mai modificato.

> [!IMPORTANT]
> Notate come, in questo caso, entrambi i `const` vadano aggiunti assieme.
> Provate a rimuoverne un `const` alla volta e compilate per verificare cosa succede.

Procediamo quindi implementando una versione preliminare di `operator+`, che
permetta di compilare con successo:

```c++
Regression operator+(Regression const& l, Regression const& r) 
{ 
  return Regression{}; 
}
```

Come potrete notare, rimangono comunque un paio di `warning`, ed il test appena
aggiunto fallisce:

```bash
$ g++ regression.test.cpp -Wall -Wextra -o regression.test
regression.test.cpp: In function 'Regression operator+(const Regression&, const Regression&)':
regression.test.cpp:105:40: warning: unused parameter 'l' [-Wunused-parameter]
  105 | Regression operator+(Regression const& l, Regression const& r) { return Regression{}; }
      |                      ~~~~~~~~~~~~~~~~~~^
regression.test.cpp:105:61: warning: unused parameter 'r' [-Wunused-parameter]
  105 | Regression operator+(Regression const& l, Regression const& r) { return Regression{}; }
      |                                           ~~~~~~~~~~~~~~~~~~^


$ ./regression.test 
[doctest] doctest version is "2.4.11"
[doctest] run with "--help" for options
===============================================================================
regression.test.cpp:210:
TEST CASE:  Testing sum of two Regression objects

regression.test.cpp:217: ERROR: CHECK( sum.size() == 2 ) is NOT correct!
  values: CHECK( 0 == 2 )

===============================================================================
[doctest] test cases:  2 |  1 passed | 1 failed | 0 skipped
[doctest] assertions: 28 | 27 passed | 1 failed |
[doctest] Status: FAILURE!

```

In teoria, i metodi `Regression::add` e `Regression::points` sarebbero sufficienti per implementare una versione correttamente funzionante di `operator+`.

> [!NOTE]
> Potete provare a farlo, se arrivate a questo punto
> dell'esercizio abbastanza presto.

Però, rispetto ad aggiungere uno ad uno tutti gli elementi sfruttando il metodo
`Regression::add`, risulterebbe più efficiente e chiaro costruire il vettore di `points_` relativo al risultato di `operator+` utilizzando il metodo `std::vector<>::insert`. Potete consultare la documentazione relativa al metodo a questo [link](https://en.cppreference.com/w/cpp/container/vector/insert).

Per fare questo suggeriamo di implementare un nuovo metodo `void add(std::vector<Point> const& new_points)` che si occupi di aggiungere al vettore di `points_` un nuovo vettore di punti (qui chiamato `new points`):

```c++
class Regression {
...
  public: 

   ...
   void add(std::vector<Point> const& new_points)
   {
     points_.insert(...);
   }
   ...
```

A questo punto, potete procedere con l'implementazione della funzione libera `operator+`, che viene lasciata interamente a voi:

```c++
Regression operator+(Regression const& l, Regression const& r) 
{ 
  ...

  return ...
}
```

Una volta terminata l'implementazione, salvate, compilate e **verificate che tutti i test siano soddisfatti**.

### Versione 2: operatore somma a partire dall'operatore membro `+=`

Come discusso a lezione, è possibile implementare un generico operatore binario
simmetrico `operator@` (definito come funzione libera) in termini di un operatore
`@=` che sia metodo della classe per la quale cui vogliamo implementare
`operator@`.

Nel nostro caso, si  tratta di aggiungere a `Regression`:

```c++
class Regression {
  ...
  Regression& operator+=(Regression const& r) {
      ...
  }
};
```

E di semplificare `operator+` in modo che faccia uso di `Regression::operator+=`.

Lasciamo a voi l'onere dell'implementazione, ma ricordate che:

- `Regression::operator+=` è un metodo di `Regression`, pertanto può accedere alle sue variabili membro private;
- `Regression::operator+=` intende modificare l'istanza della classe su cui opera,
  pertanto potete avvalervi di `this` quando restituite il risultato della
  funzione.

Dopo le modifiche compilate il codice ed eseguite i test fino a quanto il
programma funziona correttamente.

Alla fine dei turni del quarto laboratorio, potrete trovare la soluzione
dell'esercizio [al seguente link](soluzioni/regression.test.cpp).

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
[questo link](https://virtuale.unibo.it/mod/assign/view.php?id=1652614), il
quale prevede il solo caricamento di file `.zip` o `.tgz`.

Supponendo che tutto il materiale sia nella cartella `lab4` (e supponendo di
trovarsi in tale cartella), per creare un archivio `.zip` procedere come segue:

```bash
$ pwd
/home/fferrari/pf_labs/lab4
$ cd ..
$ zip -r lab4.zip lab4
$ ls
lab4 lab4.zip
```

Per creare un archivio `.tgz` procedere invece come segue:

```bash
$ pwd
/home/fferrari/pf_labs/lab4
$ cd ..
$ tar czvf lab4.tgz lab4
$ ls
lab4 lab4.tgz
```

## Approfondimenti ed esercizi

Come di consueto, riportiamo alcune possibili estensioni dell'esercizio che
potete implementare per approfondire lo studio di alcuni concetti appresi
durante il corso:

- Utilizzare l'algoritmo `std::find` al posto del ciclo `for` nella funzione `remove` per trovare eventuali punti da rimuovere.
- Migliorare il test relativo ad `operator+` (es.: l'ordine degli elementi del campione è irrilevante).
- Creare un metodo `remove_all` che rimuova tutti i punti corrispondenti ad un dato set di coordinate. In particolare potete provare a implementarlo:
  - in un modo simile a quello proposto durante il laboratorio per il metodo `remove`;
  - utilizzando l'algoritmo `remove` seguito da `erase`;
  - tramite l'utility `std::erase` (se utilizzate _C++20_).

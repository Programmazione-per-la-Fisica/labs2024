<!-- markdownlint-disable-file MD028 -->
# Laboratorio 6

In questo laboratorio utilizzeremo come base di
partenza la soluzione dell'esercizio proposto nel
[quinto laboratorio](../lab5/README.md) per impratichirci con la generazione
di numeri (pseudo)casuali e con l'utilizzo di algoritmi della _standard library_.

---

- [Laboratorio 6](#laboratorio-6)
  - [Area di lavoro](#area-di-lavoro)
  - [Generazione di numeri pseudocasuali](#generazione-di-numeri-pseudocasuali)
  - [Utilizzo di `std::accumulate` per il calcolo delle somme parziali](#utilizzo-di-stdaccumulate-per-il-calcolo-delle-somme-parziali)
    - [Il comando `time`](#il-comando-time)
    - [Un utilizzo più efficiente di `std::accumulate`](#un-utilizzo-più-efficiente-di-stdaccumulate)
  - [Mediana di un campione di punti](#mediana-di-un-campione-di-punti)
    - [Ordinamento di vettori con criteri arbitrari](#ordinamento-di-vettori-con-criteri-arbitrari)
    - [Calcolo della mediana](#calcolo-della-mediana)
    - [Un'implementazione più efficiente](#unimplementazione-più-efficiente)
  - [Lettura/scrittura da/su file](#letturascrittura-dasu-file)
  - [Consegna facoltativa del lavoro svolto](#consegna-facoltativa-del-lavoro-svolto)
  - [Approfondimenti ed esercizi](#approfondimenti-ed-esercizi)

---

## Area di lavoro

Creiamo una nuova directory di lavoro (ad esempio `pf_labs/lab6`) e aggiungiamo
i file `.clang-format` e `doctest.h`. Possiamo copiarli dalla cartella
utilizzata per il quinto laboratorio:

```bash
$ pwd
/home/fferrari/pf_labs/lab6
$ cp ../lab5/{doctest.h,.clang-format} .
```

> [!NOTE]
> Se non avete adottato la nomenclatura suggerita durante i laboratori
> precedenti per quanto riguarda le varie directory in cui sono contenuti i file
> sviluppati durante i laboratori, dovrete modificare il _path_ relativo
> indicato sopra.

Andiamo poi a copiare nell'area di lavoro il programma sviluppato durante il
quinto laboratorio, composto da `regression.cpp`, `regression.hpp`, `regression.test.cpp` e `main.cpp`. Copiamo inoltre il `CMakeLists.txt` necessario per la compilazione.

Siete liberi di scegliere se effettuare una copia del codice che voi stessi
avete sviluppato, o scaricare la soluzione da noi proposta tramite i comandi:

```bash
$ pwd
/home/fferrari/pf_labs/lab6
$ curl -sO https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2024/main/lab5/soluzioni/regression.test.cpp
$ curl -sO https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2024/main/lab5/soluzioni/regression.cpp
$ curl -sO https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2024/main/lab5/soluzioni/regression.hpp
$ curl -sO https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2024/main/lab5/soluzioni/main.cpp
$ curl -sO https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2024/main/lab5/soluzioni/CMakeLists.txt
```

> [!NOTE]
> L'opzione `-s` del comando `curl` fa si che il comando venga eseguito senza produrre nessun output a schermo, mentre l'opzione `-O` permette di non specificare il nome del file di output e di usare automaticamente quello del file copiato.

## Generazione di numeri pseudocasuali

Passiamo adesso ad esercitarci sulla generazione di numeri _random_. Come
mostrato a lezione, questo può essere fatto tramite _random engines_ e _random distributions_, che sono parte della [_random number library_](https://en.cppreference.com/w/cpp/numeric/random). Non avendo esigenze particolari, in questo esempio utilizzeremo il `std::default_random_engine`.

Andiamo a modificare il file `main.cpp` fornito come soluzione del quinto laboratorio (o quello sviluppato da voi). Creiamo un oggetto di tipo `Regression` e uno di tipo `std::default_random_engine` che sarà utilizzato
per la generazione dei numeri pseudocasuali:

```c++
#include <cstdlib>
#include <exception>
#include <random>

#include "regression.hpp"

int main() {
  try {
    pf::Regression reg{};
    std::default_random_engine eng;
    ...
  }
  ... 
}   
```

Supponiamo di voler generare un insieme di punti che abbiano le coordinate distribuite secondo:

- una distribuzione uniforme di numeri reali compresi tra 0 e 10 per quanto riguarda la coordinata $x$;
- una distribuzione Gaussiana con media 1 e sigma 0.1 per quanto riguarda la coordinata $y$.

> [!NOTE]
> Questi punti descrivono una retta con pendenza compatibile con 0 ed intercetta compatibile con 1.

 Sarà sufficiente dichiarare due oggetti, uno di tipo [`std::uniform_real_distribution<double>`](https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution) e uno di tipo
 [`std::normal_distribution<double>`](https://en.cppreference.com/w/cpp/numeric/random/normal_distribution):

```c++
...
int main() {
  ...
    pf::Regression reg{};
    std::default_random_engine eng;
    std::uniform_real_distribution<double> dist_x{0.,10.};
    std::normal_distribution<double> dist_y{1., 0.1};
  ...
}    
```

Per aggiungere punti all'istanza della classe `Regression` possiamo inserire le chiamate al metodo `add` all'interno di un ciclo _for_. Per il momento cominciamo aggiungendo 5 punti:

```c++
...
int main() {
  ...
 const auto N = 5;
 for (auto i = 0; i != N; ++i) {
   reg.add({dist_x(eng), dist_y(eng)});
 }
  ...
}    
```

> [!IMPORTANT]
> Notare che un singolo `default_random_engine` viene utilizzato per tutte le generazioni.

Aggiungiamo infine una stampa a schermo dei coefficienti $A$ e $B$ calcolati tramite il
metodo `Regression::fit`:

```c++
#include "regression.hpp"

#include <random>
#include <iostream>

int main() {
  ...

  const auto result{reg.fit()};

  std::cout << "- A: " << result.A << '\n' << "- B: " << result.B << '\n';
}   
```

Salviamo e compiliamo il codice.

> [!IMPORTANT]
> Compilate utilizzando il _tool CMake_ e il file `CMakeLists.txt` che potete scaricare all'inizio di questo laboratorio. Potete trovare i vari comandi da eseguire [qui](https://github.com/Programmazione-per-la-Fisica/labs2024/blob/main/lab5/README.md#comandi-per-compilazione-e-test).

Una volta compilato il codice, eseguiamolo:

```bash
$  build/Debug/regression
- A: 1.11102
- B: -0.0230841
```

> [!NOTE]
> Molto probabilmente, otterrete valori diversi da quelli presentati qua.

Potrete notare che, utilizzando un campione di dati così limitato, non si
ottengono esattamente i valori attesi (1 e 0, rispettivamente).

> [!NOTE]
> Provate a eseguire il codice più volte senza cambiarlo.
>
> :question: Ottenete sempre lo stesso output su schermo? Come mai?

In realtà, la sequenza di numeri generata da un qualsiasi _random number
generator_ è completamente deterministica, per cui ci attendiamo il
comportamento appena ottenuto. È però possibile variare la risposta di un _random number generator_ andando ad
utilizzare _seed_ differenti durante l'inizializzazione.

In particolare,
[`std::random_device`](https://en.cppreference.com/w/cpp/numeric/random/random_device)
permette tipicamente di accedere ad alcune risorse _hardware_ per generare
sequenze di numeri interi "veramente" casuali distribuite in maniera uniforme.

Però, **l'accesso a tali risorse risulta dispendioso**, pertanto è prassi
utilizzare `std::random_device` solo per inizializzare il _seed_ di un _random
number generator (o engine)_.

Andiamo quindi a modificare il programma nel seguente modo:

```c++
...
int main() {
    pf::Regression reg{};
    std::random_device r;
    std::default_random_engine eng{r()};
    ...
}
```

Quindi compiliamo ed eseguiamo il codice più volte senza cambiarlo.

> [!NOTE]
> :question: Come cambia il comportamento del programma rispetto a prima?

Proviamo adesso a generare un milione di punti:

```c++
...
const auto N = 1000000;
for (auto i = 0; i != N; ++i) {
  reg.add(pf::Point{dist_x(eng), dist_y(eng)});
}
...
```

Compiliamo ed eseguiamo nuovamente il codice.

> [!NOTE]
> :question: Avete ottenuto dei valori di $A$ e $B$ dalla regressione lineare più vicini alle vostre attese?

## Utilizzo di `std::accumulate` per il calcolo delle somme parziali

Proviamo ora a sfruttare gli algoritmi e le _lambda expression_ per calcolare le varie somme necessarie per la determinazione dei coefficienti $A$ e $B$ della regressione lineare, che al momento sono determinate tramite un ciclo _for_ all'interno del metodo `fit`:

```c++
...
Result Regression::fit() const {
...
  double sum_x{};
  double sum_x2{};
  double sum_y{};
  double sum_xy{};
  for (auto const& p : points_) {
    sum_x += p.x;
    sum_x2 += p.x * p.x;
    sum_y += p.y;
    sum_xy += p.x * p.y;
  }
...
}
```

Per farlo, procediamo dichiarando un metodo basato sull'algoritmo `std::accumulate` (potete trovare la documentazione relativa a `std::accumulate` a questo [link](https://en.cppreference.com/w/cpp/algorithm/accumulate)), chiamato `fit_algo`, sempre in `regression.hpp`:

```c++
...
 public:
 ...
  Result fit() const;
  Result fit_algo() const;
...
};
```

> [!IMPORTANT]
> Ricordate di implementare tutti i test che ritenete necessari per il metodo `fit_algo`. Dato che questa funzione rappresenta un modo alternativo, ma equivalente, di eseguire i calcoli rispetto al metodo `fit`, potete confrontare i risultati ottenuti dalle due versioni per verificare la correttezza dell'implementazione di `fit_algo`.

Dopo aver implementato i test, continuiamo con la definizione del metodo nel corrispondente file `regression.cpp`:

```c++
#include <numeric>
...
double sum_x{
      std::accumulate(points_.begin(), points_.end(), 0., ...)};
double sum_x2{ ... };
double sum_y{ ... };
double sum_xy{ ... };
```

> [!TIP]  
> Al posto dei `...` all’interno di `std::accumulate`, dovrete inserire una _lambda expression_ che indichi all’algoritmo come accumulare correttamente i valori.

Una volta finito, **salvate, compilate ed eseguite i test**. Se risulteranno passati con successo, potrete utilizzare nel `main.cpp` la nuova funzione `fit_algo` al posto di `fit`.

### Il comando `time`

Vogliamo adesso chiederci se l'implementazione di `fit_algo` risulti migliore a livello di performance rispetto all'originale `fit`. Un modo molto semplice per fare ciò è utilizzare il comando `time`. Proviamo ad eseguirlo, anteponendolo al comando di esecuzione del nostro eseguibile:

```bash
$ time build/Release/regression
- A: 1.00029
- B: -3.43294e-05
...
real 0m1.969s
user 0m1.075s
sys  0m0.119s
```

> [!NOTE]
> I test di performance vanno sempre eseguiti compilando il codice in modalità _Release_.

Come potete vedere, sono comparse tre misure di tempo al di sotto del normale
output del nostro programma: `real`, `user` e `sys`. Quella più interessante
per i nostri scopi è `real`, cioè il tempo trascorso da quando abbiamo lanciato
il comando di esecuzione del codice a quando il nostro programma è terminato.

> [!IMPORTANT]
> Provate ad aumentare la quantità di dati generati di un fattore 10.
>
> :question: Il tempo indicato da `real` aumenta all'incirca nel modo atteso?

Proviamo ora a sostituire nel `main.cpp` l'utilizzo di `fit` con `fit_algo`:

```c++
...
//  const auto result{reg.fit()};
  const auto result{reg.fit_algo()};
...
```

In linea di principio, gli algoritmi della _standard library_ dovrebbero risultare più ottimizzati rispetto al nostro codice, quindi la versione con `fit_algo` dovrebbe risultare più performante. Ottenete i risultati attesi?

> [!IMPORTANT]
> In realtà, la versione di `fit_algo` implementata risulta essere leggermente più lenta rispetto a quella originale.
>
> :question: Sapreste intuire perché?

### Un utilizzo più efficiente di `std::accumulate`

Procediamo quindi a modificare il codice, per evitare cicli ridondanti e compiere tutte le somme in un unico passaggio tramite `std::accumulate`. Per fare ciò è necessario introdurre una nuova `struct` che contenga le somme parziali e sulla quale possiamo accumulare tutti i valori contemporaneamente sfruttando una _lambda expression_ all'interno del `std::accumulate`. Aggiungiamo una nuova `struct` chiamata `Sums` direttamente all'interno del metodo `fit_algo`:

```c++
Result Regression::fit_algo() const {
...
  struct Sums {
    double sum_x;
    double sum_x2;
    double sum_y;
    double sum_xy;
  };
...
}
```

e utilizziamola nell'implementazione del metodo `fit_algo`.
La _lambda expression_ da utilizzare dovrà soddisfare alcuni requisiti:

- saranno necessari due parametri: il valore accumulato fino a quel momento e il valore del punto all'iterazione corrente;
- il tipo di ritorno dovrà essere il nuovo valore accumulato.

```c++
  auto [sum_x, sum_x2, sum_y, sum_xy] =
      std::accumulate(points_.begin(), points_.end(), Sums{0., 0., 0., 0.},
                      [](..., ...) {
                        ...
                        ...
                        return ...;
                      });
```

> [!NOTE]
> Notate come abbiamo utilizzato lo _structured binding_ per dichiarare e inizializzare le variabili ai valori ritornati dall'algoritmo `std::accumulate`.

> [!TIP]
> Vi suggeriamo di controllare la [documentazione](https://en.cppreference.com/w/cpp/algorithm/accumulate) relativa a `std::accumulate` per capire quale delle definizioni è utilizzata in questo caso. In particolare, notate che la _lambda expression_ che dovrete utilizzare per specificare il comportamento dell'algoritmo corrisponde all'operatore `op` menzionato nella documentazione.

Una volta terminato, **salvate, compilate ed eseguite i test**. Valutate poi se questa implementazione risulti essere più o meno veloce di quella originale, utilizzando il comando `time`.

## Mediana di un campione di punti

Vogliamo adesso controllare che i punti generati siano distribuiti effettivamente secondo le distribuzioni utilizzate per la generazione. Un modo di farlo è quello di calcolare la mediana dell'insieme di punti.

Dato un campione di valori ordinabili, la [mediana](https://it.wikipedia.org/wiki/Mediana_(statistica))
(o valore mediano) si definisce come il valore per il quale la frequenza relativa
cumulata vale (o supera) 0.5.

All'atto pratico, la mediana di un campione costituito da $N$ valori è
calcolabile nel seguente modo:

1. si ordinano gli $N$ valori in ordine crescente;
2. se $N$ è dispari: la mediana corrisponde al valore centrale, che occupa la
   posizione $N/2$ del campione ordinato;
3. se $N$ è pari: la mediana è definita come la media aritmetica dei
   valori che occupano le posizioni $N/2 - 1$ ed $N/2$.

La prima cosa da fare è quindi ordinare il vettore di punti.

### Ordinamento di vettori con criteri arbitrari

Per ordinare il vettore di punti potremmo utilizzare l'algoritmo [`std::sort`](https://en.cppreference.com/w/cpp/algorithm/sort), ma l'ordinamento eseguito di default è crescente ed è implementato solo per tipi nativi.

È possibile però modificare il criterio di ordinamento dell'algoritmo `std::sort`, passando una funzione che ritorni un `bool` e che definisca un nuovo criterio.
Questa funzione può essere:

- una funzione libera;
- una _lambda expression_.

Ci sono vantaggi e svantaggi legati ad entrambe le scelte. Per il momento, scegliamo di utilizzare una funzione libera, in quanto i criteri di ordinamento saranno riutilizzanti diverse volte durante il calcolo delle mediane.

> [!IMPORTANT]
> Per maggiori informazioni sulle _lambda expression_ consultate la [pagina dedicata su cppreference](https://en.cppreference.com/w/cpp/language/lambda) e/o le slide presentate a lezione.

Implementiamo quindi due metodi per ordinare il vettore di punti secondo criteri diversi:

- il primo, `sort_by_x`, che ordini il vettore di punti in modo crescente a seconda del valore della coordinata $x$ di ciascun punto;
- il secondo, `sort_by_y`, che ordini il vettore di punti in modo crescente a seconda del valore della coordinata $y$ di ciascun punto.
  
Come sempre, prima di definire nuove funzionalità, è utile implementare i relativi test. Procediamo quindi con l'implementazione di nuovi test nel file `regression.test.cpp`:

```c++
TEST_CASE("Checking the sorting algorithms")
{
  pf::Regression reg;
  reg.add(2., 1.);
  reg.add(-2., 4.);
  reg.add(3., 2.);

  SUBCASE("Increasing order according to x")
  {
    reg.sort_by_x();
    REQUIRE(reg.size() == 3);

    std::vector<pf::Point> const expected{{-2., 4.}, {2., 1.}, {3., 2.}};
    CHECK(reg.points() == expected);
  }
  SUBCASE("Increasing order according to y")
  {
    reg.sort_by_y();
    REQUIRE(reg.size() == 3);

    std::vector<pf::Point> const expected{{2., 1.}, {3., 2.}, {-2., 4.}};
    CHECK(reg.points() == expected);
  }
}
```

> [!NOTE]
> Notate come in questo caso non sia necessario confrontare le coordinate dei punti con quelle attese tramite `doctest::Approx`, ma si possa direttamente confrontare il vettore di punti ordinato con quello atteso.

A questo punto, dobbiamo anche dichiarare e definire due funzioni libere che implementino i criteri di ordinamento necessari per il nostro caso.
Il file `regression.hpp` dovrà quindi contenere due nuove dichiarazioni all'interno della class `Regression`:

```c++
namespace pf {

...
class Regression {  
...
  public:
  ...
   void sort_by_x();
   void sort_by_y();
   ...
};
}
```

Procediamo adesso con la definizione dei nuovi metodi e delle nuove funzioni libere nel file `regression.cpp`:

```c++
#include "algorithm"
...

bool cmp_less_x(Point const& lhs, Point const& rhs)
{
  return ...;
}

bool cmp_less_y(Point const& lhs, Point const& rhs)
{
  return ...;
}

...

void Regression::sort_by_x()
{
  std::sort(points_.begin(), points_.end(), ...);
}

void Regression::sort_by_y()
{
  std::sort(points_.begin(), points_.end(), ...);
}
```

> [!NOTE]
> Notiamo che i due metodi che eseguiranno l'ordinamento non sono _const_. Perché?

Salviamo, compiliamo e controlliamo poi che i test definiti in `regression.test.cpp` siano soddisfatti.

### Calcolo della mediana

Adesso che abbiamo definito metodi in grado di ordinare un vettore di punti in modo crescente, possiamo procedere con il calcolo della mediana.
Come sempre, per prima cosa definiamo in `regression.test.cpp` i test che dovranno essere rispettati:

```c++
...

TEST_CASE("Checking the medians computation")
{
  pf::Regression reg;
  std::vector<pf::Point> points{
      {-3., 1.}, {1., 2.}, {3., 3.}, {2., -2}, {4., -1}};
  for (auto const& p : points) {
    reg.add(p);
  }

  SUBCASE("Checking the median with an odd number of points")
  {
    REQUIRE(reg.size() == 5);    
    CHECK(reg.median_x() == doctest::Approx(2.));
    CHECK(reg.median_y() == doctest::Approx(1.));
  }

  SUBCASE("Checking the median with an even number of points")
  {
    CHECK(reg.remove({-3., 1.}) == true);
    REQUIRE(reg.size() == 4);
    CHECK(reg.median_x() == doctest::Approx(2.5));
    CHECK(reg.median_y() == doctest::Approx(0.5));
  }
}
```

Notiamo come nell'implementazione proposta vengono utilizzati due metodi `median_x` e `median_y` che calcoleranno la mediana per entrambe le coordinate di un insieme di punti. I metodi dovranno ritornare un `double` che conterrà il valore delle mediane.
Procediamo con la dichiarazione dei nuovi metodi in `regression.hpp`:

```c++
class Regression
{
...
 public:
  ...
  double median_x();
  double median_y();
};
```

e con la loro implementazione in `regression.cpp`:

```c++
double Regression::median_x()
{
  const std::size_t N = ...;
  sort_by_x();
  auto& p_sorted = points();
  return ...;
}

double Regression::median_y()
{
  ...
}
```

Salviamo, compiliamo e controlliamo poi che i test definiti in `regression.test.cpp` siano soddisfatti.

### Un'implementazione più efficiente

Per calcolare la mediana, abbiamo dovuto ordinare gli elementi del vettore di punti.
Questa può risultare un'operazione particolarmente dispendiosa, specialmente se il vettore contiene molti elementi.

Nel caso del calcolo della mediana, è possibile ottenere lo stesso risultato sfruttando l'algoritmo [`std::nth_element`](https://en.cppreference.com/w/cpp/algorithm/nth_element) della _standard library_, che esegue solamente un ordinamento parziale del vettore. In breve, questo algoritmo riordina gli elementi in un _range_ `[first, last)` in modo tale che l'elemento puntato dall'iteratore `nth` è cambiato a qualsiasi elemento che si troverebbe in quella posizione se il _range_ fosse stato ordinato. Inoltre, tutti gli elementi che si trovano nel _range_ `[first, nth)` risultano essere minori (ma non necessariamente ordinati) dell'elemento `nth` e, analogamente, tutti gli elementi nel range `[nth, last)` risultano essere maggiori (ma non necessariamente ordinati) rispetto all'elemento `nth`.

> [!IMPORTANT]
> :question: Perché questo approccio risulta essere migliore rispetto al calcolo della mediana che sfrutta `std::sort`?
> Discutetene tra di voi e poi chiedete conferma ai tutor o ai docenti.

Procediamo quindi con l'implementazione di due nuovi metodi `median_x_nth` e `median_y_nth` che sfruttino `std::nth_element`. Per quello che riguarda i test, possiamo confrontare i risultati ottenuti usando `median_x` e `median_y` con quelli restituiti dalla nuova implementazione:

```c++
TEST_CASE("Checking the medians computation")
{
 ...

  SUBCASE("Checking the median with an odd number of points")
  {
    REQUIRE(reg.size() == 5);    
    CHECK(reg.median_x() == doctest::Approx(2.));
    CHECK(reg.median_y() == doctest::Approx(1.));
    CHECK(reg.median_x_nth() == reg.median_x());
    CHECK(reg.median_y_nth() == reg.median_y());
  }

  SUBCASE("Checking the median with an even number of points")
  {
    CHECK(reg.remove({-3., 1.}) == true);
    REQUIRE(reg.size() == 4);
    CHECK(reg.median_x() == doctest::Approx(2.5));
    CHECK(reg.median_y() == doctest::Approx(0.5));
    CHECK(reg.median_x_nth() == reg.median_x());
    CHECK(reg.median_y_nth() == reg.median_y());
  }
}
```

Come sempre dovremo procedere con la dichiarazione dei metodi in `regression.hpp`

```c++
class Regression
{
...
 public:
  ...
  double median_x_nth() const;
  double median_y_nth() const;
};
```

> [!NOTE]
> In questo caso, i due metodi sono _const_. Perché?

e con la loro ``definizione`` in `regression.cpp`

```c++
double Regression::median_x_nth() const
{
  const std::size_t N = size();
  auto p              = points();
  auto it             = p.begin() + N / 2;

  std::nth_element(p.begin(), ...);

  ...

  return ...;
}

double Regression::median_y_nth() const
{
  ...
}
```

> [!IMPORTANT]
> L'implementazione di questi metodi può essere eseguita utilizzando due volte `std::nth_element` nel caso di un vettore con un numero pari di elementi. In alternativa, vi invitiamo ad esplorare l'utilizzo di `std::nth_element` per identificare uno dei due elementi e di `std::max_element` (oppure `std::min_element`) per identificare il secondo elemento.

Salvate, compilate e controllate che i test siano soddisfatti.

A questo punto, possiamo stampare a schermo le mediane dei campioni generati all'interno di `main.cpp`:

```c++
...
int main() {
  ...
  const double med_x = reg.median_x_nth();
  const double med_y = reg.median_y_nth();
  std::cout << "- Median x: " << med_x << '\n';
  std::cout << "- Median y: " << med_y << '\n';
}
```

Salvate, compilate ed eseguite il codice:

```c++
$ build/Debug/regression
...
- Median x: 4.99891
- Median y: 0.999944
```

I risultati ottenuti sono in linea con le nostre aspettative.

> [!TIP]
> Vi suggeriamo di utilizzare il comando `time` per giudicare la differenza di prestazioni fra le due implementazioni del calcolo della mediana. Ricordate di compilare il codice in modalità _Release_ se sceglierete di seguire questo suggerimento.

## Lettura/scrittura da/su file

Supponiamo adesso di avere un campione di dati salvato su file e di volerlo
leggere per processarlo tramite la classe `Regression`. Possiamo utilizzare
`std::ifstream infile{"filename"}` per leggere da file dei valori, dove `"filename"`
rappresenta il _path_ del file che vogliamo leggere.

> [!NOTE]
> Ricordatevi di includere l'header file `fstream` per poter usare sia `std::ifstream` che `std::ofstream`.

> [!IMPORTANT]
> È buona norma controllare che l'apertura del file in lettura sia
> avvenuta con successo. Come fareste?

Create una directory chiamata `samples` all'interno della directory `lab6` e scaricate il file `data.txt` da questo link:

```bash
$ pwd
/home/fferrari/pf_labs/lab6
$ mkdir samples
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2024/main/lab6/samples/data.txt -o samples/data.txt
```

Questo file contiene due colonne di dati, che rappresentano le coordinate $x$ e $y$ di 10000 punti generati casualmente.

Procedete poi a leggerlo all'interno del vostro _main program_:

```c++
...
#include "fstream"
...
int main() {
    ...
    pf::Regression reg_file;
    std::ifstream infile{"../samples/data.txt"};
    ...
    }    
```

La lettura del file viene deliberatamente non implementata e viene lasciata a voi come esercizio. Una volta letto il file, utilizzate il metodo `fit_algo` per ottenere i valori di $A$ e $B$.

> [!NOTE]
> Se avete letto correttamente il file dovreste ottenere come valori di $A$ e $B$: 0.999567 e -0.000192749.

Procediamo adesso a scrivere i risultati ottenuti dal metodo `fit_algo` in
un file di output di nome `results.txt`. Ricordate che per scrivere su file
occorre usare `std::ofstream outfile{"filename"}`, dove `"filename"` rappresenta il _path_ del file che vogliamo produrre.

> [!IMPORTANT]
> È buona norma controllare che l'apertura del file in scrittura sia avvenuta con successo. Come fareste?

La scrittura del file viene deliberatamente non implementata e viene lasciata a voi come esercizio.

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
[questo link](https://virtuale.unibo.it/mod/assign/view.php?id=1652616), il
quale prevede il solo caricamento di file `.zip` o `.tgz`.

Supponendo che tutto il materiale sia nella cartella `lab6` (e supponendo di
trovarsi in tale cartella), per creare un archivio `.zip` procedere come segue:

```bash
$ pwd
/home/fferrari/pf_labs/lab6
$ cd ..
$ zip -r lab6.zip lab6
$ ls
lab6 lab6.zip
```

Per creare un archivio `.tgz` procedere invece come segue:

```bash
$ pwd
/home/fferrari/pf_labs/lab6
$ cd ..
$ tar czvf lab6.tgz lab6
$ ls
lab6 lab6.tgz
```

## Approfondimenti ed esercizi

Per chi fosse interessato a "sperimentare ulteriormente" gli argomenti
presentati in questo laboratorio, vengono proposti dei possibili approfondimenti
**opzionali**:

- Generare i numeri random tramite `std::generate_n` invece che tramite il ciclo _for_.
- Implementare un metodo `norm` che ritorni la norma di un punto.
- Implementate un metodo `find_max_el` che ritorni l'iteratore all'elemento del vettore con la norma più grande. Potrete implementarlo in un primo momento usando gli indici o gli iteratori e poi usando gli algoritmi della _standard library_.
- Implementate una funzione `sum_vectors_elem` che ritorni un vettore contenente la somma elemento per elemento delle norme dei punti contenuti in due istanze della classe `Regression`.  Potrete dapprima implementarla usando dei _loop_ espliciti e poi usando `std::transform`.

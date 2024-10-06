<!-- markdownlint-disable-file MD014 MD033 MD028 -->
# Laboratorio 1

In questa lezione impareremo alcuni concetti base della programmazione tramite il linguaggio _C++_.

---

- [Laboratorio 1](#laboratorio-1)
  - [Area di lavoro](#area-di-lavoro)
  - [Primi passi](#primi-passi)
  - [Esercizio 1: stampa di una stringa su schermo](#esercizio-1-stampa-di-una-stringa-su-schermo)
  - [Esercizio 2: operazioni matematiche e uso di variabili](#esercizio-2-operazioni-matematiche-e-uso-di-variabili)
  - [Esercizio 3: somma dei primi _n_ numeri naturali](#esercizio-3-somma-dei-primi-n-numeri-naturali)
    - [Bonus](#bonus)
  - [Approfondimenti ed esercizi](#approfondimenti-ed-esercizi)

---

Gli obiettivi di questo laboratorio sono:

- prendere confidenza con _VSCode_ e la compilazione di programmi con `g++`
- apprendere come utilizzare _clang-format_ per gestire la formattazione del codice
- impratichirsi con alcuni concetti di base della programmazione (input/output da terminale, variabili, operazioni tra
  numeri interi)

I programmi che scriveremo e testeremo oggi effettueranno:

1. la stampa di una stringa di testo su schermo
2. alcune operazioni matematiche tra numeri interi
3. il calcolo della somma dei primi _n_ numeri naturali

Durante il laboratorio vi invitiamo a tenere aperte le
[slide](https://github.com/Programmazione-per-la-Fisica/pf2024/releases/latest) presentate a lezione.

## Area di lavoro

Qualora non si sia già provveduto a farlo, suggeriamo di creare una directory (cartella) di lavoro (diversa da quella di
eventuali download locali delle [repository GitHub del corso](https://github.com/Programmazione-per-la-Fisica/)) dove
mantenere i programmi sviluppati durante i laboratori e lo svolgimento degli esercizi.
Salvo casi particolari, suggeriamo creare sottocartelle separate per ciascuno di essi.

Creiamo quindi una struttura di directory, che ospiterà tutti i file del nostro
progetto. La cartella del primo laboratorio (`lab1/`) inizialmente risulterà
vuota.

```bash
$ pwd
/home/battilan
$ mkdir -p pf_labs/lab1
$ cd pf_labs/lab1
$ ls -la
total 0
drwxr-xr-x  2 battilan  staff  64 27 Sep 10:04 .
drwxr-xr-x  3 battilan  staff  96 27 Sep 10:04 ..
```

> [!NOTE]
> La scelta della strutture del nome della directory `pf_labs` è totalmente arbitraria, potete scegliere liberamente un
> nome diverso in base alle vostre preferenze.
>
> :question: Il path `labs/lab1` è relativo o assoluto?
>
> :question: Ricordate a cosa servono le opzioni passate ai comandi `mkdir` ed `ls`?

> [!WARNING]
> D'ora in poi evitate di copiare e incollare testo dalla traccia del laboratorio, con l'eccezione dei **rari** casi
> in cui vi verrà suggerito esplicitamente di farlo.

> [!WARNING]
> Ricordate che il simbolo `$` rappresenta il prompt e non va digitato assieme al comando.

## Primi passi

Durante il corso utilizzeremo
[_clang-format_](https://www.kernel.org/doc/html/latest/translations/it_IT/process/clang-format.html), uno strumento che
permette di mantenere il codice formattato in modo uniforme (e coerente tra diversi sviluppatori).
L'uso di standard per la formattazione del codice verrà discusso più estensivamente a lezione, per ora ci limitiamo a
generare un configurazione di default (`.clang-format`).

Tale file può essere generato utilizzando il comando `clang-format`, e specificando l'opzione `--dump-config`, come
segue:

```bash
$ clang-format --dump-config -style=google > .clang-format
$ cat .clang-format
---
Language:        Cpp
# BasedOnStyle:  Google
AccessModifierOffset: -1
AlignAfterOpenBracket: Align
AlignConsecutiveMacros: None
AlignConsecutiveAssignments: None
AlignConsecutiveBitFields: None
AlignConsecutiveDeclarations: None
AlignEscapedNewlines: Left
AlignOperands:   Align
...
```

Esistono molti stili di formattazione; qui si è scelto di partire dalla versione usata da _Google_ per il proprio
codice.

> [!NOTE]
> La sintassi `> .clang-format` permette di scrivere l'output del comando `clang-format` direttamente nel file
> `.clang-format`, invece di visualizzarlo a schermo. Il termine tecnico per questa operazione è
> [_redirezione dello standard output_](https://ryanstutorials.net/linuxtutorial/piping.php).

Per verificare la creazione di questo file, digitiamo sul terminale:

```bash
$ ls -la
drwxr-xr-x  5 battilan  staff   160 27 Sep 09:45 .
drwxr-xr-x  3 battilan  staff    96 27 Sep 09:30 ..
-rw-r--r--  1 battilan  staff  6098 27 Sep 09:29 .clang-format
```

Per verificare il funzionamento di _clang-format_ creiamo, tramite VSCode, lo scheletro del primo programma da
sviluppare durante questi laboratori.

Innanzitutto, dall'interno della nostra directory di lavoro, apriamo VSCode:

```bash
$ pwd
/home/battilan/pf_labs/lab1
$ code .
```

> [!IMPORTANT]
> **Il comando** `code .` (digitato in questo modo all'interno della cartella di lavoro) ci permette di gestirne tutto
> il contenuto come un unico progetto, quindi è **da preferire rispetto all'apertura di singoli file**.

Poi creiamo un nuovo file vuoto (es. per ora utilizziamo l'interfaccia grafica, cioè clicchiamo sull'iconcina a "forma di foglio" in alto a sinistra nell'interfaccia di VSCode e poi su quella "a forma di foglio con un +"), e salviamolo col nome `hello.cpp`.

> [!NOTE]
> Qualora non l'aveste già fatto, probabilmente VSCode vi suggerirà di installare l'_extension pack_ per `C++`. Vi
> raccomandiamo di farlo.

Immettiamo poi nel file `hello.cpp` il seguente _code snippet_ (frammento di codice):

```c++
int main() 
{



  }
```

Questo codice contiene una funzione `main()` che esegue il le istruzioni inserite tra la parentesi graffe (per ora
niente).
Salviamo di nuovo il file ed eseguiamo nel terminale il seguente comando:

```bash
$ clang-format -i hello.cpp
```

Se tutto ha funzionato correttamente, il codice dovrebbe ora risultare
formattato come segue:

```c++
int main() {}
```

Utilizzando l'opzione `-i`, abbiamo chiesto a `clang-format` di effettuare le modifiche di formattazione direttamente al
file `hello.cpp`.

> [!NOTE]
> :question: Cosa sarebbe successo se non avessimo specificato l'opzione `-i`?

Quando si utilizza VSCode, è possibile formattare il codice C++ sulla base del file `.clang-format` presente all'interno
dell'area di lavoro eseguendo il comando _Format Document_:

- su Linux il comando è associato ai tasti di scelta rapida <kbd>Ctrl</kbd> +
  <kbd>Shift</kbd> + <kbd>I</kbd>;
- su Windows il comando è associato ai tasti di scelta rapida <kbd>Alt</kbd> +
  <kbd>Shift</kbd> + <kbd>F</kbd>;
- su macOS il comando è associato ai tasti di scelta rapida <kbd>Option</kbd> +
  <kbd>Shift</kbd> + <kbd>F</kbd>.

> [!NOTE]
> Verificate rapidamente l'utilizzo di clang-format tramite VSCode, modificando il file `hello.cpp` e utilizzando ai
> tasti di scelta rapida riportati sopra.

> [!IMPORTANT]
> Come molti altri programmi, VSCode offre un'ampia gamma di ai tasti di scelta rapida. È opportuno imparare, sin dal
> principio, ad utilizzare quelli più comuni. È possibile scaricare un file `.pdf` che ne contiene una lista, dalla
> _Guida_ (_Help_) di VSCode, cliccando su  _Riferimento per tasti di scelta rapida_ (_Keyboard Shortcuts Reference_).

Prima di procedere oltre, compiliamo lo scheletro del programma `hello.cpp` (per ora minimale), tramite il seguente
comando:

```bash
$ g++ -Wall -Wextra hello.cpp -o hello
```

> [!TIP]
> Ricordiamo che chi utilizza macOS (e ha installato lo `gcc` tramite `brew`) dovrà avvalersi del comando `g++-14`
> invece di `g++`.
>
> :exclamation: Utilizzate il tasto <kbd>TAB</kbd> per completare la digitazione del comando.

> [!IMPORTANT]
> Le opzioni `-Wall` e `-Wextra` abilitano messaggi di _warning_ opzionali che aiutano ad evidenziare problemi nel
> codice.

## Esercizio 1: stampa di una stringa su schermo

In questo esercizio stamperemo su schermo una stringa di testo.

Per farlo modifichiamo il programma `hello.cpp` aggiungendo all'interno delle parentesi graffe della funzione `main()`
la seguente linea di codice:

```c++
cout << "Hello world!\n";
```

Formattiamo il codice con _clang-format_ **usando i tasti di scelta rapida**, e compiliamo:

```bash
$ g++ -Wall -Wextra hello.cpp -o hello
hello.cpp: In function 'int main()':
hello.cpp:1:14: error: 'cout' was not declared in this scope
    1 | int main() { cout << "Hello world!\n"; }
      |              ^~~~
```

La compilazione non è andata a buon fine e sullo schermo compare un errore che dice che `cout` non risulta dichiarato
in questo ambito.
Questo significa il programma non sa dove andare trovare `cout` quindi non sa quali azioni questo debba compiere.
L'oggetto `cout` fa parte della _standard library_ che verrà discussa estensivamente durante il corso.
Per dire al programma di cercare `cout` all'interno della _standard library_ dobbiamo aggiungere `std::` davanti
all'oggetto.

> [!NOTE]
> Se aprite il pannello dei problemi di Visual Studio Code (`View -> Problems`) potrete trovare un breve sommario delle
> cose che sono sbagliate nel vostro codice. In particolare in questo caso troverete:
>
> ```sh
> identifier "cout" is undefined
> ```

> [!NOTE]
> Visual Studio Code è un editor molto potente, infatti potete notare che alcune linee rosse al di sotto di parti di
> codice che probabilmente sono sbagliate.
>
> :exclamation: Attenti però, a volte anche Visual Studio Code potrebbe sbagliare, l'ultima parola spetta sempre al
> compilatore!

Provando a compilare ancora otterremo di nuovo un errore:

```bash
 $ g++ -Wall -Wextra hello.cpp -o hello
hello.cpp: In function 'int main()':
hello.cpp:5:6: error: 'cout' is not a member of 'std'
    5 | std::cout << "Hello world!\n";
      |      ^~~~
hello.cpp:1:1: note: 'std::cout' is defined in header '<iostream>'; did you forget to '#include <iostream>'?
  +++ |+#include <iostream>
    1 | //#include <iostream>
```

In questo caso l'errore è molto esplicativo: il compilatore ci sta dicendo che l'oggetto `cout` della _standard library_
è definito in un (_header_) file chiamato `iostream`. Per poterlo usare dovremo quindi includere tale _header file_
all'interno del nostro programma.
Possiamo farlo utilizzando la direttiva `#include` che dovrà essere inserita prima del `main()`:

```c++
#include <iostream>

int main()
{
  std::cout << "Hello world!\n";
}
```

A questo punto la compilazione dovrebbe andare a buon fine. Per eseguire il programma possiamo lanciare il comando
`./hello` dal terminale:

```bash
$ ./hello 
Hello world!
```

> [!IMPORTANT]
> Notate come abbiamo anteposto il `.` (che indica la directory in cui ci troviamo) al fine di eseguire del comando.
> Questo è necessario perché la _command line_, di default, va a cercare i comandi che le chiediamo di utilizzare solo
> all'interno di una serie di cartelle create appositamente per contenere eseguibili.

> [!TIP]
> Provate ad utilizzare il comando `file`, discusso durante il secondo pre-laboratorio, per verificare quanto prodotto
> dal compilatore:
>
> ```bash
> $ file hello
> hello: ELF 64-bit LSB pie executable, ARM aarch64, version 1 (SYSV), dynamically linked, interpreter
> /lib/ld-linux-aarch64.so.1, BuildID[sha1]=f0b2388b532e79fe2b84dacb90d7b8e959a3cff0, for GNU/Linux 3.7.0, not stripped
> ```
>
> :exclamation: I dettagli relativi all'output del comando appena eseguito dipendono dal processore e da altre
> caratteristiche del sistema che ciascuno di voi utilizza, pertanto potrebbero variare. L'importante è verificare che
> il programma compilato sia _eseguibile_.

In molti casi è conveniente poter passare informazioni come input al programma durante la sua esecuzione. Modifichiamo
il programma per fare sì che stampi su schermo una stringa che viene data in input:

```c++
#include <iosteram>

int main()
{
  std::cout << "Insert your name:\n";
  std::string name
  cin >> name;
  std::cout << "Hello " << name << '\n':
}
```

> [!CAUTION]
> :exclamation: Il programma non compila! Analizzate gli errori prodotti dalla compilazione del programma e cercate di
> risolverli.

Alla fine dei turni del primo laboratorio, potrete trovare la soluzione dell'esercizio 1
[al seguente link](soluzioni/hello.cpp).

## Esercizio 2: operazioni matematiche e uso di variabili

In questo secondo esercizio effettueremo alcune operazioni su di una coppia di numeri immessi come input da tastiera,
implementando un nuovo programma, che chiameremo `domath.cpp`.

Cominciamo con il calcolare la somma di due numeri interi dati in input al programma e stampare il valore risultate su
schermo:

```c++
#include <iostream>

int main() {

  std::cout << "Provide the first number\n";
  int i;
  std::cin >> i;
  std::cout << "Provide the second number\n";
  int j;
  std::cin >> j;
  int sum{i + j};
  std::cout << "The sum is: " << sum << '\n';
}
```

Dopo aver formattato il codice, compiliamo il programma ed eseguiamolo:

```bash
$ g++ -Wall -Wextra domath.cpp -o domath
$ ./domath
Provide the first number 2
Provide the second number 2
The sum is: 4
```

Una volta implementato questa prima versione del programma, passiamo ad un'altra operazione. Supponete di voler
controllare se il primo numero intero inserito come input al programma sia un multiplo del secondo numero intero
inserito.
Come fareste?

> [!TIP]
> Esiste un [operatore aritmetico](https://en.cppreference.com/w/cpp/language/operator_arithmetic) del C++ che fa al
> caso vostro.

Alla fine dei turni del primo laboratorio, potrete trovare la soluzione dell'esercizio 2
[al seguente link](soluzioni/domath.cpp).

## Esercizio 3: somma dei primi _n_ numeri naturali

Scrivete un programma `sum_n.cpp` che calcoli la somma dei primi _n_ numeri naturali positivi dato l'estremo superiore
come input.

> [!TIP]
> Esiste una formula nota che permette di calcolare tale sommatoria, quale?

### Bonus

Una volta fatto questo, estendete la funzionalità del programma per calcolare la somma dei numeri naturali compresi in
un intervalli arbitrario _[a,b]_.

Alla fine dei turni del primo laboratorio, potrete trovare la soluzione dell'esercizio 3
[al seguente link](soluzioni/sum_n.cpp).
Inoltre potete trovare la soluzione dell'esercizio bonus [qui](soluzioni/sum_a_b.cpp).

## Approfondimenti ed esercizi

In generale è buona norma prendere familiarità, il prima possibile, con i messaggi di errore del compilatore,
**abituandosi a leggerne la sintassi ed a riconoscere il tipo di errore che ci si presenta**.

Vi suggeriamo di partire dal programma
dell'[esercizio 1](#esercizio-1-stampa-di-una-stringa-su-schermo) e introdurvi alcuni errori.

Una lista (non esaustiva) potrebbe essere:

- inserire uno spazio aggiuntivo tra `<` e `iostream`;
- tralasciare i caratteri `<` e `>` dopo il termine `include`;
- tralasciare `int` prima del termine `main`;
- scrivere in maniera errata il termine `main`;
- tralasciare la parentesi `)` dopo main;
- tralasciare entrambe le parentesi `()`;
- tralasciare uno o più `;` alla fine delle linee di codice;
- tralasciare la parentesi graffa chiusa `}`;

> [!TIP]
> A cosa corrispondono i numeri in testa alle varie linee di errore?

> [!TIP]
> Un semplice errore di sintassi può produrre una lunga catena di messaggi dal compilatore, potrebbe essere necessario
> scorrere il terminale per leggere tutto l'output (<kbd>Control</kbd> + <kbd>Shift</kbd> + <kbd>↑</kbd> e
> <kbd>Control</kbd> + <kbd>Shift</kbd> + <kbd>↓</kbd>).

Un altra attività opzionale, ma decisamente utile, è quella di imparare a usare i _package manager_ installati in WSL e
mac OS: rispettivamente `apt` e `brew`.

Quello che vi suggeriamo di fare è provare a installare il programma `tree`, che permette di visualizzare la
struttura _ad albero_ di file e directory a partire da una data cartella.

> [!TIP]
> Prima di installare il programma vi consigliamo di provare i comandi: `apt search tree` e `apt info tree` (o
> `brew search tree` e `brew info tree` per chi usa mac OS) per imparare come trovare un determinato pacchetto e
> capirne il contenuto.

Potete farlo eseguendo i comandi:

**WSL**:

```bash
$ sudo apt install tree
```

**mac OS**:

```zsh
% brew install tree
```

Poi verificate il contenuto della cartella creata per contenere tutti i laboratori:

```bash
$ tree labs
├── labs
    └── lab1
        ├── domath
        ├── domath.cpp
        ├── hello
        ├── hello.cpp
        ├── sum_a_b
        ├── sum_a_b.cpp
        ├── sum_n
        └── sum_n.cpp
```

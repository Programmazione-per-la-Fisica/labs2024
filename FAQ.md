# Frequently Asked Questions (FAQs)

Quella che segue è una lista delle domande di carattere logistico che risultano più frequenti riguardo ai laboratori di
_[Programmazione per la Fisica](https://github.com/Programmazione-per-la-Fisica/pf2024)_.

- [Frequently Asked Questions (FAQs)](#frequently-asked-questions-faqs)
  - [I laboratori sono obbligatori?](#i-laboratori-sono-obbligatori)
  - [Gli elaborati dei laboratori verranno valutati ai fini dell'esame?](#gli-elaborati-dei-laboratori-verranno-valutati-ai-fini-dellesame)
  - [Posso scegliere io a quale turno di laboratorio partecipare?](#posso-scegliere-io-a-quale-turno-di-laboratorio-partecipare)
  - [Come devo comportarmi per segnalare uno scambio di turno?](#come-devo-comportarmi-per-segnalare-uno-scambio-di-turno)
  - [Come devo comportarmi in caso debba saltare un laboratorio?](#come-devo-comportarmi-in-caso-debba-saltare-un-laboratorio)
  - [Esiste una lista di _motivi validi_ per una eventuale assenza?](#esiste-una-lista-di-motivi-validi-per-una-eventuale-assenza)
  - [Come faccio a caricare su Virtuale l'elaborato relativo ad un dato laboratorio?](#come-faccio-a-caricare-su-virtuale-lelaborato-relativo-ad-un-dato-laboratorio)
  - [Gli elaborati dei laboratori consegnati su Virtuale verranno valutati ai fini dell'esame?](#gli-elaborati-dei-laboratori-consegnati-su-virtuale-verranno-valutati-ai-fini-dellesame)
  - [Quanto tempo ho per _recuperare_ un'eventuale assenza?](#quanto-tempo-ho-per-recuperare-uneventuale-assenza)
  - [Posso seguire i laboratori più volte e/o durante un anno di corso diverso dal primo?](#posso-seguire-i-laboratori-più-volte-eo-durante-un-anno-di-corso-diverso-dal-primo)

## I laboratori sono obbligatori?

**Sì**.
I laboratori vanno seguiti _per intero_ almeno una volta prima di sostenere l'esame. Si consiglia vivamente di seguirli
durante il primo anno, quando si seguono le lezioni frontali.

## Gli elaborati dei laboratori verranno valutati ai fini dell'esame?

**No**.
Detto questo _le tracce di laboratorio sono da considerarsi parte del programma_ per cui:

- il lavoro svolto durante i laboratori va portato all'esame, a dimostrazione dell’effettivo svolgimento;
- durante l'esame i docenti possono fare domande inerenti alle tracce delle attività di laboratorio.

## Posso scegliere io a quale turno di laboratorio partecipare?

**In generale no**, un sistema di alternanza dei turni è utilizzato per garantire che tutti gli studenti possano
usufruire parimenti a quelli più "convenienti" (es. tarda mattinata e primo pomeriggio).

Detto questo, sono **ammesse eccezioni** per studenti lavoratori, studenti che frequentano anni diversi dal primo ed
[emergenze _ben motivate_](#esiste-una-lista-di-motivi-validi-per-una-eventuale-assenza) (es. visite mediche, etc ...).

Infine è sempre possibile **effettuare uno scambio di turno** con una studentessa o uno studente del proprio canale, che
sia disposta/o ad accordarsi per questa operazione.

## Come devo comportarmi per segnalare uno scambio di turno?

Innanzitutto lo scambio va richiesto inviando una e-mail (dal proprio indirizzo `@studio.unibo.it`) a
[tutti i docenti](https://github.com/Programmazione-per-la-Fisica#docenti) e mettendo in copia l'altra studentessa o
l'altro studente interessata/o.

Inoltre, si chiede **a entrambe le persone coinvolte nello scambio** di segnalarlo esplicitamente in fase di iscrizione
su `https://studenti.unibo.it/sol/` **aggiungendo un commento** all'iscrizione.

> [!IMPORTANT]
> In caso di **scambi ripetuti**, ad esempio se ci si accorda per partecipare al medesimo turno per un intero semestre,
> chiediamo gentilmente di **aggiungere il commento tutte le volte** che ci si iscrive ad un laboratorio per il quale si
> effettua lo scambio.

## Come devo comportarmi in caso debba saltare un laboratorio?

In primo luogo, menziona la cosa inviando una e-mai (dal tuo indirizzo `@studio.unibo.it`) a
[tutti i docenti](https://github.com/Programmazione-per-la-Fisica#docenti) menzionando un
[_motivo valido_](#esiste-una-lista-di-motivi-validi-per-una-eventuale-assenza) per la tua assenza.

Una volta specificata la ragione della tua assenza, sarà appannaggio dei docenti confermare la validità della
giustificazione e specificarti come recuperare, ad esempio richiedendoti di seguire individualmente la traccia del
laboratorio e consegnare l'elaborato, anche parziale, del lavoro svolto in autonomia.

## Esiste una lista di _motivi validi_ per una eventuale assenza?

**Sono _motivi validi_**, ad esempio: problemi di salute, lutti, viste mediche, attività lavorative comprovate, attività
sportive agonistiche di alto livello, ulteriori corsi universitari o equiparati (es.: conservatorio).

**Non sono _motivi validi_**: attività sportive dilettantistiche, corsi seguiti per interesse personale, pendolarismo.

In caso di dubbio in merito alla validità di una data assenza, invia una e-mai (dal tuo indirizzo `@studio.unibo.it`) a
[tutti i docenti](https://github.com/Programmazione-per-la-Fisica#docenti) descrivendo il problema intercirso.

## Come faccio a caricare su Virtuale l'elaborato relativo ad un dato laboratorio?

La consegna deve avvenire, da parte dei singoli studenti, caricando l'elaborato  
[alla pagina su Virtuale relativa al laboratorio per il quale si effettua la consegna](https://virtuale.unibo.it/mod/assign/view.php?id=1368319).

La consegna prevede il solo caricamento di file `.zip` o `.tgz`.

> [!NOTE]
> Supponendo che tutto il materiale sia nella cartella `labX` (e supponendo di trovarsi in tale cartella), per creare un
> archivio `.zip` procedere come segue:
>
> ```bash
> $ pwd
> /home/fferrari/pf_labs/labX
> $ cd ..
> $ zip -r labX.zip labX
> $ ls
> labX labX.zip
> ```
>
> Per creare un archivio `.tgz` procedere invece come segue:
>
> ```bash
> $ pwd
> /home/fferrari/pf_labs/lab2
> $ cd ..
> $ tar czvf labX.tgz labX
> $ ls
> labX labX.tgz
> ```

Coloro che effettuano una consegna facoltativa, sono pregati di **riportare**, **come commento** alla consegna
stessa, **dubbi o domande sull'elaborato per i quali è richiesto feedback** esplicito da parte dei docenti.

## Gli elaborati dei laboratori consegnati su Virtuale verranno valutati ai fini dell'esame?

**No**, la consegna ha l'unico scopo di chiarire eventuali dubbi di studentesse e studenti. Restano comunque valide le
regole descritte [sopra](#gli-elaborati-dei-laboratori-verranno-valutati-ai-fini-dellesame).

## Quanto tempo ho per _recuperare_ un'eventuale assenza?

Il tempo tipicamente concesso è di **una settimana** dalla **data in cui si è svolto il laboratorio** per il quale si
risulta assenti.

Questo permette di rendere disponibili "senza troppo ritardo" le soluzioni del laboratorio al resto della classe.
**Qualora ciò non fosse possibile**, ad esempio in caso di un problema di salute che perdura oltre una settimana, si
chiede di **segnalarlo il prima possibile ai docenti**.

## Posso seguire i laboratori più volte e/o durante un anno di corso diverso dal primo?

**In generale sì**. Uno studente può seguire più volte i laboratori premesso che:

- sia correttamente iscritto ad un corso di laurea/dottorato che includa il corso nel piano di studi;
- rimanga posto in un dato turno, dopo che le esigenze degli studenti che seguono il laboratorio "in corso" sono state
  soddisfatte.

Per segnalare l'interesse a seguire i laboratori  più di una volta invia una e-mail (dall'indirizzo `@studio.unibo.it`)
a [tutti i docenti](https://github.com/Programmazione-per-la-Fisica#docenti).

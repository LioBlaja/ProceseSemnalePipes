# ProceseSemnalePipes
A set of solved problems in C

# Probleme cu procese, semnale și pipe

## Cerințe pentru finalizare

### 1. Program C pentru numărarea literelor mici din fișiere
Scrie un program C care primește ca argument în linie de comandă calea către un director de pe disc. Programul va parcurge directorul, va procesa în paralel conținutul tuturor fișierelor obișnuite identificate și va calcula numărul total de litere mici din acestea. Fiecare fișier obișnuit identificat va fi procesat într-un proces copil care va apela `exit()` cu numărul de litere mici pentru fișierul curent. Părintele va prelua informațiile folosind `wait()`, `WIFEXITED()` și `WEXITSTATUS()`.

### 2. Program C pentru compilarea fișierelor `.c`
Scrie un program C care primește ca argument în linie de comandă calea unui director. Programul va parcurge directorul și va procesa în paralel fișierele obișnuite cu extensia `.c`, invocând `gcc` pentru a le compila. Fiecare fișier `.c` va fi procesat într-un proces copil care va folosi `execlp()` sau `execvp()` pentru a invoca `gcc`.

### 3. Program pentru generarea și scrierea parolelor
Scrie un program C care generează parole și le scrie într-un fișier. Programul va primi ca argumente: numărul de procese, numărul de parole per proces, dimensiunea unei parole și calea fișierului de ieșire. Fiecare proces va genera parole și le va trimite prin pipe către procesul părinte, care le va colecta și scrie în fișier. Părintele va afisa un mesaj pentru fiecare proces fiu care s-a terminat.

### 4. Program C pentru semnalul SIGUSR1
Scrie un program C care își definește un comportament nou la recepția semnalului `SIGUSR1`. Programul va afișa un mesaj de fiecare dată când semnalul este primit. Testează-l trimițând semnalul dintr-un alt terminal folosind `ps aux` pentru a obține PID-ul și `kill` pentru a trimite semnalul.

### 5. Program C cu proces părinte și copil care trimite semnale
Scrie un program C cu două procese înrudite:
- Procesul copil va seta un comportament nou la recepția semnalului `SIGUSR1` și va termina execuția.
- Procesul părinte va trimite semnalul `SIGUSR1` către procesul copil și va prelua starea acestuia.

### 6. Program C pentru procesarea unui fișier text cu pipe
Scrie un program C cu două procese înrudite:
- Procesul copil va citi un fișier text primit ca argument și va trimite doar vocalele procesului părinte printr-un pipe.
- Procesul părinte va număra câți octeți a primit prin pipe, va afișa acest număr și va prelua starea procesului copil.

### 7. Program C care folosește pipe pentru comanda "ls -l"
Scrie un program C cu două procese înrudite:
- Procesul copil va invoca comanda `ls -l` și va transmite informațiile procesului părinte printr-un pipe.
- Procesul părinte va număra câți octeți a primit prin pipe, va afișa acest număr și va prelua starea procesului copil.

### 8. Program C pentru procesarea fișierelor `.c` și `.dat` într-un director
Scrie un program C care poate fi apelat astfel:

./program <directory> <letter> <number> <output_file_path>

- `<directory>` reprezintă calea către un director care conține fișiere obișnuite, legături simbolice și subdirectoare.
- `<letter>` este o literă specificată ca argument.
- `<number>` este un număr în format zecimal.
- `<output_file_path>` este calea către fișierul de ieșire (dacă nu există, va fi creat, dacă există, conținutul va fi șters).

Programul va parcurge directorul dat și va identifica toate fișierele obișnuite și legăturile simbolice. Pentru fiecare fișier obișnuit:
- Dacă fișierul are extensia `.c`, va invoca `gcc` pentru a-l compila.
- Dacă fișierul are extensia `.dat`, va procesa câmpurile sale în format binar. Dacă primul byte este `0x55`, va trimite semnalul `SIGUSR1` către părinte dacă numărul identificat este mai mic decât `<number>`. Dacă primul byte este `0x98`, va trimite semnalul `SIGUSR2` dacă litera dată ca argument se găsește în textul de 4 litere.

Procesul părinte va număra câte numere sunt mai mici decât `<number>` și câte litere sunt identice cu `<letter>`. Aceste informații vor fi afișate la fiecare 3 secunde folosind semnalul `SIGALRM`. La primirea semnalului `SIGTERM`, procesul părinte se va opri și va închide fișierele.

### Informații utile:
- Pentru a vedea procesele ce rulează pe sistem la un moment dat, se poate folosi: `ps aux`
- Pentru a trimite un semnal, se poate folosi: `kill`
- Pentru a seta un comportament nou la recepția unui semnal, se recomandă: `sigaction`

Pagini de manual utile: `signal(7)`, `inode(7)`

**Exemplu de testare:**
Pentru testare, se poate descărca arhiva ce conține o structură arborescentă de directoare și fișiere conform enunțului:

wget https://staff.cs.upt.ro/~valy/so/testso_dat_c.tar.gz

Arhiva se poate dezarhiva folosind comanda:
tar xf testso_dat_c.tar.gz





# Tema 2 SDA

> Autor: Trifan Bogdan-Cristian (312CD)

## Structuri de Date

Fisierul `utils.h` definește structurile esențiale pentru proiect:
- `PPM_Image`: reprezintă imaginea originală în format PPM
- `Coada`: implementată ca listă simplu înlănțuită, folosită pentru parcurgerea arborilor
- `Color`: reține canalele RGB ale unui pixel
- `QuadTree`: arborele cuaternar pentru compresia imaginii, conținând:
  - Pointeri către cei patru copii
  - Pointer către `color`, stochează valorile RGB pentru nodurile frunză (`NULL` pentru nodurile intermediare)
  - Coordonatele colțului stânga-sus și dimensiunea submatricii reprezentate de nod


```c
typedef struct TreeNode {
    unsigned int x, y, grid_size;          // Grid info
    Color *color;
    struct TreeNode *child_upper_left;     // Child 1
    struct TreeNode *child_upper_right;    // Child 2
    struct TreeNode *child_lower_right;    // Chlid 3
    struct TreeNode *child_lower_left;     // Child 4
} QuadTree;
```


## 🌳 Construirea arborerului cuaternar pentru compresie

```c
QuadTree *compress_image(PPM_Image *img, int factor);
```

Funcția de compresie împarte succesiv imaginea în câte patru blocuri,
continuând divizarea până când scorul de similaritate
pentru fiecare submatrice este mai mic sau egal cu factorul specificat in linia de comanda.

**Pașii algoritmului**:

1. Se creează radacina arborelui, reprezentând întreaga imagine
2. Radacina se adaugă în coadă
3. Atâta timp cât coada nu este goală:
   - Se extrage primul nod
   - Se calculează scorul de similaritate pentru blocul respectiv
   - Dacă scorul este mai mic sau egal cu factorul, nodul devine frunză și i se alocă memorie pentru `color`
		(se calculează culoarea medie a blocului - apel la funcția `avg_block_color()`)
   - Dacă scorul este mai mare decât factorul, nodul se extinde, creându-se cei patru copii, care se adaugă în coadă



```c
node->child_upper_left  = new_tree_node(x         , y         , size/2);
node->child_upper_right = new_tree_node(x         , y + size/2, size/2);
node->child_lower_right = new_tree_node(x + size/2, y + size/2, size/2);
node->child_lower_left  = new_tree_node(x + size/2, y         , size/2); 

queue_push(&queue, node->child_upper_left);
queue_push(&queue, node->child_upper_right);
queue_push(&queue, node->child_lower_right);
queue_push(&queue, node->child_lower_left);
```

**Observații utile:**
- **Flip image**: Swap-ul între nodurile left și right la același nivel răstoarnă imaginea.
- **Rotate image**Modificarea ordinii de inserție a copiilor în coadă poate roti imaginea.
- Un nod este frunză dacă toți copiii săi sunt `NULL`.



## 📐 Task 1: statistici asupra imaginii comprimate

Prima cerință presupune citirea imaginii PPM și construirea arborelui de compresie,
în funcție de care se determină:
- Numărul de niveluri (înălțimea) al arborelui
- Numărul de frunze
- Dimensiunea maximă a laturii pe care un nod frunză o reține

Rezultatele se vor scrie intr-un fisier text.


**Observații**:
- Înălțimea arborelui se calculează recursiv, arborele fiind parcurs în adâncime (**BFS**)
- Numărul frunzelor și dimensiunea maximă a blocului se determină simultan, folosind o singură parcurgere în lățime (**BFS**)



## 🗂️ Task 2: generarea fișierului comprimat

Similar cu cerința precedentă, mai întâi se încarcă în memorie imaginea originală
și se construiește arborele cuaternar de compresie.

Generarea fișierului comprimat presupune o parcurgere în lătime (**BFS**) a arborelui.
Aceasta folosește în spate o coadă:
- Nod intern: se scrie caracterul `0` și se adaugă copii în coadă
- Nod frunză: se scrie `1` și valorile RGB ale nodului

 


## 📂 Task 3: decompresie

Decompresia reconstruiește imaginea originală pe baza fișierul comprimat.

Imaginea se reconstruieste pe măsură ce nodurile sunt citite și procesate,
folosind o coadă pentru a păstra relațiile părinte-copil.

**Pașii algoritmului**:

1. Se creează radacina arborelui:
  aceasta referă întreaga imagine,
  iar momentan nu conține informație utilă
2. Radacina se adaugă în coadă
3. Cât timp coada conține elemente:
   - Se extrage primul nod
   - Se citește tipul nodului (`0` sau `1`):  
     - `0`: nod intern => se creează cei 4 copii și se adaugă în coadă.  
     - `1`: frunză => se citesc valorile RGB și se atribuie nodului.  


La final, imaginea reconstruită se scrie într-un fișier binar.


## 💾 Input/Output

A fost o adevărată provocare să ghicesc formatul PPM - 
documentația era neclară și am pierdut ceva timp
încercând să înțeleg exact cum să citesc și să scriu datele.
o situatie posibila la viitoare proiecte sau la un loc de munca 👨‍💼💼.

Din fericire, enunțul a oferit indicii despre tipurile de date care trebuiau folosite,
ceea ce a salvat mult timp.


- Atât imaginile PPM, cât și fișierele comprimate sunt binare
- Antetul PPM (text ASCII) se citește/scrie cu `fscanf()`/`fprintf()`
- Datele binare se citesc/scriu cu `fread()`/`fwrite()`



> `fprintf()` si `fscanf()` sunt parsere de text ASCII.

> Nu este o idee bună, și nici ușoară, să folosesc `fread()`/`fwrite()` pentru antetul PPM (plain-text).


Argumentele din linia de comandă sunt validate și parsate
folosind funcții din `string.h`.



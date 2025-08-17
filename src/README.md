# Tema 2 SDA

> Autor: Trifan Bogdan-Cristian (312CD)

## Structuri de Date

Fisierul `utils.h` definește structurile esențiale pentru proiect:
- **PPM_Image**: reprezintă imaginea originală în format PPM
- **Coada**: implementată ca listă simplu înlănțuită, folosită pentru parcurgerea arborilor
- **Color**: reține canalele RGB ale unui pixel
- **QuadTree**: arborele cuaternar pentru compresia imaginii, conținând:
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


## 🌳 Construcția arborerului cuaternar pentru compresie

```c
QuadTree *compress_image(PPM_Image *img, int factor);
```

Funcția de compresie împarte succesiv imaginea în câte patru blocuri,
continuând divizarea până când scorul de similitudine
pentru fiecare submatrice este mai mic sau egal cu factorul specificat in linia de comanda.

**Pasii algoritmului**:

1. Se creează radacina arborelui, reprezentând întreaga imagine
2. Radacina se adaugă în coadă
3. Atâta timp cât coada nu este goală:
   - Se extrage primul nod
   - Se calculează scorul de similitudine pentru blocul respectiv
   - Dacă scorul este mai mic sau egal cu factorul, nodul devine frunză și i se alocă memorie pentru `color`
		(se calculează culoarea medie a blocului)
   - Dacă scorul este mai mare decât factorul, nodul se extinde, creându-se cei patru copii, care se adaugă în coadă



Algoritmul de compresie presupune impartirea sucessiva a imagini in cate 4 blocuri,
care se vor diviza din nou si din nou, pana cand scorul similiritatii pentru sub-matricea respectiva
este mai mica sau egala decat factorul primit ca argument in linia de comanda.


Pentru a tine cu usurinta evidenta nodurilor "la care mai e de lucru", am folosit o coada care stocheaza noduri din arbore.

Pornesc cu un arbore care retine un singur nod: radicina -> refera intreaga imagine.
Adaug radacina in coada.
Cat timp coada mai contine elemente, extrag primul elementul si calculez scorul pentru blocul la care refera.
Daca blocul este mai mic sau egal cu factorul, atunci nodul este nod frunza,
altfel, expandez nodul, creandu-i copii, cate unul pentru fiecare cele 4 blocuri,
urmand sa adaug nodurile nou create in coada

In cazul nodurilor frunza, se va aloca memorie pentru campul `color` al nodului,
se va face inca o data apel la functia `avg_block_color()`
care calculeaza culoarea medie a unei sub-matrici din imagini,
iar rezultatul functiei se va copia in nod.





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

Prima cerinta presupune citirea imaginii din fisierul PPM, constructia arborelui de compresie
si mai apoi determinarea urmatoarelor statistici asupra acestuia:
- numarul de niveluri din arborele cuaternar
- numarul frunzelor
- cea mai mare dimensiune a laturii din imagine pe care un nod frunza o retine

Aceste informatii se vor scrie intr-un fisier text.


In cazul in care nivelurile arborelui sunt indexate de la 1, numarul lor este egal cu inaltimea arboorelui,
pe care am calculat-o cu o parcurgere recursive in adancime.


Numarul frunzelor si cea mai mare dimensiune a unui bloc pe care o frunza o retine
se pot afla simultan.
Pentru a rezolva asta, am ales sa mai parcurg inca o data arborele, de data asta pe latime.


## 🗂️ Task 2: generarea fisierului unei imagini PPM comprimate

Similar cu cerinta precedenta, mai intai se incarca in memorie imaginea originala
si se construiteste arborele cuaternar de compresie al acesteia.

'Printarea' arborerului presupune o parcurgere in latime (BFS), care foloseste in spate o coada.
Cat timp coada mai are elemente, extrag un nod. Daca nodul nu este frunza, atunci ii adaug cei 4 copii in coada.



## 📂 Task 3: decompresie


Decompresia (inversul operatiei de compresiei) preusupunea citirea imaginii comprimate
si constructia arborelui cuaternar,
iar ulterior a imaginii originale, in functie de informatiile pe care frunzele arborerului le retin.


Pasul de reconstructie al imaginii presupune o coada, in care sunt adaugate nodurile pe parcurs ce sunt citie.
Folosind o structura de date FIFO, se pastreaza cel mai bine evidenta parintelui nodului care se citeste la un moment dat.
Imaginea se construieste odata cu citirea, fara nicio alta parcurgere.

La final, imaginea reconstruita este scrisa intr-un fisier binar.


## 💾 Input/Output

Atat imaginile PPM, cat si fisierele comprimate sunt fisiere binare.

A fost o adevarata peripetie sa "ghicesc" formatul lor.
Am avut de a face cu o documentatie neclara,
o situatie posibila la viitoare proiecte sau la un loc de munca 👨‍💼💼.

dar noroc ca spune in enunt **tipurile de date** care se folosesc.

Am folosit:
- `fprintf()`/`fscanf()`: pentru scrierea/citirea din/in antetul PPM (partea de text de la inceput de fisier)
- `fread()`/`fwrite()`: pentru scrierea/citirea partii binare a fisierelor

> `fprintf()` si `fscanf()` sunt parsere de text ASCII.

> Nu este o idee buna, si nici usoara, sa folosesc `fread()`/`fwrite()` pentru antetul PPM (care este plain-text).


Argumentele din linia de comanda sunt validate si parsate folosind functii C fin `string.h` pe sisuri de caractere.



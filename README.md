# Quad Tree


Arborele cuaternar este o structură de date care organizează informatii pentru date
multidimensionale, fiind folosită în cartografie, procesarea imaginilor, grafică pe calculator etc.
Structura este un arbore ce reprezintă o zonă din spat, iul N-dimensional (noi vom analiza, în
cadrul acestei teme, cazul N = 2), fiecare nod al arborelui păstrează informat, ie pentru o zonă
din spat, iu, iar nodul are 2N fii, care reprezintă fiecare o zonă de 2N ori mai mică decât zona
părintelui. Zonele fiilor sunt disjuncte, iar reuniunea lor formează zona părintelui. Cu alte
cuvinte, structura pe care o vom utiliza, în cadrul acestei teme, este un arbore în care fiecare
nod neterminal va avea fix 4 descendenti.



## Formule


### Step 1

```math
red = \frac{1}{size \cdot size} \left( \sum_{i=x}^{x+size} \sum_{j=y}^{y+size} grid[i][j].red \right)
```
```math
green = \frac{1}{size \cdot size} \left( \sum_{i=x}^{x+size} \sum_{j=y}^{y+size} grid[i][j].green \right)
```
```math
blue = \frac{1}{size \cdot size} \left( \sum_{i=x}^{x+size} \sum_{j=y}^{y+size} grid[i][j].blue \right)
```


### Step 2

```math
mean = \frac{1}{3 \cdot size^2} 
       \big(  \sum_{i=x}^{x+size} \sum_{j=y}^{y+size} 
       (red - grid[i][j].red)^2 
           + (green - grid[i][j].green)^2 
           + (blue - grid[i][j].blue)^2 \big)
```

## Rulare teste


```sh
cd checker/
chmod +x check.sh
```

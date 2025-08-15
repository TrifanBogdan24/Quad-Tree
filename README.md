# Quad Tree


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

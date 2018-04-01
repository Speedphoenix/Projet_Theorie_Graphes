## Dans le code:
### En général:
- Compiler souvent (<kbd>ctrl</kbd>+<kbd>F9</kbd>). (Pas compile and run <kbd>F9</kbd>)
- Toujours enlever les warnings. Si des warning ignorés s'accumulent, ils peuvent occuper trop de place parmi les messages du compilateur, et cacher les warnings qui pourraient poser problème. 
### Dans les .c
- Bien indenter ce qui est à l’intérieur de crochets, mais pas les crochets eux mêmes  
- Mettre des espaces entre les opérateurs (exception acceptée pour `==` pour pour mieux voir les differences de priorités)  
- Pas d’espaces pour les opérateurs d'accès (`.`, `->`, `[]` etc...)  
- Pas d'espaces avant les virgules, un espace après  
- Mettre des parenthèses quand l'ordre des priorités n'est pas clair  
- Séparer les parenthèses par des espaces quand y'en a beaucoup collées ensemble

Exemple de bon code:  

    if (( (( a*b - d) < (e / b)) && e==d ) || ( g*h > d ))
    {
        a = g * h;
    }
- Pour les trop longues lignes, les séparer après ou avant des opérateurs 

      if ( (quelque chose)  &&
	       (quelque chose d'autre) )
	  {
	      a = quelque chose +
	          autre chose;
	  }
  >'long' est subjectif. Généralement les développeurs disent 80 caractères. Un bon signe d'une ligne trop longue est lorsqu'il faut quelques instants pour retrouver la bonne ligne quand on déplace les yeux de là ou on est en train d'écrire aux caractères à gauche de la ligne  

- Lorsque possible, ne pas faire de trop longues fonctions.
  > - Généralement, au dessus de 50 lignes faut considérer la diviser, ou mettre des bouts en méthode / sous programme.
  > - Si une longue ligne est recopiée beaucoup de fois, considérer la mettre dans une fonction (qui n'aurait qu'un `return (long statement);`)

### Dans les .h  
 Sur la ligne au dessus de chaque  prototype de fonction et chaque attribut, constante (autre que `#define`), struct, classe etc:
Faire un commentaire de documentation (avec `/// au lieu de //`) expliquant son utilité/ce que ça fait  
Les commentaires de documentions permettent après, dans la complétion de code d'avoir la description avec.  
> Pas besoin de le faire en permanence, mais à appliquer lors d'un clean général, ou quand on décide d'ajouter les commentaires.  

### Les warnings

git commands

## Dans le développement d'un projet
stand up
trello - backlog
doxygen


## Commandes Git à retenir

<!--stackedit_data:
eyJoaXN0b3J5IjpbLTEyOTg0NjA2MDBdfQ==
-->
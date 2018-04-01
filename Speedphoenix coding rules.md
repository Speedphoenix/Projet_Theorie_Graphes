
## Dans le développement d'un projet
### Stand-up
Pour essayer de suivre une approche de développement de projet plus efficace (pris des méthodes agiles):  
Tous les matins, chacun énonce tout ce qu'il a fait la veille, et ce qu'il compte faire durant la journée.

### Todo list
Un service comme Trello est un bon moyen de suivre la liste des choses, en ayant l'importance des taches, l'état d'avancement de chaque tache, qui s'en occupe etc

### Quand on approche des moments de coupure:
Par exemple avant le déjeuner, ou le soir
- Faire des commits de plus en plus fréquents
- Commenter de manière abondante, en particulier sur le fils des idées, tous les éléments ajoutés mais pas encore forcément utilisés...
- Si une fonctionnalité est finie peu avant le moment de coupure, mais qu'il reste un peu de temps, se lancer sur une fonctionnalité backlog (et la mettre en pause après la coupure)

## Commandes Git à retenir
### En local
Pour les conflits, et les merge de différentes "grosses" branches, je conseille de le faire graphiquement depuis github.com
|commande|descriptions|exemples|
|---|---|---|
|`git add`|Ajoute tous les changements des fichiers en paramètre au prochain commit|`git add ./* pour tout ajouter`|
|`git commit -m "title" -m "message"`|Fait un commit des changements||
|`git status`|Liste les fichiers modifiés etc||
|`git checkout -b [une branche]`|pour créer et se mettre dans une branche||
|`git checkout [une branche]`|Pour se mettre dans une branche||
|`git branch`|Liste les branches||
|`git merge [une branche]`|Fusionne une branche avec la branche active||
|`git diff`|Voir tous les conflits||

### Entre local et server

|commande|descriptions|exemples|
|---|---|---|
|`git fetch`|Télécharge tous les changements depuis le server (mais ne les applique pas)||
|`git pull`|Télécharge et fusionne tous les changement||
|`git push origin [une branche]`|Envoie les changements d'une certaine branche à la repo distante||
|`git push --all origin`|Envoie tous les changements à la répo distante||

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
> Pas besoin de le faire en permanence, mais à appliquer lors d'un clean général, ou quand on décide d'ajouter les commentaires.  

Les commentaires de documentions permettent après, dans la complétion de code d'avoir la description avec.  
> sont aussi acceptables les autres styles (que `///`) de doc pour Doxygen  

#### Plus généralement, utiliser [la documentation de Doxygen](https://sourceforge.net/projects/doxygen/)  
<!--stackedit_data:
eyJoaXN0b3J5IjpbODQxMjg0NDNdfQ==
-->
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTA4MTQ5NjE4MV19
-->
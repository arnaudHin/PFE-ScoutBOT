# Quelques commandes git

Un repo = un dossier → Ici on a 3 repos : doc, c, android

Pour mettre à jour son repos (avoir les dernières modifications) :

```bash
git pull
```

Une fois des fichiers modifiés on regarde quels fichiers sont ajoutés, modifiés ou supprimés par rapport au repo : 

```bash
git status
```

Une fois qu'on sait ce qu'il y a à ajouter dans le commit on ajoute les fichiers au commit :

```bash
git add <ton fichier (copier coller de la ligne du status (le chemin))>
```

Si c'est un fichier à supprimer : 

```bash
git rm <le fichier>
```

Pour commit : 

```bash
git commit -m "Mon message de commit"
```

Pour pousser sur le repo : 

```bash
git push
```
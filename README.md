## Construire l'image docker (à faire qu'une fois)
```console
$ docker build -t evolueur:1.0 .
```

## Lancer un serveur Redis (avec docker par ex.)
```console
$ docker run -p 6379:6379 -d redis:alpine3.13
```

## Lancer l'appli evolueur
```console
$ docker run --net="host" evolueur:1.0
```

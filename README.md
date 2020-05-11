# tp-2020-1c-Omnidata
<html lang="en">
  
<img src="Pokemon!.jpg">
<b> Espero que no pienses sólo en el bienestar de tus Pokémon... Confío en que te preocupes por todos los Pokémon que hay... </b> <br>
<i>-Señor Fuji<i>
  
 
<body>
  <h3> <b> ¿Cómo trabajamos con branches? </b> </h3>

<ul>
  <li>Primero nos paramos en la carpeta de nuestro TP (Obvio, no?) </li>
  <li>Despues tipeamos <br> <b> git checkout -b nombre-branch </b> si el branch no está creado, y <b>git checkout nombre-branch</b> en caso contrario </li>
  <li>Una vez acá, hacemos todos los <b> git add </b> y <b> git commit </b> que necesitemos </li>
  <li>Si ya estamos listos para subir nuestro avance, pasamos al siguiente paso </li>
  <li><b>git push --set-upstream origin nombre-branch</b> </li>
  <li>Si venimos a Git hub, vamos a ver una pestaña branches, donde están listados los distintos branch. Si buscamos el nuestro, nos va a   aparecer un botoncito de "New pull-request". </li>
  <li>Le damos a <b>"New pull-request"</b> y escribimos una pequeña descripción. </li>
  <li>Una vez creado, git se va a fijar que no haya conflictos y nos va a permitir hacer merge.</li>
  <li>Apretamos <b>Merge pull request</b> cuando queramos cruzar los branches y listo</li>
</ul>

<h3> Quizas alguno todavía no se lleva con git, así que hago pequeña guía </h3>
<ul>
  <li>Antes de empezar a trabajar, cada día que se levantan, <b>siempre</b> hagan <b>git pull</b> para traerse los últimos cambios</li>
  <li>Pueden elegir un branch como expliqué más arriba o trabajar en master. Intentemos trabajar en un proyecto a la vez para no generar conflictos.</li>
  <li>Luego de efectuar cambios en un archivo, cada pocos minutos recomiendo hacer un <b>git status</b> que les marca en rojo las cosas que hay diferentes desde el último <b>git pull</b>. También pueden hacer <b>git diff</b> para ver detallada la diferencia que existe.</li>
  <li>Pueden copiar la ruta de los archivos que se modificaron, y hacer <b>git add ruta-archivo</b>, o si están seguros de que todo lo que  hicieron lo quieren conservar <b>git add .</b> (git add puntito) </li>
  <li>Si hacen <b>git status</b> nuevamente van a ver en verde los cambios que agregaron.</li>
  <li>Cada que hagan algun cambio pequeño está bueno que hagan <b>git commit -m "Mensaje descriptivo"</b> para ir separando los cambios en <b>pequeños commits</b> y no todo de una.</li>
  <li>Cuando quieren subir sus cambios pueden usar lo que está mas arriba si van a usar un branch, o directamente <b>git push</b> si están en master. Siempre antes de hacer esto tienen que asegurarse de haber hecho todos los <b>git add</b> y <b>git push</b> necesarios.</li>
  <li>Si saben que alguien más está trabajando en el TP, hagan <b>git push</b> y <b>git pull</b> cada pocos minutos para estar en simultaneo con los cambios que se van haciendo </li>
 </ul>

</body>
</html>

## Ayuda de memoria para la consola

  ```
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/workspace/tp-2020-1c-Omnidata/MiLibreria/Debug 
 ```
 En eclipse hay que agregar en enviroments
 ```
 LD_LIBRARY_PATH

${workspace_loc}/tp-2020-1c-Omnidata/MiLibreria/Debug
```
 ## Markdown cheatsheet para escribir este archivo
[Clik aqui](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)

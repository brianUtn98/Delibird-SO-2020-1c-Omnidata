# tp-2020-1c-Omnidata

* Sofía Enriquez
* Marcos Villalba
* Damian Nuñez
* Pedro Bruniard
* Brian Monroy
<html lang="en">
  
<img src="delibird.png">
<b> Espero que no pienses sólo en el bienestar de tus Pokémon... Confío en que te preocupes por todos los Pokémon que hay... </b> <br>
<i>-Señor Fuji</i>
  
 [**Enunciado**](https://docs.google.com/document/d/1be91Gn93O2Vp8frZoV1i5CmtOG0scE1PS8dMHsCP314/edit)
 
 Contexto: El TP posee 4 módulos.


### Gamecard (Sofi y Dami)
Este es el filesystem del TP. Se encarga de almacenar en él los nuevos Pokemons que van apareciendo a partir del mensaje NEW. Cuando un nuevo pokemon es agregado al filesystem le avisa al proceso Broker a través de un mensaje APPEARED. 
Además, es su responsabilidad administrar los pedidos de pokemons al recibir un mensaje GET, que será respondido con un mensaje LOCALIZED que contara con la lista de coordenadas en las que se encuentra el Pokemon requerido. También informa si un pokemon se puede atrapar a través de la recepción del mensaje CATCH que responde con un CAUGHT para avisar si este está disponible o no, restando una cantidad al pokemon el la posición en caso de poder atraparse.
Temas evaluados: FileSystem.

### Broker (Peter y Marcos)
Este es el administrador de colas de mensajes del TP. Se encarga de mantener 6 colas de mensajes activas para que los demás procesos se suscriban, además de recibir mensajes de todos los módulos. Todos los mensajes son cacheados en una memoria caché virtual. Al recibir un mensaje, reenvía ese mensaje a todos los suscriptores de la cola correspondiente. La memoria caché no es infinita, así que es planificada mediante los algoritmos de reemplazo de página LRU y FIFO. También posee dos esquemas para manejo de memoria, que son Particiones dinámicas y Buddy system. Este proceso además mantiene un registro de los procesos suscriptos, y en caso de uno desconectarse, al reconectarse debería fijarse si es un suscriptor viejo o nuevo, para enviarle todos los mensajes cacheados que no tenga.
Temas evaluados: Memoria, productor consumidor.

### Team (Brian)
Este es el planificador del TP. Se encarga de planificar entrenadores en un mapa de 2 coordenadas para atrapar pokemons. Cada entrenador tiene una lista de Pokemons que necesita atrapar, y el TEAM termina cuando todos sus entrenadores poseen todos los pokemons que necesitan. Envía un mensaje GET por cada especie de pokemon que necesite y espera por la llegada de mensajes APPEARED y LOCALIZED que indiquen una apareción de Pokemon en el mapa, para planificar sus entrenadores a atrapar.
Tiene un planificador de largo plazo que se maneja por cercanía (se cambia a READY al entrenador más cercano a un pokemon que desee atrapar sin importar si lo necesita o no). También tiene un planificador de corto plazo que cambia los entrenadores a EXEC según algoritmos FIFO,RR,SJF y SJFCD.
Un entrenador puede atrapar tantos pokemons como necesite (no más). Si un entrenador ya no puede atrapar y además posee pokemons que no necesita, se lo considera en deadlock, en este caso se procede a detectar los deadlocks y solucionarlos haciendo que los entrenadores intercambien.
Temas evaluados: Planificación, sincronización y procesos.

### Gameboy (Marcos y Brian)
Este es el proceso publisher del TP. Tiene permiso de enviar mensaje a todos los módulos, no tiene mucha complejidad de por sí pero es vital para realizar las pruebas del sistema.

# tp-2020-1c-Omnidata

* Sofía Enriquez
* Marcos Villalba
* Damian Nuñez
* Pedro Bruniard
* Brian Monroy
<html lang="en">
  
<img src="delibird.png">
<b> Espero que no pienses sólo en el bienestar de tus Pokémon... Confío en que te preocupes por todos los Pokémon que hay... </b> <br>
<i>-Señor Fuji<i>
  
 [**Enunciado**](https://docs.google.com/document/d/1be91Gn93O2Vp8frZoV1i5CmtOG0scE1PS8dMHsCP314/edit)
 
 Contexto: El TP posee 4 módulos.


##Gamecard (Sofi y Dami)
Este es el filesystem del TP. Se encarga de almacenar en él los nuevos Pokemons que van apareciendo a partir del mensaje NEW. Cuando un nuevo pokemon es agregado al filesystem le avisa al proceso Broker a través de un mensaje APPEARED. 
Además, es su responsabilidad administrar los pedidos de pokemons al recibir un mensaje GET, que será respondido con un mensaje LOCALIZED que contara con la lista de coordenadas en las que se encuentra el 

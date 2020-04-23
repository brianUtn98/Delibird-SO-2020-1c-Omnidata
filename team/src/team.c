#include "team.h"

//int main(void) {
//	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
//	t_TEAMConfig *config = cargarConfigTeam();
//	//crear_conexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER);

int main(void) {
	//t_config *config = config_create("./tp_fix.config");
	char **read_array = teamConf->POKEMON_ENTRENADOR;//config_get_array_value(config, "POKEMON_ENTRENADORES");
	t_list *pokemongos = list_create();
	void _a_la_lista(char *poke) {
		if (poke != NULL) {
			list_add(pokemongos, poke);
		}
	}
	void _imprimir(char *string) {
		if (string != NULL) {
			char **pokes = string_split(string, "|");
			string_iterate_lines(pokes, _a_la_lista);
			printf("Read: %s\n", string);
		} else {
			printf("Got NULL\n");
		}
	}
	string_iterate_lines(read_array, _imprimir);

	void mostrar(void *elemento) {
		printf("El elemento: %s\n", (char *) elemento);
	}

	list_iterate(pokemongos, mostrar);

	// Atencion: aca les falta liberar la memoria que haga falta
	// Es cuarentena; no Navidad

	return EXIT_SUCCESS;
}

sin_color=\x1b[0m
verde=\x1b[32;01m
amarillo=\x1b[33;01m
negrita := $(shell tput bold)
sin_negrita := $(shell tput sgr0)

all:
	mkdir -p logs

    gcc  -g -w src/broker.c  src/funcionesBroker.c sockets/servidor.c   -lcommons -lpthread
	@printf '$(negrita)$(amarillo)MUSE$(sin_color) ..... $(verde)ok!$(sin_color)$(sin_negrita)\n'

    gcc  -g -w src/gameBoy.c  src/funcionesGameBoy.c sockets/servidor.c   -lcommons -lpthread
	@printf '$(negrita)$(amarillo)MUSE$(sin_color) ..... $(verde)ok!$(sin_color)$(sin_negrita)\n'

    gcc  -g -w src/gameCard.c  src/funcionesGameCard.c sockets/servidor.c   -lcommons -lpthread
	@printf '$(negrita)$(amarillo)SUSE$(sin_color) ..... $(verde)ok!$(sin_color)$(sin_negrita)\n'

    gcc  -g -w src/team.c  src/funcionesTeam.c sockets/servidor.c   -lcommons -lpthread
	@printf '$(negrita)$(amarillo)SAC$(sin_color) ..... $(verde)ok!$(sin_color)$(sin_negrita)\n'

*/
# Clean
clean:
	rm -f broker *.o
	rm logs/BROKER.log
	rm -f gameBoy *.o
	rm logs/GAMEBOY.log
	rm -f gameCard *.o
	rm logs/GAMECARD.log
	rm -f team *.o
	rm logs/TEAM.log

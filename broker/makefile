-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include sockets/subdir.mk
-include subdir.mk
-include objects.mk






sin_color=\x1b[0m
verde=\x1b[32;01m
amarillo=\x1b[33;01m
negrita := $(shell tput bold)
sin_negrita := $(shell tput sgr0)

all:
	
    gcc  -g -w src/broker.c  src/funcionesBroker.c sockets/servidor.c   -lcommons -lpthread
	@printf '$(negrita)$(amarillo)MUSE$(sin_color) ..... $(verde)ok!$(sin_color)$(sin_negrita)\n'

 


# Clean
clean:
	rm -f broker *.o
	rm logs/BROKER.log
	

#Autor: Alberto Hamilton Castro
#Fichero Makefile para compilar las distintas pruebas
# preparadas para verificar el correcto funcionamiento
# del ejercicio
#
# basta con ejecutar en línea de comandos
#  make  ->  para compilar las pruebas básica
#  make prueba##  -> para obtener ese ejecutable concreto (no poner cpp)
#  make clean -> borrar ficheros generados y dejar solo los fuentes
#  make exe## -> trata de compilar y ejecutar la prueba##
#  make exe -> trata de compilar y ejecutar todas las pruebas
#  make fallidas -> ídem que la anterior pero sólo muestra resultado
#      de las fallidas


GPP=g++ -Wall --std=c++14

pruebas:=$(wildcard prueba[0-8]?.cpp)

ejecutables:=$(wildcard prueba*.cpp)


clases:=$(wildcard [A-Z]*.cpp)

ficheroZip=FicherosP2.zip

#Evitamos que se borren los fichero intermedios (%.o)
.SECONDARY :

all : $(pruebas:.cpp=)

info :
	@echo "\npruebas: $(pruebas)"
	@echo "\nejecutables: $(ejecutables)"
	@echo "\nclases: $(clases)"


depend : .dependC .dependP .dependE

.dependC: $(clases) Makefile
	@rm -f ./.dependC
	@$(GPP)  -MM $(clases) > ./.dependC 2>/dev/null

.dependP: $(ejecutables) $(clases) Makefile
	@rm -f ./.dependP
	@$(GPP)  -MM $(ejecutables) > ./.dependP 2>/dev/null

.dependE: .dependP
	@rm -f ./.dependE
	@cat ./.dependP | sed -e's/\.o//' -e's/\..pp/\.o/g' > ./.dependE


-include .dependC
-include .dependP
-include .dependE

%.o : %.cpp
	-rm -f $@  $(@:.o=)
	$(GPP) -c -o $@ $<

prueba% : prueba%.o
	$(GPP) -lrt -pthread -g -o $@ $^

arrancaServer: arrancaServer.o SocketServer.o ModbusRTU.o Mensaje.o
	$(GPP) -lrt -lpthread -g -o $@ $^

CalculaCRC: CalculaCRC.cpp
	$(GPP) -g -o $@ $<

#reglas para borrar los fichero generados
# y dejar sólo los fuente
clean :
	rm -f *.o $(ejecutables:.cpp=) .depend* *.hpp.gch resumen.log core ejecucion.log

#Generación de zip
zip : $(ficheroZip)

$(ficheroZip) : Makefile $(pruebas)
	-rm $@
	zip $@ $^

#Reglas para ejecutar los distintos grupos de pruebas

exe% : prueba%
	@ulimit -v 60000; \
	ulimit -t 6; \
	./$<

exe :  $(sort $(pruebas:.cpp=))
	@for fa in $^; \
	do echo "\n$$(tput setaf 2)==== $$fa ===========================$$(tput setaf 9)"; \
	ulimit -v 60000; ulimit -t 6; \
	./$$fa;done

fallidas :   $(sort $(pruebas:.cpp=))
	@for fa in $^; \
	do ulimit -v 60000; ulimit -t 6; \
	if ./$$fa >/dev/null 2>&1; \
	then echo "==== "$$fa CORRECTA""; \
	else echo "\n$$(tput setaf 2)==== $$fa ===========================$$(tput setaf 9)"; \
	./$$fa; fi; done


fallan :
	-make clean
	make fail

fail:
	-make -k all 2>/dev/null
	@echo -e "\n\n==========================="
	@for fa in $(sort $(pruebas:.cpp=)); \
	do titulo=$$(grep 'define TITULO' $$fa.cpp | sed -e's/^#define TITULO //'); \
	ulimit -v 60000; ulimit -t 6; \
	if [ ! -x ./$$fa ] ; \
	then echo "$$(tput setaf 1)==== $$fa NO EXISTE  $$titulo $$(tput setaf 9)"; \
	elif { ./$$fa; } >ejecucion.log 2>&1; \
	then echo "$$(tput setaf 2)==== $$fa CORRECTA   $$titulo $$(tput setaf 9)"; \
	else \
	   if grep -w 'core' ejecucion.log ; \
	   then \
	      PORCEN="(core)" ;\
	   else \
	      PORCEN=$$(cat ejecucion.log | grep % | sed -e's/^.*pruebas //'); \
	   fi; \
	   echo "$$(tput setaf 5)==== $$fa INCORRECTA $$PORCEN $$titulo $$(tput setaf 9)"; \
	fi; done
	-rm -f ejecucion.log


resumen.log: $(clases) $(clases:.cpp=.hpp) Makefile
	-rm resumen.log
	-make -k all 2>/dev/null
	@for fa in $(sort $(pruebas:.cpp=)); \
	do \
	  ulimit -v 60000; ulimit -t 6; \
	  echo -n $$(echo $$fa | sed -e's/prueba//'):  >> resumen.log;\
	  if [ ! -x ./$$fa ] ; \
	  then echo "000" >> resumen.log; \
	  elif { ./$$fa; } >ejecucion.log 2>&1; \
	  then echo "+++" >> resumen.log; \
	  else \
	    if egrep -w '(core|Segmentation)' ejecucion.log ; \
	    then \
	      PORCEN="core" ;\
	    else \
	      PORCEN=$$(cat ejecucion.log | grep % | sed -e's/^.*pruebas //'); \
	    fi; \
	  echo "$$PORCEN" >> resumen.log; \
	  fi;\
	done
	rm -f ejecucion.log

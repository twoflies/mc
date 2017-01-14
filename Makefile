CC=g++
CFLAGS=-g -Wall

ODIR=obj

LIBS=-lncurses -lpanel

_OBJ = locale.o util.o event.o commandinterpreter.o plugin.o plugincommandinterpreter.o security/node.o security/sensornode.o security/securitypluginwindow.o security/securitycommandinterpreter.o security/securityplugin.o mc.o console/window.o console/pluginwindow.o console/shellwindow.o console/mcconsole.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cc %.h
	$(CC) -c -o $@ $< $(CFLAGS)

mc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/security/*.o $(ODIR)/console/*.o *~ security/*~ console/*~ core

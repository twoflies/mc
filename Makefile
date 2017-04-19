CC=g++
CFLAGS=-g -Wall -I lib/include -Llib -Wl,-rpath,lib

ODIR=obj

LIBS=-lncurses -lpanel -lxbmanager -lxbserial

_OBJ = locale.o util.o logger.o event.o outputwriter.o commandinterpreter.o plugin.o plugincommandinterpreter.o security/securitypluginwindow.o security/securitycommandinterpreter.o security/securityplugin.o mc.o console/window.o console/pluginwindow.o console/shellwindow.o console/mcconsole.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cc %.h
	$(CC) -c -o $@ $< $(CFLAGS)

mc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/security/*.o $(ODIR)/console/*.o *~ security/*~ console/*~ core

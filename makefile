project=comx
objects= Kuroko.o \
	main.o \
	photo.o PIDAndPicture.o \
	ReceiveData.o SetAT.o common.o HT.o
CFLAGS = -O2
$(project):$(objects)
	gcc -o $@ $^
.c.o:
	gcc -c $< $(CFLAGS)
Kuroko.o:Kuroko.h
main.o:Kuroko.h App.h
photo.o:Picture.h
PIDAndPicture.o:App.h Picture.h
ReceiveData.o:Kuroko.h App.h
SetAT.o:Kuroko.h App.h
common.o:Kuroko.h App.h
HT.o:Kuroko.h App.h
clean:
	rm -rf $(objects)